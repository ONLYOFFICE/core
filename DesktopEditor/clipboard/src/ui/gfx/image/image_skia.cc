// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image_skia.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "ui/gfx/image/image_skia_operations.h"
#include "ui/gfx/image/image_skia_source.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/gfx/skia_util.h"

namespace gfx {
namespace {

// static
gfx::ImageSkiaRep& NullImageRep() {
  CR_DEFINE_STATIC_LOCAL(ImageSkiaRep, null_image_rep, ());
  return null_image_rep;
}

}  // namespace

namespace internal {
namespace {

class Matcher {
 public:
  explicit Matcher(ui::ScaleFactor scale_factor) : scale_factor_(scale_factor) {
  }

  bool operator()(const ImageSkiaRep& rep) const {
    return rep.scale_factor() == scale_factor_;
  }

 private:
  ui::ScaleFactor scale_factor_;
};

}  // namespace

// A helper class such that ImageSkia can be cheaply copied. ImageSkia holds a
// refptr instance of ImageSkiaStorage, which in turn holds all of ImageSkia's
// information. Having both |base::RefCountedThreadSafe| and
// |base::NonThreadSafe| may sounds strange but necessary to turn
// the 'thread-non-safe modifiable ImageSkiaStorage' into
// the 'thread-safe read-only ImageSkiaStorage'.
class ImageSkiaStorage : public base::RefCountedThreadSafe<ImageSkiaStorage>,
                         public base::NonThreadSafe {
 public:
  ImageSkiaStorage(ImageSkiaSource* source, const gfx::Size& size)
      : source_(source),
        size_(size),
        read_only_(false) {
  }

  ImageSkiaStorage(ImageSkiaSource* source, ui::ScaleFactor scale_factor)
      : source_(source),
        read_only_(false) {
    ImageSkia::ImageSkiaReps::iterator it =
        FindRepresentation(scale_factor, true);
    if (it == image_reps_.end() || it->is_null())
      source_.reset();
    else
      size_.SetSize(it->GetWidth(), it->GetHeight());
  }

  bool has_source() const { return source_.get() != NULL; }

  std::vector<gfx::ImageSkiaRep>& image_reps() { return image_reps_; }

  const gfx::Size& size() const { return size_; }

  bool read_only() const { return read_only_; }

  void DeleteSource() {
    source_.reset();
  }

  void SetReadOnly() {
    read_only_ = true;
  }

  void DetachFromThread() {
    base::NonThreadSafe::DetachFromThread();
  }

  // Checks if the current thread can safely modify the storage.
  bool CanModify() const {
    return !read_only_ && CalledOnValidThread();
  }

  // Checks if the current thread can safely read the storage.
  bool CanRead() const {
    return (read_only_ && !source_.get()) || CalledOnValidThread();
  }

  // Returns the iterator of the image rep whose density best matches
  // |scale_factor|. If the image for the |scale_factor| doesn't exist
  // in the storage and |storage| is set, it fetches new image by calling
  // |ImageSkiaSource::GetImageForScale|. If the source returns the
  // image with different scale factor (if the image doesn't exist in
  // resource, for example), it will fallback to closest image rep.
  std::vector<ImageSkiaRep>::iterator FindRepresentation(
      ui::ScaleFactor scale_factor, bool fetch_new_image) const {
    ImageSkiaStorage* non_const = const_cast<ImageSkiaStorage*>(this);

    float scale = ui::GetScaleFactorScale(scale_factor);
    ImageSkia::ImageSkiaReps::iterator closest_iter =
        non_const->image_reps().end();
    ImageSkia::ImageSkiaReps::iterator exact_iter =
        non_const->image_reps().end();
    float smallest_diff = std::numeric_limits<float>::max();
    for (ImageSkia::ImageSkiaReps::iterator it =
             non_const->image_reps().begin();
         it < image_reps_.end(); ++it) {
      if (it->GetScale() == scale) {
        // found exact match
        fetch_new_image = false;
        if (it->is_null())
          continue;
        exact_iter = it;
        break;
      }
      float diff = std::abs(it->GetScale() - scale);
      if (diff < smallest_diff && !it->is_null()) {
        closest_iter = it;
        smallest_diff = diff;
      }
    }

    if (fetch_new_image && source_.get()) {
      DCHECK(CalledOnValidThread()) <<
          "An ImageSkia with the source must be accessed by the same thread.";

      ImageSkiaRep image = source_->GetImageForScale(scale_factor);

      // If the source returned the new image, store it.
      if (!image.is_null() &&
          std::find_if(image_reps_.begin(), image_reps_.end(),
                       Matcher(image.scale_factor())) == image_reps_.end()) {
        non_const->image_reps().push_back(image);
      }

      // If the result image's scale factor isn't same as the expected
      // scale factor, create null ImageSkiaRep with the |scale_factor|
      // so that the next lookup will fallback to the closest scale.
      if (image.is_null() || image.scale_factor() != scale_factor) {
        non_const->image_reps().push_back(
            ImageSkiaRep(SkBitmap(), scale_factor));
      }

      // image_reps_ must have the exact much now, so find again.
      return FindRepresentation(scale_factor, false);
    }
    return exact_iter != image_reps_.end() ? exact_iter : closest_iter;
  }

 private:
  virtual ~ImageSkiaStorage() {
    // We only care if the storage is modified by the same thread.
    // Don't blow up even if someone else deleted the ImageSkia.
    DetachFromThread();
  }

  // Vector of bitmaps and their associated scale factor.
  std::vector<gfx::ImageSkiaRep> image_reps_;

  scoped_ptr<ImageSkiaSource> source_;

  // Size of the image in DIP.
  gfx::Size size_;

  bool read_only_;

  friend class base::RefCountedThreadSafe<ImageSkiaStorage>;
};

}  // internal

ImageSkia::ImageSkia() : storage_(NULL) {
}

ImageSkia::ImageSkia(ImageSkiaSource* source, const gfx::Size& size)
    : storage_(new internal::ImageSkiaStorage(source, size)) {
  DCHECK(source);
  // No other thread has reference to this, so it's safe to detach the thread.
  DetachStorageFromThread();
}

ImageSkia::ImageSkia(ImageSkiaSource* source, ui::ScaleFactor scale_factor)
    : storage_(new internal::ImageSkiaStorage(source, scale_factor)) {
  DCHECK(source);
  if (!storage_->has_source())
    storage_ = NULL;
  // No other thread has reference to this, so it's safe to detach the thread.
  DetachStorageFromThread();
}

ImageSkia::ImageSkia(const ImageSkiaRep& image_rep) {
  Init(image_rep);
  // No other thread has reference to this, so it's safe to detach the thread.
  DetachStorageFromThread();
}

ImageSkia::ImageSkia(const ImageSkia& other) : storage_(other.storage_) {
}

ImageSkia& ImageSkia::operator=(const ImageSkia& other) {
  storage_ = other.storage_;
  return *this;
}

ImageSkia::~ImageSkia() {
}

// static
ImageSkia ImageSkia::CreateFrom1xBitmap(const SkBitmap& bitmap) {
  return ImageSkia(ImageSkiaRep(bitmap, ui::SCALE_FACTOR_100P));
}

scoped_ptr<ImageSkia> ImageSkia::DeepCopy() const {
  ImageSkia* copy = new ImageSkia;
  if (isNull())
    return scoped_ptr<ImageSkia>(copy);

  CHECK(CanRead());

  std::vector<gfx::ImageSkiaRep>& reps = storage_->image_reps();
  for (std::vector<gfx::ImageSkiaRep>::iterator iter = reps.begin();
       iter != reps.end(); ++iter) {
    copy->AddRepresentation(*iter);
  }
  // The copy has its own storage. Detach the copy from the current
  // thread so that other thread can use this.
  if (!copy->isNull())
    copy->storage_->DetachFromThread();
  return scoped_ptr<ImageSkia>(copy);
}

bool ImageSkia::BackedBySameObjectAs(const gfx::ImageSkia& other) const {
  return storage_.get() == other.storage_.get();
}

void ImageSkia::AddRepresentation(const ImageSkiaRep& image_rep) {
  DCHECK(!image_rep.is_null());

  // TODO(oshima): This method should be called |SetRepresentation|
  // and replace the existing rep if there is already one with the
  // same scale factor so that we can guarantee that a ImageSkia
  // instance contians only one image rep per scale factor. This is
  // not possible now as ImageLoader currently stores need
  // this feature, but this needs to be fixed.
  if (isNull()) {
    Init(image_rep);
  } else {
    CHECK(CanModify());
    storage_->image_reps().push_back(image_rep);
  }
}

void ImageSkia::RemoveRepresentation(ui::ScaleFactor scale_factor) {
  if (isNull())
    return;
  CHECK(CanModify());

  ImageSkiaReps& image_reps = storage_->image_reps();
  ImageSkiaReps::iterator it =
      storage_->FindRepresentation(scale_factor, false);
  if (it != image_reps.end() && it->scale_factor() == scale_factor)
    image_reps.erase(it);
}

bool ImageSkia::HasRepresentation(ui::ScaleFactor scale_factor) const {
  if (isNull())
    return false;
  CHECK(CanRead());

  ImageSkiaReps::iterator it =
      storage_->FindRepresentation(scale_factor, false);
  return (it != storage_->image_reps().end() &&
          it->scale_factor() == scale_factor);
}

const ImageSkiaRep& ImageSkia::GetRepresentation(
    ui::ScaleFactor scale_factor) const {
  if (isNull())
    return NullImageRep();

  CHECK(CanRead());

  ImageSkiaReps::iterator it = storage_->FindRepresentation(scale_factor, true);
  if (it == storage_->image_reps().end())
    return NullImageRep();

  return *it;
}

void ImageSkia::SetReadOnly() {
  CHECK(storage_);
  storage_->SetReadOnly();
  DetachStorageFromThread();
}

void ImageSkia::MakeThreadSafe() {
  CHECK(storage_);
  EnsureRepsForSupportedScaleFactors();
  // Delete source as we no longer needs it.
  if (storage_)
    storage_->DeleteSource();
  storage_->SetReadOnly();
  CHECK(IsThreadSafe());
}

bool ImageSkia::IsThreadSafe() const {
  return !storage_ || (storage_->read_only() && !storage_->has_source());
}

int ImageSkia::width() const {
  return isNull() ? 0 : storage_->size().width();
}

gfx::Size ImageSkia::size() const {
  return gfx::Size(width(), height());
}

int ImageSkia::height() const {
  return isNull() ? 0 : storage_->size().height();
}

std::vector<ImageSkiaRep> ImageSkia::image_reps() const {
  if (isNull())
    return std::vector<ImageSkiaRep>();

  CHECK(CanRead());

  ImageSkiaReps internal_image_reps = storage_->image_reps();
  // Create list of image reps to return, skipping null image reps which were
  // added for caching purposes only.
  ImageSkiaReps image_reps;
  for (ImageSkiaReps::iterator it = internal_image_reps.begin();
       it != internal_image_reps.end(); ++it) {
    if (!it->is_null())
      image_reps.push_back(*it);
  }

  return image_reps;
}

void ImageSkia::EnsureRepsForSupportedScaleFactors() const {
  // Don't check ReadOnly because the source may generate images
  // even for read only ImageSkia. Concurrent access will be protected
  // by |DCHECK(CalledOnValidThread())| in FindRepresentation.
  if (storage_ && storage_->has_source()) {
    std::vector<ui::ScaleFactor> supported_scale_factors =
        ui::GetSupportedScaleFactors();
    for (size_t i = 0; i < supported_scale_factors.size(); ++i)
      storage_->FindRepresentation(supported_scale_factors[i], true);
  }
}

void ImageSkia::Init(const ImageSkiaRep& image_rep) {
  // TODO(pkotwicz): The image should be null whenever image rep is null.
  if (image_rep.sk_bitmap().empty()) {
    storage_ = NULL;
    return;
  }
  storage_ = new internal::ImageSkiaStorage(
      NULL, gfx::Size(image_rep.GetWidth(), image_rep.GetHeight()));
  storage_->image_reps().push_back(image_rep);
}

SkBitmap& ImageSkia::GetBitmap() const {
  if (isNull()) {
    // Callers expect a ImageSkiaRep even if it is |isNull()|.
    // TODO(pkotwicz): Fix this.
    return NullImageRep().mutable_sk_bitmap();
  }

  // TODO(oshima): This made a few tests flaky on Windows.
  // Fix the root cause and re-enable this. crbug.com/145623.
#if !defined(OS_WIN)
  CHECK(CanRead());
#endif

  ImageSkiaReps::iterator it =
      storage_->FindRepresentation(ui::SCALE_FACTOR_100P, true);
  if (it != storage_->image_reps().end())
    return it->mutable_sk_bitmap();
  return NullImageRep().mutable_sk_bitmap();
}

bool ImageSkia::CanRead() const {
  return !storage_ || storage_->CanRead();
}

bool ImageSkia::CanModify() const {
  return !storage_ || storage_->CanModify();
}

void ImageSkia::DetachStorageFromThread() {
  if (storage_)
    storage_->DetachFromThread();
}

}  // namespace gfx

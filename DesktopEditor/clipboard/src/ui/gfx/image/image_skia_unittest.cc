// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image_skia.h"

#include "base/logging.h"
#include "base/threading/simple_thread.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/layout.h"
#include "ui/gfx/image/image_skia_rep.h"
#include "ui/gfx/image/image_skia_source.h"
#include "ui/gfx/size.h"

// Duplicated from base/threading/non_thread_safe.h so that we can be
// good citizens there and undef the macro.
#if (!defined(NDEBUG) || defined(DCHECK_ALWAYS_ON))
#define ENABLE_NON_THREAD_SAFE 1
#else
#define ENABLE_NON_THREAD_SAFE 0
#endif

namespace gfx {

namespace {

class FixedSource : public ImageSkiaSource {
 public:
  FixedSource(const ImageSkiaRep& image) : image_(image) {}

  virtual ~FixedSource() {
  }

  virtual ImageSkiaRep GetImageForScale(ui::ScaleFactor scale_factor) OVERRIDE {
    return image_;
  }

 private:
  ImageSkiaRep image_;

  DISALLOW_COPY_AND_ASSIGN(FixedSource);
};

class DynamicSource : public ImageSkiaSource {
 public:
  DynamicSource(const gfx::Size& size) : size_(size) {}

  virtual ~DynamicSource() {
  }

  virtual ImageSkiaRep GetImageForScale(ui::ScaleFactor scale_factor) OVERRIDE {
    return gfx::ImageSkiaRep(size_, scale_factor);
  }

 private:
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(DynamicSource);
};

class NullSource: public ImageSkiaSource {
 public:
  NullSource() {
  }

  virtual ~NullSource() {
  }

  virtual ImageSkiaRep GetImageForScale(ui::ScaleFactor scale_factor) OVERRIDE {
    return gfx::ImageSkiaRep();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(NullSource);
};

}  // namespace

namespace test {
class TestOnThread : public base::SimpleThread {
 public:
  explicit TestOnThread(ImageSkia* image_skia)
      : SimpleThread("image_skia_on_thread"),
        image_skia_(image_skia),
        can_read_(false),
        can_modify_(false) {
  }

  virtual void Run() OVERRIDE {
    can_read_ = image_skia_->CanRead();
    can_modify_ = image_skia_->CanModify();
    if (can_read_)
      image_skia_->image_reps();
  }

  void StartAndJoin() {
    Start();
    Join();
  }

  bool can_read() const { return can_read_; }

  bool can_modify() const { return can_modify_; }

 private:
  ImageSkia* image_skia_;

  bool can_read_;
  bool can_modify_;

  DISALLOW_COPY_AND_ASSIGN(TestOnThread);
};

}  // namespace test

TEST(ImageSkiaTest, FixedSource) {
  ImageSkiaRep image(Size(100, 200), ui::SCALE_FACTOR_100P);
  ImageSkia image_skia(new FixedSource(image), Size(100, 200));
  EXPECT_EQ(0U, image_skia.image_reps().size());

  const ImageSkiaRep& result_100p =
      image_skia.GetRepresentation(ui::SCALE_FACTOR_100P);
  EXPECT_EQ(100, result_100p.GetWidth());
  EXPECT_EQ(200, result_100p.GetHeight());
  EXPECT_EQ(ui::SCALE_FACTOR_100P, result_100p.scale_factor());
  EXPECT_EQ(1U, image_skia.image_reps().size());

  const ImageSkiaRep& result_200p =
      image_skia.GetRepresentation(ui::SCALE_FACTOR_200P);

  EXPECT_EQ(100, result_200p.GetWidth());
  EXPECT_EQ(200, result_200p.GetHeight());
  EXPECT_EQ(100, result_200p.pixel_width());
  EXPECT_EQ(200, result_200p.pixel_height());
  EXPECT_EQ(ui::SCALE_FACTOR_100P, result_200p.scale_factor());
  EXPECT_EQ(1U, image_skia.image_reps().size());

  // Get the representation again and make sure it doesn't
  // generate new image skia rep.
  image_skia.GetRepresentation(ui::SCALE_FACTOR_100P);
  image_skia.GetRepresentation(ui::SCALE_FACTOR_200P);
  EXPECT_EQ(1U, image_skia.image_reps().size());
}

TEST(ImageSkiaTest, DynamicSource) {
  ImageSkia image_skia(new DynamicSource(Size(100, 200)), Size(100, 200));
  EXPECT_EQ(0U, image_skia.image_reps().size());
  const ImageSkiaRep& result_100p =
      image_skia.GetRepresentation(ui::SCALE_FACTOR_100P);
  EXPECT_EQ(100, result_100p.GetWidth());
  EXPECT_EQ(200, result_100p.GetHeight());
  EXPECT_EQ(ui::SCALE_FACTOR_100P, result_100p.scale_factor());
  EXPECT_EQ(1U, image_skia.image_reps().size());

  const ImageSkiaRep& result_200p =
      image_skia.GetRepresentation(ui::SCALE_FACTOR_200P);
  EXPECT_EQ(100, result_200p.GetWidth());
  EXPECT_EQ(200, result_200p.GetHeight());
  EXPECT_EQ(200, result_200p.pixel_width());
  EXPECT_EQ(400, result_200p.pixel_height());
  EXPECT_EQ(ui::SCALE_FACTOR_200P, result_200p.scale_factor());
  EXPECT_EQ(2U, image_skia.image_reps().size());

  // Get the representation again and make sure it doesn't
  // generate new image skia rep.
  image_skia.GetRepresentation(ui::SCALE_FACTOR_100P);
  EXPECT_EQ(2U, image_skia.image_reps().size());
  image_skia.GetRepresentation(ui::SCALE_FACTOR_200P);
  EXPECT_EQ(2U, image_skia.image_reps().size());
}

// Tests that image_reps returns all of the representations in the
// image when there are multiple representations for a scale factor.
// This currently is the case with ImageLoader::LoadImages.
TEST(ImageSkiaTest, ManyRepsPerScaleFactor) {
  const int kSmallIcon1x = 16;
  const int kSmallIcon2x = 32;
  const int kLargeIcon1x = 32;

  ImageSkia image(new NullSource(), gfx::Size(kSmallIcon1x, kSmallIcon1x));
  // Simulate a source which loads images on a delay. Upon
  // GetImageForScaleFactor, it immediately returns null and starts loading
  // image reps slowly.
  image.GetRepresentation(ui::SCALE_FACTOR_100P);
  image.GetRepresentation(ui::SCALE_FACTOR_200P);

  // After a lengthy amount of simulated time, finally loaded image reps.
  image.AddRepresentation(ImageSkiaRep(
      gfx::Size(kSmallIcon1x, kSmallIcon1x), ui::SCALE_FACTOR_100P));
  image.AddRepresentation(ImageSkiaRep(
      gfx::Size(kSmallIcon2x, kSmallIcon2x), ui::SCALE_FACTOR_200P));
  image.AddRepresentation(ImageSkiaRep(
      gfx::Size(kLargeIcon1x, kLargeIcon1x), ui::SCALE_FACTOR_100P));

  std::vector<ImageSkiaRep> image_reps = image.image_reps();
  EXPECT_EQ(3u, image_reps.size());

  int num_1x = 0;
  int num_2x = 0;
  for (size_t i = 0; i < image_reps.size(); ++i) {
    if (image_reps[i].scale_factor() == ui::SCALE_FACTOR_100P)
      num_1x++;
    else if (image_reps[i].scale_factor() == ui::SCALE_FACTOR_200P)
      num_2x++;
  }
  EXPECT_EQ(2, num_1x);
  EXPECT_EQ(1, num_2x);
}

TEST(ImageSkiaTest, GetBitmap) {
  ImageSkia image_skia(new DynamicSource(Size(100, 200)), Size(100, 200));
  const SkBitmap* bitmap = image_skia.bitmap();
  EXPECT_NE(static_cast<SkBitmap*>(NULL), bitmap);
  EXPECT_FALSE(bitmap->isNull());
}

TEST(ImageSkiaTest, GetBitmapFromEmpty) {
  // Create an image with 1 representation and remove it so the ImageSkiaStorage
  // is left with no representations.
  ImageSkia empty_image(ImageSkiaRep(Size(100, 200), ui::SCALE_FACTOR_100P));
  ImageSkia empty_image_copy(empty_image);
  empty_image.RemoveRepresentation(ui::SCALE_FACTOR_100P);

  // Check that ImageSkia::bitmap() still returns a valid SkBitmap pointer for
  // the image and all its copies.
  const SkBitmap* bitmap = empty_image_copy.bitmap();
  ASSERT_NE(static_cast<SkBitmap*>(NULL), bitmap);
  EXPECT_TRUE(bitmap->isNull());
  EXPECT_TRUE(bitmap->empty());
}

TEST(ImageSkiaTest, BackedBySameObjectAs) {
  // Null images should all be backed by the same object (NULL).
  ImageSkia image;
  ImageSkia unrelated;
  EXPECT_TRUE(image.BackedBySameObjectAs(unrelated));

  image.AddRepresentation(gfx::ImageSkiaRep(gfx::Size(10, 10),
                                            ui::SCALE_FACTOR_100P));
  ImageSkia copy = image;
  copy.AddRepresentation(gfx::ImageSkiaRep(gfx::Size(10, 10),
                                           ui::SCALE_FACTOR_200P));
  unrelated.AddRepresentation(gfx::ImageSkiaRep(gfx::Size(10, 10),
                                                ui::SCALE_FACTOR_100P));
  EXPECT_TRUE(image.BackedBySameObjectAs(copy));
  EXPECT_FALSE(image.BackedBySameObjectAs(unrelated));
  EXPECT_FALSE(copy.BackedBySameObjectAs(unrelated));
}

#if ENABLE_NON_THREAD_SAFE
TEST(ImageSkiaTest, EmptyOnThreadTest) {
  ImageSkia empty;
  test::TestOnThread empty_on_thread(&empty);
  empty_on_thread.Start();
  empty_on_thread.Join();
  EXPECT_TRUE(empty_on_thread.can_read());
  EXPECT_TRUE(empty_on_thread.can_modify());
}

TEST(ImageSkiaTest, StaticOnThreadTest) {
  ImageSkia image(ImageSkiaRep(Size(100, 200), ui::SCALE_FACTOR_100P));
  EXPECT_FALSE(image.IsThreadSafe());

  test::TestOnThread image_on_thread(&image);
  // an image that was never accessed on this thread can be
  // read by other thread.
  image_on_thread.StartAndJoin();
  EXPECT_TRUE(image_on_thread.can_read());
  EXPECT_TRUE(image_on_thread.can_modify());
  EXPECT_FALSE(image.CanRead());
  EXPECT_FALSE(image.CanModify());

  image.DetachStorageFromThread();
  // An image is accessed by this thread,
  // so other thread cannot read/modify it.
  image.image_reps();
  test::TestOnThread image_on_thread2(&image);
  image_on_thread2.StartAndJoin();
  EXPECT_FALSE(image_on_thread2.can_read());
  EXPECT_FALSE(image_on_thread2.can_modify());
  EXPECT_TRUE(image.CanRead());
  EXPECT_TRUE(image.CanModify());

  image.DetachStorageFromThread();
  scoped_ptr<ImageSkia> deep_copy(image.DeepCopy());
  EXPECT_FALSE(deep_copy->IsThreadSafe());
  test::TestOnThread deepcopy_on_thread(deep_copy.get());
  deepcopy_on_thread.StartAndJoin();
  EXPECT_TRUE(deepcopy_on_thread.can_read());
  EXPECT_TRUE(deepcopy_on_thread.can_modify());
  EXPECT_FALSE(deep_copy->CanRead());
  EXPECT_FALSE(deep_copy->CanModify());

  scoped_ptr<ImageSkia> deep_copy2(image.DeepCopy());
  EXPECT_EQ(1U, deep_copy2->image_reps().size());
  // Access it from current thread so that it can't be
  // accessed from another thread.
  deep_copy2->image_reps();
  EXPECT_FALSE(deep_copy2->IsThreadSafe());
  test::TestOnThread deepcopy2_on_thread(deep_copy2.get());
  deepcopy2_on_thread.StartAndJoin();
  EXPECT_FALSE(deepcopy2_on_thread.can_read());
  EXPECT_FALSE(deepcopy2_on_thread.can_modify());
  EXPECT_TRUE(deep_copy2->CanRead());
  EXPECT_TRUE(deep_copy2->CanModify());

  image.DetachStorageFromThread();
  image.SetReadOnly();
  // A read-only ImageSkia with no source is thread safe.
  EXPECT_TRUE(image.IsThreadSafe());
  test::TestOnThread readonly_on_thread(&image);
  readonly_on_thread.StartAndJoin();
  EXPECT_TRUE(readonly_on_thread.can_read());
  EXPECT_FALSE(readonly_on_thread.can_modify());
  EXPECT_TRUE(image.CanRead());
  EXPECT_FALSE(image.CanModify());

  image.DetachStorageFromThread();
  image.MakeThreadSafe();
  EXPECT_TRUE(image.IsThreadSafe());
  test::TestOnThread threadsafe_on_thread(&image);
  threadsafe_on_thread.StartAndJoin();
  EXPECT_TRUE(threadsafe_on_thread.can_read());
  EXPECT_FALSE(threadsafe_on_thread.can_modify());
  EXPECT_TRUE(image.CanRead());
  EXPECT_FALSE(image.CanModify());
}

TEST(ImageSkiaTest, SourceOnThreadTest) {
  ImageSkia image(new DynamicSource(Size(100, 200)), Size(100, 200));
  EXPECT_FALSE(image.IsThreadSafe());

  test::TestOnThread image_on_thread(&image);
  image_on_thread.StartAndJoin();
  // an image that was never accessed on this thread can be
  // read by other thread.
  EXPECT_TRUE(image_on_thread.can_read());
  EXPECT_TRUE(image_on_thread.can_modify());
  EXPECT_FALSE(image.CanRead());
  EXPECT_FALSE(image.CanModify());

  image.DetachStorageFromThread();
  // An image is accessed by this thread,
  // so other thread cannot read/modify it.
  image.image_reps();
  test::TestOnThread image_on_thread2(&image);
  image_on_thread2.StartAndJoin();
  EXPECT_FALSE(image_on_thread2.can_read());
  EXPECT_FALSE(image_on_thread2.can_modify());
  EXPECT_TRUE(image.CanRead());
  EXPECT_TRUE(image.CanModify());

  image.DetachStorageFromThread();
  image.SetReadOnly();
  EXPECT_FALSE(image.IsThreadSafe());
  test::TestOnThread readonly_on_thread(&image);
  readonly_on_thread.StartAndJoin();
  EXPECT_TRUE(readonly_on_thread.can_read());
  EXPECT_FALSE(readonly_on_thread.can_modify());
  EXPECT_FALSE(image.CanRead());
  EXPECT_FALSE(image.CanModify());

  image.DetachStorageFromThread();
  image.MakeThreadSafe();
  EXPECT_TRUE(image.IsThreadSafe());
  // Check if image reps are generated for supported scale factors.
  EXPECT_EQ(ui::GetSupportedScaleFactors().size(),
            image.image_reps().size());
  test::TestOnThread threadsafe_on_thread(&image);
  threadsafe_on_thread.StartAndJoin();
  EXPECT_TRUE(threadsafe_on_thread.can_read());
  EXPECT_FALSE(threadsafe_on_thread.can_modify());
  EXPECT_TRUE(image.CanRead());
  EXPECT_FALSE(image.CanModify());
}
#endif  // ENABLE_NON_THREAD_SAFE

// Just in case we ever get lumped together with other compilation units.
#undef ENABLE_NON_THREAD_SAFE

}  // namespace gfx

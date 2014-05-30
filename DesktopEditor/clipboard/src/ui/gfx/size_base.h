// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SIZE_BASE_H_
#define UI_GFX_SIZE_BASE_H_

#include "ui/base/ui_export.h"

namespace gfx {

// A size has width and height values.
template<typename Class, typename Type>
class UI_EXPORT SizeBase {
 public:
  Type width() const { return width_; }
  Type height() const { return height_; }

  Type GetArea() const { return width_ * height_; }

  void SetSize(Type width, Type height) {
    set_width(width);
    set_height(height);
  }

  void Enlarge(Type width, Type height) {
    set_width(width_ + width);
    set_height(height_ + height);
  }

  void set_width(Type width) {
    width_ = width < 0 ? 0 : width;
  }
  void set_height(Type height) {
    height_ = height < 0 ? 0 : height;
  }

  void ClampToMax(const Class& max) {
    width_ = width_ <= max.width_ ? width_ : max.width_;
    height_ = height_ <= max.height_ ? height_ : max.height_;
  }

  void ClampToMin(const Class& min) {
    width_ = width_ >= min.width_ ? width_ : min.width_;
    height_ = height_ >= min.height_ ? height_ : min.height_;
  }

  bool IsEmpty() const {
    return (width_ == 0) || (height_ == 0);
  }

 protected:
  SizeBase(Type width, Type height)
      : width_(width < 0 ? 0 : width),
      height_(height < 0 ? 0 : height) {
  }

  // Destructor is intentionally made non virtual and protected.
  // Do not make this public.
  ~SizeBase() {}

 private:
  Type width_;
  Type height_;
};

}  // namespace gfx

#endif  // UI_GFX_SIZE_BASE_H_

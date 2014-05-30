// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_IMAGE_STUB_H_
#define UI_GL_GL_IMAGE_STUB_H_

#include "ui/gl/gl_image.h"

namespace gfx {

// A GLImage that does nothing for unit tests.
class GL_EXPORT GLImageStub : public GLImage {
 public:
  // Implement GLImage.
  virtual void Destroy() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual bool BindTexImage() OVERRIDE;
  virtual void ReleaseTexImage() OVERRIDE;

 protected:
  virtual ~GLImageStub();
};

}  // namespace gfx

#endif  // UI_GL_GL_IMAGE_STUB_H_

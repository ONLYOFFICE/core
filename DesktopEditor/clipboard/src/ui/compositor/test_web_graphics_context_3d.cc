// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/test_web_graphics_context_3d.h"

#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context_stub.h"
#include "ui/gl/gl_surface_stub.h"

namespace ui {

TestWebGraphicsContext3D::TestWebGraphicsContext3D()
    : next_texture_id_(1) {}

TestWebGraphicsContext3D::~TestWebGraphicsContext3D() {}

void TestWebGraphicsContext3D::Initialize() {
  gl_surface_ = new gfx::GLSurfaceStub;
  gl_context_ = new gfx::GLContextStub;
  gl_context_->MakeCurrent(gl_surface_.get());
}

bool TestWebGraphicsContext3D::makeContextCurrent() {
  return true;
}

int TestWebGraphicsContext3D::width() {
  return 0;
}

int TestWebGraphicsContext3D::height() {
  return 0;
}

bool TestWebGraphicsContext3D::isGLES2Compliant() {
  return false;
}

bool TestWebGraphicsContext3D::readBackFramebuffer(
    unsigned char* pixels,
    size_t bufferSize,
    WebKit::WebGLId framebuffer,
    int width,
    int height) {
  return false;
}

WebKit::WebGLId TestWebGraphicsContext3D::getPlatformTextureId() {
  return 0;
}

bool TestWebGraphicsContext3D::isContextLost() {
  return false;
}

void* TestWebGraphicsContext3D::mapBufferSubDataCHROMIUM(
    WebKit::WGC3Denum target,
    WebKit::WGC3Dintptr offset,
    WebKit::WGC3Dsizeiptr size,
    WebKit::WGC3Denum access) {
  return 0;
}

void* TestWebGraphicsContext3D::mapTexSubImage2DCHROMIUM(
    WebKit::WGC3Denum target,
    WebKit::WGC3Dint level,
    WebKit::WGC3Dint xoffset,
    WebKit::WGC3Dint yoffset,
    WebKit::WGC3Dsizei width,
    WebKit::WGC3Dsizei height,
    WebKit::WGC3Denum format,
    WebKit::WGC3Denum type,
    WebKit::WGC3Denum access) {
  return 0;
}

WebKit::WebString TestWebGraphicsContext3D::getRequestableExtensionsCHROMIUM() {
  return WebKit::WebString();
}

WebKit::WGC3Denum TestWebGraphicsContext3D::checkFramebufferStatus(
    WebKit::WGC3Denum target) {
  return GL_FRAMEBUFFER_COMPLETE;
}

bool TestWebGraphicsContext3D::getActiveAttrib(WebKit::WebGLId program,
                                               WebKit::WGC3Duint index,
                                               ActiveInfo& info) {
  return false;
}

bool TestWebGraphicsContext3D::getActiveUniform(WebKit::WebGLId program,
                                                WebKit::WGC3Duint index,
                                                ActiveInfo& info) {
  return false;
}

WebKit::WGC3Dint TestWebGraphicsContext3D::getAttribLocation(
    WebKit::WebGLId program,
    const WebKit::WGC3Dchar* name) {
  return 0;
}

TestWebGraphicsContext3D::Attributes
TestWebGraphicsContext3D::getContextAttributes() {
  return Attributes();
}

WebKit::WGC3Denum TestWebGraphicsContext3D::getError() {
  return 0;
}

void TestWebGraphicsContext3D::getIntegerv(WebKit::WGC3Denum pname,
                                           WebKit::WGC3Dint* value) {
  if (pname == GL_MAX_TEXTURE_SIZE)
    *value = 1024;
}

void TestWebGraphicsContext3D::getProgramiv(WebKit::WebGLId program,
                                            WebKit::WGC3Denum pname,
                                            WebKit::WGC3Dint* value) {
  if (pname == GL_LINK_STATUS)
    *value = 1;
}

WebKit::WebString TestWebGraphicsContext3D::getProgramInfoLog(
    WebKit::WebGLId program) {
  return WebKit::WebString();
}

void TestWebGraphicsContext3D::getShaderiv(WebKit::WebGLId shader,
                                           WebKit::WGC3Denum pname,
                                           WebKit::WGC3Dint* value) {
  if (pname == GL_COMPILE_STATUS)
    *value = 1;
}

WebKit::WebString TestWebGraphicsContext3D::getShaderInfoLog(
    WebKit::WebGLId shader) {
  return WebKit::WebString();
}

WebKit::WebString TestWebGraphicsContext3D::getShaderSource(
    WebKit::WebGLId shader) {
  return WebKit::WebString();
}

WebKit::WebString TestWebGraphicsContext3D::getString(WebKit::WGC3Denum name) {
  return WebKit::WebString();
}

WebKit::WGC3Dint TestWebGraphicsContext3D::getUniformLocation(
    WebKit::WebGLId program,
    const WebKit::WGC3Dchar* name) {
  return 0;
}

WebKit::WGC3Dsizeiptr TestWebGraphicsContext3D::getVertexAttribOffset(
    WebKit::WGC3Duint index,
    WebKit::WGC3Denum pname) {
  return 0;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isBuffer(
    WebKit::WebGLId buffer) {
  return false;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isEnabled(
    WebKit::WGC3Denum cap) {
  return false;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isFramebuffer(
    WebKit::WebGLId framebuffer) {
  return false;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isProgram(
    WebKit::WebGLId program) {
  return false;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isRenderbuffer(
    WebKit::WebGLId renderbuffer) {
  return false;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isShader(
    WebKit::WebGLId shader) {
  return false;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isTexture(
    WebKit::WebGLId texture) {
  return false;
}

WebKit::WebGLId TestWebGraphicsContext3D::createBuffer() {
  return 1;
}

WebKit::WebGLId TestWebGraphicsContext3D::createFramebuffer() {
  return 1;
}

WebKit::WebGLId TestWebGraphicsContext3D::createProgram() {
  return 1;
}

WebKit::WebGLId TestWebGraphicsContext3D::createRenderbuffer() {
  return 1;
}

WebKit::WebGLId TestWebGraphicsContext3D::createShader(
    WebKit::WGC3Denum value) {
  return 1;
}

WebKit::WebGLId TestWebGraphicsContext3D::createTexture() {
  return next_texture_id_++;
}

WebKit::WebGLId TestWebGraphicsContext3D::createQueryEXT()
{
  return 1;
}

WebKit::WGC3Dboolean TestWebGraphicsContext3D::isQueryEXT(WebKit::WebGLId)
{
  return true;
}

}  // namespace ui

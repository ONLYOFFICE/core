// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "ui/gl/gl_bindings_skia_in_process.h"

#include "base/logging.h"
#include "third_party/skia/include/gpu/gl/GrGLInterface.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_implementation.h"

namespace {

extern "C" {
// The following stub functions are required because the glXXX routines exported
// via gl_bindings.h use call-type GL_BINDING_CALL, which on Windows is stdcall.
// Skia has been built such that its GrGLInterface GL pointers are __cdecl.

GLvoid StubGLActiveTexture(GLenum texture) {
  glActiveTexture(texture);
}

GLvoid StubGLAttachShader(GLuint program, GLuint shader) {
  glAttachShader(program, shader);
}

GLvoid StubGLBeginQuery(GLenum target, GLuint id) {
  glBeginQuery(target, id);
}

GLvoid StubGLBindAttribLocation(GLuint program, GLuint index,
                                const char* name) {
  glBindAttribLocation(program, index, name);
}

GLvoid StubGLBindBuffer(GLenum target, GLuint buffer) {
  glBindBuffer(target, buffer);
}

GLvoid StubGLBindFragDataLocation(GLuint program, GLuint colorNumber,
                                  const GLchar * name) {
  glBindFragDataLocation(program, colorNumber, name);
}

GLvoid StubGLBindFragDataLocationIndexed(GLuint program, GLuint colorNumber,
                                         GLuint index, const GLchar * name) {
  glBindFragDataLocationIndexed(program, colorNumber, index, name);
}

GLvoid StubGLBindFramebuffer(GLenum target, GLuint framebuffer) {
  glBindFramebufferEXT(target, framebuffer);
}

GLvoid StubGLBindRenderbuffer(GLenum target, GLuint renderbuffer) {
  glBindRenderbufferEXT(target, renderbuffer);
}

GLvoid StubGLBindTexture(GLenum target, GLuint texture) {
  glBindTexture(target, texture);
}

GLvoid StubGLBindVertexArray(GLuint array) {
  glBindVertexArrayOES(array);
}

GLvoid StubGLBlendColor(GLclampf red, GLclampf green, GLclampf blue,
                        GLclampf alpha) {
  glBlendColor(red, green, blue, alpha);
}

GLvoid StubGLBlendFunc(GLenum sfactor, GLenum dfactor) {
  glBlendFunc(sfactor, dfactor);
}

GLvoid StubGLBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
                             GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
                             GLbitfield mask, GLenum filter) {
  glBlitFramebufferEXT(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1,
                       mask, filter);
}

GLvoid StubGLBufferData(GLenum target, GLsizeiptr size, const void* data,
                        GLenum usage) {
  glBufferData(target, size, data, usage);
}

GLvoid StubGLBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                           const void* data) {
  glBufferSubData(target, offset, size, data);
}

GLenum StubGLCheckFramebufferStatus(GLenum target) {
  return glCheckFramebufferStatusEXT(target);
}

GLvoid StubGLClear(GLbitfield mask) {
  glClear(mask);
}

GLvoid StubGLClearColor(GLclampf red, GLclampf green, GLclampf blue,
                        GLclampf alpha) {
  glClearColor(red, green, blue, alpha);
}

GLvoid StubGLClearStencil(GLint s) {
  glClearStencil(s);
}

GLvoid StubGLColorMask(GLboolean red, GLboolean green, GLboolean blue,
                       GLboolean alpha) {
  glColorMask(red, green, blue, alpha);
}

GLvoid StubGLCompileShader(GLuint shader) {
  glCompileShader(shader);
}

GLvoid StubGLCompressedTexImage2D(GLenum target, GLint level,
                                  GLenum internalformat, GLsizei width,
                                  GLsizei height, GLint border,
                                  GLsizei imageSize, const void* data) {
  glCompressedTexImage2D(target, level, internalformat, width, height, border,
                         imageSize, data);
}

GLvoid StubGLCopyTexSubImage2D(GLenum target, GLint level,
                               GLint xoffset, GLint yoffset,
                               GLint x, GLint y,
                               GLsizei width, GLsizei height) {
  glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

GLuint StubGLCreateProgram(void) {
  return glCreateProgram();
}

GLuint StubGLCreateShader(GLenum type) {
  return glCreateShader(type);
}

GLvoid StubGLCullFace(GLenum mode) {
  glCullFace(mode);
}

GLvoid StubGLDeleteBuffers(GLsizei n, const GLuint* buffers) {
  glDeleteBuffersARB(n, buffers);
}

GLvoid StubGLDeleteFramebuffers(GLsizei n, const GLuint* framebuffers) {
  glDeleteFramebuffersEXT(n, framebuffers);
}

GLvoid StubGLDeleteQueries(GLsizei n, const GLuint* ids) {
  glDeleteQueries(n, ids);
}

GLvoid StubGLDeleteProgram(GLuint program) {
  glDeleteProgram(program);
}

GLvoid StubGLDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers) {
  glDeleteRenderbuffersEXT(n, renderbuffers);
}

GLvoid StubGLDeleteShader(GLuint shader) {
  glDeleteShader(shader);
}

GLvoid StubGLDeleteTextures(GLsizei n, const GLuint* textures) {
  glDeleteTextures(n, textures);
}

GLvoid StubGLDeleteVertexArrays(GLsizei n, const GLuint* arrays) {
  glDeleteVertexArraysOES(n, arrays);
}

GLvoid StubGLDepthMask(GLboolean flag) {
  glDepthMask(flag);
}

GLvoid StubGLDisable(GLenum cap) {
  glDisable(cap);
}

GLvoid StubGLDisableVertexAttribArray(GLuint index) {
  glDisableVertexAttribArray(index);
}

GLvoid StubGLDrawArrays(GLenum mode, GLint first, GLsizei count) {
  glDrawArrays(mode, first, count);
}

GLvoid StubGLDrawBuffer(GLenum mode) {
  glDrawBuffer(mode);
}

GLvoid StubGLDrawBuffers(GLsizei n, const GLenum* bufs) {
  glDrawBuffersARB(n, bufs);
}

GLvoid StubGLDrawElements(GLenum mode, GLsizei count, GLenum type,
                          const void* indices) {
  glDrawElements(mode, count, type, indices);
}

GLvoid StubGLEnable(GLenum cap) {
  glEnable(cap);
}

GLvoid StubGLEnableVertexAttribArray(GLuint index) {
  glEnableVertexAttribArray(index);
}

GLvoid StubGLEndQuery(GLenum target) {
  glEndQuery(target);
}

GLvoid StubGLFinish() {
  glFinish();
}

GLvoid StubGLFlush() {
  glFlush();
}

GLvoid StubGLFramebufferRenderbuffer(GLenum target, GLenum attachment,
                                     GLenum renderbuffertarget,
                                     GLuint renderbuffer) {
  glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget,
                               renderbuffer);
}

GLvoid StubGLFramebufferTexture2D(GLenum target, GLenum attachment,
                                  GLenum textarget, GLuint texture,
                                  GLint level) {
  glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
}

GLvoid StubGLFrontFace(GLenum mode) {
  glFrontFace(mode);
}

GLvoid StubGLGenBuffers(GLsizei n, GLuint* buffers) {
  glGenBuffersARB(n, buffers);
}

GLvoid StubGLGenFramebuffers(GLsizei n, GLuint* framebuffers) {
  glGenFramebuffersEXT(n, framebuffers);
}

GLvoid StubGLGenQueries(GLsizei n, GLuint* ids) {
  glGenQueries(n, ids);
}

GLvoid StubGLGenRenderbuffers(GLsizei n, GLuint* renderbuffers) {
  glGenRenderbuffersEXT(n, renderbuffers);
}

GLvoid StubGLGenTextures(GLsizei n, GLuint* textures) {
  glGenTextures(n, textures);
}

GLvoid StubGLGenVertexArrays(GLsizei n, GLuint* arrays) {
  glGenVertexArraysOES(n, arrays);
}

GLvoid StubGLGetBufferParameteriv(GLenum target, GLenum pname, GLint* params) {
  glGetBufferParameteriv(target, pname, params);
}

GLvoid StubGLGetFramebufferAttachmentParameteriv(GLenum target,
                                                 GLenum attachment,
                                                 GLenum pname, GLint* params) {
  glGetFramebufferAttachmentParameterivEXT(target, attachment, pname, params);
}

GLenum StubGLGetError() {
  return glGetError();
}

GLvoid StubGLGetIntegerv(GLenum pname, GLint* params) {
  glGetIntegerv(pname, params);
}

GLvoid StubGLGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length,
                               char* infolog) {
  glGetProgramInfoLog(program, bufsize, length, infolog);
}

GLvoid StubGLGetProgramiv(GLuint program, GLenum pname, GLint* params) {
  glGetProgramiv(program, pname, params);
}

GLvoid StubGLGetRenderbufferParameteriv(GLenum target,
                                        GLenum pname, GLint* params) {
  glGetRenderbufferParameterivEXT(target, pname, params);
}

GLvoid StubGLGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length,
                              char* infolog) {
  glGetShaderInfoLog(shader, bufsize, length, infolog);
}

GLvoid StubGLGetShaderiv(GLuint shader, GLenum pname, GLint* params) {
  glGetShaderiv(shader, pname, params);
}

const GLubyte* StubGLGetString(GLenum name) {
  return glGetString(name);
}

GLvoid StubGLGetQueryiv(GLenum target, GLenum pname, GLint* params) {
  glGetQueryiv(target, pname, params);
}

GLvoid StubGLGetQueryObjecti64v(GLuint id, GLenum pname, GLint64* params) {
  glGetQueryObjecti64v(id, pname, params);
}

GLvoid StubGLGetQueryObjectiv(GLuint id, GLenum pname, GLint* params) {
  glGetQueryObjectiv(id, pname, params);
}

GLvoid StubGLGetQueryObjectui64v(GLuint id, GLenum pname, GLuint64* params) {
  glGetQueryObjectui64v(id, pname, params);
}

GLvoid StubGLGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params) {
  glGetQueryObjectuiv(id, pname, params);
}

GLvoid StubGLGetTexLevelParameteriv(GLenum target, GLint level,
                                    GLenum pname, GLint* params) {
  glGetTexLevelParameteriv(target, level, pname, params);
}

GLint StubGLGetUniformLocation(GLuint program, const char* name) {
  return glGetUniformLocation(program, name);
}

GLvoid StubGLLineWidth(GLfloat width) {
  glLineWidth(width);
}

GLvoid StubGLLinkProgram(GLuint program) {
  glLinkProgram(program);
}

void* StubGLMapBuffer(GLenum target, GLenum access) {
  return glMapBuffer(target, access);
}

GLvoid StubGLPixelStorei(GLenum pname, GLint param) {
  glPixelStorei(pname, param);
}

GLvoid StubGLQueryCounter(GLuint id, GLenum target) {
  glQueryCounter(id, target);
}

GLvoid StubGLReadBuffer(GLenum src) {
  glReadBuffer(src);
}

GLvoid StubGLReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                        GLenum format, GLenum type, void* pixels) {
  glReadPixels(x, y, width, height, format, type, pixels);
}

GLvoid StubGLRenderbufferStorage(GLenum target, GLenum internalformat,
                                 GLsizei width, GLsizei height) {
  glRenderbufferStorageEXT(target, internalformat, width, height);
}

GLvoid StubGLRenderbufferStorageMultisample(GLenum target, GLsizei samples,
                                            GLenum internalformat,
                                            GLsizei width, GLsizei height) {
  glRenderbufferStorageMultisampleEXT(target, samples, internalformat, width,
                                      height);
}

GLvoid StubGLScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
  glScissor(x, y, width, height);
}

GLvoid StubGLShaderSource(GLuint shader, GLsizei count, const char* const* str,
                          const GLint* length) {
  glShaderSource(shader, count, str, length);
}

GLvoid StubGLStencilFunc(GLenum func, GLint ref, GLuint mask) {
  glStencilFunc(func, ref, mask);
}

GLvoid StubGLStencilFuncSeparate(GLenum face, GLenum func, GLint ref,
                                 GLuint mask) {
  glStencilFuncSeparate(face, func, ref, mask);
}

GLvoid StubGLStencilMask(GLuint mask) {
  glStencilMask(mask);
}

GLvoid StubGLStencilMaskSeparate(GLenum face, GLuint mask) {
  glStencilMaskSeparate(face, mask);
}

GLvoid StubGLStencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
  glStencilOp(fail, zfail, zpass);
}

GLvoid StubGLStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail,
                               GLenum zpass) {
  glStencilOpSeparate(face, fail, zfail, zpass);
}

GLvoid StubGLTexImage2D(GLenum target, GLint level, GLint internalformat,
                        GLsizei width, GLsizei height, GLint border,
                        GLenum format, GLenum type, const void* pixels) {
  glTexImage2D(target, level, internalformat, width, height, border, format,
               type, pixels);
}

GLvoid StubGLTexParameteri(GLenum target, GLenum pname, GLint param) {
  glTexParameteri(target, pname, param);
}

GLvoid StubGLTexParameteriv(GLenum target, GLenum pname, const GLint* params) {
  glTexParameteriv(target, pname, params);
}

GLvoid StubGLTexStorage2D(GLenum target, GLsizei levels, GLenum internalFormat,
                          GLsizei width, GLsizei height) {
  glTexStorage2DEXT(target, levels, internalFormat, width, height);
}

GLvoid StubGLTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                           GLint yoffset, GLsizei width, GLsizei height,
                           GLenum format, GLenum type, const void* pixels) {
  glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type,
                  pixels);
}

GLvoid StubGLUniform1f(GLint location, GLfloat v) {
  glUniform1i(location, v);
}

GLvoid StubGLUniform1i(GLint location, GLint v) {
  glUniform1i(location, v);
}

GLvoid StubGLUniform1fv(GLint location, GLsizei count, const GLfloat* v) {
  glUniform1fv(location, count, v);
}

GLvoid StubGLUniform1iv(GLint location, GLsizei count, const GLint* v) {
  glUniform1iv(location, count, v);
}

GLvoid StubGLUniform2f(GLint location, GLfloat v0, GLfloat v1) {
  glUniform2i(location, v0, v1);
}

GLvoid StubGLUniform2i(GLint location, GLint v0, GLint v1) {
  glUniform2i(location, v0, v1);
}

GLvoid StubGLUniform2fv(GLint location, GLsizei count, const GLfloat* v) {
  glUniform2fv(location, count, v);
}

GLvoid StubGLUniform2iv(GLint location, GLsizei count, const GLint* v) {
  glUniform2iv(location, count, v);
}

GLvoid StubGLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  glUniform3i(location, v0, v1, v2);
}

GLvoid StubGLUniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
  glUniform3i(location, v0, v1, v2);
}

GLvoid StubGLUniform3fv(GLint location, GLsizei count, const GLfloat* v) {
  glUniform3fv(location, count, v);
}

GLvoid StubGLUniform3iv(GLint location, GLsizei count, const GLint* v) {
  glUniform3iv(location, count, v);
}

GLvoid StubGLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2,
                       GLfloat v3) {
  glUniform4i(location, v0, v1, v2, v3);
}

GLvoid StubGLUniform4i(GLint location, GLint v0, GLint v1, GLint v2,
                       GLint v3) {
  glUniform4i(location, v0, v1, v2, v3);
}

GLvoid StubGLUniform4fv(GLint location, GLsizei count, const GLfloat* v) {
  glUniform4fv(location, count, v);
}

GLvoid StubGLUniform4iv(GLint location, GLsizei count, const GLint* v) {
  glUniform4iv(location, count, v);
}

GLvoid StubGLUniformMatrix2fv(GLint location, GLsizei count,
                              GLboolean transpose, const GLfloat* value) {
  glUniformMatrix2fv(location, count, transpose, value);
}

GLvoid StubGLUniformMatrix3fv(GLint location, GLsizei count,
                              GLboolean transpose, const GLfloat* value) {
  glUniformMatrix3fv(location, count, transpose, value);
}

GLvoid StubGLUniformMatrix4fv(GLint location, GLsizei count,
                              GLboolean transpose, const GLfloat* value) {
  glUniformMatrix4fv(location, count, transpose, value);
}

GLboolean StubGLUnmapBuffer(GLenum target) {
  return glUnmapBuffer(target);
}

GLvoid StubGLUseProgram(GLuint program) {
  glUseProgram(program);
}

GLvoid StubGLVertexAttrib4fv(GLuint indx, const GLfloat* values) {
  glVertexAttrib4fv(indx, values);
}

GLvoid StubGLVertexAttribPointer(GLuint indx, GLint size, GLenum type,
                                 GLboolean normalized, GLsizei stride,
                                 const void* ptr) {
  glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

GLvoid StubGLViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  glViewport(x, y, width, height);
}
}  // extern "C"
}  // namespace

namespace gfx {

GrGLInterface* CreateInProcessSkiaGLBinding() {
  GrGLBinding binding;
  switch (gfx::GetGLImplementation()) {
    case gfx::kGLImplementationNone:
      NOTREACHED();
      return NULL;
    case gfx::kGLImplementationDesktopGL:
    case gfx::kGLImplementationAppleGL:
      binding = kDesktop_GrGLBinding;
      break;
    case gfx::kGLImplementationOSMesaGL:
      binding = kDesktop_GrGLBinding;
      break;
    case gfx::kGLImplementationEGLGLES2:
      binding = kES2_GrGLBinding;
      break;
    case gfx::kGLImplementationMockGL:
      NOTREACHED();
      return NULL;
    default:
      NOTREACHED();
      return NULL;
  }

  GrGLInterface* interface = new GrGLInterface;

  interface->fBindingsExported = binding;
  interface->fActiveTexture = StubGLActiveTexture;
  interface->fAttachShader = StubGLAttachShader;
  interface->fBeginQuery = StubGLBeginQuery;
  interface->fBindAttribLocation = StubGLBindAttribLocation;
  interface->fBindBuffer = StubGLBindBuffer;
  interface->fBindFragDataLocation = StubGLBindFragDataLocation;
  interface->fBindTexture = StubGLBindTexture;
  interface->fBindVertexArray = StubGLBindVertexArray;
  interface->fBlendColor = StubGLBlendColor;
  interface->fBlendFunc = StubGLBlendFunc;
  interface->fBufferData = StubGLBufferData;
  interface->fBufferSubData = StubGLBufferSubData;
  interface->fClear = StubGLClear;
  interface->fClearColor = StubGLClearColor;
  interface->fClearStencil = StubGLClearStencil;
  interface->fColorMask = StubGLColorMask;
  interface->fCompileShader = StubGLCompileShader;
  interface->fCompressedTexImage2D = StubGLCompressedTexImage2D;
  interface->fCopyTexSubImage2D = StubGLCopyTexSubImage2D;
  interface->fCreateProgram = StubGLCreateProgram;
  interface->fCreateShader = StubGLCreateShader;
  interface->fCullFace = StubGLCullFace;
  interface->fDeleteBuffers = StubGLDeleteBuffers;
  interface->fDeleteProgram = StubGLDeleteProgram;
  interface->fDeleteQueries = StubGLDeleteQueries;
  interface->fDeleteShader = StubGLDeleteShader;
  interface->fDeleteTextures = StubGLDeleteTextures;
  interface->fDeleteVertexArrays = StubGLDeleteVertexArrays;
  interface->fDepthMask = StubGLDepthMask;
  interface->fDisable = StubGLDisable;
  interface->fDisableVertexAttribArray = StubGLDisableVertexAttribArray;
  interface->fDrawArrays = StubGLDrawArrays;
  interface->fDrawBuffer = StubGLDrawBuffer;
  interface->fDrawBuffers = StubGLDrawBuffers;
  interface->fDrawElements = StubGLDrawElements;
  interface->fEnable = StubGLEnable;
  interface->fEnableVertexAttribArray = StubGLEnableVertexAttribArray;
  interface->fEndQuery = StubGLEndQuery;
  interface->fFinish = StubGLFinish;
  interface->fFlush = StubGLFlush;
  interface->fFrontFace = StubGLFrontFace;
  interface->fGenBuffers = StubGLGenBuffers;
  interface->fGenQueries = StubGLGenQueries;
  interface->fGenTextures = StubGLGenTextures;
  interface->fGenVertexArrays = StubGLGenVertexArrays;
  interface->fGetBufferParameteriv = StubGLGetBufferParameteriv;
  interface->fGetError = StubGLGetError;
  interface->fGetIntegerv = StubGLGetIntegerv;
  interface->fGetQueryiv = StubGLGetQueryiv;
  interface->fGetQueryObjecti64v = StubGLGetQueryObjecti64v;
  interface->fGetQueryObjectiv = StubGLGetQueryObjectiv;
  interface->fGetQueryObjectui64v = StubGLGetQueryObjectui64v;
  interface->fGetQueryObjectuiv = StubGLGetQueryObjectuiv;
  interface->fGetProgramInfoLog = StubGLGetProgramInfoLog;
  interface->fGetProgramiv = StubGLGetProgramiv;
  interface->fGetShaderInfoLog = StubGLGetShaderInfoLog;
  interface->fGetShaderiv = StubGLGetShaderiv;
  interface->fGetString = StubGLGetString;
  interface->fGetTexLevelParameteriv = StubGLGetTexLevelParameteriv;
  interface->fGetUniformLocation = StubGLGetUniformLocation;
  interface->fLineWidth = StubGLLineWidth;
  interface->fLinkProgram = StubGLLinkProgram;
  interface->fPixelStorei = StubGLPixelStorei;
  interface->fQueryCounter = StubGLQueryCounter;
  interface->fReadBuffer = StubGLReadBuffer;
  interface->fReadPixels = StubGLReadPixels;
  interface->fScissor = StubGLScissor;
  interface->fShaderSource = StubGLShaderSource;
  interface->fStencilFunc = StubGLStencilFunc;
  interface->fStencilFuncSeparate = StubGLStencilFuncSeparate;
  interface->fStencilMask = StubGLStencilMask;
  interface->fStencilMaskSeparate = StubGLStencilMaskSeparate;
  interface->fStencilOp = StubGLStencilOp;
  interface->fStencilOpSeparate = StubGLStencilOpSeparate;
  interface->fTexImage2D = StubGLTexImage2D;
  interface->fTexParameteri = StubGLTexParameteri;
  interface->fTexParameteriv = StubGLTexParameteriv;
  interface->fTexSubImage2D = StubGLTexSubImage2D;
  interface->fTexStorage2D = StubGLTexStorage2D;
  interface->fUniform1f = StubGLUniform1f;
  interface->fUniform1i = StubGLUniform1i;
  interface->fUniform1fv = StubGLUniform1fv;
  interface->fUniform1iv = StubGLUniform1iv;
  interface->fUniform2f = StubGLUniform2f;
  interface->fUniform2i = StubGLUniform2i;
  interface->fUniform2fv = StubGLUniform2fv;
  interface->fUniform2iv = StubGLUniform2iv;
  interface->fUniform3f = StubGLUniform3f;
  interface->fUniform3i = StubGLUniform3i;
  interface->fUniform3fv = StubGLUniform3fv;
  interface->fUniform3iv = StubGLUniform3iv;
  interface->fUniform4f = StubGLUniform4f;
  interface->fUniform4i = StubGLUniform4i;
  interface->fUniform4fv = StubGLUniform4fv;
  interface->fUniform4iv = StubGLUniform4iv;
  interface->fUniformMatrix2fv = StubGLUniformMatrix2fv;
  interface->fUniformMatrix3fv = StubGLUniformMatrix3fv;
  interface->fUniformMatrix4fv = StubGLUniformMatrix4fv;
  interface->fUseProgram = StubGLUseProgram;
  interface->fVertexAttrib4fv = StubGLVertexAttrib4fv;
  interface->fVertexAttribPointer = StubGLVertexAttribPointer;
  interface->fViewport = StubGLViewport;
  interface->fBindFramebuffer = StubGLBindFramebuffer;
  interface->fBindRenderbuffer = StubGLBindRenderbuffer;
  interface->fCheckFramebufferStatus = StubGLCheckFramebufferStatus;
  interface->fDeleteFramebuffers = StubGLDeleteFramebuffers;
  interface->fDeleteRenderbuffers = StubGLDeleteRenderbuffers;
  interface->fFramebufferRenderbuffer = StubGLFramebufferRenderbuffer;
  interface->fFramebufferTexture2D = StubGLFramebufferTexture2D;
  interface->fGenFramebuffers = StubGLGenFramebuffers;
  interface->fGenRenderbuffers = StubGLGenRenderbuffers;
  interface->fGetFramebufferAttachmentParameteriv =
    StubGLGetFramebufferAttachmentParameteriv;
  interface->fGetRenderbufferParameteriv = StubGLGetRenderbufferParameteriv;
  interface->fRenderbufferStorage = StubGLRenderbufferStorage;
  interface->fRenderbufferStorageMultisample =
    StubGLRenderbufferStorageMultisample;
  interface->fBlitFramebuffer = StubGLBlitFramebuffer;
  interface->fMapBuffer = StubGLMapBuffer;
  interface->fUnmapBuffer = StubGLUnmapBuffer;
  interface->fBindFragDataLocationIndexed =
    StubGLBindFragDataLocationIndexed;
  return interface;
}

}  // namespace gfx

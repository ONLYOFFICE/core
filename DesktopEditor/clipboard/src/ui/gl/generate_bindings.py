#!/usr/bin/env python
# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""code generator for GL/GLES extension wrangler."""

import optparse
import os
import collections
import re
import sys

GL_FUNCTIONS = [
{ 'return_type': 'void',
  'names': ['glActiveTexture'],
  'arguments': 'GLenum texture', },
{ 'return_type': 'void',
  'names': ['glAttachShader'],
  'arguments': 'GLuint program, GLuint shader', },
{ 'return_type': 'void',
  'names': ['glBeginQuery'],
  'arguments': 'GLenum target, GLuint id', },
{ 'return_type': 'void',
  'names': ['glBeginQueryARB', 'glBeginQueryEXT'],
  'arguments': 'GLenum target, GLuint id', },
{ 'return_type': 'void',
  'names': ['glBindAttribLocation'],
  'arguments': 'GLuint program, GLuint index, const char* name', },
{ 'return_type': 'void',
  'names': ['glBindBuffer'],
  'arguments': 'GLenum target, GLuint buffer', },
{ 'return_type': 'void',
  'names': ['glBindFragDataLocation'],
  'arguments': 'GLuint program, GLuint colorNumber, const char* name', },
{ 'return_type': 'void',
  'names': ['glBindFragDataLocationIndexed'],
  'arguments':
      'GLuint program, GLuint colorNumber, GLuint index, const char* name', },
{ 'return_type': 'void',
  'names': ['glBindFramebufferEXT', 'glBindFramebuffer'],
  'arguments': 'GLenum target, GLuint framebuffer', },
{ 'return_type': 'void',
  'names': ['glBindRenderbufferEXT', 'glBindRenderbuffer'],
  'arguments': 'GLenum target, GLuint renderbuffer', },
{ 'return_type': 'void',
  'names': ['glBindTexture'],
  'arguments': 'GLenum target, GLuint texture', },
{ 'return_type': 'void',
  'names': ['glBlendColor'],
  'arguments': 'GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha', },
{ 'return_type': 'void',
  'names': ['glBlendEquation'],
  'arguments': ' GLenum mode ', },
{ 'return_type': 'void',
  'names': ['glBlendEquationSeparate'],
  'arguments': 'GLenum modeRGB, GLenum modeAlpha', },
{ 'return_type': 'void',
  'names': ['glBlendFunc'],
  'arguments': 'GLenum sfactor, GLenum dfactor', },
{ 'return_type': 'void',
  'names': ['glBlendFuncSeparate'],
  'arguments':
      'GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha', },
{ 'return_type': 'void',
  'names': ['glBlitFramebufferEXT', 'glBlitFramebuffer'],
  'arguments': 'GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, '
               'GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, '
               'GLbitfield mask, GLenum filter', },
{ 'return_type': 'void',
  'names': ['glBlitFramebufferANGLE', 'glBlitFramebuffer'],
  'arguments': 'GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, '
               'GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, '
               'GLbitfield mask, GLenum filter', },
{ 'return_type': 'void',
  'names': ['glBufferData'],
  'arguments': 'GLenum target, GLsizei size, const void* data, GLenum usage', },
{ 'return_type': 'void',
  'names': ['glBufferSubData'],
  'arguments': 'GLenum target, GLint offset, GLsizei size, const void* data', },
{ 'return_type': 'GLenum',
  'names': ['glCheckFramebufferStatusEXT',
            'glCheckFramebufferStatus'],
  'arguments': 'GLenum target',
  'logging_code': """
  GL_SERVICE_LOG("GL_RESULT: " << GLES2Util::GetStringEnum(result));
""", },
{ 'return_type': 'void',
  'names': ['glClear'],
  'arguments': 'GLbitfield mask', },
{ 'return_type': 'void',
  'names': ['glClearColor'],
  'arguments': 'GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha', },
{ 'return_type': 'void',
  'names': ['glClearDepth'],
  'arguments': 'GLclampd depth', },
{ 'return_type': 'void',
  'names': ['glClearDepthf'],
  'arguments': 'GLclampf depth', },
{ 'return_type': 'void',
  'names': ['glClearStencil'],
  'arguments': 'GLint s', },
{ 'return_type': 'void',
  'names': ['glColorMask'],
  'arguments':
      'GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha', },
{ 'return_type': 'void',
  'names': ['glCompileShader'],
  'arguments': 'GLuint shader', },
{ 'return_type': 'void',
  'names': ['glCompressedTexImage2D'],
  'arguments':
      'GLenum target, GLint level, GLenum internalformat, GLsizei width, '
      'GLsizei height, GLint border, GLsizei imageSize, const void* data', },
{ 'return_type': 'void',
  'names': ['glCompressedTexSubImage2D'],
  'arguments':
     'GLenum target, GLint level, GLint xoffset, GLint yoffset, '
     'GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, '
     'const void* data', },
{ 'return_type': 'void',
  'names': ['glCopyTexImage2D'],
  'arguments':
      'GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, '
      'GLsizei width, GLsizei height, GLint border', },
{ 'return_type': 'void',
  'names': ['glCopyTexSubImage2D'],
  'arguments':
      'GLenum target, GLint level, GLint xoffset, '
      'GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height', },
{ 'return_type': 'GLuint',
  'names': ['glCreateProgram'],
  'arguments': 'void', },
{ 'return_type': 'GLuint',
  'names': ['glCreateShader'],
  'arguments': 'GLenum type', },
{ 'return_type': 'void',
  'names': ['glCullFace'],
  'arguments': 'GLenum mode', },
{ 'return_type': 'void',
  'names': ['glDeleteBuffersARB', 'glDeleteBuffers'],
  'arguments': 'GLsizei n, const GLuint* buffers', },
{ 'return_type': 'void',
  'names': ['glDeleteFramebuffersEXT', 'glDeleteFramebuffers'],
  'arguments': 'GLsizei n, const GLuint* framebuffers', },
{ 'return_type': 'void',
  'names': ['glDeleteProgram'],
  'arguments': 'GLuint program', },
{ 'return_type': 'void',
  'names': ['glDeleteQueries'],
  'arguments': 'GLsizei n, const GLuint* ids', },
{ 'return_type': 'void',
  'names': ['glDeleteQueriesARB', 'glDeleteQueriesEXT'],
  'arguments': 'GLsizei n, const GLuint* ids', },
{ 'return_type': 'void',
  'names': ['glDeleteRenderbuffersEXT', 'glDeleteRenderbuffers'],
  'arguments': 'GLsizei n, const GLuint* renderbuffers', },
{ 'return_type': 'void',
  'names': ['glDeleteShader'],
  'arguments': 'GLuint shader', },
{ 'return_type': 'void',
  'names': ['glDeleteTextures'],
  'arguments': 'GLsizei n, const GLuint* textures', },
{ 'return_type': 'void',
  'names': ['glDepthFunc'],
  'arguments': 'GLenum func', },
{ 'return_type': 'void',
  'names': ['glDepthMask'],
  'arguments': 'GLboolean flag', },
{ 'return_type': 'void',
  'names': ['glDepthRange'],
  'arguments': 'GLclampd zNear, GLclampd zFar', },
{ 'return_type': 'void',
  'names': ['glDepthRangef'],
  'arguments': 'GLclampf zNear, GLclampf zFar', },
{ 'return_type': 'void',
  'names': ['glDetachShader'],
  'arguments': 'GLuint program, GLuint shader', },
{ 'return_type': 'void',
  'names': ['glDisable'],
  'arguments': 'GLenum cap', },
{ 'return_type': 'void',
  'names': ['glDisableVertexAttribArray'],
  'arguments': 'GLuint index', },
{ 'return_type': 'void',
  'names': ['glDrawArrays'],
  'arguments': 'GLenum mode, GLint first, GLsizei count', },
{ 'return_type': 'void',
  'names': ['glDrawBuffer'],
  'arguments': 'GLenum mode', },
{ 'return_type': 'void',
  'names': ['glDrawBuffersARB', 'glDrawBuffersEXT'],
  'arguments': 'GLsizei n, const GLenum* bufs', },
{ 'return_type': 'void',
  'names': ['glDrawElements'],
  'arguments':
      'GLenum mode, GLsizei count, GLenum type, const void* indices', },
{ 'return_type': 'void',
  'names': ['glEGLImageTargetTexture2DOES'],
  'arguments': 'GLenum target, GLeglImageOES image', },
{ 'return_type': 'void',
  'names': ['glEGLImageTargetRenderbufferStorageOES'],
  'arguments': 'GLenum target, GLeglImageOES image', },
{ 'return_type': 'void',
  'names': ['glEnable'],
  'arguments': 'GLenum cap', },
{ 'return_type': 'void',
  'names': ['glEnableVertexAttribArray'],
  'arguments': 'GLuint index', },
{ 'return_type': 'void',
  'names': ['glEndQuery'],
  'arguments': 'GLenum target', },
{ 'return_type': 'void',
  'names': ['glEndQueryARB', 'glEndQueryEXT'],
  'arguments': 'GLenum target', },
{ 'return_type': 'void',
  'names': ['glFinish'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['glFlush'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['glFramebufferRenderbufferEXT', 'glFramebufferRenderbuffer'],
  'arguments': \
      'GLenum target, GLenum attachment, GLenum renderbuffertarget, '
      'GLuint renderbuffer', },
{ 'return_type': 'void',
  'names': ['glFramebufferTexture2DEXT', 'glFramebufferTexture2D'],
  'arguments':
      'GLenum target, GLenum attachment, GLenum textarget, GLuint texture, '
      'GLint level', },
{ 'return_type': 'void',
  'names': ['glFrontFace'],
  'arguments': 'GLenum mode', },
{ 'return_type': 'void',
  'names': ['glGenBuffersARB', 'glGenBuffers'],
  'arguments': 'GLsizei n, GLuint* buffers', },
{ 'return_type': 'void',
  'names': ['glGenQueries'],
  'arguments': 'GLsizei n, GLuint* ids', },
{ 'return_type': 'void',
  'names': ['glGenQueriesARB', 'glGenQueriesEXT'],
  'arguments': 'GLsizei n, GLuint* ids', },
{ 'return_type': 'void',
  'names': ['glGenerateMipmapEXT', 'glGenerateMipmap'],
  'arguments': 'GLenum target', },
{ 'return_type': 'void',
  'names': ['glGenFramebuffersEXT', 'glGenFramebuffers'],
  'arguments': 'GLsizei n, GLuint* framebuffers', },
{ 'return_type': 'void',
  'names': ['glGenRenderbuffersEXT', 'glGenRenderbuffers'],
  'arguments': 'GLsizei n, GLuint* renderbuffers', },
{ 'return_type': 'void',
  'names': ['glGenTextures'],
  'arguments': 'GLsizei n, GLuint* textures', },
{ 'return_type': 'void',
  'names': ['glGetActiveAttrib'],
  'arguments':
      'GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, '
      'GLint* size, GLenum* type, char* name', },
{ 'return_type': 'void',
  'names': ['glGetActiveUniform'],
  'arguments':
      'GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, '
      'GLint* size, GLenum* type, char* name', },
{ 'return_type': 'void',
  'names': ['glGetAttachedShaders'],
  'arguments':
      'GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders', },
{ 'return_type': 'GLint',
  'names': ['glGetAttribLocation'],
  'arguments': 'GLuint program, const char* name', },
{ 'return_type': 'void',
  'names': ['glGetBooleanv'],
  'arguments': 'GLenum pname, GLboolean* params', },
{ 'return_type': 'void',
  'names': ['glGetBufferParameteriv'],
  'arguments': 'GLenum target, GLenum pname, GLint* params', },
{ 'return_type': 'GLenum',
  'names': ['glGetError'],
  'arguments': 'void',
  'logging_code': """
  GL_SERVICE_LOG("GL_RESULT: " << GLES2Util::GetStringError(result));
""", },
{ 'return_type': 'void',
  'names': ['glGetFloatv'],
  'arguments': 'GLenum pname, GLfloat* params', },
{ 'return_type': 'void',
  'names': ['glGetFramebufferAttachmentParameterivEXT',
            'glGetFramebufferAttachmentParameteriv'],
  'arguments': 'GLenum target, '
               'GLenum attachment, GLenum pname, GLint* params', },
{ 'return_type': 'GLenum',
  'names': ['glGetGraphicsResetStatusARB',
            'glGetGraphicsResetStatusEXT'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['glGetIntegerv'],
  'arguments': 'GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetProgramBinary', 'glGetProgramBinaryOES'],
  'arguments': 'GLuint program, GLsizei bufSize, GLsizei* length, '
               'GLenum* binaryFormat, GLvoid* binary',
  'other_extensions': ['ARB_get_program_binary',
                       'OES_get_program_binary'] },
{ 'return_type': 'void',
  'names': ['glGetProgramiv'],
  'arguments': 'GLuint program, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetProgramInfoLog'],
  'arguments':
      'GLuint program, GLsizei bufsize, GLsizei* length, char* infolog', },
{ 'return_type': 'void',
  'names': ['glGetQueryiv'],
  'arguments': 'GLenum target, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetQueryivARB', 'glGetQueryivEXT'],
  'arguments': 'GLenum target, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetQueryObjecti64v'],
  'arguments': 'GLuint id, GLenum pname, GLint64* params', },
{ 'return_type': 'void',
  'names': ['glGetQueryObjectiv'],
  'arguments': 'GLuint id, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetQueryObjectui64v'],
  'arguments': 'GLuint id, GLenum pname, GLuint64* params', },
{ 'return_type': 'void',
  'names': ['glGetQueryObjectuiv'],
  'arguments': 'GLuint id, GLenum pname, GLuint* params', },
{ 'return_type': 'void',
  'names': ['glGetQueryObjectuivARB', 'glGetQueryObjectuivEXT'],
  'arguments': 'GLuint id, GLenum pname, GLuint* params', },
{ 'return_type': 'void',
  'names': ['glGetRenderbufferParameterivEXT', 'glGetRenderbufferParameteriv'],
  'arguments': 'GLenum target, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetShaderiv'],
  'arguments': 'GLuint shader, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetShaderInfoLog'],
  'arguments':
      'GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog', },
{ 'return_type': 'void',
  'names': ['glGetShaderPrecisionFormat'],
  'arguments': 'GLenum shadertype, GLenum precisiontype, '
               'GLint* range, GLint* precision', },
{ 'return_type': 'void',
  'names': ['glGetShaderSource'],
  'arguments':
      'GLuint shader, GLsizei bufsize, GLsizei* length, char* source', },
{ 'return_type': 'const GLubyte*',
  'names': ['glGetString'],
  'arguments': 'GLenum name', },
{ 'return_type': 'void',
  'names': ['glGetTexLevelParameterfv'],
  'arguments': 'GLenum target, GLint level, GLenum pname, GLfloat* params', },
{ 'return_type': 'void',
  'names': ['glGetTexLevelParameteriv'],
  'arguments': 'GLenum target, GLint level, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetTexParameterfv'],
  'arguments': 'GLenum target, GLenum pname, GLfloat* params', },
{ 'return_type': 'void',
  'names': ['glGetTexParameteriv'],
  'arguments': 'GLenum target, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetTranslatedShaderSourceANGLE'],
  'arguments':
      'GLuint shader, GLsizei bufsize, GLsizei* length, char* source', },
{ 'return_type': 'void',
  'names': ['glGetUniformfv'],
  'arguments': 'GLuint program, GLint location, GLfloat* params', },
{ 'return_type': 'void',
  'names': ['glGetUniformiv'],
  'arguments': 'GLuint program, GLint location, GLint* params', },
{ 'return_type': 'GLint',
  'names': ['glGetUniformLocation'],
  'arguments': 'GLuint program, const char* name', },
{ 'return_type': 'void',
  'names': ['glGetVertexAttribfv'],
  'arguments': 'GLuint index, GLenum pname, GLfloat* params', },
{ 'return_type': 'void',
  'names': ['glGetVertexAttribiv'],
  'arguments': 'GLuint index, GLenum pname, GLint* params', },
{ 'return_type': 'void',
  'names': ['glGetVertexAttribPointerv'],
  'arguments': 'GLuint index, GLenum pname, void** pointer', },
{ 'return_type': 'void',
  'names': ['glHint'],
  'arguments': 'GLenum target, GLenum mode', },
{ 'return_type': 'GLboolean',
  'names': ['glIsBuffer'],
  'arguments': 'GLuint buffer', },
{ 'return_type': 'GLboolean',
  'names': ['glIsEnabled'],
  'arguments': 'GLenum cap', },
{ 'return_type': 'GLboolean',
  'names': ['glIsFramebufferEXT', 'glIsFramebuffer'],
  'arguments': 'GLuint framebuffer', },
{ 'return_type': 'GLboolean',
  'names': ['glIsProgram'],
  'arguments': 'GLuint program', },
{ 'return_type': 'GLboolean',
  'names': ['glIsQueryARB', 'glIsQueryEXT'],
  'arguments': 'GLuint query', },
{ 'return_type': 'GLboolean',
  'names': ['glIsRenderbufferEXT', 'glIsRenderbuffer'],
  'arguments': 'GLuint renderbuffer', },
{ 'return_type': 'GLboolean',
  'names': ['glIsShader'],
  'arguments': 'GLuint shader', },
{ 'return_type': 'GLboolean',
  'names': ['glIsTexture'],
  'arguments': 'GLuint texture', },
{ 'return_type': 'void',
  'names': ['glLineWidth'],
  'arguments': 'GLfloat width', },
{ 'return_type': 'void',
  'names': ['glLinkProgram'],
  'arguments': 'GLuint program', },
{ 'return_type': 'void*',
  'names': ['glMapBuffer', 'glMapBufferOES'],
  'arguments': 'GLenum target, GLenum access', },
{ 'return_type': 'void',
  'names': ['glPixelStorei'],
  'arguments': 'GLenum pname, GLint param', },
{ 'return_type': 'void',
  'names': ['glPointParameteri'],
  'arguments': 'GLenum pname, GLint param', },
{ 'return_type': 'void',
  'names': ['glPolygonOffset'],
  'arguments': 'GLfloat factor, GLfloat units', },
{ 'return_type': 'void',
  'names': ['glProgramBinary', 'glProgramBinaryOES'],
  'arguments': 'GLuint program, GLenum binaryFormat, '
               'const GLvoid* binary, GLsizei length',
  'other_extensions': ['ARB_get_program_binary',
                       'OES_get_program_binary'] },
{ 'return_type': 'void',
  'names': ['glProgramParameteri'],
  'arguments': 'GLuint program, GLenum pname, GLint value',
  'other_extensions': ['ARB_get_program_binary'] },
{ 'return_type': 'void',
  'names': ['glQueryCounter'],
  'arguments': 'GLuint id, GLenum target', },
{ 'return_type': 'void',
  'names': ['glReadBuffer'],
  'arguments': 'GLenum src', },
{ 'return_type': 'void',
  'names': ['glReadPixels'],
  'arguments':
    'GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, '
    'GLenum type, void* pixels', },
{ 'return_type': 'void',
  'names': ['glReleaseShaderCompiler'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['glRenderbufferStorageMultisampleEXT',
            'glRenderbufferStorageMultisample'],
  'arguments': 'GLenum target, GLsizei samples, GLenum internalformat, '
               'GLsizei width, GLsizei height', },
{ 'return_type': 'void',
  'names': ['glRenderbufferStorageMultisampleANGLE',
            'glRenderbufferStorageMultisample'],
  'arguments': 'GLenum target, GLsizei samples, GLenum internalformat, '
               'GLsizei width, GLsizei height', },
{ 'return_type': 'void',
  'names': ['glRenderbufferStorageEXT', 'glRenderbufferStorage'],
  'arguments':
      'GLenum target, GLenum internalformat, GLsizei width, GLsizei height', },
{ 'return_type': 'void',
  'names': ['glSampleCoverage'],
  'arguments': 'GLclampf value, GLboolean invert', },
{ 'return_type': 'void',
  'names': ['glScissor'],
  'arguments': 'GLint x, GLint y, GLsizei width, GLsizei height', },
{ 'return_type': 'void',
  'names': ['glShaderBinary'],
  'arguments': 'GLsizei n, const GLuint* shaders, GLenum binaryformat, '
               'const void* binary, GLsizei length', },
{ 'return_type': 'void',
  'names': ['glShaderSource'],
  'arguments':
      'GLuint shader, GLsizei count, const char* const* str, const GLint* length',
  'logging_code': """
  GL_SERVICE_LOG_CODE_BLOCK({
    for (GLsizei ii = 0; ii < count; ++ii) {
      if (str[ii]) {
        if (length && length[ii] >= 0) {
          std::string source(str[ii], length[ii]);
          GL_SERVICE_LOG("  " << ii << ": ---\\n" << source << "\\n---");
        } else {
          GL_SERVICE_LOG("  " << ii << ": ---\\n" << str[ii] << "\\n---");
        }
      } else {
        GL_SERVICE_LOG("  " << ii << ": NULL");
      }
    }
  });
""", },
{ 'return_type': 'void',
  'names': ['glStencilFunc'],
  'arguments': 'GLenum func, GLint ref, GLuint mask', },
{ 'return_type': 'void',
  'names': ['glStencilFuncSeparate'],
  'arguments': 'GLenum face, GLenum func, GLint ref, GLuint mask', },
{ 'return_type': 'void',
  'names': ['glStencilMask'],
  'arguments': 'GLuint mask', },
{ 'return_type': 'void',
  'names': ['glStencilMaskSeparate'],
  'arguments': 'GLenum face, GLuint mask', },
{ 'return_type': 'void',
  'names': ['glStencilOp'],
  'arguments': 'GLenum fail, GLenum zfail, GLenum zpass', },
{ 'return_type': 'void',
  'names': ['glStencilOpSeparate'],
  'arguments': 'GLenum face, GLenum fail, GLenum zfail, GLenum zpass', },
{ 'return_type': 'void',
  'names': ['glTexImage2D'],
  'arguments':
      'GLenum target, GLint level, GLint internalformat, GLsizei width, '
      'GLsizei height, GLint border, GLenum format, GLenum type, '
      'const void* pixels', },
{ 'return_type': 'void',
  'names': ['glTexParameterf'],
  'arguments': 'GLenum target, GLenum pname, GLfloat param', },
{ 'return_type': 'void',
  'names': ['glTexParameterfv'],
  'arguments': 'GLenum target, GLenum pname, const GLfloat* params', },
{ 'return_type': 'void',
  'names': ['glTexParameteri'],
  'arguments': 'GLenum target, GLenum pname, GLint param', },
{ 'return_type': 'void',
  'names': ['glTexParameteriv'],
  'arguments': 'GLenum target, GLenum pname, const GLint* params', },
{ 'return_type': 'void',
  'names': ['glTexStorage2DEXT'],
  'arguments': 'GLenum target, GLsizei levels, GLenum internalformat, '
               'GLsizei width, GLsizei height', },
{ 'return_type': 'void',
  'names': ['glTexSubImage2D'],
  'arguments':
     'GLenum target, GLint level, GLint xoffset, GLint yoffset, '
     'GLsizei width, GLsizei height, GLenum format, GLenum type, '
     'const void* pixels', },
{ 'return_type': 'void',
  'names': ['glUniform1f'],
  'arguments': 'GLint location, GLfloat x', },
{ 'return_type': 'void',
  'names': ['glUniform1fv'],
  'arguments': 'GLint location, GLsizei count, const GLfloat* v', },
{ 'return_type': 'void',
  'names': ['glUniform1i'],
  'arguments': 'GLint location, GLint x', },
{ 'return_type': 'void',
  'names': ['glUniform1iv'],
  'arguments': 'GLint location, GLsizei count, const GLint* v', },
{ 'return_type': 'void',
  'names': ['glUniform2f'],
  'arguments': 'GLint location, GLfloat x, GLfloat y', },
{ 'return_type': 'void',
  'names': ['glUniform2fv'],
  'arguments': 'GLint location, GLsizei count, const GLfloat* v', },
{ 'return_type': 'void',
  'names': ['glUniform2i'],
  'arguments': 'GLint location, GLint x, GLint y', },
{ 'return_type': 'void',
  'names': ['glUniform2iv'],
  'arguments': 'GLint location, GLsizei count, const GLint* v', },
{ 'return_type': 'void',
  'names': ['glUniform3f'],
  'arguments': 'GLint location, GLfloat x, GLfloat y, GLfloat z', },
{ 'return_type': 'void',
  'names': ['glUniform3fv'],
  'arguments': 'GLint location, GLsizei count, const GLfloat* v', },
{ 'return_type': 'void',
  'names': ['glUniform3i'],
  'arguments': 'GLint location, GLint x, GLint y, GLint z', },
{ 'return_type': 'void',
  'names': ['glUniform3iv'],
  'arguments': 'GLint location, GLsizei count, const GLint* v', },
{ 'return_type': 'void',
  'names': ['glUniform4f'],
  'arguments': 'GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w', },
{ 'return_type': 'void',
  'names': ['glUniform4fv'],
  'arguments': 'GLint location, GLsizei count, const GLfloat* v', },
{ 'return_type': 'void',
  'names': ['glUniform4i'],
  'arguments': 'GLint location, GLint x, GLint y, GLint z, GLint w', },
{ 'return_type': 'void',
  'names': ['glUniform4iv'],
  'arguments': 'GLint location, GLsizei count, const GLint* v', },
{ 'return_type': 'void',
  'names': ['glUniformMatrix2fv'],
  'arguments': 'GLint location, GLsizei count, '
               'GLboolean transpose, const GLfloat* value', },
{ 'return_type': 'void',
  'names': ['glUniformMatrix3fv'],
  'arguments': 'GLint location, GLsizei count, '
               'GLboolean transpose, const GLfloat* value', },
{ 'return_type': 'void',
  'names': ['glUniformMatrix4fv'],
  'arguments': 'GLint location, GLsizei count, '
               'GLboolean transpose, const GLfloat* value', },
{ 'return_type': 'GLboolean',
  'names': ['glUnmapBuffer', 'glUnmapBufferOES'],
  'arguments': 'GLenum target', },
{ 'return_type': 'void',
  'names': ['glUseProgram'],
  'arguments': 'GLuint program', },
{ 'return_type': 'void',
  'names': ['glValidateProgram'],
  'arguments': 'GLuint program', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib1f'],
  'arguments': 'GLuint indx, GLfloat x', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib1fv'],
  'arguments': 'GLuint indx, const GLfloat* values', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib2f'],
  'arguments': 'GLuint indx, GLfloat x, GLfloat y', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib2fv'],
  'arguments': 'GLuint indx, const GLfloat* values', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib3f'],
  'arguments': 'GLuint indx, GLfloat x, GLfloat y, GLfloat z', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib3fv'],
  'arguments': 'GLuint indx, const GLfloat* values', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib4f'],
  'arguments': 'GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w', },
{ 'return_type': 'void',
  'names': ['glVertexAttrib4fv'],
  'arguments': 'GLuint indx, const GLfloat* values', },
{ 'return_type': 'void',
  'names': ['glVertexAttribPointer'],
  'arguments': 'GLuint indx, GLint size, GLenum type, GLboolean normalized, '
               'GLsizei stride, const void* ptr', },
{ 'return_type': 'void',
  'names': ['glViewport'],
  'arguments': 'GLint x, GLint y, GLsizei width, GLsizei height', },
{ 'return_type': 'void',
  'names': ['glGenFencesNV'],
  'arguments': 'GLsizei n, GLuint* fences', },
{ 'return_type': 'void',
  'names': ['glDeleteFencesNV'],
  'arguments': 'GLsizei n, const GLuint* fences', },
{ 'return_type': 'void',
  'names': ['glSetFenceNV'],
  'arguments': 'GLuint fence, GLenum condition', },
{ 'return_type': 'GLboolean',
  'names': ['glTestFenceNV'],
  'arguments': 'GLuint fence', },
{ 'return_type': 'void',
  'names': ['glFinishFenceNV'],
  'arguments': 'GLuint fence', },
{ 'return_type': 'GLboolean',
  'names': ['glIsFenceNV'],
  'arguments': 'GLuint fence', },
{ 'return_type': 'void',
  'names': ['glGetFenceivNV'],
  'arguments': 'GLuint fence, GLenum pname, GLint* params', },
{ 'return_type': 'GLsync',
  'names': ['glFenceSync'],
  'arguments': 'GLenum condition, GLbitfield flags', },
{ 'return_type': 'void',
  'names': ['glDeleteSync'],
  'arguments': 'GLsync sync', },
{ 'return_type': 'void',
  'names': ['glGetSynciv'],
  'arguments':
    'GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length,'
    'GLint* values', },
{ 'return_type': 'void',
  'names': ['glDrawArraysInstancedANGLE', 'glDrawArraysInstancedARB'],
  'arguments': 'GLenum mode, GLint first, GLsizei count, GLsizei primcount', },
{ 'return_type': 'void',
  'names': ['glDrawElementsInstancedANGLE', 'glDrawElementsInstancedARB'],
  'arguments':
      'GLenum mode, GLsizei count, GLenum type, const void* indices, '
      'GLsizei primcount', },
{ 'return_type': 'void',
  'names': ['glVertexAttribDivisorANGLE', 'glVertexAttribDivisorARB'],
  'arguments':
      'GLuint index, GLuint divisor', },
{ 'return_type': 'void',
  'names': ['glGenVertexArraysOES',
            'glGenVertexArraysAPPLE',
            'glGenVertexArrays'],
  'arguments': 'GLsizei n, GLuint* arrays',
  'other_extensions': ['OES_vertex_array_object',
                       'APPLE_vertex_array_object',
                       'ARB_vertex_array_object'] },
{ 'return_type': 'void',
  'names': ['glDeleteVertexArraysOES',
            'glDeleteVertexArraysAPPLE',
            'glDeleteVertexArrays'],
  'arguments': 'GLsizei n, const GLuint* arrays',
  'other_extensions': ['OES_vertex_array_object',
                       'APPLE_vertex_array_object',
                       'ARB_vertex_array_object'] },
{ 'return_type': 'void',
  'names': ['glBindVertexArrayOES',
            'glBindVertexArrayAPPLE',
            'glBindVertexArray'],
  'arguments': 'GLuint array',
  'other_extensions': ['OES_vertex_array_object',
                       'APPLE_vertex_array_object',
                       'ARB_vertex_array_object'] },
{ 'return_type': 'GLboolean',
  'names': ['glIsVertexArrayOES',
            'glIsVertexArrayAPPLE',
            'glIsVertexArray'],
  'arguments': 'GLuint array',
  'other_extensions': ['OES_vertex_array_object',
                       'APPLE_vertex_array_object',
                       'ARB_vertex_array_object'] },
{ 'return_type': 'void',
  'names': ['glDiscardFramebufferEXT'],
  'arguments': 'GLenum target, GLsizei numAttachments, '
      'const GLenum* attachments' },
]

OSMESA_FUNCTIONS = [
{ 'return_type': 'OSMesaContext',
  'names': ['OSMesaCreateContext'],
  'arguments': 'GLenum format, OSMesaContext sharelist', },
{ 'return_type': 'OSMesaContext',
  'names': ['OSMesaCreateContextExt'],
  'arguments':
      'GLenum format, GLint depthBits, GLint stencilBits, GLint accumBits, '
      'OSMesaContext sharelist', },
{ 'return_type': 'void',
  'names': ['OSMesaDestroyContext'],
  'arguments': 'OSMesaContext ctx', },
{ 'return_type': 'GLboolean',
  'names': ['OSMesaMakeCurrent'],
  'arguments': 'OSMesaContext ctx, void* buffer, GLenum type, GLsizei width, '
               'GLsizei height', },
{ 'return_type': 'OSMesaContext',
  'names': ['OSMesaGetCurrentContext'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['OSMesaPixelStore'],
  'arguments': 'GLint pname, GLint value', },
{ 'return_type': 'void',
  'names': ['OSMesaGetIntegerv'],
  'arguments': 'GLint pname, GLint* value', },
{ 'return_type': 'GLboolean',
  'names': ['OSMesaGetDepthBuffer'],
  'arguments':
      'OSMesaContext c, GLint* width, GLint* height, GLint* bytesPerValue, '
      'void** buffer', },
{ 'return_type': 'GLboolean',
  'names': ['OSMesaGetColorBuffer'],
  'arguments': 'OSMesaContext c, GLint* width, GLint* height, GLint* format, '
               'void** buffer', },
{ 'return_type': 'OSMESAproc',
  'names': ['OSMesaGetProcAddress'],
  'arguments': 'const char* funcName', },
{ 'return_type': 'void',
  'names': ['OSMesaColorClamp'],
  'arguments': 'GLboolean enable', },
]

EGL_FUNCTIONS = [
{ 'return_type': 'EGLint',
  'names': ['eglGetError'],
  'arguments': 'void', },
{ 'return_type': 'EGLDisplay',
  'names': ['eglGetDisplay'],
  'arguments': 'EGLNativeDisplayType display_id', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglInitialize'],
  'arguments': 'EGLDisplay dpy, EGLint* major, EGLint* minor', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglTerminate'],
  'arguments': 'EGLDisplay dpy', },
{ 'return_type': 'const char*',
  'names': ['eglQueryString'],
  'arguments': 'EGLDisplay dpy, EGLint name', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglGetConfigs'],
  'arguments': 'EGLDisplay dpy, EGLConfig* configs, EGLint config_size, '
               'EGLint* num_config', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglChooseConfig'],
  'arguments': 'EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, '
               'EGLint config_size, EGLint* num_config', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglGetConfigAttrib'],
  'arguments':
      'EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint* value', },
{ 'return_type': 'EGLImageKHR',
  'names': ['eglCreateImageKHR'],
  'arguments':
      'EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, '
      'const EGLint* attrib_list',
  'other_extensions': ['EGL_KHR_image_base'] },
{ 'return_type': 'EGLBoolean',
  'names': ['eglDestroyImageKHR'],
  'arguments': 'EGLDisplay dpy, EGLImageKHR image',
  'other_extensions': ['EGL_KHR_image_base'] },
{ 'return_type': 'EGLSurface',
  'names': ['eglCreateWindowSurface'],
  'arguments': 'EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, '
               'const EGLint* attrib_list', },
{ 'return_type': 'EGLSurface',
  'names': ['eglCreatePbufferSurface'],
  'arguments': 'EGLDisplay dpy, EGLConfig config, const EGLint* attrib_list', },
{ 'return_type': 'EGLSurface',
  'names': ['eglCreatePixmapSurface'],
  'arguments': 'EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, '
               'const EGLint* attrib_list', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglDestroySurface'],
  'arguments': 'EGLDisplay dpy, EGLSurface surface', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglQuerySurface'],
  'arguments':
      'EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint* value', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglBindAPI'],
  'arguments': 'EGLenum api', },
{ 'return_type': 'EGLenum',
  'names': ['eglQueryAPI'],
  'arguments': 'void', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglWaitClient'],
  'arguments': 'void', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglReleaseThread'],
  'arguments': 'void', },
{ 'return_type': 'EGLSurface',
  'names': ['eglCreatePbufferFromClientBuffer'],
  'arguments':
      'EGLDisplay dpy, EGLenum buftype, void* buffer, EGLConfig config, '
      'const EGLint* attrib_list', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglSurfaceAttrib'],
  'arguments':
      'EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglBindTexImage'],
  'arguments': 'EGLDisplay dpy, EGLSurface surface, EGLint buffer', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglReleaseTexImage'],
  'arguments': 'EGLDisplay dpy, EGLSurface surface, EGLint buffer', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglSwapInterval'],
  'arguments': 'EGLDisplay dpy, EGLint interval', },
{ 'return_type': 'EGLContext',
  'names': ['eglCreateContext'],
  'arguments': 'EGLDisplay dpy, EGLConfig config, EGLContext share_context, '
              'const EGLint* attrib_list', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglDestroyContext'],
  'arguments': 'EGLDisplay dpy, EGLContext ctx', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglMakeCurrent'],
  'arguments':
      'EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx', },
{ 'return_type': 'EGLContext',
  'names': ['eglGetCurrentContext'],
  'arguments': 'void', },
{ 'return_type': 'EGLSurface',
  'names': ['eglGetCurrentSurface'],
  'arguments': 'EGLint readdraw', },
{ 'return_type': 'EGLDisplay',
  'names': ['eglGetCurrentDisplay'],
  'arguments': 'void', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglQueryContext'],
  'arguments':
      'EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglWaitGL'],
  'arguments': 'void', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglWaitNative'],
  'arguments': 'EGLint engine', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglSwapBuffers'],
  'arguments': 'EGLDisplay dpy, EGLSurface surface', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglCopyBuffers'],
  'arguments':
      'EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target', },
{ 'return_type': '__eglMustCastToProperFunctionPointerType',
  'names': ['eglGetProcAddress'],
  'arguments': 'const char* procname', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglPostSubBufferNV'],
  'arguments': 'EGLDisplay dpy, EGLSurface surface, '
    'EGLint x, EGLint y, EGLint width, EGLint height', },
{ 'return_type': 'EGLBoolean',
  'names': ['eglQuerySurfacePointerANGLE'],
  'arguments':
      'EGLDisplay dpy, EGLSurface surface, EGLint attribute, void** value', },
{ 'return_type': 'EGLSyncKHR',
  'names': ['eglCreateSyncKHR'],
  'arguments': 'EGLDisplay dpy, EGLenum type, const EGLint* attrib_list',
  'other_extensions': ['EGL_KHR_fence_sync'] },
{ 'return_type': 'EGLint',
  'names': ['eglClientWaitSyncKHR'],
  'arguments': 'EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, '
      'EGLTimeKHR timeout',
  'other_extensions': ['EGL_KHR_fence_sync'] },
{ 'return_type': 'EGLBoolean',
  'names': ['eglGetSyncAttribKHR'],
  'arguments': 'EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, '
      'EGLint* value',
  'other_extensions': ['EGL_KHR_fence_sync'] },
{ 'return_type': 'EGLBoolean',
  'names': ['eglDestroySyncKHR'],
  'arguments': 'EGLDisplay dpy, EGLSyncKHR sync',
  'other_extensions': ['EGL_KHR_fence_sync'] },
{ 'return_type': 'EGLBoolean',
  'names': ['eglGetSyncValuesCHROMIUM'],
  'arguments':
      'EGLDisplay dpy, EGLSurface surface, '
      'EGLuint64CHROMIUM* ust, EGLuint64CHROMIUM* msc, '
      'EGLuint64CHROMIUM* sbc', },
]

WGL_FUNCTIONS = [
{ 'return_type': 'HGLRC',
  'names': ['wglCreateContext'],
  'arguments': 'HDC hdc', },
{ 'return_type': 'HGLRC',
  'names': ['wglCreateLayerContext'],
  'arguments': 'HDC hdc, int iLayerPlane', },
{ 'return_type': 'BOOL',
  'names': ['wglCopyContext'],
  'arguments': 'HGLRC hglrcSrc, HGLRC hglrcDst, UINT mask', },
{ 'return_type': 'BOOL',
  'names': ['wglDeleteContext'],
  'arguments': 'HGLRC hglrc', },
{ 'return_type': 'HGLRC',
  'names': ['wglGetCurrentContext'],
  'arguments': '', },
{ 'return_type': 'HDC',
  'names': ['wglGetCurrentDC'],
  'arguments': '', },
{ 'return_type': 'BOOL',
  'names': ['wglMakeCurrent'],
  'arguments': 'HDC hdc, HGLRC hglrc', },
{ 'return_type': 'BOOL',
  'names': ['wglShareLists'],
  'arguments': 'HGLRC hglrc1, HGLRC hglrc2', },
{ 'return_type': 'BOOL',
  'names': ['wglSwapIntervalEXT'],
  'arguments': 'int interval', },
{ 'return_type': 'BOOL',
  'names': ['wglSwapLayerBuffers'],
  'arguments': 'HDC hdc, UINT fuPlanes', },
{ 'return_type': 'const char*',
  'names': ['wglGetExtensionsStringARB'],
  'arguments': 'HDC hDC', },
{ 'return_type': 'const char*',
  'names': ['wglGetExtensionsStringEXT'],
  'arguments': '', },
{ 'return_type': 'BOOL',
  'names': ['wglChoosePixelFormatARB'],
  'arguments':
      'HDC dc, const int* int_attrib_list, const float* float_attrib_list, '
      'UINT max_formats, int* formats, UINT* num_formats', },
{ 'return_type': 'HPBUFFERARB',
  'names': ['wglCreatePbufferARB'],
  'arguments': 'HDC hDC, int iPixelFormat, int iWidth, int iHeight, '
               'const int* piAttribList', },
{ 'return_type': 'HDC',
  'names': ['wglGetPbufferDCARB'],
  'arguments': 'HPBUFFERARB hPbuffer', },
{ 'return_type': 'int',
  'names': ['wglReleasePbufferDCARB'],
  'arguments': 'HPBUFFERARB hPbuffer, HDC hDC', },
{ 'return_type': 'BOOL',
  'names': ['wglDestroyPbufferARB'],
  'arguments': 'HPBUFFERARB hPbuffer', },
{ 'return_type': 'BOOL',
  'names': ['wglQueryPbufferARB'],
  'arguments': 'HPBUFFERARB hPbuffer, int iAttribute, int* piValue', },
]

GLX_FUNCTIONS = [
{ 'return_type': 'int',
  'names': ['glXWaitVideoSyncSGI'],
  'arguments': 'int divisor, int remainder, unsigned int* count', },
{ 'return_type': 'XVisualInfo*',
  'names': ['glXChooseVisual'],
  'arguments': 'Display* dpy, int screen, int* attribList', },
{ 'return_type': 'void',
  'names': ['glXCopySubBufferMESA'],
  'arguments': 'Display* dpy, GLXDrawable drawable, '
               'int x, int y, int width, int height', },
{ 'return_type': 'GLXContext',
  'names': ['glXCreateContext'],
  'arguments':
      'Display* dpy, XVisualInfo* vis, GLXContext shareList, int direct', },
{ 'return_type': 'void',
  'names': ['glXBindTexImageEXT'],
  'arguments':
      'Display* dpy, GLXDrawable drawable, int buffer, int* attribList', },
{ 'return_type': 'void',
  'names': ['glXReleaseTexImageEXT'],
  'arguments': 'Display* dpy, GLXDrawable drawable, int buffer', },
{ 'return_type': 'void',
  'names': ['glXDestroyContext'],
  'arguments': 'Display* dpy, GLXContext ctx', },
{ 'return_type': 'int',
  'names': ['glXMakeCurrent'],
  'arguments': 'Display* dpy, GLXDrawable drawable, GLXContext ctx', },
{ 'return_type': 'void',
  'names': ['glXCopyContext'],
  'arguments':
      'Display* dpy, GLXContext src, GLXContext dst, unsigned long mask', },
{ 'return_type': 'void',
  'names': ['glXSwapBuffers'],
  'arguments': 'Display* dpy, GLXDrawable drawable', },
{ 'return_type': 'GLXPixmap',
  'names': ['glXCreateGLXPixmap'],
  'arguments': 'Display* dpy, XVisualInfo* visual, Pixmap pixmap', },
{ 'return_type': 'void',
  'names': ['glXDestroyGLXPixmap'],
  'arguments': 'Display* dpy, GLXPixmap pixmap', },
{ 'return_type': 'int',
  'names': ['glXQueryExtension'],
  'arguments': 'Display* dpy, int* errorb, int* event', },
{ 'return_type': 'int',
  'names': ['glXQueryVersion'],
  'arguments': 'Display* dpy, int* maj, int* min', },
{ 'return_type': 'int',
  'names': ['glXIsDirect'],
  'arguments': 'Display* dpy, GLXContext ctx', },
{ 'return_type': 'int',
  'names': ['glXGetConfig'],
  'arguments': 'Display* dpy, XVisualInfo* visual, int attrib, int* value', },
{ 'return_type': 'GLXContext',
  'names': ['glXGetCurrentContext'],
  'arguments': 'void', },
{ 'return_type': 'GLXDrawable',
  'names': ['glXGetCurrentDrawable'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['glXWaitGL'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['glXWaitX'],
  'arguments': 'void', },
{ 'return_type': 'void',
  'names': ['glXUseXFont'],
  'arguments': 'Font font, int first, int count, int list', },
{ 'return_type': 'const char*',
  'names': ['glXQueryExtensionsString'],
  'arguments': 'Display* dpy, int screen', },
{ 'return_type': 'const char*',
  'names': ['glXQueryServerString'],
  'arguments': 'Display* dpy, int screen, int name', },
{ 'return_type': 'const char*',
  'names': ['glXGetClientString'],
  'arguments': 'Display* dpy, int name', },
{ 'return_type': 'Display*',
  'names': ['glXGetCurrentDisplay'],
  'arguments': 'void', },
{ 'return_type': 'GLXFBConfig*',
  'names': ['glXChooseFBConfig'],
  'arguments':
      'Display* dpy, int screen, const int* attribList, int* nitems', },
{ 'return_type': 'int',
  'names': ['glXGetFBConfigAttrib'],
  'arguments': 'Display* dpy, GLXFBConfig config, int attribute, int* value', },
{ 'return_type': 'GLXFBConfig*',
  'names': ['glXGetFBConfigs'],
  'arguments': 'Display* dpy, int screen, int* nelements', },
{ 'return_type': 'XVisualInfo*',
  'names': ['glXGetVisualFromFBConfig'],
  'arguments': 'Display* dpy, GLXFBConfig config', },
{ 'return_type': 'GLXWindow',
  'names': ['glXCreateWindow'],
  'arguments':
      'Display* dpy, GLXFBConfig config, Window win, const int* attribList', },
{ 'return_type': 'void',
  'names': ['glXDestroyWindow'],
  'arguments': 'Display* dpy, GLXWindow window', },
{ 'return_type': 'GLXPixmap',
  'names': ['glXCreatePixmap'],
  'arguments': 'Display* dpy, GLXFBConfig config, '
               'Pixmap pixmap, const int* attribList', },
{ 'return_type': 'void',
  'names': ['glXDestroyPixmap'],
  'arguments': 'Display* dpy, GLXPixmap pixmap', },
{ 'return_type': 'GLXPbuffer',
  'names': ['glXCreatePbuffer'],
  'arguments': 'Display* dpy, GLXFBConfig config, const int* attribList', },
{ 'return_type': 'void',
  'names': ['glXDestroyPbuffer'],
  'arguments': 'Display* dpy, GLXPbuffer pbuf', },
{ 'return_type': 'void',
  'names': ['glXQueryDrawable'],
  'arguments':
      'Display* dpy, GLXDrawable draw, int attribute, unsigned int* value', },
{ 'return_type': 'GLXContext',
  'names': ['glXCreateNewContext'],
  'arguments': 'Display* dpy, GLXFBConfig config, int renderType, '
               'GLXContext shareList, int direct', },
{ 'return_type': 'int',
  'names': ['glXMakeContextCurrent'],
  'arguments':
      'Display* dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx', },
{ 'return_type': 'GLXDrawable',
  'names': ['glXGetCurrentReadDrawable'],
  'arguments': 'void', },
{ 'return_type': 'int',
  'names': ['glXQueryContext'],
  'arguments': 'Display* dpy, GLXContext ctx, int attribute, int* value', },
{ 'return_type': 'void',
  'names': ['glXSelectEvent'],
  'arguments': 'Display* dpy, GLXDrawable drawable, unsigned long mask', },
{ 'return_type': 'void',
  'names': ['glXGetSelectedEvent'],
  'arguments': 'Display* dpy, GLXDrawable drawable, unsigned long* mask', },
{ 'return_type': 'void',
  'names': ['glXSwapIntervalMESA'],
  'arguments': 'unsigned int interval', },
{ 'return_type': 'void',
  'names': ['glXSwapIntervalEXT'],
  'arguments': 'Display* dpy, GLXDrawable drawable, int interval', },
{ 'return_type': 'GLXFBConfig',
  'names': ['glXGetFBConfigFromVisualSGIX'],
  'arguments': 'Display* dpy, XVisualInfo* visualInfo', },
{ 'return_type': 'GLXContext',
  'names': ['glXCreateContextAttribsARB'],
  'arguments':
      'Display* dpy, GLXFBConfig config, GLXContext share_context, int direct, '
      'const int* attrib_list', },
{ 'return_type': 'bool',
  'names': ['glXGetSyncValuesOML'],
  'arguments':
      'Display* dpy, GLXDrawable drawable, int64* ust, int64* msc, '
      'int64* sbc' },
{ 'return_type': 'bool',
  'names': ['glXGetMscRateOML'],
  'arguments':
      'Display* dpy, GLXDrawable drawable, int32* numerator, '
      'int32* denominator' },
]

FUNCTION_SETS = [
  [GL_FUNCTIONS, 'gl', [
      'GL/glext.h',
      'GLES2/gl2ext.h',
      # Files below are Chromium-specific and shipped with Chromium sources.
      'GL/glextchromium.h',
      'GLES2/gl2chromium.h',
      'GLES2/gl2extchromium.h'
  ], []],
  [OSMESA_FUNCTIONS, 'osmesa', [], []],
  [EGL_FUNCTIONS, 'egl', [
      'EGL/eglext.h',
      # Files below are Chromium-specific and shipped with Chromium sources.
      'EGL/eglextchromium.h',
    ],
    [
      'EGL_ANGLE_d3d_share_handle_client_buffer',
      'EGL_ANGLE_surface_d3d_texture_2d_share_handle',
    ],
  ],
  [WGL_FUNCTIONS, 'wgl', ['GL/wglext.h'], []],
  [GLX_FUNCTIONS, 'glx', ['GL/glx.h', 'GL/glxext.h'], []],
]

def GenerateHeader(file, functions, set_name, used_extension_functions):
  """Generates gl_bindings_autogen_x.h"""

  # Write file header.
  file.write(
"""// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is automatically generated.

#ifndef UI_GFX_GL_GL_BINDINGS_AUTOGEN_%(name)s_H_
#define UI_GFX_GL_GL_BINDINGS_AUTOGEN_%(name)s_H_

namespace gfx {

class GLContext;

""" % {'name': set_name.upper()})

  # Write typedefs for function pointer types. Always use the GL name for the
  # typedef.
  file.write('\n')
  for func in functions:
    file.write('typedef %s (GL_BINDING_CALL *%sProc)(%s);\n' %
        (func['return_type'], func['names'][0], func['arguments']))

  # Write declarations for booleans indicating which extensions are available.
  file.write('\n')
  file.write("struct Extensions%s {\n" % set_name.upper())
  for extension, ext_functions in used_extension_functions:
    file.write('  bool b_%s;\n' % extension)
  file.write('};\n')
  file.write('\n')

  # Write Procs struct.
  file.write("struct Procs%s {\n" % set_name.upper())
  for func in functions:
    file.write('  %sProc %sFn;\n' % (func['names'][0], func['names'][0]))
  file.write('};\n')
  file.write('\n')

  # Write Driver struct.
  file.write(
"""struct GL_EXPORT Driver%(name)s {
  void InitializeBindings();
  void InitializeExtensionBindings(GLContext* context);
  void InitializeDebugBindings();
  void ClearBindings();
  void UpdateDebugExtensionBindings();

  Procs%(name)s fn;
  Procs%(name)s debug_fn;
  Extensions%(name)s ext;
""" % {'name': set_name.upper()})
  file.write('};\n')
  file.write('\n')

  # Write Api class.
  file.write(
"""class GL_EXPORT %(name)sApi {
 public:
  %(name)sApi();
  virtual ~%(name)sApi();

""" % {'name': set_name.upper()})
  for func in functions:
    file.write('  virtual %s %sFn(%s) = 0;\n' %
      (func['return_type'], func['names'][0], func['arguments']))
  file.write('};\n')
  file.write('\n')

  file.write( '}  // namespace gfx\n')

  # Write macros to invoke function pointers. Always use the GL name for the
  # macro.
  file.write('\n')
  for func in functions:
    file.write('#define %s ::gfx::g_current_%s_context->%sFn\n' %
        (func['names'][0], set_name.lower(), func['names'][0]))

  file.write('\n')
  file.write('#endif  //  UI_GFX_GL_GL_BINDINGS_AUTOGEN_%s_H_\n' %
      set_name.upper())


def GenerateAPIHeader(file, functions, set_name, used_extension_functions):
  """Generates gl_bindings_api_autogen_x.h"""

  # Write file header.
  file.write(
"""// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is automatically generated.

""" % {'name': set_name.upper()})

  # Write API declaration.
  for func in functions:
    file.write('  virtual %s %sFn(%s) OVERRIDE;\n' %
      (func['return_type'], func['names'][0], func['arguments']))

  file.write('\n')


def GenerateMockHeader(file, functions, set_name, used_extension_functions):
  """Generates gl_mock_autogen_x.h"""

  # Write file header.
  file.write(
"""// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is automatically generated.

""" % {'name': set_name.upper()})

  # Write API declaration.
  for func in functions:
    args = func['arguments']
    if args == 'void':
      args = ''
    arg_count = 0
    if len(args):
      arg_count = func['arguments'].count(',') + 1
    file.write('  MOCK_METHOD%d(%s, %s(%s));\n' %
      (arg_count, func['names'][0][2:], func['return_type'], args))

  file.write('\n')


def GenerateInterfaceHeader(
    file, functions, set_name, used_extension_functions):
  """Generates gl_interface_autogen_x.h"""

  # Write file header.
  file.write(
"""// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is automatically generated.

""" % {'name': set_name.upper()})

  # Write API declaration.
  for func in functions:
    args = func['arguments']
    if args == 'void':
      args = ''
    file.write('  virtual %s %s(%s) = 0;\n' %
      (func['return_type'], func['names'][0][2:], args))

  file.write('\n')


def GenerateSource(file, functions, set_name, used_extension_functions):
  """Generates gl_bindings_autogen_x.cc"""

  # Write file header.
  file.write(
"""// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is automatically generated.

#include <string>
#include "base/debug/trace_event.h"
#include "gpu/command_buffer/common/gles2_cmd_utils.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_%s_api_implementation.h"

using gpu::gles2::GLES2Util;

namespace gfx {
""" % set_name.lower())

  # Write definitions of function pointers.
  file.write('\n')
  file.write('static bool g_debugBindingsInitialized;\n')
  file.write('Driver%s g_driver_%s;\n' % (set_name.upper(), set_name.lower()))
  file.write('\n')

  # Write function to initialize the core function pointers. The code assumes
  # any non-NULL pointer returned by GetGLCoreProcAddress() is valid, although
  # it may be overwritten by an extension function pointer later.
  file.write('\n')
  file.write('void Driver%s::InitializeBindings() {\n' %
             set_name.upper())
  for func in functions:
    first_name = func['names'][0]
    for i, name in enumerate(func['names']):
      if i:
        file.write('  if (!fn.%sFn)\n  ' % first_name)
      file.write(
          '  fn.%sFn = reinterpret_cast<%sProc>('
          'GetGLCoreProcAddress("%s"));\n' %
          (first_name, first_name, name))
  file.write('}\n')
  file.write('\n')

  # Write function to initialize the extension function pointers. This function
  # uses a current context to query which extensions are actually supported.
  file.write("""void Driver%s::InitializeExtensionBindings(
    GLContext* context) {
""" % set_name.upper())
  file.write('  DCHECK(context && context->IsCurrent(NULL));\n')
  for extension, ext_functions in used_extension_functions:
    file.write('  ext.b_%s = context->HasExtension("%s");\n' %
        (extension, extension))
    file.write('  if (ext.b_%s) {\n' %
        (extension))
    queried_entry_points = set()
    for entry_point_name, function_name in ext_functions:
      # Replace the pointer unconditionally unless this extension has several
      # alternatives for the same entry point (e.g.,
      # GL_ARB_blend_func_extended).
      if entry_point_name in queried_entry_points:
        file.write('    if (!fn.%sFn)\n  ' % entry_point_name)
      file.write(
         '    fn.%sFn = reinterpret_cast<%sProc>(GetGLProcAddress("%s"));\n' %
             (entry_point_name, entry_point_name, function_name))
      queried_entry_points.add(entry_point_name)
    file.write('  }\n')
  file.write('  if (g_debugBindingsInitialized)\n')
  file.write('    UpdateDebugExtensionBindings();\n')
  file.write('}\n')
  file.write('\n')

  # Write logging wrappers for each function.
  file.write('extern "C" {\n')
  for func in functions:
    names = func['names']
    return_type = func['return_type']
    arguments = func['arguments']
    file.write('\n')
    file.write('static %s GL_BINDING_CALL Debug_%s(%s) {\n' %
        (return_type, names[0], arguments))
    argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2', arguments)
    argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2', argument_names)
    log_argument_names = re.sub(
        r'const char\* ([a-zA-Z0-9_]+)', r'CONSTCHAR_\1', arguments)
    log_argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\* ([a-zA-Z0-9_]+)',
        r'CONSTVOID_\2', log_argument_names)
    log_argument_names = re.sub(
        r'(?<!E)GLenum ([a-zA-Z0-9_]+)', r'GLenum_\1', log_argument_names)
    log_argument_names = re.sub(
        r'(?<!E)GLboolean ([a-zA-Z0-9_]+)', r'GLboolean_\1', log_argument_names)
    log_argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2',
        log_argument_names)
    log_argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2',
        log_argument_names)
    log_argument_names = re.sub(
        r'CONSTVOID_([a-zA-Z0-9_]+)',
        r'static_cast<const void*>(\1)', log_argument_names);
    log_argument_names = re.sub(
        r'CONSTCHAR_([a-zA-Z0-9_]+)', r'\1', log_argument_names);
    log_argument_names = re.sub(
        r'GLenum_([a-zA-Z0-9_]+)', r'GLES2Util::GetStringEnum(\1)',
        log_argument_names)
    log_argument_names = re.sub(
        r'GLboolean_([a-zA-Z0-9_]+)', r'GLES2Util::GetStringBool(\1)',
        log_argument_names)
    log_argument_names = log_argument_names.replace(',', ' << ", " <<')
    if argument_names == 'void' or argument_names == '':
      argument_names = ''
      log_argument_names = ''
    else:
      log_argument_names = " << " + log_argument_names
    function_name = names[0]
    if return_type == 'void':
      file.write('  GL_SERVICE_LOG("%s" << "(" %s << ")");\n' %
          (function_name, log_argument_names))
      file.write('  g_driver_%s.debug_fn.%sFn(%s);\n' %
          (set_name.lower(), function_name, argument_names))
      if 'logging_code' in func:
        file.write("%s\n" % func['logging_code'])
    else:
      file.write('  GL_SERVICE_LOG("%s" << "(" %s << ")");\n' %
          (function_name, log_argument_names))
      file.write('  %s result = g_driver_%s.debug_fn.%sFn(%s);\n' %
          (return_type, set_name.lower(), function_name, argument_names))
      if 'logging_code' in func:
        file.write("%s\n" % func['logging_code'])
      else:
        file.write('  GL_SERVICE_LOG("GL_RESULT: " << result);\n');
      file.write('  return result;\n')
    file.write('}\n')
  file.write('}  // extern "C"\n')

  # Write function to initialize the debug function pointers.
  file.write('\n')
  file.write('void Driver%s::InitializeDebugBindings() {\n' %
             set_name.upper())
  for func in functions:
    first_name = func['names'][0]
    file.write('  if (!debug_fn.%sFn) {\n' % first_name)
    file.write('    debug_fn.%sFn = fn.%sFn;\n' % (first_name, first_name))
    file.write('    fn.%sFn = Debug_%s;\n' % (first_name, first_name))
    file.write('  }\n')
  file.write('  g_debugBindingsInitialized = true;\n')
  file.write('}\n')

  # Write function to update the debug function pointers to extension functions
  # after the extensions have been initialized.
  file.write('\n')
  file.write('void Driver%s::UpdateDebugExtensionBindings() {\n' %
             set_name.upper())
  for extension, ext_functions in used_extension_functions:
    for name, _ in ext_functions:
      file.write('  if (debug_fn.%sFn != fn.%sFn &&\n' % (name, name))
      file.write('      fn.%sFn != Debug_%s) {\n' % (name, name))
      file.write('    debug_fn.%sFn = fn.%sFn;\n' % (name, name))
      file.write('    fn.%sFn = Debug_%s;\n' % (name, name))
      file.write('  }\n')
  file.write('}\n')

  # Write function to clear all function pointers.
  file.write('\n')
  file.write("""void Driver%s::ClearBindings() {
  memset(this, 0, sizeof(*this));
}
""" % set_name.upper())

  # Write GLApiBase functions
  for func in functions:
    names = func['names']
    return_type = func['return_type']
    arguments = func['arguments']
    file.write('\n')
    file.write('%s %sApiBase::%sFn(%s) {\n' %
        (return_type, set_name.upper(), names[0], arguments))
    argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2', arguments)
    argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2', argument_names)
    if argument_names == 'void' or argument_names == '':
      argument_names = ''
    function_name = names[0]
    if return_type == 'void':
      file.write('  driver_->fn.%sFn(%s);\n' %
          (function_name, argument_names))
    else:
      file.write('  return driver_->fn.%sFn(%s);\n' %
          (function_name, argument_names))
    file.write('}\n')

  # Write TraceGLApi functions
  for func in functions:
    names = func['names']
    return_type = func['return_type']
    arguments = func['arguments']
    file.write('\n')
    file.write('%s Trace%sApi::%sFn(%s) {\n' %
        (return_type, set_name.upper(), names[0], arguments))
    argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2', arguments)
    argument_names = re.sub(
        r'(const )?[a-zA-Z0-9_]+\** ([a-zA-Z0-9_]+)', r'\2', argument_names)
    if argument_names == 'void' or argument_names == '':
      argument_names = ''
    function_name = names[0]
    file.write('  TRACE_EVENT_BINARY_EFFICIENT0("gpu", "TraceGLAPI::%s")\n' %
               function_name)
    if return_type == 'void':
      file.write('  %s_api_->%sFn(%s);\n' %
          (set_name.lower(), function_name, argument_names))
    else:
      file.write('  return %s_api_->%sFn(%s);\n' %
          (set_name.lower(), function_name, argument_names))
    file.write('}\n')

  file.write('\n')
  file.write('}  // namespace gfx\n')


def GenerateMockSource(file, functions):
  """Generates functions that invoke a mock GLInterface"""

  file.write(
"""// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is automatically generated.

#include <string.h>

#include "ui/gl/gl_interface.h"

namespace gfx {
""")
  # Write function that trampoline into the GLInterface.
  for func in functions:
    file.write('\n')
    file.write('%s GL_BINDING_CALL Mock_%s(%s) {\n' %
        (func['return_type'], func['names'][0], func['arguments']))
    argument_names = re.sub(r'(const )?[a-zA-Z0-9]+((\s*const\s*)?\*)* ([a-zA-Z0-9]+)', r'\4',
                              func['arguments'])
    if argument_names == 'void':
      argument_names = ''
    function_name = func['names'][0][2:]
    if func['return_type'] == 'void':
      file.write('  GLInterface::GetGLInterface()->%s(%s);\n' %
          (function_name, argument_names))
    else:
      file.write('  return GLInterface::GetGLInterface()->%s(%s);\n' %
          (function_name, argument_names))
    file.write('}\n')

  # Write an 'invalid' function to catch code calling through uninitialized
  # function pointers or trying to interpret the return value of
  # GLProcAddress().
  file.write('\n')
  file.write('static void MockInvalidFunction() {\n')
  file.write('  NOTREACHED();\n')
  file.write('}\n')

  # Write a function to lookup a mock GL function based on its name.
  file.write('\n')
  file.write('void* GL_BINDING_CALL GetMockGLProcAddress(const char* name) {\n')
  for func in functions:
    first_name = func['names'][0]
    file.write('  if (strcmp(name, "%s") == 0)\n' % first_name)
    file.write('    return reinterpret_cast<void*>(Mock_%s);\n' % first_name)
  # Always return a non-NULL pointer like some EGL implementations do.
  file.write('  return reinterpret_cast<void*>(&MockInvalidFunction);\n')
  file.write('}\n');

  file.write('\n')
  file.write('}  // namespace gfx\n')


def ParseExtensionFunctionsFromHeader(header_file):
  """Parse a C extension header file and return a map from extension names to
  a list of functions.

  Args:
    header_file: Line-iterable C header file.
  Returns:
    Map of extension name => functions.
  """
  extension_start = re.compile(
      r'#ifndef ((?:GL|EGL|WGL|GLX)_[A-Z]+_[a-zA-Z]\w+)')
  extension_function = re.compile(r'.+\s+([a-z]+\w+)\s*\(')
  typedef = re.compile(r'typedef .*')
  macro_start = re.compile(r'^#(if|ifdef|ifndef).*')
  macro_end = re.compile(r'^#endif.*')
  macro_depth = 0
  current_extension = None
  current_extension_depth = 0
  extensions = collections.defaultdict(lambda: [])
  for line in header_file:
    if macro_start.match(line):
      macro_depth += 1
    elif macro_end.match(line):
      macro_depth -= 1
      if macro_depth < current_extension_depth:
        current_extension = None
    match = extension_start.match(line)
    if match:
      current_extension = match.group(1)
      current_extension_depth = macro_depth
      assert current_extension not in extensions, \
          "Duplicate extension: " + current_extension
    match = extension_function.match(line)
    if match and current_extension and not typedef.match(line):
      extensions[current_extension].append(match.group(1))
  return extensions


def GetExtensionFunctions(extension_headers):
  """Parse extension functions from a list of header files.

  Args:
    extension_headers: List of header file names.
  Returns:
    Map of extension name => list of functions.
  """
  extensions = {}
  for header in extension_headers:
    extensions.update(ParseExtensionFunctionsFromHeader(open(header)))
  return extensions


def GetFunctionToExtensionMap(extensions):
  """Construct map from a function names to extensions which define the
  function.

  Args:
    extensions: Map of extension name => functions.
  Returns:
    Map of function name => extension name.
  """
  function_to_extensions = {}
  for extension, functions in extensions.items():
    for function in functions:
      if not function in function_to_extensions:
        function_to_extensions[function] = []
      function_to_extensions[function].append(extension)
  return function_to_extensions


def LooksLikeExtensionFunction(function):
  """Heuristic to see if a function name is consistent with extension function
  naming."""
  vendor = re.match(r'\w+?([A-Z][A-Z]+)$', function)
  return vendor is not None and not vendor.group(1) in ['GL', 'API', 'DC']


def GetUsedExtensionFunctions(functions, extension_headers, extra_extensions):
  """Determine which functions belong to extensions.

  Args:
    functions: List of (return type, function names, arguments).
    extension_headers: List of header file names.
  Returns:
    List of (extension name, [function name alternatives]) sorted with least
    preferred extensions first.
  """
  # Parse known extensions.
  extensions = GetExtensionFunctions(extension_headers)
  functions_to_extensions = GetFunctionToExtensionMap(extensions)

  # Collect all used extension functions.
  used_extension_functions = collections.defaultdict(lambda: [])
  for func in functions:
    for name in func['names']:
      # Make sure we know about all extension functions.
      if (LooksLikeExtensionFunction(name) and
          not name in functions_to_extensions):
        raise RuntimeError('%s looks like an extension function but does not '
            'belong to any of the known extensions.' % name)
      if name in functions_to_extensions:
        extensions = functions_to_extensions[name][:]
        if 'other_extensions' in func:
          extensions.extend(func['other_extensions'])
        for extension in extensions:
          used_extension_functions[extension].append((func['names'][0], name))

  # Add extensions that do not have any functions.
  used_extension_functions.update(dict(
      [(e, []) for e in extra_extensions if e not in used_extension_functions]))

  def ExtensionSortKey(name):
    # Prefer ratified extensions and EXTs.
    preferences = ['_ARB_', '_OES_', '_EXT_', '']
    for i, category in enumerate(preferences):
      if category in name:
        return -i
  used_extension_functions = sorted(used_extension_functions.items(),
      key = lambda item: ExtensionSortKey(item[0]))
  return used_extension_functions


def ResolveHeader(header, header_paths):
  paths = header_paths.split(':')

  # Always use a path for Chromium-specific extensions. They are extracted
  # to separate files.
  paths.append('.')
  paths.append('../../gpu')

  root = os.path.abspath(os.path.dirname(__file__))

  for path in paths:
    result = os.path.join(path, header)
    if not os.path.isabs(path):
      result = os.path.relpath(os.path.join(root, result), os.getcwd())
    if os.path.exists(result):
      # Always use forward slashes as path separators. Otherwise backslashes
      # may be incorrectly interpreted as escape characters.
      return result.replace(os.path.sep, '/')

  raise Exception('Header %s not found.' % header)


def main(argv):
  """This is the main function."""

  parser = optparse.OptionParser()
  parser.add_option('--inputs', action='store_true')
  parser.add_option('--header-paths')

  options, args = parser.parse_args(argv)

  if options.inputs:
    for [_, _, headers, _] in FUNCTION_SETS:
      for header in headers:
        print ResolveHeader(header, options.header_paths)
    return 0

  if len(args) >= 1:
    dir = args[0]
  else:
    dir = '.'

  for [functions, set_name, extension_headers, extensions] in FUNCTION_SETS:
    extension_headers = [ResolveHeader(h, options.header_paths)
                         for h in extension_headers]
    used_extension_functions = GetUsedExtensionFunctions(
        functions, extension_headers, extensions)

    header_file = open(
        os.path.join(dir, 'gl_bindings_autogen_%s.h' % set_name), 'wb')
    GenerateHeader(header_file, functions, set_name, used_extension_functions)
    header_file.close()

    header_file = open(
        os.path.join(dir, 'gl_bindings_api_autogen_%s.h' % set_name), 'wb')
    GenerateAPIHeader(
        header_file, functions, set_name, used_extension_functions)
    header_file.close()

    source_file = open(
        os.path.join(dir, 'gl_bindings_autogen_%s.cc' % set_name), 'wb')
    GenerateSource(source_file, functions, set_name, used_extension_functions)
    source_file.close()

    header_file = open(
        os.path.join(dir, 'gl_interface_autogen_%s.h' % set_name), 'wb')
    GenerateInterfaceHeader(
        header_file, functions, set_name, used_extension_functions)
    header_file.close()

    header_file = open(
        os.path.join(dir, 'gl_mock_autogen_%s.h' % set_name), 'wb')
    GenerateMockHeader(
        header_file, functions, set_name, used_extension_functions)
    header_file.close()

  source_file = open(os.path.join(dir, 'gl_bindings_autogen_mock.cc'), 'wb')
  GenerateMockSource(source_file, GL_FUNCTIONS)
  source_file.close()
  return 0


if __name__ == '__main__':
  sys.exit(main(sys.argv[1:]))

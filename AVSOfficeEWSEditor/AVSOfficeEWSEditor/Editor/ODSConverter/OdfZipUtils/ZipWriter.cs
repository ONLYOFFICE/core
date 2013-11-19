/* 
 * Copyright (c) 2006, Clever Age
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Clever Age nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

using System;
using System.IO;

namespace CleverAge.OdfConverter.OdfZipUtils
{
    /// <summary>
    /// ZipWriter defines an abstract class to write entries into a ZIP file.
    /// To add a file, first call AddEntry with the relative path, then
    /// write the content of the file into the stream.
    /// </summary>
    public abstract class ZipWriter : Stream
    {
        public string Filename = "";

        /// <summary>
        /// Adds an entry to the ZIP file (only writes the header, to write
        /// the content use Stream methods).
        /// </summary>
        /// <param name="relativePath">The relative path of the entry in the ZIP
        /// file.</param>
        public abstract void AddEntry(string relativePath);

        /// <summary>
        /// Adds an entry to the ZIP file (only writes the header, to write
        /// the content use Stream methods).
        /// </summary>
        /// <param name="relativePath">The relative path of the entry in the ZIP file.</param>
        /// <param name="compressionMethod">The method used to compress. Supported methods are DEFLATE and no compression</param>
        public abstract void AddEntry(string relativePath, CompressionMethod compressionMethod);
    }
}

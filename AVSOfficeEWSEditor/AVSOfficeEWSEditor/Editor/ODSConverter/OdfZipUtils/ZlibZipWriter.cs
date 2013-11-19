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
    class ZlibZipWriter : ZipWriter
    {
        private IntPtr handle;
        private bool entryOpened;

        internal ZlibZipWriter(string path)
        {
            this.Filename = path;
            string resolvedPath = ZipLib.ResolvePath(path);
            this.handle = ZipLib.zipOpen(resolvedPath, 0);
            if (this.handle == IntPtr.Zero)
            {
                // Small trick to get exact error message...
                try
                {
                    using (FileStream writer = File.Create(path))
                    {
                        writer.WriteByte(0);
                    }
                    File.Delete(path);
                    throw new ZipCreationException();
                }
                catch (Exception ex)
                {
                    throw new ZipCreationException(ex.Message);
                }
            }
            this.entryOpened = false;
        }

        public override void AddEntry(string relativePath)
        {
            AddEntry(relativePath, CompressionMethod.Deflated);
        }

        public override void AddEntry(string relativePath, CompressionMethod compressionMethod)
        {
            string resolvedPath = ZipLib.ResolvePath(relativePath);
            ZipFileEntryInfo info;
            info.DateTime = DateTime.Now;

            if (this.entryOpened)
            {
                ZipLib.zipCloseFileInZip(this.handle);
                this.entryOpened = false;
            }

            int result = ZipLib.zipOpenNewFileInZip(this.handle, resolvedPath, out info, null, 0, null, 0, String.Empty, (int)compressionMethod, (int)CompressionLevel.Default);
            if (result < 0)
            {
                throw new ZipException("Error while opening entry for writing: " + relativePath + " - Errorcode: " + result);
            }
            this.entryOpened = true;
        }

        public override void Close()
        {
            if (handle != IntPtr.Zero)
            {
                int result;
                if (this.entryOpened)
                {
                    result = ZipLib.zipCloseFileInZip(this.handle);
                    if (result != 0)
                    {
                        throw new ZipException("Error while closing entry - Errorcode: " + result);
                    }
                    this.entryOpened = false;
                }
                result = ZipLib.zipClose(this.handle, "");
                handle = IntPtr.Zero;
                // Should we raise this exception ?
                if (result != 0)
                {
                    throw new ZipException("Error while closing ZIP file - Errorcode: " + result);
                }
            }
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            throw new ZipException("Error, Read not allowed on this stream");
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            int result;
            if (offset != 0)
            {
                byte[] newBuffer = new byte[count];
                Array.Copy(buffer, offset, newBuffer, 0, count);
                result = ZipLib.zipWriteInFileInZip(handle, newBuffer, (uint)count);
            }
            else
            {
                result = ZipLib.zipWriteInFileInZip(handle, buffer, (uint)count);
            }

            if (result < 0)
            {
                throw new ZipException("Error while writing entry - Errorcode: " + result);
            }
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            return 0;
        }

        public override void SetLength(long value)
        {
            return;
        }

        public override void Flush()
        {
            return;
        }

        public override long Position
        {
            get
            {
                throw new ZipException("Position not available on this stream");
            }
            set
            {
                throw new ZipException("Position not available on this stream");
            }
        }

        public override long Length
        {
            get
            {
                return 0;
            }
        }

        public override bool CanRead
        {
            get
            {
                return false;
            }
        }

        public override bool CanWrite
        {
            get
            {
                return true;
            }
        }

        public override bool CanSeek
        {
            get
            {
                return false;
            }
        }
    }
}

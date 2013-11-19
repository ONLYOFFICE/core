/* 
 * Copyright (c) 2006-2009 DIaLOGIKa
 *
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
 *     * Neither the names of copyright holders, nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */
using System;
using System.IO;
using System.Net;

namespace OdfConverter.OdfConverterLib
{
    public class UriLoader
    {
        /// <summary>
        /// Downloads a file and saves it to the user's temp folder
        /// </summary>
        /// <param name="uri">the uri of the document to be downloaded</param>
        /// <returns>the full file name to the downloaded document</returns>
        public static string DownloadFile(string uri)
        {
            string tempFileName = null;
            Uri result = null;

            if (Uri.TryCreate(uri, UriKind.RelativeOrAbsolute, out result) && !result.IsFile)
            {
                string rawFileName = Path.GetFileNameWithoutExtension(result.AbsolutePath);
                string extension = Path.GetExtension(result.AbsolutePath);
                string tempFolder = Path.GetTempPath();
                int num = 0;

                tempFileName = Path.Combine(tempFolder, Path.GetFileName(result.AbsolutePath));
                while (File.Exists(tempFileName))
                {
                    tempFileName = Path.Combine(tempFolder, rawFileName + "_" + ++num + extension);
                }

                if (result.Scheme == Uri.UriSchemeHttp || result.Scheme == Uri.UriSchemeFtp || result.Scheme == Uri.UriSchemeHttps)
                {
                    WebClient wc = new WebClient();
                    wc.DownloadFile(result, tempFileName);
                }
            }

            return tempFileName;
        }

        /// <summary>
        /// Checks whether a uri points to a local or remote file
        /// </summary>
        public static bool IsRemote(string uri)
        {
            Uri result;
            return (Uri.TryCreate(uri, UriKind.RelativeOrAbsolute, out result) && result.IsAbsoluteUri && !result.IsFile);
        }
    }
}

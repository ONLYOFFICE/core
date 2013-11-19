/*
 * Copyright (c) 2008 DIaLOGIKa
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
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace CleverAge.OdfConverter.OdfConverterLib
{
    public enum LogLevel
    {
        Debug = 1,
        Info = 2,
        Warning = 3,
        Error = 4
    }

    public class ConversionReport
    {
        private StreamWriter _writer = null;
        private LogLevel _level = LogLevel.Info;

        public ConversionReport(string filename, LogLevel level)
        {
            this._level = level;
            if (filename != null)
            {
                this._writer = new StreamWriter(new FileStream(filename, FileMode.Create, FileAccess.Write));
                Console.WriteLine("Using report file: " + filename);
            }
        }

        public void AddComment(string message, params object[] args)
        {
            string text = "*** " + string.Format(message, args);

            if (this._writer != null)
            {
                this._writer.WriteLine(text);
                this._writer.Flush();
            }
            Console.WriteLine(text);
        }

        public void LogError(string filename, string message, params object[] args)
        {
            addLog(filename, string.Format(message, args), LogLevel.Error);
        }

        public void LogWarning(string filename, string message, params object[] args)
        {
            addLog(filename, string.Format(message, args), LogLevel.Warning);
        }

        public void LogInfo(string filename, string message, params object[] args)
        {
            addLog(filename, string.Format(message, args), LogLevel.Info);
        }

        public void LogDebug(string filename, string message, params object[] args)
        {
            addLog(filename, string.Format(message, args), LogLevel.Debug);
        }

        private void addLog(string filename, string message, LogLevel level)
        {
            if (level >= this._level)
            {
                string label = null;
                switch (level)
                {
                    case LogLevel.Error:
                        label = "ERROR";
                        break;
                    case LogLevel.Warning:
                        label = "WARNING";
                        break;
                    case LogLevel.Info:
                        label = "INFO";
                        break;
                    default:
                        label = "DEBUG";
                        break;
                }
                string text = "[" + label + "]" + "[" + filename + "] " + message;

                if (this._writer != null)
                {
                    this._writer.WriteLine(text);
                    this._writer.Flush();
                }
                Console.WriteLine(text);
            }
        }

        public void Close()
        {
            if (this._writer != null)
            {
                this._writer.Close();
                this._writer = null;
            }
        }

    }
}

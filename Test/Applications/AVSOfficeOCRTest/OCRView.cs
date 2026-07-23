/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace AVSOfficeOCRTest
{
    public partial class OCRView : Form
    {
        public OCRView(string strText)
        {
            InitializeComponent();

            ShowXmlInWebBrowser(webBrowser1, strText);
        }

        public static void ShowXmlInWebBrowser(WebBrowser webBrowser, string TextDocument)
        {
            try
            {
                // generate a unique file name
                string FileNameTemp = Environment.GetFolderPath(Environment.SpecialFolder.InternetCache) + "\\"
                                      + "TextDocument_".GetHashCode().ToString();
                // clean up just incase
                if (File.Exists(FileNameTemp))
                    File.Delete(FileNameTemp);

                // write wsdl to a temporary file
                using (StreamWriter oTempFile = File.CreateText(FileNameTemp))
                {
                    //TextDocument = TextDocument.Replace("utf-16", "utf-8");

                    oTempFile.Write(TextDocument);
                }

                webBrowser.Navigate(FileNameTemp, false);
            }
            catch
            {

            }
        }
    }
}

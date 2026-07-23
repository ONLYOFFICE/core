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
using System.Text;
using System.IO;
using System.Threading;
using System.Xml;


namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class ClearFolderTask: ServiceTask
    {
        private string mFolder = "";

        public ClearFolderTask(Logger log)
            : base (log)
        {
        }

        public override TaskType GetTaskType()
        {
            return TaskType.Async;
        }

        public override void SetParams(String parameters)
        {
            base.SetParams(parameters);

            String error = "";
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                mFolder = "";
                xmlDoc.LoadXml(mStartParams);
                XmlNodeList urlElements = xmlDoc.GetElementsByTagName("folder");
                if (null == urlElements || 0 == urlElements.Count)
                    throw new Exception("Request has no element folder");
                mFolder = urlElements[0].InnerText;
            }
            catch (XmlException ex)
            {
                error = ex.Message;
            }
            catch (Exception ex)
            {
                error = ex.Message;
            }

            if (error.Length > 0 && null != mLogger)
            {
                mLogger.LogError(error);
            }
        }

        protected override void DoWork()
        {
            String sFolder = mFolder;
            if (sFolder.Length > 0)
            {
                try
                {
                    bool bRes = ServiceHelpers.EmptyFolder(sFolder, mLogger);
                    if (!bRes)
                        throw new Exception("EmptyFolder() failed. See.");
                    if (null != mLogger)
                        mLogger.LogInformation("ClearFolderTask.ClearFolderTask() succeded");
                }
                catch (Exception ex)
                {
                    string msg = "ClearFolderTask.ClearFolderTask() error ";
                    msg += ex.Message;
                    if (null != mLogger)
                        mLogger.LogError(msg);
                }
            }
        }
    }
}

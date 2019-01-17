/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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

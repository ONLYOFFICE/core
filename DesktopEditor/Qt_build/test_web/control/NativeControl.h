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
#ifndef TEST_NATIVECONTROL_H
#define TEST_NATIVECONTROL_H
#include <QApplication>
#include <QGraphicsView>

#include "../../editor/project/NativeControl.h"
class NativeControl : public CNativeCtrl
{
public:
    NativeControl(QWidget *parent = 0, const char *name = NULL) : CNativeCtrl(parent, name)
    {
    }
    virtual ~NativeControl()
    {
    }
public:
    virtual void initializeGL()
    {
        CNativeCtrl::initializeGL();

        QString sBasePath = QApplication::applicationDirPath();
        QString sDebug = "Debug";
        QString sRelease = "Release";

        int n1 = sBasePath.lastIndexOf(sDebug);
        int n2 = sBasePath.lastIndexOf(sRelease);
        if (n1 < 0 && n2 < 0)
            return;

        int nCount = 0;
        if (n1 > n2)
        {
            nCount = n1 + sDebug.length();
        }
        else
        {
            nCount = n2 + sRelease.length();
        }

        if (nCount != sBasePath.length())
        {
            sBasePath = sBasePath.mid(0, nCount);
        }

        QString sFontsPath = sBasePath + "/../../../../../OfficeWeb/Fonts/native";
        QString sSdkPath = sBasePath + "/../../test_resources/sdk-all.js";
        QString sDocumentPath = sBasePath + "/../../test_resources/document/Editor.bin";

        std::wstring s1 = sFontsPath.toStdWString();
        std::wstring s2 = sSdkPath.toStdWString();
        std::wstring s3 = sDocumentPath.toStdWString();

        InitSDK(s1, s2);
        OpenFile(s3);
    }
};

#endif // TEST_NATIVECONTROL_H

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
#ifndef DOCBUILDER_H
#define DOCBUILDER_H

#include "./common.h"

namespace NSDoctRenderer
{
    class CDocBuilderJS;

    class CDocBuilderValue_Private;
    class Q_DECL_EXPORT CDocBuilderValue
    {
    public:
        CDocBuilderValue();
        CDocBuilderValue(const CDocBuilderValue& src);
        CDocBuilderValue& operator=(const CDocBuilderValue& src);
        ~CDocBuilderValue();
        void* private_get_internal();

    public:
        bool IsEmpty();
        void Clear();
        void FreeString(wchar_t* data);

        bool IsNull();
        bool IsUndefined();
        int ToInt();
        double ToDouble();
        wchar_t* ToString();
        CDocBuilderValue GetProperty(const wchar_t* name);

    private:
        CDocBuilderValue_Private* m_internal;
    };

    class CDocBuilder_Private;
    class Q_DECL_EXPORT CDocBuilder
    {
    public:
        CDocBuilder();
        ~CDocBuilder();

    public:
        int OpenFile(const wchar_t* path, const wchar_t* params);
        bool CreateFile(const int& type);
        void SetTmpFolder(const wchar_t* folder);
        int SaveFile(const int& type, const wchar_t* path, const wchar_t* params = 0);
        void CloseFile();
        bool ExecuteCommand(const wchar_t* command, CDocBuilderValue* retValue = 0);

        bool Run(const wchar_t* path);

        bool RunTextW(const wchar_t* commands);
        bool RunTextA(const char* commands);

        void SetProperty(const char* param, const wchar_t* value);
        void SetPropertyW(const wchar_t* param, const wchar_t* value);

        void WriteData(const wchar_t* path, const wchar_t* value, const bool& append);
        bool IsSaveWithDoctrendererMode();

        char* GetVersion();

    public:
        static void Initialize();
        static void Dispose();

    private:
        CDocBuilder_Private* m_pInternal;

        friend class CDocBuilderJS;
    };
}

#endif // DOCBUILDER_H

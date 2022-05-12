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
#ifndef _ASC_HTMLRENDERER_COMMON_H_
#define _ASC_HTMLRENDERER_COMMON_H_

#include "../../DesktopEditor/common/Types.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/graphics/Matrix.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"

#define SVG_WRITER_SCALE 100

namespace NSHtmlRenderer
{
    class IBaseMatrixUpdater
    {
    public:
        virtual void OnBaseMatrixUpdate(const double& dWidth, const double& dHeight) = 0;
    };

    class CDstInfo
    {
    public:
        std::wstring m_strDstFilePath;
        std::wstring m_strAdditionalPath;
        std::wstring m_strDstMedia;
        bool	m_bIsWeb;

    public:
        CDstInfo()
        {
            m_strDstFilePath	= L"";
            m_strAdditionalPath = L"";
            m_strDstMedia		= L"";
            m_bIsWeb			= false;
        }
        CDstInfo(const CDstInfo& oInfo)
        {
            *this = oInfo;
        }
        CDstInfo& operator=(const CDstInfo& oSrc)
        {
            m_strDstFilePath	= oSrc.m_strDstFilePath;
            m_strAdditionalPath = oSrc.m_strAdditionalPath;
            m_strDstMedia		= oSrc.m_strDstMedia;
            m_bIsWeb			= oSrc.m_bIsWeb;
            return *this;
        }
    };
}

namespace NSHtmlRenderer
{
    inline LONG ConvertColor(const LONG& lBGR)
    {
        return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
    }

    inline void WriteIntsToStringBuilder(const int& n1, const int& n2, NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(21);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
    }
    inline void WriteIntsToStringBuilder(const int& n1, const int& n2,
                                    const int& n3, const int& n4,
                                    NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(65);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n3);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n4);
    }
    inline void WriteIntsToStringBuilder(const int& n1, const int& n2,
                                    const int& n3, const int& n4,
                                    const int& n5,
                                    NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(65);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n3);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n4);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n5);
    }
    inline void WriteIntsToStringBuilder(const int& n1, const int& n2,
                                    const int& n3, const int& n4,
                                    const int& n5, const int& n6,
                                    NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(65);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n3);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n4);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n5);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n6);
    }

    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
    }
    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2, const int& n2,
                        const wchar_t* s3,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
        pBuilder->AddInt(n2);
        pBuilder->WriteString(s3, wcslen(s3));
    }
    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2, const int& n2,
                        const wchar_t* s3, const int& n3,
                        const wchar_t* s4,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
        pBuilder->AddInt(n2);
        pBuilder->WriteString(s3, wcslen(s3));
        pBuilder->AddInt(n3);
        pBuilder->WriteString(s4, wcslen(s4));
    }
    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2, const int& n2,
                        const wchar_t* s3, const int& n3,
                        const wchar_t* s4, const int& n4,
                        const wchar_t* s5,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
        pBuilder->AddInt(n2);
        pBuilder->WriteString(s3, wcslen(s3));
        pBuilder->AddInt(n3);
        pBuilder->WriteString(s4, wcslen(s4));
        pBuilder->AddInt(n4);
        pBuilder->WriteString(s5, wcslen(s5));
    }

    inline void SetStringColor(LONG lBGR, NSStringUtils::CStringBuilder* pBuilder)
    {
        BYTE R = (BYTE)(lBGR & 0xFF);
        BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
        BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

        pBuilder->AddSize(50);
        pBuilder->WriteString(L"rgb(", 4);
        pBuilder->AddIntNoCheck(R);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(G);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(B);
        pBuilder->AddCharSafe(')');
    }
    inline void SetStrokeColor(LONG lBGR, LONG lA, NSStringUtils::CStringBuilder* pBuilder)
    {
        BYTE R = (BYTE)(lBGR & 0xFF);
        BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
        BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

        pBuilder->WriteString(L"c.strokeStyle = \"rgba(", 22);
        pBuilder->AddSize(70);
        pBuilder->AddIntNoCheck(R);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(G);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(B);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheckDel100(100 * lA / 255);
        pBuilder->WriteString(L")\";\n", 4);
    }
    inline void SetFillColor(LONG lBGR, LONG lA, NSStringUtils::CStringBuilder* pBuilder)
    {
        BYTE R = (BYTE)(lBGR & 0xFF);
        BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
        BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

        pBuilder->WriteString(L"c.fillStyle = \"rgba(", 20);
        pBuilder->AddSize(70);
        pBuilder->AddIntNoCheck(R);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(G);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(B);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheckDel100(100 * lA / 255);
        pBuilder->WriteString(L")\";\n", 4);
    }

    struct RECT
    {
        LONG left;
        LONG top;
        LONG right;
        LONG bottom;
    };    

    const double c_ag_Inch_to_MM	= 25.4;
    const double c_ag_1pxWidth		= 25.4 / 96;

    enum ImageType
    {
        itJPG	= 0,
        itPNG	= 1
    };

    class CImageInfo
    {
    public:
        ImageType	m_eType;
        LONG		m_lID;

        CImageInfo()
        {
                m_eType	= itJPG;
                m_lID	= -1;
        }
        CImageInfo(const CImageInfo& oSrc)
        {
                *this = oSrc;
        }
        CImageInfo& operator=(const CImageInfo& oSrc)
        {
                m_eType		= oSrc.m_eType;
                m_lID		= oSrc.m_lID;

                return *this;
        }
    };

    inline double FABS(double dVal)
    {
        return (dVal >= 0) ? dVal : -dVal;
    }
    inline int round(double dVal)
    {
        return (int)(dVal + 0.5);
    }
    inline int round2(double dVal)
    {
        // повышаем точность
        return (int)(SVG_WRITER_SCALE * dVal + 0.5);
    }
}

#endif // _ASC_HTMLRENDERER_COMMON_H_

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
#ifndef _ASC_HTMLRENDERER_FMBASE_H_
#define _ASC_HTMLRENDERER_FMBASE_H_

#include "../../DesktopEditor/common/Types.h"
#include "../../DesktopEditor/graphics/Matrix.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#include <vector>
#include <map>
#include <list>

namespace NSHtmlRenderer
{
    namespace NSFontManager
    {
        const double c_dInchToMM	= 25.4;
        const double c_dPixToMM		= 25.4 / 72.0;
        const double c_dPtToMM		= 25.4 / 72.0;
        const double c_dMMToPt		= 72.0 / 25.4;
        const double c_dDpi			= 72.0;

        class CFontProperties
        {
        public:
            std::wstring    				m_strFamilyName;
            BYTE                            m_strPANOSE[10];
            LONG							m_lStyle;
            std::vector<UINT>				m_arSignature;
            bool							m_bIsFixedWidth;
            LONG							m_lAvgWidth;

        public:
            CFontProperties()
            {
                m_strFamilyName		= L"";
                memset(m_strPANOSE, 0, 10);
                m_lStyle			= 0;
                m_arSignature.clear();
                m_bIsFixedWidth		= false;
                m_lAvgWidth			= -1;
            }

            CFontProperties(const CFontProperties& oSrc)
            {
                *this = oSrc;
            }
            CFontProperties& operator=(const CFontProperties& oSrc)
            {
                m_strFamilyName		= oSrc.m_strFamilyName;
                memcpy(m_strPANOSE, oSrc.m_strPANOSE, 10);
                m_lStyle			= oSrc.m_lStyle;
                m_arSignature       = oSrc.m_arSignature;
                m_bIsFixedWidth		= oSrc.m_bIsFixedWidth;
                m_lAvgWidth			= oSrc.m_lAvgWidth;

                return *this;
            }
            ~CFontProperties()
            {
            }
        };

        class CFontAdvanced
        {
        public:
            NSStructures::CFont m_oFont;

            // font metrics
            double							m_dAscent;
            double							m_dDescent;
            double							m_dLineSpacing;
            double							m_dEmHeight;

            double							m_dBaselineOffsetDOCX;
            double							m_dBaselineOffsetHTML;

            double							m_dSpaceWidthMM;

            // font params
            CFontProperties					m_oProperties;
        public:
            CFontAdvanced(): m_oProperties()
            {
                m_oFont.SetDefaultParams();

                m_dAscent			= 0;
                m_dDescent			= 0;
                m_dLineSpacing		= 0;
                m_dEmHeight			= 0;

                m_dBaselineOffsetDOCX	= 0;
                m_dBaselineOffsetHTML	= 0;

                m_dSpaceWidthMM		= 0;
            }

            CFontAdvanced(const CFontAdvanced& oSrc)
            {
                *this = oSrc;
            }

            CFontAdvanced& operator=(const CFontAdvanced& oSrc)
            {
                m_oFont				= oSrc.m_oFont;

                m_dAscent			= oSrc.m_dAscent;
                m_dDescent			= oSrc.m_dDescent;
                m_dLineSpacing		= oSrc.m_dLineSpacing;
                m_dEmHeight			= oSrc.m_dEmHeight;

                m_dBaselineOffsetDOCX	= oSrc.m_dBaselineOffsetDOCX;
                m_dBaselineOffsetHTML	= oSrc.m_dBaselineOffsetHTML;

                m_dSpaceWidthMM		= oSrc.m_dSpaceWidthMM;

                m_oProperties		= oSrc.m_oProperties;

                return *this;
            }
        };

        class CFontPickUp
        {
        public:
            CFontAdvanced	m_oFont;
            BYTE			m_lRangeNum;
            BYTE			m_lRange;
            std::wstring    m_strPickFont;
            LONG			m_lPickStyle;

        public:
            CFontPickUp() : m_oFont()
            {
                m_lRangeNum		= 0xFF;
                m_lRange		= 0xFF;
                m_strPickFont	= L"";
                m_lPickStyle	= 0;
            }
            CFontPickUp(const CFontPickUp& oSrc)
            {
                *this = oSrc;
            }
            CFontPickUp& operator=(const CFontPickUp& oSrc)
            {
                m_oFont			= oSrc.m_oFont;
                m_lRange		= oSrc.m_lRange;
                m_lRangeNum		= oSrc.m_lRangeNum;
                m_strPickFont	= oSrc.m_strPickFont;
                m_lPickStyle	= oSrc.m_lPickStyle;

                return *this;
            }
        };

        class CFontManagerBase
        {
        public:
            enum MeasureType
            {
                MeasureTypeGlyph	= 0,
                MeasureTypePosition	= 1
            };

        protected:
            NSFonts::IApplicationFonts* m_pApplicationFonts;
            NSFonts::IFontManager* m_pManager;
            std::wstring	m_strDefaultFont;

        public:

            CFontAdvanced					m_oFont;

            //для подбора шрифтов
            BYTE							m_pRanges[0xFFFF];
            BYTE							m_pRangesNums[0xFFFF];

            BYTE							m_mapUnicode[0xFFFF];

            std::map<std::wstring, CFontProperties>	m_mapFontPathToProperties;
            CFontProperties*                        m_pCurrentProperties;

            std::list<CFontPickUp>			m_arListPicUps;
            std::wstring					m_strCurrentPickFont;
            LONG							m_lCurrentPictFontStyle;

            bool							m_bIsUseFontWidth;

        public:
            CFontManagerBase() : m_oFont(), m_mapFontPathToProperties()
            {
                m_pApplicationFonts = NSFonts::NSApplication::Create();
                m_pApplicationFonts->Initialize();
                m_pApplicationFonts->GetCache()->SetCacheSize(16);
                m_pManager = m_pApplicationFonts->GenerateFontManager();

                SetDefaultFont(L"Arial");

                ClearPickUps();

                InitializeRanges();
                InitializeUnicodeMap();

                ClearPickUps();

                m_pCurrentProperties = NULL;
                m_bIsUseFontWidth	 = false;
            }
            virtual ~CFontManagerBase()
            {
                RELEASEOBJECT(m_pManager);
                RELEASEOBJECT(m_pApplicationFonts);
            }

            inline void ClearPickUps()
            {
                m_arListPicUps.clear();
                m_strCurrentPickFont = L"";
                m_lCurrentPictFontStyle = 0;
            }

        public:
            inline void SetDefaultFont(const std::wstring& strName)
            {
                m_strDefaultFont = strName;
                //m_pManager->SetDefaultFont(m_strDefaultFont);
            }
            inline std::wstring GetDefaultFont()
            {
                return m_strDefaultFont;
            }

            virtual void LoadFont(long lFaceIndex = 0, bool bIsNeedAddToMap = true)
            {
            }

            inline void LoadCurrentFont(long lFaceIndex = 0)
            {
                if (m_oFont.m_oFont.Path.empty())
                {
                    LoadFontByName(m_oFont.m_oFont.Name, m_oFont.m_oFont.Size, m_oFont.m_oFont.GetStyle());
                }
                else
                {
                    LoadFontByFile(m_oFont.m_oFont.Path, m_oFont.m_oFont.Size, lFaceIndex);
                    m_oFont.m_oFont.SetStyle(m_oFont.m_oProperties.m_lStyle);
                }
            }

            void LoadFontByName(std::wstring& strName, const double& dSize, const LONG& lStyle)
            {
                m_pManager->LoadFontByName(strName, (float)dSize, lStyle, c_dDpi, c_dDpi);

                LoadFontMetrics();

                m_oFont.m_oProperties.m_strFamilyName	= m_oFont.m_oFont.Name;
                m_oFont.m_oProperties.m_lStyle			= m_oFont.m_oFont.GetStyle();

                m_strCurrentPickFont	= m_oFont.m_oProperties.m_strFamilyName;
                m_lCurrentPictFontStyle	= m_oFont.m_oProperties.m_lStyle;

                m_oFont.m_oProperties.m_lAvgWidth = -1;
            }

            void LoadFontByFile(std::wstring& strPath, const double& dSize, const LONG& lFaceIndex)
            {
                m_pManager->LoadFontFromFile(strPath, lFaceIndex, dSize, c_dDpi, c_dDpi);

                m_oFont.m_oProperties.m_strFamilyName	= m_oFont.m_oFont.Name;
                m_oFont.m_oProperties.m_lStyle			= m_oFont.m_oFont.GetStyle();

                m_strCurrentPickFont	= m_oFont.m_oProperties.m_strFamilyName;
                m_lCurrentPictFontStyle	= m_oFont.m_oProperties.m_lStyle;

                m_oFont.m_oProperties.m_lAvgWidth = -1;

                LoadFontMetrics();
                LoadFontParams();
            }

        public:

            void MeasureString(const std::wstring& strText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
            {
                MeasureStringPix(strText, x, y, dBoxX, dBoxY, dBoxWidth, dBoxHeight, measureType);

                // переводим в миллиметры
                dBoxX		*= c_dPixToMM;
                dBoxY		*= c_dPixToMM;
                dBoxWidth	*= c_dPixToMM;
                dBoxHeight	*= c_dPixToMM;
            }

            void MeasureStringPix(std::wstring bsText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
            {
                dBoxX		= 0;
                dBoxY		= 0;
                dBoxWidth	= 0;
                dBoxHeight	= 0;

                if (NULL == m_pManager)
                    return;

                m_pManager->LoadString1(bsText, (float)x, (float)y);

                TBBox tBox;

                if (MeasureTypeGlyph == measureType)
                {
                    tBox = m_pManager->MeasureString();
                }
                else if (MeasureTypePosition == measureType)
                {
                    tBox = m_pManager->MeasureString2();
                }

                dBoxX		= (double)tBox.fMinX;
                dBoxY		= (double)tBox.fMinY;
                dBoxWidth	= (double)(tBox.fMaxX - tBox.fMinX);
                dBoxHeight	= (double)(tBox.fMaxY - tBox.fMinY);
            }

        public:
            void LoadFontMetrics()
            {
                m_pManager->AfterLoad();
                m_oFont.m_dAscent = m_pManager->GetAscender();
                m_oFont.m_dDescent = m_pManager->GetDescender();
                m_oFont.m_dLineSpacing = m_pManager->GetLineHeight();
                m_oFont.m_dEmHeight = m_pManager->GetUnitsPerEm();

                m_oFont.m_dBaselineOffsetDOCX = (c_dPtToMM * m_oFont.m_dDescent * m_oFont.m_oFont.Size / m_oFont.m_dEmHeight);

                double d1 = 3 * (m_oFont.m_dLineSpacing - m_oFont.m_dDescent) - m_oFont.m_dAscent;
                d1 /= 2.0;

                d1 *= (m_oFont.m_oFont.Size / m_oFont.m_dEmHeight);
                m_oFont.m_dBaselineOffsetHTML = d1;
            }

            std::wstring DeletePdfPrefix(const std::wstring& sFamilyName)
            {
                // Удаляем префикс, который встречается в шрифтах внедренных в PDF
                // Его вид : BAAAAA+FamilyName (6 латинских букв в верхнем регистре и '+')
                // Пример  : EOODIA+Poetica

                if ( sFamilyName.length() > 7 )
                {
                    const wchar_t* buf = sFamilyName.c_str();
                    bool bPrefix = true;
                    for ( int nIndex = 0; nIndex < 6; nIndex++ )
                    {
                        wchar_t nChar = buf[nIndex];
                        if ( nChar < (wchar_t)('A') || nChar > (wchar_t)('Z') )
                        {
                            bPrefix = false;
                            break;
                        }
                    }

                    if ( buf[6] != wchar_t('+') )
                        bPrefix = false;

                    if ( bPrefix )
                    {
                        return sFamilyName.substr(7);
                    }
                }
                
                // Ничего не делаем
                return sFamilyName;
            }
            
            void LoadFontParams()
            {
                // читаем и выставляем все настройки шрифта
                if (NULL == m_pManager)
                    return;

                // загрузка была из path
                std::map<std::wstring, CFontProperties>::const_iterator iFind = m_mapFontPathToProperties.find(m_oFont.m_oFont.Path);

                if (iFind != m_mapFontPathToProperties.end())
                {
                    m_oFont.m_oProperties = iFind->second;
                    return;
                }

                if (m_oFont.m_oFont.Name.empty())
                {
                    // FamilyName
                    m_oFont.m_oProperties.m_strFamilyName = DeletePdfPrefix(m_pManager->GetName());
                }
                else
                {
                    m_oFont.m_oProperties.m_strFamilyName = m_oFont.m_oFont.Name;
                }

                NSFonts::IFontFile* pFontFile = m_pManager->GetFile();
                if (!pFontFile)
                    return;

                // StyleName
                std::string strStyle = pFontFile->GetStyleName();
                if ("Bold" == strStyle)
                {
                    m_oFont.m_oProperties.m_lStyle = 0x01;
                }
                else if ("Italic" == strStyle)
                {
                    m_oFont.m_oProperties.m_lStyle = 0x02;
                }
                else if ("Bold Italic" == strStyle)
                {
                    m_oFont.m_oProperties.m_lStyle = 0x03;
                }
                else
                {
                    m_oFont.m_oProperties.m_lStyle = 0x00;
                }

                pFontFile->GetPanose(m_oFont.m_oProperties.m_strPANOSE);

                // IsFixed
                m_oFont.m_oProperties.m_bIsFixedWidth = pFontFile->IsFixedWidth();

                // Signature
                m_oFont.m_oProperties.m_arSignature.clear();

                for ( unsigned int i = 0; i < 6; i++ )
                {
                    UINT value = 0;

                    for ( unsigned long bit = 0; bit < 32; bit++ )
                    {
                        int iRes = pFontFile->IsUnicodeRangeAvailable( bit, i );

                        if( iRes > 0 )
                        {
                            value |= ( 1 << bit );
                        }
                    }

                    m_oFont.m_oProperties.m_arSignature.push_back(value);
                }

                if (m_bIsUseFontWidth)
                {
                    std::wstring sExt;
                    std::wstring sEncodingPath;

                    std::wstring::size_type nFindExt = m_oFont.m_oFont.Path.find_last_of((wchar_t)'.');
                    if (nFindExt == std::wstring::npos)
                    {
                        sEncodingPath = m_oFont.m_oFont.Path + L".enc";
                        sExt = L"";
                    }
                    else
                    {
                        sExt = m_oFont.m_oFont.Path.substr(nFindExt + 1);
                        sEncodingPath = m_oFont.m_oFont.Path.substr(0, nFindExt);
                    }

                    bool bIsCID = false;
                    if (std::wstring::npos != sExt.find(L"cid"))
                        bIsCID = true;

                    XmlUtils::CXmlNode oMainNode;
                    oMainNode.FromXmlFile(sEncodingPath);

                    if (L"PDF-resources" == oMainNode.GetName())
                    {
                        if (bIsCID)
                        {
                            XmlUtils::CXmlNode oType0Node;
                            if ( oMainNode.GetNode( L"Type0", oType0Node ) )
                            {
                                XmlUtils::CXmlNode oNode;
                                if ( oType0Node.GetNode( L"DescendantFonts", oNode ) )
                                {
                                    XmlUtils::CXmlNode oDescNode;
                                    if ( oNode.GetNode( L"FontDescriptor", oDescNode ) )
                                    {
                                        XmlUtils::CXmlNode oCurNode;
                                        if ( oNode.GetNode( L"AvgWidth", oCurNode ) )
                                        {
                                            m_oFont.m_oProperties.m_lAvgWidth = oCurNode.GetAttributeInt(L"value");
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            XmlUtils::CXmlNode oNode;
                            if ( oMainNode.GetNode( L"FontDescriptor", oNode ) )
                            {
                                XmlUtils::CXmlNode oCurNode;
                                if ( oNode.GetNode( L"AvgWidth", oCurNode ) )
                                {
                                    m_oFont.m_oProperties.m_lAvgWidth = oCurNode.GetAttributeInt(L"value");
                                }
                                else
                                {
                                    XmlUtils::CXmlNode oNodeWidths;
                                    if (oMainNode.GetNode(L"Widths", oNodeWidths))
                                    {
                                        LONG lCount		= 0;
                                        double dWidth	= 0;
                                        XmlUtils::CXmlNodes oNodesW;
                                        if (oNodeWidths.GetNodes(L"Width", oNodesW))
                                        {
                                            int nCountW = oNodesW.GetCount();
                                            for (int i = 0; i < nCountW; ++i)
                                            {
                                                XmlUtils::CXmlNode oNodeMem;
                                                oNodesW.GetAt(i, oNodeMem);

                                                double dMem = oCurNode.GetAttributeDouble(L"value");
                                                if (0 < dMem)
                                                {
                                                    ++lCount;
                                                    dWidth += dMem;
                                                }
                                            }
                                        }

                                        if (10 < lCount)
                                        {
                                            m_oFont.m_oProperties.m_lAvgWidth = (LONG)(0.8 * dWidth / lCount);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                m_mapFontPathToProperties.insert(std::pair<std::wstring, CFontProperties>(m_oFont.m_oFont.Path, m_oFont.m_oProperties));
            }

        private:
            void InitializeRanges()
            {
                memset(m_pRanges, 0xFF, 0xFFFF);
                memset(m_pRangesNums, 0xFF, 0xFFFF);

                // теперь просто по порядку заполняем все рэнджи
                int nStart = 0;
                int nCount = 0;

                // rangeNum 0
                // case 00: sUCRName = "Basic Latin"; break; /* U+0020-U+007E */
                nStart = 0x0020;
                nCount = 0x007E - nStart + 1;
                memset(m_pRanges + nStart, 0, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 01: sUCRName = "Latin-1 Supplement"; break; /* U+0080-U+00FF */
                nStart = 0x0080;
                nCount = 0x00FF - nStart + 1;
                memset(m_pRanges + nStart, 1, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 02: sUCRName = "Latin Extended-A"; break; /* U+0100-U+017F */
                nStart = 0x0100;
                nCount = 0x017F - nStart + 1;
                memset(m_pRanges + nStart, 2, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 03: sUCRName = "Latin Extended-B"; break; /* U+0180-U+024F */
                nStart = 0x0180;
                nCount = 0x024F - nStart + 1;
                memset(m_pRanges + nStart, 3, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 04: sUCRName = "IPA Extensions"; break; /* U+0250-U+02AF */ /* U+1D00-U+1D7F */ /* U+1D80-U+1DBF */
                nStart = 0x0250;
                nCount = 0x02AF - nStart + 1;
                memset(m_pRanges + nStart, 4, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x1D00;
                nCount = 0x1D7F - nStart + 1;
                memset(m_pRanges + nStart, 4, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x1D80;
                nCount = 0x1DBF - nStart + 1;
                memset(m_pRanges + nStart, 4, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 05: sUCRName = "Spacing Modifier Letters"; break; /* U+02B0-U+02FF */ /* U+A700-U+A71F */
                nStart = 0x02B0;
                nCount = 0x02FF - nStart + 1;
                memset(m_pRanges + nStart, 5, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0xA700;
                nCount = 0xA71F - nStart + 1;
                memset(m_pRanges + nStart, 5, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 06: sUCRName = "Combining Diacritical Marks"; break; /* U+0300-U+036F */ /* U+1DC0-U+1DFF */
                nStart = 0x0300;
                nCount = 0x036F - nStart + 1;
                memset(m_pRanges + nStart, 6, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x1DC0;
                nCount = 0x1DFF - nStart + 1;
                memset(m_pRanges + nStart, 6, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 07: sUCRName = "Greek and Coptic"; break; /* U+0370-U+03FF */
                nStart = 0x0370;
                nCount = 0x03FF - nStart + 1;
                memset(m_pRanges + nStart, 7, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 08: sUCRName = "Coptic"; break; /* U+2C80-U+2CFF */
                nStart = 0x2C80;
                nCount = 0x2CFF - nStart + 1;
                memset(m_pRanges + nStart, 8, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 09: sUCRName = "Cyrillic"; break; /* U+0400-U+04FF */ /* U+0500-U+052F */ /* U+2DE0-U+2DFF */ /* U+A640-U+A69F */
                nStart = 0x0400;
                nCount = 0x04FF - nStart + 1;
                memset(m_pRanges + nStart, 9, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x0500;
                nCount = 0x052F - nStart + 1;
                memset(m_pRanges + nStart, 9, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x2DE0;
                nCount = 0x2DFF - nStart + 1;
                memset(m_pRanges + nStart, 9, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0xA640;
                nCount = 0xA69F - nStart + 1;
                memset(m_pRanges + nStart, 9, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 10: sUCRName = "Armenian"; break; /* U+0530-U+058F */
                nStart = 0x0530;
                nCount = 0x058F - nStart + 1;
                memset(m_pRanges + nStart, 10, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 11: sUCRName = "Hebrew"; break; /* U+0590-U+05FF */
                nStart = 0x0590;
                nCount = 0x05FF - nStart + 1;
                memset(m_pRanges + nStart, 11, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 12: sUCRName = "Vai"; break; /* U+A500-U+A63F */
                nStart = 0xA500;
                nCount = 0xA63F - nStart + 1;
                memset(m_pRanges + nStart, 12, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 13: sUCRName = "Arabic"; break; /* U+0600-U+06FF */ /* U+0750-U+077F */
                nStart = 0x0600;
                nCount = 0x06FF - nStart + 1;
                memset(m_pRanges + nStart, 13, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x0750;
                nCount = 0x077F - nStart + 1;
                memset(m_pRanges + nStart, 13, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 14: sUCRName = "NKo"; break; /* U+07C0-U+07FF */
                nStart = 0x07C0;
                nCount = 0x07FF - nStart + 1;
                memset(m_pRanges + nStart, 14, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 15: sUCRName = "Devanagari"; break; /* U+0900-U+097F */
                nStart = 0x0900;
                nCount = 0x097F - nStart + 1;
                memset(m_pRanges + nStart, 15, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 16: sUCRName = "Bengali"; break; /* U+0980-U+09FF */
                nStart = 0x0980;
                nCount = 0x09FF - nStart + 1;
                memset(m_pRanges + nStart, 16, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 17: sUCRName = "Gurmukhi"; break; /* U+0A00-U+0A7F */
                nStart = 0x0A00;
                nCount = 0x0A7F - nStart + 1;
                memset(m_pRanges + nStart, 17, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 18: sUCRName = "Gujarati"; break; /* U+0A80-U+0AFF */
                nStart = 0x0A80;
                nCount = 0x0AFF - nStart + 1;
                memset(m_pRanges + nStart, 18, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 19: sUCRName = "Oriya"; break; /* U+0B00-U+0B7F */
                nStart = 0x0B00;
                nCount = 0x0B7F - nStart + 1;
                memset(m_pRanges + nStart, 19, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 20: sUCRName = "Tamil"; break; /* U+0B80-U+0BFF */
                nStart = 0x0B80;
                nCount = 0x0BFF - nStart + 1;
                memset(m_pRanges + nStart, 20, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 21: sUCRName = "Telugu"; break; /* U+0C00-U+0C7F */
                nStart = 0x0C00;
                nCount = 0x0C7F - nStart + 1;
                memset(m_pRanges + nStart, 21, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 22: sUCRName = "Kannada"; break; /* U+0C80-U+0CFF */
                nStart = 0x0C80;
                nCount = 0x0CFF - nStart + 1;
                memset(m_pRanges + nStart, 22, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 23: sUCRName = "Malayalam"; break; /* U+0D00-U+0D7F */
                nStart = 0x0D00;
                nCount = 0x0D7F - nStart + 1;
                memset(m_pRanges + nStart, 23, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 24: sUCRName = "Thai"; break; /* U+0E00-U+0E7F */
                nStart = 0x0E00;
                nCount = 0x0E7F - nStart + 1;
                memset(m_pRanges + nStart, 24, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 25: sUCRName = "Lao"; break; /* U+0E80-U+0EFF */
                nStart = 0x0E80;
                nCount = 0x0EFF - nStart + 1;
                memset(m_pRanges + nStart, 25, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 26: sUCRName = "Georgian"; break; /* U+10A0-U+10FF */ /* U+2D00-U+2D2F */
                nStart = 0x10A0;
                nCount = 0x10FF - nStart + 1;
                memset(m_pRanges + nStart, 26, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x2D00;
                nCount = 0x2D2F - nStart + 1;
                memset(m_pRanges + nStart, 26, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 27: sUCRName = "Balinese"; break; /* U+1B00-U+1B7F */
                nStart = 0x1B00;
                nCount = 0x1B7F - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 28: sUCRName = "Hangul Jamo"; break; /* U+1100-U+11FF */
                nStart = 0x1100;
                nCount = 0x11FF - nStart + 1;
                memset(m_pRanges + nStart, 28, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 29: sUCRName = "Latin Extended Additional"; break; /* U+1E00-U+1EFF */ /* U+2C60-U+2C7F */ /* U+A720-U+A7FF */
                nStart = 0x1E00;
                nCount = 0x1EFF - nStart + 1;
                memset(m_pRanges + nStart, 29, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x2C60;
                nCount = 0x2C7F - nStart + 1;
                memset(m_pRanges + nStart, 29, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0xA720;
                nCount = 0xA7FF - nStart + 1;
                memset(m_pRanges + nStart, 29, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 30: sUCRName = "Greek Extended"; break; /* U+1F00-U+1FFF */
                nStart = 0x1F00;
                nCount = 0x1FFF - nStart + 1;
                memset(m_pRanges + nStart, 30, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                //case 31: sUCRName = "General Punctuation"; break; /* U+2000-U+206F */ /* U+2E00-U+2E7F */
                nStart = 0x2000;
                nCount = 0x206F - nStart + 1;
                memset(m_pRanges + nStart, 31, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                nStart = 0x2E00;
                nCount = 0x2E7F - nStart + 1;
                memset(m_pRanges + nStart, 31, nCount);
                memset(m_pRangesNums + nStart, 0, nCount);

                // rangeNum 1
                //case 00: sUCRName = "Superscripts And Subscripts"; break; /* U+2070-U+209F */
                nStart = 0x2070;
                nCount = 0x209F - nStart + 1;
                memset(m_pRanges + nStart, 0, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 01: sUCRName = "Currency Symbols"; break; /* U+20A0-U+20CF */
                nStart = 0x20A0;
                nCount = 0x20CF - nStart + 1;
                memset(m_pRanges + nStart, 1, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 02: sUCRName = "Combining Diacritical Marks For Symbols"; break; /* U+20D0-U+20FF */
                nStart = 0x20D0;
                nCount = 0x20FF - nStart + 1;
                memset(m_pRanges + nStart, 2, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 03: sUCRName = "Letterlike Symbols"; break; /* U+2100-U+214F */
                nStart = 0x2100;
                nCount = 0x214F - nStart + 1;
                memset(m_pRanges + nStart, 3, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 04: sUCRName = "Number Forms"; break; /* U+2150-U+218F */
                nStart = 0x2150;
                nCount = 0x218F - nStart + 1;
                memset(m_pRanges + nStart, 4, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 05: sUCRName = "Arrows"; break; /* U+2190-U+21FF */ /* U+27F0-U+27FF */ /* U+2900-U+297F */ /* U+2B00-U+2BFF */
                nStart = 0x2190;
                nCount = 0x21FF - nStart + 1;
                memset(m_pRanges + nStart, 5, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x27F0;
                nCount = 0x27FF - nStart + 1;
                memset(m_pRanges + nStart, 5, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x2900;
                nCount = 0x297F - nStart + 1;
                memset(m_pRanges + nStart, 5, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x2B00;
                nCount = 0x2BFF - nStart + 1;
                memset(m_pRanges + nStart, 5, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 06: sUCRName = "Mathematical Operators"; break; /* U+2200-U+22FF */ /* U+2A00-U+2AFF */ /* U+27C0-U+27EF */ /* U+2980-U+29FF */
                nStart = 0x2200;
                nCount = 0x22FF - nStart + 1;
                memset(m_pRanges + nStart, 6, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x2A00;
                nCount = 0x2AFF - nStart + 1;
                memset(m_pRanges + nStart, 6, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x27C0;
                nCount = 0x27EF - nStart + 1;
                memset(m_pRanges + nStart, 6, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x2980;
                nCount = 0x29FF - nStart + 1;
                memset(m_pRanges + nStart, 6, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 07: sUCRName = "Miscellaneous Technical"; break; /* U+2300-U+23FF */
                nStart = 0x2300;
                nCount = 0x23FF - nStart + 1;
                memset(m_pRanges + nStart, 7, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 08: sUCRName = "Control Pictures"; break; /* U+2400-U+243F */
                nStart = 0x2400;
                nCount = 0x243F - nStart + 1;
                memset(m_pRanges + nStart, 8, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 09: sUCRName = "Optical Character Recognition"; break; /* U+2440-U+245F */
                nStart = 0x2440;
                nCount = 0x245F - nStart + 1;
                memset(m_pRanges + nStart, 9, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 10: sUCRName = "Enclosed Alphanumerics"; break; /* U+2460-U+24FF */
                nStart = 0x2460;
                nCount = 0x24FF - nStart + 1;
                memset(m_pRanges + nStart, 10, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 11: sUCRName = "Box Drawing"; break; /* U+2500-U+257F */
                nStart = 0x2500;
                nCount = 0x257F - nStart + 1;
                memset(m_pRanges + nStart, 11, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 12: sUCRName = "Block Elements"; break; /* U+2580-U+259F */
                nStart = 0x2580;
                nCount = 0x259F - nStart + 1;
                memset(m_pRanges + nStart, 12, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 13: sUCRName = "Geometric Shapes"; break; /* U+25A0-U+25FF */
                nStart = 0x25A0;
                nCount = 0x25FF - nStart + 1;
                memset(m_pRanges + nStart, 13, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 14: sUCRName = "Miscellaneous Symbols"; break; /* U+2600-U+26FF */
                nStart = 0x2600;
                nCount = 0x26FF - nStart + 1;
                memset(m_pRanges + nStart, 14, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 15: sUCRName = "Dingbats"; break; /* U+2700-U+27BF */
                nStart = 0x2700;
                nCount = 0x27BF - nStart + 1;
                memset(m_pRanges + nStart, 15, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 16: sUCRName = "CJK Symbols and Punctuation"; break; /* U+3000-U+303F */
                nStart = 0x3000;
                nCount = 0x303F - nStart + 1;
                memset(m_pRanges + nStart, 16, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 17: sUCRName = "Hiragana"; break;  /* U+3040-U+309F */
                nStart = 0x3040;
                nCount = 0x309F - nStart + 1;
                memset(m_pRanges + nStart, 17, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 18: sUCRName = "Katakana"; break; /* U+30A0-U+30FF */ /* U+31F0-U+31FF */
                nStart = 0x30A0;
                nCount = 0x30FF - nStart + 1;
                memset(m_pRanges + nStart, 18, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x31F0;
                nCount = 0x31FF - nStart + 1;
                memset(m_pRanges + nStart, 18, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 19: sUCRName = "Bopomofo"; break; /* U+3100-U+312F */ /* U+31A0-U+31BF */
                nStart = 0x3100;
                nCount = 0x312F - nStart + 1;
                memset(m_pRanges + nStart, 19, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x31A0;
                nCount = 0x31BF - nStart + 1;
                memset(m_pRanges + nStart, 19, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 20: sUCRName = "Hangul Compatibility Jamo"; break; /* U+3130-U+318F */
                nStart = 0x3130;
                nCount = 0x318F - nStart + 1;
                memset(m_pRanges + nStart, 20, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 21: sUCRName = "Phags-pa"; break; /* U+A840-U+A87F */
                nStart = 0xA840;
                nCount = 0xA87F - nStart + 1;
                memset(m_pRanges + nStart, 21, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 22: sUCRName = "Enclosed CJK Letters and Months"; break; /* U+3200-U+32FF */
                nStart = 0x3200;
                nCount = 0x32FF - nStart + 1;
                memset(m_pRanges + nStart, 22, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 23: sUCRName = "CJK Compatibility"; break; /* U+3300-U+33FF */
                nStart = 0x3300;
                nCount = 0x33FF - nStart + 1;
                memset(m_pRanges + nStart, 23, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 24: sUCRName = "Hangul Syllables"; break; /* U+AC00-U+D7AF */
                nStart = 0xAC00;
                nCount = 0xD7AF - nStart + 1;
                memset(m_pRanges + nStart, 24, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 25: sUCRName = "Non-Plane 0"; break; /* U+D800-U+DB7F */ /* U+DB80-U+DBFF */ /* U+DC00-U+DFFF */ // Не юникодные символы
                nStart = 0xD800;
                nCount = 0xDB7F - nStart + 1;
                memset(m_pRanges + nStart, 25, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0xDB80;
                nCount = 0xDBFF - nStart + 1;
                memset(m_pRanges + nStart, 25, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0xDC00;
                nCount = 0xDFFF - nStart + 1;
                memset(m_pRanges + nStart, 25, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 26: sUCRName = "Phoenician"; break; /*U+10900-U+1091F*/

                //case 27: sUCRName = "CJK Unified Ideographs"; break; /* U+4E00-U+9FFF */ /* U+2E80-U+2EFF */ /* U+2F00-U+2FDF */ /* U+2FF0-U+2FFF */ /* U+3400-U+4DB5 */ /*U+20000-U+2A6D6*/ /* U+3190-U+319F */
                nStart = 0x4E00;
                nCount = 0x9FFF - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x2E80;
                nCount = 0x2EFF - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x2F00;
                nCount = 0x2FDF - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x2FF0;
                nCount = 0x2FFF - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x3400;
                nCount = 0x4DB5 - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0x3190;
                nCount = 0x319F - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 28: sUCRName = "Private Use Area (plane 0)"; break; /* U+E000-U+F8FF */ // Не юникодные символы
                nStart = 0xE000;
                nCount = 0xF8FF - nStart + 1;
                memset(m_pRanges + nStart, 28, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 29: sUCRName = "CJK Strokes"; break; /* U+31C0-U+31EF */ /* U+F900-U+FAFF */ /*U+2F800-U+2FA1F*/
                nStart = 0x31C0;
                nCount = 0x31EF - nStart + 1;
                memset(m_pRanges + nStart, 29, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                nStart = 0xF900;
                nCount = 0xFAFF - nStart + 1;
                memset(m_pRanges + nStart, 29, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 30: sUCRName = "Alphabetic Presentation Forms"; break; /* U+FB00-U+FB4F */
                nStart = 0xFB00;
                nCount = 0xFB4F - nStart + 1;
                memset(m_pRanges + nStart, 30, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                //case 31: sUCRName = "Arabic Presentation Forms-A"; break; /* U+FB50-U+FDFF */
                nStart = 0xFB50;
                nCount = 0xFDFF - nStart + 1;
                memset(m_pRanges + nStart, 31, nCount);
                memset(m_pRangesNums + nStart, 1, nCount);

                // rangeNum 2
                //case 00: sUCRName = "Combining Half Marks"; break; /* U+FE20-U+FE2F */
                nStart = 0xFE20;
                nCount = 0xFE2F - nStart + 1;
                memset(m_pRanges + nStart, 0, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 01: sUCRName = "Vertical forms"; break; /* U+FE10-U+FE1F */ /* U+FE30-U+FE4F */
                nStart = 0xFE10;
                nCount = 0xFE1F - nStart + 1;
                memset(m_pRanges + nStart, 1, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0xFE30;
                nCount = 0xFE4F - nStart + 1;
                memset(m_pRanges + nStart, 1, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 02: sUCRName = "Small Form Variants"; break; /* U+FE50-U+FE6F */
                nStart = 0xFE50;
                nCount = 0xFE6F - nStart + 1;
                memset(m_pRanges + nStart, 2, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 03: sUCRName = "Arabic Presentation Forms-B"; break; /* U+FE70-U+FEFE */
                nStart = 0xFE70;
                nCount = 0xFEFE - nStart + 1;
                memset(m_pRanges + nStart, 3, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 04: sUCRName = "Halfwidth and Fullwidth Forms"; break; /* U+FF00-U+FFEF */
                nStart = 0xFF00;
                nCount = 0xFFEF - nStart + 1;
                memset(m_pRanges + nStart, 4, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 05: sUCRName = "Specials"; break; /* U+FFF0-U+FFFF */
                nStart = 0xFFF0;
                nCount = 0xFFFF - nStart + 1;
                memset(m_pRanges + nStart, 5, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 06: sUCRName = "Tibetan"; break; /* U+0F00-U+0FFF */
                nStart = 0x0F00;
                nCount = 0x0FFF - nStart + 1;
                memset(m_pRanges + nStart, 6, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 07: sUCRName = "Syriac"; break; /* U+0700-U+074F */
                nStart = 0x0700;
                nCount = 0x074F - nStart + 1;
                memset(m_pRanges + nStart, 7, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 08: sUCRName = "Thaana"; break; /* U+0780-U+07BF */
                nStart = 0x0780;
                nCount = 0x07BF - nStart + 1;
                memset(m_pRanges + nStart, 8, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 09: sUCRName = "Sinhala"; break; /* U+0D80-U+0DFF */
                nStart = 0x0D80;
                nCount = 0x0DFF - nStart + 1;
                memset(m_pRanges + nStart, 9, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 10: sUCRName = "Myanmar"; break; /* U+1000-U+109F */
                nStart = 0x1000;
                nCount = 0x109F - nStart + 1;
                memset(m_pRanges + nStart, 10, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 11: sUCRName = "Ethiopic"; break; /* U+1200-U+137F */ /* U+1380-U+139F */ /* U+2D80-U+2DDF */
                nStart = 0x1200;
                nCount = 0x137F - nStart + 1;
                memset(m_pRanges + nStart, 11, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0x1380;
                nCount = 0x139F - nStart + 1;
                memset(m_pRanges + nStart, 11, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0x2D80;
                nCount = 0x2DDF - nStart + 1;
                memset(m_pRanges + nStart, 11, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 12: sUCRName = "Cherokee"; break; /* U+13A0-U+13FF */
                nStart = 0x13A0;
                nCount = 0x13FF - nStart + 1;
                memset(m_pRanges + nStart, 12, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 13: sUCRName = "Unified Canadian Aboriginal Syllabics"; break; /* U+1400-U+167F */
                nStart = 0x1400;
                nCount = 0x167F - nStart + 1;
                memset(m_pRanges + nStart, 13, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 14: sUCRName = "Ogham"; break; /* U+1680-U+169F */
                nStart = 0x1680;
                nCount = 0x169F - nStart + 1;
                memset(m_pRanges + nStart, 14, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 15: sUCRName = "Runic"; break; /* U+16A0-U+16FF */
                nStart = 0x16A0;
                nCount = 0x16FF - nStart + 1;
                memset(m_pRanges + nStart, 15, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 16: sUCRName = "Khmer"; break; /* U+1780-U+17FF */ /* U+19E0-U+19FF */
                nStart = 0x1780;
                nCount = 0x17FF - nStart + 1;
                memset(m_pRanges + nStart, 16, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0x19E0;
                nCount = 0x19FF - nStart + 1;
                memset(m_pRanges + nStart, 16, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 17: sUCRName = "Mongolian"; break; /* U+1800-U+18AF */
                nStart = 0x1800;
                nCount = 0x18AF - nStart + 1;
                memset(m_pRanges + nStart, 17, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 18: sUCRName = "Braille Patterns"; break; /* U+2800-U+28FF */
                nStart = 0x2800;
                nCount = 0x28FF - nStart + 1;
                memset(m_pRanges + nStart, 18, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 19: sUCRName = "Yi Syllables"; break; /* U+A000-U+A48F */ /* U+A490-U+A4CF */
                nStart = 0xA000;
                nCount = 0xA48F - nStart + 1;
                memset(m_pRanges + nStart, 19, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0xA490;
                nCount = 0xA4CF - nStart + 1;
                memset(m_pRanges + nStart, 19, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 20: sUCRName = "Tagalog"; break; /* U+1700-U+171F */ /* U+1720-U+173F */ /* U+1740-U+175F */ /* U+1760-U+177F */
                nStart = 0x1700;
                nCount = 0x171F - nStart + 1;
                memset(m_pRanges + nStart, 20, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0x1720;
                nCount = 0x173F - nStart + 1;
                memset(m_pRanges + nStart, 20, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0x1740;
                nCount = 0x175F - nStart + 1;
                memset(m_pRanges + nStart, 20, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                nStart = 0x1760;
                nCount = 0x177F - nStart + 1;
                memset(m_pRanges + nStart, 20, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 21: sUCRName = "Old Italic"; break; /*U+10300-U+1032F*/
                //case 22: sUCRName = "Gothic"; break; /*U+10330-U+1034F*/
                //case 23: sUCRName = "Deseret"; break; /*U+10400-U+1044F*/
                //case 24: sUCRName = "Byzantine Musical Symbols"; break; /*U+1D000-U+1D0FF*/ /*U+1D100-U+1D1FF*/ /*U+1D200-U+1D24F*/
                //case 25: sUCRName = "Mathematical Alphanumeric Symbols"; break; /*U+1D400-U+1D7FF*/
                //case 26: sUCRName = "Private Use (plane 15)"; break; /*U+F0000-U+FFFFD*/ /*U+100000-U+10FFFD*/ // Не юникодные символы

                //case 27: sUCRName = "Variation Selectors"; break; /* U+FE00-U+FE0F */ /*U+E0100-U+E01EF*/
                nStart = 0xFE00;
                nCount = 0xFE0F - nStart + 1;
                memset(m_pRanges + nStart, 27, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 28: sUCRName = "Tags"; break; /*U+E0000-U+E007F*/
                //case 29: sUCRName = "Limbu"; break; /* U+1900-U+194F */
                nStart = 0x1900;
                nCount = 0x194F - nStart + 1;
                memset(m_pRanges + nStart, 29, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 30: sUCRName = "Tai Le"; break; /* U+1950-U+197F */
                nStart = 0x1950;
                nCount = 0x197F - nStart + 1;
                memset(m_pRanges + nStart, 30, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                //case 31: sUCRName = "New Tai Lue"; break; /* U+1980-U+19DF */
                nStart = 0x1980;
                nCount = 0x19DF - nStart + 1;
                memset(m_pRanges + nStart, 31, nCount);
                memset(m_pRangesNums + nStart, 2, nCount);

                // rangeNum 3
                //case 00: sUCRName = "Buginese"; break; /* U+1A00-U+1A1F */
                nStart = 0x1A00;
                nCount = 0x1A1F - nStart + 1;
                memset(m_pRanges + nStart, 0, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 01: sUCRName = "Glagolitic"; break; /* U+2C00-U+2C5F */
                nStart = 0x2C00;
                nCount = 0x2C5F - nStart + 1;
                memset(m_pRanges + nStart, 1, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 02: sUCRName = "Tifinagh"; break; /* U+2D30-U+2D7F */
                nStart = 0x2D30;
                nCount = 0x2D7F - nStart + 1;
                memset(m_pRanges + nStart, 2, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 03: sUCRName = "Yijing Hexagram Symbols"; break; /* U+4DC0-U+4DFF */
                nStart = 0x4DC0;
                nCount = 0x4DFF - nStart + 1;
                memset(m_pRanges + nStart, 3, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 04: sUCRName = "Syloti Nagri"; break; /* U+A800-U+A82F */
                nStart = 0xA800;
                nCount = 0xA82F - nStart + 1;
                memset(m_pRanges + nStart, 4, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 05: sUCRName = "Linear B Syllabary"; break; /*U+10000-U+1007F*/ /*U+10080-U+100FF*/ /*U+10100-U+1013F*/
                //case 06: sUCRName = "Ancient Greek Numbers"; break; /*U+10140-U+1018F*/
                //case 07: sUCRName = "Ugaritic"; break; /*U+10380-U+1039F*/
                //case 08: sUCRName = "Old Persian"; break; /*U+103A0-U+103DF*/
                //case 09: sUCRName = "Shavian"; break; /*U+10450-U+1047F*/
                //case 10: sUCRName = "Osmanya"; break; /*U+10480-U+104AF*/
                //case 11: sUCRName = "Cypriot Syllabary"; break; /*U+10800-U+1083F*/
                //case 12: sUCRName = "Kharoshthi"; break; /*U+10A00-U+10A5F*/
                //case 13: sUCRName = "Tai Xuan Jing Symbols"; break; /*U+1D300-U+1D35F*/
                //case 14: sUCRName = "Cuneiform"; break; /*U+12000-U+123FF*/ /*U+12400-U+1247F*/
                //case 15: sUCRName = "Counting Rod Numerals"; break; /*U+1D360-U+1D37F*/

                //case 16: sUCRName = "Sundanese"; break; /* U+1B80-U+1BBF */
                nStart = 0x1B80;
                nCount = 0x1BBF - nStart + 1;
                memset(m_pRanges + nStart, 16, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 17: sUCRName = "Lepcha"; break; /* U+1C00-U+1C4F */
                nStart = 0x1C00;
                nCount = 0x1C4F - nStart + 1;
                memset(m_pRanges + nStart, 17, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 18: sUCRName = "Ol Chiki"; break; /* U+1C50-U+1C7F */
                nStart = 0x1C50;
                nCount = 0x1C7F - nStart + 1;
                memset(m_pRanges + nStart, 18, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 19: sUCRName = "Saurashtra"; break; /* U+A880-U+A8DF */
                nStart = 0xA880;
                nCount = 0xA8DF - nStart + 1;
                memset(m_pRanges + nStart, 19, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 20: sUCRName = "Kayah Li"; break; /* U+A900-U+A92F */
                nStart = 0xA900;
                nCount = 0xA92F - nStart + 1;
                memset(m_pRanges + nStart, 20, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 21: sUCRName = "Rejang"; break; /* U+A930-U+A95F */
                nStart = 0xA930;
                nCount = 0xA95F - nStart + 1;
                memset(m_pRanges + nStart, 21, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 22: sUCRName = "Cham"; break; /* U+AA00-U+AA5F */
                nStart = 0xAA00;
                nCount = 0xAA5F - nStart + 1;
                memset(m_pRanges + nStart, 22, nCount);
                memset(m_pRangesNums + nStart, 3, nCount);

                //case 23: sUCRName = "Ancient Symbols"; break; /*U+10190-U+101CF*/
                //case 24: sUCRName = "Phaistos Disc"; break; /*U+101D0-U+101FF*/
                //case 25: sUCRName = "Carian"; break; /*U+102A0-U+102DF*/ /*U+10280-U+1029F*/ /*U+10920-U+1093F*/
                //case 26: sUCRName = "Domino Tiles"; break; /*U+1F030-U+1F09F*/ /*U+1F000-U+1F02F*/
                //case 27: sUCRName = "Reserved for process-internal usage"; break;
                //case 28: sUCRName = "Reserved for process-internal usage"; break;
                //case 29: sUCRName = "Reserved for process-internal usage"; break;
                //case 30: sUCRName = "Reserved for process-internal usage"; break;
                //case 31: sUCRName = "Reserved for process-internal usage"; break;
            }

            inline void InitializeUnicodeMap()
            {
                memset(m_mapUnicode, 0, 0xFFFF);

                m_mapUnicode[0x0009] = 1;
                m_mapUnicode[0x000A] = 1;
                m_mapUnicode[0x000D] = 1;

                memset(m_mapUnicode + 0x0020, 1, 0xD7FF - 0x0020 + 1);
                memset(m_mapUnicode + 0xE000, 1, 0xFFFD - 0xE000 + 1);
            }

            inline bool GetRange(const WCHAR& symbol, BYTE& lRangeNum, BYTE& lRange)
            {
                lRangeNum	= m_pRangesNums[symbol];
                lRange		= m_pRanges[symbol];

                return (0xFF != lRangeNum);
            }

            inline void CheckRanges(DWORD& lRange1, DWORD& lRange2, DWORD& lRange3, DWORD& lRange4, const std::wstring& strText)
            {
                int lCount   = strText.length();
                const WCHAR* pData = strText.c_str();

                BYTE lRangeNum  = 0xFF;
                BYTE lRange		= 0xFF;
                for (int i = 0; i < lCount; ++i, ++pData)
                {
                    if (GetRange(*pData, lRangeNum, lRange))
                    {
                        if (0 == lRangeNum)
                            lRange1 |= 1 << lRange;
                        else if (1 == lRangeNum)
                            lRange2 |= 1 << lRange;
                        else if (2 == lRangeNum)
                            lRange3 |= 1 << lRange;
                        else
                            lRange4 |= 1 << lRange;
                    }
                }
            }
            inline void CheckRanges(UINT& lRange1, UINT& lRange2, UINT& lRange3, UINT& lRange4, BYTE& lRangeNum, BYTE& lRange)
            {
                if (0 == lRangeNum)
                    lRange1 |= 1 << lRange;
                else if (1 == lRangeNum)
                    lRange2 |= 1 << lRange;
                else if (2 == lRangeNum)
                    lRange3 |= 1 << lRange;
                else
                    lRange4 |= 1 << lRange;
            }

        public:
            inline static bool IsEqual3(NSStructures::CFont* f1, NSStructures::CFont* f2)
            {
                return ((f1->Path == f2->Path) && (f1->Bold == f2->Bold) && (f1->Italic == f2->Italic));
            }
            inline static bool IsEqual2(NSStructures::CFont* f1, NSStructures::CFont* f2, double dScalingSrcSize = 1)
            {
                if (!f1->Path.empty() && !f2->Path.empty())
                    return ((f1->Path == f2->Path) &&
                            (f1->StringGID == f2->StringGID) &&
                            (fabs(f1->Size - (f2->Size * dScalingSrcSize)) < 0.0001) &&
                            (f1->Bold == f2->Bold) &&
                            (f1->Italic == f2->Italic));

                return ((f1->Name == f2->Name) &&
                        (f1->Path == f2->Path) &&
                        (f1->StringGID == f2->StringGID) &&
                        (f1->Size == (f2->Size * dScalingSrcSize)) &&
                        (f1->Bold == f2->Bold) &&
                        (f1->Italic == f2->Italic));
            }

        public:
            inline bool GenerateFontName(NSStringUtils::CStringBuilder& oTextItem, bool bIsFontChanged)
            {
                if (m_oFont.m_oFont.Path.empty())
                    return false;

                BYTE lRangeNum	= 0xFF;
                BYTE lRange		= 0xFF;

                GetRange(oTextItem.GetBuffer()[0], lRangeNum, lRange);

                if (!bIsFontChanged && (0 != m_arListPicUps.size()))
                {
                    CFontPickUp& oPick = *m_arListPicUps.begin();

                    if ((lRangeNum == oPick.m_lRangeNum) && (lRange == oPick.m_lRange))
                    {
                        // по идее не надо приравнивать, они должны были быть выставлены ещ раньше
                        //m_strCurrentPickFont	= oPick.m_strPickFont;
                        //m_lCurrentPictFontStyle = oPick.m_lPickStyle;
                        return false;
                    }
                }

                std::list<CFontPickUp>::iterator pos = m_arListPicUps.begin();
                while (pos != m_arListPicUps.end())
                {
                    CFontPickUp& oPick = *pos;
                    pos++;

                    if ((IsEqual3(&m_oFont.m_oFont, &oPick.m_oFont.m_oFont)) && (lRangeNum == oPick.m_lRangeNum) && (lRange == oPick.m_lRange))
                    {
                        // нашли! ничего подбирать не нужно
                        // нужно просто выкинуть этот шрифт наверх
                        if (pos != m_arListPicUps.begin())
                        {
                            m_arListPicUps.splice(m_arListPicUps.begin(), m_arListPicUps, pos, std::next(pos));
                        }

                        m_strCurrentPickFont = oPick.m_strPickFont;
                        m_lCurrentPictFontStyle = oPick.m_lPickStyle;
                        return false;
                    }
                }

                // не нашли...
                CFontPickUp oPick;

                oPick.m_lRangeNum	= lRangeNum;
                oPick.m_lRange		= lRange;
                oPick.m_oFont		= m_oFont;
                oPick.m_strPickFont	= m_oFont.m_oProperties.m_strFamilyName;
                oPick.m_lPickStyle	= m_oFont.m_oProperties.m_lStyle;

                LONG lCountSigs = (LONG)m_oFont.m_oProperties.m_arSignature.size();
                UINT dwR1 = 0;
                if (0 < lCountSigs)
                    dwR1 = m_oFont.m_oProperties.m_arSignature[0];
                UINT dwR2 = 0;
                if (1 < lCountSigs)
                    dwR2 = m_oFont.m_oProperties.m_arSignature[1];
                UINT dwR3 = 0;
                if (2 < lCountSigs)
                    dwR3 = m_oFont.m_oProperties.m_arSignature[2];
                UINT dwR4 = 0;
                if (3 < lCountSigs)
                    dwR4 = m_oFont.m_oProperties.m_arSignature[3];

                DWORD dwCodePage1	= 0;
                DWORD dwCodePage2	= 0;

                if ((lRangeNum == 1) && (lRange == 28))
                {
                    dwCodePage1 = 0x80000000;
                    //strText = (WCHAR)(strText[0] - 0xF000);
                }
                else if (((lRangeNum == 2) && (lRange == 3)) || ((lRangeNum == 1) && (lRange == 31)) || ((lRangeNum == 0) && (lRange == 13)))
                {
                    // арабский язык!!!
                    dwR1 = 1 << 13;
                    dwR2 = 1 << 31;
                    dwR3 = 1 << 3;
                }
                else
                {
                    CheckRanges(dwR1, dwR2, dwR3, dwR4, lRangeNum, lRange);
                }

                NSFonts::CFontSelectFormat oFormat;
                oFormat.wsName = new std::wstring(m_oFont.m_oProperties.m_strFamilyName);
                oFormat.pPanose = new BYTE[10];
                memcpy(oFormat.pPanose, m_oFont.m_oProperties.m_strPANOSE, 10);
                oFormat.bBold = new INT(m_oFont.m_oFont.Bold);
                oFormat.bItalic = new INT(m_oFont.m_oFont.Italic);
                oFormat.bFixedWidth = new INT(m_oFont.m_oProperties.m_bIsFixedWidth ? 1 : 0);

                if (0 != dwR1)
                    oFormat.ulRange1 = new UINT(dwR1);
                if (0 != dwR2)
                    oFormat.ulRange2 = new UINT(dwR2);
                if (0 != dwR3)
                    oFormat.ulRange3 = new UINT(dwR3);
                if (0 != dwR4)
                    oFormat.ulRange4 = new UINT(dwR4);
                if (0 != dwCodePage1)
                    oFormat.ulCodeRange1 = new UINT(dwCodePage1);
                if (0 != dwCodePage2)
                    oFormat.ulCodeRange2 = new UINT(dwCodePage2);

                NSFonts::CFontInfo* pInfo = m_pManager->GetFontInfoByParams(oFormat);
                oPick.m_strPickFont = pInfo->m_wsFontName;
                oPick.m_lPickStyle	= m_oFont.m_oFont.GetStyle();
                m_strCurrentPickFont = oPick.m_strPickFont;
                m_lCurrentPictFontStyle = oPick.m_lPickStyle;

                m_arListPicUps.push_front(oPick);
                return true;
            }

            inline void SetStringGid(const LONG& lGid)
            {
                m_pManager->SetStringGID(lGid);
            }
            inline LONG GetStringGid()
            {
                return m_pManager->GetStringGID();
            }

            inline wchar_t* GetUnicodeString(wchar_t* bsText)
            {
                wchar_t* start = (wchar_t*)bsText;
                wchar_t* s = start;
                for (; *s != 0; ++s);

                LONG lLen = (LONG)(s - start);

                wchar_t* pData = new wchar_t[lLen + 1];
                for (LONG i = 0; i < lLen; ++i, ++start)
                {
                    if (m_mapUnicode[*start])
                        pData[i] = *start;
                    else
                        pData[i] = (wchar_t)(' ');
                }
                return pData;
            }

            std::wstring GetFontPath(NSStructures::CFont* pFont)
            {
                NSFonts::CFontSelectFormat oFormat;
                oFormat.bBold = new INT(pFont->Bold);
                oFormat.bItalic = new INT(pFont->Italic);
                oFormat.wsName = new std::wstring(pFont->Name);

                NSFonts::CFontInfo* pInfo = m_pManager->GetFontInfoByParams(oFormat);
                return pInfo->m_wsFontPath;
            }
        };
    }
}

#endif // _ASC_HTMLRENDERER_FMBASE_H_

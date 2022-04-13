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
#ifndef _ASC_HTMLRENDERER_TEXT_H_
#define _ASC_HTMLRENDERER_TEXT_H_

#include "FontManager.h"
#include "../../Common/OfficeFileFormats.h"
#include "Meta.h"

#ifdef min
#undef min
#endif
#include <algorithm>

namespace NSHtmlRenderer
{
    class CHChar
    {
    public:
        int 	unicode;	// юникодное значение
        int 	gid;		// индекс глифа в файле
        double	x;			// сдвиг по baseline
        double	width;		// ширина символа (сдвиг до след буквы)
        double* matrix;		// матрица преобразования (!!! без сдвига)

    public:
        CHChar()
        {
            unicode = 0;
            gid = 0;
            width = 0;
            matrix = NULL;
        }
        CHChar(const CHChar& oSrc)
        {
            *this = oSrc;
        }
        CHChar& operator=(const CHChar& oSrc)
        {
            unicode	= oSrc.unicode;
            gid		= oSrc.gid;
            width	= oSrc.width;
            matrix	= NULL;
            if (NULL != oSrc.matrix)
            {
                matrix = new double[4];
                memcpy(matrix, oSrc.matrix, 4 * sizeof(double));
            }
            return *this;
        }
        ~CHChar()
        {
            RELEASEARRAYOBJECTS(matrix);
        }

        inline void Clear()
        {
            unicode = 0;
            gid = 0;
            width = 0;

            RELEASEARRAYOBJECTS(matrix);
        }
    };

    class CHLine
    {
    public:
        double m_dAscent;
        double m_dDescent;
        double m_dX;
        double m_dY;

        double m_dEndX;
        double m_dEndY;

        // baseline ruler: y = k*x + b
        double m_dK;
        double m_dB;
        double m_ex;
        double m_ey;
        bool m_bIsConstX;

        // symbols
        // не менять на списки. постоянное создание объектов и их удаление -
        // плохо влияет на скорость
        CHChar*	m_pChars;
        LONG m_lSizeChars;
        LONG m_lCharsTail;

        bool m_bIsSetUpTransform;
        double m_sx;
        double m_sy;
        double m_shx;
        double m_shy;

    public:
        CHLine()
        {
            m_dAscent	= 0;
            m_dDescent	= 0;
            m_dX		= 0;
            m_dY		= 0;

            m_dK		= 0;
            m_dB		= 0;
            m_bIsConstX	= false;

            m_ex		= 0;
            m_ey		= 0;

            m_lSizeChars = 1000;
            m_lCharsTail = 0;
            m_pChars = new CHChar[m_lSizeChars];

            m_bIsSetUpTransform = false;
            m_sx = 1;
            m_sy = 1;
            m_shx = 0;
            m_shy = 0;
        }
        CHLine& operator=(const CHLine& oLine)
        {
            m_dAscent	= oLine.m_dAscent;
            m_dDescent	= oLine.m_dDescent;
            m_dX		= oLine.m_dX;
            m_dY		= oLine.m_dY;

            m_dK		= oLine.m_dK;
            m_dB		= oLine.m_dB;

            m_lSizeChars = oLine.m_lSizeChars;
            m_lCharsTail = oLine.m_lCharsTail;

            RELEASEARRAYOBJECTS(m_pChars);
            m_pChars = new CHChar[m_lSizeChars];

            for (LONG i = 0; i < m_lSizeChars; ++i)
                m_pChars[i] = oLine.m_pChars[i];

            m_bIsSetUpTransform = oLine.m_bIsSetUpTransform;
            m_sx = oLine.m_sx;
            m_sy = oLine.m_sy;
            m_shx = oLine.m_shx;
            m_shy = oLine.m_shy;

            return *this;
        }
        ~CHLine()
        {
            RELEASEARRAYOBJECTS(m_pChars);
        }

        inline void Clear()
        {
            m_dAscent	= 0;
            m_dDescent	= 0;
            m_dX		= 0;
            m_dY		= 0;

            m_dK		= 0;
            m_dB		= 0;
            m_bIsConstX	= false;

            m_ex		= 0;
            m_ey		= 0;

            m_lCharsTail = 0;

            m_bIsSetUpTransform = false;
            m_sx = 1;
            m_sy = 1;
            m_shx = 0;
            m_shy = 0;
        }

        inline CHChar* AddTail()
        {
            if (m_lCharsTail >= m_lSizeChars)
            {
                CHChar* pNews = new CHChar[2 * m_lSizeChars];
                for (LONG i = 0; i < m_lSizeChars; ++i)
                {
                    pNews[i] = m_pChars[i];
                }

                RELEASEARRAYOBJECTS(m_pChars);
                m_pChars = pNews;
                m_lSizeChars *= 2;
            }

            CHChar* pChar = &m_pChars[m_lCharsTail];
            ++m_lCharsTail;
            pChar->Clear();

            return pChar;
        }

        inline CHChar* GetTail()
        {
            if (0 == m_lCharsTail)
                return NULL;

            return &m_pChars[m_lCharsTail - 1];
        }

        inline LONG GetCountChars()
        {
            return m_lCharsTail;
        }
    };

    const BYTE g_lfHorizontal	= 0x01;
    const BYTE g_lfGids			= 0x02;
    const BYTE g_lfWidth		= 0x04;

    class CHFontInfo
    {
    public:
        USHORT m_lAscent;
        USHORT m_lDescent;
        USHORT m_lLineHeight;
        USHORT m_lUnitsPerEm;

    public:
        CHFontInfo()
        {
            m_lAscent		= 0;
            m_lDescent		= 0;
            m_lLineHeight	= 0;
            m_lUnitsPerEm	= 0;
        }

        CHFontInfo(const CHFontInfo& oSrc)
        {
            m_lAscent		= oSrc.m_lAscent;
            m_lDescent		= oSrc.m_lDescent;
            m_lLineHeight	= oSrc.m_lLineHeight;
            m_lUnitsPerEm	= oSrc.m_lUnitsPerEm;
        }

        CHFontInfo& operator=(const CHFontInfo& oSrc)
        {
            m_lAscent		= oSrc.m_lAscent;
            m_lDescent		= oSrc.m_lDescent;
            m_lLineHeight	= oSrc.m_lLineHeight;
            m_lUnitsPerEm	= oSrc.m_lUnitsPerEm;

            return *this;
        }
    };

    class CFontMapInfo
    {
    public:
        std::wstring Path;
        int FaceIndex;

    public:
        CFontMapInfo()
        {
            Path = L"";
            FaceIndex = 0;
        }
        CFontMapInfo(const CFontMapInfo& oSrc)
        {
            Path = oSrc.Path;
            FaceIndex = oSrc.FaceIndex;
        }
        CFontMapInfo& operator=(const CFontMapInfo& oSrc)
        {
            Path = oSrc.Path;
            FaceIndex = oSrc.FaceIndex;

            return *this;
        }
    };

    class CFontManagerWrapper
    {
    private:
        NSFonts::IFontManager*	m_pManager;
    public:
        CHFontInfo m_oCurrentInfo;
        NSStructures::CFont*			m_pFont;

        std::map<std::wstring, CHFontInfo> m_mapInfos;

    public:
        CFontManagerWrapper()
        {
            m_pManager = NULL;
        }
        void Init(NSFonts::IApplicationFonts* pApplicationFonts, int nCacheSize = 0)
        {
            RELEASEOBJECT(m_pManager);
            m_pManager = pApplicationFonts->GenerateFontManager();

            if (0 != nCacheSize)
            {
                NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
                pFontCache->SetStreams(pApplicationFonts->GetStreams());
                pFontCache->SetCacheSize(nCacheSize);
                m_pManager->SetOwnerCache(pFontCache);
            }
        }

        virtual ~CFontManagerWrapper()
        {
            RELEASEOBJECT(m_pManager);
        }

        void SetStringGID(INT bGid)
        {
            m_pManager->SetStringGID(bGid);
        }

    public:
        inline void LoadCurrentFont(bool bIsAttack, int lFaceIndex = 0)
        {
            if (m_pFont->Path.empty())
            {
                std::wstring sFind = m_pFont->Name + L"__ASC_FONT__" + std::to_wstring(m_pFont->GetStyle());

                std::map<std::wstring, CHFontInfo>::const_iterator pPair = m_mapInfos.find(sFind);
                if (m_mapInfos.end() != pPair)
                {
                    if (bIsAttack)
                    {
                        LoadFontByName(m_pFont->Name, m_pFont->Size, m_pFont->GetStyle());
                    }
                    else
                    {
                        m_oCurrentInfo = pPair->second;
                    }
                }
                else
                {
                    LoadFontByName(m_pFont->Name, m_pFont->Size, m_pFont->GetStyle());
                    m_mapInfos.insert(std::pair<std::wstring, CHFontInfo>(sFind, m_oCurrentInfo));
                }
            }
            else
            {
                std::map<std::wstring, CHFontInfo>::const_iterator pPair = m_mapInfos.find(m_pFont->Path);
                if (m_mapInfos.end() != pPair)
                {
                    if (bIsAttack)
                    {
                        LoadFontByFile(m_pFont->Path, m_pFont->Size, lFaceIndex);
                    }
                    else
                    {
                        m_oCurrentInfo = pPair->second;
                    }
                }
                else
                {
                    LoadFontByFile(m_pFont->Path, m_pFont->Size, lFaceIndex);
                    m_mapInfos.insert(std::pair<std::wstring, CHFontInfo>(m_pFont->Path, m_oCurrentInfo));
                }
            }
        }

        inline void LoadFontByName(const std::wstring& strName, const double& dSize, const LONG& lStyle)
        {
            m_pManager->LoadFontByName(strName, dSize, lStyle, c_dDpi, c_dDpi);
            LoadFontMetrics();
        }

        inline void LoadFontByFile(const std::wstring& strPath, const double& dSize, const int& lFaceIndex)
        {
            m_pManager->LoadFontFromFile(strPath, lFaceIndex, dSize, c_dDpi, c_dDpi);
            LoadFontMetrics();
        }

    public:

        double MeasureString(const unsigned int* symbols, const int& count, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight)
        {
            dBoxX		= 0;
            dBoxY		= 0;
            dBoxWidth	= 0;
            dBoxHeight	= 0;

            if (NULL == m_pManager)
                return 0;

            m_pManager->LoadString1(symbols, count, (float)x, (float)y);

            TBBox _box  = m_pManager->MeasureString2();

            dBoxX		= (double)(_box.fMinX);
            dBoxY		= (double)(_box.fMinY);
            dBoxWidth	= (double)(_box.fMaxX - _box.fMinX);
            dBoxHeight	= (double)(_box.fMaxY - _box.fMinY);

            if (dBoxWidth < 0)
                dBoxWidth = -dBoxWidth;
            if (dBoxHeight < 0)
                dBoxHeight = -dBoxHeight;

            // переводим в миллиметры
            dBoxX		*= c_dPixToMM;
            dBoxY		*= c_dPixToMM;
            dBoxWidth	*= c_dPixToMM;
            dBoxHeight	*= c_dPixToMM;

            return dBoxWidth;
        }

    public:
        void LoadFontMetrics()
        {
            m_pManager->AfterLoad();
            int lA = m_pManager->GetAscender();
            int lD = m_pManager->GetDescender();
            int lL = m_pManager->GetLineHeight();
            int lU = m_pManager->GetUnitsPerEm();

            if (lA < 0)
                lA = -lA;
            if (lD < 0)
                lD = -lD;
            if (lL < 0)
                lL = -lL;
            if (lU < 0)
                lU = -lU;

            m_oCurrentInfo.m_lAscent = (USHORT)(lA);
            m_oCurrentInfo.m_lDescent = (USHORT)(lD);
            m_oCurrentInfo.m_lLineHeight = (USHORT)(lL);
            m_oCurrentInfo.m_lUnitsPerEm = (USHORT)(lU);
        }
    };

    class CHText
    {
    public:
        CFontManagerWrapper	m_oFontManager;

        CHLine m_oLine;
        CMetafile* m_pMeta;

        NSStructures::CBrush*	m_pBrush;
        NSStructures::CFont*	m_pFont;

        NSStructures::CBrush*	m_pLastBrush;

        Aggplus::CMatrix*		m_pTransform;
        Aggplus::CMatrix*		m_pLastTransform;

        CMetafile				m_oMeta;
        CMetafile*				m_pPageMeta;

        double					m_dTextSpaceEps;

        LONG m_lCountParagraphs;
        LONG m_lCountWords;
        LONG m_lCountSymbols;
        LONG m_lCountSpaces;

    public:
        CHText() : m_oFontManager(), m_oLine()
        {
            m_dTextSpaceEps = 0.1;

            m_lCountParagraphs = 0;
            m_lCountWords = 0;
            m_lCountSymbols = 0;
            m_lCountSpaces = 0;
        }
        void Init(NSFonts::IApplicationFonts* pApplicationFonts, int nCacheSize = 0)
        {
            m_oFontManager.Init(pApplicationFonts, nCacheSize);
        }

        void ClearStatistics()
        {
            m_lCountParagraphs = 0;
            m_lCountWords = 0;
            m_lCountSymbols = 0;
            m_lCountSpaces = 0;
        }

        template<typename T>
        void SetParams(T writer)
        {
            m_oFontManager.m_pFont = writer->m_pFont;

            m_pBrush	= writer->m_pBrush;
            m_pFont		= writer->m_pFont;

            m_pLastBrush	= &writer->m_oLastBrush;

            m_pTransform		= writer->m_pTransform;
            m_pLastTransform	= &writer->m_oLastTransform;

            m_pPageMeta			= &writer->m_oPage;

            switch (writer->m_lSrcFileType)
            {
            case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
            case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:
                break;
            default:
                m_dTextSpaceEps = 0.1;
                break;
            }
        }

        ~CHText()
        {
        }

        void NewPage()
        {
            m_oMeta.ClearNoAttack();
        }
        void ClosePage()
        {
            LONG nCount = m_oLine.GetCountChars();
            if (0 != nCount)
                DumpLine();
        }

    public:
        template<typename T>
        void CommandText(const int* pUnicodes, const int* pGids, const int& nCount,
                         const double& x, const double& y, const double& width, const double& height, bool bIsDumpFont, T writer)
        {
            // 1) сначала определяем точку отсчета и направление baseline
            double _x1 = x;
            double _y1 = y;
            double _x2 = x + 1;
            double _y2 = y;
            m_pTransform->TransformPoint(_x1, _y1);
            m_pTransform->TransformPoint(_x2, _y2);

            double _k = 0;
            double _b = 0;
            bool _isConstX = false;
            if (fabs(_x1 - _x2) < 0.001)
            {
                _isConstX = true;
                _b = _x1;
            }
            else
            {
                _k = (_y1 - _y2) / (_x1 - _x2);
                _b = _y1 - _k * _x1;
            }

            double dAbsVec = sqrt((_x1 - _x2) * (_x1 - _x2) + (_y1 - _y2) * (_y1 - _y2));
            if (dAbsVec == 0)
                dAbsVec = 1;

            LONG nCountChars = m_oLine.GetCountChars();

            bool bIsNewLine = true;

            if (0 != nCountChars)
            {
                if (_isConstX && m_oLine.m_bIsConstX && fabs(_b - m_oLine.m_dB) < 0.001)
                    bIsNewLine = false;
                else if (!_isConstX && !m_oLine.m_bIsConstX && fabs(_k - m_oLine.m_dK) < 0.001 && fabs(_b - m_oLine.m_dB) < 0.001)
                    bIsNewLine = false;
            }

            if (bIsNewLine && (0 != nCountChars))
            {
                // не совпала baseline. поэтому просто скидываем линию в поток
                DumpLine();
            }

            // теперь нужно определить сдвиг по baseline относительно destination точки
            nCountChars = m_oLine.GetCountChars();
            double dOffsetX = 0;
            if (0 == nCountChars)
            {
                m_oLine.m_bIsConstX = _isConstX;
                m_oLine.m_dK = _k;
                m_oLine.m_dB = _b;

                m_oLine.m_dX = _x1;
                m_oLine.m_dY = _y1;

                m_oLine.m_ex = (_x2 - _x1) / dAbsVec;
                m_oLine.m_ey = (_y2 - _y1) / dAbsVec;

                m_oLine.m_dEndX = _x1;
                m_oLine.m_dEndY = _y1;
            }
            else
            {
                double sx = _x1 - m_oLine.m_dEndX;
                double sy = _y1 - m_oLine.m_dEndY;
                double len = sqrt(sx*sx + sy*sy);

                if (sx*m_oLine.m_ex >= 0 && sy*m_oLine.m_ey >= 0)
                {
                    // продолжаем линию
                    dOffsetX = len;

                    // теперь посмотрим, может быть нужно вставить пробел??
                    CHChar* pLastChar = m_oLine.GetTail();
                    if (dOffsetX > (pLastChar->width + 0.5))
                    {
                        // вставляем пробел. Пробел у нас будет не совсем пробел. А специфический
                        CHChar* pSpaceChar = m_oLine.AddTail();
                        pSpaceChar->x = pLastChar->width;
                        pSpaceChar->width = dOffsetX - pLastChar->width;
                        pSpaceChar->unicode = 0xFFFF;
                        pSpaceChar->gid = 0xFFFF;
                        dOffsetX -= pLastChar->width;

                        m_oMeta.WriteBYTE(0);
                    }
                }
                else
                {
                    // буква сдвинута влево относительно предыдущей буквы
                    // на такую ситуацию реагируем просто - просто начинаем новую линию,
                    // предварительно сбросив старую
                    DumpLine();

                    m_oLine.m_bIsConstX = _isConstX;

                    m_oLine.m_dX = _x1;
                    m_oLine.m_dY = _y1;

                    m_oLine.m_dK = _k;
                    m_oLine.m_dB = _b;

                    m_oLine.m_ex = (_x2 - _x1) / dAbsVec;
                    m_oLine.m_ey = (_y2 - _y1) / dAbsVec;
                }

                m_oLine.m_dEndX = _x1;
                m_oLine.m_dEndY = _y1;
            }

            // смотрим, совпадает ли главная часть матрицы.
            bool bIsTransform = !IsEqualMain(m_pLastTransform, m_pTransform);
            if (bIsTransform)
                bIsDumpFont = true;

            bool bIsColor = ((m_pBrush->Color1 != m_pLastBrush->Color1) || (m_pBrush->Alpha1 != m_pLastBrush->Alpha1));

            BYTE nLenMetaCommands = 0;
            if (bIsColor)
                nLenMetaCommands += 5;
            if (bIsTransform)
                nLenMetaCommands += 17;
            if (bIsDumpFont)
                nLenMetaCommands += 13;

            m_oMeta.WriteBYTE(nLenMetaCommands);

            double _dumpSize = writer->m_dCurrentFontSize;
            double _dumpMtx[4];
            _dumpMtx[0] = m_pTransform->sx();
            _dumpMtx[1] = m_pTransform->shy();
            _dumpMtx[2] = m_pTransform->shx();
            _dumpMtx[3] = m_pTransform->sy();

            double dTextScale = std::min( sqrt( _dumpMtx[2] * _dumpMtx[2] + _dumpMtx[3] * _dumpMtx[3] ), sqrt( _dumpMtx[0] * _dumpMtx[0] + _dumpMtx[1] * _dumpMtx[1] ) );

            if ((_dumpSize < 0.1 && dTextScale > 10) || (_dumpSize > 10 && dTextScale < 0.1))
            {
                _dumpSize *= dTextScale;

                _dumpMtx[0] /= dTextScale;
                _dumpMtx[1] /= dTextScale;
                _dumpMtx[2] /= dTextScale;
                _dumpMtx[3] /= dTextScale;
            }

            if (bIsDumpFont)
            {
                m_oMeta.WriteCommandType(CMetafile::ctFontName);
                m_oMeta.WriteLONG(writer->m_lCurrentFont);
                m_oMeta.WriteLONG(writer->m_pFont->GetStyle());
                m_oMeta.WriteDouble(_dumpSize/*writer->m_dCurrentFontSize*/);
            }
            if (bIsTransform)
            {
                m_pLastTransform->SetElements(m_pTransform->sx(), m_pTransform->shy(), m_pTransform->shx(), m_pTransform->sy(), m_pLastTransform->tx(), m_pLastTransform->ty());

                m_oLine.m_bIsSetUpTransform = true;
                m_oLine.m_sx = m_pTransform->sx();
                m_oLine.m_shx = m_pTransform->shx();
                m_oLine.m_shy = m_pTransform->shy();
                m_oLine.m_sy = m_pTransform->sy();

                m_oMeta.WriteBYTE(CMetafile::ctCommandTextTransform);
                //m_oMeta.WriteDouble(_dst->sx);
                //m_oMeta.WriteDouble(_dst->shy);
                //m_oMeta.WriteDouble(_dst->shx);
                //m_oMeta.WriteDouble(_dst->sy);

                m_oMeta.WriteDouble(_dumpMtx[0]);
                m_oMeta.WriteDouble(_dumpMtx[1]);
                m_oMeta.WriteDouble(_dumpMtx[2]);
                m_oMeta.WriteDouble(_dumpMtx[3]);
            }
            if (bIsColor)
            {
                m_pLastBrush->Color1 = m_pBrush->Color1;
                m_pLastBrush->Alpha1 = m_pBrush->Alpha1;

                m_oMeta.WriteBYTE(CMetafile::ctBrushColor1);

                LONG lBGR = m_pBrush->Color1;
                m_oMeta.WriteBYTE((BYTE)(lBGR & 0xFF));
                m_oMeta.WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
                m_oMeta.WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
                m_oMeta.WriteBYTE((BYTE)m_pBrush->Alpha1);
            }

            // все, baseline установлен. теперь просто продолжаем линию
            LONG lTextLen = nCount;
            bool bIsLoadFontAttack = true;

            // плохие значения приходят из пдф
            /*
            if (1 == lTextLen && 0 <= width)
                bIsLoadFontAttack = false;
            */

            if (bIsDumpFont)
                m_oFontManager.LoadCurrentFont(bIsLoadFontAttack);

            double dKoef = m_oFontManager.m_pFont->Size * 25.4 / (72 * m_oFontManager.m_oCurrentInfo.m_lUnitsPerEm);
            double dKoefMetr = dAbsVec;
            double dAscender = m_oFontManager.m_oCurrentInfo.m_lAscent * dKoef * dKoefMetr;
            double dDescender = m_oFontManager.m_oCurrentInfo.m_lDescent * dKoef * dKoefMetr;

            if (m_oLine.m_dAscent < dAscender)
                m_oLine.m_dAscent = dAscender;
            if (m_oLine.m_dDescent < dDescender)
                m_oLine.m_dDescent = dDescender;

            if (!bIsLoadFontAttack)
            {
                CHChar* pChar = m_oLine.AddTail();

                pChar->unicode = pUnicodes[0];
                pChar->gid = (NULL == pGids) ? 0xFFFF : pGids[0];
                pChar->width = width;
                pChar->x = dOffsetX;
            }
            else
            {
                double dPlusOffset = 0;

                const int* input = NULL;
                if (NULL != pGids)
                {
                    input = pGids;
                    m_oFontManager.SetStringGID(TRUE);
                }
                else
                {
                    input = pUnicodes;
                    m_oFontManager.SetStringGID(FALSE);
                }

                double dBoxX = 0;
                double dBoxY = 0;
                double dBoxW = 0;
                double dBoxH = 0;

                double dPrevW = dOffsetX;
                for (LONG lIndex = 0; lIndex < lTextLen; ++lIndex)
                {
                    double dW = m_oFontManager.MeasureString((const unsigned int*)(input + lIndex), 1, 0, 0, dBoxX, dBoxY, dBoxW, dBoxH);

                    CHChar* pChar = m_oLine.AddTail();
                    pChar->unicode = pUnicodes[lIndex];
                    pChar->gid = (NULL == pGids) ? 0xFFFF : pGids[lIndex];

                    pChar->x = dPrevW;
                    if (lIndex != 0)
                        dPlusOffset += dPrevW;
                    dPrevW = dW;

                    pChar->width = dW * dAbsVec;

                    if (0 != lIndex)
                        m_oMeta.WriteBYTE(0);

                    if (lIndex == (lTextLen - 1))
                    {
                        m_oLine.m_dEndX += dPlusOffset * m_oLine.m_ex;
                        m_oLine.m_dEndY += dPlusOffset * m_oLine.m_ey;
                    }
                }
            }
        }

        void DumpLine()
        {
            if (m_oLine.m_bIsSetUpTransform)
            {
                // выставится трансформ!!!
                // cравнивать нужно с ним!!!
                m_pLastTransform->SetElements(m_oLine.m_sx, m_oLine.m_shy, m_oLine.m_shx, m_oLine.m_sy);
            }

            // скидываем линию в поток pMeta
            BYTE mask = 0;
            if (fabs(m_oLine.m_ex - 1.0) < 0.001 && fabs(m_oLine.m_ey) < 0.001)
                mask |= g_lfHorizontal;

            LONG lCountSpaces = 0;
            LONG lCountSymbols = 0;
            LONG lCountWords = 0;
            bool bIsLastSymbol = false;

            bool bIsGidExist = false;

            LONG nCount = m_oLine.GetCountChars();
            for (LONG i = 0; i < nCount; ++i)
            {
                CHChar* pChar = &m_oLine.m_pChars[i];
                if (pChar->gid != 0xFFFF)
                {
                    mask |= g_lfGids;
                    bIsGidExist = true;
                }

                if (0xFFFF == pChar->unicode || ((WCHAR)' ') == pChar->unicode || ((WCHAR)'\t') == pChar->unicode)
                {
                    lCountSpaces++;
                    if (bIsLastSymbol)
                    {
                        bIsLastSymbol = false;
                        lCountWords++;
                    }
                }
                else
                {
                    lCountSymbols++;
                    bIsLastSymbol = true;
                }
            }

            if (bIsLastSymbol)
                lCountWords++;

            if (0 == nCount)
            {
                m_oLine.Clear();
                m_oMeta.ClearNoAttack();
                return;
            }

            m_lCountParagraphs += 1;
            m_lCountWords += lCountWords;
            m_lCountSymbols += lCountSymbols;
            m_lCountSpaces += lCountSpaces;

            if (nCount > 1)
                mask |= g_lfWidth;

            m_pPageMeta->CheckBufferSize(60);

            m_pPageMeta->WriteBYTE_nocheck(CMetafile::ctCommandTextLine);
            m_pPageMeta->WriteBYTE_nocheck(mask);

            m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dX);
            m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dY);

            if ((mask & g_lfHorizontal) == 0)
            {
                m_pPageMeta->WriteDouble_nocheck(m_oLine.m_ex);
                m_pPageMeta->WriteDouble_nocheck(m_oLine.m_ey);
            }

            m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dAscent);
            m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dDescent);

            LONG _position = 0;
            if (nCount > 1)
            {
                _position = m_pPageMeta->GetPosition();
                m_pPageMeta->WriteLONG_nocheck(0);
            }

            BYTE* pBufferMeta = m_oMeta.GetData();
            double dWidthLine = 0;

            double dCurrentGlyphLineOffset = 0;
            for (LONG lIndexChar = 0; lIndexChar < nCount; ++lIndexChar)
            {
                CHChar* pChar = &m_oLine.m_pChars[lIndexChar];

                // все настроки буквы (m_oMeta)
                BYTE lLen = *pBufferMeta;
                ++pBufferMeta;
                if (lLen > 0)
                {
                    m_pPageMeta->Write(pBufferMeta, lLen);
                }
                pBufferMeta += lLen;
                // смещение относительно предыдущей буквы (у всех, кроме первой)
                // юникодное значение
                // гид (если bIsGidExist == true)
                // ширина буквы

                m_pPageMeta->CheckBufferSize(20);

                m_pPageMeta->WriteBYTE_nocheck(CMetafile::ctDrawText);
                if (0 != lIndexChar)
                {
                    m_pPageMeta->WriteDouble2_nocheck(pChar->x);
                }

                m_pPageMeta->WriteWCHAR_nocheck2(pChar->unicode);
                if (bIsGidExist)
                    m_pPageMeta->WriteUSHORT_nocheck(pChar->gid);
                m_pPageMeta->WriteDouble2_nocheck(pChar->width);

                if (lIndexChar != 0)
                    dCurrentGlyphLineOffset += pChar->x;

                if (lIndexChar == (nCount - 1))
                    dWidthLine = dCurrentGlyphLineOffset + pChar->width;
            }

            if (nCount > 1)
            {
                int nWidthBuf = (int)(dWidthLine * 10000);
                memcpy(m_pPageMeta->GetData() + _position, &nWidthBuf, 4);
            }

            m_oLine.Clear();
            m_oMeta.ClearNoAttack();

            m_pPageMeta->WriteBYTE(CMetafile::ctCommandTextLineEnd);
        }
    };
}

#endif // _ASC_HTMLRENDERER_TEXT_H_

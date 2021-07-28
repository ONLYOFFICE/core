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
#pragma once

#include "libdjvu/DjVuDocument.h"
#include "libdjvu/DjVuImage.h"
#include "libdjvu/GBitmap.h"
#include "libdjvu/GScaler.h"
#include "libdjvu/IFFByteStream.h"
#include "libdjvu/BSByteStream.h"
#include "libdjvu/DataPool.h"
#include "libdjvu/DjVuText.h"
#include "libdjvu/DjVmNav.h"
#include "libdjvu/IW44Image.h"

#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)

#define UNICODE
#define _UNICODE
#define _USE_LIBXML2_READER_
#define LIBXML_READER_ENABLED
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"

class CDjVuFileImplementation
{
private:

	std::wstring     m_wsTempDirectory;
	GP<DjVuDocument> m_pDoc;
    NSFonts::IApplicationFonts* m_pApplicationFonts;

public:

    CDjVuFileImplementation(NSFonts::IApplicationFonts* pFonts);
	~CDjVuFileImplementation();

	bool         LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions = L"");
    bool         LoadFromMemory(BYTE* data, DWORD length, const std::wstring& wsXmlOptions = L"");
	void         Close();
	std::wstring GetTempDirectory() const;
	void         SetTempDirectory(const std::wstring& wsDirectory);
	int          GetPagesCount() const;
	void         GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) const;
	void         DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, double dKoefX = 0, double dKoefY = 0);
    BYTE*        ConvertToPixels(int nPageIndex, const int& nRasterW = -1, const int& nRasterH = -1);
    void         ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType, const int& nRasterW = -1, const int& nRasterH = -1);
    void         ConvertToPdf(const std::wstring& wsDstPath);
    #ifdef WASM_MODE
    class        CData
    {
    protected:
        unsigned char* m_pData;
        size_t m_lSize;

        unsigned char* m_pDataCur;
        size_t m_lSizeCur;

    public:
        CData()
        {
            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur = m_pData;
            m_lSizeCur = m_lSize;
        }
        virtual ~CData()
        {
            Clear();
        }

        inline void AddSize(size_t nSize)
        {
            if (NULL == m_pData)
            {
                m_lSize = 1000;
                if (nSize > m_lSize)
                    m_lSize = nSize;

                m_pData = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));

                m_lSizeCur = 0;
                m_pDataCur = m_pData;
                return;
            }

            if ((m_lSizeCur + nSize) > m_lSize)
            {
                while ((m_lSizeCur + nSize) > m_lSize)
                    m_lSize *= 2;

                unsigned char* pRealloc = (unsigned char*)realloc(m_pData, m_lSize * sizeof(unsigned char));
                if (NULL != pRealloc)
                {
                    m_pData    = pRealloc;
                    m_pDataCur = m_pData + m_lSizeCur;
                }
                else
                {
                    unsigned char* pMalloc = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));
                    memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(unsigned char));

                    free(m_pData);
                    m_pData    = pMalloc;
                    m_pDataCur = m_pData + m_lSizeCur;
                }
            }
        }

    public:
        void AddInt(unsigned int value)
        {
            AddSize(4);
            memcpy(m_pDataCur, &value, sizeof(unsigned int));
            m_pDataCur += 4;
            m_lSizeCur += 4;
        }
        void WriteString(unsigned char* value, unsigned int len)
        {
            AddSize(len + 4);
            memcpy(m_pDataCur, &len, sizeof(unsigned int));
            m_pDataCur += 4;
            m_lSizeCur += 4;
            memcpy(m_pDataCur, value, len);
            m_pDataCur += len;
            m_lSizeCur += len;
        }
        unsigned char* GetBuffer()
        {
            return m_pData;
        }

        void Clear()
        {
            if (m_pData) free(m_pData);

            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur = m_pData;
            m_lSizeCur = 0;
        }
        void ClearWithoutAttack()
        {
            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur = m_pData;
            m_lSizeCur = 0;
        }
        void ClearNoAttack()
        {
            m_pDataCur = m_pData;
            m_lSizeCur = 0;
        }
        unsigned int GetSize()
        {
            return (unsigned int)m_lSizeCur;
        }

        void SkipLen()
        {
            AddInt(0);
        }
        void WriteLen()
        {
            unsigned int len = (unsigned int)m_lSizeCur;
            memcpy(m_pData, &len, sizeof(unsigned int));
        }
    };
    BYTE*        GetStructure();
    BYTE*        GetPageGlyphs();
    CData*       m_pGlyphs;
    #endif

private:

	void               CreateFrame(IRenderer* pRenderer, GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& oText);
	void               CreatePdfFrame(IRenderer* pRenderer, GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& oText);
	void               CreateGrFrame(IRenderer* pRenderer, GP<DjVuImage>& pImage, bool* pBreak);
	XmlUtils::CXmlNode ParseText(GP<DjVuImage> pPage, double dKoefX, double dKoefY);
	void               TextToRenderer(IRenderer* pRenderer, XmlUtils::CXmlNode text, double koef, bool isView = true);
	void               DrawPageText(IRenderer* pRenderer, double* pdCoords, const std::wstring& wsText);
	void               ParseCoords(const std::wstring& wsCoordsStr, double* pdCoords, double dKoef);
};

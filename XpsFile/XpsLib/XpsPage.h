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
#ifndef _XPS_XPSLIB_PAGE_H
#define _XPS_XPSLIB_PAGE_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#include "FontList.h"
#include "Utils.h"
#include "ContextState.h"

namespace XPS
{
	class CDocument;
	class CStaticResource;

	class Page
	{
	public:
        Page(const std::wstring& wsFile, IFolder* Path, CFontList* pFontList, NSFonts::IFontManager* pFontManager, CDocument* pDocument);
		~Page();

		void GetSize(int& nW, int& nH) const;
		void Draw(IRenderer* pRenderer, bool* pbBreak, int nRasterW, int nRasterH);
		#ifdef BUILDING_WASM_MODULE
		BYTE* GetGlyphs();
        BYTE* GetLinks();
        bool  CompareWH(int nRasterW, int nRasterH);
        class CData
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
    private:

        CData* m_pGlyphs;
        struct CPageLink
        {
            double dX;
            double dY;
            double dW;
            double dH;
            std::wstring sLink;
        };
        std::vector<CPageLink> m_vLinks;
        int nLastW, nLastH;
        #endif

	private:

		void DrawCanvas      (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak);
		bool ReadResource    (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawGlyph       (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawPath        (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		bool StrokeToRenderer(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void ReadPathData    (XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTranform);
		
		bool ClipToRenderer     (const wchar_t* wsString, CContextState* pState);
		bool TransformToRenderer(const wchar_t* wsString, CContextState* pState);

	private:

        std::wstring            m_wsPagePath;
        IFolder*                m_wsRootPath;
        CFontList*              m_pFontList;
        NSFonts::IFontManager*  m_pFontManager;
        CDocument*              m_pDocument;
	};
}

#endif // _XPS_XPSLIB_PAGE_H

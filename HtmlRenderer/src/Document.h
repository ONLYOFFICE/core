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
#ifndef _ASC_HTMLRENDERER_DOCUMENT_H_
#define _ASC_HTMLRENDERER_DOCUMENT_H_

#include "Common.h"
#include <vector>
#include "../../DesktopEditor/graphics/pro/Graphics.h"

namespace NSHtmlRenderer
{
	class CPageInfo
	{
	private:
		double m_dWidthMM;
		double m_dHeightMM;

		bool m_bInitW;
		bool m_bInitH;

		IBaseMatrixUpdater* m_pUpdater;

	public:
		CPageInfo()
		{
			m_dWidthMM	= 190;
			m_dHeightMM = 270;

			m_bInitW	= false;
			m_bInitH	= false;

			m_pUpdater	= NULL;
		}
		CPageInfo(const CPageInfo& oSrc)
		{
			*this = oSrc;
		}
		CPageInfo& operator=(const CPageInfo& oSrc)
		{
			m_dWidthMM	= oSrc.m_dWidthMM;
			m_dHeightMM = oSrc.m_dHeightMM;

			m_bInitW	= oSrc.m_bInitW;
			m_bInitH	= oSrc.m_bInitH;

			m_pUpdater	= oSrc.m_pUpdater;
            return *this;
		}

		inline double GetWidth()
		{
			return m_dWidthMM;
		}
		inline double GetHeight()
		{
			return m_dHeightMM;
		}
		inline void SetWidth(const double& width)
		{
			m_dWidthMM	= width;
			m_bInitW	= true;

			if (m_bInitH && (NULL != m_pUpdater))
			{
				m_pUpdater->OnBaseMatrixUpdate(m_dWidthMM, m_dHeightMM);
			}
		}
		inline void SetHeight(const double& height)
		{
			m_dHeightMM	= height;
			m_bInitH	= true;

			if (m_bInitW && (NULL != m_pUpdater))
			{
				m_pUpdater->OnBaseMatrixUpdate(m_dWidthMM, m_dHeightMM);
			}
		}
		inline void SetUpdater(IBaseMatrixUpdater* pUpdater)
		{
			m_pUpdater = pUpdater;
		}
	};

	class CDocument
	{
	private:
		
	public:
        std::vector<CPageInfo> m_arrPages;
		IBaseMatrixUpdater*	 m_pUpdater;	

	public:
		CDocument() : m_arrPages()
		{
			m_pUpdater = NULL;
		}
		inline void SetUpdater(IBaseMatrixUpdater* pUpdater)
		{
			m_pUpdater = pUpdater;
		}

	public:
		
		inline void NewPage()
		{
            CPageInfo oInfo;
            oInfo.SetUpdater(m_pUpdater);
            m_arrPages.push_back(oInfo);
		}
		inline void SetWidth(const double& dValue)
		{
            size_t nCount = m_arrPages.size();
			if (nCount > 0)
			{
				m_arrPages[nCount - 1].SetWidth(dValue);
			}
		}
		inline void SetHeight(const double& dValue)
		{
            size_t nCount = m_arrPages.size();
			if (nCount > 0)
			{
				m_arrPages[nCount - 1].SetHeight(dValue);
			}
		}

		// --------------------------------------------------------------------

        std::wstring GetThumbnailsHTML()
		{
            std::wstring strHTML = L"<html>";

			strHTML += GetThumbnailsHeader();
			strHTML += GetThumbnailsBody();

            strHTML += L"</html>";

			return strHTML;
		}
        std::wstring GetViewerHTML()
		{
            std::wstring strHTML = L"<html>";

			strHTML += GetViewerHeader();
			strHTML += GetViewerBody();

            strHTML += L"</html>";

			return strHTML;
		}

        std::wstring GetThumbnailBlockHTML(int nPageNum)
		{
            std::wstring strPage = std::to_wstring(nPageNum);
			
            std::wstring strResult = L"<div class=\"blockpage\"><div class=\"blockthumbnail\" align=\"center\"><img align=\"center\" src=\"thumbnails\\page" +
                                    strPage + L".png\" onClick=\"OnChangePage(" + strPage + L")\" width=\"100%\" height=\"90%\"/>" + L"page" + strPage + L"</div></div>";
			return strResult;
		}
        std::wstring GetThumbnailsBody()
		{
            std::wstring strBody = L"<body bgcolor=\"#FEFEFE\">";

            size_t nCount = m_arrPages.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				strBody += GetThumbnailBlockHTML((int)(i + 1));
			}

            strBody += L"</body>";
			return strBody;
		}

        std::wstring GetThumbnailsHeader()
		{
            return L"<head>\
                        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
                        <title>thumbnails</title>\
                        <style type=\"text/css\">\
                            .blockpage {\
                            width: 80%;\
                            height: 200px;\
                            background: #FEFEFE;\
                            padding: 10px;\
                            float: none;\
                            text-align: center;\
                            }\
                            .blockthumbnail {\
                            width: 100%;\
                            height: 100%;\
                            background: #FEFEFE;\
                            padding: 0px;\
                            float: none;\
                            }\
                        </style>\
                        <script language=\"JavaScript\">\
                            function OnChangePage(pageNum)\
                            {\
                                top.frames['viewer'].OnChangePage(pageNum);\
                            }\
                        </script>\
                    </head>";
		}

        std::wstring GetViewerHeader()
		{
            return L"<head>\
                         <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
                         <title>viewer</title>\
                         <style type=\"text/css\">\
                             .blockpage {\
                             position: relative;\
                             left: 20%;\
                             width: 60%;\
                             height: 1200px;\
                             background: #FFFFFF;\
                             padding: 10px;\
                             border-style: outset;\
                             border-color: #F0F0F0;\
                             border-width: 2px 3px 3px 2px;\
                             float: none;\
                             text-align: center;\
                             }\
                             .blockpagebetween {\
                             width: 100%;\
                             height: 20px;\
                             background: #FEFEFE;\
                             padding: 0px;\
                             float: none;\
                             text-align: center;\
                             }\
                             .blockpagenatural {\
                             width: 100%;\
                             height: 100%;\
                             background: #FEFEFE;\
                             padding: 0px;\
                             float: none;\
                             }\
                         </style>\
                         <script language=\"JavaScript\">\
                             function OnChangePage(pageNum)\
                             {\
                                var nPage = Number(pageNum);\
                                var position = ((nPage - 1) * 1225 + (nPage - 1) * 20);\
                                scroll(0, position);\
                             }\
                         </script>\
                     </head>";
		}

        std::wstring GetViewerBlockHTML(int nPageNum)
		{
            std::wstring strPage = std::to_wstring(nPageNum);
			
            std::wstring strResult = L"<div class=\"blockpage\">\n<div class=\"blockpagenatural\" align=\"center\">\n<img align=\"center\" src=\"thumbnails\\page" +
                                    strPage + L".png\" onClick=\"OnChangePage(" + strPage + L")\" width=\"100%\" height=\"100%\"/>\n" + L"</div>\n</div>\n" + L"<div class=\"blockpagebetween\"></div>\n";
			return strResult;
		}
        std::wstring GetViewerBody()
		{
            std::wstring strBody = L"<body bgcolor=\"#FEFEFE\">";

            size_t nCount = m_arrPages.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				strBody += GetViewerBlockHTML((int)(i + 1));
			}

            strBody += L"</body>";

			return strBody;
		}

        std::wstring GetMenuHTML()
		{
            return L"<html>\
                         <head>\
                             <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
                             <title>menu</title>\
                         </head>\
                        <body bgcolor=\"#5F5F5F\">\
                        </body>\
                    </html>";
		}
        std::wstring GetDocumentHTML()
		{
            return L"<html>\
                     <head>\
                         <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
                         <title>document viewer</title>\
                     </head>\
                     <frameset rows=\"50,*\" framespacing=\"0\" frameborder=\"0\">\
                         <frame src=\"menu.html\" name=\"menu\" noresize border=\"1\" bordercolor=\"#F0F0F0\" scrolling=\"no\"></frame>\
                         <frameset cols=\"*,200\">\
                             <frame id=\"id_viewer\" src=\"viewer.html\" name=\"viewer\" noresize></frame>\
                             <frame id=\"id_thumbnails\" src=\"thumbnails.html\" name=\"thumbnail\"></frame>\
                         </frameset>\
                     </frameset>\
                 </html>";
		}

        void CreateDirectories(std::wstring strHTML)
		{
            NSDirectory::CreateDirectory(strHTML);
            NSDirectory::CreateDirectory(strHTML + L"/thumbnails");
		}
		
        void CreateHTMLs(std::wstring strHTML)
		{
            NSFile::CFileBinary::SaveToFile(strHTML + L"/docviewer.html", GetDocumentHTML(), true);
            NSFile::CFileBinary::SaveToFile(strHTML + L"/menu.html", GetMenuHTML(), true);
            NSFile::CFileBinary::SaveToFile(strHTML + L"/viewer.html", GetViewerHTML(), true);
            NSFile::CFileBinary::SaveToFile(strHTML + L"/thumbnails.html", GetThumbnailsHTML(), true);
		}

	};

    class CThumbnails
	{
	private:
        NSGraphics::IGraphicsRenderer*	m_pRenderer;
        CBgraFrame*         m_pFrame;

		LONG	m_lWidth;
		LONG	m_lHeight;

	public:
		CThumbnails()
		{
			m_pRenderer		= NULL;
			m_pFrame		= NULL;
			m_lWidth		= 0;
			m_lHeight		= 0;
		}
		~CThumbnails()
		{
            RELEASEOBJECT(m_pRenderer);
            RELEASEOBJECT(m_pFrame);
		}

	public:
		void Create(const double& dWidth, const double& dHeight, LONG lSizeMax = 200)
		{
			LONG lWidthNew	= 0;
			LONG lHeightNew = 0;

			if (dWidth >= dHeight)
			{
				lWidthNew	= lSizeMax;
				lHeightNew	= (LONG)((dHeight / dWidth) * lWidthNew);
			}
			else
			{
				lHeightNew	= lSizeMax;
				lWidthNew	= (LONG)((dWidth / dHeight) * lHeightNew);
			}

			if ((m_lWidth == lWidthNew) && (m_lHeight == lHeightNew) && (NULL != m_pFrame))
			{
				// размер не поменялся - значит и память перевыделять не нужно
                BYTE* pBuffer = m_pFrame->get_Data();
				memset(pBuffer, 0xFF, 4 * m_lWidth * m_lHeight);

				CreateRenderer();
				return;
			}
			
            RELEASEOBJECT(m_pFrame);
			m_lWidth	= lWidthNew;
			m_lHeight	= lHeightNew;

			CreateMediaData();
			CreateRenderer();

			m_pRenderer->put_Width(dWidth);
			m_pRenderer->put_Height(dHeight);
		}
        inline void Save(std::wstring& strFile)
		{
			SaveFrame(strFile);			
		}

	protected:
		void CreateMediaData()
		{
            RELEASEOBJECT(m_pFrame);

            m_pFrame = new CBgraFrame();
            m_pFrame->put_Width(m_lWidth);
            m_pFrame->put_Height(m_lHeight);
            m_pFrame->put_Stride(4 * m_lWidth);
            BYTE* pBuffer = new BYTE[4 * m_lWidth * m_lHeight];
            memset(pBuffer, 0xFF, 4 * m_lWidth * m_lHeight);
            m_pFrame->put_Data(pBuffer);
		}
		void CreateRenderer()
		{
			// теперь на всякий случай (сбросить все состояния) - пересоздадим рендерер
			RELEASEINTERFACE(m_pRenderer);

            m_pRenderer = NSGraphics::Create();
            m_pRenderer->CreateFromBgraFrame(m_pFrame);
		}

        void SaveFrame(const std::wstring& strFile)
		{
            m_pFrame->SaveFile(strFile, 4);
		}

	public:
		inline HRESULT get_Type(LONG* lType)
		{
			return m_pRenderer->get_Type(lType);
		}

		//-------- Функции для работы со страницей --------------------------------------------------
		inline HRESULT NewPage()
		{
			if (NULL == m_pRenderer)
				return S_OK;

			return m_pRenderer->NewPage();
		}
		inline HRESULT get_Height(double* dHeight)
		{
			return m_pRenderer->get_Height(dHeight);
		}
        inline HRESULT put_Height(const double& dHeight)
		{
			if (NULL == m_pRenderer)
				return S_OK;

			return m_pRenderer->put_Height(dHeight);
		}
		inline HRESULT get_Width(double* dWidth)
		{
			return m_pRenderer->get_Width(dWidth);
		}
        inline HRESULT put_Width(const double& dWidth)
		{
			if (NULL == m_pRenderer)
				return S_OK;

			return m_pRenderer->put_Width(dWidth);
		}
		inline HRESULT get_DpiX(double* dDpiX)
		{
			return m_pRenderer->get_DpiX(dDpiX);
		}
		inline HRESULT get_DpiY(double* dDpiY)
		{
			return m_pRenderer->get_DpiY(dDpiY);
		}

	// pen --------------------------------------------------------------------------------------
		inline HRESULT get_PenColor(LONG* lColor)
		{
			return m_pRenderer->get_PenColor(lColor);
		}
        inline HRESULT put_PenColor(const LONG& lColor)
		{
			return m_pRenderer->put_PenColor(lColor);
		}
		inline HRESULT get_PenAlpha(LONG* lAlpha)
		{
			return m_pRenderer->get_PenAlpha(lAlpha);
		}
        inline HRESULT put_PenAlpha(const LONG& lAlpha)
		{
			return m_pRenderer->put_PenAlpha(lAlpha);
		}
		inline HRESULT get_PenSize(double* dSize)
		{
			return m_pRenderer->get_PenSize(dSize);
		}
        inline HRESULT put_PenSize(const double& dSize)
		{
			return m_pRenderer->put_PenSize(dSize);
		}
		inline HRESULT get_PenDashStyle(BYTE* val)
		{
			return m_pRenderer->get_PenDashStyle(val);
		}
        inline HRESULT put_PenDashStyle(const BYTE& val)
		{
			return m_pRenderer->put_PenDashStyle(val);
		}
		inline HRESULT get_PenLineStartCap(BYTE* val)
		{
			return m_pRenderer->get_PenLineStartCap(val);
		}
        inline HRESULT put_PenLineStartCap(const BYTE& val)
		{
			return m_pRenderer->put_PenLineStartCap(val);
		}
		inline HRESULT get_PenLineEndCap(BYTE* val)
		{
			return m_pRenderer->get_PenLineEndCap(val);
		}
        inline HRESULT put_PenLineEndCap(const BYTE& val)
		{
			return m_pRenderer->put_PenLineEndCap(val);
		}
		inline HRESULT get_PenLineJoin(BYTE* val)
		{
			return m_pRenderer->get_PenLineJoin(val);
		}
        inline HRESULT put_PenLineJoin(const BYTE& val)
		{
			return m_pRenderer->put_PenLineJoin(val);
		}
		inline HRESULT get_PenDashOffset(double* val)
		{
			return m_pRenderer->get_PenDashOffset(val);
		}
        inline HRESULT put_PenDashOffset(const double& val)
		{
			return m_pRenderer->put_PenDashOffset(val);
		}
		inline HRESULT get_PenAlign(LONG* val)
		{
			return m_pRenderer->get_PenAlign(val);
		}
        inline HRESULT put_PenAlign(const LONG& val)
		{
			return m_pRenderer->put_PenAlign(val);
		}
		inline HRESULT get_PenMiterLimit(double* val)
		{
			return m_pRenderer->get_PenMiterLimit(val);
		}
        inline HRESULT put_PenMiterLimit(const double& val)
		{
			return m_pRenderer->put_PenMiterLimit(val);
		}
        inline HRESULT PenDashPattern(double* pPattern, LONG lCount)
		{
            return m_pRenderer->PenDashPattern(pPattern, lCount);
		}

	// brush ------------------------------------------------------------------------------------
		inline HRESULT get_BrushType(LONG* lType)
		{
			return m_pRenderer->get_BrushType(lType);
		}
        inline HRESULT put_BrushType(const LONG& lType)
		{
			return m_pRenderer->put_BrushType(lType);
		}
		inline HRESULT get_BrushColor1(LONG* lColor)
		{
			return m_pRenderer->get_BrushColor1(lColor);
		}
        inline HRESULT put_BrushColor1(const LONG& lColor)
		{
			return m_pRenderer->put_BrushColor1(lColor);
		}
		inline HRESULT get_BrushAlpha1(LONG* lAlpha)
		{
			return m_pRenderer->get_BrushAlpha1(lAlpha);
		}
        inline HRESULT put_BrushAlpha1(const LONG& lAlpha)
		{
			return m_pRenderer->put_BrushAlpha1(lAlpha);
		}
		inline HRESULT get_BrushColor2(LONG* lColor)
		{
			return m_pRenderer->get_BrushColor2(lColor);
		}
        inline HRESULT put_BrushColor2(const LONG& lColor)
		{
			return m_pRenderer->put_BrushColor2(lColor);
		}
		inline HRESULT get_BrushAlpha2(LONG* lAlpha)
		{
			return m_pRenderer->get_BrushAlpha2(lAlpha);
		}
        inline HRESULT put_BrushAlpha2(const LONG& lAlpha)
		{
			return m_pRenderer->put_BrushAlpha2(lAlpha);
		}
        inline HRESULT get_BrushTexturePath(std::wstring* sPath)
		{
            return m_pRenderer->get_BrushTexturePath(sPath);
		}
        inline HRESULT put_BrushTexturePath(const std::wstring& bsPath)
		{
			return m_pRenderer->put_BrushTexturePath(bsPath);
		}
		inline HRESULT get_BrushTextureMode(LONG* lMode)
		{
			return m_pRenderer->get_BrushTextureMode(lMode);
		}
        inline HRESULT put_BrushTextureMode(const LONG& lMode)
		{
			return m_pRenderer->put_BrushTextureMode(lMode);
		}
		inline HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)
		{
			return m_pRenderer->get_BrushTextureAlpha(lTxAlpha);
		}
        inline HRESULT put_BrushTextureAlpha(const LONG lTxAlpha)
		{
			return m_pRenderer->put_BrushTextureAlpha(lTxAlpha);
		}
		inline HRESULT get_BrushLinearAngle(double* dAngle)
		{
			return m_pRenderer->get_BrushLinearAngle(dAngle);
		}
        inline HRESULT put_BrushLinearAngle(const double& dAngle)
		{
			return m_pRenderer->put_BrushLinearAngle(dAngle);
		}
        inline HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
        {
            return m_pRenderer->BrushRect(val, left, top, width, height);
        }

        inline HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height)
        {
            return m_pRenderer->BrushBounds(left, top, width, height);
        }

        inline HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
        {
            return m_pRenderer->put_BrushGradientColors(lColors, pPositions, nCount);
        }

	// font -------------------------------------------------------------------------------------
        inline HRESULT get_FontName(std::wstring* bsName)
		{
			return m_pRenderer->get_FontName(bsName);
		}
        inline HRESULT put_FontName(const std::wstring& bsName)
		{
			return m_pRenderer->put_FontName(bsName);
		}
        inline HRESULT get_FontPath(std::wstring* bsName)
		{
			return m_pRenderer->get_FontPath(bsName);
		}
        inline HRESULT put_FontPath(const std::wstring& bsName)
		{
			return m_pRenderer->put_FontPath(bsName);
		}
		inline HRESULT get_FontSize(double* dSize)
		{
			return m_pRenderer->get_FontSize(dSize);
		}
        inline HRESULT put_FontSize(const double& dSize)
		{
			return m_pRenderer->put_FontSize(dSize);
		}
		inline HRESULT get_FontStyle(LONG* lStyle)
		{
			return m_pRenderer->get_FontStyle(lStyle);
		}
        inline HRESULT put_FontStyle(const LONG& lStyle)
		{
			return m_pRenderer->put_FontStyle(lStyle);
		}
        inline HRESULT get_FontStringGID(INT* bGID)
		{
            return m_pRenderer->get_FontStringGID(bGID);
		}
        inline HRESULT put_FontStringGID(const INT& bGID)
		{
			return m_pRenderer->put_FontStringGID(bGID);
		}
		inline HRESULT get_FontCharSpace(double* dSpace)
		{
			return m_pRenderer->get_FontCharSpace(dSpace);
		}
        inline HRESULT put_FontCharSpace(const double& dSpace)
		{
			return m_pRenderer->put_FontCharSpace(dSpace);
		}
        inline HRESULT get_FontFaceIndex(int* lFaceIndex)
        {
            return m_pRenderer->get_FontFaceIndex(lFaceIndex);
        }
        inline HRESULT put_FontFaceIndex(const int& lFaceIndex)
        {
            return m_pRenderer->put_FontFaceIndex(lFaceIndex);
        }

	//-------- Функции для вывода текста --------------------------------------------------------
        virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->CommandDrawTextCHAR(c, x, y, w, h);
        }
        virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->CommandDrawText(bsText, x, y, w, h);
        }
        virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->CommandDrawTextExCHAR(c, gid, x, y, w, h);
        }
        virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->CommandDrawTextEx(bsUnicodeText, pGids, nGidsCount, x, y, w, h);
        }


	//-------- Маркеры для команд ---------------------------------------------------------------
        inline HRESULT BeginCommand(const DWORD& lType)
		{
			return m_pRenderer->BeginCommand(lType);
		}
        inline HRESULT EndCommand(const DWORD& lType)
		{
			return m_pRenderer->EndCommand(lType);
		}

	//-------- Функции для работы с Graphics Path -----------------------------------------------
        inline HRESULT PathCommandMoveTo(const double& fX, const double& fY)
		{
			return m_pRenderer->PathCommandMoveTo(fX, fY);
		}
        inline HRESULT PathCommandLineTo(const double& fX, const double& fY)
		{
			return m_pRenderer->PathCommandLineTo(fX, fY);
		}
        inline HRESULT PathCommandLinesTo(double* points, const int& count)
		{
            return m_pRenderer->PathCommandLinesTo(points, count);
		}
        inline HRESULT PathCommandCurveTo(const double& fX1, const double& fY1, const double& fX2, const double& fY2, const double& fX3, const double& fY3)
		{
			return m_pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		}
        inline HRESULT PathCommandCurvesTo(double* points, const int& count)
		{
            return m_pRenderer->PathCommandCurvesTo(points, count);
		}
        inline HRESULT PathCommandArcTo(const double& fX, const double& fY, const double& fWidth, const double& fHeight, const double& fStartAngle, const double& fSweepAngle)
		{
			return m_pRenderer->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		}
		inline HRESULT PathCommandClose()
		{
			return m_pRenderer->PathCommandClose();
		}
		inline HRESULT PathCommandEnd()
		{
			return m_pRenderer->PathCommandEnd();
		}
        inline HRESULT DrawPath(const LONG& nType)
		{
			return m_pRenderer->DrawPath(nType);
		}
		inline HRESULT PathCommandStart()
		{
			return m_pRenderer->PathCommandStart();
		}
		inline HRESULT PathCommandGetCurrentPoint(double* fX, double* fY)
		{
			return m_pRenderer->PathCommandGetCurrentPoint(fX, fY);
		}

        inline HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->PathCommandTextCHAR(c, x, y, w, h);
        }
        inline HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->PathCommandText(bsText, x, y, w, h);
        }
        inline HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->PathCommandTextExCHAR(c, gid, x, y, w, h);
        }
        inline HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
        {
            return m_pRenderer->PathCommandTextEx(bsUnicodeText, pGids, nGidsCount, x, y, w, h);
        }

	//-------- Функции для вывода изображений ---------------------------------------------------
        inline HRESULT DrawImage(IGrObject* pInterface, const double& fX, const double& fY, const double& fWidth, const double& fHeight)
		{
			return m_pRenderer->DrawImage(pInterface, fX, fY, fWidth, fHeight);
		}
        inline HRESULT DrawImageFromFile(const std::wstring& sPath, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)
		{
            return m_pRenderer->DrawImageFromFile(sPath, x, y, w, h, lAlpha);
		}

	// transform --------------------------------------------------------------------------------
        inline HRESULT SetTransform(const double& dA, const double& dB, const double& dC, const double& dD, const double& dE, const double& dF)
		{
			return m_pRenderer->SetTransform(dA, dB, dC, dD, dE, dF);
		}
		inline HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
		{
			return m_pRenderer->GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
		}
		inline HRESULT ResetTransform(void)
		{
			return m_pRenderer->ResetTransform();
		}

	// -----------------------------------------------------------------------------------------
		inline HRESULT get_ClipMode(LONG* plMode)
		{
			return m_pRenderer->get_ClipMode(plMode);
		}
        inline HRESULT put_ClipMode(const LONG& lMode)
		{
			return m_pRenderer->put_ClipMode(lMode);
		}

	// additiaonal params ----------------------------------------------------------------------
        inline HRESULT CommandLong(const LONG& lType, const LONG& lCommand)
        {
            return m_pRenderer->CommandLong(lType, lCommand);
        }
        inline HRESULT CommandDouble(const LONG& lType, const double& dCommand)
        {
            return m_pRenderer->CommandDouble(lType, dCommand);
        }
        inline HRESULT CommandString(const LONG& lType, const std::wstring& sCommand)
        {
            return m_pRenderer->CommandString(lType, sCommand);
        }

	};
}

#endif // _ASC_HTMLRENDERER_DOCUMENT_H_

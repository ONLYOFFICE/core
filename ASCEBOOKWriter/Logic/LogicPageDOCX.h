#pragma once
#include "LogicItems.h"

namespace NSEBook
{
	class CBaseWriter
	{
	public:
		virtual void WriteShape(IUnknown* punkImage, double& x, double& y, double& width, double& height)	= 0;
	};

		
	const double c_dSpaceW = 0.5;
	class CTextSmart
	{
	public:
		CFontManager*		m_pFontManager;
		CFontDstGenerator*	m_pFontsDst;

		double			m_dDpiX;
		double			m_dDpiY;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		NSCommon::CMatrix*		m_pTransform;

		CEbookParagraph	m_oParagraph;
		CTextItem		m_oTextItem;
		
	private:
		double			m_dEpsX;
		double			m_dEpsY;

	public:
		CTextSmart() : m_pFontManager(NULL), m_oTextItem(10)
 		{
			m_dEpsX		= 0.5;
			m_dEpsY		= 0.5;

			m_pPen		= NULL;
			m_pBrush	= NULL;
			m_pFont		= NULL;

			m_pTransform	= NULL;

			m_pFontsDst = NULL;
		}

		void SetParams(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, NSStructures::CFont* pFont, NSCommon::CMatrix* pTransform)
		{
			m_pPen					= pPen;
			m_pBrush				= pBrush;
			m_pFont					= pFont;

			m_pFontManager->m_pFont	= pFont;
			m_pFontManager->m_pTransform = pTransform;
			m_pTransform			= pTransform;
		}

		~CTextSmart()
		{
		}

		void NewPage(double& dDpiX, double& dDpiY)
		{
			m_dDpiX	= dDpiX;
			m_dDpiY = dDpiY;

			m_pPen->SetDefaultParams();
			m_pBrush->SetDefaultParams();
			m_pFont->SetDefaultParams();

			m_oParagraph.Clear();
		}

		void NewDocument()
		{
			m_pPen->SetDefaultParams();
			m_pBrush->SetDefaultParams();
			m_pFont->SetDefaultParams();
		}

		CEbookParagraph::CLine& GetCurrentLine()
		{
			if (0 == m_oParagraph.m_arLines.GetCount())
				m_oParagraph.m_arLines.AddTail();
			
			return m_oParagraph.m_arLines.GetTail();
		}
	
	protected:

		void CommandText(double& x, double& y, double& width, double& height, double& baselineoffset, double& dCurrentBaselineOffset)
		{
			double dScaleFont = ((m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2);
			BOOL bIsFontChanged = !m_pFontManager->m_oFont.m_oFont.IsEqual2(m_pFont, dScaleFont);

			BOOL bIsTextDecorationChanged = ((m_pFontManager->m_oFont.m_oFont.Underline != m_pFont->Underline) ||
												(m_pFontManager->m_oFont.m_oFont.Strikeout != m_pFont->Strikeout));

			if (bIsFontChanged)
			{
				m_pFontManager->m_oFont.m_oFont = *m_pFont;
				m_pFontManager->m_oFont.m_oFont.Size *= dScaleFont;
			}
			else if (bIsTextDecorationChanged)
			{
				m_pFontManager->m_oFont.m_oFont.Underline	= m_pFont->Underline;
				m_pFontManager->m_oFont.m_oFont.Strikeout	= m_pFont->Strikeout;
			}

			// никакого подбора здесь нет. нужное имя должно быть выставлено у m_pFont			
			m_pFontManager->m_strCurrentPickFont		= _T("");

			if (0 == width)
			{
				// откуда такое может быть?? формулы??
				m_pFontManager->LoadCurrentFont();

				double dOffset = m_pFontManager->m_oFont.m_dBaselineOffsetHTML;
				dOffset *= (m_dDpiY / 72.0);

				y -= dOffset;
				baselineoffset = dOffset;

				// текст не измерен. для начала нужно его измерить
				double _x = 0;
				double _y = 0;
				double _w = 0;
				double _h = 0;

				LONG lGid = m_pFontManager->GetStringGid();
				m_pFontManager->SetStringGid(0);
				m_pFontManager->MeasureStringPix(m_oTextItem.GetCString(), x, y, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				m_pFontManager->SetStringGid(lGid);

				width  = _w;
				height = _h;
			}

			CEbookParagraph::CLine& oCurrentLine = GetCurrentLine();
			
			double dYPos		= y + baselineoffset;
			LONG lCountWords	= (LONG)oCurrentLine.m_listSpans.GetCount();

			m_pFontsDst->AddFont(m_pFont, bIsFontChanged, m_pFontManager);
			if (0 == lCountWords)
			{
				// слов в линии нет, без всяких проверок создаем линию
				oCurrentLine.m_listSpans.AddTail();
				CEbookParagraph::CSpan& oSpan = oCurrentLine.m_listSpans.GetTail();

				//oSpan.m_oFont		= m_oFontManager.m_oFont.m_oFont;
				//oSpan.m_oFont.Name		= m_oFontManager.m_strCurrentPickFont;
				//oSpan.m_oFont.SetStyle(m_oFontManager.m_lCurrentPictFontStyle);
				oSpan.m_oFont	= *m_pFont;
				
				oSpan.m_oBrush		= *m_pBrush;
				oSpan.m_oText		= m_oTextItem;
				oSpan.m_dX			= x;
				oSpan.m_dWidthSpan	= width;
				oSpan.m_dBaselineOffset = dCurrentBaselineOffset;
				return;
			}

			CEbookParagraph::CSpan& oTail = oCurrentLine.m_listSpans.GetTail();
			double dRight = oTail.m_dX + oTail.m_dWidthSpan;
			if (oTail.m_oImageInfo.m_lID != -1)
			{
				// расстояние слишком большое.
				oCurrentLine.m_listSpans.AddTail();
				CEbookParagraph::CSpan& oSpan = oCurrentLine.m_listSpans.GetTail();

				//oSpan.m_oFont		= m_oFontManager.m_oFont.m_oFont;
				//oSpan.m_oFont.Name		= m_oFontManager.m_strCurrentPickFont;
				//oSpan.m_oFont.SetStyle(m_oFontManager.m_lCurrentPictFontStyle);
				oSpan.m_oFont	= *m_pFont;

				oSpan.m_oBrush		= *m_pBrush;
				oSpan.m_oText		= m_oTextItem;
				oSpan.m_dX			= x;
				oSpan.m_dWidthSpan	= width;
				oSpan.m_dBaselineOffset = dCurrentBaselineOffset;

				if (c_dSpaceW < abs(oTail.m_dX + oTail.m_dWidthSpan - x))
				{
					// это не одно и то же слово!!!
					oSpan.m_oText.AddSpaceFirst();
				}

				return;
			}

			// текст уже измерен
			if (!bIsFontChanged && oTail.m_oBrush.IsEqual(m_pBrush) && !bIsTextDecorationChanged && (oTail.m_dBaselineOffset == dCurrentBaselineOffset))
			{
				if (c_dSpaceW < abs(oTail.m_dX + oTail.m_dWidthSpan - x))
				{
					// это не одно и то же слово!!!
					oTail.m_oText.AddSpace();
				}

				oTail.m_oText		+= m_oTextItem;
				oTail.m_dWidthSpan = (x + width - oTail.m_dX);
			}
			else
			{
				if (c_dSpaceW < abs(oTail.m_dX + oTail.m_dWidthSpan - x))
				{
					// это не одно и то же слово!!!
					oTail.m_oText.AddSpace();
				}

				oCurrentLine.m_listSpans.AddTail();
				CEbookParagraph::CSpan& oSpan = oCurrentLine.m_listSpans.GetTail();

				//oSpan.m_oFont		= m_oFontManager.m_oFont.m_oFont;
				//oSpan.m_oFont.Name		= m_oFontManager.m_strCurrentPickFont;
				//oSpan.m_oFont.SetStyle(m_oFontManager.m_lCurrentPictFontStyle);
				oSpan.m_oFont	= *m_pFont;

				oSpan.m_oBrush		= *m_pBrush;
				oSpan.m_oText		= m_oTextItem;
				oSpan.m_dX			= x;
				oSpan.m_dWidthSpan	= width;
				oSpan.m_dBaselineOffset = dCurrentBaselineOffset;
			}
		}
		
	public:
		AVSINLINE void CommandText(BSTR& bsText, BSTR& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset, double& dCurrentBaselineOffset)
		{
			if (NULL == bsText)
				return;

			m_oTextItem.SetText(bsText);

			if (0 == m_oTextItem.GetCurSize())
				return;

			m_oParagraph.m_rcBounds.top = min(m_oParagraph.m_rcBounds.top, y);
			m_oParagraph.m_rcBounds.bottom = max(m_oParagraph.m_rcBounds.bottom, y);

			return CommandText(x, y, width, height, baselineoffset, dCurrentBaselineOffset);
		}
	};

	class CLogicPageDOCX : public ILogicPage, public CBaseWriter
	{
	public:
		class CLogicShape
		{
		public:
			CDoubleRect								m_rcBounds;
			MediaCore::IAVSUncompressedVideoFrame*	m_pFrame;
			ASCGraphics::IASCGraphicsRenderer*		m_pGraphicsRenderer;			

		public:
			CLogicShape()
			{
				m_rcBounds.ClearBounds();

				m_pFrame			= NULL;
				m_pGraphicsRenderer	= NULL;
			}
			~CLogicShape()
			{
				RELEASEINTERFACE(m_pFrame);
				RELEASEINTERFACE(m_pGraphicsRenderer);
			}
			void Clear()
			{
				m_rcBounds.ClearBounds();
				RELEASEINTERFACE(m_pGraphicsRenderer);
			}
			void CreateShape(double& dWidth, double& dHeight)
			{
				Clear();

				LONG lWidthCur	= (LONG)(96.0 * dWidth / 25.4);
				LONG lHeightCur	= (LONG)(96.0 * dHeight / 25.4);

				bool bIsNeedCreate = true;
				if (NULL != m_pFrame)
				{
					LONG lWidth		= 0;
					LONG lHeight	= 0;
					m_pFrame->get_Width(&lWidth);
					m_pFrame->get_Height(&lHeight);

					if ((lWidthCur == lWidth) && (lHeightCur == lHeight))
						bIsNeedCreate = false;
				}

				if (bIsNeedCreate)
				{
					RELEASEINTERFACE(m_pFrame);
							
					CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
						MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrame);

					m_pFrame->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
					m_pFrame->put_Width( lWidthCur );
					m_pFrame->put_Height( lHeightCur );
					m_pFrame->put_AspectRatioX( lWidthCur );
					m_pFrame->put_AspectRatioY( lHeightCur );
					m_pFrame->put_Interlaced( VARIANT_FALSE );
					m_pFrame->put_Stride( 0, 4 * lWidthCur );
					m_pFrame->AllocateBuffer( -1 );
				}

				BYTE* pBuffer = NULL;
				m_pFrame->get_Buffer(&pBuffer);
				memset(pBuffer, 0xFF, 4 * lWidthCur * lHeightCur);

				CoCreateInstance(ASCGraphics::CLSID_CASCGraphicsRenderer, NULL, CLSCTX_ALL, 
					ASCGraphics::IID_IASCGraphicsRenderer, (void**)&m_pGraphicsRenderer);

				//ставим FontManager
				VARIANT vtVariant;
				vtVariant.vt = VT_UNKNOWN;
				vtVariant.punkVal = NULL;
				m_pGraphicsRenderer->SetAdditionalParam( L"FontManager", vtVariant );

				m_pGraphicsRenderer->put_Width(dWidth);
				m_pGraphicsRenderer->put_Height(dHeight);
				m_pGraphicsRenderer->CreateFromMediaData((IUnknown*)m_pFrame, 0, 0, lWidthCur, lHeightCur );
			}
			void GetShape(CBaseWriter* pBaseWriter)
			{
				LONG lWidthCur = 0;
				LONG lHeightCur = 0;
				m_pFrame->get_Width(&lWidthCur);
				m_pFrame->get_Height(&lHeightCur);

				LONG lLeft		= max(0, min(lWidthCur - 1, (LONG)(m_rcBounds.left * 96.0 / 25.4)));
				LONG lTop		= max(0, min(lHeightCur - 1, (LONG)(m_rcBounds.top * 96.0 / 25.4)));
				LONG lRight		= max(0, min(lWidthCur - 1, (LONG)(m_rcBounds.right * 96.0 / 25.4)));
				LONG lBottom	= max(0, min(lHeightCur - 1, (LONG)(m_rcBounds.bottom * 96.0 / 25.4)));

				if (lRight < lLeft || lBottom < lTop)
					return;

				MediaCore::IAVSUncompressedVideoFrame* pShapePicture = NULL;
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
										MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pShapePicture);

				LONG lWidthShape  = lRight - lLeft + 1;
				LONG lHeightShape = lBottom - lTop + 1;

				pShapePicture->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				pShapePicture->put_Width( lWidthShape );
				pShapePicture->put_Height( lHeightShape );
				pShapePicture->put_AspectRatioX( lWidthShape );
				pShapePicture->put_AspectRatioY( lHeightShape );
				pShapePicture->put_Interlaced( VARIANT_FALSE );
				pShapePicture->put_Stride( 0, 4 * lWidthShape );
				pShapePicture->AllocateBuffer( -1 );

				BYTE* pBufferDst = NULL;
				pShapePicture->get_Buffer(&pBufferDst);

				BYTE* pBuffer = NULL;
				m_pFrame->get_Buffer(&pBuffer);

				BYTE* pDstLine = pBufferDst;
				BYTE* pSrcLine = pBuffer + 4 * (lHeightCur - 1 - lBottom) * lWidthCur + lLeft * 4;

				for (int ind = 0; ind < lHeightShape; ++ind)
				{
					memcpy(pDstLine, pSrcLine, 4 * lWidthShape);
					pDstLine += (4 * lWidthShape);
					pSrcLine += (4 * lWidthCur);
				}

				double __x = m_rcBounds.left;
				double __y = m_rcBounds.top;
				double __w = m_rcBounds.right - __x;
				double __h = m_rcBounds.bottom - __y;
				
				pBaseWriter->WriteShape((IUnknown*)pShapePicture, __x, __y, __w, __h);

				RELEASEINTERFACE(pShapePicture);
			}


			AVSINLINE void MoveTo(double& x, double& y)
			{
				if (NULL != m_pGraphicsRenderer)
					m_pGraphicsRenderer->PathCommandMoveTo(x, y);
				m_rcBounds.CheckBounds(x, y);
			}
			AVSINLINE void LineTo(double& x, double& y)
			{
				if (NULL != m_pGraphicsRenderer)
					m_pGraphicsRenderer->PathCommandLineTo(x, y);
				m_rcBounds.CheckBounds(x, y);
			}
			AVSINLINE void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
			{
				if (NULL != m_pGraphicsRenderer)
					m_pGraphicsRenderer->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
				m_rcBounds.CheckBounds(x1, y1);
				m_rcBounds.CheckBounds(x2, y2);
				m_rcBounds.CheckBounds(x3, y3);
			}
			AVSINLINE void Close()
			{
				if (NULL != m_pGraphicsRenderer)
					m_pGraphicsRenderer->PathCommandClose();
			}
			AVSINLINE void BeginPath()
			{
				if (NULL != m_pGraphicsRenderer)
					m_pGraphicsRenderer->PathCommandStart();
			}
			AVSINLINE void EndPath()
			{
				if (NULL != m_pGraphicsRenderer)
					m_pGraphicsRenderer->PathCommandEnd();			
			}

			AVSINLINE void WriteText(BSTR& bsText, BSTR& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset, 
				NSStructures::CFont* pFont, NSStructures::CBrush* pBrush)
			{
				if (NULL != m_pGraphicsRenderer)
				{
					pFont->SetToRenderer((IASCRenderer*)m_pGraphicsRenderer);
					pBrush->SetToRenderer((IASCRenderer*)m_pGraphicsRenderer);
					
					if (NULL == bsGid)
						m_pGraphicsRenderer->CommandDrawText(bsText, x, y, width, height, baselineoffset);
					else
						m_pGraphicsRenderer->CommandDrawTextEx(bsText, bsGid, NULL, x, y, width, height, baselineoffset, 0);
				}
				
				m_rcBounds.CheckBounds(x, y);
				m_rcBounds.CheckBounds(x + width, y + height);
			}
			AVSINLINE void DrawPath(LONG lPath, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush)
			{
				if (NULL != m_pGraphicsRenderer)
				{
					pPen->SetToRenderer((IASCRenderer*)m_pGraphicsRenderer);
					pBrush->SetToRenderer((IASCRenderer*)m_pGraphicsRenderer);
					m_pGraphicsRenderer->DrawPath(lPath);
				}
			}
		};

	public:
		CLogicShape				m_oCurrentShape;
		LONG					m_lShapeTop;

		CTextSmart				m_oText;
		bool					m_bIsPar;
		double					m_dCurrentBaselineOffset;

		double					m_dLastTop;

	public:
		CLogicPageDOCX() : ILogicPage()
		{
			m_lShapeTop = 0;
			m_bIsPar = false;
		}
		~CLogicPageDOCX()
		{
		}
	public:
		virtual void ConvertToEbookPage(NSEBook::CFormat& oFormat)
		{
			// сначала отсортируем по методу GetPositionY()

			CAtlList<ILogicItem*> arraySort;
			
			while (0 != m_arItems.GetCount())
			{
				POSITION pos = m_arItems.GetHeadPosition();
				POSITION posNeed = pos;

				ILogicItem* pItem = m_arItems.GetNext(pos);
				double dMin = pItem->GetPositionY();
				while (NULL != pos)
				{
					POSITION posOld = pos;
					pItem = m_arItems.GetNext(pos);

					double dMem = pItem->GetPositionY();
					if (dMem < dMin)
					{
						dMin = dMem;
						posNeed = posOld;
					}
				}

				ILogicItem* pNeed = m_arItems.GetAt(posNeed);
				m_arItems.RemoveAt(posNeed);
				arraySort.AddTail(pNeed);
			}
			m_arItems.RemoveAll();
			m_arItems.AddTailList(&arraySort);
			arraySort.RemoveAll();

			CEbookParagraph* pLogicParagraph = NULL;

			if (0 != oFormat.m_listSections.GetCount())
			{
				CSection& oSectionLast = oFormat.m_listSections.GetTail();
				IEbookItem* pItem = oSectionLast.m_arItems.GetTail();

				if (NULL != pItem)
				{
					if (IEbookItem::ebitParagraph == pItem->m_eType)
					{
						pLogicParagraph = (CEbookParagraph*)pItem;

						if (pLogicParagraph->m_bIsComplete)
							pLogicParagraph = NULL;
					}
				}
			}

			POSITION pos = m_arItems.GetHeadPosition();
			while (NULL != pos)
			{
				CSection* pSection = NULL;
				bool bIsFirstSection = true;
				if (0 == oFormat.m_listSections.GetCount())
				{
					oFormat.m_listSections.AddTail();
					pSection = &oFormat.m_listSections.GetTail();
				}
				else
				{
					pSection = &oFormat.m_listSections.GetTail();
					bIsFirstSection = false;
				}

				ILogicItem* pItem = m_arItems.GetNext(pos);

				if (ILogicItem::elitTextParagraph == pItem->m_eType)
				{
					CLogicParagraph* paragraph = dynamic_cast<CLogicParagraph*>(pItem);
					
					if (paragraph->m_oParagraph.m_lStyle == 1 && !bIsFirstSection)
					{
						// это два параграфа подряд title. если так будет выглядеть не очень - то будем менять
						if (1 < pSection->m_arItems.GetCount())
						{
							oFormat.m_listSections.AddTail();
							pSection = &oFormat.m_listSections.GetTail();
						}
					}
				}

				switch (pItem->m_eType)
				{
				case ILogicItem::elitTextParagraph:
					{
						CLogicParagraph* pCurrentPar = (CLogicParagraph*)pItem;
						if (NULL != pLogicParagraph)
						{
							// не законченный параграф!!!
							pLogicParagraph->m_arLines.AddTailList(&pCurrentPar->m_oParagraph.m_arLines);
							pLogicParagraph->m_bIsComplete = pCurrentPar->m_oParagraph.m_bIsComplete;

							pLogicParagraph->m_bIsPageBreakAfter = pItem->m_bIsPageBreakAfter;

							pLogicParagraph = NULL;
						}
						else
						{
							CEbookParagraph* pParagraph = new CEbookParagraph(pCurrentPar->m_oParagraph);
							pParagraph->m_bIsPageBreakAfter = pItem->m_bIsPageBreakAfter;
							pSection->m_arItems.AddTail(pParagraph);

							CheckWatermark(oFormat);
						}
						break;
					}
				case ILogicItem::elitTable:
					{
						CEbookTable* pTable = new CEbookTable();
						pTable->m_strTableXml = ((CLogicTable*)pItem)->m_strTableXml;
						pSection->m_arItems.AddTail(pTable);

						pTable->m_bIsPageBreakAfter = pItem->m_bIsPageBreakAfter;

						CheckWatermark(oFormat);
						break;
					}
				case ILogicItem::elitGraphic:
					{
						CLogicImage* pLogicImage = ((CLogicImage*)pItem);
						
						CEbookGraphicObject* pImage = new CEbookGraphicObject();						
						pImage->m_oInfo = pLogicImage->m_oImageInfo;
						pImage->m_rcBounds = pItem->m_rcBounds;
						pSection->m_arItems.AddTail(pImage);

						pImage->m_bIsPageBreakAfter = pItem->m_bIsPageBreakAfter;

						CheckWatermark(oFormat);
						break;
					}
				default:
					break;
				}
			}
		}

		virtual void MoveTo(double& x, double& y)
		{
			m_oCurrentShape.MoveTo(x, y);
		}
		virtual void LineTo(double& x, double& y)
		{
			m_oCurrentShape.LineTo(x, y);
		}
		virtual void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_oCurrentShape.CurveTo(x1, y1, x2, y2, x3, y3);
		}
		virtual void Close()
		{
			m_oCurrentShape.Close();
		}
		virtual void BeginPath()
		{
			m_oCurrentShape.BeginPath();
		}
		virtual void EndPath()
		{
			m_oCurrentShape.EndPath();
		}

		AVSINLINE void BeginParagraph()
		{
			if (m_bIsPar)
				return;

			m_bIsPar = true;
			m_oText.m_oParagraph.m_rcBounds.top		= 100000;
			m_oText.m_oParagraph.m_rcBounds.bottom	= -100000;
		}
		AVSINLINE void EndParagraph()
		{
			if (100000 == m_oText.m_oParagraph.m_rcBounds.top)
			{
				if (100000 != m_dLastTop)
				{
					++m_dLastTop;
					m_oText.m_oParagraph.m_rcBounds.top = m_dLastTop;
				}
			}
			
			if (100000 != m_oText.m_oParagraph.m_rcBounds.top)
			{
				CLogicParagraph* pPar = new CLogicParagraph(m_oText.m_oParagraph);
				pPar->m_rcBounds = m_oText.m_oParagraph.m_rcBounds;
				m_arItems.AddTail(pPar);
			}
			else
			{
				m_dLastTop = m_oText.m_oParagraph.m_rcBounds.top;
			}

			m_oText.m_oParagraph.m_bIsComplete = true;
			m_oText.m_oParagraph.Clear();
			m_bIsPar = false;
		}
		AVSINLINE void BeginLine()
		{
			m_dCurrentBaselineOffset = 0;
			m_oText.m_oParagraph.m_arLines.AddTail();
		}
		AVSINLINE void EndLine()
		{
		}
		AVSINLINE void SetBaselineOffset(double dOffset)
		{
			m_dCurrentBaselineOffset = dOffset;
		}
		AVSINLINE void SetParagraphStyle(LONG lStyle)
		{
			m_oText.m_oParagraph.m_lStyle = lStyle;
		}
		AVSINLINE void SetParTextAlign(LONG lAlign)
		{
			m_oText.m_oParagraph.m_lTextAlign = lAlign;
		}
		AVSINLINE void SetParagraphNumLevel(LONG lLevel)
		{
			m_oText.m_oParagraph.m_lNumberingLevel = lLevel;
		}

		virtual void WriteText(BSTR& bsText, BSTR& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset)
		{
			if (0 != m_lShapeTop)
			{
				// значит идет шейп
				m_oCurrentShape.WriteText(bsText, bsGid, x, y, width, height, baselineoffset, m_pFont, m_pBrush);
				return;
			}

			m_oText.CommandText(bsText, bsGid, x, y, width, height, baselineoffset, m_dCurrentBaselineOffset);
			m_dCurrentBaselineOffset = 0.0;
		}

		virtual void WriteTable(NSEBook::ILogicItem* pItem)
		{
			m_arItems.AddTail(pItem);
		}

		AVSINLINE void WriteTableTop(double dTop)
		{
			ILogicItem* pItem = m_arItems.GetTail();
			if (NULL != pItem)
			{
				if (ILogicItem::elitTable == pItem->m_eType)
					pItem->m_rcBounds.top = dTop;				
			}
		}

		virtual void BeginShape()
		{
			if (0 == m_lShapeTop)
				m_oCurrentShape.CreateShape(m_dWidth, m_dHeight);
			++m_lShapeTop;
		}
		virtual void EndShape()
		{
			--m_lShapeTop;
			if (0 == m_lShapeTop)
			{
				// все, готовый шейп
				m_oCurrentShape.GetShape((CBaseWriter*)this);
			}
		}

		virtual void DrawPath(LONG lType)
		{
			m_oCurrentShape.DrawPath(lType, m_pPen, m_pBrush);
		}
		virtual void Clear()
		{
			POSITION pos = m_arItems.GetHeadPosition();
			while (NULL != pos)
			{
				ILogicItem* pItem = m_arItems.GetNext(pos);
				RELEASEOBJECT(pItem);
			}
			m_arItems.RemoveAll();

			m_oCurrentShape.Clear();
			m_lShapeTop = 0;
		}

		virtual void BeginPage()
		{
			m_bIsPar = false;
			m_lShapeTop = 0;
			m_dCurrentBaselineOffset = 0;
			m_dLastTop = 100000;
		}
		virtual void EndPage()
		{
			if (m_bIsPar)
			{
				m_oText.m_oParagraph.m_bIsComplete = false;
				EndParagraph();
			}

			m_bIsDumpWatermark = true;
		}
		virtual void CloseFile(NSEBook::CFormat& oFormat)
		{
		}

		virtual void WriteImage(IUnknown* punkImage, double& x, double& y, double& width, double& height)
		{
			if (!m_bIsPar)
			{
				CLogicImage* pImage = new CLogicImage();
				pImage->m_oImageInfo = m_pImageManager->WriteImage(punkImage, x, y, width, height);
				pImage->m_rcBounds.left		= x;
				pImage->m_rcBounds.top		= y;
				pImage->m_rcBounds.right	= x + width;
				pImage->m_rcBounds.bottom	= y + height;
				m_arItems.AddTail(pImage);
			}
			else
			{
				CEbookParagraph::CSpan oSpan;
				oSpan.m_oImageInfo = m_pImageManager->WriteImage(punkImage, x, y, width, height);
				oSpan.m_dImageWidth	 = width;
				oSpan.m_dImageHeight = height;

				oSpan.m_dX			= x;
				oSpan.m_dWidthSpan	= width;

				m_oText.m_oParagraph.m_rcBounds.top = min(m_oText.m_oParagraph.m_rcBounds.top, y);
				m_oText.m_oParagraph.m_rcBounds.bottom = max(m_oText.m_oParagraph.m_rcBounds.bottom, y);
				
				CEbookParagraph::CLine& oCurLine = m_oText.GetCurrentLine();

				if (0 == oCurLine.m_listSpans.GetCount())
				{
					oCurLine.m_listSpans.AddTail(oSpan);
				}
				else
				{
					CEbookParagraph::CSpan& oTail = oCurLine.m_listSpans.GetTail();
					if (-1 == oTail.m_oImageInfo.m_lID)
					{
						if (c_dSpaceW < abs(oTail.m_dX + oTail.m_dWidthSpan - x))
						{
							oTail.m_oText.AddSpace();
						}
					}
					oCurLine.m_listSpans.AddTail(oSpan);
				}
			}
		}
		virtual void WriteShape(IUnknown* punkImage, double& x, double& y, double& width, double& height)
		{
			WriteImage(punkImage, x, y, width, height);
		}
		virtual void WriteImage(CString strFilePath, double& x, double& y, double& width, double& height)
		{
			if (!m_bIsPar)
			{
				CLogicImage* pImage = new CLogicImage();
				pImage->m_oImageInfo = m_pImageManager->WriteImage(strFilePath, x, y, width, height);	
				pImage->m_rcBounds.left		= x;
				pImage->m_rcBounds.top		= y;
				pImage->m_rcBounds.right	= x + width;
				pImage->m_rcBounds.bottom	= y + height;
				m_arItems.AddTail(pImage);
			}
			else
			{
				CEbookParagraph::CSpan oSpan;
				oSpan.m_oImageInfo = m_pImageManager->WriteImage(strFilePath, x, y, width, height);
				oSpan.m_dImageWidth	 = width;
				oSpan.m_dImageHeight = height;

				oSpan.m_dX			= x;
				oSpan.m_dWidthSpan	= width;
				
				CEbookParagraph::CLine& oCurLine = m_oText.GetCurrentLine();
				
				if (0 == oCurLine.m_listSpans.GetCount())
				{
					oCurLine.m_listSpans.AddTail(oSpan);
				}
				else
				{
					CEbookParagraph::CSpan& oTail = oCurLine.m_listSpans.GetTail();
					if (-1 == oTail.m_oImageInfo.m_lID)
					{
						if (c_dSpaceW < abs(oTail.m_dX + oTail.m_dWidthSpan - x))
						{
							oTail.m_oText.AddSpace();
						}
					}
					oCurLine.m_listSpans.AddTail(oSpan);
				}
			}
		}

		virtual void InitProp()
		{
			m_oText.m_pFontsDst = &m_oFontsGen;
			m_oText.m_pFontManager = &m_oFontManager;
			m_oText.SetParams(m_pPen, m_pBrush, m_pFont, m_pTransform);
			m_oFontsGen.m_lCountFonts = 0;
		}
	};
}
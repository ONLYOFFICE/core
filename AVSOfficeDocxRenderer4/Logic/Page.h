#pragma once
#include "ElementShape.h"
#include "ElementParagraph.h"
#include "ElementImage.h"
#include "ElementTable.h"
#include "TableDetector.h"

namespace NSDocxRenderer
{
	static CString scXmlParagraphEnd		=	L"</w:p>";
	static CString scXmlPgSzVert			=	L"<w:pgSz w:w=\"%d\" w:h=\"%d\" w:orient=\"portrait\"/>";
	static CString scXmlPgSzHor				=	L"<w:pgSz w:w=\"%d\" w:h=\"%d\" w:orient=\"landscape\"/>";
	static CString scXmlSectPrBegin			=	L"<w:p><w:pPr><w:sectPr>";

	static _bstr_t g_bstr_lastSect			=	L"<w:type w:val=\"continuous\"/>";
	static _bstr_t g_bstr_sectEnd			=	L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\"/></w:sectPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr></w:p>";

	static _bstr_t g_bstr_drawingParStart	=	L"<w:p><w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>";

	static CString g_bstr_lastsection1		=	L"<w:sectPr>";
	static CString g_bstr_lastsection2		=	L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\" w:header=\"0\" w:footer=\"0\" w:gutter=\"0\"/></w:sectPr>";
	
	class CPage
	{
	public:
		CPage() : m_oManager(), m_oManagerLight()
		{
			m_pFont						=	NULL;
			m_pBrush					=	NULL;
			m_pPen						=	NULL;
			m_pShadow					=	NULL;
			m_pEdgeText					=	NULL;

			m_pTransform				=	NULL;
			m_pSimpleGraphicsConverter	=	NULL;

			m_dWidth					=	0;
			m_dHeight					=	0;

			m_pCurrentLine				=	NULL;
			m_eTextAssociationType		=	TextAssociationTypeNoFrames;

			m_bIsDeleteTextClipPage		=	true;

			m_pTableDetector			=	NULL;
		}

	public:
		inline void Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, NSDocxRenderer::CMatrix* pMatrix, AVSGraphics::IAVSGraphicSimpleComverter* pSimple)
		{
			m_pFont		= pFont;
			m_pPen		= pPen;
			m_pBrush	= pBrush;
			m_pShadow	= pShadow;
			m_pEdgeText	= pEdge;

			m_pTransform				= pMatrix;
			m_pSimpleGraphicsConverter	= pSimple;

			m_oManager.m_pFont			= m_pFont;
			m_oManager.m_pTransform		= m_pTransform;

			m_pCurrentLine = NULL;

			m_oWriterVML.AddSize(1000, 1000);
		}

		inline void Clear()
		{
			size_t nCount = 0;

			nCount = m_arTextLine.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CTextLine* pTemp = m_arTextLine[i];
				RELEASEOBJECT(pTemp);
			}
			m_arTextLine.RemoveAll();

			nCount = m_arGraphicItems.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CBaseItem* pTemp = m_arGraphicItems[i];
				RELEASEOBJECT(pTemp);
			}
			m_arGraphicItems.RemoveAll();

			nCount = m_arParagraphs.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CParagraph* pTemp = m_arParagraphs[i];
				RELEASEOBJECT(pTemp);
			}
			m_arParagraphs.RemoveAll();

			m_pCurrentLine = NULL;

			RemoveTables();

			m_oWriterVML.ClearNoAttack();
		}

		~CPage()
		{
			Clear();
		}

		inline void SetCurrentLineByBaseline(const double& dBaseLinePos)
		{
			if ((NULL == m_pCurrentLine) || (TextAssociationTypeDefault == m_eTextAssociationType))
			{
				// пустая (в плане текста) страница
				m_pCurrentLine = new CTextLine();

				m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
				m_arTextLine.Add(m_pCurrentLine);
				return;
			}
			if (fabs(m_pCurrentLine->m_dBaselinePos - dBaseLinePos) <= THE_SAME_STRING_Y_PRECISION_MM)
			{
				return;
			}
			size_t nCount = m_arTextLine.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (fabs(m_arTextLine[i]->m_dBaselinePos - dBaseLinePos) <= THE_SAME_STRING_Y_PRECISION_MM)
				{
					m_pCurrentLine = m_arTextLine[i];
					return;
				}
			}

			// линия не нашлась - не беда - создадим новую
			m_pCurrentLine = new CTextLine();
			m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
			m_arTextLine.Add(m_pCurrentLine);
			return;
		}

		// image commands
		inline void WriteImage(CImageInfo& oInfo, double& fX, double& fY, double& fWidth, double& fHeight)
		{
			CImage* pImage = new CImage(oInfo, _T(""));

			double dRotation = m_pTransform->z_Rotation();

			if (fabs(dRotation) < 5.0)
			{
				double x1 = fX;
				double y1 = fY;
				double x2 = fX + fWidth;
				double y2 = fY + fHeight;

				m_pTransform->TransformPoint(x1, y1);
				m_pTransform->TransformPoint(x2, y2);

				if (x1 <= x2)
				{
					pImage->m_dLeft		= x1;
					pImage->m_dWidth	= x2 - x1;
				}
				else
				{
					pImage->m_dLeft		= x2;
					pImage->m_dWidth	= x1 - x2;
				}

				if (y1 <= y2)
				{
					pImage->m_dTop		= y1;
					pImage->m_dHeight	= y2 - y1;
				}
				else
				{
					pImage->m_dTop		= y2;
					pImage->m_dHeight	= y1 - y2;
				}

				pImage->m_dRotate = 0.0;
			}
			else
			{
				double x1 = fX;
				double y1 = fY;
				double x2 = fX + fWidth;
				double y2 = fY + fHeight;

				NSDocxRenderer::CMatrix oTemp  = *m_pTransform;

				double dCx = (x1 + x2) / 2;
				double dCy = (y1 + y2) / 2;
				m_pTransform->TransformPoint(dCx, dCy);
				oTemp.RotateAt(-dRotation, dCx, dCy, Aggplus::MatrixOrderAppend);

				oTemp.TransformPoint(x1, y1);
				oTemp.TransformPoint(x2, y2);

				if (x1 <= x2)
				{
					pImage->m_dLeft		= x1;
					pImage->m_dWidth	= x2 - x1;
				}
				else
				{
					pImage->m_dLeft		= x2;
					pImage->m_dWidth	= x1 - x2;
				}

				if (y1 <= y2)
				{
					pImage->m_dTop		= y1;
					pImage->m_dHeight	= y2 - y1;
				}
				else
				{
					pImage->m_dTop		= y2;
					pImage->m_dHeight	= y1 - y2;
				}

				pImage->m_dRotate = dRotation;
			}

			m_arGraphicItems.Add(pImage);
		}

		// path commands
		inline void MoveTo(double& dX, double& dY)
		{
			m_pTransform->TransformPoint(dX, dY);
			m_oVector.MoveTo(dX, dY);

			//ATLTRACE (L"MOVE : (%f, %f)\n", dX, dY);

		}
		inline void LineTo(double& dX, double& dY)
		{
			m_pTransform->TransformPoint(dX, dY);
			m_oVector.LineTo(dX, dY);

			//ATLTRACE (L"LINE : (%f, %f)\n", dX, dY);
		}
		inline void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_pTransform->TransformPoint(x1, y1);
			m_pTransform->TransformPoint(x2, y2);
			m_pTransform->TransformPoint(x3, y3);

			m_oVector.CurveTo(x1, y1, x2, y2, x3, y3);
		}
		inline void Start()
		{			
		}
		inline void End()
		{
			m_oVector.End();
			m_oWriterVML.ClearNoAttack();
		}
		inline void Close()
		{
			m_oVector.Close();
		}
		inline void DrawPath(LONG lType, LONG lTxId)
		{
			if ((m_oVector.m_dLeft <= m_oVector.m_dRight) && (m_oVector.m_dTop <= m_oVector.m_dBottom))
			{
				CShape* pShape = new CShape();

				pShape->m_lTxId		= lTxId;

				pShape->m_oPen		= *m_pPen;
				pShape->m_oBrush	= *m_pBrush;

				// нормализуем толщину линии
				double dScaleTransform = (m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2.0;
				pShape->m_oPen.Size *= dScaleTransform;

				if ((lType & 0x01) == 0x00)
				{
					if ((fabs(m_oVector.m_dLeft - m_oVector.m_dRight) < 0.3) || (fabs(m_oVector.m_dTop - m_oVector.m_dBottom) < 0.3))
					{
						lType = 0x01;
						pShape->m_bIsStroke  = true;
						pShape->m_oPen.Color = m_pBrush->Color1;
						pShape->m_oPen.Alpha = m_pBrush->Alpha1;
						//pShape->m_oPen.Size	 = max(pShape->m_oPen.Size, 1);	
					}
				}

				pShape->CreateFromVectorData(&m_oVector, m_oWriterVML, 100000, lType);
				m_arGraphicItems.Add(pShape);
			}
		}

		inline void AddTextToLine( CTextLine* pCurLine, const CString& strText, double dBaseLinePos, double dTextX, double dTextY, double dTextW, double dTextH, bool bIsPath )
		{
			CContText* pLastCont = NULL;
			size_t nCountConts = pCurLine->m_arConts.GetCount();
			if (nCountConts != 0)
				pLastCont = pCurLine->m_arConts[nCountConts - 1];

			if (pLastCont)
			{
				// продолжение линии
				double dRight = pLastCont->m_dX + pLastCont->m_dWidth;

				if (pLastCont->m_oFont.IsEqual(&m_oManager.m_oFont.m_oFont) && pLastCont->m_oBrush.IsEqual(m_pBrush))
				{
					// настройки одинаковые. Теперь смотрим, на расположение
					if (fabs(dRight - dTextX) < 0.5)
					{
						// продолжаем слово
						pLastCont->m_strText += strText;
						pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dX);

						if (_T(" ") != strText)
						{
							if (0 == pLastCont->m_dWidthWithoutSpaces)
								pLastCont->m_dLeftWithoutSpaces = dTextX;

							pLastCont->m_dWidthWithoutSpaces = dTextX + dTextW - pLastCont->m_dLeftWithoutSpaces;
						}
						else if (0 == pLastCont->m_dWidthWithoutSpaces)
						{
							pLastCont->m_dLeftWithoutSpaces = dTextX + dTextW;
						}

						return;
					}
					else if ((dRight < dTextX) && ((dTextX - dRight) < m_oManager.m_dSpaceWidthMM))
					{
						// продолжаем слово с пробелом
						pLastCont->m_strText += (_T(" ") + strText);
						pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dX);

						if (_T(" ") != strText)
						{
							if (0 == pLastCont->m_dWidthWithoutSpaces)
								pLastCont->m_dLeftWithoutSpaces = dTextX;

							pLastCont->m_dWidthWithoutSpaces = dTextX + dTextW - pLastCont->m_dLeftWithoutSpaces;
						}
						else if (0 == pLastCont->m_dWidthWithoutSpaces)
						{
							pLastCont->m_dLeftWithoutSpaces = dTextX + dTextW;
						}

						return;
					}
				}
			}

			// либо пробел большой между словами, либо новый текст левее, либо настройки не те (шрифт, кисть)
			// либо все вместе... просто добавляем новое слово
			CContText* pCont = new CContText();

			pCont->m_dX = dTextX;
			pCont->m_dY = dBaseLinePos;

			pCont->m_dWidth		= dTextW;
			pCont->m_dHeight	= dTextH;

			if (_T(" ") == strText)
			{
				pCont->m_dWidthWithoutSpaces	= 0;
				pCont->m_dLeftWithoutSpaces		= dTextX + dTextW;
			}
			else
			{
				pCont->m_dWidthWithoutSpaces	= dTextW;
				pCont->m_dLeftWithoutSpaces		= dTextX;
			}

			pCont->m_strText	= strText;

			pCont->m_oFont		= m_oManager.m_oFont.m_oFont;
			pCont->m_oBrush		= *m_pBrush;

			if (bIsPath)
			{
				pCont->m_strPickFontName	= m_oManager.m_strCurrentPickFont;
				pCont->m_lPickFontStyle		= m_oManager.m_lCurrentPictFontStyle;
			}

			pCont->m_dSpaceWidthMM = m_oManager.m_dSpaceWidthMM;

			pCurLine->AddCont(pCont, m_oManager.m_oFont.m_dBaselineOffset);
		}

		inline void WriteText(BSTR bsText, BSTR bsGid, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, bool bIsPDFAnalyzer)
		{
			double dTextX = fX;
			double dTextY = fY;
			double dTextR = fX + fWidth;
			double dTextB = fY + fHeight;

			m_pTransform->TransformPoint(dTextX, dTextY);
			m_pTransform->TransformPoint(dTextR, dTextB);

			double dTextW = abs(dTextR - dTextX);
			double dTextH = abs(dTextB - dTextY);

			if( dTextX > dTextR )
				dTextX = dTextR;

			if( dTextY > dTextB )
				dTextY = dTextB;

			CString strText = _T("");

			if ((bsText != NULL) && (bsGid != NULL))
			{
				UINT lLen = SysStringLen(bsText);
				for (unsigned int i = 0; i < lLen; ++i)
				{
					if ( IsUnicodeSymbol( bsText[i] ) )
					{
						strText += bsText[i];
					}
					else
					{
						strText += _T(" ");
					}
				}
			}
			else
			{
				strText = bsText;
			}

			bool bIsPath = ((NULL == bsGid) && !bIsPDFAnalyzer) ? false : true;

			m_oManager.LoadFont(0, !bIsPath);

			if (bIsPath)
				m_oManager.GenerateFontName2(strText);

			if ((0 == dTextW) || (dTextW > 5 * m_oManager.m_dSpaceWidthMM))
			{
				double _x = 0;
				double _y = 0;
				double _w = 0;
				double _h = 0;

				if (NULL != bsGid)
				{
					m_oManager.SetStringGid(1);
					m_oManager.MeasureString(bsGid, dTextX, dTextY, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				}
				else
				{
					// такого быть не должно (только из xps)
					m_oManager.SetStringGid(0);
					m_oManager.MeasureString(bsText, dTextX, dTextY, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				}

				dTextW = _w;
				//dTextW *= c_dPixToMM;
			}

			double dBaseLinePos = dTextY + fBaseLineOffset;
			dTextH = m_oManager.GetFontHeight();

			if (m_pCollector)
			{
				for (size_t i = 0; i < m_pCollector->GetCount(); ++i)
				{
					Analyzer::CTable* pTable = m_pCollector->GetTable(i);
					if (NULL == pTable)
						continue;

					Analyzer::CPointInd oInd = pTable->GetIndCell (dTextX, dTextY);
					if (oInd.IsValid ())
					{
						Analyzer::CPoint oCellLeftTop = pTable->GetCellPos(oInd);

						double dX		=	pTable->GetLeftTop().GetX() + oCellLeftTop.GetX();
						double dY		=	pTable->GetLeftTop().GetY() + oCellLeftTop.GetY();

						dTextX			-=	dX;
						dTextY			-=	dY;
						dBaseLinePos	-=	dY;

						CCell* pCell = m_arTables[i]->GetCell(oInd.GetY(), oInd.GetX());
						pCell->m_eTextAssociationType = m_eTextAssociationType;
						pCell->m_pManagerLight = &m_oManagerLight;

						CTextLine* pCurLine = pCell->SetCurrentLineByBaseline(dBaseLinePos);
						AddTextToLine(pCurLine, strText, dBaseLinePos, dTextX, dTextY, dTextW, dTextH, bIsPath);

						return;
					}
				}

				if (_T(" ") == strText)	//	пробелы переходы на новые строки
				{
					for (size_t i = 0; i < m_pCollector->GetCount(); ++i)
					{
						Analyzer::CTable* pTable = m_pCollector->GetTable(i);
						if (NULL == pTable)
							continue;


						if (pTable->IsContainByY(dTextY))// && pTable->IsContainByY(dTextY + dTextH))
						{
							return;
						}
					}
				}
			}

			//if( m_pTableDetector )
			//{
			//	int nRow, nCol;
			//	int nCurTable = 0;

			//	for( CTable* pTable = m_pTableDetector->GetFirstTable(); pTable != NULL; pTable = m_pTableDetector->GetNextTable(), nCurTable++ )
			//	{
			//		if( pTable->IsRectInTable( dTextX, dTextY, dTextX + dTextW, dTextY + dTextH, &nRow, &nCol ) )
			//		{
			//			double x = pTable->GetX() + pTable->GetColumnPosition( nCol );
			//			double y = pTable->GetY() + pTable->GetRowPosition( nRow );

			//			dTextX -= x;
			//			dTextY -= y;
			//			dBaseLinePos -= y;

			//			CCell* pCell = m_arTables[nCurTable]->GetCell( nRow, nCol );
			//			pCell->m_eTextAssociationType = m_eTextAssociationType;
			//			pCell->m_pManagerLight = &m_oManagerLight;
			//			
			//			CTextLine* pCurLine = pCell->SetCurrentLineByBaseline( dBaseLinePos );
			//			AddTextToLine( pCurLine, strText, dBaseLinePos, dTextX, dTextY, dTextW, dTextH, bIsPath );
			//			return;
			//		}
			//	}
			//}

			SetCurrentLineByBaseline(dBaseLinePos);
			AddTextToLine( m_pCurrentLine, strText, dBaseLinePos, dTextX, dTextY, dTextW, dTextH, bIsPath );
		}

		inline void Build()
		{
			SortElements( m_arTables );

			if (m_bIsDeleteTextClipPage)
			{
				// удалим все линии, которые выходят за границы страницы
				size_t nCount = m_arTextLine.GetCount();

				for (size_t i = 0; i < nCount; ++i)
				{
					CTextLine* pTextLine = m_arTextLine[i];

					double dTop		=	pTextLine->m_dBaselinePos - pTextLine->m_dHeight;
					double dBottom	=	pTextLine->m_dBaselinePos;

					if (dTop >= m_dHeight || dBottom <= 0)
					{
						m_arTextLine.RemoveAt(i);
						--i;
						--nCount;						
					}
					else
					{
						//if (m_pCollector)
						//{
						//	if (1 == pTextLine->m_arConts.GetCount())
						//	{
						//		if (_T(" ") == pTextLine->m_arConts[0]->m_strText)
						//		{
						//			for (size_t i = 0; i < m_pCollector->GetCount(); ++i)
						//			{
						//				Analyzer::CTable* pTable = m_pCollector->GetTable(i);
						//				if (pTable)
						//				{
						//					if (pTable->IsContainByY(dTop) && pTable->IsContainByY(dBottom))
						//					{
						//						m_arTextLine.RemoveAt(i);
						//						--i;
						//						--nCount;

						//						break;
						//					}
						//				}
						//			}
						//		}
						//	}
						//}
					}
				}
			}

			switch (m_eTextAssociationType)
			{
			case TextAssociationTypeDefault:
			case TextAssociationTypeLine:
				{
					size_t nCount = m_arTextLine.GetCount();
					for (size_t i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = true;

						pParagraph->m_dLeft	= pTextLine->m_dX;
						pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;

						pParagraph->m_arLines.Add(pTextLine);

						m_arParagraphs.Add(pParagraph);
					}

					// удалим все линии
					m_arTextLine.RemoveAll();
					break;
				}
			case TextAssociationTypeBlock:
				{
					size_t nCount = m_arTextLine.GetCount();

					if (0 == nCount)
						break;

					CTextLine* pFirstLine = m_arTextLine[0];

					CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
					pParagraph->m_pManagerLight = &m_oManagerLight;
					pParagraph->m_bIsTextFrameProperties = true;

					pParagraph->m_dLeft	= pFirstLine->m_dX;
					pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight + pFirstLine->m_dBaselineOffset;

					pParagraph->m_arLines.Add(pFirstLine);

					m_arParagraphs.Add(pParagraph);

					for (size_t i = 1; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = true;

						if (((fabs(pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pFirstLine->m_dBaselinePos) > STANDART_STRING_HEIGHT_MM) && (pTextLine->m_dX == pFirstLine->m_dX)) ||
							((pTextLine->m_dX != pFirstLine->m_dX) && (pTextLine->m_dBaselinePos != pFirstLine->m_dBaselinePos)))
						{
							pParagraph->m_dLeft	= pTextLine->m_dX;
							pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;
						}
						else
						{
							pParagraph->m_dLeft	= pFirstLine->m_dX;
							pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight + pFirstLine->m_dBaselineOffset;
						}

						pFirstLine = pTextLine;

						pParagraph->m_arLines.Add(pTextLine);
						m_arParagraphs.Add(pParagraph);
					}

					// удалим все линии
					m_arTextLine.RemoveAll();
					break;
				}
			case TextAssociationTypeNoFrames:
				{
					SortElements(m_arTextLine);
					Merge(STANDART_STRING_HEIGHT_MM / 3);

					double previousStringOffset = 0;

					size_t nCount = m_arTextLine.GetCount();
					for (size_t i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = false;

						pParagraph->m_dLeft	= pTextLine->m_dX;

						double dTextBaseline = pTextLine->m_dBaselinePos;
						double dTableOffset = 0;

						for( size_t i = 0; i < m_arTables.GetCount(); i++ )
						{
							if( dTextBaseline < m_arTables[i]->m_dY )
								break;

							if( dTextBaseline < m_arTables[i]->m_dY + m_arTables[i]->m_dHeight )
								break;

							dTableOffset += m_arTables[i]->m_dHeight;
						}
						dTextBaseline -= dTableOffset;

						double dBeforeSpacing = dTextBaseline - (pTextLine->m_dHeight - pTextLine->m_dBaselineOffset) - previousStringOffset;

						pParagraph->m_dSpaceBefore = max(dBeforeSpacing, 0);

						double dHeight = 1;
						if (pTextLine->m_dHeight != 0)
						{
							dHeight = pTextLine->m_dHeight;

							if (dBeforeSpacing < 0)
								dHeight += dBeforeSpacing;
						}

						pParagraph->m_dHeight = dHeight;

						previousStringOffset = dTextBaseline + pTextLine->m_dBaselineOffset;

						pParagraph->m_arLines.Add(pTextLine);
						m_arParagraphs.Add(pParagraph);
					}

					m_arTextLine.RemoveAll();
					break;
				}
			}
		}

		inline void Merge(double dAffinity)
		{
			size_t nCount = m_arTextLine.GetCount();
			if (1 < nCount)
			{
				CTextLine* pPrev = m_arTextLine[0];
				for (size_t i = 1; i < nCount; ++i)
				{
					CTextLine* pNext = m_arTextLine[i];

					if (fabs(pNext->m_dBaselinePos - pPrev->m_dBaselinePos) < dAffinity)
					{
						pPrev->Merge(pNext);

						pNext->m_arConts.RemoveAll();
						RELEASEOBJECT(pNext);

						m_arTextLine.RemoveAt(i);
						--i;
						--nCount;
						continue;
					}
					pPrev = pNext;
				}
			}
		}

		inline void Write(NSDocxRenderer::CStringWriter& oWriter)
		{
			// drawings
			size_t nCountDrawings = m_arGraphicItems.GetCount();
			if (0 != nCountDrawings)
			{
				oWriter.WriteString(g_bstr_drawingParStart);

				for (size_t i = 0; i < nCountDrawings; ++i)
				{
					m_arGraphicItems[i]->ToXml(oWriter);
				}

				oWriter.WriteString(scXmlParagraphEnd);
			}

			size_t nCountParagraphs = m_arParagraphs.GetCount();

			size_t nCountTables = m_arTables.GetCount();
			size_t nCurTable = 0;
			double dCurY = 0;

			for (size_t i = 0; i < nCountParagraphs; ++i)
			{
				CParagraph* pPara = m_arParagraphs[i];
				dCurY += pPara->m_dSpaceBefore;

				// ATLTRACE ("%f, %f\n", pPara->m_dSpaceBefore, pPara->m_dHeight);

				if( nCurTable < nCountTables )
				{
					CDocxTable* pTable = m_arTables[nCurTable];

					if( dCurY >= pTable->m_dY )
					{
						pTable->m_dTextLeft = m_dWidth;
						pTable->m_dTextRight = m_dWidth;

						pTable->ToXml( oWriter );

						//dCurY += pTable->m_dHeight;
						++nCurTable;

						//dCurY += pPara->m_dHeight;
						//dCurY = pTable->m_dY;
						pPara->ToXml(oWriter);
						continue;

					}
				}

				dCurY += pPara->m_dHeight;
				pPara->ToXml(oWriter);
			}

			while (nCurTable < nCountTables )
			{
				CDocxTable* pTable = m_arTables[nCurTable];
				pTable->ToXml( oWriter );
				++nCurTable;
			}

			RemoveTables();
		}

		inline void WriteSectionToFile(bool bLastPage, NSDocxRenderer::CStringWriter& oWriter)
		{
			// section
			LONG lWidthDx  = (LONG)(m_dWidth * c_dMMToDx);
			LONG lHeightDx = (LONG)(m_dHeight * c_dMMToDx);

			if (!bLastPage)
				oWriter.WriteString(scXmlSectPrBegin);
			else
				oWriter.WriteString(g_bstr_lastsection1);

			if (lWidthDx >= lHeightDx)
			{
				CString strSize = _T("");
				strSize.Format(scXmlPgSzHor, lWidthDx, lHeightDx);
				oWriter.WriteString(strSize);
			}
			else
			{
				CString strSize = _T("");
				strSize.Format(scXmlPgSzVert, lWidthDx, lHeightDx);
				oWriter.WriteString(strSize);
			}

			if (!bLastPage)
				oWriter.WriteString(g_bstr_sectEnd);
			else
				oWriter.WriteString(g_bstr_lastsection2);
		}

		inline void SetTables( CTableDetector* pTableDetector ) //	DEPRECATED
		{
			RemoveTables();

			m_pTableDetector = pTableDetector;

			if( !pTableDetector )
				return;

			for( CTable* pTable = pTableDetector->GetFirstTable(); pTable != NULL; pTable = pTableDetector->GetNextTable() )
			{
				CDocxTable* t = new CDocxTable();
				if( !t )
					return;

				m_arTables.Add( t );

				t->m_dX = pTable->GetX();
				t->m_dY = pTable->GetY();
				t->m_dWidth = pTable->GetWidth();
				t->m_dHeight = pTable->GetHeight();

				for( int i = 0; i < pTable->GetCountRows(); i++ )
				{
					double size = pTable->GetRowSize( i );
					t->AddRow( size );
				}

				for( int i = 0; i < pTable->GetCountColumns(); i++ )
				{
					double size = pTable->GetColumnSize( i );
					t->AddColumn( size );
				}
			}
		}

		inline void SetTables (Analyzer::CTableCollector* pCollector)
		{
			if (NULL == pCollector)
				return;

			RemoveTables();

			m_pCollector = pCollector;

			for (size_t i = 0; i < pCollector->GetCount(); ++i)
			{
				Analyzer::CTable* pTable = pCollector->GetTable(i);
				if (NULL == pTable)
					continue;

				CDocxTable* pXmlTable = new CDocxTable();
				if (NULL == pXmlTable)
					continue;

				pXmlTable->m_dX			=	pTable->GetLeftTop().GetX();
				pXmlTable->m_dY			=	pTable->GetLeftTop().GetY();
				pXmlTable->m_dWidth		=	pTable->GetSize().GetX();
				pXmlTable->m_dHeight	=	pTable->GetSize().GetY();
				pXmlTable->AddGridCol (pTable->GetGridCol());

				if (pTable->GetRowsCount())
				{
					for (size_t j = 0; j < pTable->GetRowsCount(); ++j)
					{
						pXmlTable->AddRow(pTable->GetRow(j)->GetHeight());
						
						Analyzer::CRowCells* pRow = pTable->GetRow(j);
						for (size_t i = 0; i < pRow->GetCount(); ++i)
						{
							pXmlTable->AddCell(j, pRow->GetCell(i)->GetWidth(), pRow->GetCell(i)->GetSpan());
						}
					}
				}

				m_arTables.Add(pXmlTable);
			}
		}

	private:

		inline void RemoveTables()
		{
			size_t count = m_arTables.GetCount();
			for(size_t i = 0; i < count; ++i)
			{
				delete m_arTables[i];
			}

			m_arTables.RemoveAll();

			m_pTableDetector = NULL;
		}

		// help

		inline bool IsUnicodeSymbol (WCHAR symbol)
		{
			if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || 
				( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||	( ( 0x10000 <= symbol ) && symbol ) )
			{
				return true;  
			}

			return false;		  
		}

	public:

		NSStructures::CFont*						m_pFont;
		NSStructures::CPen*							m_pPen;
		NSStructures::CBrush*						m_pBrush;
		NSStructures::CShadow*						m_pShadow;
		NSStructures::CEdgeText*					m_pEdgeText;

		NSDocxRenderer::CMatrix*					m_pTransform;
		AVSGraphics::IAVSGraphicSimpleComverter*	m_pSimpleGraphicsConverter;

		CVectorGraphics								m_oVector;

		double										m_dWidth;
		double										m_dHeight;

		LONG										m_lCurrentCommand;

		CAtlArray<CBaseItem*>						m_arGraphicItems;
		CAtlArray<CParagraph*>						m_arParagraphs;

		CAtlArray<CTextLine*>						m_arTextLine;
		CTextLine*									m_pCurrentLine;

		CFontManager								m_oManager;
		CFontManagerLight							m_oManagerLight;

		TextAssociationType							m_eTextAssociationType;

		NSDocxRenderer::CStringWriter				m_oWriterVML;

		bool										m_bIsDeleteTextClipPage;

		Analyzer::CTableCollector*					m_pCollector;

	protected:

		CTableDetector*								m_pTableDetector;
		CAtlArray<CDocxTable*>						m_arTables;
	};
}
#pragma once
#include "Formula.h"
#include "../Path.h"

namespace NSGuidesVML
{
#ifndef ENABLE_PPT_TO_PPTX_CONVERT 
	class CSlicePath
	{
	public:
		RulesType m_eRuler;
		CSimpleArray<POINT> m_arPoints;
		CSimpleArray<SPointType> m_arPointsType;

	private:
		int m_nCountElementsPoint;

	public:
		CSlicePath(RulesType eType = rtMoveTo)
		{
			m_eRuler = eType;
			m_nCountElementsPoint = 0;
		}

		void AddParam(LONG lParam, ParamType eParType)
		{
			long lPoint = m_nCountElementsPoint % 2;
			if (0 == lPoint)
			{
				POINT point;
				SPointType pointType;

				point.x = lParam;
				point.y = 0;
				pointType.x = eParType;
				pointType.y = ptValue;
				m_arPoints.push_back(point);
				m_arPointsType.push_back(pointType);
			}
			else
			{
				m_arPoints[m_arPoints.GetSize() - 1].y = lParam;
				m_arPointsType[m_arPoints.GetSize() - 1].y = eParType;
			}
			++m_nCountElementsPoint;
		}
	};
#endif

	class CConverterPPTXPPT
	{
		private:
			CSimpleArray<CSlice> m_arSlices;
			CPPTShape* pPPTShape;
			std::map<CString, LONG> m_arMapFormula;	
			std::map<CString, LONG> m_arMapAdj;
			CSimpleArray<CSlicePath> m_arSlicesPath;
			LONG m_lIndexDst;
			LONG m_lIndexAdj;
			SPointType pPointType1;
			SPointType pPointType2;
			SPointType pPointType3;
			POINT pPoint1;
			POINT pPoint2;
			POINT pPoint3;
			LONG m_lPPTX2PPT;
			LONG m_lPPT2PPTX;
			
		public:

			CConverterPPTXPPT ()
			{
				m_lPPTX2PPT = -1;
				m_lPPT2PPTX = -1;
				m_lIndexDst = -1;
				m_lIndexAdj = -1;
				pPPTShape = new CPPTShape();
			}

			inline CPPTShape* GetConvertedShape()
			{
				return pPPTShape;				
			}

			void Convert ( CPPTXShape* pPPTXShape )
			{
				ConvertAdjastments ( pPPTXShape->m_arAdjustments, pPPTXShape->FManager.mapAdjustments );
				AddSizeGuides ((LONG)pPPTXShape->FManager.GetWidth(), (LONG)pPPTXShape->FManager.GetHeight());
				ConvertGuides ( pPPTXShape->FManager.strGuides, pPPTXShape->FManager.mapGuides);
				ConvertPath( pPPTXShape->m_strPath, pPPTXShape->FManager.strGuides, pPPTXShape->FManager.mapGuides);

				//TODO текстовые поля
				//ConvertTextRects ( pPPTXShape->m_arStringTextRects );
				ConvertHandles ( pPPTXShape->m_arHandles );
				
				//ConvertType ( pPPTXShape->m_eType );
				//------------------------------------ 
				int i=0;
			}

			void ConvertHandles (CSimpleArray<CHandle_>& arHnd)
			{
				
				for (int nIndex=0; nIndex<arHnd.GetSize(); nIndex++)
				{
					CHandle_ oHandle;
					//TODO переименовать названия формул и прокинуть текстовые атрибуты topleft, rightbottom в полях хендла

					pPPTShape->m_arHandles.push_back(oHandle);
				}
				
				return;
			}

			void ConvertAdjastments( CSimpleArray<long> &arAdj, std::map<CString, long> &mapAdj )
			{
				for(int i=0; i<arAdj.GetSize(); i++)
				{
					m_arMapAdj.push_back(mapAdj.GetKeyAt(i), ++m_lIndexAdj);
					pPPTShape->m_arAdjustments.push_back(arAdj[i]);
				}
				//это аджасменты для перевода углов  tan(angle, adj)
				//тк все угла в формулах считаются в pptx
				//pptx->ppt
				pPPTShape->m_arAdjustments.push_back(3114601);
				m_lPPTX2PPT = ++m_lIndexAdj;
				//ppt->pptx
				pPPTShape->m_arAdjustments.push_back(2783638);
				m_lPPT2PPTX = ++m_lIndexAdj;
				return;
			}

			void ConvertTextRects (CAtlArray<CString> &arTextRects)
			{
				pPPTShape->m_arStringTextRects.Copy(arTextRects);
				return;
			}

			void ConvertType (OOXMLShapes::ShapeType eType)
			{
				switch (eType)
				{
					case OOXMLShapes::sptCRect:
						pPPTShape->m_eType = PPTShapes::sptCRect;
					case OOXMLShapes::sptCArc:
						pPPTShape->m_eType = PPTShapes::sptCRect;
					default:
						pPPTShape->m_eType = PPTShapes::sptCustom;
				}
				return;
			}

			void AddSizeGuides (LONG lWidth, LONG lHeight)
			{
				//TODO прокинуть w и h в формулах
				NSGuidesVML::CFormula pNewFmla3;
				pNewFmla3.m_eFormulaType = ftVal;
				pNewFmla3.m_lIndex = ++m_lIndexDst;
				pNewFmla3.m_eType1 = ptValue;
				pNewFmla3.m_lParam1 = 21600;// lWidth;
				pPPTShape->m_oManager.m_arFormulas.push_back(pNewFmla3);
				m_arMapFormula.push_back(_T("w"), m_lIndexDst);

				pNewFmla3.m_lIndex = ++m_lIndexDst;
				pNewFmla3.m_eType1 = ptValue;
				pNewFmla3.m_lParam1 = 21600;//lHeight;
				pPPTShape->m_oManager.m_arFormulas.push_back(pNewFmla3);
				m_arMapFormula.push_back(_T("h"), m_lIndexDst);		

				return;
			}

			LONG ConvertFmlaParam (CString strParam, NSGuidesVML::ParamType &eType, CString strKey, CSimpleArray<NSGuidesOOXML::CFormula> &strGuides, std::map<CString, long> &mapGuides)
			{
				LONG lVal = 0;
				LONG lNumFmla = m_arMapFormula.FindKey(strParam);
				LONG lNumAdj = m_arMapAdj.FindKey(strParam);
				LONG lNumGuides = mapGuides.FindKey(strParam);
				if (lNumAdj != -1)
				{
					eType = ptAdjust;
					lVal = lNumAdj;
				}				
				else if (lNumFmla == -1)
				{
					//пришло число
					if (lNumGuides == -1)
					{						
						lVal = (LONG)XmlUtils::GetInteger(strParam);
						if (lVal > 65535)
						{
							ConvertFmla( ftSumangle, lVal%65536, ptValue, (int)lVal/65536, ptValue, 0, ptValue);
							eType = ptFormula;
							lVal = m_lIndexDst;
						}
						else if (lVal < 0)
						{
							ConvertFmla( ftSum, 0, ptValue, 0, ptValue, abs(lVal), ptValue);
							eType = ptFormula;
							lVal = m_lIndexDst;
						}
						else
							eType = ptValue;
					}
					else //пришла стандартная формула из набора, которую надо теперь добавить
					{
						NSGuidesOOXML::CFormula pFormula = strGuides[lNumGuides];
						strKey = mapGuides.GetKeyAt(lNumGuides);
						ConvertGuid(pFormula, strKey, strGuides, mapGuides);
						eType = ptFormula;
						lVal = m_lIndexDst;
					}
				}
				else
				{
					eType = ptFormula;
					lVal = m_arMapFormula.Lookup(strParam);
				}
				return lVal;
			}

			void ConvertFmla ( FormulaType eFmlaType, LONG lParam1, ParamType eType1, LONG lParam2, ParamType eType2, LONG lParam3, ParamType eType3 )
			{
				NSGuidesVML::CFormula pNewFmla;
				pNewFmla.m_lIndex = ++m_lIndexDst;
				pNewFmla.m_eFormulaType = eFmlaType;

				pNewFmla.m_eType1 = eType1;
				pNewFmla.m_lParam1 = lParam1;

				pNewFmla.m_eType2 = eType2;
				pNewFmla.m_lParam2 = lParam2;

				pNewFmla.m_eType3 = eType3;
				pNewFmla.m_lParam3 = lParam3;

				pPPTShape->m_oManager.m_arFormulas.push_back(pNewFmla);
				return;
			}

			void ConvertFmla ( FormulaType eFmlaType, LONG lParam1, ParamType eType1, LONG lParam2, ParamType eType2 )
			{
				CFormula pNewFmla;
				pNewFmla.m_lIndex = ++m_lIndexDst;
				pNewFmla.m_eFormulaType = eFmlaType;

				pNewFmla.m_eType1 = eType1;
				pNewFmla.m_lParam1 = lParam1;

				pNewFmla.m_eType2 = eType2;
				pNewFmla.m_lParam2 = lParam2;

				pPPTShape->m_oManager.m_arFormulas.push_back(pNewFmla);
				return;
			}

			void ConvertFmla ( FormulaType eFmlaType, LONG lParam1, ParamType eType1 )
			{
				CFormula pNewFmla;
				pNewFmla.m_lIndex = ++m_lIndexDst;
				pNewFmla.m_eFormulaType = eFmlaType;

				pNewFmla.m_eType1 = eType1;
				pNewFmla.m_lParam1 = lParam1;

				pPPTShape->m_oManager.m_arFormulas.push_back(pNewFmla);
				return;
			}

			void ConvertGuid ( NSGuidesOOXML::CFormula pFormula, CString strKey, CSimpleArray<NSGuidesOOXML::CFormula> &strGuides, std::map<CString, long> &mapGuides)
			{				
				LONG lParam1, lParam2, lParam3;
				NSGuidesVML::ParamType eType1, eType2, eType3;
				lParam1 = ConvertFmlaParam(pFormula.m_lParam1, eType1, strKey, strGuides, mapGuides);
				lParam2 = ConvertFmlaParam(pFormula.m_lParam2, eType2, strKey, strGuides, mapGuides);
				lParam3 = ConvertFmlaParam(pFormula.m_lParam3, eType3, strKey, strGuides, mapGuides);

				switch (pFormula.m_eFormulaType)
				{
				case NSGuidesOOXML::CFormula::ftOOXMLSum:
						ConvertFmla( ftSum, lParam1, eType1, lParam2, eType2, lParam3, eType3);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLProduct:
						ConvertFmla( ftProduct, lParam1, eType1, lParam2, eType2, lParam3, eType3);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLIf:
						ConvertFmla( ftIf, lParam1, eType1, lParam2, eType2, lParam3, eType3);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLAbsolute:
						ConvertFmla( ftAbsolute, lParam1, eType1);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLAtan2:
						ConvertFmla( ftAtan2, lParam1, eType1, lParam2, eType2);
						ConvertFmla( ftTan, m_lIndexDst, ptFormula, m_lPPT2PPTX, ptAdjust);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLCos:
						ConvertFmla( ftCos, lParam1, eType1, lParam2, eType2);
						ConvertFmla( ftTan, m_lIndexDst, ptFormula, m_lPPT2PPTX, ptAdjust);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLMax:
						ConvertFmla( ftMax, lParam1, eType1, lParam2, eType2);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLMin:
						ConvertFmla( ftMin, lParam1, eType1, lParam2, eType2);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLMod:
						ConvertFmla( ftMod, lParam1, eType1, lParam2, eType2, lParam3, eType3);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLSinatan2:
						ConvertFmla( ftSinatan2, lParam1, eType1, lParam2, eType2, lParam3, eType3);
						ConvertFmla( ftTan, m_lIndexDst, ptFormula, m_lPPT2PPTX, ptAdjust);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLCosatan2:
						ConvertFmla( ftCosatan2, lParam1, eType1, lParam2, eType2, lParam3, eType3);
						ConvertFmla( ftTan, m_lIndexDst, ptFormula, m_lPPT2PPTX, ptAdjust);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLSin:
						ConvertFmla( ftSin, lParam1, eType1, lParam2, eType2);
						ConvertFmla( ftTan, m_lIndexDst, ptFormula, m_lPPT2PPTX, ptAdjust);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLSqrt:
						ConvertFmla( ftSqrt, lParam1, eType1);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLTan:
						ConvertFmla( ftTan, lParam1, eType1, lParam2, eType2);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLVal:
						ConvertFmla( ftVal, lParam1, eType1);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLAddDivide:
						ConvertFmla( ftSum, lParam1, eType1, lParam2, eType2, 0, ptValue);
						ConvertFmla( ftProduct, m_lIndexDst, ptFormula, 1, ptValue, lParam3, eType3);
						break;
				case NSGuidesOOXML::CFormula::ftOOXMLPin:
						ConvertFmla( ftSum, lParam1, eType1, 0, ptValue, lParam2, eType2);
						ConvertFmla( ftSum, lParam2, eType2, 0, ptValue, lParam3, eType3);
						ConvertFmla( ftIf, m_lIndexDst, ptFormula, lParam3, eType3, lParam2, eType2);
						ConvertFmla( ftIf, m_lIndexDst-2, ptFormula, lParam1, eType1, m_lIndexDst, ptFormula);
						break;
				}	
				m_arMapFormula.push_back(strKey, m_lIndexDst);
				return;
			}


			void ConvertGuides ( CSimpleArray<NSGuidesOOXML::CFormula> &strGuides, std::map<CString, long> &mapGuides )
			{
				//стандартные формулы для пптх будем добавлять, если только они встретятся
				for (int nIndex=32; nIndex<strGuides.GetSize(); ++nIndex)
				{
					NSGuidesOOXML::CFormula pFormula = strGuides[nIndex];
					CString strKey = mapGuides.GetKeyAt(nIndex);
					ConvertGuid(pFormula, strKey, strGuides, mapGuides);
				}
			}

			void ConvertPath(const CString& xml, CSimpleArray<NSGuidesOOXML::CFormula> &strGuides, std::map<CString, long> &mapGuides)
			{
				XmlUtils::CXmlNode pathLst;
				if(pathLst.FromXmlString(xml))
				{
					XmlUtils::CXmlNodes list;
					if (pathLst.GetNodes(_T("path"), list))
					{
						int nPathCount = list.GetCount();
						for (int i=0; i<nPathCount; i++)
						{
							XmlUtils::CXmlNode PathNode;
							if(list.GetAt(i, PathNode))
							{
								XmlUtils::CXmlNodes listNode;
								PathNode.GetNodes(_T("*"), listNode);

								BOOL bFill = PathNode.GetAttribute(_T("fill"), _T("norm")) != _T("none");
								CString stroke = PathNode.GetAttribute(_T("stroke"), _T("true"));
								BOOL bStroke = (stroke == _T("true")) || (stroke == _T("1"));
	
								int nNodeCount = listNode.GetCount();
								for (int j=0; j<nNodeCount; j++)
								{
									XmlUtils::CXmlNode node;
									if(listNode.GetAt(j, node))
									{										
										BOOL bNum = false; //управляем запятыми

										CString strName = node.GetName();
										if (strName == _T("moveTo"))
											pPPTShape->m_strPath += _T("m");
										else if (strName == _T("lnTo"))
											pPPTShape->m_strPath += _T("l");
										else if (strName == _T("arcTo"))
											pPPTShape->m_strPath += _T("ae");
										else if (strName == _T("cubicBezTo"))
											pPPTShape->m_strPath += _T("c");
										else if (strName == _T("quadBezTo"))
											pPPTShape->m_strPath += _T("qb");
										else
											pPPTShape->m_strPath += _T("x");

										if (strName == _T("arcTo"))
										{
											LONG lStartX = m_lIndexDst-1, lStartY = m_lIndexDst;
											LONG lParam1, lParam2, lParam3, lParam4;
											NSGuidesVML::ParamType eType1, eType2, eType3, eType4;
											lParam1 = ConvertFmlaParam(node.GetAttribute(_T("wR")), eType1, node.GetAttribute(_T("wR")), strGuides, mapGuides);
											lParam2 = ConvertFmlaParam(node.GetAttribute(_T("hR")), eType2, node.GetAttribute(_T("hR")), strGuides, mapGuides);
											lParam3 = ConvertFmlaParam(node.GetAttribute(_T("stAng")), eType3, node.GetAttribute(_T("stAng")), strGuides, mapGuides);
											lParam4 = ConvertFmlaParam(node.GetAttribute(_T("swAng")), eType4, node.GetAttribute(_T("swAng")), strGuides, mapGuides);

											//stAng
											ConvertFmla( ftTan, lParam3, eType3, m_lPPTX2PPT, ptAdjust);
											//swAng
											ConvertFmla( ftTan, lParam4, eType4, m_lPPTX2PPT, ptAdjust);
											//a
											ConvertFmla( ftVal, lParam1, eType1);
											//b
											ConvertFmla( ftVal, lParam2, eType2);
											LONG lstAng = m_lIndexDst-3, lswAng = m_lIndexDst-2, la = m_lIndexDst-1, lb = m_lIndexDst;
											//радиус эллипса в stAng--------------------
											//(b*cos)^2
											ConvertFmla( ftCos, m_lIndexDst, ptFormula, m_lIndexDst-3, ptFormula);
											ConvertFmla( ftProduct, m_lIndexDst, ptFormula, m_lIndexDst, ptFormula, 1, ptValue);
											//(a*sin)^2
											ConvertFmla( ftSin, m_lIndexDst-3, ptFormula, m_lIndexDst-5, ptFormula);
											ConvertFmla( ftProduct, m_lIndexDst, ptFormula, m_lIndexDst, ptFormula, 1, ptValue);
											ConvertFmla( ftSum, m_lIndexDst-2, ptFormula, m_lIndexDst, ptFormula, 0, ptValue);
											ConvertFmla( ftSqrt, m_lIndexDst, ptFormula);
											//r
											ConvertFmla( ftProduct, la, ptFormula, lb, ptFormula, m_lIndexDst, ptFormula);

											//r*cos(stAng)
											ConvertFmla( ftCos, m_lIndexDst, ptFormula, lstAng, ptFormula);
											//xc
											ConvertFmla( ftSum, lStartX, ptFormula, 0, ptValue, m_lIndexDst, ptFormula);
											//r*sin(stAng)
											ConvertFmla( ftSin, m_lIndexDst-2, ptFormula, lstAng, ptFormula);
											//yc
											ConvertFmla( ftSum, lStartY, ptFormula, 0, ptValue, m_lIndexDst, ptFormula);
											//endAng
											ConvertFmla( ftSum, lstAng, ptFormula, lswAng, ptFormula, 0, ptValue);

											//радиус эллипса в endAng--------------------
											//(b*cos)^2
											ConvertFmla( ftCos, lb, ptFormula, m_lIndexDst, ptFormula);
											ConvertFmla( ftProduct, m_lIndexDst, ptFormula, m_lIndexDst, ptFormula, 1, ptValue);
											//(a*sin)^2
											ConvertFmla( ftSin, la, ptFormula, m_lIndexDst-2, ptFormula);
											ConvertFmla( ftProduct, m_lIndexDst, ptFormula, m_lIndexDst, ptFormula, 1, ptValue);
											ConvertFmla( ftSum, m_lIndexDst-2, ptFormula, m_lIndexDst, ptFormula, 0, ptValue);
											ConvertFmla( ftSqrt, m_lIndexDst, ptFormula);
											//r
											ConvertFmla( ftProduct, la, ptFormula, lb, ptFormula, m_lIndexDst, ptFormula);

											//r*cos(endAng)
											ConvertFmla( ftCos, m_lIndexDst, ptFormula, m_lIndexDst-7, ptFormula);
											//xEnd
											ConvertFmla( ftSum, m_lIndexDst-11, ptFormula, m_lIndexDst, ptFormula, 0, ptValue);
											//r*sin(endAng)
											ConvertFmla( ftSin, m_lIndexDst-2, ptFormula, m_lIndexDst-9, ptFormula);
											//yEnd
											ConvertFmla( ftSum, m_lIndexDst-11, ptFormula, m_lIndexDst, ptFormula, 0, ptValue );

											CString xC = _T(""); xC.Format(_T("@%d"), m_lIndexDst-14);
											CString yC = _T(""); yC.Format(_T("@%d"), m_lIndexDst-12);
											CString wR = _T(""); wR.Format(_T("@%d"), la);
											CString hR = _T(""); hR.Format(_T("@%d"), lb);

											//360
											ConvertFmla( ftSumangle, 0, ptValue, 360, ptValue, 0, ptValue );
											//360-stAng
											ConvertFmla( ftSum, m_lIndexDst, ptFormula, 0, ptValue, lstAng, ptFormula);
											//-1
											ConvertFmla( ftSum, 0, ptValue, 0, ptValue, 1, ptValue);
											//-1*swAng
											ConvertFmla( ftProduct, lswAng, ptFormula, m_lIndexDst, ptFormula, 1, ptValue);

											CString stAng = _T(""); stAng.Format(_T("@%d"), m_lIndexDst-2);
											CString swAng = _T(""); swAng.Format(_T("@%d"), m_lIndexDst);
											//CString xEnd = _T(""); xEnd.Format(_T("@%d"), m_lIndexDst-5);
											//CString yEnd = _T(""); yEnd.Format(_T("@%d"), m_lIndexDst-4);

											pPPTShape->m_strPath += xC + yC + wR + hR + stAng + swAng;

											ConvertFmla( ftVal, m_lIndexDst-6, ptFormula);
											ConvertFmla( ftVal, m_lIndexDst-5, ptFormula);

										}
										else
										{
											XmlUtils::CXmlNodes ptList;
											node.GetNodes(_T("pt"), ptList);
											int nPtCount = ptList.GetCount();									

											for (int k=0; k<nPtCount; k++)
											{
												BOOL bLastPount = false;
												if ( k == nPtCount-1)
													bLastPount = true;

												XmlUtils::CXmlNode ptNode;
												if(ptList.GetAt(k, ptNode))
												{
													CString ptX = ptNode.GetAttribute(_T("x"));
													CString ptY = ptNode.GetAttribute(_T("y"));

													pPPTShape->m_strPath += ConvertPathPoint( ptX, ptY, bNum, strGuides, mapGuides, bLastPount);													
												}
											}
										}
									}									
								}
								if (!bFill)
											pPPTShape->m_strPath += _T("nf");
								if (!bStroke)
									pPPTShape->m_strPath += _T("ns");
							}
							pPPTShape->m_strPath += _T("e");
						}
					}
				}
				return;
			}

			LONG ConvertArcParam (CString strParam, NSGuidesVML::ParamType &eType)
			{
				LONG lVal = 0;
				LONG lNumFmla = m_arMapFormula.FindKey(strParam);
				LONG lNumAdj = m_arMapAdj.FindKey(strParam);
				if (lNumAdj != -1)
				{
					eType = ptAdjust;
					lVal = lNumAdj;
				}				
				else if (lNumFmla == -1)
				{
					eType = ptValue;
					lVal = (LONG)XmlUtils::GetInteger(strParam);

				}
				else
				{
					eType = ptFormula;
					lVal = m_arMapFormula.Lookup(strParam);
				}
				return lVal;
			}

			CString ConvertPathPoint (CString strX, CString strY, BOOL &bNum, CSimpleArray<NSGuidesOOXML::CFormula> &strGuides, std::map<CString, long> &mapGuides, BOOL bLPoint)
			{
				CString strRes = _T("");
				ParamType eType1, eType2;
				LONG lParam1, lParam2;

				if (bNum)
					strRes += _T(",");
				lParam1 = ConvertFmlaParam(strX, eType1, strX, strGuides, mapGuides);
				strRes += ConvertPathParam (lParam1, eType1, bNum);

				if (bNum)
					strRes += _T(",");

				lParam2 = ConvertFmlaParam(strY, eType2, strY, strGuides, mapGuides);
				strRes += ConvertPathParam (lParam2, eType2, bNum);

				if (bLPoint)
				{
					ConvertFmla( ftVal, lParam1, eType1);
					ConvertFmla( ftVal, lParam2, eType2);
				}
				return strRes;
			}

			CString ConvertPathParam (LONG lParam, ParamType eType, BOOL &bNum)
			{				
				CString strRes = _T("");
				if (eType == ptAdjust)
					strRes.Format(_T("#%d"), lParam);
				else if (eType == ptValue)
				{
					strRes.Format(_T("%d"), lParam);
					bNum = true;
				}
				else
					strRes.Format(_T("@%d"), lParam);

				return strRes;
			}
	};
}

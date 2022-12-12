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

#include "Geometry.h"
#include "../../../MsBinaryFile/PptFile/Drawing/Elements.h"
#include "../../../MsBinaryFile/Common/Vml/toVmlConvert.h"

namespace PPTX
{
	namespace Logic
	{
		void Geometry::ConvertToCustomVML(IRenderer* pOOXToVMLRenderer, std::wstring& strPath, std::wstring& strRect, LONG& lWidth, LONG& lHeight)
		{
			if (!pOOXToVMLRenderer) return;

			PPT::CShapeElement* lpShapeElement = NULL;
			if (this->is<PPTX::Logic::PrstGeom>())
			{
				const PPTX::Logic::PrstGeom & lpGeom = this->as<PPTX::Logic::PrstGeom>();

				OOXMLShapes::ShapeType _lspt = PPTX2EditorAdvanced::GetShapeTypeFromStr(lpGeom.prst.get());
				if(_lspt == OOXMLShapes::sptNil) 
					return;

				lpShapeElement = new PPT::CShapeElement(NSBaseShape::pptx, (int)_lspt);
				std::wstring strAdjustValues = lpGeom.GetODString();
				
				lpShapeElement->m_pShape->getBaseShape()->LoadAdjustValuesList(strAdjustValues);
			}
			else if (this->is<PPTX::Logic::CustGeom>())
			{
				const PPTX::Logic::CustGeom & lpGeom = this->as<PPTX::Logic::CustGeom>();
				std::wstring strShape = lpGeom.GetODString();
				lpShapeElement = new PPT::CShapeElement(strShape);
			}

			if (lpShapeElement == NULL)
				return;

            double dCoordSizeX = (double)(std::max)((int)lWidth, 1);
            double dCoordSizeY = (double)(std::max)((int)lHeight, 1);

			LONG lCoordSize = 100000;

			lpShapeElement->m_pShape->getBaseShape()->SetWidthHeightLogic(dCoordSizeX, dCoordSizeY);
			lpShapeElement->m_pShape->getBaseShape()->ReCalculate();

			pOOXToVMLRenderer->put_Width((double)lCoordSize / dCoordSizeX);
			pOOXToVMLRenderer->put_Height((double)lCoordSize / dCoordSizeY);						

			CGeomShapeInfo oInfo;
			
			oInfo.m_dLeft	= 0;
			oInfo.m_dTop	= 0;
			
			oInfo.m_dWidth	= dCoordSizeX;
			oInfo.m_dHeight	= dCoordSizeY;

			ODRAW::CPath& oPath = lpShapeElement->m_pShape->getBaseShape()->m_oPath;
			
			COOXToVMLGeometry* pOOXToVMLGeometry = dynamic_cast<COOXToVMLGeometry*>(pOOXToVMLRenderer);

            if(NULL != pOOXToVMLGeometry)
				pOOXToVMLGeometry->NewShape();

			ODRAW::CGraphicPath oGrPath;			

			for (size_t nIndex = 0; nIndex < oPath.m_arParts.size(); ++nIndex)
			{
				oGrPath.Clear();
				oPath.m_arParts[nIndex].ToRendererOOX(&oGrPath, oInfo, NSBaseShape::pptx);

				pOOXToVMLRenderer->put_PenAlpha(oGrPath.m_bStroke ? 255 : 0);
				pOOXToVMLRenderer->put_BrushAlpha1(oGrPath.m_bFill ? 255 : 0);

				oGrPath.Draw(pOOXToVMLRenderer);
			}
			if(NULL != pOOXToVMLGeometry)
				pOOXToVMLGeometry->ResultPath(&strPath);

			if (lpShapeElement->m_pShape->getBaseShape()->m_arTextRects.size() <= 0)
			{
				strRect = _T("0,0,100000,100000");
			}
			else
			{
				Aggplus::RECT& txRect = lpShapeElement->m_pShape->getBaseShape()->m_arTextRects[0];
				//double dkoefX = (double)lCoordSize / max(1, dCoordSizeX);
				//double dkoefY = (double)lCoordSize / max(1, dCoordSizeY);

				double _dWidth = ShapeSize;
				double _dHeight = ShapeSize;
				lpShapeElement->m_pShape->getBaseShape()->GetWidthHeightLogic(_dWidth, _dHeight);

                double dkoefX = (double)lCoordSize / (std::max)(1., _dWidth);
                double dkoefY = (double)lCoordSize / (std::max)(1., _dHeight);

                strRect =   std::to_wstring((int)(dkoefX * txRect.left))    + L"," +
                            std::to_wstring((int)(dkoefY * txRect.top))     + L"," +
                            std::to_wstring((int)(dkoefX * txRect.right))   + L"," +
                            std::to_wstring((int)(dkoefY * txRect.bottom));
			}
            if (lpShapeElement)
                delete lpShapeElement;
        }

		Geometry::Geometry()
		{
		}
		OOX::EElementType Geometry::getType() const
		{
			if (m_geometry.IsInit())
				return m_geometry->getType();
			return OOX::et_Unknown;
		}
		void Geometry::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (strName == _T("prstGeom"))
				m_geometry.reset(new Logic::PrstGeom(oReader));
			else if (strName == _T("custGeom"))
				m_geometry.reset(new Logic::CustGeom(oReader));
			else
				m_geometry.reset();
		}
		void Geometry::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = XmlUtils::GetNameNoNS(node.GetName());

			if (strName == _T("prstGeom"))
				m_geometry.reset(new Logic::PrstGeom(node));
			else if (strName == _T("custGeom"))
				m_geometry.reset(new Logic::CustGeom(node));
			else m_geometry.reset();
		}
		void Geometry::GetGeometryFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:prstGeom"), oNode))
				m_geometry.reset(new Logic::PrstGeom(oNode));
			else if (element.GetNode(_T("a:custGeom"), oNode))
				m_geometry.reset(new Logic::CustGeom(oNode));
			else m_geometry.reset();
		}
		std::wstring Geometry::toXML() const
		{
			if (m_geometry.IsInit())
				return m_geometry->toXML();
			return _T("");
		}
		void Geometry::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_geometry.is_init())
				m_geometry->toXmlWriter(pWriter);
		}
		void Geometry::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (m_geometry.is_init())
				m_geometry->toPPTY(pWriter);
		}
		void Geometry::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			if (pReader->GetPos() < _end_rec)
			{
				BYTE _t = pReader->GetUChar();

				if (GEOMETRY_TYPE_PRST == _t)
				{
					// preset shape
					LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;
					pReader->Skip(1); // start attributes

					Logic::PrstGeom* pGeom = new Logic::PrstGeom();

					while (true)
					{
						BYTE _at = pReader->GetUChar_TypeNode();
						if (_at == NSBinPptxRW::g_nodeAttributeEnd)
							break;

						if (0 == _at)
							pGeom->prst.set(pReader->GetString2());
						else
							break;
					}

					while (pReader->GetPos() < _e)
					{
						BYTE _at = pReader->GetUChar();
						switch (_at)
						{
							case 0:
							{
								LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;
								ULONG _c = pReader->GetULong();

								for (ULONG i = 0; i < _c; ++i)
								{
									pReader->Skip(1);
									pGeom->avLst.push_back(Gd());
									pGeom->avLst[i].fromPPTY(pReader);
								}

								pReader->Seek(_end_rec2);
								break;
							}
							default:
								break;
						}
					}

					m_geometry.reset(pGeom);
				}
				else if (GEOMETRY_TYPE_CUSTOM == _t)
				{
					LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

					Logic::CustGeom* pGeom = new Logic::CustGeom();
					while (pReader->GetPos() < _e)
					{
						BYTE _at = pReader->GetUChar();
						switch (_at)
						{
							case 0:
							{
								LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;
								ULONG _c = pReader->GetULong();

								for (ULONG i = 0; i < _c; ++i)
								{
									pReader->Skip(1);
									pGeom->avLst.push_back(Gd());
									pGeom->avLst[i].fromPPTY(pReader);
								}

								pReader->Seek(_end_rec2);
								break;
							}
							case 1:
							{
								LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;
								ULONG _c = pReader->GetULong();

								for (ULONG i = 0; i < _c; ++i)
								{
									pReader->Skip(1);
									pGeom->gdLst.push_back(Gd());
									pGeom->gdLst[i].fromPPTY(pReader);
								}

								pReader->Seek(_end_rec2);
								break;
							}
							case 2:
							{
								LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;
								ULONG _c = pReader->GetULong();

								for (ULONG i = 0; i < _c; ++i)
								{
									BYTE _type1 = pReader->GetUChar();
									pReader->Skip(4); // len
									BYTE _type = pReader->GetUChar();
									pReader->Skip(5); // len + start attributes

									if (1 == _type)
									{
										Logic::AhPolar* p = new Logic::AhPolar();
										while (true)
										{
											BYTE _at2 = pReader->GetUChar_TypeNode();
											if (_at2 == NSBinPptxRW::g_nodeAttributeEnd)
												break;

											switch (_at2)
											{
											case 0: p->x = pReader->GetString2(); break;
											case 1: p->y = pReader->GetString2(); break;
											case 2: p->gdRefAng = pReader->GetString2(); break;
											case 3: p->gdRefR = pReader->GetString2(); break;
											case 4: p->maxAng = pReader->GetString2(); break;
											case 5: p->maxR = pReader->GetString2(); break;
											case 6: p->minAng = pReader->GetString2(); break;
											case 7: p->minR = pReader->GetString2(); break;
											default:
												break;
											}
										}
										pGeom->ahLst.push_back(AhBase());
										pGeom->ahLst[i].ah.reset(p);
									}
									else
									{
										Logic::AhXY* p = new Logic::AhXY();
										while (true)
										{
											BYTE _at2 = pReader->GetUChar_TypeNode();
											if (_at2 == NSBinPptxRW::g_nodeAttributeEnd)
												break;

											switch (_at2)
											{
											case 0: p->x = pReader->GetString2(); break;
											case 1: p->y = pReader->GetString2(); break;
											case 2: p->gdRefX = pReader->GetString2(); break;
											case 3: p->gdRefY = pReader->GetString2(); break;
											case 4: p->maxX = pReader->GetString2(); break;
											case 5: p->maxY = pReader->GetString2(); break;
											case 6: p->minX = pReader->GetString2(); break;
											case 7: p->minY = pReader->GetString2(); break;
											default:
												break;
											}
										}
										pGeom->ahLst.push_back(AhBase());
										pGeom->ahLst[i].ah.reset(p);
									}
								}

								pReader->Seek(_end_rec2);
								break;
							}
							case 3:
							{
								LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;

								ULONG _c = pReader->GetULong();

								for (ULONG i = 0; i < _c; ++i)
								{
									BYTE _type = pReader->GetUChar();
									pReader->Skip(5); // len + start attributes

									pGeom->cxnLst.push_back(Cxn());
									while (true)
									{
										BYTE _at2 = pReader->GetUChar_TypeNode();
										if (_at2 == NSBinPptxRW::g_nodeAttributeEnd)
											break;

										switch (_at2)
										{
										case 0:
											pGeom->cxnLst[i].x = pReader->GetString2();
											break;
										case 1:
											pGeom->cxnLst[i].y = pReader->GetString2();
											break;
										case 2:
											pGeom->cxnLst[i].ang = pReader->GetString2();
											break;
										default:
											break;
										}
									}
								}

								pReader->Seek(_end_rec2);
								break;
							}
							case 4:
							{
								LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;
								ULONG _c = pReader->GetULong();

								for (ULONG i = 0; i < _c; ++i)
								{
									BYTE _type = pReader->GetUChar();
									pGeom->pathLst.push_back(Path2D());
									pGeom->pathLst[i].fromPPTY(pReader);
								}

								pReader->Seek(_end_rec2);
								break;
							}
							case 5:
							{
								LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;

								pReader->Skip(1); // start attributes

								pGeom->rect = new Logic::Rect();
								pGeom->rect->m_name = _T("a:rect");
								pGeom->rect->l = _T("l");
								pGeom->rect->t = _T("t");
								pGeom->rect->r = _T("r");
								pGeom->rect->b = _T("b");
								while (true)
								{
									BYTE _at2 = pReader->GetUChar_TypeNode();
									if (_at2 == NSBinPptxRW::g_nodeAttributeEnd)
										break;

									switch (_at2)
									{
									case 0:
										pGeom->rect->l = pReader->GetString2();
										break;
									case 1:
										pGeom->rect->t = pReader->GetString2();
										break;
									case 2:
										pGeom->rect->r = pReader->GetString2();
										break;
									case 3:
										pGeom->rect->b = pReader->GetString2();
										break;
									default:
										break;
									}
								}

								pReader->Seek(_end_rec2);
								break;
							}
							default:
								break;
						}
					}

					m_geometry.reset(pGeom);
				}
			}

			pReader->Seek(_end_rec);
		}
		bool Geometry::is_init() const
		{
			return (m_geometry.IsInit());
		}
		void Geometry::FillParentPointersForChilds() {}
		void Geometry::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				m_geometry->SetParentPointer(pParent);
		}
	}
}


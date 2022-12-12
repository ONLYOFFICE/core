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

#include "Path2D.h"

namespace PPTX
{
	namespace Logic
	{
		Path2D& Path2D::operator=(const Path2D& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			extrusionOk	= oSrc.extrusionOk;
			fill		= oSrc.fill;
			h			= oSrc.h;
			stroke		= oSrc.stroke;
			w			= oSrc.w;

			Paths = oSrc.Paths;

			return *this;
		}
		OOX::EElementType Path2D::getType() const
		{
			return OOX::et_a_path;
		}
		void Path2D::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			Paths.clear();
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				UniPath2D p;
				Paths.push_back(p);
				Paths.back().fromXML(oReader);
			}

			FillParentPointersForChilds();
		}
		void Path2D::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("extrusionOk"), extrusionOk)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("fill"), fill )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("h"), h )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("stroke"), stroke )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w"), w )
			WritingElement_ReadAttributes_End( oReader )
		}
		void Path2D::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"extrusionOk", extrusionOk);
			XmlMacroReadAttributeBase(node, L"fill", fill);
			XmlMacroReadAttributeBase(node, L"h", h);
			XmlMacroReadAttributeBase(node, L"stroke", stroke);
			XmlMacroReadAttributeBase(node, L"w", w);

			Paths.clear();
			XmlMacroLoadArray(node, _T("*"), Paths, UniPath2D);

			FillParentPointersForChilds();
		}
		std::wstring Path2D::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("w"), w);
			oAttr.Write(_T("h"), h);
			oAttr.WriteLimitNullable(_T("fill"), fill);
			oAttr.Write(_T("stroke"), stroke);
			oAttr.Write(_T("extrusionOk"), extrusionOk);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Paths);

			return XmlUtils::CreateNode(_T("a:path"), oAttr, oValue);
		}
		void Path2D::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:path"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("w"), w);
			pWriter->WriteAttribute(_T("h"), h);
			pWriter->WriteAttribute(_T("fill"), fill);
			pWriter->WriteAttribute(_T("stroke"), stroke);
			pWriter->WriteAttribute(_T("extrusionOk"), extrusionOk);
			pWriter->EndAttributes();

			size_t nCount = Paths.size();
			for (size_t i = 0; i < nCount; ++i)
				Paths[i].toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:path"));
		}
		void Path2D::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, extrusionOk);
			pWriter->WriteLimit2(1, fill);
			pWriter->WriteInt2(2, h);
			pWriter->WriteBool2(3, stroke);
			pWriter->WriteInt2(4, w);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecordArray(0, 1, Paths);
		}
		void Path2D::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						extrusionOk = pReader->GetBool();
						break;
					}
					case 1:
					{
						fill = pReader->GetUChar();
						break;
					}
					case 2:
					{
						h = pReader->GetLong();
						break;
					}
					case 3:
					{
						stroke = pReader->GetBool();
						break;
					}
					case 4:
					{
						w = pReader->GetLong();
						break;
					}
					default:
						break;
				}
			}

			while (pReader->GetPos() < _e)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						pReader->Skip(4); // len

						ULONG _c = pReader->GetULong();

						for (ULONG j = 0; j < _c; ++j)
						{
							pReader->Skip(5); // type + len

							BYTE _type = pReader->GetUChar();
							LONG _end = pReader->GetPos() + pReader->GetRecordSize() + 4;

							Paths.push_back(UniPath2D());
							UniPath2D& oPath = Paths[j];

							if (_type == GEOMETRY_TYPE_PATH_CLOZE)
							{
								Logic::Close* p = new Logic::Close();
								oPath.Path2D.reset(p);
								pReader->Seek(_end);
								continue;
							}

							pReader->Skip(1);

							std::wstring arr[6];

							while (true)
							{
								BYTE _at2 = pReader->GetUChar_TypeNode();
								if (_at2 == NSBinPptxRW::g_nodeAttributeEnd)
									break;

								arr[_at2] = pReader->GetString2();
							}

							switch (_type)
							{
							case GEOMETRY_TYPE_PATH_MOVETO:
								{
									Logic::MoveTo* p = new Logic::MoveTo();
									p->x = arr[0];
									p->y = arr[1];
									oPath.Path2D.reset(p);
									break;
								}
							case GEOMETRY_TYPE_PATH_LINETO:
								{
									Logic::LineTo* p = new Logic::LineTo();
									p->x = arr[0];
									p->y = arr[1];
									oPath.Path2D.reset(p);
									break;
								}
							case GEOMETRY_TYPE_PATH_CUBICBEZTO:
								{
									Logic::CubicBezTo* p = new Logic::CubicBezTo();
									p->x[0] = arr[0];
									p->y[0] = arr[1];
									p->x[1] = arr[2];
									p->y[1] = arr[3];
									p->x[2] = arr[4];
									p->y[2] = arr[5];
									oPath.Path2D.reset(p);
									break;
								}
							case GEOMETRY_TYPE_PATH_ARCTO:
								{
									Logic::ArcTo* p = new Logic::ArcTo();
									p->wR = arr[0];
									p->hR = arr[1];
									p->stAng = arr[2];
									p->swAng = arr[3];
									oPath.Path2D.reset(p);
									break;
								}
							case GEOMETRY_TYPE_PATH_QUADBEZTO:
								{
									Logic::QuadBezTo* p = new Logic::QuadBezTo();
									p->x[0] = arr[0];
									p->y[0] = arr[1];
									p->x[1] = arr[2];
									p->y[1] = arr[3];
									oPath.Path2D.reset(p);
									break;
								}
							default:
								break;
							}

							pReader->Seek(_end);
						}

						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_e);
		}
		void Path2D::FillParentPointersForChilds()
		{
			size_t count = Paths.size();
			for (size_t i = 0; i < count; ++i)
				Paths[i].SetParentPointer(this);
		}
		std::wstring Path2D::GetODString()const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("w"), w);
			oAttr.Write(_T("h"), h);
			oAttr.WriteLimitNullable(_T("fill"), fill);
			oAttr.Write(_T("stroke"), stroke);
			oAttr.Write(_T("extrusionOk"), extrusionOk);

			std::wstring strXml = _T("");
			size_t nCount = Paths.size();
			for (size_t i = 0; i < nCount; ++i)
				strXml += Paths[i].GetODString();

			return XmlUtils::CreateNode(_T("path"), oAttr, strXml);
		}
	} // namespace Logic
} // namespace PPTX

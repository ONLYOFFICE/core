#pragma once
#ifndef PPTX_LOGIC_PATH2D_INCLUDE_H_
#define PPTX_LOGIC_PATH2D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/PathFillMode.h"
#include "UniPath2D.h"

namespace PPTX
{
	namespace Logic
	{

		class Path2D : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Path2D)

			Path2D& operator=(const Path2D& oSrc)
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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"extrusionOk", extrusionOk);
				node.ReadAttributeBase(L"fill", fill);
				node.ReadAttributeBase(L"h", h);
				node.ReadAttributeBase(L"stroke", stroke);
				node.ReadAttributeBase(L"w", w);

				Paths.clear();
				node.LoadArray(_T("*"), Paths);
				
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
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
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
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
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;
				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
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
							fill = new Limit::PathFillMode();
							fill->SetBYTECode(pReader->GetUChar());
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
								LONG _end = pReader->GetPos() + pReader->GetLong() + 4;

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

								CString arr[6];

								while (true)
								{
									BYTE _at2 = pReader->GetUChar();
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
										p->x1 = arr[0];
										p->y1 = arr[1];
										p->x2 = arr[2];
										p->y2 = arr[3];
										p->x3 = arr[4];
										p->y3 = arr[5];
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
										p->x1 = arr[0];
										p->y1 = arr[1];
										p->x2 = arr[2];
										p->y2 = arr[3];
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
			
		public:
			nullable_bool							extrusionOk;
			nullable_limit<Limit::PathFillMode>		fill;
			nullable_int							h;
			nullable_bool							stroke; 
			nullable_int							w;

			std::vector<UniPath2D>					Paths;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = Paths.size();
				for (size_t i = 0; i < count; ++i)
					Paths[i].SetParentPointer(this);
			}
		public:
			virtual CString GetODString()const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("w"), w);
				oAttr.Write(_T("h"), h);
				oAttr.WriteLimitNullable(_T("fill"), fill);
				oAttr.Write(_T("stroke"), stroke);
				oAttr.Write(_T("extrusionOk"), extrusionOk);

				CString strXml = _T("");
				size_t nCount = Paths.size();
				for (size_t i = 0; i < nCount; ++i)
					strXml += Paths[i].GetODString();

				return XmlUtils::CreateNode(_T("path"), oAttr, strXml);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATH2D_INCLUDE_H_
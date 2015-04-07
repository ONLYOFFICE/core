#pragma once
#ifndef PPTX_LOGIC_TABLE_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_TABLE_PROPERTIES_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniFill.h"
#include "./../EffectProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class TableProperties : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TableProperties)

			TableProperties& operator=(const TableProperties& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Fill		= oSrc.Fill;
				Effects		= oSrc.Effects;
				TableStyleId = oSrc.TableStyleId;

				Rtl			= oSrc.Rtl;
				FirstRow	= oSrc.FirstRow;
				FirstCol	= oSrc.FirstCol;
				LastRow		= oSrc.LastRow;
				LastCol		= oSrc.LastCol;
				BandRow		= oSrc.BandRow;
				BandCol		= oSrc.BandCol;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Fill.GetFillFrom(node);
				Effects.GetEffectListFrom(node);

				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:tableStyleId"), oNode))
					TableStyleId = oNode.GetTextExt();

				node.ReadAttributeBase(L"rtl", Rtl);
				node.ReadAttributeBase(L"firstRow", FirstRow);
				node.ReadAttributeBase(L"firstCol", FirstCol);
				node.ReadAttributeBase(L"lastRow", LastRow);
				node.ReadAttributeBase(L"lastCol", LastCol);
				node.ReadAttributeBase(L"bandRow", BandRow);
				node.ReadAttributeBase(L"bandCol", BandCol);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("rtl"), Rtl);
				oAttr.Write(_T("firstRow"), FirstRow);
				oAttr.Write(_T("firstCol"), FirstCol);
				oAttr.Write(_T("lastRow"), LastRow);
				oAttr.Write(_T("lastCol"), LastCol);
				oAttr.Write(_T("bandRow"), BandRow);
				oAttr.Write(_T("bandCol"), BandCol);

				XmlUtils::CNodeValue oValue;
				oValue.Write(Fill);
				oValue.Write(Effects);

				if (TableStyleId.IsInit())
					oValue.m_strValue += (_T("<a:tableStyleId>") + *TableStyleId + _T("</a:tableStyleId>"));

				return XmlUtils::CreateNode(_T("a:tblPr"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tblPr"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("rtl"), Rtl);
				pWriter->WriteAttribute(_T("firstRow"), FirstRow);
				pWriter->WriteAttribute(_T("firstCol"), FirstCol);
				pWriter->WriteAttribute(_T("lastRow"), LastRow);
				pWriter->WriteAttribute(_T("lastCol"), LastCol);
				pWriter->WriteAttribute(_T("bandRow"), BandRow);
				pWriter->WriteAttribute(_T("bandCol"), BandCol);

				pWriter->EndAttributes();

				if (TableStyleId.is_init())
					pWriter->WriteString(_T("<a:tableStyleId>") + *TableStyleId + _T("</a:tableStyleId>"));

				Fill.toXmlWriter(pWriter);
				Effects.toXmlWriter(pWriter);

				pWriter->EndNode(_T("a:tblPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, TableStyleId);

				pWriter->WriteBool2(1, Rtl);
				pWriter->WriteBool2(2, FirstRow);
				pWriter->WriteBool2(3, FirstCol);
				pWriter->WriteBool2(4, LastRow);
				pWriter->WriteBool2(5, LastCol);
				pWriter->WriteBool2(6, BandRow);
				pWriter->WriteBool2(7, BandCol);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Fill);
				pWriter->WriteRecord1(1, Effects);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
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
							TableStyleId = pReader->GetString2();
							break;
						}
						case 1:
						{
							Rtl = pReader->GetBool();
							break;
						}
						case 2:
						{
							FirstRow = pReader->GetBool();
							break;
						}
						case 3:
						{
							FirstCol = pReader->GetBool();
							break;
						}
						case 4:
						{
							LastRow = pReader->GetBool();
							break;
						}
						case 5:
						{
							LastCol = pReader->GetBool();
							break;
						}
						case 6:
						{
							BandRow = pReader->GetBool();
							break;
						}
						case 7:
						{
							BandCol = pReader->GetBool();
							break;
						}
						default:
							break;
					}
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							Fill.fromPPTY(pReader);							
							break;
						}
						case 1:
						{
							Effects.fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}
			
		public:
			UniFill						Fill;
			EffectProperties			Effects;
			nullable_string				TableStyleId;
/*
			<xsd:choice minOccurs="0" maxOccurs="1"> 
			<xsd:element name="tableStyle" type="CT_TableStyle"/> 
			<xsd:element name="tableStyleId" type="s:ST_Guid"/> 
			</xsd:choice> 
*/
			nullable_bool				Rtl;
			nullable_bool				FirstRow;
			nullable_bool				FirstCol;
			nullable_bool				LastRow;
			nullable_bool				LastCol;
			nullable_bool				BandRow;
			nullable_bool				BandCol;

		protected:
			virtual void FillParentPointersForChilds()
			{
				if(Fill.is_init())
					Fill.SetParentPointer(this);
				if(Effects.is_init())
					Effects.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_PROPERTIES_INCLUDE_H_
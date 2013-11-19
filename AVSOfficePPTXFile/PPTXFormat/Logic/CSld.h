#pragma once
#ifndef PPTX_SLIDES_COMMON_SLIDE_DATA_INCLUDE_H_
#define PPTX_SLIDES_COMMON_SLIDE_DATA_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Bg.h"
#include "SpTree.h"

namespace PPTX
{
	namespace Logic
	{
		class CSld : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CSld)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"name", attrName);
				bg			= node.ReadNode(_T("p:bg"));
				spTree		= node.ReadNodeNoNS(_T("spTree"));
				
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), attrName);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(bg);
				oValue.Write(spTree);

				return XmlUtils::CreateNode(_T("p:cSld"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:cSld"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute2(_T("name"), attrName);
				pWriter->EndAttributes();

				pWriter->Write(bg);
				spTree.toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:cSld"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, attrName);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, bg);
				pWriter->WriteRecord1(1, spTree);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						attrName = pReader->GetString2();
					else
						break;
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							bg = new Bg();
							bg->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							spTree.m_name = _T("p:spTree");
							spTree.fromPPTY(pReader);
							break;
						}
						default:
						{
							pReader->Seek(_end_rec);
							return;
						}
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable_string			attrName;

			nullable<Bg>			bg;
			SpTree					spTree;
			//custDataLst
			//controls
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(bg.IsInit())
					bg->SetParentPointer(this);
				spTree.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_COMMON_SLIDE_DATA_INCLUDE_H_
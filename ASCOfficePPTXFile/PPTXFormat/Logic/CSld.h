#pragma once
#ifndef PPTX_SLIDES_COMMON_SLIDE_DATA_INCLUDE_H_
#define PPTX_SLIDES_COMMON_SLIDE_DATA_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Bg.h"
#include "SpTree.h"
#include "Controls.h"

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
				controls	= node.ReadNode(_T("p:controls"));
				
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
				////проблема переноса картинок !!!
				//if (controls.IsInit())
				//{
				//	controls->AddObjectsTo(&spTree.SpTreeElems, pWriter->m_pCommon->m_pImageManager);
				//}

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, attrName);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, bg);
				//придется расписывать полностью SpTree - так как меняет spTree.toPPTY нельязя (много где используется)

				pWriter->StartRecord(1);
					pWriter->StartRecord(SPTREE_TYPE_SPTREE);
						pWriter->WriteRecord1(0, spTree.nvGrpSpPr);
						pWriter->WriteRecord1(1, spTree.grpSpPr);						
						
						pWriter->StartRecord(2);		//pWriter->WriteRecordArray(2, 0, spTree.SpTreeElems);  - вручную
							pWriter->WriteULONG((_UINT32)spTree.SpTreeElems.size() + (controls.IsInit() ? 1 : 0));
							for(int i = 0; i < spTree.SpTreeElems.size();i++)
								pWriter->WriteRecord1(0, spTree.SpTreeElems[i]);
						
							if (controls.IsInit())	controls->toPPTY(pWriter); //можно искуственно добавить controls в sptree .. и все "это" перенести туда
						pWriter->EndRecord();
					pWriter->EndRecord();
				pWriter->EndRecord();
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
			nullable<Controls>		controls;
			//custDataLst
			//controls
		protected:
			virtual void FillParentPointersForChilds()
			{
				spTree.SetParentPointer(this);
				if(bg.IsInit())			bg->SetParentPointer(this);
				if(controls.IsInit())	controls->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_COMMON_SLIDE_DATA_INCLUDE_H_
#pragma once
#ifndef PPTX_LOGIC_SLIDE_BACKGROUND_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_BACKGROUND_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/BWMode.h"
#include "BgPr.h"
#include "StyleRef.h"

namespace PPTX
{
	namespace Logic
	{

		class Bg : public WrapperWritingElement
		{
		public:
			Bg();
			virtual ~Bg();
			explicit Bg(XmlUtils::CXmlNode& node);
			const Bg& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:bg"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("bwMode"), bwMode);
				pWriter->EndAttributes();

				pWriter->Write(bgPr);
				pWriter->Write(bgRef);
				
				pWriter->EndNode(_T("p:bg"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, bwMode);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, bgPr);
				pWriter->WriteRecord2(1, bgRef);
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
					{
						bwMode = new Limit::BWMode();
						bwMode->SetBYTECode(pReader->GetUChar());
					}
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
							bgPr = new BgPr();
							bgPr->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							bgRef = new StyleRef();
							bgRef->m_name = _T("p:bgRef");
							bgRef->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable_limit<Limit::BWMode>	bwMode;
			nullable<BgPr>					bgPr;
			nullable<StyleRef>				bgRef;

			virtual void GetBackground(Logic::BgPr& bg, DWORD& ARGB)const;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_BACKGROUND_INCLUDE_H_
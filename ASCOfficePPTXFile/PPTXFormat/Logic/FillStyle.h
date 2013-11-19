#pragma once
#ifndef PPTX_LOGIC_FILLSTYLE_INCLUDE_H_
#define PPTX_LOGIC_FILLSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "UniFill.h"

namespace PPTX
{
	namespace Logic
	{
		class FillStyle: public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FillStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Fill.GetFillFrom(node);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				return _T("<a:fill>") + Fill.toXML() + _T("</a:fill>");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:fill"));
				pWriter->EndAttributes();

				Fill.toXmlWriter(pWriter);

				pWriter->EndNode(_T("a:fill"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, Fill);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

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
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
			UniFill Fill;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Fill.SetParentPointer(this);
			}
		};

	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FILLSTYLE_INCLUDE_H
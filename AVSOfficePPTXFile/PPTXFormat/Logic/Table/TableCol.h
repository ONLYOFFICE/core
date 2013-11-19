#pragma once
#ifndef PPTX_LOGIC_TABLECOL_INCLUDE_H_
#define PPTX_LOGIC_TABLECOL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class TableCol : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TableCol)

			TableCol& operator=(const TableCol& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Width = oSrc.Width;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Width = node.ReadAttributeInt(L"w");
			}

			virtual CString toXML() const
			{
				CString str = _T("");
				str.Format(_T("<a:gridCol w=\"%d\" />"), Width);
				return str;
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:gridCol"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"w", Width);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:gridCol"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt1(0, Width);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
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

					switch (_at)
					{
						case 0:
						{
							Width = pReader->GetLong();
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}
			
		public:
			int Width;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLECOL_INCLUDE_H_
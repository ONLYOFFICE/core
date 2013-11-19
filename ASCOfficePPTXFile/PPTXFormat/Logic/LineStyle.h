#pragma once
#ifndef PPTX_LOGIC_LINESTYLE_INCLUDE_H_
#define PPTX_LOGIC_LINESTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Ln.h"
#include "StyleRef.h"

namespace PPTX
{
	namespace Logic
	{
		class LineStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(LineStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name	= node.GetName();

				ln		= node.ReadNodeNoNS(_T("ln"));
				lnRef	= node.ReadNodeNoNS(_T("lnRef"));
				
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				if (ln.IsInit())
					oValue.WriteNullable(ln);
				else
					oValue.WriteNullable(lnRef);

				return XmlUtils::CreateNode(m_name, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);
				pWriter->EndAttributes();

				if (ln.IsInit())
					pWriter->Write(ln);
				else
					pWriter->Write(lnRef);

				pWriter->EndNode(m_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, ln);
				pWriter->WriteRecord2(1, lnRef);				
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
							ln = new Ln();
							ln->fromPPTY(pReader);							
							break;
						}
						case 1:
						{
							lnRef = new StyleRef();
							lnRef->fromPPTY(pReader);
							lnRef->m_name = _T("a:lnRef");
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
			nullable<Ln>		ln;
			nullable<StyleRef>	lnRef;
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(ln.IsInit())
					ln->SetParentPointer(this);
				if(lnRef.IsInit())
					lnRef->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LINESTYLE_INCLUDE_H
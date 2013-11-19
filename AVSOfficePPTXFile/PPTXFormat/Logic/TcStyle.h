#pragma once
#ifndef PPTX_LOGIC_TCSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TCSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TcBdr.h"
#include "Cell3D.h"
#include "FillStyle.h"
#include "StyleRef.h"

namespace PPTX
{
	namespace Logic
	{
		class TcStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TcStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				tcBdr	= node.ReadNode(_T("a:tcBdr"));
				cell3D	= node.ReadNode(_T("a:cell3D"));
				fill	= node.ReadNode(_T("a:fill"));
				fillRef = node.ReadNodeNoNS(_T("fillRef"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(tcBdr);
				oValue.WriteNullable(fill);
				oValue.WriteNullable(fillRef);
				oValue.WriteNullable(cell3D);

				return XmlUtils::CreateNode(_T("a:tcStyle"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tcStyle"));
				pWriter->EndAttributes();

				pWriter->Write(tcBdr);
				pWriter->Write(fillRef);
				pWriter->Write(fill);
				pWriter->Write(cell3D);

				pWriter->EndNode(_T("a:tcStyle"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, tcBdr);
				pWriter->WriteRecord2(1, fillRef);
				pWriter->WriteRecord2(2, fill);
				pWriter->WriteRecord2(3, cell3D);
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
							tcBdr = new TcBdr();
							tcBdr->fromPPTY(pReader);							
							break;
						}
						case 1:
						{
							fillRef = new StyleRef();
							fillRef->fromPPTY(pReader);
							fillRef->m_name = _T("a:fillRef");
							break;
						}
						case 2:
						{
							fill = new FillStyle();
							fill->fromPPTY(pReader);
							break;
						}
						case 3:
						{
							cell3D = new Cell3D();
							cell3D->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
			nullable<TcBdr>			tcBdr;
			//boost::shared_ptr<WrapperWritingElement> Fill;
			nullable<StyleRef>		fillRef;
			nullable<FillStyle>		fill;
			nullable<Cell3D>		cell3D;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(tcBdr.IsInit())
					tcBdr->SetParentPointer(this);
				if(fill.IsInit())
					fill->SetParentPointer(this);
				if(fillRef.IsInit())
					fillRef->SetParentPointer(this);
				if(cell3D.IsInit())
					cell3D->SetParentPointer(this);
			}
		};

	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TCSTYLE_INCLUDE_H
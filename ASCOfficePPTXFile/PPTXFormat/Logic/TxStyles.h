#pragma once
#ifndef PPTX_LOGIC_TXSTYLES_INCLUDE_H_
#define PPTX_LOGIC_TXSTYLES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "BodyPr.h"
#include "TextListStyle.h"
#include "Paragraph.h"

namespace PPTX
{
	namespace Logic
	{
		class TxStyles : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TxStyles)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("titleStyle") == strName)
							titleStyle = oNode;
						else if (_T("bodyStyle") == strName)
							bodyStyle = oNode;
						else if (_T("otherStyle") == strName)
							otherStyle = oNode;
					}
				}

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(titleStyle);
				oValue.WriteNullable(bodyStyle);
				oValue.WriteNullable(otherStyle);

				return XmlUtils::CreateNode(_T("p:txStyles"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, titleStyle);
				pWriter->WriteRecord2(1, bodyStyle);
				pWriter->WriteRecord2(2, otherStyle);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG end = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < end)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							titleStyle = new TextListStyle();
							titleStyle->m_name = _T("p:titleStyle");
							titleStyle->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							bodyStyle = new TextListStyle();
							bodyStyle->m_name = _T("p:bodyStyle");
							bodyStyle->fromPPTY(pReader);
							break;
						}
						case 2:
						{
							otherStyle = new TextListStyle();
							otherStyle->m_name = _T("p:otherStyle");
							otherStyle->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(end);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:txStyles"));
				pWriter->EndAttributes();

				pWriter->Write(titleStyle);
				pWriter->Write(bodyStyle);
				pWriter->Write(otherStyle);

				pWriter->EndNode(_T("p:txStyles"));
			}

		public:
			nullable<TextListStyle> titleStyle;
			nullable<TextListStyle> bodyStyle;
			nullable<TextListStyle> otherStyle;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(titleStyle.is_init())
					titleStyle->SetParentPointer(this);
				if(bodyStyle.is_init())
					bodyStyle->SetParentPointer(this);
				if(otherStyle.is_init())
					otherStyle->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TXSTYLES_INCLUDE_H
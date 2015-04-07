#pragma once
#ifndef PPTX_LOGIC_TCBDR_INCLUDE_H_
#define PPTX_LOGIC_TCBDR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "LineStyle.h"

namespace PPTX
{
	namespace Logic
	{
		class TcBdr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TcBdr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int count = oNodes.GetCount();
					
					for (int i = 0; i < count; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("left") == strName)
							left = oNode;
						else if (_T("right") == strName)
							right = oNode;
						else if (_T("top") == strName)
							top = oNode;
						else if (_T("bottom") == strName)
							bottom = oNode;
						else if (_T("insideH") == strName)
							insideH = oNode;
						else if (_T("insideV") == strName)
							insideV = oNode;
						else if (_T("tl2br") == strName)
							tl2br = oNode;
						else if (_T("tr2bl") == strName)
							tr2bl = oNode;
					}
				}

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(left);
				oValue.WriteNullable(right);
				oValue.WriteNullable(top);
				oValue.WriteNullable(bottom);
				oValue.WriteNullable(insideH);
				oValue.WriteNullable(insideV);
				oValue.WriteNullable(tl2br);
				oValue.WriteNullable(tr2bl);
				
				return XmlUtils::CreateNode(_T("a:tcBdr"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tcBdr"));
				pWriter->EndAttributes();

				pWriter->Write(left);
				pWriter->Write(right);
				pWriter->Write(top);
				pWriter->Write(bottom);
				pWriter->Write(insideH);
				pWriter->Write(insideV);
				pWriter->Write(tl2br);
				pWriter->Write(tr2bl);

				pWriter->EndNode(_T("a:tcBdr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, left);
				pWriter->WriteRecord2(1, right);
				pWriter->WriteRecord2(2, top);
				pWriter->WriteRecord2(3, bottom);
				pWriter->WriteRecord2(4, insideH);
				pWriter->WriteRecord2(5, insideV);
				pWriter->WriteRecord2(6, tl2br);
				pWriter->WriteRecord2(7, tr2bl);
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
							left = new LineStyle();
							left->fromPPTY(pReader);
							left->m_name = _T("a:left");
							break;
						}
						case 1:
						{
							right = new LineStyle();
							right->fromPPTY(pReader);
							right->m_name = _T("a:right");
							break;
						}
						case 2:
						{
							top = new LineStyle();
							top->fromPPTY(pReader);
							top->m_name = _T("a:top");
							break;
						}
						case 3:
						{
							bottom = new LineStyle();
							bottom->fromPPTY(pReader);
							bottom->m_name = _T("a:bottom");
							break;
						}
						case 4:
						{
							insideH = new LineStyle();
							insideH->fromPPTY(pReader);
							insideH->m_name = _T("a:insideH");
							break;
						}
						case 5:
						{
							insideV = new LineStyle();
							insideV->fromPPTY(pReader);
							insideV->m_name = _T("a:insideV");
							break;
						}
						case 6:
						{
							tl2br = new LineStyle();
							tl2br->fromPPTY(pReader);
							tl2br->m_name = _T("a:tl2br");
							break;
						}
						case 7:
						{
							tr2bl = new LineStyle();
							tr2bl->fromPPTY(pReader);
							tr2bl->m_name = _T("a:tr2bl");
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
			nullable<LineStyle> left;
			nullable<LineStyle> right;
			nullable<LineStyle> top;
			nullable<LineStyle> bottom;
			nullable<LineStyle> insideH;
			nullable<LineStyle> insideV;
			nullable<LineStyle> tl2br;
			nullable<LineStyle> tr2bl;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(left.IsInit())
					left->SetParentPointer(this);
				if(right.IsInit())
					right->SetParentPointer(this);
				if(top.IsInit())
					top->SetParentPointer(this);
				if(bottom.IsInit())
					bottom->SetParentPointer(this);
				if(insideH.IsInit())
					insideH->SetParentPointer(this);
				if(insideV.IsInit())
					insideV->SetParentPointer(this);
				if(tl2br.IsInit())
					tl2br->SetParentPointer(this);
				if(tr2bl.IsInit())
					tr2bl->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TCBDR_INCLUDE_H
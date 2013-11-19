#pragma once
#ifndef PPTX_LOGIC_DEFAULTSHAPEDEFINITION_INCLUDE_H_
#define PPTX_LOGIC_DEFAULTSHAPEDEFINITION_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "SpPr.h"
#include "BodyPr.h"
#include "TextListStyle.h"
#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{
		class DefaultShapeDefinition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(DefaultShapeDefinition)

			DefaultShapeDefinition& operator=(const DefaultShapeDefinition& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				spPr		= oSrc.spPr;
				bodyPr		= oSrc.bodyPr;
				lstStyle	= oSrc.lstStyle;
				style		= oSrc.style;
				m_name		= oSrc.m_name;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name		= XmlUtils::GetNameNoNS(node.GetName());

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int count = oNodes.GetCount();
					for (int i = 0; i < count; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("spPr") == strName)
							spPr = oNode;
						else if (_T("bodyPr") == strName)
							bodyPr = oNode;
						else if (_T("lstStyle") == strName)
							lstStyle = oNode;
						else if (_T("style") == strName)
							style = oNode;
					}
				}

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(spPr);
				oValue.Write(bodyPr);
				oValue.Write(lstStyle);
				oValue.WriteNullable(style);

				return XmlUtils::CreateNode(_T("a:") + m_name, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, spPr);
				pWriter->WriteRecord1(1, bodyPr);
				pWriter->WriteRecord1(2, lstStyle);
				pWriter->WriteRecord2(3, style);
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
							spPr.m_namespace = _T("a");
							spPr.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							bodyPr.m_namespace = _T("a");
							bodyPr.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							lstStyle.m_name = _T("a:lstStyle");
							lstStyle.fromPPTY(pReader);
							break;
						}
						case 3:
						{
							style = new ShapeStyle();
							style->m_ns = _T("a");
							style->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:") + m_name);
				pWriter->EndAttributes();

				spPr.toXmlWriter(pWriter);
				bodyPr.toXmlWriter(pWriter);
				lstStyle.toXmlWriter(pWriter);
				pWriter->Write(style);

				pWriter->EndNode(_T("a:") + m_name);
			}

		public:
			SpPr					spPr;
			BodyPr					bodyPr;
			TextListStyle			lstStyle;
			nullable<ShapeStyle>	style;
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				spPr.SetParentPointer(this);
				bodyPr.SetParentPointer(this);
				lstStyle.SetParentPointer(this);
				if(style.IsInit())
					style->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_DEFAULTSHAPEDEFINITION_INCLUDE_H
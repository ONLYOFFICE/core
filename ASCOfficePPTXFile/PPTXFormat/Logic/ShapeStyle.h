#pragma once
#ifndef PPTX_LOGIC_SHAPESTYLE_INCLUDE_H_
#define PPTX_LOGIC_SHAPESTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "StyleRef.h"
#include "FontRef.h"

namespace PPTX
{
	namespace Logic
	{
		class ShapeStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ShapeStyle)

			ShapeStyle& operator=(const ShapeStyle& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				lnRef		= oSrc.lnRef;
				fillRef		= oSrc.fillRef;
				effectRef	= oSrc.effectRef;
				fontRef		= oSrc.fontRef;

				m_ns		= oSrc.m_ns;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_ns = XmlUtils::GetNamespace(node.GetName());

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("lnRef") == strName)
							lnRef = oNode;
						else if (_T("fillRef") == strName)
							fillRef = oNode;
						else if (_T("effectRef") == strName)
							effectRef = oNode;
						else if (_T("fontRef") == strName)
							fontRef = oNode;
					}
				}
				
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("xmlns:") + PPTX::g_Namespaces.p.m_strName, PPTX::g_Namespaces.p.m_strLink);
				oAttr.Write(_T("xmlns:") + PPTX::g_Namespaces.a.m_strName, PPTX::g_Namespaces.a.m_strLink);

				XmlUtils::CNodeValue oValue;
				oValue.Write(lnRef);
				oValue.Write(fillRef);
				oValue.Write(effectRef);
				oValue.Write(fontRef);

				return XmlUtils::CreateNode(m_ns + _T(":style"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_ns + _T(":style"));
				pWriter->EndAttributes();

				lnRef.toXmlWriter(pWriter);
				fillRef.toXmlWriter(pWriter);
				effectRef.toXmlWriter(pWriter);
				fontRef.toXmlWriter(pWriter);
				
				pWriter->EndNode(m_ns + _T(":style"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, lnRef);
				pWriter->WriteRecord1(1, fillRef);
				pWriter->WriteRecord1(2, effectRef);
				pWriter->WriteRecord1(3, fontRef);
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
							lnRef.m_name = _T("a:lnRef");
							lnRef.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							fillRef.m_name = _T("a:fillRef");
							fillRef.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							effectRef.m_name = _T("a:effectRef");
							effectRef.fromPPTY(pReader);
							break;
						}
						case 3:
						{
							fontRef.m_name = _T("a:fontRef");
							fontRef.fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			StyleRef	lnRef;
			StyleRef	fillRef;
			StyleRef	effectRef;
			FontRef		fontRef;

			mutable CString m_ns;
		protected:
			virtual void FillParentPointersForChilds()
			{
				lnRef.SetParentPointer(this);
				fillRef.SetParentPointer(this);
				effectRef.SetParentPointer(this);
				fontRef.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SHAPESTYLE_INCLUDE_H
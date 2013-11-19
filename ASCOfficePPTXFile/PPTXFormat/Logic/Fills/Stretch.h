#pragma once
#ifndef PPTX_LOGIC_STRETCH_INCLUDE_H_
#define PPTX_LOGIC_STRETCH_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../Rect.h"

namespace PPTX
{
	namespace Logic
	{

		class Stretch : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Stretch)

			Stretch& operator=(const Stretch& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				fillRect = oSrc.fillRect;
				return *this;
			}

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

						if (_T("fillRect") == XmlUtils::GetNameNoNS(oNode.GetName()))
							fillRect = oNode;
					}
				}
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(fillRect);

				return XmlUtils::CreateNode(_T("a:stretch"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:stretch"));
				pWriter->EndAttributes();

				pWriter->Write(fillRect);

				pWriter->EndNode(_T("a:stretch"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, fillRect);				
			}

		public:
			nullable<Rect> fillRect;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(fillRect.IsInit())
					fillRect->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_STRETCH_INCLUDE_H_
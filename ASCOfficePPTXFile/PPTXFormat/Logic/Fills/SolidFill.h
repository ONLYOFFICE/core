#pragma once
#ifndef PPTX_LOGIC_SOLIDFILL_INCLUDE_H_
#define PPTX_LOGIC_SOLIDFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class SolidFill : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SolidFill)

			SolidFill& operator=(const SolidFill& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Color		= oSrc.Color;
				m_namespace	= oSrc.m_namespace;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());
				Color.GetColorFrom(node);
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				CString strName = (_T("") == m_namespace) ? _T("solidFill") : (m_namespace + _T(":solidFill"));

				XmlUtils::CNodeValue oValue;
				oValue.Write(Color);

				return XmlUtils::CreateNode(strName, oValue);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString strName = (_T("") == m_namespace) ? _T("solidFill") : (m_namespace + _T(":solidFill"));
				pWriter->StartNode(strName);
				pWriter->EndAttributes();
				
				if (Color.is_init())
					Color.toXmlWriter(pWriter);

				pWriter->EndNode(strName);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(FILL_TYPE_SOLID);

				pWriter->WriteRecord1(0, Color);

				pWriter->EndRecord();
			}

			void Merge(SolidFill& fill)const
			{
				if(Color.is_init())
					fill.Color = Color;
			}
		public:
			UniColor Color;

			CString m_namespace;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SOLIDFILL_INCLUDE_H_
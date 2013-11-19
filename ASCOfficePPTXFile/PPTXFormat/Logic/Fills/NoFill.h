#pragma once
#ifndef PPTX_LOGIC_NOFILL_INCLUDE_H_
#define PPTX_LOGIC_NOFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class NoFill : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NoFill)

			NoFill& operator=(const NoFill& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				m_namespace = oSrc.m_namespace;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());
			}
			virtual CString toXML() const
			{
				if (_T("") == m_namespace)
					return _T("<noFill/>");
				return _T("<") + m_namespace + _T(":noFill/>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<a:noFill/>"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(FILL_TYPE_NOFILL);
				pWriter->EndRecord();
			}
		public:
			CString m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NOFILL_INCLUDE_H_
#pragma once
#ifndef PPTX_LOGIC_TXEL_INCLUDE_H_
#define PPTX_LOGIC_TXEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class TxEl : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TxEl)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("p:charRg"), oNode))
				{
					charRg = true;
					oNode.ReadAttributeBase(L"st", st);
					oNode.ReadAttributeBase(L"end", end);
				}
				else if(node.GetNode(_T("p:pRg"), oNode))
				{
					charRg = false;
					oNode.ReadAttributeBase(L"st", st);
					oNode.ReadAttributeBase(L"end", end);
				}
				else
				{
					charRg.reset();
					st.reset();
					end.reset();
				}
			}

			virtual CString toXML() const
			{
				if (charRg.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("st"), st);
					oAttr.Write(_T("end"), end);

					CString strName = _T("p:pRg");
					if (*charRg)
						strName = _T("p:charRg");	

					return XmlUtils::CreateNode(_T("p:txEl"), XmlUtils::CreateNode(strName, oAttr));
				}
				return _T("<p:txEl/>");
			}
		public:
			nullable_sizet	st;
			nullable_sizet	end;
			nullable_bool	charRg;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TXEL_INCLUDE_H_
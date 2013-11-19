#pragma once
#ifndef PPTX_LOGIC_TGTEL_INCLUDE_H_
#define PPTX_LOGIC_TGTEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "../../DocxFormat/RId.h"
#include "SpTgt.h"

namespace PPTX
{
	namespace Logic
	{
		class TgtEl : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TgtEl)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("p:inkTgt"), oNode))
					inkTgt = oNode.ReadAttributeBase(L"spid");
				else if(node.GetNode(_T("p:sndTgt"), oNode))
				{
					oNode.ReadAttributeBase(L"embed", embed);
					oNode.ReadAttributeBase(L"name", name);
					oNode.ReadAttributeBase(L"builtIn", builtIn);
				}
				else if(node.GetNode(_T("p:spTgt"), oNode))
					spTgt = oNode;

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				if (inkTgt.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("spid"), inkTgt);

					return XmlUtils::CreateNode(_T("p:tgtEl"), XmlUtils::CreateNode(_T("p:inkTgt"), oAttr));
				}
				if (embed.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("embed"), embed->ToString());
					oAttr.Write(_T("name"), name);
					oAttr.Write(_T("builtIn"), builtIn);

					return XmlUtils::CreateNode(_T("p:tgtEl"), XmlUtils::CreateNode(_T("p:sndTgt"), oAttr));
				}
				if (spTgt.IsInit())
				{
					return XmlUtils::CreateNode(_T("p:tgtEl"), spTgt->toXML());
				}
				return _T("<p:tgtEl><p:sldTgt></p:tgtEl>");
			}
		public:
			nullable_string		inkTgt;

			//sndTgt
			nullable_string		name;
			nullable<OOX::RId>	embed;
			nullable_bool		builtIn;

			nullable<SpTgt>		spTgt;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(spTgt.IsInit())
					spTgt->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TGTEL_INCLUDE_H_
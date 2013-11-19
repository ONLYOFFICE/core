#pragma once
#ifndef PPTX_SLIDES_STSND_INCLUDE_H_
#define PPTX_SLIDES_STSND_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "../../DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{

		class StSnd : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(StSnd)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"loop", loop);

				XmlUtils::CXmlNode oNode;
				node.GetNode(_T("p:snd"), oNode);

				oNode.ReadAttributeBase(L"r:embed", embed);
				oNode.ReadAttributeBase(L"name", name);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("loop"), loop);

				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("r:embed"), embed.ToString());
				oAttr2.Write(_T("name"), name);

				return XmlUtils::CreateNode(_T("p:stSnd"), oAttr1, XmlUtils::CreateNode(_T("p:snd"), oAttr2));
			}

		public:
			OOX::RId			embed;
			nullable_string		name;
			nullable_bool		loop;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_STSND_INCLUDE_H_
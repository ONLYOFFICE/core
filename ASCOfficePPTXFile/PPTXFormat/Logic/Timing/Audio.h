#pragma once
#ifndef PPTX_LOGIC_AUDIO_INCLUDE_H_
#define PPTX_LOGIC_AUDIO_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CMediaNode.h"

namespace PPTX
{
	namespace Logic
	{
		class Audio : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Audio)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"isNarration", isNarration);
				cMediaNode	= node.ReadNode(_T("p:cMediaNode"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("isNarration"), isNarration);

				return XmlUtils::CreateNode(_T("p:audio"), oAttr, cMediaNode.toXML());
			}

		public:
			CMediaNode		cMediaNode;
			nullable_bool	isNarration;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cMediaNode.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AUDIO_INCLUDE_H
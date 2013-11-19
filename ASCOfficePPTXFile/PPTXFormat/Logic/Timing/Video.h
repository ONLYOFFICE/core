#pragma once
#ifndef PPTX_LOGIC_VIDEO_INCLUDE_H_
#define PPTX_LOGIC_VIDEO_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CMediaNode.h"

namespace PPTX
{
	namespace Logic
	{
		class Video : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Video)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"fullScrn", fullScrn);
				cMediaNode	= node.ReadNode(_T("cMediaNode"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("fullScrn"), fullScrn);

				return XmlUtils::CreateNode(_T("p:video"), oAttr, cMediaNode.toXML());
			}

		public:
			CMediaNode		cMediaNode;
			nullable_bool	fullScrn;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cMediaNode.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_VIDEO_INCLUDE_H
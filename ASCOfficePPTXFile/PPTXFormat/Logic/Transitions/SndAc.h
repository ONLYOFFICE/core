#pragma once
#ifndef PPTX_SLIDES_SNDAC_INCLUDE_H_
#define PPTX_SLIDES_SNDAC_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "StSnd.h"

namespace PPTX
{
	namespace Logic
	{

		class SndAc : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SndAc)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				stSnd = node.ReadNode(_T("p:stSnd"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				if(stSnd.IsInit())
				{
					return XmlUtils::CreateNode(_T("p:sndAc"), stSnd->toXML());
				}
				return _T("<p:sndAc><p:endSnd/></p:sndAc>");
			}

		public:
			nullable<StSnd> stSnd;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(stSnd.IsInit())
					stSnd->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_SNDAC_INCLUDE_H_
#pragma once
#ifndef PPTX_LOGIC_AUDIOCD_INCLUDE_H_
#define PPTX_LOGIC_AUDIOCD_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class AudioCD : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AudioCD)

			AudioCD& operator=(const AudioCD& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				stTrack		= oSrc.stTrack;
				endTrack	= oSrc.endTrack;

				stTime		= oSrc.stTime;
				endTime		= oSrc.endTime;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oMem;

				oMem		= node.ReadNode(_T("a:st"));
				stTrack		= oMem.ReadAttributeInt(L"track");
				oMem.ReadAttributeBase(L"time", stTime);

				oMem		= node.ReadNode(_T("a:end"));
				endTrack	= oMem.ReadAttributeInt(L"track");
				oMem.ReadAttributeBase(L"time", endTime);

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("track"), stTrack);
				oAttr1.Write(_T("time"), stTime);
				
				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("track"), endTrack);
				oAttr2.Write(_T("time"), endTime);

				return _T("<a:audioCd>") + XmlUtils::CreateNode(_T("a:st"), oAttr1) + XmlUtils::CreateNode(_T("a:end"), oAttr2) + _T("</a:audioCd>");
			}
		public:
			int stTrack;
			int endTrack;

			nullable_int	stTime;
			nullable_int	endTime;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
                stTrack = (std::max)(0, (std::min)(255, stTrack));
                stTrack = (std::max)(0, (std::min)(255, endTrack));

				stTime.normalize_positive();
				endTime.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AUDIOCD_INCLUDE_H_

#pragma once
#ifndef PPTX_LOGIC_UNIMEDIA_INCLUDE_H_
#define PPTX_LOGIC_UNIMEDIA_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Media/AudioCD.h"
#include "Media/WavAudioFile.h"
#include "Media/MediaFile.h"

namespace PPTX
{
	namespace Logic
	{
		class UniMedia : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(UniMedia)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString name = XmlUtils::GetNameNoNS(node.GetName());

				if (name == _T("audioCd"))
					Media.reset(new Logic::AudioCD(node));
				else if (name == _T("wavAudioFile"))
					Media.reset(new Logic::WavAudioFile(node));
				else if (name == _T("audioFile"))
					Media.reset(new Logic::MediaFile(node));
				else if (name == _T("videoFile"))
					Media.reset(new Logic::MediaFile(node));
				else if (name == _T("quickTimeFile"))
					Media.reset(new Logic::MediaFile(node));
				else Media.reset();
			}

			virtual void GetMediaFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				if (element.GetNode(_T("a:audioCd"), oNode))
					Media.reset(new Logic::AudioCD(oNode));
				else if (element.GetNode(_T("a:wavAudioFile"), oNode))
					Media.reset(new Logic::WavAudioFile(oNode));
				else if (element.GetNode(_T("a:audioFile"), oNode))
					Media.reset(new Logic::MediaFile(oNode));
				else if (element.GetNode(_T("a:videoFile"), oNode))
					Media.reset(new Logic::MediaFile(oNode));
				else if (element.GetNode(_T("a:quickTimeFile"), oNode))
					Media.reset(new Logic::MediaFile(oNode));
				else Media.reset();
			}

			virtual CString toXML() const
			{
				if (Media.IsInit())
					return Media->toXML();
				return _T("");
			}

			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				// TODO:
			}

			virtual void toPPTY(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				// TODO:
				if (Media.is_init())
					Media->toXmlWriter(pWriter);
			}

			virtual bool is_init()const{return (Media.IsInit());};
			
			template<class T> AVSINLINE const bool	is() const	{ return Media.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return Media.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return Media.as<T>(); }
		//public:
		private:
			smart_ptr<WrapperWritingElement> Media;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					Media->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIMEDIA_INCLUDE_H
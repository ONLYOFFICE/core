#include "UniMedia.h"
#include "Media/AudioCD.h"
#include "Media/WavAudioFile.h"
#include "Media/MediaFile.h"

namespace PPTX
{
	namespace Logic
	{

		UniMedia::UniMedia()
		{
		}

		UniMedia::~UniMedia()
		{
			//Media.reset();
		}

		UniMedia::UniMedia(const XML::XNode& node)
		{
			fromXML(node);
		}

		const UniMedia& UniMedia::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void UniMedia::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if (name == "audioCd")
				Media.reset(new Logic::AudioCD(node));
			else if (name == "wavAudioFile")
				Media.reset(new Logic::WavAudioFile(node));
			else if (name == "audioFile")
				Media.reset(new Logic::MediaFile(node));
			else if (name == "videoFile")
				Media.reset(new Logic::MediaFile(node));
			else if (name == "quickTimeFile")
				Media.reset(new Logic::MediaFile(node));
			else Media.reset();
		}

		void UniMedia::GetMediaFrom(const XML::XElement& element){
			if(element.element("audioCd").exist())
				Media.reset(new Logic::AudioCD(element.element("audioCd")));
			else if(element.element("wavAudioFile").exist())
				Media.reset(new Logic::WavAudioFile(element.element("wavAudioFile")));
			else if(element.element("audioFile").exist())
				Media.reset(new Logic::MediaFile(element.element("audioFile")));
			else if(element.element("videoFile").exist())
				Media.reset(new Logic::MediaFile(element.element("videoFile")));
			else if(element.element("quickTimeFile").exist())
				Media.reset(new Logic::MediaFile(element.element("quickTimeFile")));
			else Media.reset();
		}

		const XML::XNode UniMedia::toXML() const
		{
			return XML::Write(Media);
		}
	} // namespace Logic
} // namespace PPTX
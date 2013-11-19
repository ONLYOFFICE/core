#include "SndAc.h"

namespace PPTX
{
	namespace Logic
	{

		SndAc::SndAc()
		{
		}

		SndAc::~SndAc()
		{
		}

		SndAc::SndAc(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SndAc& SndAc::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SndAc::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			stSnd = element.element("stSnd");
			FillParentPointersForChilds();
		}

		const XML::XNode SndAc::toXML() const
		{
			if(stSnd.is_init())
				return XML::XElement(ns.p + "sndAc", XML::Write(stSnd));
			return XML::XElement(ns.p + "sndAc", XML::XElement(ns.p + "endSnd"));
		}

		void SndAc::FillParentPointersForChilds()
		{
			if(stSnd.is_init())
				stSnd->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
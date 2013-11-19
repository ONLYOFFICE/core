#include "Timing.h"

namespace PPTX
{
	namespace Logic
	{

		Timing::Timing()
		{
		}


		Timing::~Timing()
		{
		}
	

		Timing::Timing(const XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const Timing& Timing::operator =(const XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Timing::fromXML(const XmlUtils::CXmlNode& node)
		{
//			const XML::XElement element(node);
		}


		CString Timing::toXML() const
		{
			return XML::XElement(ns.p + "timing");
		}

	} // namespace Logic
} // namespace PPTX
#include "Grayscl.h"

namespace PPTX
{
	namespace Logic
	{

		Grayscl::Grayscl()
		{
		}

		Grayscl::~Grayscl()
		{
		}

		Grayscl::Grayscl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Grayscl& Grayscl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Grayscl::fromXML(const XML::XNode& node)
		{
		}

		const XML::XNode Grayscl::toXML() const
		{
			return XML::XElement(ns.a + "grayscl");
		}
	} // namespace Logic
} // namespace PPTX
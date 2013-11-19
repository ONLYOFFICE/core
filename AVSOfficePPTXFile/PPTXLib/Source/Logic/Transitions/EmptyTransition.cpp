#include "EmptyTransition.h"

namespace PPTX
{
	namespace Logic
	{

		EmptyTransition::EmptyTransition()
		{
		}

		EmptyTransition::~EmptyTransition()
		{
		}

		EmptyTransition::EmptyTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const EmptyTransition& EmptyTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EmptyTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.XName->Name;
		}

		const XML::XNode EmptyTransition::toXML() const
		{
			return XML::XElement(ns.p + name.get());
		}

	} // namespace Logic
} // namespace PPTX
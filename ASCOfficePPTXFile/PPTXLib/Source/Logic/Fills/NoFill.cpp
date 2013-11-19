#include "NoFill.h"

namespace PPTX
{
	namespace Logic
	{

		NoFill::NoFill()
		{
		}


		NoFill::~NoFill()
		{
		}


		NoFill::NoFill(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NoFill& NoFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NoFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_namespace = element.XName.get().Ns;
		}


		const XML::XNode NoFill::toXML() const
		{
			 return XML::XElement(m_namespace + "noFill");
		}

	} // namespace Logic
} // namespace PPTX
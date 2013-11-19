#include "SpTreeElem.h"
#include "Shape.h"
#include "Pic.h"
#include "CxnSp.h"
#include "SpTree.h"
#include "GraphicFrame.h"

namespace PPTX
{
	namespace Logic
	{

		SpTreeElem::SpTreeElem()
		{
		}


		SpTreeElem::~SpTreeElem()
		{
		}
	

		SpTreeElem::SpTreeElem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SpTreeElem& SpTreeElem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SpTreeElem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if (name == "sp")
				m_elem.reset(new Logic::Shape(node));
			else if (name == "pic")
				m_elem.reset(new Logic::Pic(node));
			else if (name == "cxnSp")
				m_elem.reset(new Logic::CxnSp(node));
			else if(name == "grpSp")
				m_elem.reset(new Logic::SpTree(node));
			else if(name == "graphicFrame")
				m_elem.reset(new Logic::GraphicFrame(node));
			else m_elem.reset();
		}


		const XML::XNode SpTreeElem::toXML() const
		{
			return XML::Write(m_elem);
		}

	} // namespace Logic
} // namespace PPTX
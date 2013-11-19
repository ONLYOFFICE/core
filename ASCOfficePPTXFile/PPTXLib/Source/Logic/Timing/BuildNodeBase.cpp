#include "BuildNodeBase.h"
#include "BldP.h"
#include "BldDgm.h"
#include "BldGraphic.h"
#include "BldOleChart.h"

namespace PPTX
{
	namespace Logic
	{

		BuildNodeBase::BuildNodeBase()
		{
		}

		BuildNodeBase::~BuildNodeBase()
		{
		}

		BuildNodeBase::BuildNodeBase(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BuildNodeBase& BuildNodeBase::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BuildNodeBase::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if(name == "bldP")
				m_node.reset(new Logic::BldP(node));
			else if(name == "bldDgm")
				m_node.reset(new Logic::BldDgm(node));
			else if(name == "bldGraphic")
				m_node.reset(new Logic::BldGraphic(node));
			else if(name == "bldOleChart")
				m_node.reset(new Logic::BldOleChart(node));
			else m_node.reset();
		}

		void BuildNodeBase::GetBuildNodeFrom(const XML::XElement& element)
		{
			if(element.element("bldP").exist())
				m_node.reset(new Logic::BldP(element.element("bldP")));
			else if(element.element("bldDgm").exist())
				m_node.reset(new Logic::BldDgm(element.element("bldDgm")));
			else if(element.element("bldGraphic").exist())
				m_node.reset(new Logic::BldGraphic(element.element("bldGraphic")));
			else if(element.element("bldOleChart").exist())
				m_node.reset(new Logic::BldOleChart(element.element("bldOleChart")));
			else m_node.reset();
		}

		const XML::XNode BuildNodeBase::toXML() const
		{
			return XML::Write(m_node);
		}

	} // namespace Logic
} // namespace PPTX
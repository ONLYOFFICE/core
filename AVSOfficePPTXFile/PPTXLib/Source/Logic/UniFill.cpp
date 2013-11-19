#include "UniFill.h"
#include "Fills/BlipFill.h"
#include "Fills/NoFill.h"
#include "Fills/SolidFill.h"
#include "Fills/GradFill.h"
#include "Fills/PattFill.h"

namespace PPTX
{
	namespace Logic
	{
		UniFill::UniFill()
		{
		}

		UniFill::~UniFill()
		{
			//Fill.reset();
		}

		UniFill::UniFill(const XML::XNode& node)
		{
			fromXML(node);
		}

		const UniFill& UniFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void UniFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if (name == "blipFill")
			{
				m_type = blipFill;
				Fill.reset(new Logic::BlipFill(node));
			}
			else if(name == "noFill")
			{
				m_type = noFill;
				Fill.reset(new Logic::NoFill(node));
			}
			else if(name == "solidFill")
			{
				m_type = solidFill;
				Fill.reset(new Logic::SolidFill(node));
			}
			else if(name == "gradFill")
			{
				m_type = gradFill;
				Fill.reset(new Logic::GradFill(node));
			}
			else if(name == "pattFill")
			{
				m_type = pattFill;
				Fill.reset(new Logic::PattFill(node));
			}
			else
			{
				m_type = notInit;
				Fill.reset();
			}
		}

		void UniFill::GetFillFrom(const XML::XElement& element){
			if(element.element("blipFill").exist())
			{
				m_type = blipFill;
				Fill.reset(new Logic::BlipFill(element.element("blipFill")));
			}
			else if(element.element("noFill").exist())
			{
				m_type = noFill;
				Fill.reset(new Logic::NoFill(element.element("noFill")));
			}
			else if(element.element("solidFill").exist())
			{
				m_type = solidFill;
				Fill.reset(new Logic::SolidFill(element.element("solidFill")));
			}
			else if(element.element("gradFill").exist())
			{
				m_type = gradFill;
				Fill.reset(new Logic::GradFill(element.element("gradFill")));
			}
			else if(element.element("pattFill").exist())
			{
				m_type = pattFill;
				Fill.reset(new Logic::PattFill(element.element("pattFill")));
			}
			else
			{
				m_type = notInit;
				Fill.reset();
			}
		}

		const XML::XNode UniFill::toXML() const
		{
			return XML::Write(Fill);
		}

		const UniFill& UniFill::operator+=(const UniFill& fill)
		{
			if(!fill.is_init())
				return (*this);
			*this = fill;
			return (*this);
		}

		void UniFill::Merge(UniFill& fill)const
		{
			if(!fill.is_init())
			{
				fill = *this;
				return;
			}

			if(*m_type != *fill.m_type)
			{
				fill = *this;
				return;
			}
			if(*m_type == solidFill)
			{
				as<SolidFill>().Merge(fill.as<SolidFill>());
				return;
			}
			if(*m_type == gradFill)
			{
				as<GradFill>().Merge(fill.as<GradFill>());
				return;
			}
		}

		void UniFill::FillParentPointersForChilds()
		{
			if(is_init())
				Fill->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX
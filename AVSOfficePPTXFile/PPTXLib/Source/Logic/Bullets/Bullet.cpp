#include "Bullet.h"
#include "BuNone.h"
#include "BuChar.h"
#include "BuAutoNum.h"
#include "BuBlip.h"

namespace PPTX
{
	namespace Logic
	{

		Bullet::Bullet()
		{
		}


		Bullet::~Bullet()
		{
		}
	

		Bullet::Bullet(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Bullet& Bullet::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Bullet::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element->XName == "buNone")
				m_Bullet.reset(new Logic::BuNone(node));
			else if (element->XName == "buChar")
				m_Bullet.reset(new Logic::BuChar(node));
			else if (element->XName == "buAutoNum")
				m_Bullet.reset(new Logic::BuAutoNum(node));
			else if (element->XName == "buBlip")
				m_Bullet.reset(new Logic::BuBlip(node));
			else m_Bullet.reset();
		}

		void Bullet::ReadBulletFrom(const XML::XElement& element){
			if(element.element("buNone").exist())
				m_Bullet.reset(new Logic::BuNone(element.element("buNone")));
			else if(element.element("buChar").exist())
				m_Bullet.reset(new Logic::BuChar(element.element("buChar")));
			else if(element.element("buAutoNum").exist())
				m_Bullet.reset(new Logic::BuAutoNum(element.element("buAutoNum")));
			else if(element.element("buBlip").exist())
				m_Bullet.reset(new Logic::BuBlip(element.element("buBlip")));
			else m_Bullet.reset();
		}

		const XML::XNode Bullet::toXML()const
		{
			return XML::Write(m_Bullet);
		}

	} // namespace Logic
} // namespace PPTX
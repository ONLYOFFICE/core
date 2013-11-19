#include "BulletSize.h"
#include "BuSzTx.h"
#include "BuSzPct.h"
#include "BuSzPts.h"

namespace PPTX
{
	namespace Logic
	{

		BulletSize::BulletSize()
		{
		}


		BulletSize::~BulletSize()
		{
		}
	

		BulletSize::BulletSize(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BulletSize& BulletSize::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BulletSize::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element->XName == "buSzTx")
				m_Size.reset(new Logic::BuSzTx(node));
			else if (element->XName == "buSzPct")
				m_Size.reset(new Logic::BuSzPct(node));
			else if (element->XName == "buSzPts")
				m_Size.reset(new Logic::BuSzPts(node));
			else m_Size.reset();
		}

		void BulletSize::ReadBulletSizeFrom(const XML::XElement& element){
			if(element.element("buSzTx").exist())
				m_Size.reset(new Logic::BuSzTx(element.element("buSzTx")));
			else if(element.element("buSzPct").exist())
				m_Size.reset(new Logic::BuSzPct(element.element("buSzPct")));
			else if(element.element("buSzPts").exist())
				m_Size.reset(new Logic::BuSzPts(element.element("buSzPts")));
			else m_Size.reset();
		}

		const XML::XNode BulletSize::toXML()const
		{
			return XML::Write(m_Size);
		}

	} // namespace Logic
} // namespace PPTX
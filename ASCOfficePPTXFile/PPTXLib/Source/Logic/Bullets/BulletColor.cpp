#include "BulletColor.h"
#include "BuClrTx.h"
#include "BuClr.h"

namespace PPTX
{
	namespace Logic
	{

		BulletColor::BulletColor()
		{
		}


		BulletColor::~BulletColor()
		{
		}
	

		BulletColor::BulletColor(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BulletColor& BulletColor::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BulletColor::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element->XName == "buClrTx")
				m_Color.reset(new Logic::BuClrTx(node));
			else if (element->XName == "buClr")
				m_Color.reset(new Logic::BuClr(node));
			else m_Color.reset();
		}

		void BulletColor::ReadBulletColorFrom(const XML::XElement& element){
			if(element.element("buClrTx").exist())
				m_Color.reset(new Logic::BuClrTx(element.element("buClrTx")));
			else if(element.element("buClr").exist())
				m_Color.reset(new Logic::BuClr(element.element("buClr")));
			else m_Color.reset();
		}

		const XML::XNode BulletColor::toXML()const
		{
			return XML::Write(m_Color);
		}

		DWORD BulletColor::GetRGBA()const
		{
			if(has_spec_color())
				return as<BuClr>().GetRGBA();
			return 0;
		}

		DWORD BulletColor::GetARGB()const
		{
			if(has_spec_color())
				return as<BuClr>().GetARGB();
			return 0;
		}

		DWORD BulletColor::GetBGRA()const
		{
			if(has_spec_color())
				return as<BuClr>().GetBGRA();
			return 0;
		}

		DWORD BulletColor::GetABGR()const
		{
			if(has_spec_color())
				return as<BuClr>().GetABGR();
			return 0;
		}

	} // namespace Logic
} // namespace PPTX
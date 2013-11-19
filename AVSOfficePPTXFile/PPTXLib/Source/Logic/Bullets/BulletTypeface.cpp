#include "BulletTypeface.h"
#include "./../TextFont.h"
#include "BuFontTx.h"

namespace PPTX
{
	namespace Logic
	{

		BulletTypeface::BulletTypeface()
		{
		}


		BulletTypeface::~BulletTypeface()
		{
		}
	

		BulletTypeface::BulletTypeface(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BulletTypeface& BulletTypeface::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BulletTypeface::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element->XName == "buFontTx")
				m_Typeface.reset(new Logic::BuFontTx(node));
			else if (element->XName == "buFont")
				m_Typeface.reset(new Logic::TextFont(node));
			else m_Typeface.reset();
		}

		void BulletTypeface::ReadBulletTypefaceFrom(const XML::XElement& element){
			if(element.element("buFontTx").exist())
				m_Typeface.reset(new Logic::BuFontTx(element.element("buFontTx")));
			else if(element.element("buFont").exist())
				m_Typeface.reset(new Logic::TextFont(element.element("buFont")));
			else m_Typeface.reset();
		}

		const XML::XNode BulletTypeface::toXML()const
		{
			return XML::Write(m_Typeface);
		}

	} // namespace Logic
} // namespace PPTX
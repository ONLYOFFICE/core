#include "TableBgStyle.h"

namespace PPTX
{
	namespace Logic
	{

		TableBgStyle::TableBgStyle()
		{
		}


		TableBgStyle::~TableBgStyle()
		{
		}
	

		TableBgStyle::TableBgStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableBgStyle& TableBgStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableBgStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			fill = element.element("fill");
			fillRef = element.element("fillRef");

			FillParentPointersForChilds();
		}


		const XML::XNode TableBgStyle::toXML() const
		{
			return XML::XElement(ns.a + "tblBg",
				XML::Write(fill) +
				XML::Write(fillRef)
				);
		}

		void TableBgStyle::FillParentPointersForChilds()
		{
			if(fill.is_init())
				fill->SetParentPointer(*this);
			if(fillRef.is_init())
				fillRef->SetParentPointer(*this);
		}

		const UniFill TableBgStyle::GetFillStyle(UniColor& Color)const
		{
			UniFill result;
			result.SetParentFilePointer(*parentFile);
			UniColor resColor;
			resColor.SetParentFilePointer(*parentFile);
			Color = resColor;

			if(fill.is_init())
				return fill->Fill;
			if(fillRef.is_init())
			{
				m_Theme->GetFillStyle(fillRef->idx.get_value_or(0), result);
				Color = fillRef->Color;
				return result;
			}
			return result;
		}

		void TableBgStyle::SetTheme(const boost::shared_ptr<PPTX::Theme> theme)
		{
			m_Theme = theme;
		}

	} // namespace Logic
} // namespace PPTX
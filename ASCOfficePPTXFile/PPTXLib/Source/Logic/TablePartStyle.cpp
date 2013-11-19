#include "TablePartStyle.h"

namespace PPTX
{
	namespace Logic
	{

		TablePartStyle::TablePartStyle()
		{
		}


		TablePartStyle::~TablePartStyle()
		{
		}
	

		TablePartStyle::TablePartStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TablePartStyle& TablePartStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TablePartStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			tcTxStyle = element.element("tcTxStyle");
			tcStyle = element.element("tcStyle");

			FillParentPointersForChilds();
		}


		const XML::XNode TablePartStyle::toXML() const
		{
			return XML::XElement(ns.a + m_name.get(),
					XML::Write(tcTxStyle) +
					XML::Write(tcStyle)
				);
		}

		const UniFill TablePartStyle::GetFillStyle(UniColor& Color)const
		{
			UniFill result;
			result.SetParentFilePointer(*parentFile);
			UniColor resColor;
			resColor.SetParentFilePointer(*parentFile);
			Color = resColor;
			if(!tcStyle.is_init())
				return result;
			if(tcStyle->fill.is_init())
				return tcStyle->fill->Fill;
			if(tcStyle->fillRef.is_init())
			{
				m_Theme->GetFillStyle(tcStyle->fillRef->idx.get_value_or(0), result);
				Color = tcStyle->fillRef->Color;
				return result;
			}
			return result;
		}

		void TablePartStyle::SetTheme(const boost::shared_ptr<PPTX::Theme> theme)
		{
			m_Theme = theme;
		}

		void TablePartStyle::FillParentPointersForChilds()
		{
			if(tcTxStyle.is_init())
				tcTxStyle->SetParentPointer(*this);
			if(tcStyle.is_init())
				tcStyle->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
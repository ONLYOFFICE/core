#include "TableStyle.h"

namespace PPTX
{
	namespace Logic
	{

		TableStyle::TableStyle()
		{
		}


		TableStyle::~TableStyle()
		{
		}
	

		TableStyle::TableStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableStyle& TableStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			styleId = element.attribute("styleId").value();
			styleName = element.attribute("styleName").value();

			tblBg = element.element("tblBg");
			wholeTbl = element.element("wholeTbl");
			band1H = element.element("band1H");
			band2H = element.element("band2H");
			band1V = element.element("band1V");
			band2V = element.element("band2V");
			lastCol = element.element("lastCol");
			firstCol = element.element("firstCol");
			lastRow = element.element("lastRow");
			seCell = element.element("seCell");
			swCell = element.element("swCell");
			firstRow = element.element("firstRow");
			neCell = element.element("neCell");
			nwCell = element.element("nwCell");

			FillParentPointersForChilds();
		}


		const XML::XNode TableStyle::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
					XML::XAttribute("styleId", styleId) +
					XML::XAttribute("styleName", styleName) +
					XML::Write(tblBg) +
					XML::Write(wholeTbl) +
					XML::Write(band1H) +
					XML::Write(band2H) +
					XML::Write(band1V) +
					XML::Write(band2V) +
					XML::Write(lastCol) +
					XML::Write(firstCol) +
					XML::Write(lastRow) +
					XML::Write(seCell) +
					XML::Write(swCell) +
					XML::Write(firstRow) +
					XML::Write(neCell) +
					XML::Write(nwCell)
				);
		}

		void TableStyle::FillParentPointersForChilds()
		{
			if(tblBg.is_init())
				tblBg->SetParentPointer(*this);
			if(wholeTbl.is_init())
				wholeTbl->SetParentPointer(*this);
			if(band1H.is_init())
				band1H->SetParentPointer(*this);
			if(band2H.is_init())
				band2H->SetParentPointer(*this);
			if(band1V.is_init())
				band1V->SetParentPointer(*this);
			if(band2V.is_init())
				band2V->SetParentPointer(*this);
			if(lastCol.is_init())
				lastCol->SetParentPointer(*this);
			if(firstCol.is_init())
				firstCol->SetParentPointer(*this);
			if(lastRow.is_init())
				lastRow->SetParentPointer(*this);
			if(seCell.is_init())
				seCell->SetParentPointer(*this);
			if(swCell.is_init())
				swCell->SetParentPointer(*this);
			if(firstRow.is_init())
				firstRow->SetParentPointer(*this);
			if(neCell.is_init())
				neCell->SetParentPointer(*this);
			if(nwCell.is_init())
				nwCell->SetParentPointer(*this);
		}

		void TableStyle::SetTheme(const boost::shared_ptr<PPTX::Theme> theme)
		{
			m_Theme = theme;
			if(tblBg.is_init())
				tblBg->SetTheme(m_Theme);
			if(wholeTbl.is_init())
				wholeTbl->SetTheme(m_Theme);
			if(band1H.is_init())
				band1H->SetTheme(m_Theme);
			if(band2H.is_init())
				band2H->SetTheme(m_Theme);
			if(band1V.is_init())
				band1V->SetTheme(m_Theme);
			if(band2V.is_init())
				band2V->SetTheme(m_Theme);
			if(lastCol.is_init())
				lastCol->SetTheme(m_Theme);
			if(firstCol.is_init())
				firstCol->SetTheme(m_Theme);
			if(lastRow.is_init())
				lastRow->SetTheme(m_Theme);
			if(seCell.is_init())
				seCell->SetTheme(m_Theme);
			if(swCell.is_init())
				swCell->SetTheme(m_Theme);
			if(firstRow.is_init())
				firstRow->SetTheme(m_Theme);
			if(neCell.is_init())
				neCell->SetTheme(m_Theme);
			if(nwCell.is_init())
				nwCell->SetTheme(m_Theme);
		}

	} // namespace Logic
} // namespace PPTX
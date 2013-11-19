#include "TxStyles.h"

namespace PPTX
{
	namespace Logic
	{

		TxStyles::TxStyles()
		{
		}


		TxStyles::~TxStyles()
		{
		}
	

		TxStyles::TxStyles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TxStyles& TxStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TxStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			titleStyle = element.element("titleStyle");
			bodyStyle = element.element("bodyStyle");
			otherStyle = element.element("otherStyle");

			FillParentPointersForChilds();
		}


		const XML::XNode TxStyles::toXML() const
		{
			return XML::XElement(ns.p + "txStyles",
					XML::Write(titleStyle) +
					XML::Write(bodyStyle) +
					XML::Write(otherStyle)
				);
		}

		void TxStyles::FillParentPointersForChilds()
		{
			if(titleStyle.is_init())
				titleStyle->SetParentPointer(*this);
			if(bodyStyle.is_init())
				bodyStyle->SetParentPointer(*this);
			if(otherStyle.is_init())
				otherStyle->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
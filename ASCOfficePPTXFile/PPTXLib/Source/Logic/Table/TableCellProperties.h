#pragma once
#ifndef PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/TextVerticalType.h"
#include "./../../Limit/TextAnchor.h"
#include "./../../Limit/HorzOverflow.h"
#include "./../Ln.h"
#include "./../Cell3D.h"
#include "./../UniFill.h"

namespace PPTX
{
	namespace Logic
	{
		class TableCellProperties : public WrapperWritingElement
		{
		public:
			TableCellProperties();
			virtual ~TableCellProperties();			
			explicit TableCellProperties(const XML::XNode& node);
			const TableCellProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<Ln> LnL;
			nullable_property<Ln> LnR;
			nullable_property<Ln> LnT;
			nullable_property<Ln> LnB;
			nullable_property<Ln> LnTlToBr;
			nullable_property<Ln> LnBlToTr;
			nullable_property<Cell3D> Cell3D;
			UniFill Fill;
//			<xsd:element name="headers" type="CT_Headers" minOccurs="0"/> 

			nullable_property<int> MarL;//default="91440"
			nullable_property<int> MarR;//default="91440"
			nullable_property<int> MarT;//default="45720"
			nullable_property<int> MarB;//default="45720"
			nullable_property<std::string, Limit::TextVerticalType> Vert;//default="horz"
			nullable_property<std::string, Limit::TextAnchor> Anchor;//default="t"
			nullable_property<bool> AnchorCtr;//default="false"
			nullable_property<std::string, Limit::HorzOverflow> HorzOverflow;//default="clip"
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_
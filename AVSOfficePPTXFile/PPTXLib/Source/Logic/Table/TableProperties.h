#pragma once
#ifndef PPTX_LOGIC_TABLE_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_TABLE_PROPERTIES_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../UniFill.h"
#include "./../EffectProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class TableProperties : public WrapperWritingElement
		{
		public:
			TableProperties();
			virtual ~TableProperties();			
			explicit TableProperties(const XML::XNode& node);
			const TableProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			
		public:
			UniFill Fill;
			EffectProperties Effects;
			nullable_property<std::string> TableStyleId;
/*
			<xsd:choice minOccurs="0" maxOccurs="1"> 
			<xsd:element name="tableStyle" type="CT_TableStyle"/> 
			<xsd:element name="tableStyleId" type="s:ST_Guid"/> 
			</xsd:choice> 
*/
			nullable_property<bool> Rtl;
			nullable_property<bool> FirstRow;
			nullable_property<bool> FirstCol;
			nullable_property<bool> LastRow;
			nullable_property<bool> LastCol;
			nullable_property<bool> BandRow;
			nullable_property<bool> BandCol;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_PROPERTIES_INCLUDE_H_
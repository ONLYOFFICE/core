#pragma once
#ifndef ODT_STYLES_LIST_LEVEL_PROPERTIES_INCLUDE_H_
#define ODT_STYLES_LIST_LEVEL_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "ListLevelLabelAlignment.h"
#include "nullable.h"
#include "./../Unit.h"


namespace Odt
{
	namespace Styles
	{
		class ListLevelProperties : public WritingElement
		{
		public:
			ListLevelProperties();
			virtual ~ListLevelProperties();
			explicit ListLevelProperties(const XML::XNode& node);
			const ListLevelProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			nullable<std::string>							m_listLevelPositionSpaceMode;
			nullable<ListLevelLabelAlignment>	m_aligments;
			nullable<UnitCm>									m_minLabelDistance;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_LIST_LEVEL_PROPERTIES_INCLUDE_H_
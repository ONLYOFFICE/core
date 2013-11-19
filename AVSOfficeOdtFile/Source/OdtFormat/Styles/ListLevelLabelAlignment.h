#pragma once
#ifndef ODT_STYLES_LIST_LEVEL_LABEL_ALIGNMENT_INCLUDE_H_
#define ODT_STYLES_LIST_LEVEL_LABEL_ALIGNMENT_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace Odt
{
	namespace Styles
	{
		class ListLevelLabelAlignment : public WritingElement
		{
		public:
			ListLevelLabelAlignment();
			virtual ~ListLevelLabelAlignment();
			explicit ListLevelLabelAlignment(const XML::XNode& node);
			const ListLevelLabelAlignment& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string		m_labelFollowedBy;
			std::string		m_listTabStopPosition;
			std::string		m_textIndent;
			std::string		m_marginLeft;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_LIST_LEVEL_LABEL_ALIGNMENT_INCLUDE_H_
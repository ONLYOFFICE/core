#pragma once
#ifndef ODT_STYLES_MASTER_STYLES_INCLUDE_H_
#define ODT_STYLES_MASTER_STYLES_INCLUDE_H_

#include "./../WritingElement.h"
#include "IEnumerable.h"
#include "MasterPage.h"
#include <vector>
#include <string>


namespace Odt
{
	namespace Styles
	{
		class MasterStyles : public WritingElement
		{
		public:
			MasterStyles();
			virtual ~MasterStyles();
			explicit MasterStyles(const XML::XNode& node);
			const MasterStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool exist(const std::string& name) const;
			const MasterPage& find(const std::string& name) const;
			MasterPage& find(const std::string& name);

		private:
			std::vector<MasterPage>	m_pages;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_MASTER_STYLES_INCLUDE_H_
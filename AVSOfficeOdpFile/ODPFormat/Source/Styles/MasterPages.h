#pragma once
#ifndef ODP_STYLES_MASTER_PAGES_INCLUDE_H_
#define ODP_STYLES_MASTER_PAGES_INCLUDE_H_

#include "OdtFormat/WritingVector.h"
#include "MasterPage.h"

namespace Odp
{
	namespace Styles
	{
		class MasterPages : public Odt::WritingVector<MasterPage>
		{
		public:
			MasterPages();
			virtual ~MasterPages();
			explicit MasterPages(const XML::XNode& node);
			const MasterPages& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const MasterPage& operator[](const std::string& name) const;
			MasterPage& operator[](const std::string& name);
			const MasterPage& find(const std::string& name) const;
			MasterPage& find(const std::string& name);
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_MASTER_PAGES_INCLUDE_H_
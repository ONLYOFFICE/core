#pragma once
#ifndef ODP_STYLES_FILE_INCLUDE_H_
#define ODP_STYLES_FILE_INCLUDE_H_

#include "OdtFormat/File.h"
#include "property.h"

#include "Markers.h"
#include "FillImages.h"
#include "Gradients.h"
#include "OdtFormat/Logic/DefaultStyles.h"
#include "OdtFormat/Logic/NamedStyles.h"
#include "TableTemplates.h"
//#include "Layouts.h"

#include "PageLayouts.h"
#include "OdtFormat/Logic/ListStyles.h"

//#include "LayerSet.h"
//#include "HandoutMaster.h"
#include "MasterPages.h"
#include <vector>

namespace Odp
{
	namespace Styles
	{
		class File : public Odt::File
		{
		public:
			File();
			virtual ~File();

		public:
			virtual void read(const boost::filesystem::wpath& path);
			virtual void write(const boost::filesystem::wpath& path) const;
			virtual const bool isValid(const boost::filesystem::wpath& path) const;

		public:
			static const bool exist(const boost::filesystem::wpath& path);

		public:
			property<std::string> m_version;
			property<FillImages> FillImages;
			property<Gradients> Gradients;
			property<Markers> Markers;
			property<Odt::Logic::DefaultStyles> DefaultStyles;
			property<Odt::Logic::NamedStyles> Styles;
			property<TableTemplates> TableTemplates;
			//property<Layouts> Layouts;

			property<PageLayouts> PageLayouts;
			property<Odt::Logic::NamedStyles>	AutomaticStyles;
			property<Odt::Logic::ListStyles>	ListStyles;

			//property<LayerSet> LayerSet;
			//property<HandoutMaster> HandoutMaster;
			property<MasterPages> MasterPages;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_FILE_INCLUDE_H_
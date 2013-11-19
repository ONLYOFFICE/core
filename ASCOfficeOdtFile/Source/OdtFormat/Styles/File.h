#pragma once
#ifndef ODT_STYLES_FILE_INCLUDE_H_
#define ODT_STYLES_FILE_INCLUDE_H_

#include "./../File.h"
#include "property.h"
#include "./../Logic/FontFaceDecls.h"
#include "Styles.h"
#include "AutomaticStyles.h"
#include "MasterStyles.h"
#include "NotesConfiguration.h"


namespace Odt
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
			property<Logic::FontFaceDecls>	FontFaceDecls;
			property<Odt::Styles::Styles>	Styles;
			property<AutomaticStyles>		AutomaticStyles;
			property<MasterStyles>			MasterStyles;

		private:
			std::string		m_version;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_FILE_INCLUDE_H_
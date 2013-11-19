#pragma once
#ifndef ODT_CONTENT_FILE_INCLUDE_H_
#define ODT_CONTENT_FILE_INCLUDE_H_

#include "./../File.h"
#include "./../Logic/FontFaceDecls.h"
#include "Body.h"
#include "./../Logic/NamedStyles.h"
#include "./../Logic/ListStyles.h"
#include "property.h"
#include <boost/shared_ptr.hpp>


namespace Odt
{
	namespace Content
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
			const bool hasList() const;
			const bool hasNote() const;

		public:
			property<Logic::FontFaceDecls>	FontFaceDecls;
			property<Body>									Body;
			property<Logic::NamedStyles>		NamedStyles;
			property<Logic::ListStyles>			ListStyles;

		private:
			std::string		m_version;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_FILE_INCLUDE_H_
#pragma once
#ifndef PPTX_TABLESTYLES_FILE_INCLUDE_H_
#define PPTX_TABLESTYLES_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "property.h"
#include "nullable.h"
#include "Logic/TableStyle.h"
#include <map>
#include "Theme.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	class TableStyles: public WrapperFile
	{
	public:
		TableStyles();
		TableStyles(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~TableStyles();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		property<std::string> def;
		//property<std::list<Logic::TableStyle> > TblStyles;
		property<std::map<std::string, Logic::TableStyle> > Styles;

		void SetTheme(const boost::shared_ptr<PPTX::Theme> theme);

		virtual DWORD GetRGBAFromMap(const std::string& str)const;
		virtual DWORD GetARGBFromMap(const std::string& str)const;
		virtual DWORD GetBGRAFromMap(const std::string& str)const;
		virtual DWORD GetABGRFromMap(const std::string& str)const;
	private:
		boost::shared_ptr<PPTX::Theme> m_Theme;
	};
} // namespace PPTX

#endif // PPTX_TABLESTYLES_FILE_INCLUDE_H_
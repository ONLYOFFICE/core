#pragma once
#ifndef PPTX_THEME_FILE_INCLUDE_H_
#define PPTX_THEME_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"

#include "property.h"
#include "nullable.h"
#include "XML.h"
#include "Theme/ThemeElements.h"
#include "Logic/DefaultShapeDefinition.h"
#include "Theme/ExtraClrScheme.h"
#include <list>

#include "Logic/ShapeProperties.h"
#include "Logic/ClrMap.h"

#include "Presentation.h"

namespace PPTX
{
	class Theme : public PPTX::WrapperFile, public PPTX::FileContainer
	{
	public:
		Theme();
		Theme(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~Theme();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

		DWORD GetRGBAFromScheme(const std::string& str)const;
		DWORD GetARGBFromScheme(const std::string& str)const;
		DWORD GetBGRAFromScheme(const std::string& str)const;
		DWORD GetABGRFromScheme(const std::string& str)const;

		DWORD GetRGBAFromMap(const std::string& str)const;
		DWORD GetARGBFromMap(const std::string& str)const;
		DWORD GetBGRAFromMap(const std::string& str)const;
		DWORD GetABGRFromMap(const std::string& str)const;

		virtual void FillShapeProperties(Logic::ShapeProperties& props, const std::string& type)const;
		virtual std::wstring GetMediaFullPathNameFromRId(const OOX::RId& rid)const;
		void GetLineStyle(int number, Logic::Ln& lnStyle)const;
		void GetFillStyle(int number, Logic::UniFill& fillStyle)const;

	public:
		nullable_property<std::string> name;
		property<nsTheme::ThemeElements> themeElements;
		nullable_property<Logic::DefaultShapeDefinition> spDef;
		nullable_property<Logic::DefaultShapeDefinition> lnDef;
		nullable_property<Logic::DefaultShapeDefinition> txDef;
		property<std::list<nsTheme::ExtraClrScheme> > extraClrSchemeLst;

		boost::shared_ptr<Presentation> Presentation;
		void SetColorMap(const Logic::ClrMap& map){m_map = &map;};
	private:
		Logic::ClrMap const* m_map;
	};
} // namespace PPTX

#endif // PPTX_THEME_FILE_INCLUDE_H_
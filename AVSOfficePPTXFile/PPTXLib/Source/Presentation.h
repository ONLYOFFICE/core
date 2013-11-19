#pragma once
#ifndef PPTX_PRESENTATION_FILE_INCLUDE_H_
#define PPTX_PRESENTATION_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"

#include "property.h"
#include "nullable.h"
#include "Limit/Conformance.h"
#include "Logic/TextListStyle.h"
#include "Logic/ClrMap.h"
#include "Theme/ClrScheme.h"

#include "Presentation/EmbeddedFont.h"
#include "Presentation/Kinsoku.h"
#include "Presentation/NotesSz.h"
#include "Presentation/PhotoAlbum.h"
#include "Logic/XmlId.h"
#include "Presentation/SldSz.h"
#include <list>

namespace PPTX
{
	class Presentation : public WrapperFile, public PPTX::FileContainer
	{
	public:
		Presentation();
		Presentation(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~Presentation();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		//Childs
		//custDataLst (Customer Data List)
//			property<std::list<Presentation::CustShow> > custShowLst (List of Custom Shows)
		nullable_property<Logic::TextListStyle> defaultTextStyle;
		property<std::list<nsPresentation::EmbeddedFont> > embeddedFontLst;
		property<std::list<Logic::XmlId> > handoutMasterIdLst;
		nullable_property<nsPresentation::Kinsoku> kinsoku;
		//modifyVerifier (Modification Verifier)
		property<std::list<Logic::XmlId> > notesMasterIdLst;
		nullable_property<nsPresentation::NotesSz> notesSz;
		nullable_property<nsPresentation::PhotoAlbum> photoAlbum;
		property<std::list<Logic::XmlId> > sldIdLst;
		property<std::list<Logic::XmlId> > sldMasterIdLst;
		nullable_property<nsPresentation::SldSz> sldSz;
		//smartTags (Smart Tags)

		// Attrs
		nullable_property<bool> attrAutoCompressPictures;
		nullable_property<int, setter::between<int, 1, 2147483647> > attrBookmarkIdSeed;
		nullable_property<bool> attrCompatMode;
		nullable_property<std::string, Limit::Conformance> attrConformance;
		nullable_property<bool> attrEmbedTrueTypeFonts;
		nullable_property<int, setter::only_positive<int> > attrFirstSlideNum;
		nullable_property<bool> attrRemovePersonalInfoOnSave;
		nullable_property<bool> attrRtl;
		nullable_property<bool> attrSaveSubsetFonts;
		nullable_property<std::string> attrServerZoom;
		nullable_property<bool> attrShowSpecialPlsOnTitleSld;
		nullable_property<bool> attrStrictFirstAndLastChars;

	private:
		property<Logic::ClrMap> m_clrMap;
		property<nsTheme::ClrScheme> m_clrScheme;
	public:
		void SetClrMap(property<Logic::ClrMap> map)				{m_clrMap = map;};
		void SetClrScheme(property<nsTheme::ClrScheme> scheme)	{m_clrScheme = scheme;};

		DWORD GetRGBAFromMap(const std::string& str)const;
		DWORD GetARGBFromMap(const std::string& str)const;
		DWORD GetBGRAFromMap(const std::string& str)const;
		DWORD GetABGRFromMap(const std::string& str)const;
	};
} // namespace PPTX

#endif // PPTX_PRESENTATION_FILE_INCLUDE_H_
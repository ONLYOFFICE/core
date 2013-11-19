#pragma once
#ifndef PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_
#define PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_

#include <boost/filesystem.hpp>
#include "WrapperFile.h"
#include "FileContainer.h"

#include "property.h"
#include "nullable.h"
#include "XML.h"
#include "Logic/ClrMap.h"
#include "Logic/CSld.h"
#include "Logic/Transitions/Transition.h"
#include "Logic/Timing/Timing.h"
#include "Logic/Hf.h"
#include "Logic/TxStyles.h"
#include "Logic/XmlId.h"

#include "Logic/Shape.h"
#include "Logic/ShapeProperties.h"
#include "Logic/TxBody.h"

#include "Theme.h"
#include "TableStyles.h"
#include "VmlDrawing.h"

namespace PPTX
{
	class SlideMaster : public WrapperFile, public FileContainer
	{
	public:
		SlideMaster();
		SlideMaster(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~SlideMaster();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		void GetLevelUp(const Logic::Shape& pShape)const;
		void FillShapeProperties(Logic::ShapeProperties& props, const std::string& type)const;
		void GetBackground(Logic::BgPr& bg, DWORD& ARGB)const;
		std::wstring GetMediaFullPathNameFromRId(const OOX::RId& rid)const;

		DWORD GetRGBAFromMap(const std::string& str)const;
		DWORD GetARGBFromMap(const std::string& str)const;
		DWORD GetBGRAFromMap(const std::string& str)const;
		DWORD GetABGRFromMap(const std::string& str)const;

		DWORD GetRGBAFromScheme(const std::string& str)const;
		DWORD GetARGBFromScheme(const std::string& str)const;
		DWORD GetBGRAFromScheme(const std::string& str)const;
		DWORD GetABGRFromScheme(const std::string& str)const;

		//void ApplyColors();

	public:
		property<Logic::CSld> cSld;
		property<Logic::ClrMap> clrMap;
		property<std::list<Logic::XmlId> > sldLayoutIdLst;
		nullable_property<Logic::Transition> transition;
		nullable_property<Logic::Timing> timing;
		nullable_property<Logic::HF> hf;
		nullable_property<Logic::TxStyles> txStyles;
		nullable_property<bool> preserve;

		boost::shared_ptr<Theme> Theme;
		boost::shared_ptr<TableStyles> TableStyles;
		boost::shared_ptr<VmlDrawing> Vml;
		void ApplyRels();
		const boost::filesystem::wpath GetPathBySpid(const std::string& spid)const;
	};
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_
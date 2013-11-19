#pragma once
#ifndef PPTX_SLIDES_SLIDELAYOUT_INCLUDE_H_
#define PPTX_SLIDES_SLIDELAYOUT_INCLUDE_H_

#include <boost/filesystem.hpp>
#include "WrapperFile.h"
#include "FileContainer.h"

#include "property.h"
#include "nullable.h"
#include "XML.h"
#include "Logic/CSld.h"
#include "Logic/ClrMapOvr.h"
#include "Logic/Transitions/Transition.h"
#include "Logic/Timing/Timing.h"
#include "Logic/Hf.h"
#include "Limit/SlideLayoutType.h"

#include "Logic/Shape.h"
#include "Logic/ShapeProperties.h"

#include "Theme.h"
#include "SlideMaster.h"
#include "TableStyles.h"
#include "VmlDrawing.h"

namespace PPTX
{
	class SlideLayout : public WrapperFile, public PPTX::FileContainer
	{
	public:
		SlideLayout();
		SlideLayout(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~SlideLayout();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		virtual void GetLevelUp(const Logic::Shape& pShape)const;
		virtual void FillShapeProperties(Logic::ShapeProperties& props, const std::string& type)const;
		virtual void GetBackground(Logic::BgPr& bg, DWORD& ARGB)const;
		virtual std::wstring GetMediaFullPathNameFromRId(const OOX::RId& rid)const;

		virtual DWORD GetRGBAFromMap(const std::string& str)const;
		virtual DWORD GetARGBFromMap(const std::string& str)const;
		virtual DWORD GetBGRAFromMap(const std::string& str)const;
		virtual DWORD GetABGRFromMap(const std::string& str)const;

	public:
		property<Logic::CSld> cSld;
		nullable_property<Logic::ClrMapOvr> clrMapOvr;
		nullable_property<Logic::Transition> transition;
		nullable_property<Logic::Timing> timing;
		nullable_property<Logic::HF> hf;

		// Attributes
		nullable_property<std::string> matchingName;
		nullable_property<bool> preserve;
		nullable_property<bool> showMasterPhAnim;
		nullable_property<bool> showMasterSp;
		nullable_property<std::string, Limit::SlideLayoutType> attrType;
		nullable_property<bool> userDrawn;

		boost::shared_ptr<SlideMaster> Master;
		boost::shared_ptr<Theme> Theme;
		boost::shared_ptr<TableStyles> TableStyles;
		boost::shared_ptr<VmlDrawing> Vml;
		void ApplyRels();
		const boost::filesystem::wpath GetPathBySpid(const std::string& spid)const;
	};
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDELAYOUT_INCLUDE_H_
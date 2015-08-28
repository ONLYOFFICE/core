#pragma once

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "progressCallback.h"

namespace oox
{
	class xlsx_conversion_context;
	namespace package
	{
		class xlsx_document;
	}

}
namespace XLS
{
	class BaseObject;
	class BiffStructure;

	typedef boost::shared_ptr<BaseObject> BaseObjectPtr;
	
	class GlobalWorkbookInfo;
	typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

	class WorkbookStreamObject;
	class WorksheetSubstream;
	class GlobalsSubstream;
	
	class FORMATTING;
	class THEME;
	class SHAREDSTRINGS;
	class HLINK;
	class LBL;
	class OBJECTS;
	class MSODRAWINGGROUP;
}

namespace ODRAW
{
	class OfficeArtRecord;
	class OfficeArtBStoreContainer;
	class OfficeArtSpContainer;
	class OfficeArtFOPT;
	class OfficeArtFSP;
	class OfficeArtFOPTE;	
	typedef boost::shared_ptr<OfficeArtFOPTE> OfficeArtFOPTEPtr;
		
}

class XlsConverter
{
public:
	XlsConverter(const std::wstring & xls_file, const std::wstring & xlsx_path, const ProgressCallback* ffCallBack);
	~XlsConverter() ;

	const	ProgressCallback* pCallBack;
	short	bUserStopConvert;
	bool	UpdateProgress(long nComplete);

	void convertDocument();
	void write();

	void convert(XLS::BaseObject	*xls_unknown);

private:		
	std::wstring					xlsx_path;
	oox::package::xlsx_document		*output_document;
    oox::xlsx_conversion_context	*xlsx_context;

	XLS::BaseObjectPtr				xls_document;	
	XLS::GlobalWorkbookInfoPtr		xls_global_info;

	void convert(XLS::WorkbookStreamObject		* woorkbook);
	void convert(XLS::WorksheetSubstream		* sheet);
	void convert(XLS::GlobalsSubstream			* elem);
	void convert(XLS::FORMATTING				* formating);
	void convert(XLS::THEME						* theme);
	void convert(XLS::SHAREDSTRINGS				* sharedstrings);
	void convert(XLS::HLINK						* hlink);
	void convert(XLS::LBL						* def_name);
	void convert(XLS::OBJECTS					* objects);
	void convert(XLS::MSODRAWINGGROUP			* mso_drawing);

	void convert(ODRAW::OfficeArtRecord			* art);
	void convert(ODRAW::OfficeArtBStoreContainer* art_bstore);
	void convert(ODRAW::OfficeArtSpContainer	* sp, bool anchor_only = false);
	void convert(ODRAW::OfficeArtFOPT 			* fort);
	void convert(ODRAW::OfficeArtFSP 			* fsp);

	void convert_shape			(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_group_shape	(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_blip			(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_text			(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_geometry		(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_geometry_text	(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_fill_style		(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_line_style		(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_shadow			(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);
	void convert_transform		(std::vector<ODRAW::OfficeArtFOPTEPtr> & fort);

	std::wstring GetTargetMoniker(XLS::BiffStructure *moniker);

};

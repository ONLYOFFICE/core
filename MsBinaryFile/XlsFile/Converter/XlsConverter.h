/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

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
	class BiffStructure;

	class CompoundFile;
	typedef boost::shared_ptr<CompoundFile> CompoundFilePtr;

	class BaseObject;
	typedef boost::shared_ptr<BaseObject> BaseObjectPtr;
	
	class GlobalWorkbookInfo;
	typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

	class WorkbookStreamObject;
	class CommonSubstream;
	class WorksheetSubstream;
	class GlobalsSubstream;
	class ChartSheetSubstream;
	class MacroSheetSubstream;
	
	class BACKGROUND;
	class FORMATTING;
	class THEME;
	class SHAREDSTRINGS;
	class HLINK;
	class LBL;
	class OBJECTS;
	class MSODRAWINGGROUP;
	class IMDATA;
	class PIVOTVIEW;
	class PIVOTCACHEDEFINITION;
	class SUPBOOK;
	class QUERYTABLE;
	class FEAT11;

	class Note;
	class TxO;
	class Obj;
}

namespace ODRAW
{
	class OfficeArtRecord;
	class OfficeArtBStoreContainer;
	class OfficeArtSpgrContainer;
	class OfficeArtSpContainer;
	class OfficeArtFOPT;
	class OfficeArtFSP;
	class OfficeArtFOPTE;	
	class OfficeArtTertiaryFOPT;
	typedef boost::shared_ptr<OfficeArtFOPTE> OfficeArtFOPTEPtr;
		
}

class XlsConverter
{
public:
	XlsConverter();
	XlsConverter(const std::wstring & xlsFileName, const std::wstring & xlsxFilePath, const std::wstring & password, const std::wstring & fontsPath, const std::wstring & tempPath, const int lcid_user, bool & bMacros);
	~XlsConverter() ;

    oox::xlsx_conversion_context	*xlsx_context;
	XLS::BaseObjectPtr				xls_document;	
	XLS::GlobalWorkbookInfoPtr		xls_global_info;

	void convertDocument();
	void write();

	void convert(XLS::BaseObject *xls_unknown);

	void convert_common(XLS::CommonSubstream* strm);

	void convert(XLS::WorkbookStreamObject		* woorkbook);
	void convert(XLS::WorksheetSubstream		* sheet);
	void convert(XLS::ChartSheetSubstream		* chart);
	void convert(XLS::GlobalsSubstream			* elem);
	void convert(XLS::MacroSheetSubstream		* chart);

	void convert(XLS::FORMATTING				* formating);
	void convert(XLS::BACKGROUND 				* back);
	void convert(XLS::SHAREDSTRINGS				* sharedstrings);
	void convert(XLS::HLINK						* hlink);
	void convert(XLS::LBL						* def_name);
	void convert(XLS::OBJECTS					* objects, XLS::WorksheetSubstream * sheet);
	void convert(XLS::MSODRAWINGGROUP			* mso_drawing);
	void convert(XLS::TxO						* text_obj);
	void convert(XLS::Obj						* obj);
	void convert(XLS::Note						* note);
	void convert(XLS::IMDATA					* imadata);
	void convert(XLS::PIVOTVIEW					* pivot_view);
	void convert(XLS::PIVOTCACHEDEFINITION		* pivot_cached);
	void convert(XLS::SUPBOOK					* external);
	void convert(XLS::QUERYTABLE				* query_table);
	void convert(XLS::FEAT11					* shared_feature);

	void convert(ODRAW::OfficeArtRecord			* art);
	void convert(ODRAW::OfficeArtBStoreContainer* art_bstore, int start_id = 0);
	void convert(ODRAW::OfficeArtSpgrContainer	* spgr);
	void convert(ODRAW::OfficeArtSpContainer	* sp, bool anchor_only = false);
	void convert(ODRAW::OfficeArtFOPT 			* fort);
	void convert(ODRAW::OfficeArtTertiaryFOPT	* fort);
	void convert(ODRAW::OfficeArtFSP 			* fsp);

	std::wstring GetTargetMoniker(XLS::BiffStructure *moniker);

	bool isError();
	bool is_encrypted;
	bool is_older_version;

	std::wstring WriteMediaFile	(char *data, int size, std::wstring type_ext, int id = -1);
private:	
	void convert_theme			();
	void convert_old			(XLS::OBJECTS				* objects, XLS::WorksheetSubstream * sheet);
	void convert_chart_sheet	(XLS::ChartSheetSubstream	* chart);

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

	std::wstring					xlsx_path;
	oox::package::xlsx_document		*output_document;

	XLS::CompoundFilePtr			xls_file;
};
typedef boost::shared_ptr<XlsConverter> XlsConverterPtr;

/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "CompositeObject.h"

namespace XLS
{;

class StreamCacheReader;
class BOF;

class GlobalsSubstream;
typedef boost::shared_ptr<GlobalsSubstream>		GlobalsSubstreamPtr;

class GlobalsSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GlobalsSubstream)
public:
	GlobalsSubstream(const unsigned short code_page);
	~GlobalsSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType type = typeGlobalsSubstream;

	int serialize_format(std::wostream & _stream);
	int serialize_protection(std::wostream & _stream);
	
	BaseObjectPtr					m_THEME;
	BaseObjectPtr					m_Formating;
	BaseObjectPtr					m_Template;
	BaseObjectPtr					m_SHAREDSTRINGS;
	BaseObjectPtr					m_ExtSST;
	BaseObjectPtr					m_CodePage;
	BaseObjectPtr					m_Country;
	BaseObjectPtr					m_WriteProtect;
	BaseObjectPtr					m_PROTECTION;
	BaseObjectPtr					m_RRTabId;
	BaseObjectPtr					m_FNGROUPS;
	BaseObjectPtr					m_BookExt;
	BaseObjectPtr					m_CodeName;
	BaseObjectPtr					m_METADATA;
	BaseObjectPtr					m_MTRSettings;
	BaseObjectPtr					m_Date1904;
	BaseObjectPtr					m_CalcPrecision;

	std::vector<BaseObjectPtr>		m_arBIGNAME;
	std::vector<BaseObjectPtr>		m_arHFPicture;
	std::vector<BaseObjectPtr>		m_arLBL;
	std::vector<BaseObjectPtr>		m_arMSODRAWINGGROUP;
	std::vector<BaseObjectPtr>		m_arWindow1;
	std::vector<BaseObjectPtr>		m_arUserBView;
	std::vector<BaseObjectPtr>		m_arBUNDLESHEET;
	std::vector<BaseObjectPtr>		m_arSUPBOOK;
	//std::vector<BaseObjectPtr>	m_arPIVOTCACHEDEFINITION; -> GlobalWorkbookInfo
	std::vector<BaseObjectPtr>		m_arDConn;

	std::vector<BiffStructurePtr>	m_arHFPictureDrawing;
	
	unsigned short					code_page_;
	GlobalWorkbookInfoPtr			global_info_;

	void LoadHFPicture();
	void UpdateXti();
	void UpdateXFC();
	void UpdateDefineNames();
	void UpdateExternalDefineNames();
};

} // namespace XLS

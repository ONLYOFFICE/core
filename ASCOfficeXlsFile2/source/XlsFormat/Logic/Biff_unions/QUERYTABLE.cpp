/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "QUERYTABLE.h"
#include "DBQUERY.h"
#include "DBQUERYEXT.h"
#include "SXADDLQSI.h"
#include "QSIR.h"
#include "SORTDATA12.h"

#include "../Biff_records/Qsi.h"
#include "../Biff_records/QsiSXTag.h"
#include "../Biff_records/DbOrParamQry.h"
#include "../Biff_records/SXString.h"
#include "../Biff_records/TxtQry.h"

namespace XLS
{

QUERYTABLE::QUERYTABLE()
{
}

QUERYTABLE::~QUERYTABLE()
{
}

BaseObjectPtr QUERYTABLE::clone()
{
	return BaseObjectPtr(new QUERYTABLE(*this));
}

// QUERYTABLE = Qsi DBQUERY QsiSXTag DBQUERYEXT [SXADDLQSI] [QSIR] [SORTDATA12]

const bool QUERYTABLE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Qsi>()) return false;

	global_info = proc.getGlobalWorkbookInfo();

	m_Qsi = elements_.back(); elements_.pop_back();

	if (proc.mandatory<DBQUERY>())
	{
		m_DBQUERY = elements_.back(); elements_.pop_back();
	}
	if (proc.mandatory<QsiSXTag>())
	{
		m_QsiSXTag = elements_.back(); elements_.pop_back();
	}
	if (proc.mandatory<DBQUERYEXT>())
	{
		m_DBQUERYEXT = elements_.back(); elements_.pop_back();
	}
	if (proc.optional<SXADDLQSI>())
	{
		m_SXADDLQSI = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<QSIR>())
	{
		m_QSIR = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<SORTDATA12>())
	{
		m_SORTDATA12 = elements_.back(); elements_.pop_back();
	}

	return true;
}
int QUERYTABLE::serialize(std::wostream & strm)
{
	Qsi *info = dynamic_cast<Qsi*>(m_Qsi.get());
	if (!info) return 0;	

	int connectionId = serialize_connection();
	if (connectionId < 1) return 0;

	std::wstring name = info->rgchName.value();
	if (name.empty())
		name = L"Connection" + std::to_wstring(connectionId);

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"queryTable")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");	
			CP_XML_ATTR(L"connectionId", connectionId);	
			CP_XML_ATTR(L"name", name);

			if (info->fAutoFormat)
			{
				CP_XML_ATTR(L"autoFormatId", info->itblAutoFmt); 
			}
			CP_XML_ATTR(L"applyNumberFormats",		info->fibitAtrNum);//0" 
			CP_XML_ATTR(L"applyBorderFormats",		info->fibitAtrBdr);//0" 
			CP_XML_ATTR(L"applyFontFormats",		info->fibitAtrFnt);//1" 
			CP_XML_ATTR(L"applyPatternFormats",		info->fibitAtrPat);//1" 
			CP_XML_ATTR(L"applyAlignmentFormats",	info->fibitAtrAlc);//0" 
			//CP_XML_ATTR(L"applyWidthHeightFormats", info->);//0"/>
		}
	}

	return 0;
}
int QUERYTABLE::serialize_connection()
{
	Qsi *info = dynamic_cast<Qsi*>(m_Qsi.get());
	if (!info) return -1;	
		
	DBQUERY *query = dynamic_cast<DBQUERY*>(m_DBQUERY.get());
	if (!query) return -1;

	DbOrParamQry* queryOrParam = dynamic_cast<DbOrParamQry*>(query->m_DbQry.get());
	if (!queryOrParam) return -1;

	DBQUERYEXT *query_ext = dynamic_cast<DBQUERYEXT*>(m_DBQUERYEXT.get());

	int connectionId = ++global_info->connectionId;

	std::wstring name = info->rgchName.value();
	if (name.empty())
		name = L"Connection" + std::to_wstring(connectionId);

	CP_XML_WRITER(global_info->connections_stream)
	{
		CP_XML_NODE(L"connection")
		{
			CP_XML_ATTR(L"id", connectionId);	
			CP_XML_ATTR(L"name", name);

			CP_XML_ATTR(L"type", queryOrParam->query.dbt);
			//background="1" 
			//saveData="1"

			if (queryOrParam->query.fSavePwd) CP_XML_ATTR(L"savePassword", 1);

			CP_XML_ATTR(L"refreshedVersion", 1);

			if (queryOrParam->query.dbt == 6)
			{
				TxtQry *query_txt = dynamic_cast<TxtQry*>(query_ext->m_TxtQry.get());
				if (query_txt)
				{
					CP_XML_NODE(L"textPr")
					{
						CP_XML_ATTR(L"sourceFile", query_txt->rgchFile.value());
						//delimited="0"
						CP_XML_NODE(L"textFields")
						{
							for (size_t i = 0; i < query_txt->rgtxtwf.size(); i++)
							{
								CP_XML_NODE(L"textField")
								{
									switch(query_txt->rgtxtwf[i].fieldType)
									{
									case 0:	CP_XML_ATTR(L"type", L"general");	break;
									case 1:	CP_XML_ATTR(L"type", L"text");	break;
									case 2:	CP_XML_ATTR(L"type", L"MDY");	break;
									case 3:	CP_XML_ATTR(L"type", L"DMY");	break;
									case 4:	CP_XML_ATTR(L"type", L"YMD");	break;
									case 5:	CP_XML_ATTR(L"type", L"MYD");	break;
									case 6:	CP_XML_ATTR(L"type", L"DYM");	break;
									case 7:	CP_XML_ATTR(L"type", L"YDM");	break;
									case 8:	CP_XML_ATTR(L"type", L"skip");	break;
									case 9:	CP_XML_ATTR(L"type", L"EMD");	break;
									}
									CP_XML_ATTR(L"position", query_txt->rgtxtwf[i].fieldStart);
								}
							}
						}
					}
				}
			}
			else
			{
				int index = 0;

				CP_XML_NODE(L"dbPr")
				{
					std::wstring command, connection;
					for (index = 0; index < queryOrParam->query.cstQuery; index++)
					{
						command += query->m_arSXString[index];
					}
					
					for (; index < queryOrParam->query.cstQuery + queryOrParam->query.cstOdbcConn; index++)
					{
						connection += query->m_arSXString[index];
					}
					
					CP_XML_ATTR(L"connection", connection);
					CP_XML_ATTR(L"command", command);
				}
			}
		}
	}
	return connectionId;
}

} // namespace XLS


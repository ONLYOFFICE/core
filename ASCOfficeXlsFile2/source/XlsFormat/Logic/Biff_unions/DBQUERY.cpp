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

#include "DBQUERY.h"
#include "DBQUERYEXT.h"

#include "../Biff_records/DbOrParamQry.h"
#include "../Biff_records/SXString.h"
#include "../Biff_records/TxtQry.h"

namespace XLS
{


DBQUERY::DBQUERY()
{
}


DBQUERY::~DBQUERY()
{
}


class Parenthesis_DBQUERY_3: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERY_3)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERY_3(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXString>())
		{
			return false;
		}
		DbOrParamQry param(1);
		proc.mandatory(param);
		return true;
	};
};


class Parenthesis_DBQUERY_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERY_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERY_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		DbOrParamQry param(1);
		if(!proc.mandatory(param))
		{
			return false;
		}
	
		int count = proc.repeated<Parenthesis_DBQUERY_3>(0, 0);
		
		return true;
	};
};


class Parenthesis_DBQUERY_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERY_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERY_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXString>())
		{
			return false;
		}
		
		int count = proc.repeated<SXString>(0, 0);	

		proc.optional<Parenthesis_DBQUERY_2>();

		return true;
	};
};


BaseObjectPtr DBQUERY::clone()
{
	return BaseObjectPtr(new DBQUERY(*this));
}

// DBQUERY = DbOrParamQry [1*SXString [DbOrParamQry *(SXString DbOrParamQry)]] *SXString
const bool DBQUERY::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<DbOrParamQry>())
	{
		return false;
	}
	m_DbQry = elements_.front(); elements_.pop_front();

	proc.optional<Parenthesis_DBQUERY_1>();

	while(!elements_.empty())
	{
		SXString* str = dynamic_cast<SXString*>(elements_.front().get());
		if (!str) 
		{
			m_DbParam = elements_.front(); elements_.pop_front();
			break;
		}
		m_arSXString.push_back(str->value());
		elements_.pop_front();
	}

	while(!elements_.empty())
	{
		SXString* str = dynamic_cast<SXString*>(elements_.front().get());
		if (str)
		{
			_DbParam a;
			a.string = str->value();
			m_arParams.push_back(a);
		}
		else
		{
			DbOrParamQry* param = dynamic_cast<DbOrParamQry*>(elements_.front().get());
			if (param)
			{
				m_arParams.back().param = elements_.front(); 
			}

		}
		elements_.pop_front();
	}		
	int count = proc.repeated<SXString>(0, 0);
	while(!elements_.empty())
	{
		SXString* str = dynamic_cast<SXString*>(elements_.front().get());
		if (str)
		{
			m_arSXString.push_back(str->value()); 
		}
		elements_.pop_front();
	}
	return true;
}

int DBQUERY::serialize(std::wostream & strm)
{
	std::wstring name;
	
	int connectionId = serialize_connection(name);
	
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"cacheSource")
		{
			CP_XML_ATTR(L"type", L"external");
			CP_XML_ATTR(L"connectionId", connectionId);	//connectionId in connections(root)
		}
	}
	return 0;
}

int DBQUERY::serialize_connection(std::wstring & name, DBQUERYEXT *query_ext)
{
	DbOrParamQry* queryOrParam = dynamic_cast<DbOrParamQry*>(m_DbQry.get());
	if (!queryOrParam) return -1;

	++global_info->connectionId;
	
	if (name.empty())
		name = L"Connection" + std::to_wstring(global_info->connectionId);

	CP_XML_WRITER(global_info->connections_stream)
	{
		CP_XML_NODE(L"connection")
		{
			CP_XML_ATTR(L"id", global_info->connectionId);	
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
						command += m_arSXString[index];
					}
					
					for (; index < queryOrParam->query.cstQuery + queryOrParam->query.cstOdbcConn; index++)
					{
						connection += m_arSXString[index];
					}
					
					CP_XML_ATTR(L"connection", connection);
					CP_XML_ATTR(L"command", command);
				}
			}
		}
	}
	return global_info->connectionId;
}


} // namespace XLS


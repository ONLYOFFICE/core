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
#include <Logic/Biff_records/DbOrParamQry.h>
#include <Logic/Biff_records/SXString.h>

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
	connectionId = ++global_info->connectionId;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"cacheSource")
		{
			CP_XML_ATTR(L"type", L"external");
			CP_XML_ATTR(L"connectionId", connectionId);	//connectionId in connections(root)
		}
	}
	serialize_connection(global_info->connections_stream);
	return 0;
}

int DBQUERY::serialize_connection(std::wostream & strm)
{
	DbOrParamQry* queryOrParam = dynamic_cast<DbOrParamQry*>(m_DbQry.get());
	if (!queryOrParam) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"connection")
		{
			CP_XML_ATTR(L"id", 1);	//connectionId in connections(root)
			CP_XML_ATTR(L"name", L"Connection");

			CP_XML_ATTR(L"type", queryOrParam->query.dbt);
			//switch(queryOrParam->query.dbt)
			//{
			//	case 0x1:	CP_XML_ATTR(L"type", 1); break;
			//	case 0x2:
			//	case 0x3:
			//	case 0x4:
			//	case 0x5:
			//	case 0x6:
			//	case 0x7:
			//}			

			if (queryOrParam->query.fSavePwd) CP_XML_ATTR(L"savePassword", 1);
			if (queryOrParam->query.fSavePwd) CP_XML_ATTR(L"refreshedVersion", 1);

			int index = 0;
			CP_XML_NODE(L"dbPr")
			{
				if (index < m_arSXString.size())
				{
					CP_XML_ATTR(L"command", m_arSXString[index]);
				}
				index++;

				if (index < m_arSXString.size())
				{
					CP_XML_ATTR(L"connection", m_arSXString[index]);
				}
				index++;
			}
		}
	}
	return 0;
}


} // namespace XLS


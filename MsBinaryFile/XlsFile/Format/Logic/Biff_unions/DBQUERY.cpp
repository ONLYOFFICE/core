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

#include "DBQUERY.h"
#include "DBQUERYEXT.h"

#include "../Biff_records/DbOrParamQry.h"
#include "../Biff_records/SXString.h"
#include "../Biff_records/TxtQry.h"
#include "../Biff_records/DBQueryExt.h"
#include "../Biff_records/ExtString.h"
#include "../Biff_records/DConn.h"
#include "../Biff_records/Qsi.h"

#include "../Biff_structures/DConnConnectionWeb.h"
#include "../Biff_structures/DConnConnectionOleDb.h"
#include "../Biff_structures/ConnGrbitDbtWeb.h"
#include "../Biff_structures/ConnGrbitDbtOledb.h"
#include "../Biff_structures/ConnGrbitDbtAdo.h"

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

int DBQUERY::serialize_connection(std::wstring & name)
{
	DbOrParamQry* dbQry = dynamic_cast<DbOrParamQry*>(m_DbQry.get());
	if (!dbQry) return -1;

	DConn* dcon = dynamic_cast<DConn*>(m_DConn.get());

	++global_info->connectionId;

	if (dcon && name.empty())
	{
		name = dcon->rgchConnectionName.strTotal;
	}
	//имена уникальны должны быть!!!
	if (name.empty())
		name = L"Connection" + std::to_wstring(global_info->connectionId);

	std::map<std::wstring, int>::iterator pFind = global_info->connectionNames.find(name);
	if (pFind == global_info->connectionNames.end())
	{
		global_info->connectionNames.insert(std::make_pair(name, 1));
	}
	else
	{
		name = name + L"_" + std::to_wstring(pFind->second);
		pFind->second++;
	}

	DBQUERYEXT	*query_ext	= dynamic_cast<DBQUERYEXT*>(m_DBQUERYEXT.get());	
	DBQueryExt	*dbQry_ext	= query_ext ? dynamic_cast<DBQueryExt*>(query_ext->m_DBQueryExt.get()) : NULL;
	Qsi			*info		= dynamic_cast<Qsi*>(m_Qsi.get());

	DConnConnectionWeb		*webDb = dcon ? dynamic_cast<DConnConnectionWeb*>(dcon->connection.get()) : NULL;
	DConnConnectionOleDb	*oleDb = dcon ? dynamic_cast<DConnConnectionOleDb*>(dcon->connection.get()) : NULL;
	XLUnicodeStringSegmented*adoDb = dcon ? dynamic_cast<XLUnicodeStringSegmented*>(dcon->connection.get()) : NULL;

	ConnGrbitDbtWeb		*webGrDb = dcon ? dynamic_cast<ConnGrbitDbtWeb*>(dcon->grbitDbt.get()) : NULL;
	ConnGrbitDbtOledb	*oleGrDb = dcon ? dynamic_cast<ConnGrbitDbtOledb*>(dcon->grbitDbt.get()) : NULL;
	ConnGrbitDbtAdo		*adoGrDb = dcon ? dynamic_cast<ConnGrbitDbtAdo*>(dcon->grbitDbt.get()) : NULL;

	CP_XML_WRITER(global_info->connections_stream)
	{
		CP_XML_NODE(L"connection")
		{
			CP_XML_ATTR(L"id", global_info->connectionId);	
			
			if ( dbQry->query.dbt == 5 )
			{
				CP_XML_ATTR(L"keepAlive", 1);
			}
			CP_XML_ATTR(L"name", name);
			
			if ((dcon) && (!dcon->rgchConnectionDesc.strTotal.empty()))
			{
				CP_XML_ATTR(L"description", dcon->rgchConnectionDesc.strTotal);
			}

			CP_XML_ATTR(L"type", dbQry->query.dbt);
			CP_XML_ATTR(L"refreshedVersion", 2);

			if (info)
			{
				CP_XML_ATTR(L"background", info->fAsync);
				CP_XML_ATTR(L"saveData", info->fSaveData);
			}

			if (dbQry->query.fSavePwd) CP_XML_ATTR(L"savePassword", 1);

			if (dbQry->typeRecord == 1)
			{
			}
			else
			{
				if ( dbQry->query.dbt == 6 )
				{
					TxtQry *query_txt = query_ext ? dynamic_cast<TxtQry*>(query_ext->m_TxtQry.get()) : NULL;

					if (query_txt)
					{
						CP_XML_NODE(L"textPr")
						{
							CP_XML_ATTR(L"codePage", 1148);
							CP_XML_ATTR(L"sourceFile", query_txt->rgchFile.value());
							CP_XML_ATTR(L"delimited", query_txt->fDelimited);
							wchar_t v = query_txt->chDecimal;
							CP_XML_ATTR(L"decimal", std::wstring(&v, 1));
							v = query_txt->chThousSep;
							CP_XML_ATTR(L"thousands", std::wstring(&v, 1));

							CP_XML_NODE(L"textFields")
							{
								CP_XML_ATTR(L"count", query_txt->rgtxtwf.size());

								for (size_t i = 0; i < query_txt->rgtxtwf.size(); i++)
								{
									CP_XML_NODE(L"textField")
									{
										switch(query_txt->rgtxtwf[i].fieldType)
										{
										//case 0:	CP_XML_ATTR(L"type", L"general");	break;
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
					size_t index = 0;
					size_t commandType = 1;

					CP_XML_NODE(L"dbPr")
					{
						std::wstring command, connection, post;
						size_t index_max = (std::min)((size_t )dbQry->query.cstQuery, m_arSXString.size());
						for (index = 0; index < index_max; index++)
						{
							command += m_arSXString[index];
						}
						index_max = (std::min)((size_t )(dbQry->query.cstQuery + dbQry->query.cstOdbcConn), m_arSXString.size());
						for (; index < index_max; index++)
						{
							connection += m_arSXString[index];
						}
						index_max = (std::min)((size_t )(dbQry->query.cstQuery + dbQry->query.cstOdbcConn + dbQry->query.cstWebPost), m_arSXString.size());

						for (;index < index_max; index++)
						{
							post += m_arSXString[index];
						}
						if (connection.empty() && (query_ext) && (!query_ext->m_arOleDbConn.empty()))
						{
							for (size_t i = 0; i < query_ext->m_arOleDbConn[0].arExtString.size(); i++)
							{
								ExtString* ex_str = dynamic_cast<ExtString*>(query_ext->m_arOleDbConn[0].arExtString[i].get());
								connection += ex_str->string.value();
							}
						}
						
						CP_XML_ATTR(L"connection", connection);
						CP_XML_ATTR(L"command", command);

						if (oleGrDb && commandType > 0)
						{
	//1. Query specifies a cube name
	//2. Query specifies a SQL statement
	//3. Query specifies a table name
	//4. Query specifies that default information has been given, and it is up to the provider
	//5. Query is against a web based List Data Provider.
							CP_XML_ATTR(L"commandType", oleGrDb->dbost);
						}
					}
					if (oleDb)
					{
						CP_XML_NODE(L"olapPr")
						{
							if (oleGrDb)
							{
								if (oleGrDb->fLocalConn)	
									CP_XML_ATTR(L"local",  oleGrDb->fLocalConn);
							}							
							if (oleDb->nDrillthroughRows > 0)
								CP_XML_ATTR(L"rowDrillCount", oleDb->nDrillthroughRows);
						}
					}
				}
			}
		}
	}
	return global_info->connectionId;
}


} // namespace XLS


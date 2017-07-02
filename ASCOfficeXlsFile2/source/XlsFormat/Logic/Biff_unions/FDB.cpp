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

#include "FDB.h"

#include "SXFMLA_bu.h"
#include "SXOPER.h"
#include "SXRANGE.h"

#include "../Biff_records/SXFDB.h"
#include "../Biff_records/SXFDBType.h"
#include "../Biff_records/SxIsxoper.h"

namespace XLS
{
//  (*GRPSXOPER [SXRANGE / *(SxIsxoper *Continue)])
class Parenthesis_FDB: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FDB)
public:
	Parenthesis_FDB(int count = 0)
	{
		m_count = count;
	}
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FDB(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if (m_count < 1) return false;

		int count = proc.repeated<SXOPER>(0, m_count);
		
		if (!proc.optional<SXRANGE>())
		{
			count = proc.repeated<SxIsxoper>(0, 0);
		}
		return true;
	};
private:
	int m_count;
};

FDB::FDB()
{
}


FDB::~FDB()
{
}


BaseObjectPtr FDB::clone()
{
	return BaseObjectPtr(new FDB(*this));
}

// FDB			= SXFDB SXFDBType [SXFMLA / (*GRPSXOPER [SXRANGE / *(SxIsxoper *Continue)])] *SRCSXOPER
// SXRANGE		= SXOPER
// SRCSXOPER	= SXOPER

const bool FDB::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXFDB>())
	{
		return false;
	}
	m_SXFDB = elements_.back();
	elements_.pop_back();
	
	SXFDB*	fdb	= dynamic_cast<SXFDB*>(m_SXFDB.get());

	if(proc.mandatory<SXFDBType>())
	{
		m_SXFDBType = elements_.back();
		elements_.pop_back();
	}

	if(proc.optional<SXFMLA>())
	{
		m_SXFMLA = elements_.back();
		elements_.pop_back();
	}
	else
	{
		Parenthesis_FDB parenthesis_FDB(fdb->csxoper);
		
		if (proc.optional(parenthesis_FDB))
		{
			int count = elements_.size();

			while(count > 0)
			{
				SXOPER	* oper	= dynamic_cast<SXOPER*>	(elements_.front().get());
				if (oper)
				{
					m_arGRPSXOPER.push_back(elements_.front());
				}
				else
				{
					SXRANGE *range = dynamic_cast<SXRANGE*>	(elements_.front().get());
					if (range)
						m_SXRANGE = elements_.front();
					else
					{
						SxIsxoper * isOper = dynamic_cast<SxIsxoper*> (elements_.front().get());
						if (isOper)
							m_arSxIsxoper.push_back(elements_.front());
					}
				}
				elements_.pop_front();
				count--;
			}
		}
	}
	
	int count = proc.repeated<SXOPER>(0, 0);
	while(count--)
	{
		m_arSRCSXOPER.push_back(elements_.front());	elements_.pop_front();
	}	

	return true;
}

int FDB::serialize(std::wostream & strm)
{
	SXFDB*		fdb			= dynamic_cast<SXFDB*>(m_SXFDB.get());
	SXFDBType*	fdb_type	= dynamic_cast<SXFDBType*>(m_SXFDBType.get());

	if (!fdb || !fdb_type) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"cacheField")
		{ 
			CP_XML_ATTR(L"name", fdb->stFieldName.value());
			CP_XML_ATTR(L"numFmtId", fdb_type->wTypeSql);	

			switch(fdb_type->wTypeSql)//format code
			{
			case 0x0000:
			case 0x0001:
			case 0x0003:
			case 0x0004:
			case 0x0005:
			case 0x0006:
			case 0x0007:
			case 0x0008:
			case 0x000B:
			case 0x000C:
			case 0xFFF9:
			case 0xFFFE:
				break;
			}
			if(m_SXFMLA)
			{
			//{formula
			}
			//caption, databaseFields,  ..

			if (m_arSRCSXOPER.empty() == false)
			{
				CP_XML_NODE(L"sharedItems")
				{
					//CP_XML_ATTR(L"containsSemiMixedTypes", 0);
					CP_XML_ATTR(L"containsNonDate", fdb->fNonDates);	
					CP_XML_ATTR(L"containsDate",	fdb->fDateInField);
					CP_XML_ATTR(L"containsNumber",	fdb->fNumField);
					CP_XML_ATTR(L"containsBlank",	fdb->fTextEtcField);
					//CP_XML_ATTR(L"containsString", 0);
					if (fdb->fnumMinMaxValid)
					{
						if (fdb->fDateInField)
						{
							CP_XML_ATTR(L"minDate", 0);				 // "2007-11-18T00:00:00" 
							CP_XML_ATTR(L"maxDate", 0);				 // "2007-12-25T00:00:00" 
						}
						else if (fdb->fNumField)
						{
							CP_XML_ATTR(L"minValue", 0);
							CP_XML_ATTR(L"maxValue", 0);
						}
					}
					CP_XML_ATTR(L"count", fdb->catm);	

					for (size_t i = 0; i < m_arSRCSXOPER.size(); i++)
					{
						m_arSRCSXOPER[i]->serialize(CP_XML_STREAM());
					}
				}
			}

			if (m_arGRPSXOPER.empty() == false)
			{
				CP_XML_NODE(L"fieldGroup")
				{
					if (fdb->ifdbParent > 0)
						CP_XML_ATTR(L"par", fdb->ifdbParent);	

					if (m_SXRANGE)
						m_SXRANGE->serialize(CP_XML_STREAM());
					CP_XML_NODE(L"groupItems")
					{
						CP_XML_ATTR(L"count", m_arGRPSXOPER.size());	
						for (size_t i = 0; i < m_arGRPSXOPER.size(); i++)
						{
							m_arGRPSXOPER[i]->serialize(CP_XML_STREAM());
						}
					}
				}
			}
		}
	}
	
	return 0;
}

} // namespace XLS


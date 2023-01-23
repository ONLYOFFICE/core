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
#include "Info.h"
#include "Utils.h"
#include "Objects.h"

#include <ctime>

static const char  *c_asInfoItemNames[] =
{
	"CreationDate",
	"ModDate",
	"Author",
	"Creator",
	"Producer",
	"Title",
	"Subject",
	"Keywords",
	NULL
};

namespace PdfWriter
{
	static const char* InfoTypeToName(EInfoType eType)
	{
		unsigned int nIndex = (unsigned int)eType;
		return c_asInfoItemNames[nIndex];
	}
	void TDate::AppendToString(std::string& s) const
	{
		s += std::to_string(nYear);
		if (nMonth < 10)
			s += "-0";
		else
			s += "-";
		s += std::to_string(nMonth);

		if (nDay < 10)
			s += "-0";
		else
			s +=  "-";

		s += std::to_string(nDay);

		if (nHour < 10)
			s += "T0";
		else
			s += "T";

		s += std::to_string(nHour);

		if (nMinutes < 10)
			s += ":0";
		else
			s += ":";

		s += std::to_string(nMinutes);

		if (nSeconds < 10)
			s += ":0";
		else
			s += ":";

		s += std::to_string(nSeconds);

		s += "+00:00";			 
	}
	//----------------------------------------------------------------------------------------
	// CInfoDict
	//----------------------------------------------------------------------------------------
	CInfoDict::CInfoDict(CXref* pXref)
	{
		pXref->Add(this);
	}
	void CInfoDict::SetInfo(EInfoType eType, const char* sValue)
	{
		const char* sName = InfoTypeToName(eType);

		if (eType <= InfoModaDate)
			return;

        Add(sName, new CStringObject(sValue, true));
	}
	const char* CInfoDict::GetInfo(EInfoType eType)
	{
		const char* sName = InfoTypeToName(eType);
		CObjectBase* pString = Get(std::string(sName));

		if (!pString)
			return NULL;

		if (object_type_STRING == pString->GetType())
			return (const char*)((CStringObject*)pString)->GetString();

		return NULL;
	}
	void CInfoDict::SetInfo(EInfoType eType, const TDate& oDate)
	{
		char sTemp[DATE_TIME_STR_LEN + 1];
		char* pTemp = NULL;
		const char* sName = InfoTypeToName(eType);

		if (eType > InfoModaDate)
			return;

		MemSet(sTemp, 0, DATE_TIME_STR_LEN + 1);

		if (oDate.nMonth < 1 
			|| 12 < oDate.nMonth 
			|| oDate.nDay < 1 
			|| 23 < oDate.nHour 
			|| 59 < oDate.nMinutes 
			|| 59 < oDate.nSeconds 
			|| 23 < oDate.nOffHour
			|| 59 < oDate.nOffMinutes)
		{
			return;
		}

		switch (oDate.nMonth)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if (oDate.nDay > 31)
				return;

			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if (oDate.nDay > 30)
				return;

			break;
		case 2:
			if (oDate.nDay > 29 || (oDate.nDay == 29 && (oDate.nYear % 4 != 0 || (oDate.nYear % 100 == 0 && oDate.nYear % 400 != 0))))
				return;

			break;
		default:
			return;
		}

		pTemp = (char*)MemCpy((BYTE*)sTemp, (BYTE*)"D:", 2); 
		*pTemp++; 
		*pTemp++;
		pTemp = ItoA2(pTemp, oDate.nYear, 5);
		pTemp = ItoA2(pTemp, oDate.nMonth, 3);
		pTemp = ItoA2(pTemp, oDate.nDay, 3);
		pTemp = ItoA2(pTemp, oDate.nHour, 3);
		pTemp = ItoA2(pTemp, oDate.nMinutes, 3);
		pTemp = ItoA2(pTemp, oDate.nSeconds, 3);
		*pTemp++ = '+';
		pTemp = ItoA2(pTemp, oDate.nOffHour, 3);
		*pTemp++ = '\'';
		pTemp = ItoA2(pTemp, oDate.nOffMinutes, 3);
		*pTemp++ = '\'';
		*pTemp = 0;

		Add(sName, new CStringObject(sTemp));
	}
	void CInfoDict::SetTime(EInfoType eType)
	{
		if (eType > InfoModaDate)
			return;

		time_t oTime = time(0);
		struct tm* oNow = gmtime(&oTime);

		TDate oDate;

		oDate.nYear       = oNow->tm_year + 1900;
		oDate.nMonth      = oNow->tm_mon + 1;
		oDate.nDay        = oNow->tm_mday;
		oDate.nHour       = oNow->tm_hour;
		oDate.nMinutes    = oNow->tm_min;
		oDate.nSeconds    = oNow->tm_sec;
		oDate.nOffHour    = 0;
		oDate.nOffMinutes = 0;

		SetInfo(eType == InfoCreationDate ? eType : InfoModaDate, oDate);

		m_oDate = oDate;
	}
	TDate CInfoDict::GetDate()
	{
		return m_oDate;
	}
}

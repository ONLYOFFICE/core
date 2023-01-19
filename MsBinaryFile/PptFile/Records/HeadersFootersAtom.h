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
#include "../Reader/Records.h"
#include "CString.h"

namespace PPT
{
class CRecordHeadersFootersAtom : public CUnknownRecord
{
public:
	WORD m_nFormatID;
	
    bool m_bHasDate;
	bool m_bHasTodayDate;
	bool m_bHasUserDate;
	bool m_bHasSlideNumber;
	bool m_bHasHeader;
    bool m_bHasFooter;


    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};


class CRecordRoundTripHeaderFooterDefaults12Atom : public CUnknownRecord
{
public:
	WORD m_nFormatID;
	
	bool m_bIncludeDate;
	bool m_bIncludeFooter;
	bool m_bIncludeHeader;
	bool m_bIncludeSlideNumber;


    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class CRecordMetaCharacterAtom : public CUnknownRecord
{
public:
    _UINT32 m_nPosition = -1;


    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class CRecordGenericDateMetaAtom : public CRecordMetaCharacterAtom
{
};

class CRecordRTFDateTimeMetaAtom : public CRecordGenericDateMetaAtom
{
public:
	std::string m_strFormat;
	

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream);
};

class CRecordDateTimeMetaAtom : public CRecordGenericDateMetaAtom
{
public:
	_UINT32 m_FormatID;

	
    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};


class CRecordFooterMetaAtom : public CRecordMetaCharacterAtom
{
};

class CRecordHeaderMetaAtom : public CRecordMetaCharacterAtom
{
};

class CRecordSlideNumberMetaAtom : public CRecordMetaCharacterAtom
{
};

class CRecordHeadersFootersContainer : public CRecordsContainer
{
public:
	vector_string m_HeadersFootersString[3]; //0-dates, 1 - headers, 2 - footers

    CRecordHeadersFootersAtom *m_oHeadersFootersAtom = nullptr;


    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};
}

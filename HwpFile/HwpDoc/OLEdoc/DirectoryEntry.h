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

#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H

#include "../Common/Common.h"

namespace HWP
{
class CDirectoryEntry
{
	HWP_STRING m_sDirectoryEntryName;
	int m_nObjectType;
	int m_nColorFlag;
	int m_nLeftSiblingID;
	int m_nRightSiblingID;
	int m_nChildID;
	long long m_lClsID1;
	long long m_lClsID2;
	int m_nStateBit;
	long long m_lCreationTime;
	long long m_lModifiedTime;
	int m_nStartingSectorID;
	long long m_lStreamSize;
	VECTOR<int> m_arSecNums;
public:
	CDirectoryEntry();

	void SetDirectoryEntryName(const HWP_STRING& sDirectoryEntryName);
	void SetObjectType(int nObjectType);
	void SetColorFlag(int nColorFlag);
	void SetLeftSiblingID(int nLeftSiblingID);
	void SetRightSiblingID(int nRightSiblingID);
	void SetChildID(int nChildID);
	void SetClsID1(const long long& lClsID1);
	void SetClsID2(const long long& lClsID2);
	void SetStateBit(int nStateBit);
	void SetCreationTime(const long long& lCreationTime);
	void SetModifiedTime(const long long& lModifiedTime);
	void SetStartingSectorID(int nStartingSectorID);
	void SetStreamSize(const long long& lStreamSize);

	int GetLeftSiblingID() const;
	int GetRightSiblingID() const;
	int GetChildID() const;
	int GetStartingSectorID() const;
	long long GetStreamSize() const;
	VECTOR<int> GetSecNums() const;

	VECTOR<int>& GetSecNums();

	int GetObjectType() const;
	HWP_STRING GetDirectoryEntryName() const;
};
}

#endif // DIRECTORYENTRY_H

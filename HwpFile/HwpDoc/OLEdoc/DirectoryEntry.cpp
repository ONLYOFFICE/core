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

#include "DirectoryEntry.h"

namespace HWP
{
HWP::CDirectoryEntry::CDirectoryEntry()
{}

void CDirectoryEntry::SetDirectoryEntryName(const HWP_STRING& sDirectoryEntryName)
{
	m_sDirectoryEntryName = sDirectoryEntryName;
}

void CDirectoryEntry::SetObjectType(int nObjectType)
{
	m_nObjectType = nObjectType;
}

void CDirectoryEntry::SetColorFlag(int nColorFlag)
{
	m_nColorFlag = nColorFlag;
}

void CDirectoryEntry::SetLeftSiblingID(int nLeftSiblingID)
{
	m_nLeftSiblingID = nLeftSiblingID;
}

void CDirectoryEntry::SetRightSiblingID(int nRightSiblingID)
{
	m_nRightSiblingID = nRightSiblingID;
}

void CDirectoryEntry::SetChildID(int nChildID)
{
	m_nChildID = nChildID;
}

void CDirectoryEntry::SetClsID1(const long long& lClsID1)
{
	m_lClsID1 = lClsID1;
}

void CDirectoryEntry::SetClsID2(const long long& lClsID2)
{
	m_lClsID2 = lClsID2;
}

void CDirectoryEntry::SetStateBit(int nStateBit)
{
	m_nStateBit = nStateBit;
}

void CDirectoryEntry::SetCreationTime(const long long& lCreationTime)
{
	m_lCreationTime = lCreationTime;
}

void CDirectoryEntry::SetModifiedTime(const long long& lModifiedTime)
{
	m_lModifiedTime = lModifiedTime;
}

void CDirectoryEntry::SetStartingSectorID(int nStartingSectorID)
{
	m_nStartingSectorID = nStartingSectorID;
}

void CDirectoryEntry::SetStreamSize(const long long& lStreamSize)
{
	m_lStreamSize = lStreamSize;
}

HWP_STRING CDirectoryEntry::GetDirectoryEntryName() const
{
	return m_sDirectoryEntryName;
}

int CDirectoryEntry::GetLeftSiblingID() const
{
	return m_nLeftSiblingID;
}

int CDirectoryEntry::GetRightSiblingID() const
{
	return m_nRightSiblingID;
}

int CDirectoryEntry::GetChildID() const
{
	return m_nChildID;
}

int CDirectoryEntry::GetStartingSectorID() const
{
	return m_nStartingSectorID;
}

long long CDirectoryEntry::GetStreamSize() const
{
	return m_lStreamSize;
}

VECTOR<int> CDirectoryEntry::GetSecNums() const
{
	return m_arSecNums;
}

VECTOR<int>& CDirectoryEntry::GetSecNums()
{
	return m_arSecNums;
}

int CDirectoryEntry::GetObjectType() const
{
	return m_nObjectType;
}
}

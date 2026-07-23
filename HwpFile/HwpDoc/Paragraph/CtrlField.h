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

#ifndef CTRLFIELD_H
#define CTRLFIELD_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
enum class EFieldType
{
	Hyperlink,
	HyperlinkClosing,
	Bookmark,
	BookmarkClosing,

	Unknown
};

class CCtrlField : public CCtrl
{
	int m_nProperty;
	HWP_BYTE m_chEtcProperty;
	HWP_STRING m_sCommand;
	int m_nInstanceID;

	EFieldType m_eType;

	std::map<HWP_STRING, HWP_STRING> m_mStringParams;
	std::map<HWP_STRING, int> m_mIntegerParam;

	void UpdateType(const HWP_STRING& sCtrlID);
	void ReadType(CXMLReader& oReader, EHanType eType);
public:
	CCtrlField(const HWP_STRING& sCtrlID);
	CCtrlField(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlField(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);

	ECtrlObjectType GetCtrlType() const override;

	void AddStringParam(const HWP_STRING& wsName, const HWP_STRING& wsValue);
	void AddIntegerParam(const HWP_STRING& wsName, int nValue);

	HWP_STRING GetStringParam(const HWP_STRING& wsName) const;
	int GetIntegerParam(const HWP_STRING& wsName) const;

	int GetInstanceID() const;
	EFieldType GetType() const;
	HWP_STRING GetCommand() const;
};
}

#endif // CTRLFIELD_H

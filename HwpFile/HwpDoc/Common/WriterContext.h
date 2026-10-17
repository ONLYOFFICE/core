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

#ifndef WRITERCONTEXT_H
#define WRITERCONTEXT_H

#include "../HanType.h"
#include "../HWPFile.h"
#include "../HWPXFile.h"
#include "../HWPMLFile.h"

#include "../HWPElements/HWPRecordParaShape.h"
#include "../HWPElements/HWPRecordStyle.h"
#include "../HWPElements/HWPRecordCharShape.h"
#include "../HWPElements/HWPRecordNumbering.h"
#include "../HWPElements/HWPRecordBullet.h"
#include "../HWPElements/HwpRecordTabDef.h"

namespace HWP
{
class CWriterContext
{
	EHanType    m_eType;
	CHWPFile*   m_pHWPFile;
	CHWPXFile*  m_pHWPXFile;
	CHWPMLFile* m_pHWPMLFile;
public:
	CWriterContext();
	~CWriterContext();

	void Clear();
	EHanType GetType();

	VECTOR<const CHWPSection*> GetSections();
	static EHanType DetectHancom(const HWP_STRING& sPathToFile);
	bool Detect();
	bool Open(const HWP_STRING& sPathToFile, EHanType eHanType, const HWP_STRING& sPassword = HWP_STRING());
	void Close();

	bool Encrypted();
	EHwpxCryptResult GetCryptResult();

	const CHWPDocInfo* GetDocInfo();
	const CHWPRecordBorderFill* GetBorderFill(short shId);
	const CHWPRecordParaShape* GetParaShape(int nId);
	const CHWPRecordStyle* GetParaStyle(short shId);
	const CHWPRecordCharShape* GetCharShape(int nId);
	const CHWPRecordNumbering* GetNumbering(short shId);
	const CHWPRecordBullet* GetBullet(short shId);
	const CHwpRecordTabDef* GetTabDef(short shId);

	HWP_STRING GetBinFilename(const HWP_STRING& sId);
	bool GetBinBytes(const HWP_STRING& sId, CHWPStream& oBuffer, HWP_STRING& sFileName);
	HWP_STRING GetBinFormat(const HWP_STRING& sId);
};
}

#endif // WRITERCONTEXT_H

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
#pragma once

#include "BiffRecordContinued.h"
#include "MsoDrawing.h"
#include "../Biff_structures/ControlInfo.h"
#include "../Biff_structures/FontIndex.h"
#include "../Biff_structures/ObjFmla.h"
#include "../Biff_structures/TxORuns.h"

namespace XLS
{

class TxO: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TxO)
	BASE_OBJECT_DEFINE_CLASS_NAME(TxO)
public:
	TxO(MsoDrawingPtr mso_drawing) : mso_drawing_(mso_drawing), fmla(false)
	{
		macrosSize			= 0;
		nameLength			= 0;
		cbRuns				= 0;
		sp_enabled			= false;
		preserve_enabled	= false;
	}
	~TxO();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
	void writeFields(CFRecord& record);

	static const ElementType	type = typeTxO;

	int serialize		(std::wostream & _stream);	
	int serialize_rPr	(std::wostream & _stream, int iFmt, std::wstring namespace_= L"a:");
	int serialize_vml	(std::wostream & _stream);	

	GlobalWorkbookInfoPtr	global_info;

	unsigned char			hAlignment = 1;
	unsigned char			vAlignment = 1;

	unsigned char			fLockText = 0;
	unsigned char			fJustLast = 0;
	unsigned char			fSecretEdit = 0;

	_UINT16					rot = 0;

	ControlInfo				controlInfo;
	bool					fcontrolInfoExist = false;

	_UINT16					cchText = 0;
	_UINT16					cbRuns = 0;
	FontIndex				ifntEmpty;
	ObjFmla					fmla;
	TxORuns					TxOruns;
	XLUnicodeStringNoCch	rawText;

	bool					sp_enabled;

	MsoDrawingPtr			mso_drawing_;


//---------------------------------------------
	bool					preserve_enabled;
	short					macrosSize;
	short					nameLength;
	ShortXLAnsiString		name;
};

} // namespace XLS


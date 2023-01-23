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
		hAlignment			= 0;
		vAlignment			= 0;
	}
	~TxO();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);

	static const ElementType	type = typeTxO;

	int serialize		(std::wostream & _stream);	
	int serialize_rPr	(std::wostream & _stream, int iFmt, std::wstring namespace_= L"a:");
	int serialize_vml	(std::wostream & _stream);	

	GlobalWorkbookInfoPtr	global_info;

	unsigned char			hAlignment;
	unsigned char			vAlignment;

	unsigned char			fLockText;
	unsigned char			fJustLast;
	unsigned char			fSecretEdit;

	_UINT16					rot;

	ControlInfo				controlInfo;

	_UINT16					cchText;
	_UINT16					cbRuns;
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


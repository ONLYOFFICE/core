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

#include "BiffRecord.h"
#include "../Biff_structures/Xnum.h"

namespace XLS
{

class Setup: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Setup)
	BASE_OBJECT_DEFINE_CLASS_NAME(Setup)
public:
	Setup(bool isChart = false);
	~Setup();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);

	static const ElementType type = typeSetup;

	_UINT32 iPaperSize = 0;
	_UINT32 iScale = 100;
	_INT32 iPageStart = 1;
	_UINT32 iFitWidth = 0;
	_UINT32 iFitHeight = 0;

	bool fLeftToRight = false;
	bool fPortrait = false;
	bool fNoPls = false;
	bool fNoColor = false;
	bool fDraft = false;
	bool fNotes = false;
	bool fNoOrient = false;
	bool fUsePage = false;
	bool fEndNotes = false;
	unsigned char iErrors = 0;

	_UINT32 iRes = 0;
	_UINT32 iVRes = 0;

	Xnum	numHdr;
	Xnum	numFtr;
	_UINT32 iCopies = 1;

	std::wstring szRelID; // biff12
	bool fLandscape; // biff12

	bool _isChart;
};

} // namespace XLS


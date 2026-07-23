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

#include "TEXTOBJECT.h"
#include "../Biff_records/TxO.h"
#include "../Biff_records/Continue.h"
#include "../Biff_records/MsoDrawing.h"

namespace XLS
{


TEXTOBJECT::~TEXTOBJECT()
{
}


BaseObjectPtr TEXTOBJECT::clone()
{
	return BaseObjectPtr(new TEXTOBJECT(*this));
}


// TEXTOBJECT = TxO *Continue
const bool TEXTOBJECT::loadContent(BinProcessor& proc)
{
	TxO TxO_(mso_drawing_);
	if(!proc.mandatory(TxO_))
	{
		return false;
	}
	m_TxO = elements_.back();
	elements_.pop_back();
	//proc.repeated<Continue>(0, 0);

	return true;
}

const bool TEXTOBJECT::saveContent(BinProcessor& proc)
{
	if(m_TxO == nullptr)
		return false;
	proc.mandatory(*m_TxO);
	
	auto castedObj = static_cast<TxO*>(m_TxO.get());
	if(castedObj->cchText > 0)
	{
		auto textPointer = 0;
		{
			auto fragmentSize = (castedObj->cchText * 2) + 1;
			{
				XLS::Continue continueRec;
				if(fragmentSize > 8224)
					fragmentSize = 8224;
				continueRec.m_iDataSize = fragmentSize;
				continueRec.m_pData = new char[fragmentSize];
				continueRec.m_pData[0] = 1; // wchar byte
				memcpy(continueRec.m_pData+1, castedObj->rawText.value().data(), fragmentSize -1);
				proc.mandatory(continueRec);
			}
			textPointer+= (fragmentSize-1) / 2;
		}

		// if cchText is too large for 1 continue
		while(textPointer < castedObj->cchText)
		{
			auto fragmentSize = (castedObj->cchText - textPointer)*2;
			{
				XLS::Continue continueRec;
				if(fragmentSize > 8224)
					fragmentSize = 8224;
				continueRec.m_iDataSize = fragmentSize;
				continueRec.m_pData = new char[fragmentSize];
				memcpy(continueRec.m_pData, castedObj->rawText.value().data() + textPointer, fragmentSize);
				proc.mandatory(continueRec);
			}
			textPointer+= fragmentSize / 2;
		}
	}
	if(castedObj->cbRuns > 0)
	{
		unsigned int runsCounter = 0;
		while(runsCounter < castedObj->TxOruns.rgTxoRuns.size())
		{
			auto fragmentSize = 0;
			if((castedObj->TxOruns.rgTxoRuns.size() - runsCounter + 1) * 8 > 8224)
				fragmentSize = 1028;
			else
				break;
			XLS::Continue continueRec;
			continueRec.m_iDataSize = fragmentSize;
			continueRec.m_pData = new char[fragmentSize];
			CFRecord tempRec(rt_Continue, proc.getGlobalWorkbookInfo());
			for(auto i = runsCounter; i <  (runsCounter + fragmentSize/8); i++)
			{
				castedObj->TxOruns.rgTxoRuns[i]->save(tempRec);
			}

			auto copyData = tempRec.getCurStaticData<char>() - tempRec.getRdPtr();
			memcpy(continueRec.m_pData, copyData, fragmentSize);
			proc.mandatory(continueRec);
			runsCounter += fragmentSize/8;
		}
		{
			auto fragmentSize = (castedObj->TxOruns.rgTxoRuns.size() - runsCounter +1) * 8;
			XLS::Continue continueRec;
			continueRec.m_iDataSize = fragmentSize;
			continueRec.m_pData = new char[fragmentSize];
			CFRecord tempRec(rt_Continue, proc.getGlobalWorkbookInfo());
			for(auto i = runsCounter; i <  castedObj->TxOruns.rgTxoRuns.size(); i++)
			{
				castedObj->TxOruns.rgTxoRuns[i]->save(tempRec);
			}
			castedObj->TxOruns.lastRun.save(tempRec);
			auto copyData = tempRec.getCurStaticData<char>() - tempRec.getRdPtr();
			memcpy(continueRec.m_pData, copyData, fragmentSize);
			proc.mandatory(continueRec);
		}
	}
	return true;
}

} // namespace XLS


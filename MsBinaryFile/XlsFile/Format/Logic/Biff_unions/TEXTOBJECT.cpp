/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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


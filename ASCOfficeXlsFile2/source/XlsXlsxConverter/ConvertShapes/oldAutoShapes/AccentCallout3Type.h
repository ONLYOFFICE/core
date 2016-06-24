/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../CustomShape.h"

// 46
namespace NSCustomShapesConvert
{

class CAccentCallout3Type : public CCustomShape
{
public:
	CAccentCallout3Type()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Encaps: Flat

        m_strPath = _T("m@0@1l@2@3@4@5@6@7nfem@6,l@6,21600nfem,l21600,r,21600l,21600nsxe");

		AddGuide(_T("val #0"));  
		AddGuide(_T("val #1"));
		AddGuide(_T("val #2"));  
		AddGuide(_T("val #3"));
		AddGuide(_T("val #4"));  
		AddGuide(_T("val #5"));
		AddGuide(_T("val #6"));  
		AddGuide(_T("val #7")); 

        m_arAdjustments.push_back(23400);
		m_arAdjustments.push_back(24400);
		m_arAdjustments.push_back(25200);
		m_arAdjustments.push_back(21600);
		m_arAdjustments.push_back(25200);
		m_arAdjustments.push_back(4050);
		m_arAdjustments.push_back(23400);
		m_arAdjustments.push_back(4050);

        LoadConnectorsList(_T("@0,@1;10800,0;10800,21600;0,10800;21600,10800"));

		CHandle_ oHandle1;
		oHandle1.position = _T("#0,#1");
        m_arHandles.push_back(oHandle1);

		CHandle_ oHandle2;
		oHandle2.position = _T("#2,#3");
        m_arHandles.push_back(oHandle2);

		CHandle_ oHandle3;
		oHandle3.position = _T("#4,#5");
        m_arHandles.push_back(oHandle3);

		CHandle_ oHandle4;
		oHandle4.position = _T("#6,#7");
        m_arHandles.push_back(oHandle4);
    }
};
}
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

// 98
namespace NSCustomShapesConvert
{
class CHorizontalScrollType : public CCustomShape
{
public:
	CHorizontalScrollType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Encaps: Flat

        m_strPath = _T("m0@5qy@2@1l@0@1@0@2qy@7,,21600@2l21600@9qy@7@10l@1@10@1@11qy@2,21600,0@11xem0@5nfqy@2@6@1@5@3@4@2@5l@2@6em@1@5nfl@1@10em21600@2nfqy@7@1l@0@1em@0@2nfqy@8@3@7@2l@7@1e");

		AddGuide(_T("sum width 0 #0"));  
		AddGuide(_T("val #0"));  
		AddGuide(_T("prod @1 1 2"));  
		AddGuide(_T("prod @1 3 4"));  
		AddGuide(_T("prod @1 5 4")); 
		AddGuide(_T("prod @1 3 2"));  
		AddGuide(_T("prod @1 2 1"));  
		AddGuide(_T("sum width 0 @2"));  
		AddGuide(_T("sum width 0 @3"));  
		AddGuide(_T("sum height 0 @5"));  
		AddGuide(_T("sum height 0 @1"));  
		AddGuide(_T("sum height 0 @2"));  
		AddGuide(_T("val width"));  
		AddGuide(_T("prod width 1 2"));  
		AddGuide(_T("prod height 1 2"));

        m_arAdjustments.push_back(2700);

        LoadConnectorsList(_T("@13,@1;0,@14;@13,@10;@12,@14"));
        LoadTextRect(_T("@1,@1,@7,@10"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

		CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.yrange = _T("0,5400");
        m_arHandles.push_back(oHandle1);

		m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};
}
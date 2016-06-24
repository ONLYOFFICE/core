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

// 63
namespace NSCustomShapesConvert
{

class CWedgeEllipseCalloutType : public CCustomShape
{
public:
	CWedgeEllipseCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("wr,,21600,21600@15@16@17@18l@21@22xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("sum 10800 0 #0"));
        AddGuide(_T("sum 10800 0 #1"));
        AddGuide(_T("atan2 @2 @3"));
        AddGuide(_T("sumangle @4 11 0"));
        AddGuide(_T("sumangle @4 0 11"));
        AddGuide(_T("cos 10800 @4"));
        AddGuide(_T("sin 10800 @4"));
        AddGuide(_T("cos 10800 @5"));
        AddGuide(_T("sin 10800 @5"));
        AddGuide(_T("cos 10800 @6"));
        AddGuide(_T("sin 10800 @6"));
        AddGuide(_T("sum 10800 0 @7"));
        AddGuide(_T("sum 10800 0 @8"));
        AddGuide(_T("sum 10800 0 @9"));
        AddGuide(_T("sum 10800 0 @10"));
        AddGuide(_T("sum 10800 0 @11"));
        AddGuide(_T("sum 10800 0 @12"));
        AddGuide(_T("mod @2 @3 0"));
        AddGuide(_T("sum @19 0 10800"));
        AddGuide(_T("if @20 #0 @13"));
        AddGuide(_T("if @20 #1 @14"));

		m_arAdjustments.push_back(1350);
		m_arAdjustments.push_back(25920);

        LoadConnectorsList(_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163;@21,@22"));
        LoadTextRect(_T("3163,3163,18437,18437"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,#1");
        m_arHandles.push_back(oHandle1);
    }
};
}
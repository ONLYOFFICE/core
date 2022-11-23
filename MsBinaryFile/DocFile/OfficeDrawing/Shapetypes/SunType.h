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

#include "../ShapeType.h"

namespace DocFileFormat
{
	class SunType : public ShapeType
	{
	public:
		SunType () : ShapeType(msosptSun)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m21600,10800l@15@14@15@18xem18436,3163l@17@12@16@13xem10800,l@14@10@18@10xem3163,3163l@12@13@13@12xem,10800l@10@18@10@14xem3163,18436l@13@16@12@17xem10800,21600l@18@15@14@15xem18436,18436l@16@17@17@16xem10800@19qx@19,10800,10800@20@20,10800,10800@19xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"prod @0 30274 32768");
            Formulas.push_back(L"prod @0 12540 32768");
            Formulas.push_back(L"sum @1 10800 0");
            Formulas.push_back(L"sum @2 10800 0");
            Formulas.push_back(L"sum 10800 0 @1");
            Formulas.push_back(L"sum 10800 0 @2");
            Formulas.push_back(L"prod @0 23170 32768");
            Formulas.push_back(L"sum @7 10800 0");
            Formulas.push_back(L"sum 10800 0 @7");
            Formulas.push_back(L"prod @5 3 4");
            Formulas.push_back(L"prod @6 3 4");
            Formulas.push_back(L"sum @10 791 0");
            Formulas.push_back(L"sum @11 791 0");
            Formulas.push_back(L"sum @11 2700 0");
            Formulas.push_back(L"sum 21600 0 @10");
            Formulas.push_back(L"sum 21600 0 @12");
            Formulas.push_back(L"sum 21600 0 @13");
            Formulas.push_back(L"sum 21600 0 @14");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");

            AdjustmentValues	=	L"5400";
            ConnectorLocations	=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
            TextBoxRectangle	=	L"@9,@9,@8,@8";

			Handle one;
            one.position		=	L"#0,center";
            one.xrange			=	L"2700,10125";
			Handles.push_back (one);
		}
	};
}

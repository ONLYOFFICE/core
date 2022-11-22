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
	class BentConnector2: public ShapeType
	{
	public:
		BentConnector2() : ShapeType(msosptBentConnector2)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,l21600,r,21600e";
            //AdjustmentValues	=	L"10800";

			Filled				=	true;
		}
	};

	class BentConnector3: public ShapeType
	{
	public:
		BentConnector3() : ShapeType(msosptBentConnector3)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,l@0,0@0,21600,21600,21600e";
            AdjustmentValues	=	L"10800";

			Filled				=	true;

            Formulas.push_back(L"val #0");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};

	class BentConnector4: public ShapeType
	{
	public:
		BentConnector4() : ShapeType(msosptBentConnector4)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,l@0,0@0,21600,21600,21600e";
            AdjustmentValues	=	L"10800";

			Filled				=	true;

            Formulas.push_back(L"val #0");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};

	class BentConnector5 : public ShapeType
	{
	public:
		BentConnector5() : ShapeType(msosptBentConnector5)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,l@0,0@0,21600,21600,21600e";
            AdjustmentValues	=	L"10800";

			Filled				=	true;

            Formulas.push_back(L"val #0");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};

	
	class CurvedConnector2: public ShapeType
	{
	public:
		CurvedConnector2() : ShapeType(msosptCurvedConnector2)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e";

			Filled				=	true;

            Formulas.push_back(L"mid #0 0");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"mid #0 21600");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};
	class CurvedConnector3: public ShapeType
	{
	public:
		CurvedConnector3() : ShapeType(msosptCurvedConnector3)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e";

			Filled				=	true;

            Formulas.push_back(L"mid #0 0");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"mid #0 21600");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};
	class CurvedConnector4 : public ShapeType
	{
	public:
		CurvedConnector4() : ShapeType(msosptCurvedConnector4)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e";

			Filled				=	true;

            Formulas.push_back(L"mid #0 0");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"mid #0 21600");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};
	class CurvedConnector5 : public ShapeType
	{
	public:
		CurvedConnector5() : ShapeType(msosptCurvedConnector5)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e";

			Filled				=	true;

            Formulas.push_back(L"mid #0 0");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"mid #0 21600");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};
}

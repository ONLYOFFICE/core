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

#include "../ShapeType.h"

namespace DocFileFormat
{
	class StraightConnector: public ShapeType
	{
	public:
		StraightConnector() : ShapeType(msosptStraightConnector1)
		{
			Path				=	L"m,l21600,21600e";
			ConnectorLocations	=	L"0,0;21600,21600";
		}
	};

	class BentConnector: public ShapeType
	{
	public:
		BentConnector() : ShapeType(msosptBentConnector2)
		{
			ShapeConcentricFill =	true;
			Joins				=	round;
			Path				=	L"m,l@0,0@0,21600,21600,21600e";

			Formulas.push_back(L"val #0");

			Adjustments.push_back(10800);

			ConnectorLocations	=	L"0,0;21600,21600";
		}
	};

	class BentConnector2: public ShapeType
	{
	public:
		BentConnector2() : ShapeType(msosptBentConnector2)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
            Path				=	L"m,l21600,r,21600e";

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
			Filled				=	true;

            Path				=	L"m,l@0,0@0,21600,21600,21600e";
			
			Adjustments.push_back(10800);

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
			Filled				=	true;
           
			Path				=	L"m,l@0,0@0,21600,21600,21600e";
          
			Adjustments.push_back(10800);
			
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
			Filled				=	true;
            
			Path				=	L"m,l@0,0@0,21600,21600,21600e";
			
			Adjustments.push_back(10800);

            Formulas.push_back(L"val #0");

			Handle one;
            one.position		=	L"#0,center";
			Handles.push_back (one);
		}
	};

	class CurvedConnector: public ShapeType
	{
	public:
		CurvedConnector() : ShapeType(msosptCurvedConnector2)
		{
			ShapeConcentricFill =	true;
			Joins				=	round;
			
			Path				=	L"m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e";

			Formulas.push_back(L"mid #0 0");
			Formulas.push_back(L"val #0");
			Formulas.push_back(L"mid #0 21600");

			Adjustments.push_back(10800);

			ConnectorLocations	=	L"0,0;21600,21600";
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

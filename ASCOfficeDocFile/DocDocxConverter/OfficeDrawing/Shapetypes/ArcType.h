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

#include "../ShapeType.h"

namespace DocFileFormat
{
	class ArcType : public ShapeType
	{
	public:
		ArcType () : ShapeType(msosptArc)
		{
			ShapeConcentricFill		=	true;
			Joins					=	round;
			Path					=	_T( "wr-21600,,21600,43200,,,21600,21600nfewr-21600,,21600,43200,,,21600,21600l,21600nsxe" );

			Formulas.push_back( _T( "val #2" ) );
			Formulas.push_back( _T( "val #3" ) );
			Formulas.push_back( _T( "val #4" ) );

			AdjustmentValues		=	_T( "-5898240,,,21600,21600" );
			ConnectorLocations		=	_T( "0,0;21600,21600;0,21600" );

			Handle HandleOne;
			HandleOne.position		=	_T( "@2,#0" );
			HandleOne.polar			=	_T( "@0,@1" ); 
			Handles.push_back( HandleOne );

			Handle HandleTwo;
			HandleOne.position		=	_T( "@2,#1" );
			HandleOne.polar			=	_T( "@0,@1" ); 
			Handles.push_back( HandleTwo );
		}
	};
}
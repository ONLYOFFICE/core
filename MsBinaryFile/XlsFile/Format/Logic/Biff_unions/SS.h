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

#include "../CompositeObject.h"

#define CHART_TYPE_Bar			1
#define CHART_TYPE_Line			2
#define CHART_TYPE_Pie			3
#define CHART_TYPE_Area			4
#define CHART_TYPE_Surf			5		
#define CHART_TYPE_Radar		6
#define CHART_TYPE_RadarArea	7
#define CHART_TYPE_BopPop		8		
#define CHART_TYPE_Scatter		9
#define CHART_TYPE_Bubble		10
#define CHART_TYPE_Stock		11
#define CHART_TYPE_Doughnut		12

namespace XLS
{


// Logical representation of SS union of records 
class SS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SS)
public:
	SS();
	~SS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
	virtual const bool saveContent(BinProcessor& proc);

	static const ElementType	type = typeSS;

	void apply_crt_ss		(BaseObjectPtr crt_ss);

	int serialize			(std::wostream & _stream, int series_type, int indPt);
	int serialize_default	(std::wostream & _stream, int series_type, int ind);
	int serialize2			(std::wostream & _stream, int series_type);
	
	BaseObjectPtr	m_DataFormat;
	
	BaseObjectPtr	m_Chart3DBarShape;
	BaseObjectPtr	m_LineFormat;
	BaseObjectPtr	m_AreaFormat;
	BaseObjectPtr	m_PieFormat;

	BaseObjectPtr	m_SerFmt;
	BaseObjectPtr	m_GELFRAME;
	BaseObjectPtr	m_MarkerFormat;
	BaseObjectPtr	m_AttachedLabel;
	
	std::vector<BaseObjectPtr >	m_arSHAPEPROPS;

//---------------------------------------------------

	bool			m_is3D;
	_CP_OPT(bool)	m_isVaried;

	bool			m_isAutoFill;
	bool			m_isAutoLine;	
};

} // namespace XLS


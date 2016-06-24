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

#include <Logic/CompositeObject.h>

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

	static const ElementType	type = typeSS;

	void apply_crt_ss		(BaseObjectPtr crt_ss);

	int serialize			(std::wostream & _stream, int series_type, int ind = -1);
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


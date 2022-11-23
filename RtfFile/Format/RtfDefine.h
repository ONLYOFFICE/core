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

const long g_cdMaxPercent = 1000000;

#ifdef INT_MIN
    #define PROP_DEF INT_MIN
#else
    #define PROP_DEF (-0x7FFFFFFF-1)
#endif

#define RTF_MAX 1
#define RTF_MID 2
#define RTF_LOW 3

//#define RTF_MATH_OOX

#define ELEMENT_UNDEFINED	0x001;
#define ELEMENT_OOXFIELD	0x010;

#define TYPE_UNKNOWN							0
#define TYPE_RTF_DOCUMENT						1
#define TYPE_RTF_PARAGRAPH						2
#define TYPE_RTF_BOOKMARKSTART					3
#define TYPE_RTF_BOOKMARKEND					4
#define TYPE_RTF_CHAR							5
#define TYPE_RTF_FIELD							6
#define TYPE_RTF_MATH							7
#define TYPE_RTF_PICTURE						8
#define TYPE_RTF_PROPERTY_DOCUMENT				9
#define TYPE_RTF_PROPERTY_SECTION				10
#define TYPE_RTF_FONT							11
#define TYPE_RTF_PROPERTY_SHADING				13
#define TYPE_RTF_PROPERTY_SHADING_PARAGRAPH		14
#define TYPE_RTF_PROPERTY_SHADING_CHAR			15
#define TYPE_RTF_PROPERTY_SHADING_CELL			16
#define TYPE_RTF_PROPERTY_SHADING_ROW			17
#define TYPE_RTF_PROPERTY_SHADING_TABLESTYLE	18
#define TYPE_RTF_PROPERTY_BORDER				19
#define TYPE_RTF_PROPERTY_STYLE_CHAR			21
#define TYPE_RTF_PROPERTY_STYLE_PARAGRAPH		22
#define TYPE_RTF_PROPERTY_STYLE_TABLE			22
#define TYPE_RTF_SECTION						23
#define TYPE_RTF_TABLE							24
#define TYPE_RTF_TABLE_CELL						25
#define TYPE_RTF_PROPERTY_CHAR					26
#define TYPE_RTF_PROPERTY_STYLE					27
#define TYPE_RTF_TABLE_ROW						28
#define TYPE_RTF_OLE							29

#define TYPE_OOX_FIELD							29

#define TYPE_RTF_FOOTNOTE						30

#define TYPE_RTF_ANNOTSTART						31
#define TYPE_RTF_ANNOTEND						32
#define TYPE_RTF_ANNOTVALUE						33
#define TYPE_RTF_ANNOTATION						34

#define TYPE_OOX_FORMFIELD						35

#define RENDER_TO_OOX_PARAM_UNKNOWN				0
#define RENDER_TO_OOX_PARAM_LAST				1
#define RENDER_TO_OOX_PARAM_RUN					2
#define RENDER_TO_OOX_PARAM_PLAIN				3
#define RENDER_TO_OOX_PARAM_TEXT				4
#define RENDER_TO_OOX_PARAM_MATH				5
#define RENDER_TO_OOX_PARAM_NESTED				6
#define RENDER_TO_OOX_PARAM_FONTTABLE			7
#define RENDER_TO_OOX_PARAM_MAJOR_FONT			10
#define RENDER_TO_OOX_PARAM_MINOR_FONT			11
#define RENDER_TO_OOX_PARAM_CORE				12
#define RENDER_TO_OOX_PARAM_APP					13
#define RENDER_TO_OOX_PARAM_COLOR_VALUE			14
#define RENDER_TO_OOX_PARAM_COLOR_ATTRIBUTE		15
#define RENDER_TO_OOX_PARAM_COLOR_TAG			16
#define RENDER_TO_OOX_PARAM_NUMBERING			17
#define RENDER_TO_OOX_PARAM_STYLES				18
#define RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE	19
#define RENDER_TO_OOX_PARAM_BORDER_TAG			20
#define RENDER_TO_OOX_PARAM_SHAPE_WSHAPE		22
#define RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2		23
#define RENDER_TO_OOX_PARAM_FIRST_SECTION		24
#define RENDER_TO_OOX_PARAM_OLE_ONLY			25
#define RENDER_TO_OOX_PARAM_OLDLIST_ABS			26
#define RENDER_TO_OOX_PARAM_OLDLIST_OVR			27
#define RENDER_TO_OOX_PARAM_COMMENT				28

#define RENDER_TO_RTF_PARAM_UNKNOWN				0
#define RENDER_TO_RTF_PARAM_CHAR				1
#define RENDER_TO_RTF_PARAM_NO_PAR				2
#define RENDER_TO_RTF_PARAM_PAR					3
#define RENDER_TO_RTF_PARAM_NESTED				4
#define RENDER_TO_RTF_PARAM_PLAIN				5
#define RENDER_TO_RTF_PARAM_NO_SECT				6
#define RENDER_TO_RTF_PARAM_FONT_TBL			7
#define RENDER_TO_RTF_PARAM_COLOR_TBL			8
#define RENDER_TO_RTF_PARAM_NO_WROWD			9





















































































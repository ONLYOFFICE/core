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

namespace VBA
{
	typedef enum CF_RECORD_TYPE
	{
		rt_PROJECTSYSKIND = 0x0001,
		rt_PROJECTCOMPATVERSION = 0x004A,
		rt_PROJECTLCID = 0x0002,
		rt_PROJECTLCIDINVOKE = 0x0014,
		rt_PROJECTCODEPAGE = 0x0003,
		rt_PROJECTNAME = 0x0004,
		rt_PROJECTDOCSTRING = 0x0005,
		rt_PROJECTHELPFILEPATH = 0x0006,
		rt_PROJECTHELPCONTEXT = 0x0007,
		rt_PROJECTLIBFLAGS = 0x0008,
		rt_PROJECTVERSION = 0x0009,
		rt_PROJECTCONSTANTS = 0x000C,
		rt_REFERENCENAME = 0x0016,
		rt_REFERENCEORIGINAL = 0x0033,
		rt_REFERENCEREGISTERED = 0x000D,
		rt_REFERENCEPROJECT = 0x000E,
		rt_REFERENCECONTROL = 0x002F,
		rt_PROJECTMODULES = 0x000F,
		rt_PROJECTCOOKIE = 0x0013,
		rt_MODULENAME = 0x0019,
		rt_MODULENAMEUNICODE = 0x0047,
		rt_MODULESTREAMNAME = 0x001A,
		rt_MODULEDOCSTRING = 0x001C,
		rt_MODULEOFFSET = 0x0031,
		rt_MODULEHELPCONTEXT = 0x001E,
		rt_MODULECOOKIE = 0x002C,
		rt_MODULETYPE_Procedural = 0x0021,
		rt_MODULETYPE_Class = 0x0022,
		rt_MODULEREADONLY = 0x0025,
		rt_MODULEPRIVATE = 0x0028,
		rt_Terminator = 0x002B,
		rt_TerminatorStream = 0x0010,

		rt_PROJECTINFORMATION = 0xFF01,
		rt_REFERENCE,
		rt_MODULE,
		rt_MODULETYPE,
		rt_AnsiString,
		rt_Utf16String,
		rt_FormControl,
		rt_StdFont,
		rt_TextProps,
		rt_FormSiteData,
		rt_FormDesignExData,
		rt_SiteClassInfo,
		rt_OleSiteConcreteControl,
		rt_FormObjectDepthTypeCount,

		rt_Unknown = 0xffff
	} RecordType;
}


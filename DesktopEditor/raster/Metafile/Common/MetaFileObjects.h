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
#ifndef _METAFILE_COMMON_METAFILEOBJECTS_H
#define _METAFILE_COMMON_METAFILEOBJECTS_H

#include <string>

namespace MetaFile
{
	class IFont
	{
	public:
		IFont(){}
		virtual ~IFont(){}

		virtual int          GetHeight() = 0;
		virtual std::wstring GetFaceName() = 0;
		virtual int          GetWeight() = 0;
		virtual bool         IsItalic() = 0;
		virtual bool         IsStrikeOut() = 0;
		virtual bool         IsUnderline() = 0;
		virtual int          GetEscapement() = 0;
		virtual int          GetCharSet() = 0;
		virtual int          GetOrientation() = 0;
	};

	class IBrush
	{
	public:
		IBrush(){}
		virtual ~IBrush(){}

		virtual int          GetColor()			= 0;
		virtual int          GetColor2()		= 0;
		virtual unsigned int GetStyle()			= 0;
		virtual unsigned int GetStyleEx()		= 0;
		virtual unsigned int GetHatch()			= 0;
		virtual unsigned int GetAlpha()			= 0;
		virtual unsigned int GetAlpha2()		= 0;
		virtual std::wstring GetDibPatterPath() = 0;
		virtual void GetBounds(double& left, double& top, double& width, double& height) = 0;
	};
	class IPen
	{
	public:
		IPen(){}
		virtual ~IPen(){}

		virtual int          GetColor() = 0;
		virtual unsigned int GetStyle() = 0;
		virtual unsigned int GetWidth() = 0;
	};
}

#endif //_METAFILE_COMMON_METAFILEOBJECTS_H

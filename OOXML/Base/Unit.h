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

#include "Base.h"
#include "../../DesktopEditor/common/Types.h"

double Cm_To_Mm(const double &dValue);
double Cm_To_Pt(const double &dValue);
double Cm_To_Px(const double &dValue);
double Cm_To_Inch(const double &dValue);
double Cm_To_Dx(const double &dValue);
double Cm_To_Sx(const double &dValue);
double Cm_To_Multi(const double &dValue);
double Cm_To_Emu(const double &dValue);

double Mm_To_Cm(const double &dValue);
double Mm_To_Pt(const double &dValue);
double Mm_To_Px(const double &dValue);
double Mm_To_Inch(const double &dValue);
double Mm_To_Dx(const double &dValue);
double Mm_To_Sx(const double &dValue);
double Mm_To_Multi(const double &dValue);
double Mm_To_Emu(const double &dValue);

double Pt_To_Cm(const double &dValue);
double Pt_To_Mm(const double &dValue);
double Pt_To_Px(const double &dValue);
double Pt_To_Inch(const double &dValue);
double Pt_To_Dx(const double &dValue);
double Pt_To_Sx(const double &dValue);
double Pt_To_Multi(const double &dValue);
double Pt_To_Emu(const double &dValue);

double Px_To_Cm(const double &dValue);
double Px_To_Mm(const double &dValue);
double Px_To_Pt(const double &dValue);
double Px_To_Inch(const double &dValue);
double Px_To_Dx(const double &dValue);
double Px_To_Sx(const double &dValue);
double Px_To_Multi(const double &dValue);
double Px_To_Emu(const double &dValue);

double Inch_To_Cm(const double &dValue);
double Inch_To_Mm(const double &dValue);
double Inch_To_Pt(const double &dValue);
double Inch_To_Px(const double &dValue);
double Inch_To_Dx(const double &dValue);
double Inch_To_Sx(const double &dValue);
double Inch_To_Multi(const double &dValue);
double Inch_To_Emu(const double &dValue);

double Dx_To_Cm(const double &dValue);
double Dx_To_Mm(const double &dValue);
double Dx_To_Pt(const double &dValue);
double Dx_To_Px(const double &dValue);
double Dx_To_Inch(const double &dValue);
double Dx_To_Sx(const double &dValue);
double Dx_To_Multi(const double &dValue);
double Dx_To_Emu(const double &dValue);

double Sx_To_Cm(const double &dValue);
double Sx_To_Mm(const double &dValue);
double Sx_To_Pt(const double &dValue);
double Sx_To_Px(const double &dValue);
double Sx_To_Inch(const double &dValue);
double Sx_To_Dx(const double &dValue);
double Sx_To_Multi(const double &dValue);
double Sx_To_Emu(const double &dValue);

double Multi_To_Cm(const double &dValue);
double Multi_To_Mm(const double &dValue);
double Multi_To_Pt(const double &dValue);
double Multi_To_Px(const double &dValue);
double Multi_To_Inch(const double &dValue);
double Multi_To_Sx(const double &dValue);
double Multi_To_Dx(const double &dValue);
double Multi_To_Emu(const double &dValue);

double Emu_To_Cm(const double &dValue);
double Emu_To_Mm(const double &dValue);
double Emu_To_Pt(const double &dValue);
double Emu_To_Twips(const double &dValue);
double Emu_To_Px(const double &dValue);
double Emu_To_Inch(const double &dValue);
double Emu_To_Sx(const double &dValue);
double Emu_To_Dx(const double &dValue);
double Emu_To_Multi(const double &dValue);

namespace XmlUtils
{
	int GetDigit(wchar_t c);
	int GetDigit(char c);
	bool IsDigit(wchar_t c);

	_INT64 GetHex(const std::wstring& string);
	_INT64 GetHex (const std::string& string);
	int GetColorBGR(const std::wstring& string);

	std::wstring GetLower(const std::wstring& string);
	std::wstring GetUpper(const std::wstring& string);

	void replace_all(std::wstring& subject, const std::wstring& search, const std::wstring& replace);
	void replace_all(std::string& subject, const std::string& search, const std::string& replace);

	bool GetBoolean(const std::wstring& string);
	bool GetBoolean2(const std::wstring& string);

	_INT64 GetInteger64(const std::wstring& string);
	int GetInteger(const std::wstring& string);
	unsigned int GetUInteger(const std::wstring& string);
	double GetDouble(const std::wstring& string);
	float GetFloat(const std::wstring& string);

	std::wstring ToString(const bool& value);
	std::wstring ToString(const float& value);
	std::wstring ToString(const double& value);
	std::wstring ToString(const int& value);
	std::wstring ToString(const long& value);
	std::wstring ToString(const unsigned long& value);

	std::wstring ToString(BYTE value, const wchar_t* format);
	std::string ToString(BYTE value, const char* format);

	std::wstring ToString(_INT16 value, const wchar_t* format);
	std::string ToString(_INT16 value, const char* format);
	std::wstring ToString(_UINT16 value, const wchar_t* format);
	std::string ToString(_UINT16 value, const char* format);

	std::wstring ToString(_INT64 value, const wchar_t* format);	
	std::string ToString(_INT64 value, const char* format);
	std::wstring ToString(_UINT64 value, const wchar_t* format);
	std::string ToString(_UINT64 value, const char* format);

	std::wstring ToString(_INT32 value, const wchar_t* format);
	std::string ToString(_INT32 value, const char* format);
	std::wstring ToString(_UINT32 value, const wchar_t* format);
	std::string ToString(_UINT32 value, const char* format);

	std::wstring ToString(double value, const wchar_t* format);
	std::string ToString(double value, const char* format);

	int Rand();
	int GenerateInt();

	std::wstring GenerateGuid();
	std::wstring DoubleToString(double value, wchar_t* format);
	std::string EncodeXmlString(const std::string& data, bool bDeleteNoUnicode = true);
	std::wstring EncodeXmlString(const std::wstring& data, bool bDeleteNoUnicode = true);
	std::wstring DeleteNonUnicode(const std::wstring& data);
	std::wstring EncodeXmlStringExtend(const std::wstring& data, bool bDeleteNoUnicode = true);
}


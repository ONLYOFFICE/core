/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "SimpleTypes_Draw.h"

namespace SimpleTypes
{
namespace Draw
{
	EForeignType CForeignType::FromString(const std::wstring& sValue)
	{
		if (L"Bitmap" == sValue) this->m_eValue = typeBitmap;
		else if (L"EnhMetaFile" == sValue) this->m_eValue = typeEnhMetaFile;
		else if (L"Ink" == sValue) this->m_eValue = typeInk;
		else if (L"Object" == sValue) this->m_eValue = typeObject;
		else this->m_eValue = typeBitmap;

		return this->m_eValue;
	}
	std::wstring CForeignType::ToString() const
	{
		switch (this->m_eValue)
		{
		case typeBitmap: return L"Bitmap";
		case typeEnhMetaFile: return L"EnhMetaFile";
		case typeInk:  return L"Ink";
		case typeObject:  return L"Object";
		default: return L"Bitmap";

		}
	}
	ECompressionType CCompressionType::FromString(const std::wstring& sValue)
	{
		if (L"JPEG" == sValue || L"JPG" == sValue) this->m_eValue = typeJPEG;
		else if (L"DIB" == sValue) this->m_eValue = typeDIB;
		else if (L"PNG" == sValue) this->m_eValue = typePNG;
		else if (L"TIFF" == sValue) this->m_eValue = typeTIFF;
		else if (L"GIF" == sValue) this->m_eValue = typeGIF;
		else this->m_eValue = typeJPEG;

		return this->m_eValue;
	}
	std::wstring CCompressionType::ToString() const
	{
		switch (this->m_eValue)
		{
		case typeJPEG: return L"JPEG";
		case typeDIB: return L"DIB";
		case typePNG:  return L"PNG";
		case typeTIFF:  return L"TIFF";
		case typeGIF:  return L"GIF";
		default: return L"JPEG";

		}
	}
	EShapeType CShapeType::FromString(const std::wstring& sValue)
	{
		if (L"Group" == sValue) this->m_eValue = typeGroup;
		else if (L"Guide" == sValue) this->m_eValue = typeGuide;
		else if (L"Foreign" == sValue) this->m_eValue = typeForeign;
		else this->m_eValue = typeShape;

		return this->m_eValue;
	}
	std::wstring CShapeType::ToString() const
	{
		switch (this->m_eValue)
		{
		case typeGroup: return L"Group";
		case typeGuide: return L"Guide";
		case typeForeign:  return L"Foreign";
		default: return L"Shape";

		}
	}
	EContainerType CContainerType::FromString(const std::wstring& sValue)
	{
		if (L"Page" == sValue) this->m_eValue = typeContainerPage;
		else if (L"Sheet" == sValue) this->m_eValue = typeContainerSheet;
		else if (L"Master" == sValue) this->m_eValue = typeContainerMaster;
		else this->m_eValue = typeContainerPage;

		return this->m_eValue;
	}

	std::wstring CContainerType::ToString() const
	{
		switch (this->m_eValue)
		{
		case typeContainerPage: return L"Page";
		case typeContainerSheet: return L"Sheet";
		case typeContainerMaster:  return L"Master";
		default: return L"Page";
		}
	}

	EWindowType CWindowType::FromString(const std::wstring& sValue)
	{
		if (L"Drawing" == sValue) this->m_eValue = EWindowType::typeDrawing;
		else if (L"Sheet" == sValue) this->m_eValue = EWindowType::typeSheet;
		else if (L"Stencil" == sValue) this->m_eValue = EWindowType::typeStencil;
		else  this->m_eValue = EWindowType::typeSheet;

		return this->m_eValue;
	}
	std::wstring CWindowType::ToString() const
	{
		switch (this->m_eValue)
		{
		case EWindowType::typeDrawing: return L"Drawing";
		case EWindowType::typeSheet: return L"Sheet";
		case EWindowType::typeStencil: return L"Stencil";
		default: return L"Sheet";

		}
	}
}// Draw
} // SimpleTypes

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

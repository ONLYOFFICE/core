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

#include "SVGTransformer.h"
#include "SVGFramework.h"

class CSVGTransformer_private
{
public:
	SVG::Parser		m_oXmlParser;
	SVG::Storage	m_oStorage;
	SVG::Painter	m_oRender;
};

CSVGTransformer::CSVGTransformer()
{
	m_internal = new CSVGTransformer_private();
}
CSVGTransformer::~CSVGTransformer()
{
	delete m_internal;
}

bool CSVGTransformer::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize)
{
	m_internal->m_oStorage.Clear();

	int nSize = NSBase64::Base64EncodeGetRequiredLength(unSize);

	if (nSize <= 0)
		return false;

	BYTE* ucValue = new BYTE[nSize];

	if (!ucValue)
		return false;

	NSBase64::Base64Encode(pBuffer, unSize, ucValue, &nSize);
	std::wstring wsValue(ucValue, ucValue + nSize);

	RELEASEOBJECT(ucValue);

	if (wsValue.empty())
		return false;

	if (0 == m_internal->m_oXmlParser.LoadFromString(wsValue, &m_internal->m_oStorage))
		return false;
}

bool CSVGTransformer::OpenFromFile(const std::wstring& file)
{
	m_internal->m_oStorage.Clear();

	std::wstring sDirectory = NSFile::GetDirectoryName(file);

	m_internal->m_oXmlParser.SetWorkingDirectory(sDirectory);
	m_internal->m_oRender.SetWorkingDirectory(sDirectory);
	m_internal->m_oStorage.SetWorkingDirectory(sDirectory);

	try
	{
		if (!m_internal->m_oXmlParser.LoadFromFile(file, &m_internal->m_oStorage))
			return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}
bool CSVGTransformer::Load(const std::wstring& content)
{
	m_internal->m_oStorage.Clear();

	if (0 == m_internal->m_oXmlParser.LoadFromString(content, &m_internal->m_oStorage))
		return false;

	return true;
}

void CSVGTransformer::Close()
{
	delete m_internal;
	m_internal = new CSVGTransformer_private();
}

void CSVGTransformer::SetFontManager(NSFonts::IFontManager* pFontManager)
{
	m_internal->m_oRender.SetFontManager(pFontManager);
	return;
}

bool CSVGTransformer::Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
{
	m_internal->m_oRender.SetCSS(m_internal->m_oXmlParser.GetCSS());
	m_internal->m_oRender.Draw(&m_internal->m_oStorage, pRenderer, m_internal->m_oXmlParser.GetUnitSystem(), dX, dY, dWidth, dHeight);
	return true;
}
int CSVGTransformer::get_Width()
{
	return m_internal->m_oXmlParser.GetWidth();
}
int CSVGTransformer::get_Height()
{
	return m_internal->m_oXmlParser.GetHeight();
}
int CSVGTransformer::get_Metrics()
{
	return m_internal->m_oXmlParser.GetMetrics();
}

bool CSVGTransformer::GetBounds(double &dX, double &dY, double &dWidth, double &dHeight)
{
	dX = 0;
	dY = 0;
	dWidth = get_Width();
	dHeight = get_Height();

	return true;
}

/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

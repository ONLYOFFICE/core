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

#include "./AnnotField.h"


CAnnotFieldInfo::CAnnotFieldInfo()
{
	m_nType = 0;

	m_nID = 0;
	m_nAnnotFlag = 0;
	m_nPage = 0;

	m_dX = 0;
	m_dY = 0;
	m_dW = 0;
	m_dH = 0;

	m_dBaseLineOffset = 0;

	m_pMarkupPr = NULL;
	m_pTextPr = NULL;
}
CAnnotFieldInfo::~CAnnotFieldInfo()
{
	RELEASEOBJECT(m_pMarkupPr);
	RELEASEOBJECT(m_pTextPr);
}

void CAnnotFieldInfo::SetType(int nType)
{

	switch (nType)
	{
	case 0:
	{
		m_nType = 7;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pTextPr);
		m_pTextPr = new CAnnotFieldInfo::CTextAnnotPr();
		break;
	}
	}
}
bool CAnnotFieldInfo::IsValid() const
{
	return (m_nType != 0);
}

// Common
void CAnnotFieldInfo::SetBounds(const double& dX, const double& dY, const double& dW, const double& dH)
{
	m_dX = dX;
	m_dY = dY;
	m_dW = dW;
	m_dH = dH;
}
void CAnnotFieldInfo::GetBounds(double& dX, double& dY, double& dW, double& dH) const
{
	dX = m_dX;
	dY = m_dY;
	dW = m_dW;
	dH = m_dH;
}

bool CAnnotFieldInfo::isWidget() const
{
	return (m_nType != 0 && m_nType < 7);
}
bool CAnnotFieldInfo::isMarkup() const
{
	return (m_nType > 6 && m_nType < 24);
}
bool CAnnotFieldInfo::IsText() const
{
	return (m_nType == 7);
}
bool CAnnotFieldInfo::IsInk() const
{
	return (m_nType == 8);
}
bool CAnnotFieldInfo::IsLine() const
{
	return (m_nType == 9);
}
CAnnotFieldInfo::CTextAnnotPr* CAnnotFieldInfo::GetTextAnnotPr()
{
	return m_pTextPr;
}

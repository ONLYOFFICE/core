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

#include "CtrlShapeVideo.h"

namespace HWP
{
CCtrlShapeVideo::CCtrlShapeVideo(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeVideo::CCtrlShapeVideo(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeVideo::CCtrlShapeVideo(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeVideo::CCtrlShapeVideo(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("type" == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if ("Local" == sType)
				m_nVideoType = 0;
			else if ("Web" == sType)
				m_nVideoType = 1;
		}
		else if ("fileIDRef" == sAttributeName)
			m_shVideoBinID = oReader.GetInt();
		else if ("imageIDRef" == sAttributeName)
			m_sThumnailBinID = oReader.GetInt();
		else if ("tag" == sAttributeName)
			m_sWebURL = oReader.GetText();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, eType);
	END_WHILE
}

EShapeType CCtrlShapeVideo::GetShapeType() const
{
	return EShapeType::Video;
}

int CCtrlShapeVideo::GetVideoType() const
{
	return m_nVideoType;
}

short CCtrlShapeVideo::GetVedeoBinID() const
{
	return m_shVideoBinID;
}

HWP_STRING CCtrlShapeVideo::GetWebUrl() const
{
	return m_sWebURL;
}

HWP_STRING CCtrlShapeVideo::GetThumnailBinID() const
{
	return m_sThumnailBinID;
}

int CCtrlShapeVideo::ParseElement(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nVideoType);

	if (0 == oObj.m_nVideoType)
		oBuffer.ReadShort(oObj.m_shVideoBinID);
	else if (1 == oObj.m_nVideoType)
		oBuffer.ReadString(oObj.m_sWebURL, EStringCharacter::UTF16);

	short m_sBinID;
	oBuffer.ReadShort(m_sBinID);
	oObj.m_sThumnailBinID = std::to_wstring(m_sBinID);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}

int CCtrlShapeVideo::ParseCtrl(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}
}

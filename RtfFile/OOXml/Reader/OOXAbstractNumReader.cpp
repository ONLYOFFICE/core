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

#include "OOXAbstractNumReader.h"
#include "../../../OOXML/DocxFormat/Numbering.h"

OOXAbstractNumReader::OOXAbstractNumReader(OOX::Numbering::CAbstractNum  *ooxAbstractNum)
{
	m_ooxAbstractNum = ooxAbstractNum;
}
bool OOXAbstractNumReader::Parse( ReaderParameter oParam )
{
	if (m_ooxAbstractNum == NULL) return false;

	RtfListProperty oNewList;

	if (m_ooxAbstractNum->m_oAbstractNumId.IsInit())
		oNewList.m_nID = *m_ooxAbstractNum->m_oAbstractNumId;

	if (m_ooxAbstractNum->m_oMultiLevelType.IsInit())
	{
		switch(m_ooxAbstractNum->m_oMultiLevelType->m_oVal->GetValue())
		{
		case SimpleTypes::emultileveltypeHybridMultilevel : 	oNewList.m_bListHybrid = 1; break;
		case SimpleTypes::emultileveltypeSingleLevel      : 	oNewList.m_nListSimple = 1; break;
		case SimpleTypes::emultileveltypeMultilevel       :
		default											  :		break;
		}
	}
	if (m_ooxAbstractNum->m_oName.IsInit())
		oNewList.m_sName = *m_ooxAbstractNum->m_oName->m_sVal;

	if (m_ooxAbstractNum->m_oTmpl.IsInit() && m_ooxAbstractNum->m_oTmpl->m_oVal.IsInit())
		oNewList.m_nTemplateId = m_ooxAbstractNum->m_oTmpl->m_oVal->GetValue();

	for (size_t i=0 ; i < m_ooxAbstractNum->m_arrLvl.size(); i++)
	{
		OOXLevelReader oLevelReader(m_ooxAbstractNum->m_arrLvl[i]);
		RtfListLevelProperty oNewLevel;

		if( true == oLevelReader.Parse( oParam, oNewLevel) )
			oNewList.AddItem( oNewLevel );
	}
	oParam.oRtf->m_oListTable.AddItem( oNewList );

	return true;
}

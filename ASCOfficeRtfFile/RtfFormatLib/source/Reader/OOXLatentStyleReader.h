/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../RtfDocument.h"
#include "../RtfProperty.h"


class OOXLatentStyleReader
{
public: 
	OOXLatentStyleReader()
	{
	}
	bool Parse( ReaderParameter oParam )
	{
		XmlUtils::CXmlLiteReader oXmlReader;
		if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
		{
			CString nCount = oXmlReader.ReadNodeAttribute( _T("w:count") );
			if( _T("") != nCount )
				oParam.oRtf->m_oLatentStyleTable.m_nCount = Strings::ToInteger( nCount );
			CString nLocked = oXmlReader.ReadNodeAttribute( _T("w:defLockedState") );
			if( _T("") != nLocked )
				oParam.oRtf->m_oLatentStyleTable.m_nLocked= Strings::ToInteger( nLocked );
			CString nSemiHid = oXmlReader.ReadNodeAttribute( _T("w:defSemiHidden") );
			if( _T("") != nSemiHid )
				oParam.oRtf->m_oLatentStyleTable.m_nSemiHidden= Strings::ToInteger( nSemiHid );
			CString nUNhidWU = oXmlReader.ReadNodeAttribute( _T("w:defUnhideWhenUsed") );
			if( _T("") != nUNhidWU )
				oParam.oRtf->m_oLatentStyleTable.m_nUnHiddenWhenUse= Strings::ToInteger( nUNhidWU );
			CString nQFormat = oXmlReader.ReadNodeAttribute( _T("w:defQFormat") );
			if( _T("") != nQFormat )
				oParam.oRtf->m_oLatentStyleTable.m_nQFormat= Strings::ToInteger( nQFormat );
			CString nUIPriority = oXmlReader.ReadNodeAttribute( _T("w:defUIPriority") );
			if( _T("") != nUIPriority )
				oParam.oRtf->m_oLatentStyleTable.m_nPriority= Strings::ToInteger( nUIPriority );

			oXmlReader.ReadNodeList( _T("w:lsdException") );
			for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
			{
				RtfStyleException oNewException;
				oNewException.m_sName = oXmlReader.ReadNodeAttribute( _T("w:name"), _T("") );
				CString nPriority = oXmlReader.ReadNodeAttribute( _T("w:uiPriority"));
				if( _T("") != nPriority )
					oNewException.m_nPriority = Strings::ToInteger( nPriority );
				CString nHiddenWhenUse = oXmlReader.ReadNodeAttribute( _T("w:unhideWhenUsed") );
				if( _T("") != nHiddenWhenUse )
					oNewException.m_nHiddenWhenUse = Strings::ToInteger( nHiddenWhenUse );
				CString nQFormat = oXmlReader.ReadNodeAttribute( _T("w:qFormat") );
				if( _T("") != nQFormat )
					oNewException.m_nQFormat = Strings::ToInteger( nQFormat );
				CString nLocked = oXmlReader.ReadNodeAttribute( _T("w:locked") );
				if( _T("") != nLocked )
					oNewException.m_nLocked = Strings::ToInteger( nLocked );
				CString nSemiHidden = oXmlReader.ReadNodeAttribute( _T("w:semiHidden") );
				if( _T("") != nSemiHidden )
					oNewException.m_nSemiHidden = Strings::ToInteger( nSemiHidden );

				if( true == oNewException.IsValid() )
					oParam.oRtf->m_oLatentStyleTable.AddItem( oNewException );
			}

		}
		return false;
	}

};

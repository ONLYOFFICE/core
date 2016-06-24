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
#include "OOXPictureGraphicReader.h"
#include "OOXReaderBasic.h"

#include "../RtfDocument.h"
#include "../RtfShape.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/Vml.h"

class OOXShapeReader
{
public: 
	OOXShapeReader(OOX::Vml::CVmlCommonElements * vmlElem)
	{
		m_vmlElement = vmlElem;
		m_arrElement = vmlElem;
	}
    OOXShapeReader(OOX::WritingElementWithChilds<OOX::WritingElement> * elem);

	bool Parse( ReaderParameter oParam , RtfShapePtr& oOutput);
	bool Parse2( ReaderParameter oParam , RtfShapePtr& oOutput);
	
	void ParseAdjustment(RtfShape& oShape, CString sAdjustment)
	{
		 int nPosition = 0;
		 CString sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue2 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue3 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue4 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue5 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue6 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue7 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue8 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue9 = Strings::ToInteger( sAdj );
		 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
		 if( _T("") == sAdj )
			 return;
		 oShape.m_nAdjustValue10 = Strings::ToInteger( sAdj );
	 }

private:
	OOX::Vml::CVmlCommonElements						*m_vmlElement;
	OOX::WritingElementWithChilds<OOX::WritingElement>  *m_arrElement;

	bool ParseStyles(RtfShape& oShape, std::vector<SimpleTypes::Vml::CCssProperty*> & props)
	{
		for (long i=0; i< props.size(); i++)
		{
			ParseStyle( oShape, props[i]);
		}
		return true;
	}
	bool ParseStyle(RtfShape& oShape, SimpleTypes::Vml::CCssProperty* prop);

};


class OOXShapeGroupReader
{
private:
	OOX::Vml::CGroup *m_vmlGroup;
public: 
	OOXShapeGroupReader(OOX::Vml::CGroup *vmlGroup)
	{
		m_vmlGroup = vmlGroup;
	}

	bool Parse( ReaderParameter oParam , RtfShapeGroupPtr& oOutput)
	{
		if (m_vmlGroup == NULL) return false;

		//RtfShapePtr oBaseShape = boost::static_pointer_cast<RtfShape, RtfShapeGroup>( oOutput );
		//
		//OOXShapeReader oShapeReader(m_vmlGroup);
		//oShapeReader.Parse( oParam, oBaseShape );
		//todooo притащить сюда атрибуты группы

		for( int i = 0; i < m_vmlGroup->m_arrItems.size() ; i++ )
		{
			if (m_vmlGroup->m_arrItems[i] == NULL) continue;

			if (m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_group)
			{
				RtfShapeGroupPtr oNewShape( new RtfShapeGroup() );
				
				OOXShapeGroupReader oShapeReader(dynamic_cast<OOX::Vml::CGroup*>(m_vmlGroup->m_arrItems[i]));
				
				if( true == oShapeReader.Parse( oParam, oNewShape ) )
					 oOutput->AddItem( oNewShape );
			}
			else if (	m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_arc	||
						m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_line	||
						m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_oval	||
						m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_shape	||
						m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_rect	||
						m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_roundrect ||
						m_vmlGroup->m_arrItems[i]->getType() == OOX::et_v_polyline)
			{
				RtfShapePtr oNewShape( new RtfShape() );//set type .. .todooo
				
				OOXShapeReader oShapeReader(dynamic_cast<OOX::Vml::CVmlCommonElements*>(m_vmlGroup->m_arrItems[i]));
				
				if( true == oShapeReader.Parse( oParam, oNewShape ) )
					 oOutput->AddItem( oNewShape );
			}
			else
			{
				//??? todooo
				//shapetype как минимум нужен !!!
			}
		}
		return true;
	}
};

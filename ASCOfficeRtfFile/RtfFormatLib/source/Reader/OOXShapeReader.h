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
	OOXShapeReader(OOX::WritingElementWithChilds<OOX::WritingElement> * elem)
	{
		m_arrElement = elem;
	}	
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

		//RtfShapePtr oBaseShape = boost::shared_static_cast<RtfShape, RtfShapeGroup>( oOutput );
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
#pragma once
#include "OOXReader.h"
#include "../RtfDocument.h"
#include <limits.h>

class OOXFootnotesReader
{
private:
	OOX::CEndnotes	* m_ooxEndNotes;
	OOX::CFootnotes * m_ooxFootNotes;

public: 
	OOXFootnotesReader(OOX::CEndnotes * ooxNotes)
	{
		m_ooxFootNotes	= NULL;
		m_ooxEndNotes	= ooxNotes;
	}
	OOXFootnotesReader(OOX::CFootnotes * ooxNotes)
	{
		m_ooxFootNotes	= ooxNotes;
		m_ooxEndNotes	= NULL;
	}

	bool Parse( ReaderParameter oParam)
	{
		int nSeparatorId = 0;
		int nContinueId = 1;

		if (m_ooxEndNotes)
		{
			int nMin1 = INT_MAX;
			if( oParam.oRtf->m_oProperty.m_aSpecialEndnotes.size() > 0 )
			{
				int nMin1 = INT_MAX;
				int nMin2 = INT_MAX;
				for( int i = 0; i < (int)oParam.oRtf->m_oProperty.m_aSpecialEndnotes.size(); i++ )
					if( nMin1 > oParam.oRtf->m_oProperty.m_aSpecialEndnotes[i] )
					{
						nMin2 = nMin1;
						nMin1 = oParam.oRtf->m_oProperty.m_aSpecialEndnotes[i];
					}
				if( INT_MAX != nMin1 )
					nSeparatorId = nMin1;
				if( INT_MAX != nMin2 )
					nContinueId = nMin1;
			}

			for (long i =0 ; i < m_ooxEndNotes->m_arrEndnote.size() ; i++)
				ParseOne(m_ooxEndNotes->m_arrEndnote[i], oParam, false, nSeparatorId, nContinueId);
		}

		if (m_ooxFootNotes)
		{
			int nMin1 = INT_MAX;
			if( oParam.oRtf->m_oProperty.m_aSpecialFootnotes.size() > 0 )
			{
				int nMin1 = INT_MAX;
				int nMin2 = INT_MAX;
				for( int i = 0; i < (int)oParam.oRtf->m_oProperty.m_aSpecialFootnotes.size(); i++ )
					if( nMin1 > oParam.oRtf->m_oProperty.m_aSpecialFootnotes[i] )
					{
						nMin2 = nMin1;
						nMin1 = oParam.oRtf->m_oProperty.m_aSpecialFootnotes[i];
					}
				if( INT_MAX != nMin1 )
					nSeparatorId = nMin1;
				if( INT_MAX != nMin2 )
					nContinueId = nMin1;
			}
			for (long i =0 ; i < m_ooxFootNotes->m_arrFootnote.size() ; i++)
				ParseOne(m_ooxFootNotes->m_arrFootnote[i], oParam, true, nSeparatorId, nContinueId);
		}
		return true;
	}
	bool ParseOne(OOX::CFtnEdn* ooxFtnEdn, ReaderParameter & oParam, bool bFootnote, int nSeparatorId, int nContinueId)
	{
		if (ooxFtnEdn == NULL) return false;
		if (ooxFtnEdn->m_oId.IsInit() == false) return false;

		int nId = ooxFtnEdn->m_oId->GetValue();
		OOXTextItemReader oTextItemReader;
		
		for (long i = 0 ; i < ooxFtnEdn->m_arrItems.size(); i++)
		{			
			if( nSeparatorId == nId )
			{
				TextItemContainerPtr oNewTextItem( new TextItemContainer() );
				oTextItemReader.m_oTextItems = oNewTextItem;
				
				if( true == oTextItemReader.Parse( ooxFtnEdn->m_arrItems[i], oParam ) )
				{
					if( true == bFootnote )
						oParam.oRtf->m_oFootnoteSep = oNewTextItem;
					else
						oParam.oRtf->m_oEndnoteSep = oNewTextItem;
				}
			}
			else if( nContinueId == nId )
			{
				TextItemContainerPtr oNewTextItem( new TextItemContainer() );
				oTextItemReader.m_oTextItems = oNewTextItem;
				
				if( true == oTextItemReader.Parse( ooxFtnEdn->m_arrItems[i], oParam ) )
				{
					if( true == bFootnote )
						oParam.oRtf->m_oFootnoteCon = oNewTextItem;
					else
						oParam.oRtf->m_oEndnoteCon = oNewTextItem;
				}
			}
			else
			{
				TextItemContainerPtr oNewTextItem( new TextItemContainer() );
				oTextItemReader.m_oTextItems = oNewTextItem;
				
				if( true == oTextItemReader.Parse( ooxFtnEdn->m_arrItems[i], oParam ) )
				{
					if( true == bFootnote )
						oParam.oReader->m_mapFootnotes[ nId] = oNewTextItem;
					else
						oParam.oReader->m_mapEndnotes[ nId] = oNewTextItem;
				}
			}
		}
		return true;
	}
};

#pragma once
#include "Basic.h"

#include "RtfGlobalTables.h"
#include "RtfSection.h"

class RtfDocument :public ItemContainer<RtfSectionPtr>
{
public: 
	RtfDocumentProperty m_oProperty;
	RtfFontTable m_oFontTable;
	RtfColorTable m_oColorTable;
	RtfStyleTable m_oStyleTable;
	RtfInformation m_oInformation;

	RtfListTable m_oListTabel;
	RtfListOverrideTable m_oListOverrideTabel;

	RtfParagraphProperty m_oDefaultParagraphProp;
	RtfCharProperty m_oDefaultCharProp;
	RtfMathProperty m_oMathProp;

	TextItemContainerPtr m_oFootnoteSep;
	TextItemContainerPtr m_oFootnoteCon;
	TextItemContainerPtr m_oEndnoteSep;
	TextItemContainerPtr m_oEndnoteCon;

//для того чтобы конвертировать старый формат List в Numbering
	std::vector<RtfOldListPtr> m_aOldLists;

private: 
	std::vector<int> m_aShapeId;
public: 
	IdGenerator m_oIdGenerator;
	void SetShapeId( int nShapeId )
	{
		for( int i = 0; i < (int)m_aShapeId.size(); i++ )
			if( nShapeId == m_aShapeId[i] )
				return;
		m_aShapeId.push_back( nShapeId );
	}
	int GetShapeId(  int& nShapeId  )
	{
		if( PROP_DEF != nShapeId )
			return nShapeId;
		int nNewShapeId;
		while( true )
		{
			bool bUnique = true;
			nNewShapeId = m_oIdGenerator.Generate_ShapeId();
			for( int i = 0; i < (int)m_aShapeId.size(); i++ )
				if( nNewShapeId == m_aShapeId[i] )
				{
					bUnique = false;
					break;
				}
			if( true == bUnique )
			{
				nShapeId = nNewShapeId;
				m_aShapeId.push_back( nNewShapeId );
				break;
			}
		}
		return nShapeId;
	}
	RtfDocument();
	int GetType();

	CString RenderToRtf(RenderParameter oRenderParameter);
	CString RenderToOOX(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfDocument> RtfDocumentPtr;
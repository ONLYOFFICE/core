#pragma once
#include "Basic.h"

#include "RtfGlobalTables.h"
#include "RtfSection.h"

class RtfDocument :public ItemContainer<RtfSectionPtr>
{
public: RtfDocumentProperty m_oProperty;
public: RtfFontTable m_oFontTable;
public: RtfColorTable m_oColorTable;
public: RtfStyleTable m_oStyleTable;
public: RtfInformation m_oInformation;

public: RtfListTable m_oListTabel;
public: RtfListOverrideTable m_oListOverrideTabel;

public: RtfParagraphProperty m_oDefaultParagraphProp;
public: RtfCharProperty m_oDefaultCharProp;
public: RtfMathProperty m_oMathProp;

public: TextItemContainerPtr m_oFootnoteSep;
public: TextItemContainerPtr m_oFootnoteCon;
public: TextItemContainerPtr m_oEndnoteSep;
public: TextItemContainerPtr m_oEndnoteCon;

//для того чтобы конвертировать старый формат List в Numbering
public: CAtlArray<RtfOldListPtr> m_aOldLists;

private: CAtlArray<int> m_aShapeId;
public: IdGenerator m_oIdGenerator;
public: void SetShapeId( int nShapeId )
		{
			for( int i = 0; i < (int)m_aShapeId.GetCount(); i++ )
				if( nShapeId == m_aShapeId[i] )
					return;
			m_aShapeId.Add( nShapeId );
		}
public: int GetShapeId(  int& nShapeId  )
		{
			if( PROP_DEF != nShapeId )
				return nShapeId;
			int nNewShapeId;
			while( true )
			{
				bool bUnique = true;
				nNewShapeId = m_oIdGenerator.Generate_ShapeId();
				for( int i = 0; i < (int)m_aShapeId.GetCount(); i++ )
					if( nNewShapeId == m_aShapeId[i] )
					{
						bUnique = false;
						break;
					}
				if( true == bUnique )
				{
					nShapeId = nNewShapeId;
					m_aShapeId.Add( nNewShapeId );
					break;
				}
			}
			return nShapeId;
		}
public: RtfDocument();
public: int GetType();

public: CString RenderToRtf(RenderParameter oRenderParameter);
public: CString RenderToOOX(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfDocument> RtfDocumentPtr;
#pragma once 

class IdGenerator
{
public: IdGenerator()
		{
			m_nrIdCount = 1;
			m_nFitTextIdCount = 1;
			m_nImageIndex = 1;
			m_nOleIndex = 1;
			m_nHeaderNumber = 1;
			m_nFooterNumber = 1;
			m_nBookmarkNumber = 1;
			m_nFootnoteNumber = 2;
			m_nEndnoteNumber = 2;
			m_nImageProp = 1;
			m_nShapeId = 1;
			m_nOleId = 1;
			m_nPnListId = 1;
		}

private: int m_nrIdCount;
public:	CString Generate_rId()
		{
			CString sResult;
			sResult.AppendFormat( _T("rId%d"), m_nrIdCount++ );
			return sResult;
		}

private: int m_nFitTextIdCount;
public:	CString Generate_FitTextId()
		{
			CString sResult;
			sResult.AppendFormat( _T("%d"), m_nFitTextIdCount++ );
			return sResult;
		}

private: int m_nImageIndex;
public:	int Generate_ImageIndex()
		{
			return m_nImageIndex++;
		}
private: int m_nOleIndex;
public:	int Generate_OleIndex()
		{
			return m_nOleIndex++;
		}
private: int m_nHeaderNumber;
public:	int Generate_HeaderNumber()
		{
			return m_nHeaderNumber++;
		}
private: int m_nFooterNumber;
public:	int Generate_FooterNumber()
		{
			return m_nFooterNumber++;
		}
private: int m_nBookmarkNumber;
public:	int Generate_BookmarkNumber()
		{
			return m_nBookmarkNumber++;
		}
private: int m_nFootnoteNumber;
public:	int Generate_FootnoteNumber()
		{
			return m_nFootnoteNumber++;
		}
private: int m_nEndnoteNumber;
public:	int Generate_EndnoteNumber()
		{
			return m_nEndnoteNumber++;
		}
private: int m_nImageProp;
public:	int Generate_ImagePropId()
		{
			return m_nImageProp++;
		}
private: int m_nShapeId;
public:	int Generate_ShapeId()
		{
			return m_nShapeId++;
		}
private: int m_nOleId;
public:	CString Generate_OleId()
		{
			CString sId;
			sId.AppendFormat( _T("%d"), m_nOleId++ );
			CString sResult = _T("_1330071130");
			sResult = sResult.Left( sResult.GetLength() - sId.GetLength() );
			sResult.Append( sId );
			return sResult;
		}
private: int m_nPnListId;
public:	int Generate_PnId()
		{
			return m_nPnListId++;
		}	
};

class OOXIdGenerator
{
private: CAtlMap<CString, long> m_mapId;
private: int m_nCounter;
public: OOXIdGenerator()
		{
			m_nCounter = 1;
		}
public: int GetId( CString sKey )
		{
			CAtlMap<CString, long>::CPair *pPair = NULL;
			pPair = m_mapId.Lookup( sKey );
			if( NULL == pPair )
			{
				int nResult = m_nCounter;
				m_mapId.SetAt( sKey, nResult );
				m_nCounter++;
				return nResult;
			}
			else
				return pPair->m_value;
		}
};
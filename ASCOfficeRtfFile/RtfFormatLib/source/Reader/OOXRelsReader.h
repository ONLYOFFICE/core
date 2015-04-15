#pragma once

//class OOXRelsReader
//{
//private: 
//	XmlUtils::CXmlLiteReader m_oReader;
//public:		
//	OOXRelsReader()
//	{
//	}
//	void Init( CString sFilePath )
//	{
//		m_oReader.OpenFromFile(sFilePath);
//		m_oReader.ReadRootNode( _T("Relationships") );
//		m_oReader.ReadNodeList( _T("Relationship") );
//	}
//	bool GetByType(CString  sType, std::vector<CString>& aOutArray)
//	{
//		aOutArray.clear();
//		for(int i=0;i<m_oReader.GetLengthList();i++)
//		{
//			if(m_oReader.ReadNodeAttribute(i, _T("Type") ) == sType)
//			{
//				CString sResult = m_oReader.ReadNodeAttribute(i, _T("Target") );
//				sResult.Replace('/','\\');
//				aOutArray.push_back( sResult );
//			}
//		}
//		return aOutArray.size() > 0;
//	}
//	CString GetByID(CString  sId)
//	{
//		for(int i=0;i<m_oReader.GetLengthList();i++)
//		{
//			if( m_oReader.ReadNodeAttribute(i, _T("Id") ) == sId )
//			{
//				CString sResult = m_oReader.ReadNodeAttribute( i, _T("Target") );
//				//sResult.Replace('/','\\');
//				return sResult;
//			}
//		}
//		return _T("");
//	}
//};
//typedef boost::shared_ptr<OOXRelsReader> OOXRelsReaderPtr;
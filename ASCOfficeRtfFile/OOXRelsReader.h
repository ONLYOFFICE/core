#pragma once
#include "XmlUtils.h"

class OOXRelsReader
{
private: XmlUtils::CXmlReader m_oReader;

public: OOXRelsReader()
		{
		}
public: void Init( CString sFilePath )
		{
			m_oReader.OpenFromFile(sFilePath);
			m_oReader.ReadRootNode( _T("Relationships") );
			m_oReader.ReadNodeList( _T("Relationship") );
		}
public: bool GetByType(CString  sType, CAtlArray<CString>& aOutArray)
		{
			aOutArray.RemoveAll();
			for(int i=0;i<m_oReader.GetLengthList();i++)
			{
				if(m_oReader.ReadNodeAttribute(i, _T("Type") ) == sType)
				{
					CString sResult = m_oReader.ReadNodeAttribute(i, _T("Target") );
					sResult.Replace('/','\\');
					aOutArray.Add( sResult );
				}
			}
			return aOutArray.GetCount() > 0;
		}
public: CString GetByID(CString  sId)
		{
			for(int i=0;i<m_oReader.GetLengthList();i++)
			{
				if( m_oReader.ReadNodeAttribute(i, _T("Id") ) == sId )
				{
					CString sResult = m_oReader.ReadNodeAttribute( i, _T("Target") );
					//sResult.Replace('/','\\');
					return sResult;
				}
			}
			return _T("");
		}
};
typedef boost::shared_ptr<OOXRelsReader> OOXRelsReaderPtr;
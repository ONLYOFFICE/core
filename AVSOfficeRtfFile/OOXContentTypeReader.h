#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"

class OOXContentTypeReader
{
private: CString m_sFilepath;
private: XmlUtils::CXmlReader m_oReader;

public: OOXContentTypeReader(CString sFilePath)
		{
			m_sFilepath = sFilePath;
			m_oReader.OpenFromFile(m_sFilepath);
			m_oReader.ReadRootNode("Relationships");
			m_oReader.ReadNodeList("Relationship");
		}
public: bool GetByType(CString  sType, CAtlArray<CString>& aOutArray)
		{
			aOutArray.RemoveAll();
			for(int i=0;i<oParam.oReader->GetLengthList();i++)
			{
				if(oParam.oReader->ReadNodeAttribute(i,"Type") == sType)
				{
					CString sResult = oParam.oReader->ReadNodeAttribute(i,"Target");
					sResult = sResult.Replace('/','\\');
					aOutArray.Append( sResult );
				}
			}
			return aOutArray.GetCount() > 0;
		}
public: CString GetByID(CString  sId)
		{
			for(int i=0;i<oParam.oReader->GetLengthList();i++)
			{
				if(oParam.oReader->ReadNodeAttribute(i,"Id") == sId)
				{
					CString sResult = oParam.oReader->ReadNodeAttribute(i,"Target");
					sResult = sResult.Replace('/','\\');
					return sResult;
				}
			}
			return "";
		}
};
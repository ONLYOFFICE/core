#pragma once
#include "Common.h"

namespace Writers
{
	class DocumentRelsWriter
	{
		XmlUtils::CStringWriter  m_oWriter;
		CString	m_sDir;
		int m_nRid;
		std::vector<CString> m_aRels;
		bool bDocumentRels;
	public:
		DocumentRelsWriter(CString sDir, bool bDocumentRels, int nRid = 1):m_sDir(sDir),bDocumentRels(bDocumentRels)
		{
			m_nRid = nRid;
		}
		void Write(CString sFileName)
		{
			CString s_dr_Start;
			CString s_dr_End;
			if(true == bDocumentRels)
			{
				s_dr_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/><Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/><Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/><Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/><Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>");
				s_dr_End = _T("</Relationships>");
			}
			else
			{
				s_dr_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");
				s_dr_End = _T("</Relationships>");
			}
			if(m_nRid > 1)
			{
				m_oWriter.WriteString(s_dr_Start);
				for(int i = 0, length = m_aRels.size(); i < length; ++i)
				{
					m_oWriter.WriteString(m_aRels[i]);
				}
				m_oWriter.WriteString(s_dr_End);

				CFile oFile;
				oFile.CreateFile(m_sDir + _T("\\word\\_rels\\") + sFileName);
				oFile.WriteStringUTF8(m_oWriter.GetData());
				oFile.CloseFile();
			}
		}
		CString AddRels(CString sType, CString sTarget, bool bExternal = false)
		{
			CorrectString(sType);
			CorrectString(sTarget);
			CString srId;srId.Format(_T("rId%d"), m_nRid);
			CString sRels;
			if(bExternal)
				sRels.Format(_T("<Relationship Id=\"%s\" Type=\"%s\" Target=\"%s\" TargetMode=\"External\"/>"), srId, sType, sTarget);
			else
				sRels.Format(_T("<Relationship Id=\"%s\" Type=\"%s\" Target=\"%s\"/>"), srId, sType, sTarget);
			m_nRid++;
			m_aRels.push_back(sRels);
			return srId;
		}
	};
}
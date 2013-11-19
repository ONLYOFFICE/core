#pragma once
#include "stdafx.h"
#include "XmlUtils.h"
#include "IdGenerator.h"
#include "ATLDefine.h"
#include "Resource.h"

class OOXThemeWriter
{
private: OOXWriter& m_oWriter;
public: OOXThemeWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
		{
		}
public: bool Save( CString sFolder ) //todo доделать очистку поле ошибок
		{
			CreateDirectory(sFolder + _T("\\word"), NULL);
			CreateDirectory(sFolder + _T("\\word\\theme"),NULL);
			if( true == RtfUtility:: SaveResourceToFile( IDR_THEME, L"XML", sFolder + _T("\\word\\theme\\theme1.xml") ) )
			{
				m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme"), _T("theme/theme1.xml") );
				m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.theme+xml"), _T("/word/theme/theme1.xml") );
			}
			else
			{
				RemoveDirectory( sFolder + _T("\\word\\theme") );
			}
			return true;
		}
};
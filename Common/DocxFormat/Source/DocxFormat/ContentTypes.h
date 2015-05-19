#pragma once
#ifndef OOX_CONTENT_TYPES_INCLUDE_H_
#define OOX_CONTENT_TYPES_INCLUDE_H_

#include "../SystemUtility/SystemUtility.h"

#include "FileType.h"
#include "WritingElement.h"
#include <map>


namespace OOX
{
	namespace ContentTypes
	{
		class CExtensionTable
		{
		public:
			CExtensionTable()
			{
                m_mTable.insert( std::pair<CString,CString>( _T("bmp"),  _T("image/bmp")));
                m_mTable.insert( std::pair<CString,CString>(  _T("gif"),  _T("image/gif")));
                m_mTable.insert( std::pair<CString,CString>(  _T("png"),  _T("image/png")));
                m_mTable.insert( std::pair<CString,CString>(  _T("tif"),  _T("image/tiff")));
                m_mTable.insert( std::pair<CString,CString>(  _T("tiff"), _T("image/tiff")));
                m_mTable.insert( std::pair<CString,CString>(  _T("jpeg"), _T("image/jpeg")));
                m_mTable.insert( std::pair<CString,CString>(  _T("jpg"),  _T("image/jpeg")));
                m_mTable.insert( std::pair<CString,CString>(  _T("jpe"),  _T("image/jpeg")));
                m_mTable.insert( std::pair<CString,CString>(  _T("jfif"), _T("image/jpeg")));
                m_mTable.insert( std::pair<CString,CString>(  _T("rels"), _T("application/vnd.openxmlformats-package.relationships+xml")));
                m_mTable.insert( std::pair<CString,CString>(  _T("bin"),  _T("application/vnd.openxmlformats-officedocument.oleObject")));
                m_mTable.insert( std::pair<CString,CString>(  _T("xml"),  _T("application/xml")));
                m_mTable.insert( std::pair<CString,CString>(  _T("emf"),  _T("image/x-emf")));
                m_mTable.insert( std::pair<CString,CString>(  _T("emz"),  _T("image/x-emz")));
                m_mTable.insert( std::pair<CString,CString>(  _T("wmf"),  _T("image/x-wmf")));
                m_mTable.insert( std::pair<CString,CString>(  _T("svm"),  _T("image/svm")));
                m_mTable.insert( std::pair<CString,CString>(  _T("wav"),  _T("audio/wav")));
                m_mTable.insert( std::pair<CString,CString>(  _T("xls"),  _T("application/vnd.ms-excel")));
                m_mTable.insert( std::pair<CString,CString>(  _T("xlsm"), _T("application/vnd.ms-excel.sheet.macroEnabled.12")));
                m_mTable.insert( std::pair<CString,CString>(  _T("xlsb"), _T("application/vnd.ms-excel.sheet.binary.macroEnabled.12")));
                m_mTable.insert( std::pair<CString,CString>(  _T("xlsx"), _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")));
                m_mTable.insert( std::pair<CString,CString>(  _T("ppt"),  _T("application/vnd.ms-powerpoint")));
                m_mTable.insert( std::pair<CString,CString>(  _T("pptm"), _T("application/vnd.ms-powerpoint.presentation.macroEnabled.12")));
                m_mTable.insert( std::pair<CString,CString>(  _T("pptx"), _T("application/vnd.openxmlformats-officedocument.presentationml.presentation")));
                m_mTable.insert( std::pair<CString,CString>(  _T("sldm"), _T("application/vnd.ms-powerpoint.slide.macroEnabled.12")));
                m_mTable.insert( std::pair<CString,CString>(  _T("sldx"), _T("application/vnd.openxmlformats-officedocument.presentationml.slide")));
                m_mTable.insert( std::pair<CString,CString>(  _T("doc"),  _T("application/msword")));
                m_mTable.insert( std::pair<CString,CString>(  _T("docm"), _T("aapplication/vnd.ms-word.document.macroEnabled.12")));
                m_mTable.insert( std::pair<CString,CString>(  _T("docx"), _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document")));
                m_mTable.insert( std::pair<CString,CString>(  _T("vml"),  _T("application/vnd.openxmlformats-officedocument.vmlDrawing")));
			}
			const CString operator[] (const CString& sExtension) const
			{
                std::map<CString, CString>::const_iterator it = m_mTable.find(sExtension);
                if (it == m_mTable.end())
                    return _T("");

                return it->second;
			}

		private:
            std::map <CString, CString> m_mTable;
		};
		class CDefault : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDefault)
			CDefault() 
			{
				m_sExtension = _T("");
			}
			CDefault(const CString& sExtension) : m_sExtension(sExtension)
			{
			}
			virtual ~CDefault()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_sExtension = oNode.GetAttribute(_T("Extension"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				static const CExtensionTable oTable;
				
				XmlUtils::CAttribute oAttr;
				oAttr.Write( _T("Extension"),   m_sExtension );
				oAttr.Write( _T("ContentType"), oTable[m_sExtension] );

				return XmlUtils::CreateNode(_T("Default"), oAttr );
			}
			virtual EElementType getType() const
			{
				return et_Default;
			}

			CString	m_sExtension;
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("Extension"), m_sExtension )
				WritingElement_ReadAttributes_End( oReader )
			}


		};
		class COverride : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COverride)
			COverride()
			{
			}
			COverride(const CString& sType, const CPath& oPath) : m_sType(sType), m_oPart(oPath)
			{
			}
			virtual ~COverride()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_oPart = oNode.GetAttribute(_T("PartName"));
				m_sType = oNode.GetAttribute(_T("ContentType"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				CString sPartName = m_oPart.m_strFilename;
				sPartName.Replace(_T("\\"), _T("/"));
				oAttr.Write( _T("PartName"), _T("/") + sPartName);
				oAttr.Write( _T("ContentType"), m_sType );

				return XmlUtils::CreateNode(_T("Override"), oAttr);
			}
			virtual EElementType getType() const
			{
				return et_Override;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("PartName"),    m_oPart )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("ContentType"), m_sType )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			AVSINLINE const CString type() const
			{
				return m_sType;
			}
			AVSINLINE const OOX::CPath filename() const
			{
				return m_oPart;
			}

		private:

			CString						m_sType;
			OOX::CPath					m_oPart;

		};
	} // namespace ContentTypes
} // namespace OOX

namespace OOX
{
    static const CPath c_oContentTypeFileName (_T("[Content_Types].xml"));

	class CContentTypes
	{
	public:
		CContentTypes()
		{
			AddDefault(OOX::CPath(_T(".bin")));
			AddDefault(OOX::CPath(_T(".rels")));
			AddDefault(OOX::CPath(_T(".bmp")));
			AddDefault(OOX::CPath(_T(".jpg")));
			AddDefault(OOX::CPath(_T(".jpeg")));
			AddDefault(OOX::CPath(_T(".jpe")));
			AddDefault(OOX::CPath(_T(".png")));
			AddDefault(OOX::CPath(_T(".gif")));
			AddDefault(OOX::CPath(_T(".emf")));
			AddDefault(OOX::CPath(_T(".wmf")));
			AddDefault(OOX::CPath(_T(".jpeg")));
		}
		CContentTypes(const CPath& oPath)
		{
			Read( oPath );
		}
		~CContentTypes()
		{
            for ( unsigned int nIndex = 0; nIndex < m_arrDefault.size(); nIndex++ )
			{
				if ( m_arrDefault[nIndex] ) delete m_arrDefault[nIndex];
				m_arrDefault[nIndex] = NULL;
			}
			m_arrDefault.clear();
		}

	public:

                bool Read (const CPath& oDirPath)
		{
			OOX::CPath oFullPath = oDirPath / c_oContentTypeFileName;

			XmlUtils::CXmlLiteReader oReader;
			if ( !oReader.FromFile( oFullPath.m_strFilename ) )
                                return false;
			return ReadFromReader(oReader);
		}
                bool ReadFromString (CString& sXml)
		{
			XmlUtils::CXmlLiteReader oReader;
			if ( !oReader.FromString( sXml ) )
                                return false;
			return ReadFromReader(oReader);
		}
                bool Write(const CPath& oDirPath) const
		{
			CString sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">");

			for (unsigned  int nIndex = 0; nIndex < m_arrDefault.size(); nIndex++ )
			{
				if (m_arrDefault[nIndex])
				{
					sXml += m_arrDefault[nIndex]->toXML();
				}
			}


            for (std::map<CString, ContentTypes::COverride>::const_iterator it = m_arrOverride.begin(); it != m_arrOverride.end(); ++it)
            {
                sXml += it->second.toXML();
            }

			sXml += _T("</Types>");

			OOX::CPath oFullPath = oDirPath / c_oContentTypeFileName;
			XmlUtils::SaveToFile( oFullPath.m_strFilename, sXml );

                        return true;
		}

	public:
		void Registration(const CString& sType, const CPath& oDirectory, const CPath& oFilename)
		{
            OOX::CPath oFullPath = oDirectory / oFilename;
			AddOverride( sType, oFullPath.m_strFilename );
			AddDefault ( oFullPath );
		}

		void AddDefault(const OOX::CPath& oPath)
		{
			CString sExt = oPath.GetExtention();
			const CString sExtension = sExt.Mid( 1 );

			size_t nCount = m_arrDefault.size();
			size_t nIndex = 0;	

			while ( nIndex < nCount )
			{
				if (( m_arrDefault[(int) nIndex]) && (m_arrDefault[(int) nIndex]->m_sExtension == sExtension ))
					break;

				++nIndex;
			}

			if ( nIndex == nCount )
				m_arrDefault.push_back(new ContentTypes::CDefault( sExtension ) );
		}

	private:
                bool ReadFromReader (XmlUtils::CXmlLiteReader& oReader)
		{
			CWCharWrapper sName;
			if ( !oReader.ReadNextNode() || _T("Types") != ( sName = oReader.GetName() ) || oReader.IsEmptyNode() )
                                return false;

			int nTypesDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nTypesDepth ) )
			{
				sName = oReader.GetName();

				if ( _T("Default") == sName )
				{
					ContentTypes::CDefault *oDefault = new ContentTypes::CDefault(oReader);
					if (oDefault) m_arrDefault.push_back( oDefault );
				}
				else if ( _T("Override") == sName )
				{
					ContentTypes::COverride oOverride = oReader;
                    m_arrOverride [oOverride.filename().GetPath()] = oOverride;
				}
			}

                        return true;
		}
        void AddOverride(const CString& sType, CString& sPath)
		{
#if !defined(_WIN32) && !defined (_WIN64)
            if (sPath.GetAt(0) ==  FILE_SEPARATOR_CHAR)
            {
                sPath.Delete(0,1);
            }
#endif
			ContentTypes::COverride oOverride( sType, sPath );
            m_arrOverride [oOverride.filename().GetPath()] = oOverride;
		}

	public:

		std::vector<ContentTypes::CDefault*>		m_arrDefault;
        std::map<CString, ContentTypes::COverride>	m_arrOverride;
	};
} // namespace OOX

#endif // OOX_CONTENT_TYPES_INCLUDE_H_

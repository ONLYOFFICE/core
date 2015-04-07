#pragma once
#ifndef PPTX_VMLDRAWING_INCLUDE_H_
#define PPTX_VMLDRAWING_INCLUDE_H_

#include "../../WrapperFile.h"
#include "../../FileContainer.h"

#include "../RId.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/Vml.h"

namespace PPTX
{
	class VmlDrawing : public WrapperFile, public PPTX::FileContainer
	{
	public:
		VmlDrawing()
		{
		}
		VmlDrawing(const OOX::CPath& filename, FileMap& map)
		{
			read( filename, map);

			m_sFilename = filename.GetPath();
		}
		virtual ~VmlDrawing()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			//FileContainer::read(filename, map);

			//так как это не совсем xml - поправим

			CFile file;
			if (file.OpenFile(filename.GetPath()) != S_OK) return;
			int   DataSize = file.GetFileSize();
			BYTE* Data = new BYTE[DataSize];

			std::wstring fileContent;
			if (Data)
			{
				file.ReadFile(Data,DataSize); 
				fileContent = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(Data,DataSize);

				delete []Data;
			}
			file.CloseFile();

			if (fileContent.length() > 0)
			{
				// элементы вида <br> без </br>
				// test_vml4.xlsx
				replace_all(fileContent, _T("<br>"), _T(""));


				// элементы вида <![if ...]>, <![endif]>
				// Zigmunds.pptx
				while(true)
				{
					int res1 = fileContent.find(_T("<!["));
					if (res1 < 0) break;

					int res2 = fileContent.find(_T(">"), res1);

					if (res1 >=0 && res2>=0)
					{
						fileContent = fileContent.erase(res1 ,res2 - res1 + 1);
					}
				}
				read(fileContent);
			}

		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{
			content.registration(type().OverrideType(), directory, filename);
			m_written = false;
			
			//m_WrittenFileName = filename.GetFilename();
			//FileContainer::write(filename, directory, content);
		}

	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::VmlDrawing;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		CString							m_sFilename;
		std::map<CString,CString>		m_mapShapesXml; //связь id (_x0000_s1025) с  xml для OfficeDrawing

	private:
	
		void replace_all(std::wstring& subject, const std::wstring search, const std::wstring replace)
		{
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				 subject.replace(pos, search.length(), replace);
				 pos += replace.length();
			}
		}
		void read(std::wstring & fileContent)
		{
			XmlUtils::CXmlLiteReader oReader;
			
			if ( !oReader.FromString( fileContent))
				return;

			if ( !oReader.ReadNextNode() )
				return;

			CString sName1 = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( _T("xml") == sName1 )
			{
				//ReadAttributes( oReader );

				CString elementContent;
				bool bReadyElement = false;//собираем все до нахождения собственно элемента

				if ( !oReader.IsEmptyNode() )
				{
					int nStylesDepth = oReader.GetDepth();
					while ( oReader.ReadNextSiblingNode( nStylesDepth ) )//
					{
						CString NodeContent = oReader.GetOuterXml();

						CString strXml = _T("<xml xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
 xmlns:p=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" xmlns:oa=\"urn:schemas-microsoft-com:office:activation\">");
								strXml.Append(NodeContent);
								strXml.Append(_T("</xml>"));
								
						XmlUtils::CXmlLiteReader oSubReader;//нам нужны xml //и сами объекты 
						
						if (oSubReader.FromString(strXml) == false) continue;						
						oSubReader.ReadNextNode();

						OOX::WritingElement *pItem = NULL;

						int nStylesDepth1 = oSubReader.GetDepth();
						while ( oSubReader.ReadNextSiblingNode( nStylesDepth1 ) )//
						{
							CWCharWrapper sName = oSubReader.GetName();
							switch (sName[0])// вынесены только объекты .. 
							{
							case 'v':
								{
									wchar_t wChar2 = sName[2]; // v:_
									switch ( wChar2 )
									{
									case 'a':
										if ( _T("v:arc") == sName )
										{
											pItem = new OOX::Vml::CArc(oSubReader);
											bReadyElement = true;

										}
										break;
									case 'c':
										if ( _T("v:curve") == sName )
										{
											pItem = new OOX::Vml::CCurve(oSubReader);
											bReadyElement = true;
										}
										break;
									case 'g':
										if ( _T("v:group") == sName )
										{
											pItem = new OOX::Vml::CGroup(oSubReader);
											bReadyElement = true;
										}
										break;
									case 'i':
										if ( _T("v:image") == sName )
										{
											pItem = new OOX::Vml::CImage( oSubReader );
											bReadyElement = true;
										}
										break;
									case 'l':
										if ( _T("v:line") == sName )
										{
											pItem = new OOX::Vml::CLine(oSubReader);
											bReadyElement = true;
										}
										break;
									case 'o':
										if ( _T("v:oval") == sName )
										{
											pItem = new OOX::Vml::COval(oSubReader);
											bReadyElement = true;
										}
										break;
									case 'p':
										if ( _T("v:polyline") == sName )
										{
											pItem = new OOX::Vml::CPolyLine(oSubReader);
											bReadyElement = true;
										}
										break;
									case 'r':
										if ( _T("v:rect") == sName )
										{
											pItem = new OOX::Vml::CRect(oSubReader);
											bReadyElement = true;
										}
										else if ( _T("v:roundrect") == sName )
										{
											pItem =  new OOX::Vml::CRoundRect(oSubReader);
											bReadyElement = true;
										}
										break;
									case 's':
										if ( _T("v:shape") == sName )
										{
											pItem = new OOX::Vml::CShape(oSubReader);
											bReadyElement = true;
										}
										else if ( _T("v:shapetype") == sName )
										{
											pItem = new OOX::Vml::CShapeType(oSubReader);
										}
										break;
									}								
								}break;
							}
						}	

						elementContent += NodeContent;
						if ( pItem )
						{
							//m_arrItems.push_back( pItem );

							OOX::Vml::CVmlCommonElements* common = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pItem);
							CString sSpid;
							
							if (common)
							{
								if (common->m_sSpId.IsInit())	sSpid = *common->m_sSpId;
								else if (common->m_sId.IsInit())sSpid = *common->m_sId;
							}
							else
							{
								OOX::Vml::CGroup *group = dynamic_cast<OOX::Vml::CGroup*>(pItem);
								if (group)
								{
									if (group->m_sSpId.IsInit())	sSpid = *group->m_sSpId;
									else if (group->m_sId.IsInit()) sSpid = *group->m_sId;
								}
							}
							if (bReadyElement)
							{		
								if (sSpid.GetLength() > 0)
								{
									//m_mapShapes.insert(std::pair<CString,int>(sSpid, m_arrItems.size()-1));		
								
									m_mapShapesXml.insert(std::pair<CString,CString>(sSpid,elementContent));
								}
								elementContent = _T("");
								bReadyElement = false;
							}
							delete pItem;
						}						
					}
				}
			}		
		}
	};
} // namespace PPTX

#endif // PPTX_VmlDrawingS_VmlDrawing_INCLUDE_H_

#pragma once
#ifndef OOX_VMLDRAWING_FILE_INCLUDE_H_
#define OOX_VMLDRAWING_FILE_INCLUDE_H_

//#include ".CommonInclude.h"
//#include "../SharedStrings/Si.h"

#include "IFileContainer.h"
#include "Logic/Vml.h"

#include "../XlsxFormat/Comments/Comments.h"

#include <map>

namespace OOX
{

//в файле VmlDrawing могут быть как отобразительная часть комментариев Xlsx, так и просто обычные объекты 

	class CVmlDrawing : public OOX::WritingElementWithChilds<OOX::WritingElement>, public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CVmlDrawing()
		{
			m_mapComments = NULL;
		}
		CVmlDrawing(const CPath& oRootPath, const CPath& oPath)
		{
			m_mapComments = NULL;
			read( oRootPath, oPath );

			m_sFilename = oPath.GetPath();
		}
		virtual ~CVmlDrawing()
		{
		}
	public:

		virtual void read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void fromXML(XmlUtils::CXmlNode &)
		{
		}
		CString toXML() const
		{
			return _T("");
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
				ReadAttributes( oReader );

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
								
						XmlUtils::CXmlLiteReader oSubReader;//нам нужны xml и сами объекты 
						
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
							m_arrItems.push_back( pItem );

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
									m_mapShapes.insert(std::pair<CString,int>(sSpid, m_arrItems.size()-1));		
								
									m_mapShapesXml.insert(std::pair<CString,CString>(sSpid,elementContent));
								}
								elementContent = _T("");
								bReadyElement = false;
							}
						}						
					}
				}
			}		
		}
		void replace_all(std::wstring& subject, const std::wstring search, const std::wstring replace)
		{
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				 subject.replace(pos, search.length(), replace);
				 pos += replace.length();
			}
		}
		virtual void read(const CPath& oRootPath, const CPath& oPath)
		{
			IFileContainer::Read( oRootPath, oPath );

			//так как это не совсем xml - поправим

			CFile file;
			if (file.OpenFile(oPath.GetPath()) != S_OK) return;
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
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			//for Comment SpreadsheetML only
			if(NULL != m_mapComments && m_mapComments->size() > 0)
			{
				XmlUtils::CStringWriter sXml;
				sXml.WriteString(_T("<xml xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\"><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout><v:shapetype id=\"_x0000_t202\" coordsize=\"21600,21600\" o:spt=\"202\" path=\"m,l,21600r21600,l21600,xe\"><v:stroke joinstyle=\"miter\"/><v:path gradientshapeok=\"t\" o:connecttype=\"rect\"/></v:shapetype>"));
				int nIndex = 1025;

				for (std::map<CString, OOX::Spreadsheet::CCommentItem*>::const_iterator it = m_mapComments->begin(); it != m_mapComments->end(); ++it)
				{
						OOX::Spreadsheet::CCommentItem* comment = it->second;
						CString sStyle;
						if(comment->m_dLeftMM.IsInit())
						{
							SimpleTypes::CPoint oPoint;oPoint.FromMm(comment->m_dLeftMM.get());
                            sStyle.AppendFormat(_T("margin-left:%lspt;"), (const TCHAR *) OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()));
						}
						if(comment->m_dTopMM.IsInit())
						{
							SimpleTypes::CPoint oPoint;oPoint.FromMm(comment->m_dTopMM.get());
                            sStyle.AppendFormat(_T("margin-top:%lspt;"), (const TCHAR *) OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()));
						}
						if(comment->m_dWidthMM.IsInit())
						{
							SimpleTypes::CPoint oPoint;oPoint.FromMm(comment->m_dWidthMM.get());
                            sStyle.AppendFormat(_T("width:%lspt;"), (const TCHAR *) OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()));
						}
						if(comment->m_dHeightMM.IsInit())
						{
							SimpleTypes::CPoint oPoint;oPoint.FromMm(comment->m_dHeightMM.get());
                            sStyle.AppendFormat(_T("height:%lspt;"), (const TCHAR *) OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()));
						}
						CString sClientData;
						sClientData.Append(_T("<x:ClientData ObjectType=\"Note\">"));

						if(comment->m_bMove.IsInit() && true == comment->m_bMove.get())
							sClientData.Append(_T("<x:MoveWithCells/>"));

						if(comment->m_bSize.IsInit() && true == comment->m_bSize.get())
							sClientData.Append(_T("<x:SizeWithCells/>"));

						if(comment->m_nLeft.IsInit() && comment->m_nLeftOffset.IsInit() &&
							comment->m_nTop.IsInit() && comment->m_nTopOffset.IsInit() &&
							comment->m_nRight.IsInit() && comment->m_nRightOffset.IsInit() && 
							comment->m_nBottom.IsInit() && comment->m_nBottomOffset.IsInit())
						{
								sClientData.AppendFormat(_T("<x:Anchor>%d, %d, %d, %d, %d, %d, %d, %d</x:Anchor>"), 
								comment->m_nLeft.get(), comment->m_nLeftOffset.get(), comment->m_nTop.get(), comment->m_nTopOffset.get(),
								comment->m_nRight.get(), comment->m_nRightOffset.get(), comment->m_nBottom.get(), comment->m_nBottomOffset.get());
						}
						sClientData.Append(_T("<x:AutoFill>False</x:AutoFill>"));

						if(comment->m_nRow.IsInit())
							sClientData.AppendFormat(_T("<x:Row>%d</x:Row>"), comment->m_nRow.get());

						if(comment->m_nCol.IsInit())
							sClientData.AppendFormat(_T("<x:Column>%d</x:Column>"), comment->m_nCol.get());

						sClientData.Append(_T("</x:ClientData>"));

						CString sGfxdata;
						if(comment->m_sGfxdata.IsInit())
							sGfxdata.Format(_T("o:gfxdata=\"%ls\""), comment->m_sGfxdata.get2());
						CString sShape; 
						sShape.Format(_T("<v:shape id=\"_x0000_s%d\" type=\"#_x0000_t202\" style='position:absolute;"),nIndex);
						sShape.Append(sStyle);
						sShape.Append(_T("z-index:4;visibility:hidden' "));
						sShape.Append(sGfxdata);
						sShape.AppendFormat(_T(" fillcolor=\"#ffffe1\" o:insetmode=\"auto\"><v:fill color2=\"#ffffe1\"/><v:shadow on=\"t\" color=\"black\" obscured=\"t\"/><v:path o:connecttype=\"none\"/><v:textbox style='mso-direction-alt:auto'><div style='text-align:left'></div></v:textbox>"));
						sShape.Append(sClientData);
						sShape.Append(_T("</v:shape>"));
						sXml.WriteString(sShape);
						nIndex++;
				}
				sXml.WriteString(_T("</xml>"));

				CDirectory::SaveToFile( oPath.GetPath(), sXml.GetData() );
				oContent.AddDefault( oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
		}
		virtual const OOX::FileType type() const
		{
			return OOX::FileTypes::VmlDrawing;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& GetReadPath()
		{
			return m_sFilename;
		}
	private:
	
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

	public:
		std::map<CString, OOX::Spreadsheet::CCommentItem*>*		m_mapComments;
		std::map<CString, int>									m_mapShapes; //связь id (_x0000_s1025) с номером объекта  для комментов
		std::map<CString,CString>								m_mapShapesXml; //связь id (_x0000_s1025) с  xml для OfficeDrawing
	};
} // namespace OOX

#endif // OOX_VMLDRAWING_FILE_INCLUDE_H_

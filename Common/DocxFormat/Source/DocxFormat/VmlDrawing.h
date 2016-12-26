/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
		CVmlDrawing(bool bSpreadsheet_ = false)
		{
			bSpreadsheet	= bSpreadsheet_;
			m_mapComments	= NULL;
			m_lObjectIdVML = 0;
		}
		CVmlDrawing(const CPath& oRootPath, const CPath& oPath)
		{
			m_mapComments = NULL;
			read( oRootPath, oPath );
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

			std::wstring sName1 = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( _T("xml") == sName1 )
			{
				ReadAttributes( oReader );

                                std::wstring elementContent;
				bool bReadyElement = false;//собираем все до нахождения собственно элемента

				if ( !oReader.IsEmptyNode() )
				{
					int nStylesDepth = oReader.GetDepth();
					while ( oReader.ReadNextSiblingNode( nStylesDepth ) )//
					{
                                                std::wstring NodeContent = oReader.GetOuterXml();

                                                std::wstring strXml = L"<xml xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
 xmlns:p=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" xmlns:oa=\"urn:schemas-microsoft-com:office:activation\">";
                                                            strXml += NodeContent;
                                                            strXml += L"</xml>";
								
						XmlUtils::CXmlLiteReader oSubReader;//нам нужны xml и сами объекты 
						
						if (oSubReader.FromString(strXml) == false) continue;						
						oSubReader.ReadNextNode();

						OOX::WritingElement *pItem = NULL;

						int nStylesDepth1 = oSubReader.GetDepth();
						while ( oSubReader.ReadNextSiblingNode( nStylesDepth1 ) )//
						{
							std::wstring sName = oSubReader.GetName();
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
                                                        std::wstring sSpid;
							
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
                                                                if (!sSpid.empty())
								{
                                                                    m_mapShapes.insert(std::pair<std::wstring,int>(sSpid, m_arrItems.size()-1));
								
                                                                    m_mapShapesXml.insert(std::pair<std::wstring,std::wstring>(sSpid, elementContent));
								}
                                                                elementContent.clear();
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
			m_oReadPath = oPath;
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
			if((NULL != m_mapComments && m_mapComments->size() > 0) || m_aXml.size() > 0)
			{
				XmlUtils::CStringWriter sXml;
                                sXml.WriteString(L"<xml xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\"><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout><v:shapetype id=\"_x0000_t202\" coordsize=\"21600,21600\" o:spt=\"202\" path=\"m,l,21600r21600,l21600,xe\"><v:stroke joinstyle=\"miter\"/><v:path gradientshapeok=\"t\" o:connecttype=\"rect\"/></v:shapetype>");
				for (size_t i = 0; i < m_aXml.size(); ++i)
				{
                                    sXml.WriteString(m_aXml[i]);
				}
				long nIndex = m_lObjectIdVML;
				if(NULL != m_mapComments && m_mapComments->size() > 0)
				{
					for (std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = m_mapComments->begin(); it != m_mapComments->end(); ++it)
					{
						OOX::Spreadsheet::CCommentItem* comment = it->second;
                                                std::wstring sStyle;
						if(comment->m_dLeftMM.IsInit())
						{
                                                    SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dLeftMM.get());
                                                    sStyle += L"margin-left:" + OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()) + L"pt;";
						}
						if(comment->m_dTopMM.IsInit())
						{
                                                    SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dTopMM.get());
                                                    sStyle += L"margin-top:" + OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()) + L"pt;";
						}
						if(comment->m_dWidthMM.IsInit())
						{
                                                    SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dWidthMM.get());
                                                    sStyle += L"width:" + OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()) + L"pt;";
						}
						if(comment->m_dHeightMM.IsInit())
						{
                                                    SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dHeightMM.get());
                                                    sStyle += L"height:" + OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oPoint.ToPoints()) + L"pt;";
						}
                                                std::wstring sClientData = L"<x:ClientData ObjectType=\"Note\">";

						if(comment->m_bMove.IsInit() && true == comment->m_bMove.get())
                                                    sClientData += L"<x:MoveWithCells/>";

						if(comment->m_bSize.IsInit() && true == comment->m_bSize.get())
                                                    sClientData += L"<x:SizeWithCells/>";

                                                if( comment->m_nLeft.IsInit()   && comment->m_nLeftOffset.IsInit()  &&
                                                    comment->m_nTop.IsInit()    && comment->m_nTopOffset.IsInit()   &&
                                                    comment->m_nRight.IsInit()  && comment->m_nRightOffset.IsInit() &&
                                                    comment->m_nBottom.IsInit() && comment->m_nBottomOffset.IsInit())
						{
                                                    sClientData += L"<x:Anchor>";
                                                    sClientData += std::to_wstring(comment->m_nLeft.get())          + L",";
                                                    sClientData += std::to_wstring(comment->m_nLeftOffset.get())    + L",";
                                                    sClientData += std::to_wstring(comment->m_nTop.get())           + L",";
                                                    sClientData += std::to_wstring(comment->m_nTopOffset.get())     + L",";
                                                    sClientData += std::to_wstring(comment->m_nRight.get())         + L",";
                                                    sClientData += std::to_wstring(comment->m_nRightOffset.get())   + L",";
                                                    sClientData += std::to_wstring(comment->m_nBottom.get())        + L",";
                                                    sClientData += std::to_wstring(comment->m_nBottomOffset.get())  + L",";
                                                    sClientData += L"</x:Anchor>";
						}
                                                sClientData += L"<x:AutoFill>False</x:AutoFill>";

						if(comment->m_nRow.IsInit())
                                                    sClientData += L"<x:Row>" + std::to_wstring(comment->m_nRow.get()) + L"</x:Row>";

						if(comment->m_nCol.IsInit())
                                                    sClientData += L"<x:Column>" + std::to_wstring(comment->m_nCol.get()) + L"</x:Column>";

                                                sClientData += L"</x:ClientData>";

                                                std::wstring sGfxdata;
						if(comment->m_sGfxdata.IsInit())
                                                    sGfxdata = L"o:gfxdata=\"" + comment->m_sGfxdata.get2() + L"\"";

                                                std::wstring sShape;
                                                sShape += L"<v:shape id=\"_x0000_s" + std::to_wstring(nIndex++) + L" \" type=\"#_x0000_t202\" style='position:absolute;";
                                                sShape += sStyle;
                                                sShape += L"z-index:4;visibility:hidden' ";
                                                sShape += sGfxdata;
                                                sShape += L" fillcolor=\"#ffffe1\" o:insetmode=\"auto\"><v:fill color2=\"#ffffe1\"/><v:shadow on=\"t\" color=\"black\" obscured=\"t\"/><v:path o:connecttype=\"none\"/><v:textbox style='mso-direction-alt:auto'><div style='text-align:left'></div></v:textbox>";
                                                sShape += sClientData;
                                                sShape += L"</v:shape>";
						sXml.WriteString(sShape);
					}
				}
                                sXml.WriteString(L"</xml>");

				CDirectory::SaveToFile( oPath.GetPath(), sXml.GetData() );
				oContent.AddDefault( oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
		}
		virtual const OOX::FileType type() const
		{
			if (bSpreadsheet)	return OOX::Spreadsheet::FileTypes::VmlDrawing;
                        else			return OOX::FileTypes::VmlDrawing;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath GetReadPath()
		{
			return m_oReadPath;
		}
	private:
	
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		bool bSpreadsheet;

	public:
		CPath m_oReadPath;

                std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>*    m_mapComments;
                std::map<std::wstring, int>                                 m_mapShapes;    //связь id (_x0000_s1025) с номером объекта  для комментов
                std::map<std::wstring,std::wstring>                         m_mapShapesXml; //связь id (_x0000_s1025) с  xml для OfficeDrawing
                std::vector<std::wstring>                                   m_aXml;
                long                                                        m_lObjectIdVML;
	};
} // namespace OOX

#endif // OOX_VMLDRAWING_FILE_INCLUDE_H_

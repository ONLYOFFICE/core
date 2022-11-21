/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "VmlDrawing.h"
#include <boost/lexical_cast.hpp>
#include "../../DesktopEditor/common/StringBuilder.h"

namespace OOX
{

//в файле VmlDrawing могут быть как отобразительная часть комментариев Xlsx, так и просто обычные объекты 

	void CVmlDrawing::ClearShapeTypes()
	{
		for ( size_t i = 0; i < m_arrShapeTypes.size(); ++i)
		{
			if ( m_arrShapeTypes[i].pElement ) delete m_arrShapeTypes[i].pElement;
		}
		m_arrShapeTypes.clear();
	}

	void CVmlDrawing::read(std::wstring & fileContent)
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
            bool bReadyElement  = false;//собираем все до нахождения собственно элемента

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
                        bool bComment = false;

						if (common)
						{
							if (common->m_sSpId.IsInit())	sSpid = *common->m_sSpId;
							else if (common->m_sId.IsInit())sSpid = *common->m_sId;

                            bComment = common->m_bComment;
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
								_vml_shape element;
								
								element.nId			= (int)m_arrItems.size()-1;
								element.sXml		= elementContent;
								element.pElement	= pItem;
                                element.bUsed       = bComment;

								m_mapShapes.insert(std::make_pair(sSpid, element));
							}
							elementContent.clear();
                            bReadyElement   = false;
                            bComment        = false;
						}
						else
						{
							_vml_shape element;
							element.nId			= -1;
							element.sXml		= elementContent;
							element.pElement	= pItem;
							element.bUsed       = false;
							m_arrShapeTypes.push_back(element);
						}
					}						
				}
			}
		}		
	}
	void CVmlDrawing::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

		//так как это не совсем xml - поправим

		NSFile::CFileBinary file;
		if (file.OpenFile(oPath.GetPath()))
		{
			DWORD   DataSize = file.GetFileSize();
			BYTE* Data = new BYTE[DataSize];

			if (Data)
			{
				file.ReadFile(Data, DataSize, DataSize); 
				m_sFileContent = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(Data,DataSize);

				delete []Data;
			}
			file.CloseFile();
		}


		if (false == m_sFileContent.empty())
		{
			// элементы вида <br> без </br>
			// test_vml4.xlsx
			XmlUtils::replace_all(m_sFileContent, _T("<br>"), _T(""));


			// элементы вида <![if ...]>, <![endif]>
			// Zigmunds.pptx
			while(true)
			{
				int res1 = (int)m_sFileContent.find(_T("<!["));
				if (res1 < 0) break;

				int res2 = (int)m_sFileContent.find(_T(">"), res1);

				if (res1 >=0 && res2>=0)
				{
					m_sFileContent = m_sFileContent.erase(res1 ,res2 - res1 + 1);
				}
			}
			read(m_sFileContent);
		}
	}
	void CVmlDrawing::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if ((!m_mapComments || m_mapComments->empty()) && m_arObjectXml.empty() && m_arControlXml.empty())
		{
			if (false == m_sFileContent.empty())
			{
				NSFile::CFileBinary::SaveToFile(oPath.GetPath(), m_sFileContent);
				oContent.AddDefault(oPath.GetExtention(false));
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
			return;
		}
		
		//for Comment SpreadsheetML & others vml (hf, objects, ...) !!

		NSStringUtils::CStringBuilder sXml;
		sXml.WriteString(L"<xml \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:x=\"urn:schemas-microsoft-com:office:excel\">");
		
		for (size_t i = 0; i < m_arObjectXml.size(); ++i)
		{
			sXml.WriteString(m_arObjectXml[i]);
		}
		
		if (false == m_arControlXml.empty() || ((NULL != m_mapComments) && (false == m_mapComments->empty())))
		{
			int data = (int)((m_lObjectIdVML + 4096) / 1024);
			sXml.WriteString(L"<o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"" + std::to_wstring(data) + L"\"/></o:shapelayout>");
		}
		
		if (false == m_arControlXml.empty())
		{

			sXml.WriteString(L"<v:shapetype id=\"_x0000_t201\" coordsize=\"21600,21600\" o:spt=\"201\"");
			sXml.WriteString(L" path=\"m,l,21600r21600,l21600,xe\"><v:stroke joinstyle=\"miter\"/>");
			sXml.WriteString(L"<v:path shadowok=\"f\" o:extrusionok=\"f\" strokeok=\"f\" fillok=\"f\" o:connecttype=\"rect\"/>");
			sXml.WriteString(L"<o:lock v:ext=\"edit\" shapetype=\"t\"/></v:shapetype>");

			for (size_t i = 0; i < m_arControlXml.size(); ++i)
			{
				sXml.WriteString(m_arControlXml[i]);
			}
		}
		
		long nIndex = 4096 + m_lObjectIdVML;
		if ((NULL != m_mapComments) && (false == m_mapComments->empty()))
		{				
			sXml.WriteString(L"<v:shapetype id=\"_x0000_t202\" coordsize=\"21600,21600\" o:spt=\"202\"");
			sXml.WriteString(L" path=\"m,l,21600r21600,l21600,xe\">");
			sXml.WriteString(L"<v:stroke joinstyle=\"miter\"/><v:path gradientshapeok=\"t\" o:connecttype=\"rect\"/></v:shapetype>");
           
			for (std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = m_mapComments->begin(); it != m_mapComments->end(); ++it)
			{
				OOX::Spreadsheet::CCommentItem* comment = it->second;
                                        
				std::wstring sStyle;
				if(comment->m_dLeftMM.IsInit())
				{
					SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dLeftMM.get());
					sStyle += L"margin-left:" + XmlUtils::ToString(oPoint.ToPoints()) + L"pt;";
				}
				if(comment->m_dTopMM.IsInit())
				{
					SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dTopMM.get());
					sStyle += L"margin-top:" + XmlUtils::ToString(oPoint.ToPoints()) + L"pt;";
				}
				if(comment->m_dWidthMM.IsInit())
				{
					SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dWidthMM.get());
					sStyle += L"width:" + XmlUtils::ToString(oPoint.ToPoints()) + L"pt;";
				}
				if(comment->m_dHeightMM.IsInit())
				{
					SimpleTypes::CPoint oPoint; oPoint.FromMm(comment->m_dHeightMM.get());
					sStyle += L"height:" + XmlUtils::ToString(oPoint.ToPoints()) + L"pt;";
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
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nLeft.get())          + L",";
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nLeftOffset.get())    + L",";
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nTop.get())           + L",";
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nTopOffset.get())     + L",";
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nRight.get())         + L",";
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nRightOffset.get())   + L",";
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nBottom.get())        + L",";
					sClientData += boost::lexical_cast<std::wstring>(comment->m_nBottomOffset.get());
					sClientData += L"</x:Anchor>";
				}
				sClientData += L"<x:AutoFill>False</x:AutoFill>";

				if(comment->m_nRow.IsInit())
						sClientData += L"<x:Row>" + boost::lexical_cast<std::wstring>(comment->m_nRow.get()) + L"</x:Row>";

				if(comment->m_nCol.IsInit())
					sClientData += L"<x:Column>" + boost::lexical_cast<std::wstring>(comment->m_nCol.get()) + L"</x:Column>";

				sClientData += L"</x:ClientData>";

				std::wstring sGfxdata;
				if(comment->m_sGfxdata.IsInit())
					sGfxdata = L"o:gfxdata=\"" + *comment->m_sGfxdata + L"\"";

				std::wstring sShape;
				sShape += L"<v:shape id=\"_x0000_s" + boost::lexical_cast<std::wstring>(nIndex++) + L" \" type=\"#_x0000_t202\" style='position:absolute;";
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

        NSFile::CFileBinary::SaveToFile( oPath.GetPath(), sXml.GetData() );
		oContent.AddDefault( oPath.GetExtention(false) );
		IFileContainer::Write(oPath, oDirectory, oContent);
	}
	bool CVmlDrawing::IsEmpty()
	{
		return m_arObjectXml.empty() && m_arControlXml.empty() && (!m_mapComments || ((m_mapComments) && (m_mapComments->empty())));
	}
	smart_ptr<OOX::WritingElement> CVmlDrawing::FindVmlObject(const std::wstring &spid)
	{
		smart_ptr<OOX::WritingElement> oElement;
		
		boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pFind = m_mapShapes.find(spid);
		
		if (pFind != m_mapShapes.end())
		{
			oElement.reset(pFind->second.pElement);
			oElement.AddRef();
		}			
		return oElement;
	}
} // namespace OOX

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
#pragma once
#ifndef OOX_DRAWING_FILE_INCLUDE_H_
#define OOX_DRAWING_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "CellAnchor.h"

namespace OOX
{
	using namespace Drawing;

	namespace Spreadsheet
	{
		class CDrawingWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDrawingWorksheet)
			CDrawingWorksheet()
			{
			}
			virtual ~CDrawingWorksheet()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oId.IsInit())
				{
					writer.WriteString(L"<drawing r:id=\"");
					writer.WriteString(m_oId->ToString());
					writer.WriteString(L"\"/>");
				}
				
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_FromTo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"), m_oId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};

		class CDrawing : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CDrawing(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CDrawing()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( _T("wsDr") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nCurDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nCurDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							CCellAnchor *pItem = NULL;

							if ( _T("absoluteAnchor") == sName )
							{
								pItem = new CCellAnchor( oReader );
								pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorAbsolute);
							}
							else if ( _T("oneCellAnchor") == sName )
							{
								pItem = new CCellAnchor( oReader );
								pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorOneCell);
							}
							else if ( _T("twoCellAnchor") == sName )
							{
								pItem = new CCellAnchor( oReader );
								pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
							}
							else if ( _T("AlternateContent") == sName)
							{
								nCurDepth++;
								while( oReader.ReadNextSiblingNode( nCurDepth ) )
								{
									sName = XmlUtils::GetNameNoNS(oReader.GetName());
									if ( _T("Choice") != sName  && _T("Fallback") != sName ) continue;
		
                                    nullable<std::wstring> sRequires;
									WritingElement_ReadAttributes_Start( oReader )
										WritingElement_ReadAttributes_Read_if ( oReader, _T("Requires"), sRequires )
									WritingElement_ReadAttributes_End( oReader )
									
									if (sRequires.IsInit() == false) continue;
									if (*sRequires != _T("a14"))continue;									

									nCurDepth++;
									while( oReader.ReadNextSiblingNode( nCurDepth ) )
									{
										sName = XmlUtils::GetNameNoNS(oReader.GetName());
										if ( _T("absoluteAnchor") == sName )
										{
											pItem = new CCellAnchor( oReader );
											pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorAbsolute);
										}
										else if ( _T("oneCellAnchor") == sName )
										{
											pItem = new CCellAnchor( oReader );
											pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorOneCell);
										}
										else if ( _T("twoCellAnchor") == sName )
										{
											pItem = new CCellAnchor( oReader );
											pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
										}									
									}nCurDepth--;
									break;
								}
								nCurDepth--;
							}

							if ( pItem )
								m_arrItems.push_back( pItem );
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><xdr:wsDr xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">"));
				for(size_t i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(sXml);
				sXml.WriteString(_T("</xdr:wsDr>"));				

                std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath.c_str(), sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Drawings;
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
				return m_oReadPath;
			}

			bool IsEmpty()
			{
				return m_arrItems.empty();
			}

		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
			void ClearItems()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.clear();
			}

		public:
			std::vector<CCellAnchor *>	m_arrItems;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_DRAWING_FILE_INCLUDE_H_

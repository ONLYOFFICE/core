/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "IFileContainer.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTreeElem.h"
#include "../XlsxFormat/WritingElement.h"

namespace OOX
{
	class CFromTo : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CFromTo)
		CFromTo()
		{
		}
		virtual ~CFromTo()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& node)
		{
		}
        virtual std::wstring toXML() const
		{
			return L"";
		}
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML2(writer, m_sName);
		}
		virtual void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sName);
			writer.WriteString(L">");

			if(m_oX.IsInit())
			{
				WritingStringValDouble(L"cdr:x", m_oX.get());
			}
			if(m_oY.IsInit())
			{
				WritingStringValDouble(L"cdr:y", m_oY.get());
			}
			writer.WriteString(L"</");
			writer.WriteString(sName);
			writer.WriteString(L">");
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			m_sName = oReader.GetName();
			
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("x") == sName )
					m_oX = oReader.GetText2();
				else if ( _T("y") == sName )
					m_oY = oReader.GetText2();
			}
		}

		virtual EElementType getType () const
		{
			return et_cdr_FromTo;
		}

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
	public:
		std::wstring	m_sName;
		nullable_double	m_oX;
		nullable_double	m_oY;
	};
	class CExt : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CExt)
		CExt()
		{
		}
		virtual ~CExt()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& node)
		{
		}
        virtual std::wstring toXML() const
		{
			return L"";
		}
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<cdr:ext>");

			if(m_oCx.IsInit())
			{
				WritingStringValDouble(L"cdr:cx", m_oCx.get());
			}
			if(m_oCy.IsInit())
			{
				WritingStringValDouble(L"cdr:cy", m_oCy.get());
			}
			writer.WriteString(L"</cdr:ext>");
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("cx") == sName )
					m_oCx = oReader.GetText2();
				else if ( _T("cy") == sName )
					m_oCy = oReader.GetText2();
			}
		}

		virtual EElementType getType () const
		{
			return et_cdr_FromTo;
		}

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
	public:
		nullable_double	m_oCx;
		nullable_double	m_oCy;
	};
	class CSizeAnchor : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CSizeAnchor)
        CSizeAnchor()
		{
		}
		virtual ~CSizeAnchor()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& node) = 0;
        virtual std::wstring toXML() const = 0;
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const = 0;
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) = 0;
		virtual EElementType getType () const = 0;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		nullable<OOX::CFromTo>				m_oFrom;
		nullable<PPTX::Logic::SpTreeElem>	m_oElement;
	};
	class CRelSizeAnchor : public CSizeAnchor
	{
	public:
		WritingElement_AdditionConstructors(CRelSizeAnchor)
        CRelSizeAnchor()
		{
		}
		virtual ~CRelSizeAnchor()
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
			writer.WriteString(L"<cdr:relSizeAnchor>");
			if (m_oFrom.IsInit())
			{
				m_oFrom->m_sName = L"cdr:from";
				m_oFrom->toXML(writer);
			}
			if (m_oTo.IsInit())
			{
				m_oTo->m_sName = L"cdr:to";
				m_oTo->toXML(writer);
			}
			if (m_oElement.IsInit())
			{
				NSBinPptxRW::CXmlWriter xmlWriter;
				xmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_CHART_DRAWING;
				
				m_oElement->toXmlWriter(&xmlWriter);
			
				writer.WriteString(xmlWriter.GetXmlString());
			}
			writer.WriteString(L"</cdr:relSizeAnchor>");
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"from" == sName )
					m_oFrom = oReader;
				else if ( L"to" == sName )
					m_oTo = oReader;
                else if (L"graphicFrame" == sName || L"pic" == sName || L"sp" == sName || L"grpSp" == sName || L"cxnSp" == sName || L"AlternateContent" == sName)
				{	
					//Demo-2010WinterOlympics2.xlsx - AlternateContent
					m_oElement = oReader;

					if (m_oElement.IsInit())
					{
						//smart_ptr<PPTX::Logic::Shape> shape = m_oElement->GetElem().smart_dynamic_cast<PPTX::Logic::Shape>();
						//if (shape.IsInit())
						//{
						//	m_nId = shape->nvSpPr.cNvPr.id;
						//	if (shape->nvSpPr.cNvPr.vmlSpid.IsInit())
						//	{
						//		//ссылка на объект или шейп в vmlDrawing
						//		m_sVmlSpId = shape->nvSpPr.cNvPr.vmlSpid.get();
						//	}
						//}
						//smart_ptr<PPTX::Logic::GraphicFrame> frame = m_oElement->GetElem().smart_dynamic_cast<PPTX::Logic::GraphicFrame>();
						//if ((frame.IsInit()) && (frame->vmlSpid.IsInit()))
						//{
						//	//ссылка на объект или шейп в vmlDrawing
						//	m_sVmlSpId = frame->vmlSpid.get();
						//}
					}
				}
			}
		}

		virtual EElementType getType () const
		{
			return et_cdr_RelSizeAnchor;
		}

		nullable<OOX::CFromTo> m_oTo;
	};

	class CAbsSizeAnchor : public CSizeAnchor
	{
	public:
		WritingElement_AdditionConstructors(CAbsSizeAnchor)
        CAbsSizeAnchor()
		{
		}
		virtual ~CAbsSizeAnchor()
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
			writer.WriteString(L"<cdr:absSizeAnchor>");
			if (m_oFrom.IsInit())
			{
				m_oFrom->m_sName = L"cdr:from";
				m_oFrom->toXML(writer);
			}
			if (m_oExt.IsInit())
			{
				m_oExt->toXML(writer);
			}
			if (m_oElement.IsInit())
			{
				NSBinPptxRW::CXmlWriter xmlWriter;
				xmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_CHART_DRAWING;
				
				m_oElement->toXmlWriter(&xmlWriter);
			
				writer.WriteString(xmlWriter.GetXmlString());
			}
			writer.WriteString(L"</cdr:absSizeAnchor>");
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"from" == sName )
					m_oFrom = oReader;
				else if ( L"ext" == sName )
					m_oExt = oReader;
                else if (L"graphicFrame" == sName || L"pic" == sName || L"sp" == sName || L"grpSp" == sName || L"cxnSp" == sName || L"AlternateContent" == sName)
				{	
					//Demo-2010WinterOlympics2.xlsx - AlternateContent
					m_oElement = oReader;

					if (m_oElement.IsInit())
					{
						//smart_ptr<PPTX::Logic::Shape> shape = m_oElement->GetElem().smart_dynamic_cast<PPTX::Logic::Shape>();
						//if (shape.IsInit())
						//{
						//	m_nId = shape->nvSpPr.cNvPr.id;
						//	if (shape->nvSpPr.cNvPr.vmlSpid.IsInit())
						//	{
						//		//ссылка на объект или шейп в vmlDrawing
						//		m_sVmlSpId = shape->nvSpPr.cNvPr.vmlSpid.get();
						//	}
						//}
						//smart_ptr<PPTX::Logic::GraphicFrame> frame = m_oElement->GetElem().smart_dynamic_cast<PPTX::Logic::GraphicFrame>();
						//if ((frame.IsInit()) && (frame->vmlSpid.IsInit()))
						//{
						//	//ссылка на объект или шейп в vmlDrawing
						//	m_sVmlSpId = frame->vmlSpid.get();
						//}
					}
				}
			}
		}
		virtual EElementType getType () const
		{
			return et_cdr_AbsSizeAnchor;
		}

		nullable<OOX::CExt> m_oExt;
	};

	class CChartDrawing : public OOX::WritingElementWithChilds<OOX::CSizeAnchor>, public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CChartDrawing(OOX::Document* pMain, bool bDocument = true) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = bDocument;
		}
		CChartDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = false;
			read( oRootPath, oPath );
		}
		virtual ~CChartDrawing()
		{
		}
		virtual void ClearItems()
		{
		}
		virtual void read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void fromXML(XmlUtils::CXmlNode &)
		{
		}
		std::wstring toXML() const
		{
			return _T("");
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
			if ( L"userShapes" != sName ) return;
			
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
			{
				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = XmlUtils::GetNameNoNS(oReader.GetName());

					CSizeAnchor *pItem = NULL;

					if ( L"relSizeAnchor" == sName )
					{
						pItem = new CRelSizeAnchor( oReader );
					}
					else if ( L"absSizeAnchor" == sName ) 
					{
						pItem = new CAbsSizeAnchor( oReader );
					}
					if ( pItem )
						m_arrItems.push_back( pItem );
				}
			}
		}
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<c:userShapes \
xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" \
xmlns:cdr=\"http://schemas.openxmlformats.org/drawingml/2006/chartDrawing\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"\
>");
			
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				m_arrItems[i]->toXML(sXml);
			}
			sXml.WriteString(L"</c:userShapes>");

            NSFile::CFileBinary::SaveToFile( oPath.GetPath(), sXml.GetData() );
			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );

			IFileContainer::Write(oPath, oDirectory, oContent);
		}
		virtual const OOX::FileType type() const
		{
			return OOX::FileTypes::ChartDrawing;
		}
		virtual const CPath DefaultDirectory() const
		{
			//if (m_bDocument) return type().DefaultDirectory();
			//else	
				return L"../" + type().DefaultDirectory();
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

		bool m_bDocument;

	public:
//reading
        CPath	m_oReadPath;
	};
} // namespace OOX

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

#include "ChartDrawing.h"

namespace OOX
{	
	CFromTo::CFromTo()
	{
	}
	CFromTo::~CFromTo()
	{
	}
	void CFromTo::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CFromTo::toXML() const
	{
		return L"";
	}
	void CFromTo::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		toXML2(writer, m_sName);
	}
	void CFromTo::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
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
	void CFromTo::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
	EElementType CFromTo::getType () const
	{
		return et_cdr_FromTo;
	}
	void CFromTo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
	}

	CExt::CExt()
	{
	}
	CExt::~CExt()
	{
	}
	void CExt::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CExt::toXML() const
	{
		return L"";
	}
	void CExt::toXML(NSStringUtils::CStringBuilder& writer) const
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
	void CExt::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
	EElementType CExt::getType () const
	{
		return et_cdr_FromTo;
	}
	void CExt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
	}

	CSizeAnchor::CSizeAnchor()
	{
	}
	CSizeAnchor::~CSizeAnchor()
	{
	}
	void CSizeAnchor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
	}

	CRelSizeAnchor::CRelSizeAnchor()
	{
	}
	CRelSizeAnchor::~CRelSizeAnchor()
	{
	}
	void CRelSizeAnchor::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRelSizeAnchor::toXML() const
	{
		return _T("");
	}
	void CRelSizeAnchor::toXML(NSStringUtils::CStringBuilder& writer) const
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
	void CRelSizeAnchor::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
	EElementType CRelSizeAnchor::getType () const
	{
		return et_cdr_RelSizeAnchor;
	}

	CAbsSizeAnchor::CAbsSizeAnchor()
	{
	}
	CAbsSizeAnchor::~CAbsSizeAnchor()
	{
	}
	void CAbsSizeAnchor::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CAbsSizeAnchor::toXML() const
	{
		return _T("");
	}
	void CAbsSizeAnchor::toXML(NSStringUtils::CStringBuilder& writer) const
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
	void CAbsSizeAnchor::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
	EElementType CAbsSizeAnchor::getType () const
	{
		return et_cdr_AbsSizeAnchor;
	}

	CChartDrawing::CChartDrawing(OOX::Document* pMain, bool bDocument) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		m_bDocument = bDocument;
	}
	CChartDrawing::CChartDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		m_bDocument = (NULL != dynamic_cast<OOX::CDocument*>(pMain));
		read( oRootPath, oPath );
	}
	CChartDrawing::~CChartDrawing()
	{
	}
	void CChartDrawing::ClearItems()
	{
	}
	void CChartDrawing::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	void CChartDrawing::fromXML(XmlUtils::CXmlNode &)
	{
	}
	std::wstring CChartDrawing::toXML() const
	{
		return _T("");
	}
	void CChartDrawing::read(const CPath& oRootPath, const CPath& oPath)
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
	void CChartDrawing::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
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
	const OOX::FileType CChartDrawing::type() const
	{
		return OOX::FileTypes::ChartDrawing;
	}
	const CPath CChartDrawing::DefaultDirectory() const
	{
		//if (m_bDocument) return type().DefaultDirectory();
		//else
			return L"../" + type().DefaultDirectory();
	}
	const CPath CChartDrawing::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath CChartDrawing::GetReadPath()
	{
		return m_oReadPath;
	}
	void CChartDrawing::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

} // namespace OOX

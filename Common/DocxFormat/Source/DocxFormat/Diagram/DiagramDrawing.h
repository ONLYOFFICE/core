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

#include "../IFileContainer.h"
#include "../FileTypes.h"
#include "../File.h"
#include "../../Base/Nullable.h"

#include "../WritingElement.h"

#include "../../../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTree.h"
//#include "../Drawing/DrawingCoreInfo.h"
//#include "../Drawing/DrawingText.h"


namespace OOX
{
	//namespace Diagram
	//{
	//	//--------------------------------------------------------------------------------
	//	//		?????? sp (Shape)
	//	//--------------------------------------------------------------------------------
	//	class CShape : public WritingElement
	//	{
	//	public:
	//		CShape(XmlUtils::CXmlNode &oNode)
	//		{
	//			fromXML( oNode );
	//		}
	//		CShape(XmlUtils::CXmlLiteReader& oReader)
	//		{
	//			fromXML( oReader );
	//		}
	//		CShape()
	//		{
	//		}
	//		virtual ~CShape()
	//		{
	//		}

	//	public:
	//		virtual std::wstring      toXML() const
	//		{
	//			return _T("");
	//		}
	//		virtual void toXML(NSStringUtils::CStringBuilder& writer) const
	//		{
	//		}
	//		virtual void         fromXML(XmlUtils::CXmlNode& node)
	//		{
	//		}
	//		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
	//		{
	//			ReadAttributes( oReader );

	//			if ( oReader.IsEmptyNode() )
	//				return;

	//			int nCurDepth = oReader.GetDepth();
	//			while( oReader.ReadNextSiblingNode( nCurDepth ) )
	//			{
	//				std::wstring sName = oReader.GetName();

	//				if ( _T("dsp:spPr") == sName )
	//					m_oSpPr = oReader;
	//				//else if ( _T("dsp:nvSpPr") == sName )
	//				//	m_oNvSpPr = oReader;
	//				else if ( _T("dsp:style") == sName )
	//					m_oShapeStyle = oReader;
	//				else if (_T("dsp:txBody") == sName)
	//					m_oTxBody = oReader;
	//				else if (_T("dsp:txXfrm") == sName)
	//					m_oTxXfrm = oReader;
	//			}
	//		}

	//		virtual EElementType getType () const
	//		{
	//			return et_dsp_Shape;
	//		}

	//	private:
	//		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	//		{
	//		}
	//	public:
	//		//nullable<OOX::Drawing::CShapeNonVisual>		m_oNvSpPr;

	//		nullable<OOX::Drawing::CShapeProperties>		m_oSpPr;
	//		nullable<OOX::Drawing::CShapeStyle>				m_oShapeStyle;
	//		nullable<OOX::Drawing::CTxBody>					m_oTxBody;
	//		nullable<OOX::Drawing::CTransform2D>			m_oTxXfrm;

	//		//extLst (Extension List) І20.1.2.2.15

	//	};

	//	class CShapeTree : public WritingElementWithChilds<OOX::Diagram::CShape>
	//	{
	//	public:
	//		WritingElement_AdditionConstructors(CShapeTree)
	//		CShapeTree()
	//		{
	//		}
	//		virtual ~CShapeTree()
	//		{
	//		}

	//	public:
	//		virtual void fromXML(XmlUtils::CXmlNode& oNode)
	//		{
	//		}
	//		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
	//		{
	//			ReadAttributes( oReader );

	//			if ( oReader.IsEmptyNode() )
	//				return;

	//			int nParentDepth = oReader.GetDepth();
	//			while( oReader.ReadNextSiblingNode( nParentDepth ) )
	//			{
	//				std::wstring sName = oReader.GetName();
	//				WritingElement *pItem = NULL;

	//				if ( _T("dsp:sp") == sName )
	//					m_arrItems.push_back( new Diagram::CShape( oReader ));
	//				//else if ( _T("dsp:nvGrpSpPr") == sName )
	//				//	m_oGroupShapeNonVisual = oReader;
	//				else if ( _T("dsp:grpSpPr") == sName )
	//					m_oGroupShapeProperties = oReader;
	//			}
	//		}
	//		virtual std::wstring toXML() const
	//		{
	//			std::wstring sResult = _T("");
	//			return sResult;
	//		}

	//		virtual EElementType getType() const
	//		{
	//			return et_dsp_ShapeTree;
	//		}
	//	private:
	//		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	//		{
	//			// Читаем атрибуты
	//			WritingElement_ReadAttributes_Start	( oReader )
	//			WritingElement_ReadAttributes_End	( oReader )
	//		}

	//	public:

	//		// Attributes

	//		// Childs
	//		//nullable<Diagram::CGroupShapeNonVisual>		m_oGroupShapeNonVisual;
	//		nullable<Drawing::CGroupShapeProperties>	m_oGroupShapeProperties;
	//	};
	//}
    class CDiagramDrawing : public OOX::File, public OOX::IFileContainer
    {
    public:
        CDiagramDrawing(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
        {
        }
		CDiagramDrawing(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain), OOX::IFileContainer(pMain)
		{
			read(uri.GetDirectory(), uri);
		}
		CDiagramDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
        {
            read( oRootPath, oPath );
        }
        virtual ~CDiagramDrawing()
        {
        }
        virtual void read(const CPath& oFilePath)
        {
            //don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
            CPath oRootPath;
            read(oRootPath, oFilePath);
        }
        virtual void read(const CPath& oRootPath, const CPath& oFilePath)
        {
			m_oReadPath = oFilePath;
            IFileContainer::Read( oRootPath, oFilePath );

			XmlUtils::CXmlNode oNodeDW;
            if (oNodeDW.FromXmlFile(oFilePath.GetPath() ))
			{
				m_oShapeTree = oNodeDW.ReadNodeNoNS(L"spTree");
			}
        }

        virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
        {
        }

        virtual const OOX::FileType type() const
        {
            return FileTypes::DiagDrawing;
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

		CPath									m_oReadPath;
		NSCommon::nullable<PPTX::Logic::SpTree> m_oShapeTree;

    };
} // namespace OOX

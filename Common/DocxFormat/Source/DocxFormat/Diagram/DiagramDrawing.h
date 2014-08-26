#pragma once

#include "../FileTypes.h"
#include "../File.h"
#include "../../Base/Nullable.h"

#include "../WritingElement.h"

#include "../Drawing/DrawingCoreInfo.h"
#include "../Drawing/DrawingText.h"


namespace OOX
{
namespace Diagram
{
		//--------------------------------------------------------------------------------
		//		?????? sp (Shape)
		//--------------------------------------------------------------------------------
		class CShape : public WritingElement
		{
		public:
			CShape(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CShape(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CShape()
			{
			}
			virtual ~CShape()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("dsp:spPr") == sName )
						m_oSpPr = oReader;
					//else if ( _T("dsp:nvSpPr") == sName )
					//	m_oNvSpPr = oReader;
					else if ( _T("dsp:style") == sName )
						m_oShapeStyle = oReader;	
					else if (_T("dsp:txBody") == sName)
						m_oTxBody = oReader; 		
				}
			}

			virtual EElementType getType () const
			{
				return et_dsp_Shape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			//nullable<OOX::Drawing::CShapeNonVisual>			m_oNvSpPr;

			nullable<OOX::Drawing::CShapeProperties>		m_oSpPr;
			nullable<OOX::Drawing::CShapeStyle>				m_oShapeStyle;
			nullable<OOX::Drawing::CTxBody>					m_oTxBody;	

//extLst (Extension List) §20.1.2.2.15

		};

	class CShapeTree : public WritingElementWithChilds<OOX::Diagram::CShape>
	{
	public:
		WritingElement_AdditionConstructors(CShapeTree)
		CShapeTree()
		{
		}
		virtual ~CShapeTree()
		{
		}

	public:
		virtual void		fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void		fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CString sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("dsp:sp") == sName )
					m_arrItems.push_back( new Diagram::CShape( oReader ));
				//else if ( _T("dsp:nvGrpSpPr") == sName )
				//	m_oGroupShapeNonVisual = oReader;
				else if ( _T("dsp:grpSpPr") == sName )
					m_oGroupShapeProperties = oReader;
			}
		}
		virtual CString      toXML() const
		{
			CString sResult = _T("");
			return sResult;
		}

		virtual EElementType getType() const
		{
			return et_dsp_ShapeTree;
		}
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_End( oReader )
		}

	public:

		// Attributes

		// Childs
		//nullable<Diagram::CGroupShapeNonVisual>		m_oGroupShapeNonVisual;
		nullable<Drawing::CGroupShapeProperties>	m_oGroupShapeProperties;
	};
}
	class CDiagramDrawing : public OOX::File
	{
	public:
		CDiagramDrawing()
		{
		}
		CDiagramDrawing(const CPath& oPath)
		{
			read( oPath );
		}
		virtual ~CDiagramDrawing()
		{
			for(int i = 0, length = m_arrShapeTree.GetSize(); i < length; ++i)
				delete m_arrShapeTree[i];
			m_arrShapeTree.RemoveAll();
		}
	public:

		virtual void read(const CPath& oFilePath)
		{
#ifdef USE_LITE_READER

			XmlUtils::CXmlLiteReader oReader;
			
			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			CWCharWrapper sName = oReader.GetName();
			if ( _T("dsp:drawing") == sName && !oReader.IsEmptyNode() )
			{
				int nNumberingDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
				{
					sName = oReader.GetName();
					if ( _T("dsp:spTree") == sName )
						m_arrShapeTree.Add( new Diagram::CShapeTree(oReader) );
				}
			}
#endif
		}

		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
		}

	public:

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

	public:

		CSimpleArray<Diagram::CShapeTree*> m_arrShapeTree;

	};
} // namespace OOX

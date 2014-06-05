#pragma once
#ifndef OOX_LOGIC_DRAWING_TRANSFORM_INCLUDE_H_
#define OOX_LOGIC_DRAWING_TRANSFORM_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/ComplexTypes.h"

#include "../WritingElement.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CGroupTransform2D 20.1.7.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CGroupTransform2D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGroupTransform2D)
			CGroupTransform2D()
			{
				m_eType = et_Unknown;
			}
			virtual ~CGroupTransform2D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CGroupTransform2D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
				CWCharWrapper sName = oReader.GetName();
				if ( _T("a:xfrm") == sName )
					m_eType = et_a_xfrm;
				else
					return;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = oReader.GetName();

					if ( _T("a:chExt") == sName )
						m_oChExt = oReader;
					else if ( _T("a:chOff") == sName )
						m_oChOff = oReader;
					else if ( _T("a:ext") == sName )
						m_oExt = oReader;
					else if ( _T("a:off") == sName )
						m_oOff = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				
				switch ( m_eType )
				{
				case et_a_xfrm: 
					
					sResult = _T("<a:xfrm flipH=\"") + m_oFlipH.ToString()
						+ _T("\" flipV=\"") + m_oFlipV.ToString()
						+ _T("\" rot=\"") + m_oRot.ToString()
						+ _T("\">");

					break;

				default:

					return _T("");
				}

				if ( m_oOff.IsInit() )
					sResult += _T("<a:off ") + m_oOff->ToString() + _T("/>");

				if ( m_oExt.IsInit() )
					sResult += _T("<a:ext ") + m_oExt->ToString() + _T("/>");

				if ( m_oChOff.IsInit() )
					sResult += _T("<a:chOff ") + m_oChOff->ToString() + _T("/>");

				if ( m_oChExt.IsInit() )
					sResult += _T("<a:chExt ") + m_oChExt->ToString() + _T("/>");

				switch ( m_eType )
				{
				case et_a_xfrm: sResult = _T("</a:xfrm>"); break;
				}

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("flipH"), m_oFlipH )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("flipV"), m_oFlipV )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rot"),   m_oRot )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType                                     m_eType;

			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse>     m_oFlipH;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse>     m_oFlipV;
			SimpleTypes::CAngle<0>                           m_oRot;

			// Childs
			nullable<ComplexTypes::Drawing::CPositiveSize2D> m_oChExt;
			nullable<ComplexTypes::Drawing::CPoint2D>        m_oChOff;
			nullable<ComplexTypes::Drawing::CPositiveSize2D> m_oExt;
			nullable<ComplexTypes::Drawing::CPoint2D>		 m_oOff;

		};
		//--------------------------------------------------------------------------------
		// CTransform2D 20.1.7.6 (Part 1)
		//--------------------------------------------------------------------------------	
		class CTransform2D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTransform2D)
			CTransform2D()
			{
				m_eType = et_Unknown;
			}
			virtual ~CTransform2D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CTransform2D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
				CWCharWrapper sName = oReader.GetName();
				if ( _T("a:xfrm") == sName )
					m_eType = et_a_xfrm;
				else
					return;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = oReader.GetName();

					if ( _T("a:ext") == sName )
						m_oExt = oReader;
					else if ( _T("a:off") == sName )
						m_oOff = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				
				switch ( m_eType )
				{
				case et_a_xfrm: 
					
					sResult = _T("<a:xfrm flipH=\"") + m_oFlipH.ToString()
						+ _T("\" flipV=\"") + m_oFlipV.ToString()
						+ _T("\" rot=\"") + m_oRot.ToString()
						+ _T("\">");

					break;

				default:

					return _T("");
				}

				if ( m_oOff.IsInit() )
					sResult += _T("<a:off ") + m_oOff->ToString() + _T("/>");

				if ( m_oExt.IsInit() )
					sResult += _T("<a:ext ") + m_oExt->ToString() + _T("/>");

				switch ( m_eType )
				{
				case et_a_xfrm: sResult = _T("</a:xfrm>"); break;
				}

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("flipH"), m_oFlipH )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("flipV"), m_oFlipV )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rot"),   m_oRot )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType                                     m_eType;

			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse>     m_oFlipH;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse>     m_oFlipV;
			SimpleTypes::CAngle<0>                           m_oRot;

			// Childs
			nullable<ComplexTypes::Drawing::CPositiveSize2D> m_oExt;
			nullable<ComplexTypes::Drawing::CPoint2D>		 m_oOff;

		};



	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_TRANSFORM_INCLUDE_H_
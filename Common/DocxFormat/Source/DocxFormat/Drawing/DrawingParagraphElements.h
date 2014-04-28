#pragma once
#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// 21.1.2.2.11 spcPct (Spacing Percent)
		//--------------------------------------------------------------------------------	
		class CLineSpacingPercent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineSpacingPercent)
			CLineSpacingPercent()
			{
			}
			virtual ~CLineSpacingPercent()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_spcPct;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("val"),        m_oVal )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<SimpleTypes::CTextSpacingPercentOrPercentString>		m_oVal;
		};
		//--------------------------------------------------------------------------------
		// 21.1.2.2.5 lnSpc (Line Spacing)
		//--------------------------------------------------------------------------------	
		class CLineSpacingPoints : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineSpacingPoints)
			CLineSpacingPoints()
			{
			}
			virtual ~CLineSpacingPoints()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					
				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_spcPts;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("val"),        m_oVal )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<SimpleTypes::CTextSpacingPoint<>>		m_oVal;

		};
		//--------------------------------------------------------------------------------
		// 21.1.2.2.5 lnSpc (Line Spacing), 21.1.2.2.9 spcAft (Space After), 21.1.2.2.10 spcBef (Space Before)
		//--------------------------------------------------------------------------------	
		class CLineSpacing : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineSpacing)
			CLineSpacing()
			{
			}
			virtual ~CLineSpacing()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{

				// TO DO
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					
					if ( _T("a:spcPct") == sName )
						m_oLineSpacingPercent = oReader;
					if ( _T("a:spcPts") == sName )
						m_oLineSpacingPoints = oReader;
				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_lnSpc;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			// Childs
			nullable<CLineSpacingPercent>		m_oLineSpacingPercent;
			nullable<CLineSpacingPoints>		m_oLineSpacingPoints;
		};
	} // namespace Drawing
} // namespace OOX

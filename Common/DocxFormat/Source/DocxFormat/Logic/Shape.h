#pragma once
#include "../WritingElement.h"

//WordprocessingML Drawing
#include "../Drawing/DrawingCoreInfo.h"


namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CTxbxContent 9.8.1.1 (Part 4)
		// 20.4.2.38 txbxContent (Rich Text Box Content Container)
		//--------------------------------------------------------------------------------	
		class CTxbxContent : public WritingElementWithChilds<>
		{
		public:
			CTxbxContent()
			{
			}
			CTxbxContent(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CTxbxContent(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CTxbxContent()
			{

			}

		public:

			const CTxbxContent &operator =(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}

			const CTxbxContent &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}


		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_txbxContent;
			}

		public:

			// Childs
		};

		//--------------------------------------------------------------------------------
		// 20.4.2.37 txbx (Textual contents of shape)
		//--------------------------------------------------------------------------------	
		class CTextBody : public WritingElement
		{
		public:
			CTextBody(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CTextBody(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CTextBody()
			{
			}
			virtual ~CTextBody()
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
                    CString sName = oReader.GetName();

					if ( _T("w:txbxContent") == sName )
						m_oTxtbxContent = oReader;				
				}
			}

			virtual EElementType getType () const
			{
				return et_w_ShapeTextBody;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<OOX::Logic::CTxbxContent>                    m_oTxtbxContent;
		};
		//--------------------------------------------------------------------------------
		// 20.4.2.28 cNvSpPr (Non-Visual Drawing Properties for a Shape)
		// одно и тоже в 2-х namespace - wps & xdr (todooo - объекдинить)
		//--------------------------------------------------------------------------------	
		class CConnectionNonVisualShapeProps : public WritingElement
		{
		public:
			CConnectionNonVisualShapeProps(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CConnectionNonVisualShapeProps(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CConnectionNonVisualShapeProps()
			{
			}
			virtual ~CConnectionNonVisualShapeProps()
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
                    CString sName = oReader.GetName();

					sName = oReader.GetName();
					if ( _T("a:spLocks") == sName )
						m_oSpLocks = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_w_ConnectionNonVisualShapeProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("txBox"), m_otxBox )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_otxBox;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CPictureLocking>			m_oSpLocks;//todo CShapeLocking
		};
		//--------------------------------------------------------------------------------
		// 20.4.2.26 cNvGrpSpPr (Non-Visual Group Shape Drawing Properties)
		// одно и тоже в 2-х namespace - wps & xdr (todooo - объекдинить)
		//--------------------------------------------------------------------------------	
		class CNonVisualGroupShapeDrawingProps : public WritingElement
		{
		public:
			CNonVisualGroupShapeDrawingProps(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CNonVisualGroupShapeDrawingProps(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CNonVisualGroupShapeDrawingProps()
			{
			}
			virtual ~CNonVisualGroupShapeDrawingProps()
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
                    CString sName = oReader.GetName();

					sName = oReader.GetName();
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					//else ( _T("a:grpSpLocks") == sName )
					//	m_oSpLocks = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_w_NonVisualGroupShapeDrawingProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_otxBox;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			//nullable<OOX::Drawing::CPictureLocking>			m_oSpLocks;//todo grpSpLocks
		};
		
		//--------------------------------------------------------------------------------
		// 20.4.2.23 cNvCnPr (Non-Visual Connector Shape Drawing Properties)
		// одно и тоже в 2-х namespace - wps & xdr (todooo - объекдинить)
		//--------------------------------------------------------------------------------	
		class CConnectionNonVisualConnShapeProps : public WritingElement
		{
		public:
			CConnectionNonVisualConnShapeProps(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CConnectionNonVisualConnShapeProps(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CConnectionNonVisualConnShapeProps()
			{
			}
			virtual ~CConnectionNonVisualConnShapeProps()
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
                    CString sName = oReader.GetName();

					sName = oReader.GetName();
					if ( _T("a:picLocks") == sName )
						m_oPicLocks = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_w_ConnectionNonVisualConnShapeProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("preferRelativeResize"), m_oPreferRelativeResize )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffTrue> m_oPreferRelativeResize;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CPictureLocking>         m_oPicLocks;
		};
		//--------------------------------------------------------------------------------
		//		20.4.2.42 wsp (WordprocessingML Shape)
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
                    CString sName = oReader.GetName();

					if ( _T("wps:cNvSpPr") == sName )
						m_oCNvSpPr = oReader;
					else if ( _T("wps:cNvPr") == sName )
						m_oCNvPr = oReader;
					else if ( _T("wps:cNvCnPr") == sName )
						m_oCNvConnSpPr = oReader;
					else if ( _T("wps:spPr") == sName )
						m_oSpPr = oReader;
					else if ( _T("wps:style") == sName )
						m_oShapeStyle = oReader;	
					else if (_T("wps:txbx") == sName)
						m_oTxBody = oReader; 		
					else if (_T("wps:bodyPr") == sName)
						m_oTxBodyProperties = oReader; 
				}
			}

			virtual EElementType getType () const
			{
				return et_w_Shape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;
			nullable<CConnectionNonVisualShapeProps>		m_oCNvSpPr;
			nullable<CConnectionNonVisualConnShapeProps>	m_oCNvConnSpPr;

			nullable<OOX::Drawing::CShapeProperties>		m_oSpPr;
			nullable<OOX::Drawing::CShapeStyle>				m_oShapeStyle;
			nullable<OOX::Logic::CTextBody>					m_oTxBody;	
			nullable<OOX::Drawing::CTextBodyProperties>		m_oTxBodyProperties;	
		};

		//--------------------------------------------------------------------------------
		//		20.4.2.39 wgp (WordprocessingML Shape Group)
		//--------------------------------------------------------------------------------
		class CGroupShape : public WritingElementWithChilds<WritingElement>
		{
		public:
			CGroupShape(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CGroupShape(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CGroupShape()
			{
			}
			virtual ~CGroupShape()
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
                    CString sName = oReader.GetName();

					if ( _T("wpg:grpSpPr") == sName )
						m_oGroupSpPr = oReader;
					else if ( _T("wpg:cNvPr") == sName )
						m_oCNvPr = oReader;
					else if ( _T("wpg:cNvGrpSpPr") == sName )
						m_oCNvGroupSpPr = oReader;

					else if ( _T("wps:wsp") == sName )
						m_arrItems.push_back( new OOX::Logic::CShape( oReader ));
					else if ( _T("c:chart") == sName ) //??? 
						m_arrItems.push_back( new OOX::Drawing::CChart( oReader ));
					else if ( _T("pic:pic") == sName )
						m_arrItems.push_back( new OOX::Drawing::CPicture( oReader ));
					else if ( _T("wpg:grpSp") == sName )
						m_arrItems.push_back( new OOX::Logic::CGroupShape( oReader ));	
				}
			}

			virtual EElementType getType () const
			{
				return et_w_GroupShape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<OOX::Drawing::CGroupShapeProperties>	m_oGroupSpPr;
			nullable<CNonVisualGroupShapeDrawingProps>		m_oCNvGroupSpPr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;

		};
		//--------------------------------------------------------------------------------
		//		20.4.2.41 wpc (WordprocessingML Drawing Canvas)
		//--------------------------------------------------------------------------------
		class CLockedCanvas : public WritingElementWithChilds<WritingElement>
		{
		public:
			CLockedCanvas(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CLockedCanvas(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CLockedCanvas()
			{
			}
			virtual ~CLockedCanvas()
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
                    CString sName = oReader.GetName();

					//if ( _T("wpg:grpSpPr") == sName )
					//	m_oGroupSpPr = oReader;
					//else if ( _T("wpg:cNvPr") == sName )
					//	m_oCNvPr = oReader;
					//else if ( _T("wpg:cNvGrpSpPr") == sName )
					//	m_oCNvGroupSpPr = oReader;

					if ( _T("wps:wsp") == sName )
						m_arrItems.push_back( new OOX::Logic::CShape( oReader ));
					else if ( _T("c:chart") == sName ) //??? 
						m_arrItems.push_back( new OOX::Drawing::CChart( oReader ));
					else if ( _T("pic:pic") == sName )
						m_arrItems.push_back( new OOX::Drawing::CPicture( oReader ));
					else if ( _T("wpg:grpSp") == sName )
						m_arrItems.push_back( new OOX::Logic::CGroupShape( oReader ));	
				}
			}

			virtual EElementType getType () const
			{
				return et_w_LockedCanvas;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			//nullable<OOX::Drawing::CGroupShapeProperties>	m_oGroupSpPr;
			//nullable<CNonVisualGroupShapeDrawingProps>		m_oCNvGroupSpPr;
			//nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;

		};
	} //Logic
} // namespace OOX

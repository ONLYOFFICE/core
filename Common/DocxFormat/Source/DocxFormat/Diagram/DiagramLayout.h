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

#include "DiagramData.h"

namespace OOX
{
	namespace Diagram
	{
		class CDiferentData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDiferentData)

			CDiferentData();
			virtual ~CDiferentData();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_bool m_bUseDef;
			nullable<Diagram::CDataModel> m_oDataModel;

			std::wstring node_name;
		};

		class CLayoutNode : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CLayoutNode)

			CLayoutNode();
			virtual ~CLayoutNode();

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sName;
			nullable_string m_sStyleLbl;
			nullable_string m_sMoveWith;
			nullable<SimpleTypes::CChOrder> m_oChOrder;
		};

		class CForEach : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CForEach)

			CForEach();
			virtual ~CForEach();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::vector<SimpleTypes::CAxisTypes> m_arAxis;
			std::vector<SimpleTypes::CElementTypes> m_arPtType;
			std::vector<int> m_arCnt;
			std::vector<int> m_arSt;
			std::vector<int> m_arStep;
			std::vector<bool> m_arHideLastTrans;
			nullable_string m_sName;
			nullable_string m_sRef;
		};

		class CRule : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRule)

			CRule();
			virtual ~CRule();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDouble> m_oFact;
			nullable<SimpleTypes::CConstraintRelationship> m_oFor;
			nullable_string m_oForName;
			nullable<SimpleTypes::CDouble> m_oMax;
			nullable<SimpleTypes::CElementTypes> m_oPtType;
			nullable<SimpleTypes::CConstraintType> m_oType;
			nullable<SimpleTypes::CDouble> m_oVal;
			
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};

		class CRuleLst : public WritingElementWithChilds<CRule>
		{
		public:
			WritingElement_AdditionConstructors(CRuleLst)

			CRuleLst();
			virtual ~CRuleLst();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};

		class CConstraint : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConstraint)

			CConstraint();
			virtual ~CConstraint();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CConstraintRelationship> m_oFor;
			nullable_string m_oForName;
			nullable_double m_oFact;
			nullable<SimpleTypes::CBoolOperator> m_oOp;
			nullable<SimpleTypes::CElementTypes> m_oPtType;
			nullable<SimpleTypes::CConstraintRelationship> m_oRefFor;
			nullable_string m_oRefForName;
			nullable<SimpleTypes::CElementTypes> m_oRefPtType;
			nullable<SimpleTypes::CConstraintType> m_oRefType;
			nullable<SimpleTypes::CConstraintType> m_oType;
			nullable<SimpleTypes::CDouble> m_oVal;

			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};

		class CConstrLst : public WritingElementWithChilds<CConstraint>
		{
		public:
			WritingElement_AdditionConstructors(CConstrLst)

			CConstrLst();
			virtual ~CConstrLst();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};

		class CParam : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CParam)

			CParam();
			virtual ~CParam();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CParameterId> m_oType;
			nullable_string m_oVal; //union of SimpleTypes ... todooo?
		};

		class CAlg : public WritingElementWithChilds<CParam>
		{
		public:
			WritingElement_AdditionConstructors(CAlg)

			CAlg();
			virtual ~CAlg();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CAlgorithmType> m_oType;
			nullable_uint m_nRev;

			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};

		class CIf : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CIf)

			CIf();
			virtual ~CIf();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sArg; //union of types
			nullable<SimpleTypes::CFunctionType> m_oFunc;
			nullable<SimpleTypes::CFunctionOperator> m_oOp;
			std::vector<SimpleTypes::CAxisTypes> m_arAxis;
			std::vector<SimpleTypes::CElementTypes> m_arPtType;
			std::vector<int> m_arCnt;
			std::vector<int> m_arSt;
			std::vector<int> m_arStep;		
			std::vector<bool> m_arHideLastTrans;
			nullable_string m_sName;
			nullable_string m_sRef;

			nullable_string m_sVal;
		};

		class CElse : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CElse)

			CElse();
			virtual ~CElse();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sName;
		};

		class CChoose : public WritingElementWithChilds<CIf>
		{
		public:
			WritingElement_AdditionConstructors(CChoose)

			CChoose();
			virtual ~CChoose();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sName;
			nullable<Diagram::CElse> m_oElse;
		};

		class CPresOf : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPresOf)

			CPresOf();
			virtual ~CPresOf();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::vector<SimpleTypes::CAxisTypes> m_arAxis;
			std::vector<SimpleTypes::CElementTypes> m_arPtType;
			std::vector<int> m_arCnt;
			std::vector<int> m_arSt;
			std::vector<int> m_arStep;
			std::vector<bool> m_arHideLastTrans;
			
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};

		class CShapeAdjust : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShapeAdjust)

			CShapeAdjust();
			virtual ~CShapeAdjust();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint m_oIdx;
			nullable_double m_oVal;
		};

		class CAdjLst : public WritingElementWithChilds<CShapeAdjust>
		{
		public:
			WritingElement_AdditionConstructors(CAdjLst)

			CAdjLst();
			virtual ~CAdjLst();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};

		class CShape : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShape)

			CShape();
			virtual ~CShape();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sBlip;
			nullable_bool m_bBlipPhldr;
			nullable_bool m_bHideGeom;
			nullable_bool m_bLkTxEntry;
			nullable_double m_dRot;
			nullable_int m_nZOrderOff;
			nullable_string m_oType; //prst_geom + none + conn

			nullable<CAdjLst> m_oAdjLst;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
	}

	class CDiagramLayout : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CDiagramLayout(OOX::Document* pMain);
		CDiagramLayout(OOX::Document* pMain, const CPath& uri);
		CDiagramLayout(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
		virtual ~CDiagramLayout();

		virtual void read(const CPath& oFilePath);
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;
		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

//------------------------------------

		nullable_string m_sUniqueId;
		nullable_string m_sMinVer;
		nullable_string m_sDefStyle;

		nullable<Diagram::CCatLst> m_oCatLst;
		nullable<Diagram::CDiferentData> m_oClrData;
		nullable<Diagram::CText> m_oDesc;
		nullable<Diagram::CText> m_oTitle;
		nullable<Diagram::CLayoutNode> m_oLayoutNode;
		nullable<Diagram::CDiferentData> m_oSampData;
		nullable<Diagram::CDiferentData> m_oStyleData;
		
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
	};

} // namespace OOX

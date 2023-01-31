/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#include "../Drawing/DrawingExt.h"

#include "../../../OOXML/PPTXFormat/Logic/SpTree.h"
#include "../../../OOXML/PPTXFormat/Logic/TxBody.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

namespace OOX
{
	namespace Diagram
	{
		template<typename E>
		class ComplexType : public WritingElement
		{
		public:
			ComplexType() {}
			virtual ~ComplexType() {}
			virtual std::wstring toXML() const = 0;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader) = 0;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const = 0;
			
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const = 0;
			
			void toXmlWriter(const std::wstring & node_name, NSBinPptxRW::CXmlWriter* pWriter) const;
			std::wstring toXML(const std::wstring & node_name) const;

			BYTE ReadByteFromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			int ReadIntFromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			void WriteByteToPPTY(BYTE val, NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			void WriteIntToPPTY(int val, NSBinPptxRW::CBinaryFileWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<E> m_oVal;
		};

		class CAnimLvl : public Diagram::ComplexType<SimpleTypes::CAnimLvlStr>
		{
		public:
			WritingElement_AdditionConstructors(CAnimLvl)

			CAnimLvl();
			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CAnimOne : public Diagram::ComplexType<SimpleTypes::CAnimOneStr>
		{
		public:
			WritingElement_AdditionConstructors(CAnimOne)

			CAnimOne();
			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CBulletEnabled : public Diagram::ComplexType<SimpleTypes::CBool>
		{
		public:
			WritingElement_AdditionConstructors(CBulletEnabled)

			CBulletEnabled();

			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CChMax : public Diagram::ComplexType<SimpleTypes::CDecimalNumber>
		{
		public:
			WritingElement_AdditionConstructors(CChMax)

			CChMax();
			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CChPref : public Diagram::ComplexType<SimpleTypes::CDecimalNumber>
		{
		public:
			WritingElement_AdditionConstructors(CChPref)

			CChPref();

			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CDirection : public Diagram::ComplexType<SimpleTypes::CDirectionDraw>
		{
		public:
			WritingElement_AdditionConstructors(CDirection)

			CDirection();

			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CHierBranch : public Diagram::ComplexType<SimpleTypes::CHierBranch>
		{
		public:
			WritingElement_AdditionConstructors(CHierBranch)

			CHierBranch();

			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class COrgChart : public Diagram::ComplexType<SimpleTypes::CBool>
		{
		public:
			WritingElement_AdditionConstructors(COrgChart)
			
			COrgChart();

			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CResizeHandles : public Diagram::ComplexType<SimpleTypes::CResizeHandles>
		{
		public:
			WritingElement_AdditionConstructors(CResizeHandles)
			
			CResizeHandles();

			virtual EElementType getType() const;

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};

		class CVariableList : public WritingElement 
		{
		public:
			WritingElement_AdditionConstructors(CVariableList)
			
			CVariableList();
			virtual ~CVariableList();
			
			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			nullable<Diagram::CAnimLvl> m_oAnimLvl;
			nullable<Diagram::CAnimOne> m_oAnimOne;
			nullable<Diagram::CBulletEnabled> m_oBulletEnabled;
			nullable<Diagram::CChMax> m_oChMax;
			nullable<Diagram::CChPref> m_oChPref;
			nullable<Diagram::CDirection> m_oDir;
			nullable<Diagram::CHierBranch> m_oHierBranch;
			nullable<Diagram::COrgChart> m_oOrgChart;
			nullable<Diagram::CResizeHandles> m_oResizeHandles;

			std::wstring node_name;
		};

		class CPropertySet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPropertySet)

			CPropertySet();
			virtual ~CPropertySet();

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
			nullable<Diagram::CVariableList>	m_oPresLayoutVars;
			nullable<PPTX::Logic::ShapeStyle>	m_oStyle;

			nullable_bool	m_coherent3DOff;
			nullable_string	m_csCatId;
			nullable_string	m_csTypeId;
			nullable_int	m_custAng;
			nullable_bool	m_custFlipHor;
			nullable_bool	m_custFlipVert;
			nullable_int	m_custLinFactNeighborX;
			nullable_int	m_custLinFactNeighborY;
			nullable_int	m_custLinFactX;
			nullable_int	m_custLinFactY;
			nullable_int	m_custRadScaleInc;
			nullable_int	m_custRadScaleRad;
			nullable_int	m_custScaleX;
			nullable_int	m_custScaleY;
			nullable_int	m_custSzX;
			nullable_int	m_custSzY;
			nullable_bool	m_custT;
			nullable_string	m_loCatId;
			nullable_string	m_loTypeId;
			nullable_bool	m_phldr;
			nullable_string	m_phldrT;
			nullable_string	m_presAssocID; //guid modelId
			nullable_string	m_presName;
			nullable_int	m_presStyleCnt;
			nullable_int	m_presStyleIdx;
			nullable_string	m_presStyleLbl;
			nullable_string	m_qsCatId;
			nullable_string	m_qsTypeId;
		};

		class CPt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPt)

			CPt();
			virtual ~CPt();

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
			nullable<PPTX::Logic::SpPr>		m_oSpPr;
			nullable<PPTX::Logic::TxBody>	m_oTxBody;
			nullable<Diagram::CPropertySet> m_oPrSet;
			
			nullable_string m_sModelId;	//guid
			nullable<SimpleTypes::CPtTypes> m_oType;
			nullable_string m_sCxnId;	//guid
		};

		class CPtLst : public WritingElementWithChilds<OOX::Diagram::CPt>
		{
		public:
			WritingElement_AdditionConstructors(CPtLst)

			CPtLst();
			virtual ~CPtLst();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
			virtual std::wstring toXML() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};

		class CCxn : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCxn)

			CCxn();
			virtual ~CCxn();

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
			nullable_string m_sModelId;	//guid or int
			nullable_string m_sType; // parOf, presOf, presParOf, unknownRelationship

			nullable_string m_sDestId; // ...
			nullable_string m_sDestOrd;
			nullable_string m_sSrcId;
			nullable_string m_sSrcOrd;
			nullable_string m_sParTransId;
			nullable_string m_sSibTransId;
			nullable_string m_sPresId;

		};

		class CCxnLst : public WritingElementWithChilds<OOX::Diagram::CCxn>
		{
		public:
			WritingElement_AdditionConstructors(CCxnLst)

			CCxnLst();
			virtual ~CCxnLst();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};

		class CWhole : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWhole)

			CWhole();
			virtual ~CWhole();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			nullable<PPTX::Logic::Ln>		m_oLn;
			PPTX::Logic::EffectProperties	m_oEffectList;
		};

		class CBg: public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBg)

			CBg();
			virtual ~CBg();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			PPTX::Logic::EffectProperties	m_oEffectList;
			PPTX::Logic::UniFill			m_oFill;
		};

		class CDataModel : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataModel)

			CDataModel();
			virtual ~CDataModel();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			nullable<Diagram::CPtLst> m_oPtLst;
			nullable<Diagram::CCxnLst> m_oCxnLst;
			nullable<Diagram::CBg> m_oBg;
			nullable<Diagram::CWhole> m_oWhole;

			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

			bool bMain = true;
		};

		class CCat : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCat)

			CCat();
			virtual ~CCat();

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
			nullable_string m_sType;	//uri
			nullable_uint m_nPri;
		};

		class CCatLst : public WritingElementWithChilds<OOX::Diagram::CCat>
		{
		public:
			WritingElement_AdditionConstructors(CCatLst)

			CCatLst();
			virtual ~CCatLst();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
			virtual std::wstring toXML() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};

		class CText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CText)

			CText() {}
			virtual ~CText() {}
			virtual std::wstring toXML() const
			{
				NSBinPptxRW::CXmlWriter oWriter;
				toXmlWriter(&oWriter);

				return oWriter.GetXmlString();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType() const
			{
				return et_dgm_text;
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::wstring node_name;

			nullable_string m_sLang;
			nullable_string m_sVal;
		};
	}

//-------------------------------------------------------------------------------------------------------------------------------

    class CDiagramData : public OOX::IFileContainer, public OOX::FileGlobalEnumerated
    {
    public:
		CDiagramData(OOX::Document* pMain);
		CDiagramData(OOX::Document* pMain, const CPath& uri);
		CDiagramData(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
		virtual ~CDiagramData();

		virtual void read(const CPath& oFilePath);
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

        std::wstring m_strFilename;

		nullable<Diagram::CDataModel> m_oDataModel;
	
		nullable<OOX::RId> id_drawing;
	};

} // namespace OOX

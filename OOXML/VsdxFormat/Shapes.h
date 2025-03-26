/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "../Base/Nullable.h"

#include "FileTypes_Draw.h"
#include "../DocxFormat/IFileContainer.h"
#include "../Common/SimpleTypes_Draw.h"
#include "../Common/SimpleTypes_Shared.h"

namespace OOX
{
	class CRelationShip;

	namespace Draw
	{
		class CRel : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRel)
			CRel() {}
			virtual ~CRel() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CRelationshipId> Rid;
		};
		class CIcon : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CIcon)
			CIcon() {}
			virtual ~CIcon() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring content;
		};
		class CForeignData : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CForeignData)
			CForeignData() {}
			virtual ~CForeignData() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Draw::CForeignType> ForeignType;
			nullable<SimpleTypes::Draw::CCompressionType> CompressionType;
			nullable_double CompressionLevel;
			nullable_uint ObjectType;
			nullable_bool ShowAsIcon;
			nullable_double ObjectWidth;
			nullable_double ObjectHeight;
			nullable_double ExtentX;
			nullable_double ExtentY;

			nullable<CRel> Rel;
		};
		class CText_cp: public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CText_cp)
			CText_cp() {}
			virtual ~CText_cp() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint IX;
		};
		class CText_pp : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CText_pp)
			CText_pp() {}
			virtual ~CText_pp() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint IX;
		};
		class CText_tp : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CText_tp)
			CText_tp() {}
			virtual ~CText_tp() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint IX;
		};
		class CText_fld : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CText_fld)
			CText_fld() {}
			virtual ~CText_fld() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint IX;
			std::wstring content;
		};
		class CText_text : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CText_text)
			CText_text() {}
			virtual ~CText_text() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) {}

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring content;
		};
		class CText : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CText)
			CText() {}
			virtual ~CText() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;
		};
		class CRow : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CRow)
			CRow() {}
			virtual ~CRow() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_string N;
			nullable_string LocalName;
			nullable_uint IX;
			nullable_string T; // todooo GeometryRowTypes
			nullable_bool Del;
		};
		class CSection : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CSection)
			CSection() {}
			virtual ~CSection() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_string N;
			nullable_bool Del;
			nullable_uint IX;
		};
		class CRefBy : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRefBy)

			CRefBy() {}
			virtual ~CRefBy() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint ID;
			nullable_string T;
		};
		class CCell : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CCell)

			CCell() {}
			virtual ~CCell() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string N;
			nullable_string U; //todooo type 
			nullable_string E; //todooo err 
			nullable_string F;
			nullable_string V;

			nullable<CRefBy> RefBy;
		};
		class CTrigger : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTrigger)

			CTrigger() {}
			virtual ~CTrigger() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string N;

			nullable<CRefBy> RefBy;
		};
		class CShape : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CShape)
			CShape() {}
			virtual ~CShape() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable_uint OriginalID;
			nullable_bool Del;
			nullable_uint MasterShape;
			nullable_string UniqueID;
			nullable_string Name;
			nullable_string NameU;
			nullable_bool IsCustomName;
			nullable_bool IsCustomNameU;
			nullable_uint Master;
			nullable<SimpleTypes::Draw::CShapeType> Type;
			nullable_uint LineStyle;
			nullable_uint FillStyle;
			nullable_uint TextStyle;
		};
		class CShapes : public WritingElementWithChilds<CShape>
		{
		public:
			WritingElement_AdditionMethods(CShapes)
			CShapes() {}
			virtual ~CShapes() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader) {}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;
		};
		class CConnect : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CConnect)
			CConnect() {}
			virtual ~CConnect() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint FromSheet;
			nullable_string FromCell;
			nullable_int FromPart;
			nullable_uint ToSheet;
			nullable_string ToCell;
			nullable_int ToPart;
		};
		class CConnects : public WritingElementWithChilds<CConnect>
		{
		public:
			WritingElement_AdditionMethods(CConnects)
			CConnects() {}
			virtual ~CConnects() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
	} //Draw
} // namespace OOX

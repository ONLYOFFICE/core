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


#include "../Worksheets/Worksheet.h"
#include "../SharedStrings/Si.h"

#include "../Styles/Styles.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CThreadedComment;
		class CCommentItem
		{
		public:
			nullable<unsigned int> m_nLeft;
			nullable<unsigned int> m_nTop;
			nullable<unsigned int> m_nRight;
			nullable<unsigned int> m_nBottom;
			nullable<unsigned int> m_nLeftOffset;
			nullable<unsigned int> m_nTopOffset;
			nullable<unsigned int> m_nRightOffset;
			nullable<unsigned int> m_nBottomOffset;
			nullable_double	m_dLeftMM;
			nullable_double m_dTopMM;
			nullable_double m_dWidthMM;
			nullable_double m_dHeightMM;
			nullable_string	m_sAuthor;
			nullable<unsigned int> m_nRow;
			nullable<unsigned int> m_nCol;
			nullable_bool	m_bMove;
			nullable_bool	m_bSize;
			nullable<CSi>	m_oText;
			CThreadedComment*	m_pThreadedComment;
			bool m_bThreadedCommentCopy;
			nullable_string m_sGfxdata;
			nullable_bool	m_bVisible;
			nullable_string m_sFillColorRgb;

			CCommentItem();
			bool IsValid();
		};

		class CAuthors : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAuthors)
            WritingElement_XlsbConstructors(CAuthors)
			CAuthors();
			virtual ~CAuthors();

			virtual void ClearItems();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::vector<std::wstring>  m_arrItems;
		};

		class CComment : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CComment)
            WritingElement_XlsbConstructors(CComment)
			CComment();
			virtual ~CComment();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oAuthorId;
			nullable<SimpleTypes::CGuid > m_oUid;

            nullable<CSi> m_oText;
		};

		class CCommentList : public WritingElementWithChilds<CComment>
		{
		public:
			WritingElement_AdditionConstructors(CCommentList)
            WritingElement_XlsbConstructors(CCommentList)
			CCommentList();
			virtual ~CCommentList();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

        private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        };

		class CComments : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CComments(OOX::Document* pMain);
			CComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CComments();

			void readBin(const CPath& oPath);
			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

		private:
			CPath m_oReadPath;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<CAuthors >		m_oAuthors;
			nullable<CCommentList > m_oCommentList;
		};

		class CLegacyDrawingWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLegacyDrawingWorksheet)
            WritingElement_XlsbConstructors(CLegacyDrawingWorksheet)
			CLegacyDrawingWorksheet();
			virtual ~CLegacyDrawingWorksheet();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};
	} //Spreadsheet
} // namespace OOX

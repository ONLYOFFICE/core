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

#include <unordered_map>

#include "../Xlsx.h"
#include "../Workbook/Workbook.h"
#include "../SharedStrings/Si.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPerson : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPerson)
			CPerson();
			virtual ~CPerson();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:		
			nullable_string displayName;
			nullable_string userId;
			nullable_string providerId;
			nullable_string id;

			nullable<OOX::Drawing::COfficeArtExtensionList> extLst;
		};

		class CPersonList : public OOX::File, public WritingElementWithChilds<CPerson>
		{
		public:
			CPersonList(OOX::Document* pMain);
			CPersonList(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CPersonList();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType () const;
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

		public:
			const nullable<std::unordered_map<std::wstring, CPerson*>> GetPersonList();

		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CThreadedCommentMention : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CThreadedCommentMention)
			CThreadedCommentMention();
			virtual ~CThreadedCommentMention();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CGuid>						mentionpersonId;
			nullable<SimpleTypes::CGuid>						mentionId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		startIndex;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		length;
		};

		class CThreadedCommentMentions : public WritingElementWithChilds<CThreadedCommentMention>
		{
		public:
			WritingElement_AdditionConstructors(CThreadedCommentMentions)
			CThreadedCommentMentions();
			virtual ~CThreadedCommentMentions();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CThreadedComment : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CThreadedComment)
			CThreadedComment();
			virtual ~CThreadedComment();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			bool operator < (const CThreadedComment& elem) const;

			void PrepareText ();

			static bool Compare (CThreadedComment* val1, CThreadedComment* val2);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string						ref;
			nullable<SimpleTypes::CDateTime>	dT;
			nullable<SimpleTypes::CGuid>		personId;
			nullable<SimpleTypes::CGuid>		id;
			nullable<SimpleTypes::CGuid>		parentId;
			nullable_bool						done;

			nullable<CText>						m_oText;
			nullable<CThreadedCommentMentions>	m_oMentions;

			std::vector<CThreadedComment*>  m_arrReplies;
		};

        class CThreadedComments : public OOX::FileGlobalEnumerated, public OOX::IFileContainer, public WritingElementWithChilds<CThreadedComment>
		{
		public:
			CThreadedComments(OOX::Document* pMain);
			CThreadedComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CThreadedComments();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void PrepareTopLevelComments ();

			virtual EElementType getType () const;
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

		public:
			std::unordered_map<std::wstring, CThreadedComment*> m_mapTopLevelThreadedComments;

		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} //Spreadsheet
} // namespace OOX

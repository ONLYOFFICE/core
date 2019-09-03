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
#ifndef OOX_COMMENTS_FILE_INCLUDE_H_
#define OOX_COMMENTS_FILE_INCLUDE_H_

#include "File.h"
#include "../Base/Nullable.h"
#include "FileTypes.h"
#include "WritingElement.h"

#include "../Common/SimpleTypes_Shared.h"
#include "../Common/SimpleTypes_Word.h"

namespace OOX
{
	class CComment : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionConstructors(CComment)
			
		CComment()
		{
		}
		virtual ~CComment()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const
		{
			return L"";
		}

		virtual EElementType getType() const
		{
			return et_w_comment;
		}
		std::wstring getText() const;
	private:
        std::wstring getTextArr(const std::vector<WritingElement* > & arrItems, bool& bFirstPar) const;
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
	public:

		nullable<std::wstring > m_oAuthor;
		nullable<SimpleTypes::CDateTime > m_oDate;
		nullable<SimpleTypes::CDecimalNumber<> > m_oId;
		nullable<std::wstring > m_oInitials;

		//teamlab editor information!!!
		nullable<std::wstring > m_oOOData;

	};

	class CComments : public OOX::File
	{
	public:
		CComments(OOX::Document *pMain);
		CComments(OOX::Document *pMain, const CPath& oPath);
		virtual ~CComments();
		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::Comments;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		std::vector<CComment*>	m_arrComments;
		std::map<int, int>		m_mapComments; //id, index

	};
	class CDocumentComments : public CComments
	{
	public:
		CDocumentComments(OOX::Document *pMain);
		CDocumentComments(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const
		{
			return FileTypes::DocumentComments;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
	};
	class CCommentExt : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CCommentExt)
			CCommentExt()
		{
		}
		virtual ~CCommentExt()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const
		{
			return L"";
		}

		virtual EElementType getType() const
		{
			return et_w15_commentEx;
		}
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<SimpleTypes::CLongHexNumber<> >	m_oParaId;
		nullable<SimpleTypes::CLongHexNumber<> >	m_oParaIdParent;
		nullable<SimpleTypes::COnOff<> >			m_oDone;
	};

	class CCommentsExt : public OOX::File
	{
	public:
		CCommentsExt(OOX::Document *pMain);
		CCommentsExt(OOX::Document *pMain, const CPath& oPath);
		virtual ~CCommentsExt();
		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::CommentsExt;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		std::vector<CCommentExt*>	m_arrComments;
		std::map<unsigned int, int>			m_mapComments; //paraId, index
	};
	class CDocumentCommentsExt : public CCommentsExt
	{
	public:
		CDocumentCommentsExt(OOX::Document *pMain);
		CDocumentCommentsExt(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const
		{
			return FileTypes::DocumentCommentsExt;
		}
	};

	class CCommentId : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CCommentId)
		CCommentId()
		{
		}
		virtual ~CCommentId()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const
		{
			return L"";
		}

		virtual EElementType getType() const
		{
			return et_w16_commentId;
		}
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<SimpleTypes::CLongHexNumber<> >	m_oParaId;
		nullable<SimpleTypes::CLongHexNumber<> >	m_oDurableId;
	};

	class CCommentsIds : public OOX::File
	{
	public:
		CCommentsIds(OOX::Document *pMain);
		CCommentsIds(OOX::Document *pMain, const CPath& oPath);
		virtual ~CCommentsIds();
		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::CommentsIds;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		std::vector<CCommentId*>	m_arrComments;
	};
	class CDocumentCommentsIds : public CCommentsIds
	{
	public:
		CDocumentCommentsIds(OOX::Document *pMain);
		CDocumentCommentsIds(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const
		{
			return FileTypes::DocumentCommentsIds;
		}
	};

	class CPresenceInfo : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CPresenceInfo)
			CPresenceInfo()
		{
		}
		virtual ~CPresenceInfo()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const
		{
			return L"";
		}

		virtual EElementType getType() const
		{
			return et_w15_presenceInfo;
		}
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<std::wstring > m_oProviderId;
		nullable<std::wstring > m_oUserId;
	};

	class CPerson : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CPerson)
		CPerson()
		{
		}
		virtual ~CPerson()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const
		{
			return L"";
		}

		virtual EElementType getType() const
		{
			return et_w15_person;
		}
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<std::wstring > m_oAuthor;
		nullable<CPresenceInfo> m_oPresenceInfo;
	};
	class CPeople : public OOX::File
	{
	public:
		CPeople(OOX::Document *pMain);
		CPeople(OOX::Document *pMain, const CPath& oPath);
		virtual ~CPeople();
		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::People;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		std::vector<CPerson*> m_arrPeoples;
	};

	class CDocumentPeople : public CPeople
	{
	public:
		CDocumentPeople(OOX::Document *pMain);
		CDocumentPeople(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const
		{
			return FileTypes::DocumentPeople;
		}
	};

} // namespace OOX

#endif // OOX_COMMENTS_FILE_INCLUDE_H_

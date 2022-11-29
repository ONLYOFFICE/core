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

#include "File.h"
#include "IFileContainer.h"
#include "../Base/Nullable.h"
#include "FileTypes.h"
#include "WritingElement.h"

#include "../Common/SimpleTypes_Shared.h"
#include "../Common/SimpleTypes_Word.h"

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}
}

namespace OOX
{
	class CComment : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionConstructors(CComment)
			
		CComment(OOX::Document *pMain = NULL);
		virtual ~CComment();

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
				void fromXML2(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;
		std::wstring getText() const;

	private:
        std::wstring getTextArr(const std::vector<WritingElement* > & arrItems, bool& bFirstPar) const;
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<std::wstring > m_oAuthor;
		nullable<SimpleTypes::CDateTime > m_oDate;
		nullable<SimpleTypes::CDecimalNumber > m_oId;
		nullable<std::wstring > m_oInitials;

		//teamlab editor information!!!
		nullable<std::wstring > m_oOOData;

	};

	class CComments : public OOX::File , public OOX::IFileContainer
	{
	public:
		CComments(OOX::Document *pMain);
		CComments(OOX::Document *pMain, const CPath& oPath);
		virtual ~CComments();

		virtual void read(const CPath& oPath);
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		std::vector<CComment*>	m_arrComments;
		std::map<int, int>		m_mapComments; //id, index

	};
	class CDocumentComments : public CComments
	{
	public:
		CDocumentComments(OOX::Document *pMain);
		CDocumentComments(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const;
		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;
	};
	class CCommentExt : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CCommentExt)
		CCommentExt();
		virtual ~CCommentExt();

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<SimpleTypes::CLongHexNumber>	m_oParaId;
		nullable<SimpleTypes::CLongHexNumber>	m_oParaIdParent;
		nullable<SimpleTypes::COnOff>			m_oDone;
	};

	class CCommentsExt : public OOX::File//, public OOX::IFileContainer
	{
	public:
		CCommentsExt(OOX::Document *pMain);
		CCommentsExt(OOX::Document *pMain, const CPath& oPath);
		virtual ~CCommentsExt();
		virtual void read(const CPath& oPath);

		virtual void read(const CPath& oRootPath, const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		std::vector<CCommentExt*>		m_arrComments;
		std::map<unsigned int, int>		m_mapComments; //paraId, index
	};
	class CDocumentCommentsExt : public CCommentsExt
	{
	public:
		CDocumentCommentsExt(OOX::Document *pMain);
		CDocumentCommentsExt(OOX::Document *pMain, const CPath& oPath);

		virtual const OOX::FileType type() const;
	};

	class CCommentExtensible : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CCommentExtensible)
		CCommentExtensible();
		virtual ~CCommentExtensible();

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<SimpleTypes::CLongHexNumber >		m_oDurableId;
		nullable<SimpleTypes::CDateTime >			m_oDateUtc;
		nullable<SimpleTypes::COnOff>				m_oIntelligentPlaceholder;

		nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
	};

	class CCommentsExtensible : public OOX::File//, public OOX::IFileContainer
	{
	public:
		CCommentsExtensible(OOX::Document *pMain);
		CCommentsExtensible(OOX::Document *pMain, const CPath& oPath);
		virtual ~CCommentsExtensible();

		virtual void read(const CPath& oPath);
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		std::vector<CCommentExtensible*>		m_arrComments;
	};
	class CDocumentCommentsExtensible : public CCommentsExtensible
	{
	public:
		CDocumentCommentsExtensible(OOX::Document *pMain);
		CDocumentCommentsExtensible(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const;
	};
	class CCommentsUserData : public CCommentsExtensible
	{
	public:
		CCommentsUserData(OOX::Document *pMain);
		CCommentsUserData(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const;
	};

	//
	class CCommentId : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CCommentId)
		CCommentId();
		virtual ~CCommentId();

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<SimpleTypes::CLongHexNumber>	m_oParaId;
		nullable<SimpleTypes::CLongHexNumber>	m_oDurableId;
	};

	class CCommentsIds : public OOX::File
	{
	public:
		CCommentsIds(OOX::Document *pMain);
		CCommentsIds(OOX::Document *pMain, const CPath& oPath);
		virtual ~CCommentsIds();
		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		std::vector<CCommentId*>	m_arrComments;
	};
	class CDocumentCommentsIds : public CCommentsIds
	{
	public:
		CDocumentCommentsIds(OOX::Document *pMain);
		CDocumentCommentsIds(OOX::Document *pMain, const CPath& oPath);
		virtual const OOX::FileType type() const;
	};

	class CPresenceInfo : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CPresenceInfo)
			
		CPresenceInfo();
		virtual ~CPresenceInfo();

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

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
		CPerson();
		virtual ~CPerson();

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

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
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		std::vector<CPerson*> m_arrPeoples;
	};

	class CDocumentPeople : public CPeople
	{
	public:
		CDocumentPeople(OOX::Document *pMain);
		CDocumentPeople(OOX::Document *pMain, const CPath& oPath);

		virtual const OOX::FileType type() const;
	};

} // namespace OOX

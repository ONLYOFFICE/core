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

#include "../../Base/Nullable.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/ComplexTypes.h"

#include "./../WritingElement.h"

namespace OOX
{
	// Comments 17.13.4
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CCommentRangeEnd 17.13.4.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CCommentRangeEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCommentRangeEnd)

			CCommentRangeEnd(OOX::Document *pMain = NULL);
			CCommentRangeEnd(CCommentRangeEnd *pOther);
			virtual ~CCommentRangeEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
		};

		//--------------------------------------------------------------------------------
		// CCommentRangeStart 17.13.4.4 (Part 1)
		//--------------------------------------------------------------------------------
		class CCommentRangeStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCommentRangeStart)

			CCommentRangeStart(OOX::Document *pMain = NULL);
			virtual ~CCommentRangeStart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
		};
	
	// Revisisons 17.13.5

		//--------------------------------------------------------------------------------
		// CCustomXmlDelRangeEnd 17.13.5.4 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlDelRangeEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlDelRangeEnd)

			CCustomXmlDelRangeEnd(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlDelRangeEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber> m_oId;
		};

		//--------------------------------------------------------------------------------
		// CCustomXmlDelRangeStart 17.13.5.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlDelRangeStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlDelRangeStart)

			CCustomXmlDelRangeStart(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlDelRangeStart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<std::wstring>					m_sAuthor;
            nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oID;
            nullable<std::wstring>					m_sUserId;

		};

		//--------------------------------------------------------------------------------
		// CCustomXmlInsRangeEnd 17.13.5.6 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlInsRangeEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlInsRangeEnd)

			CCustomXmlInsRangeEnd(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlInsRangeEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber> m_oId;
		};

		//--------------------------------------------------------------------------------
		// CCustomXmlInsRangeStart 17.13.5.7 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlInsRangeStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlInsRangeStart)

			CCustomXmlInsRangeStart(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlInsRangeStart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<std::wstring>					m_sAuthor;
            nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oID;
            nullable<std::wstring>					m_sUserId;
		};

		//--------------------------------------------------------------------------------
		// CCustomXmlMoveFromRangeEnd 17.13.5.8 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlMoveFromRangeEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlMoveFromRangeEnd)

			CCustomXmlMoveFromRangeEnd(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlMoveFromRangeEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber> m_oId;
		};

		//--------------------------------------------------------------------------------
		// CCustomXmlMoveFromRangeStart 17.13.5.9 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlMoveFromRangeStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlMoveFromRangeStart)

			CCustomXmlMoveFromRangeStart(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlMoveFromRangeStart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

            nullable<std::wstring>					m_sAuthor;
            nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oID;
            nullable<std::wstring>					m_sUserId;

		};

		//--------------------------------------------------------------------------------
		// CCustomXmlMoveToRangeEnd 17.13.5.10 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlMoveToRangeEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlMoveToRangeEnd)

			CCustomXmlMoveToRangeEnd(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlMoveToRangeEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber> m_oId;
		};

		//--------------------------------------------------------------------------------
		// CCustomXmlMoveToRangeStart 17.13.5.11 (Part 1)
		//--------------------------------------------------------------------------------
		class CCustomXmlMoveToRangeStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomXmlMoveToRangeStart)

			CCustomXmlMoveToRangeStart(OOX::Document *pMain = NULL);
			virtual ~CCustomXmlMoveToRangeStart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

            nullable<std::wstring>					m_sAuthor;
            nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oID;
            nullable<std::wstring>					m_sUserId;

		};

		//--------------------------------------------------------------------------------
		// CMoveFromRangeEnd 17.13.5.23 (Part 1)
		//--------------------------------------------------------------------------------
		class CMoveFromRangeEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMoveFromRangeEnd)

			CMoveFromRangeEnd(OOX::Document *pMain = NULL);
			virtual ~CMoveFromRangeEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
		};

		//--------------------------------------------------------------------------------
		// CMoveFromRangeStart 17.13.5.24 (Part 1)
		//--------------------------------------------------------------------------------
		class CMoveFromRangeStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMoveFromRangeStart)

			CMoveFromRangeStart(OOX::Document *pMain = NULL);
			virtual ~CMoveFromRangeStart();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<std::wstring>							m_sAuthor;
			nullable<SimpleTypes::CDecimalNumber>			m_oColFirst;
			nullable<SimpleTypes::CDecimalNumber>			m_oColLast;
			nullable<SimpleTypes::CDateTime>				m_oDate;
			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
			nullable<std::wstring>							m_sName;
			nullable<std::wstring>							m_sUserId;
		};

		//--------------------------------------------------------------------------------
		// CMoveToRangeEnd 17.13.5.27 (Part 1)
		//--------------------------------------------------------------------------------
		class CMoveToRangeEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMoveToRangeEnd)

			CMoveToRangeEnd(OOX::Document *pMain = NULL);
			virtual ~CMoveToRangeEnd();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
		};

		//--------------------------------------------------------------------------------
		// CMoveToRangeStart 17.13.5.28 (Part 1)
		//--------------------------------------------------------------------------------
		class CMoveToRangeStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMoveToRangeStart)

			CMoveToRangeStart(OOX::Document *pMain = NULL);
			virtual ~CMoveToRangeStart();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<std::wstring> m_sAuthor;
			nullable<SimpleTypes::CDecimalNumber>			m_oColFirst;
			nullable<SimpleTypes::CDecimalNumber>			m_oColLast;
			nullable<SimpleTypes::CDateTime>				m_oDate;
			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
            nullable<std::wstring>							m_sName;
            nullable<std::wstring>							m_sUserId;
		};

		class CMoveFrom : public WritingElementWithChilds<>
		{
		public:
			CMoveFrom(OOX::Document *pMain = NULL);
			CMoveFrom(XmlUtils::CXmlNode &oNode);
			CMoveFrom(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CMoveFrom();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring>						m_sAuthor;
			nullable<SimpleTypes::CDateTime>			m_oDate;
			nullable<SimpleTypes::CDecimalNumber>		m_oId;
			nullable<std::wstring>						m_sUserId;
		};

		class CMoveTo : public WritingElementWithChilds<>
		{
		public:
			CMoveTo(OOX::Document *pMain = NULL);
			CMoveTo(XmlUtils::CXmlNode &oNode);
			CMoveTo(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CMoveTo();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			// Childs
		};
	
	// Bookmarks 17.13.6

		//--------------------------------------------------------------------------------
		// CBookmarkEnd 17.13.6.1 (Part 1)
		//--------------------------------------------------------------------------------
		class CBookmarkEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBookmarkEnd)

			CBookmarkEnd(OOX::Document *pMain = NULL);
			CBookmarkEnd(CBookmarkEnd* pOther);
			virtual ~CBookmarkEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
		};

		//--------------------------------------------------------------------------------
		// CBookmarkStart 17.13.6.2 (Part 1)
		//--------------------------------------------------------------------------------
		class CBookmarkStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBookmarkStart)

			CBookmarkStart(OOX::Document *pMain = NULL);
			CBookmarkStart(CBookmarkStart *pOther);
			virtual ~CBookmarkStart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber>			m_oColFirst;
			nullable<SimpleTypes::CDecimalNumber>			m_oColLast;
			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
			nullable<SimpleTypes::CDecimalNumber>			m_oId;
            nullable<std::wstring>							m_sName;
		};


	// Range Permissions 17.13.7

		//--------------------------------------------------------------------------------
		// CPermEnd 17.13.7.1 (Part 1)
		//--------------------------------------------------------------------------------
		class CPermEnd : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPermEnd)

			CPermEnd(OOX::Document *pMain = NULL);
			virtual ~CPermEnd();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
            nullable<std::wstring>							m_sId;
		};

		//--------------------------------------------------------------------------------
		// CPermStart 17.13.7.2 (Part 1)
		//--------------------------------------------------------------------------------
		class CPermStart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPermStart)

			CPermStart(OOX::Document *pMain = NULL);
			virtual ~CPermStart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber>			m_oColFirst;
			nullable<SimpleTypes::CDecimalNumber>			m_oColLast;
			nullable<SimpleTypes::CDisplacedByCustomXml>	m_oDisplacedByCustomXml;
            nullable<std::wstring>							m_sEd;
			nullable<SimpleTypes::CEdGrp>					m_oEdGrp;
            nullable<std::wstring>							m_sId;
		};


	// Spelling and Grammar 17.13.8

		//--------------------------------------------------------------------------------
		// CProofErr 17.13.8.1 (Part 1)
		//--------------------------------------------------------------------------------
		class CProofErr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProofErr)

			CProofErr(OOX::Document *pMain = NULL);
			virtual ~CProofErr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CProofErr> m_oType;
		};

		class CIns : public WritingElementWithChilds<>
		{
		public:
			CIns(OOX::Document *pMain = NULL);
			CIns(XmlUtils::CXmlNode &oNode);
			CIns(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CIns();;

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private: 
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
            nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
            nullable<std::wstring>					m_sUserId;

			// Childs
		};

		class CDel : public WritingElementWithChilds<>
		{
		public:
			CDel(OOX::Document *pMain = NULL);
			CDel(XmlUtils::CXmlNode &oNode);
			CDel(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CDel();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private: 
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
            nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
            nullable<std::wstring>					m_sUserId;

			// Childs
		};

	// Revisions 17.13.5

	} // Logic
} // OOX


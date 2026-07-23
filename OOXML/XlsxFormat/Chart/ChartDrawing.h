/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../../DocxFormat/Document.h"
#include "../../DocxFormat/IFileContainer.h"

namespace PPTX
{
	namespace Logic
	{
		class SpTreeElem;
	}
}

namespace OOX
{
	class CFromTo : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CFromTo)
		CFromTo();
		virtual ~CFromTo();

		virtual void fromXML(XmlUtils::CXmlNode& node);
		virtual std::wstring toXML() const;

		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual EElementType getType () const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		std::wstring	m_sName;
		nullable_double	m_oX;
		nullable_double	m_oY;
	};

	class CExt : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CExt)
		CExt();
		virtual ~CExt();

		virtual void fromXML(XmlUtils::CXmlNode& node);

		virtual std::wstring toXML() const;
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual EElementType getType () const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable_double	m_oCx;
		nullable_double	m_oCy;
	};

	class CSizeAnchor : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CSizeAnchor)
		CSizeAnchor();
		virtual ~CSizeAnchor();

		virtual void fromXML(XmlUtils::CXmlNode& node) = 0;
        virtual std::wstring toXML() const = 0;
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const = 0;
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) = 0;
		virtual EElementType getType () const = 0;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<OOX::CFromTo>				m_oFrom;
		nullable<PPTX::Logic::SpTreeElem>	m_oElement;
	};

	class CRelSizeAnchor : public CSizeAnchor
	{
	public:
		WritingElement_AdditionMethods(CRelSizeAnchor)
		CRelSizeAnchor();
		virtual ~CRelSizeAnchor();

		virtual void fromXML(XmlUtils::CXmlNode& node);

		virtual std::wstring toXML() const;
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual EElementType getType () const;

		nullable<OOX::CFromTo> m_oTo;
	};

	class CAbsSizeAnchor : public CSizeAnchor
	{
	public:
		WritingElement_AdditionMethods(CAbsSizeAnchor)
		CAbsSizeAnchor();
		virtual ~CAbsSizeAnchor();

		virtual void fromXML(XmlUtils::CXmlNode& node);

		virtual std::wstring toXML() const;
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual EElementType getType () const;

		nullable<OOX::CExt> m_oExt;
	};

	class CChartDrawing : public OOX::WritingElementWithChilds<OOX::CSizeAnchor>, public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CChartDrawing(OOX::Document* pMain, bool bDocument = true);
		CChartDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
		virtual ~CChartDrawing();

		virtual void ClearItems();

		virtual void read(const CPath& oPath);
		void fromXML(XmlUtils::CXmlNode &);
		std::wstring toXML() const;
		virtual void read(const CPath& oRootPath, const CPath& oPath);
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		const CPath GetReadPath();

	private:	
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		bool m_bDocument = false;

	public:
//reading
        CPath	m_oReadPath;
	};
} // namespace OOX

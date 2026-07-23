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

#include "IFileContainer.h"
#include "Logic/Vml.h"

#include "../XlsxFormat/Comments/Comments.h"
#include "Document.h"

namespace OOX
{

//VmlDrawing file can contain both the display part of Xlsx comments and regular objects 

	class CVmlDrawing : public OOX::WritingElementWithChilds<OOX::WritingElement>, public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		struct _vml_shape
		{
			_vml_shape() : bUsed(false), pElement(NULL), nId(0), bComment(false) {}
			
			int						nId;		// for comments
			std::wstring			sXml;		// for pptx 
			OOX::WritingElement*	pElement;	// for docx/xlsx
			bool					bUsed;		// for single drawing
			bool					bComment;
		};
		CVmlDrawing(OOX::Document* pMain, bool bDocument = true) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument		= bDocument;
			m_mapComments	= NULL;
			m_lObjectIdVML	= 1024;
		}
		CVmlDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = (NULL != dynamic_cast<OOX::CDocument*>(pMain));;
			m_mapComments = NULL;
			read( oRootPath, oPath );
		}
		virtual ~CVmlDrawing()
		{
		}
		void ClearShapeTypes();
		virtual void read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void fromXML(XmlUtils::CXmlNode &)
		{
		}
		std::wstring toXML() const
		{
			return _T("");
		}
		void read(std::wstring & fileContent);
		virtual void read(const CPath& oRootPath, const CPath& oPath);
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
		virtual const OOX::FileType type() const
		{
			return OOX::FileTypes::VmlDrawing;
		}
		virtual const CPath DefaultDirectory() const
		{
			if (m_bDocument) return type().DefaultDirectory();
			else	return L"../" + type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath GetReadPath()
		{
			return m_oReadPath;
		}
		bool IsEmpty();
		smart_ptr<OOX::WritingElement> FindVmlObject(const std::wstring &spid);
	private:
	
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		bool m_bDocument;

	public:
//reading
        CPath m_oReadPath;
        std::map<std::wstring, _vml_shape> m_mapShapes;
		std::vector<_vml_shape> m_arrShapeTypes;

		std::wstring m_sFileContent;
//writing
        std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>* m_mapComments;
        std::vector<std::wstring> m_arObjectXml;
        std::vector<std::wstring> m_arControlXml;

		long m_lObjectIdVML;
	};
} // namespace OOX
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

#include "IFileContainer.h"
#include "Logic/Vml.h"

#include "../XlsxFormat/Comments/Comments.h"
#include "Document.h"

namespace OOX
{

//в файле VmlDrawing могут быть как отобразительная часть комментариев Xlsx, так и просто обычные объекты 

	class CVmlDrawing : public OOX::WritingElementWithChilds<OOX::WritingElement>, public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		struct _vml_shape
		{
			_vml_shape() : bUsed(false), pElement(NULL), nId(0) {}
			
			int						nId;		// for comments
			std::wstring			sXml;		// for pptx 
			OOX::WritingElement*	pElement;	// for docx/xlsx
			bool					bUsed;		// for single drawing
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
        CPath                                                                   m_oReadPath;
        boost::unordered_map<std::wstring, _vml_shape>							m_mapShapes;
		std::vector<_vml_shape>													m_arrShapeTypes;

		std::wstring															m_sFileContent;
//writing
        std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>*				m_mapComments;
        std::vector<std::wstring>                                               m_arObjectXml;
        std::vector<std::wstring>                                               m_arControlXml;

		long                                                                    m_lObjectIdVML;
	};
} // namespace OOX
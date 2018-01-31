﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "Docx.h"

#include "App.h"
#include "Core.h"
#include "Document.h"
#include "FontTable.h"
#include "Numbering.h"
#include "Comments.h"
#include "Styles.h"
#include "Footnote.h"
#include "Endnote.h"
#include "Settings/WebSettings.h"
#include "Settings/Settings.h"
#include "External/HyperLink.h"
#include "Media/Image.h"
#include "Media/OleObject.h"
#include "Media/ActiveX.h"
#include "Media/VbaProject.h"
#include "Media/JsaProject.h"
#include "HeaderFooter.h"

#include "../../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

namespace OOX {
	
    bool CDocx::Read(const CPath& oFilePath)
    {
        // Ищем "/_rels/.rels" и читаем все файлы по рельсам
        OOX::CRels oRels( oFilePath / FILE_SEPARATOR_STR );
        IFileContainer::Read( oRels, oFilePath, oFilePath );

         return true;
	}
	OOX::CHdrFtr *CDocx::GetHeaderOrFooter(const OOX::RId& rId) const
	{
		if ( m_pDocument )
		{
			OOX::IFileContainer* pDocumentContainer = (OOX::IFileContainer*)m_pDocument;

            smart_ptr<OOX::File> pFile = pDocumentContainer->Find( rId );
			if ( pFile.IsInit() && ( OOX::FileTypes::Header == pFile->type() || OOX::FileTypes::Footer == pFile->type() ) )
				return (OOX::CHdrFtr*)pFile.operator->();
			else 
				return NULL;
		}
		
		return NULL;
	}	
}

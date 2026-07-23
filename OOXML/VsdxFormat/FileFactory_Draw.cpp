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
#include "FileTypes_Draw.h"
#include "Vsdx.h"
#include "VisioDocument.h"
#include "VisioConnections.h"
#include "VisioPages.h"
#include "VisioOthers.h"
 //#include "Comments.h"
#include "../DocxFormat/Rels.h"

#ifdef CreateFile
#undef CreateFile
#endif

namespace OOX
{
	namespace Draw
	{
		smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath, const OOX::Rels::CRelationShip& oRelation, OOX::Document *pMain)
		{
			OOX::CPath	oRelationFilename = oRelation.Filename();
			CPath oFileName;
			
			if (oRelation.IsExternal())
			{
				oFileName = oRelationFilename;
			}
			else
			{
				if (oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
					oFileName = oRootPath / oRelationFilename;
				else
					oFileName = oPath / oRelationFilename;
			}
			if ( oRelation.Type() == FileTypes::Document || oRelation.Type() == FileTypes::DocumentMacro)
				return smart_ptr<OOX::File>(new CDocumentFile( pMain, oRootPath, oFileName ));
			else if (oRelation.Type() == FileTypes::Pages)
				return smart_ptr<OOX::File>(new CPagesFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Masters)
				return smart_ptr<OOX::File>(new CMastersFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Page)
				return smart_ptr<OOX::File>(new CPageFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Master)
				return smart_ptr<OOX::File>(new CMasterFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Recordsets)
				return smart_ptr<OOX::File>(new CRecordsetsFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Recordset)
				return smart_ptr<OOX::File>(new CRecordsetFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Connections)
				return smart_ptr<OOX::File>(new CConnectionsFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Windows)
				return smart_ptr<OOX::File>(new CWindowsFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Validation)
				return smart_ptr<OOX::File>(new CValidationFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Comments)
				return smart_ptr<OOX::File>(new CCommentsFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Solutions)
				return smart_ptr<OOX::File>(new CSolutionsFile(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Solution)
				return smart_ptr<OOX::File>(new CSolutionFile(pMain, oRootPath, oFileName));

			return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
		}
        smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath,  OOX::Rels::CRelationShip* pRelation, OOX::Document *pMain)
		{
			if (pRelation == NULL) return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
			
			OOX::CPath oRelationFilename = pRelation->Filename();
			CPath oFileName;

			if (pRelation->IsExternal())
			{
				oFileName = oRelationFilename;
			}
			else
			{
				if (oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
					oFileName = oRootPath / oRelationFilename;
				else
					oFileName = oPath / oRelationFilename;
			}
			if ( pRelation->Type() == FileTypes::Document || pRelation->Type() == FileTypes::DocumentMacro)
				return smart_ptr<OOX::File>(new CDocumentFile( pMain, oRootPath, oFileName ));
			else if (pRelation->Type() == FileTypes::Pages)
				return smart_ptr<OOX::File>(new CPagesFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Masters)
				return smart_ptr<OOX::File>(new CMastersFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Page)
				return smart_ptr<OOX::File>(new CPageFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Master)
				return smart_ptr<OOX::File>(new CMasterFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Comments)
				return smart_ptr<OOX::File>(new CCommentsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Connections)
				return smart_ptr<OOX::File>(new CConnectionsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Windows)
				return smart_ptr<OOX::File>(new CWindowsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Validation)
				return smart_ptr<OOX::File>(new CValidationFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Recordsets)
				return smart_ptr<OOX::File>(new CRecordsetsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Recordset)
				return smart_ptr<OOX::File>(new CRecordsetFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Solutions)
				return smart_ptr<OOX::File>(new CSolutionsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Solution)
				return smart_ptr<OOX::File>(new CSolutionFile(pMain, oRootPath, oFileName));
			return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
		}
	}
} // namespace OOX

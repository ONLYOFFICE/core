/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "FileTypes_Draw.h"
#include "Vsdx.h"
#include "VisioDocument.h"
#include "VisioConnections.h"
#include "VisioPages.h"
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
				return smart_ptr<OOX::File>(new CValidation(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == FileTypes::Comments)
				return smart_ptr<OOX::File>(new CComments(pMain, oRootPath, oFileName));

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
				return smart_ptr<OOX::File>(new CComments(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Connections)
				return smart_ptr<OOX::File>(new CConnectionsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Windows)
				return smart_ptr<OOX::File>(new CWindowsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Validation)
				return smart_ptr<OOX::File>(new CValidation(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Recordsets)
				return smart_ptr<OOX::File>(new CRecordsetsFile(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::Recordset)
				return smart_ptr<OOX::File>(new CRecordsetFile(pMain, oRootPath, oFileName));

			return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
		}
	}
} // namespace OOX

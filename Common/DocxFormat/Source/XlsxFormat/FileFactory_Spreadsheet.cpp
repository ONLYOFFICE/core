//#include "stdafx.h"
#include "CommonInclude.h"

#include "FileTypes_Spreadsheet.h"

#include "../DocxFormat/Theme/Theme.h"
#include "../DocxFormat/Theme/ThemeOverride.h"
#include "../DocxFormat/VmlDrawing.h"


#include "SharedStrings/SharedStrings.h"
#include "Styles/Styles.h"
#include "Workbook/Workbook.h"
#include "Worksheets/Worksheet.h"
#include "CalcChain/CalcChain.h"
#include "Drawing/Image.h"
#include "Table/Table.h"
#include "ExternalLinks/ExternalLinks.h"
#include "ExternalLinks/ExternalLinkPath.h"

namespace OOX
{
	namespace Spreadsheet
	{
		smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath, const OOX::Rels::CRelationShip& oRelation)
		{
			OOX::CPath oRelationFilename = oRelation.Filename();
			CPath oFileName;
			if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().GetLength() > 0)
				oFileName = oRootPath / oRelationFilename;
			else
				oFileName = oPath / oRelationFilename;

			if ( oRelation.Type() == FileTypes::Workbook )
				return smart_ptr<OOX::File>(new CWorkbook( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::SharedStrings )
				return smart_ptr<OOX::File>(new CSharedStrings( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Styles )
				return smart_ptr<OOX::File>(new CStyles( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Worksheet )
				return smart_ptr<OOX::File>(new CWorksheet( oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::Theme )
				return smart_ptr<OOX::File>(new CTheme( oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::ThemeOverride )
				return smart_ptr<OOX::File>(new CThemeOverride( oFileName ));
			else if ( oRelation.Type() == FileTypes::Drawings )
				return smart_ptr<OOX::File>(new CDrawing( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::CalcChain )
				return smart_ptr<OOX::File>(new CCalcChain( oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::Image )
				return smart_ptr<OOX::File>(new Image( oFileName ));
			else if ( oRelation.Type() == FileTypes::Chartsheets )
				return smart_ptr<OOX::File>(new CWorksheet( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::VmlDrawing )
				return smart_ptr<OOX::File>(new CVmlDrawing( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Charts )
				return smart_ptr<OOX::File>(new CChartSpace( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::ExternalLinks )
				return smart_ptr<OOX::File>(new CExternalLink( oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::ExternalLinkPath)
				return smart_ptr<OOX::File>(new ExternalLinkPath( oRelation.Target()));

			return smart_ptr<OOX::File>( new UnknowTypeFile() );
		}
        smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath,  OOX::Rels::CRelationShip* pRelation)
		{
			if (pRelation == NULL) return smart_ptr<OOX::File>( new UnknowTypeFile() );
			
			OOX::CPath oRelationFilename = pRelation->Filename();
			CPath oFileName;
			if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().GetLength() > 0)
				oFileName = oRootPath / oRelationFilename;
			else
				oFileName = oPath / oRelationFilename;

			if ( pRelation->Type() == FileTypes::Workbook )
				return smart_ptr<OOX::File>(new CWorkbook( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::SharedStrings )
				return smart_ptr<OOX::File>(new CSharedStrings( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Styles )
				return smart_ptr<OOX::File>(new CStyles( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Worksheet )
				return smart_ptr<OOX::File>(new CWorksheet( oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::Theme )
				return smart_ptr<OOX::File>(new CTheme( oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::ThemeOverride )
				return smart_ptr<OOX::File>(new CThemeOverride( oFileName ));
			else if ( pRelation->Type() == FileTypes::Drawings )
				return smart_ptr<OOX::File>(new CDrawing( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::CalcChain )
				return smart_ptr<OOX::File>(new CCalcChain( oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::Image )
				return smart_ptr<OOX::File>(new Image( oFileName ));
			else if ( pRelation->Type() == FileTypes::Chartsheets )
				return smart_ptr<OOX::File>(new CWorksheet( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::VmlDrawing )
				return smart_ptr<OOX::File>(new CVmlDrawing( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Charts )
				return smart_ptr<OOX::File>(new CChartSpace( oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::ExternalLinks )
				return smart_ptr<OOX::File>(new CExternalLink( oRootPath, oFileName ));
			else if (  pRelation->Type() == FileTypes::ExternalLinkPath)
				return smart_ptr<OOX::File>(new ExternalLinkPath( oRelationFilename ));

			return smart_ptr<OOX::File>( new UnknowTypeFile() );
		}
	}
} // namespace OOX

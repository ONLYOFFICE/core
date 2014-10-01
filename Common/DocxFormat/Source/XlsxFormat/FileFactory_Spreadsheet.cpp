//#include "stdafx.h"
#include "CommonInclude.h"

//#include "../../../../../AVSEBOOKWriter/Common/File.h"
#include "FileTypes_Spreadsheet.h"

#include "SharedStrings/SharedStrings.h"
#include "Styles/Styles.h"
#include "Workbook/Workbook.h"
#include "Worksheets/Worksheet.h"
#include "CalcChain/CalcChain.h"
#include "../DocxFormat/Theme/Theme.h"
#include "../DocxFormat/Theme/ThemeOverride.h"
#include "Drawing/Image.h"
#include "Table/Table.h"

namespace OOX
{
	namespace Spreadsheet
	{
		smart_ptr<OOX::File> CreateFile(const OOX::CPath& oPath, const OOX::Rels::CRelationShip& oRelation)
		{
			const CPath oFileName = oPath / oRelation.Filename();

			if ( oRelation.Type() == FileTypes::Workbook )
				return smart_ptr<OOX::File>(new CWorkbook( oFileName ));
			else if ( oRelation.Type() == FileTypes::SharedStrings )
				return smart_ptr<OOX::File>(new CSharedStrings( oFileName ));
			else if ( oRelation.Type() == FileTypes::Styles )
				return smart_ptr<OOX::File>(new CStyles( oFileName ));
			else if ( oRelation.Type() == FileTypes::Worksheet )
				return smart_ptr<OOX::File>(new CWorksheet( oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::Theme )
				return smart_ptr<OOX::File>(new CTheme( oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::ThemeOverride )
				return smart_ptr<OOX::File>(new CThemeOverride( oFileName ));
			else if ( oRelation.Type() == FileTypes::Drawings )
				return smart_ptr<OOX::File>(new CDrawing( oFileName ));
			else if ( oRelation.Type() == FileTypes::CalcChain )
				return smart_ptr<OOX::File>(new CCalcChain( oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::Image )
				return smart_ptr<OOX::File>(new Image( oFileName ));
			else if ( oRelation.Type() == FileTypes::Chartsheets )
				return smart_ptr<OOX::File>(new CWorksheet( oFileName ));
			else if ( oRelation.Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( oFileName ));
			else if ( oRelation.Type() == FileTypes::LegacyDrawings )
				return smart_ptr<OOX::File>(new CLegacyDrawing( oFileName ));
			else if ( oRelation.Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( oFileName ));
			else if ( oRelation.Type() == FileTypes::Charts )
				return smart_ptr<OOX::File>(new CChartSpace( oFileName ));

			return smart_ptr<OOX::File>( new UnknowTypeFile() );
		}
        smart_ptr<OOX::File> CreateFile(const OOX::CPath& oPath,  OOX::Rels::CRelationShip* pRelation)
		{
			if (pRelation == NULL) return smart_ptr<OOX::File>( new UnknowTypeFile() );
			
			const CPath oFileName = oPath / pRelation->Filename();

			if ( pRelation->Type() == FileTypes::Workbook )
				return smart_ptr<OOX::File>(new CWorkbook( oFileName ));
			else if ( pRelation->Type() == FileTypes::SharedStrings )
				return smart_ptr<OOX::File>(new CSharedStrings( oFileName ));
			else if ( pRelation->Type() == FileTypes::Styles )
				return smart_ptr<OOX::File>(new CStyles( oFileName ));
			else if ( pRelation->Type() == FileTypes::Worksheet )
				return smart_ptr<OOX::File>(new CWorksheet( oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::Theme )
				return smart_ptr<OOX::File>(new CTheme( oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::ThemeOverride )
				return smart_ptr<OOX::File>(new CThemeOverride( oFileName ));
			else if ( pRelation->Type() == FileTypes::Drawings )
				return smart_ptr<OOX::File>(new CDrawing( oFileName ));
			else if ( pRelation->Type() == FileTypes::CalcChain )
				return smart_ptr<OOX::File>(new CCalcChain( oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::Image )
				return smart_ptr<OOX::File>(new Image( oFileName ));
			else if ( pRelation->Type() == FileTypes::Chartsheets )
				return smart_ptr<OOX::File>(new CWorksheet( oFileName ));
			else if ( pRelation->Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( oFileName ));
			else if ( pRelation->Type() == FileTypes::LegacyDrawings )
				return smart_ptr<OOX::File>(new CLegacyDrawing( oFileName ));
			else if ( pRelation->Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( oFileName ));
			else if ( pRelation->Type() == FileTypes::Charts )
				return smart_ptr<OOX::File>(new CChartSpace( oFileName ));

			return smart_ptr<OOX::File>( new UnknowTypeFile() );
		}
	}
} // namespace OOX

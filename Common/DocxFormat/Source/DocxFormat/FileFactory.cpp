#include "FileFactory.h"
#include "File.h"
#include "FileTypes.h"
#include "Rels.h"
#include "App.h"
#include "Core.h"
#include "Document.h"
#include "Theme/Theme.h"
#include "Theme/ThemeOverride.h"
#include "Settings/Settings.h"
#include "Settings/WebSettings.h"
#include "FontTable.h"
#include "Styles.h"
#include "Bibliography.h"
#include "Footnote.h"
#include "Endnote.h"
#include "Media/Image.h"
#include "Media/OleObject.h"
#include "Media/Audio.h"
#include "Media/Video.h"
#include "External/HyperLink.h"
#include "External/ExternalVideo.h"
#include "External/ExternalAudio.h"
#include "External/ExternalImage.h"
#include "HeaderFooter.h"
#include "Numbering.h"
#include "Comments.h"
#include "UnknowTypeFile.h"
#include "Diagram/DiagramDrawing.h"


namespace OOX
{
	const smart_ptr<OOX::File> CreateFile(const OOX::CPath& oPath, const OOX::Rels::CRelationShip& oRelation)
	{
		const CPath oFileName = oPath / oRelation.Filename();

		if ( oRelation.Type() == FileTypes::App )
			return smart_ptr<OOX::File>(new CApp( oFileName ));
		else if ( oRelation.Type() == FileTypes::Core)
			return smart_ptr<OOX::File>(new CCore( oFileName ));
		else if ( oRelation.Type() == FileTypes::Document)
			return smart_ptr<OOX::File>(new CDocument( oFileName ));
		else if ( oRelation.Type() == FileTypes::Theme)
			return smart_ptr<OOX::File>(new CTheme( oFileName ));
		else if ( oRelation.Type() == FileTypes::ThemeOverride)
			return smart_ptr<OOX::File>(new CThemeOverride( oFileName ));
		else if ( oRelation.Type() == FileTypes::Setting)
			return smart_ptr<OOX::File>(new CSettings( oFileName ));
		else if ( oRelation.Type() == FileTypes::FontTable)
			return smart_ptr<OOX::File>(new CFontTable( oFileName ));
		else if ( oRelation.Type() == FileTypes::Style)
			return smart_ptr<OOX::File>(new CStyles( oFileName ));
		else if ( oRelation.Type() == FileTypes::Bibliography)
			return smart_ptr<OOX::File>(new CBibliography( oFileName ));
		else if ( oRelation.Type() == FileTypes::FootNote)
			return smart_ptr<OOX::File>(new CFootnotes( oFileName ));
		else if ( oRelation.Type() == FileTypes::EndNote)
			return smart_ptr<OOX::File>(new CEndnotes( oFileName ));
		else if ( oRelation.Type() == FileTypes::WebSetting)
			return smart_ptr<OOX::File>(new CWebSettings( oFileName ));
		else if ( oRelation.Type() == FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new HyperLink( oRelation.Target()));
		else if (( oRelation.Type() == FileTypes::ExternalVideo ) && ( oRelation.IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalVideo( oRelation.Target()));
		else if (( oRelation.Type() == FileTypes::ExternalAudio ) && ( oRelation.IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalAudio( oRelation.Target()));
		else if (( oRelation.Type() == FileTypes::ExternalImage ) && ( oRelation.IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalImage( oRelation.Target()));
		else if ( oRelation.Type() == FileTypes::Image)
			return smart_ptr<OOX::File>(new Image( oFileName ));
		else if ( oRelation.Type() == FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OleObject( oFileName ));
		else if ( oRelation.Type() == FileTypes::Audio)
			return smart_ptr<OOX::File>(new Audio( oFileName ));
		else if ( oRelation.Type() == FileTypes::Video)
			return smart_ptr<OOX::File>(new Video( oFileName ));
		else if ( oRelation.Type() == FileTypes::Numbering)
			return smart_ptr<OOX::File>(new CNumbering( oFileName ));
		else if ( oRelation.Type() == FileTypes::Header)
			return smart_ptr<OOX::File>(new CHdrFtr( oFileName ));
		else if ( oRelation.Type() == FileTypes::Footer)
			return smart_ptr<OOX::File>(new CHdrFtr( oFileName ));
		else if ( oRelation.Type() == FileTypes::Comments)
			return smart_ptr<OOX::File>(new CComments( oFileName ));
		else if ( oRelation.Type() == FileTypes::CommentsExt )
			return smart_ptr<OOX::File>(new CCommentsExt( oFileName ));
		else if ( oRelation.Type() == FileTypes::People )
			return smart_ptr<OOX::File>(new CPeople( oFileName ));
////
		else if (oRelation.Type() == FileTypes::Data)				// нужен только filepath
			return smart_ptr<OOX::File>(new Image( oFileName ));
		else if (oRelation.Type() == FileTypes::DiagDrawing)
			return smart_ptr<OOX::File>(new CDiagramDrawing( oFileName )); 

		return smart_ptr<OOX::File>( new UnknowTypeFile() );
	}

	const smart_ptr<OOX::File> CreateFile(const OOX::CPath& oPath, OOX::Rels::CRelationShip* pRelation)
	{
		if (pRelation == NULL) return smart_ptr<OOX::File>( new UnknowTypeFile() );

		const CPath oFileName = oPath / pRelation->Filename();

		if ( pRelation->Type() == FileTypes::App )
			return smart_ptr<OOX::File>(new CApp( oFileName ));
		else if ( pRelation->Type() == FileTypes::Core)
			return smart_ptr<OOX::File>(new CCore( oFileName ));
		else if ( pRelation->Type() == FileTypes::Document)
			return smart_ptr<OOX::File>(new CDocument( oFileName ));
		else if ( pRelation->Type() == FileTypes::Theme)
			return smart_ptr<OOX::File>(new CTheme( oFileName ));
		else if ( pRelation->Type() == FileTypes::ThemeOverride)
			return smart_ptr<OOX::File>(new CThemeOverride( oFileName ));
		else if ( pRelation->Type() == FileTypes::Setting)
			return smart_ptr<OOX::File>(new CSettings( oFileName ));
		else if ( pRelation->Type() == FileTypes::FontTable)
			return smart_ptr<OOX::File>(new CFontTable( oFileName ));
		else if ( pRelation->Type() == FileTypes::Style)
			return smart_ptr<OOX::File>(new CStyles( oFileName ));
		else if ( pRelation->Type() == FileTypes::Bibliography)
			return smart_ptr<OOX::File>(new CBibliography( oFileName ));
		else if ( pRelation->Type() == FileTypes::FootNote)
			return smart_ptr<OOX::File>(new CFootnotes( oFileName ));
		else if ( pRelation->Type() == FileTypes::EndNote)
			return smart_ptr<OOX::File>(new CEndnotes( oFileName ));
		else if ( pRelation->Type() == FileTypes::WebSetting)
			return smart_ptr<OOX::File>(new CWebSettings( oFileName ));
		else if ( pRelation->Type() == FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new HyperLink( pRelation->Target()));
		else if (( pRelation->Type() == FileTypes::ExternalVideo ) && ( pRelation->IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalVideo( pRelation->Target()));
		else if (( pRelation->Type() == FileTypes::ExternalAudio ) && ( pRelation->IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalAudio( pRelation->Target()));
		else if (( pRelation->Type() == FileTypes::ExternalImage ) && ( pRelation->IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalImage( pRelation->Target()));
		else if ( pRelation->Type() == FileTypes::Image)
			return smart_ptr<OOX::File>(new Image( oFileName ));
		else if ( pRelation->Type() == FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OleObject( oFileName ));
		else if ( pRelation->Type() == FileTypes::Audio)
			return smart_ptr<OOX::File>(new Audio( oFileName ));
		else if ( pRelation->Type() == FileTypes::Video)
			return smart_ptr<OOX::File>(new Video( oFileName ));
		else if ( pRelation->Type() == FileTypes::Numbering)
			return smart_ptr<OOX::File>(new CNumbering( oFileName ));
		else if ( pRelation->Type() == FileTypes::Header)
			return smart_ptr<OOX::File>(new CHdrFtr( oFileName ));
		else if ( pRelation->Type() == FileTypes::Footer)
			return smart_ptr<OOX::File>(new CHdrFtr( oFileName ));
		else if ( pRelation->Type() == FileTypes::Comments)
			return smart_ptr<OOX::File>(new CComments( oFileName ));
		else if ( pRelation->Type() == FileTypes::CommentsExt )
			return smart_ptr<OOX::File>(new CCommentsExt( oFileName ));
		else if ( pRelation->Type() == FileTypes::People )
			return smart_ptr<OOX::File>(new CPeople( oFileName ));
////
		else if (pRelation->Type() == FileTypes::Data)				// нужен только filepath
			return smart_ptr<OOX::File>(new Image( oFileName ));
		else if (pRelation->Type() == FileTypes::DiagDrawing)
			return smart_ptr<OOX::File>(new CDiagramDrawing( oFileName )); 

		return smart_ptr<OOX::File>( new UnknowTypeFile() );
	}

} // namespace OOX

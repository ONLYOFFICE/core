
#include "DestinationCommand.h"
#include "RtfOle.h"

bool ShapeReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("shp") == sCommand )
		return true;
	else if( _T("shpinst") == sCommand )
		return true;
	else if( _T("shprslt") == sCommand )
		return false;
	else if( _T("picprop") == sCommand )
		return true;
	else if( _T("shptxt") == sCommand )
	{
		if( PROP_DEF == m_oShape.m_nShapeType )
			m_oShape.m_nShapeType = 202;//Text box
		ParagraphReader oParagraphReader(_T("shptxt"), oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oShape.m_aTextItems = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if( _T("sp") == sCommand )
	{
		ShapePropertyReader oShapePropReader(m_oShape);
		StartSubReader(oShapePropReader, oDocument, oReader);
	}
	else if( _T("shpleft") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nLeft = parameter;
	}
	else if( _T("shptop") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nTop = parameter;
	}
	else if( _T("shpbottom") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nBottom = parameter;
	}
	else if( _T("shpright") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nRight = parameter;
	}
	else if( _T("shplid") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oShape.m_nID = parameter;
			oDocument.SetShapeId( parameter );
		}
	}
	else if( _T("shpz") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nZOrder = parameter;
	}
	else if( _T("shpfhdr") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nHeader = parameter;
	}
	//else if( _T("shpbxignore") == sCommand )
	//	m_oShape.m_eXAnchor = RtfShape::ax_ignore;
	else if( _T("shpbxpage") == sCommand )
		m_oShape.m_eXAnchor = RtfShape::ax_page;
	else if( _T("shpbxmargin") == sCommand )
		m_oShape.m_eXAnchor = RtfShape::ax_margin;
	else if( _T("shpbxcolumn") == sCommand )
		m_oShape.m_eXAnchor = RtfShape::ax_column;
	//else if( _T("shpbyignore") == sCommand )
	//	m_oShape.m_eYAnchor = RtfShape::ay_ignore;
	else if( _T("shpbypage") == sCommand )
		m_oShape.m_eYAnchor = RtfShape::ay_page;
	else if( _T("shpbymargin") == sCommand )
		m_oShape.m_eYAnchor = RtfShape::ay_margin;
	else if( _T("shpbypara") == sCommand )
		m_oShape.m_eYAnchor = RtfShape::ay_Para;
	else if( _T("shpwr") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nWrapType = parameter;
	}
	else if( _T("shpwrk") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nWrapSideType = parameter;
	}
	else if( _T("shpfblwtxt") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nZOrderRelative = parameter;
	}
	else if( _T("shplockanchor") == sCommand )
		m_oShape.m_bLockAnchor = true;
	else
		return false;
	return true;
}
bool PictureReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("pict") == sCommand )
		return true;
	else if( _T("picprop") == sCommand )
	{
		ShapeReader oShapeReader(m_oShape);
		StartSubReader( oShapeReader, oDocument, oReader );
	}
	else if( _T("emfblip") == sCommand )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_emf;
	else if( _T("pngblip") == sCommand )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_png;
	else if( _T("jpegblip") == sCommand )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_png;
	else if( _T("wmetafile") == sCommand )
	{
		if( true == hasParameter && 8 == parameter )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_wmf;
	}
	else if( _T("picw") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nWidth = parameter;
	}
	else if( _T("pich") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nHeight = parameter;
	}
	else if( _T("picwgoal") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nWidthGoal = parameter;
	}	
	else if( _T("pichgoal") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nHeightGoal = parameter;
	}
	else if( _T("picscalex") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_dScaleX = parameter;
	}
	else if( _T("picscaley") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_dScaleY = parameter;
	}
	else if( _T("picscaled") == sCommand )
			m_oShape.m_oPicture->m_bScaled = 1;
	else if( _T("piccropl") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropL = parameter;
	}
	else if( _T("piccropt") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropT = parameter;
	}
	else if( _T("piccropr") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropR = parameter;
	}
	else if( _T("piccropb") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropB = parameter;
	}
	else if( _T("bin") == sCommand)
	{
		if( true == hasParameter )
		{
			//читаем картинку как бинарник длиной parameter
			m_bBin = true;
			m_nBinLength = parameter;
			oReader.m_oLex.ReadBytes( parameter, &m_pbBin );//читаем сразу байты, потому что если между ними и был пробел, то он пропустится в RtfLex::parseKeyword
		}
	}
	else
		return false;
	return true;
}
bool FieldReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("field") == sCommand )
		return true;
	else if( _T("flddirty") == sCommand )
		m_oField.m_eMode = RtfField::fm_flddirty;
	else if( _T("fldedit") == sCommand )
		m_oField.m_eMode = RtfField::fm_fldedit;
	else if( _T("fldlock") == sCommand )
		m_oField.m_eMode = RtfField::fm_fldlock;
	else if( _T("fldpriv") == sCommand )
		m_oField.m_eMode = RtfField::fm_fldpriv;
	else if( _T("fldalt") == sCommand )
		m_oField.m_bReferenceToEndnote = true;

	else if( _T("fldinst") == sCommand )
	{
		ParagraphReader oParagraphReader( _T("fldinst"), oReader );
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oField.m_oInsert = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if( _T("datafield") == sCommand )
		Skip( oDocument, oReader );
	else if( _T("fldrslt") == sCommand )
	{
		ParagraphReader oParagraphReader( _T("fldrslt"), oReader );
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oField.m_oResult = oParagraphReader.m_oParPropDest.m_oTextItems;
		oReader.m_oLex.putString( "}{" );//чтобы не терять после fldrslt
		//{\field{\*\fldinst...}{\*\fldrslt...} ??? }
		//{\field{\*\fldinst...}{\*\fldrslt...}}{ ??? }
	}
	else
		return false;
	return true;
}
bool OleReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("object") == sCommand )
		return true;
	COMMAND_RTF_INT( _T("objw"), m_oOle.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("objh"), m_oOle.m_nHeight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("objemb"), m_oOle.m_eOleType, sCommand, true, RtfOle::ot_emb )
	COMMAND_RTF_INT( _T("objlink"), m_oOle.m_eOleType, sCommand, true, RtfOle::ot_link )
	else if( _T("objclass") == sCommand )
	{
		TextReader oTextReader( m_oOle.m_sOleClass, false );
		StartSubReader( oTextReader, oDocument, oReader );
	}
	else if( _T("objdata") == sCommand )
	{
		CString sOleData;
		TextReader oTextReader( sOleData, false );
		StartSubReader( oTextReader, oDocument, oReader );

		BYTE *pData = NULL;
		long nSize = 0;
		RtfUtility::WriteDataToBinary( sOleData, &pData, nSize );
		if( 0 != nSize  && pData)
		{
			HRESULT hRes = S_FALSE;
			//обекь для конвертации Ole1 в Ole2
			//RtfOle1ToOle2Stream oStream;
			//todooo тут был чисто микрософтовский вариант - переделать !!!
			//oStream.lpstbl = new OLESTREAMVTBL();
			//oStream.lpstbl->Get = &OleGet1;
			//oStream.lpstbl->Put = &OlePut1;
			//oStream.pBuffer = pData;
			//oStream.nBufferSize = nSize;
			//oStream.nCurPos = 0;

			CString sOleStorageName = Utils::CreateTempFile( oReader.m_sTempFolder );
			
			POLE::Storage * piRootStorage = new POLE::Storage(string2std_string(sOleStorageName).c_str());			
			if( piRootStorage)
			{
				//hRes = OleConvertOLESTREAMToIStorage( &oStream, piRootStorage, NULL );
				//m_oOle.SetFilename( sOleStorageName );
				//m_oOle.SetOle( piRootStorage );
				RELEASEOBJECT( piRootStorage );
				hRes = S_OK;
			}

			//delete oStream.lpstbl;
			delete[] pData;
			if( FAILED( hRes ) )
				Utils::RemoveDirOrFile( sOleStorageName );
		}
	}
	else if( _T("result") == sCommand )
	{
		RtfShapePtr oNewShape = RtfShapePtr( new RtfShape() );
		AllPictReader oAllPictReader( *oNewShape );
		StartSubReader( oAllPictReader, oDocument, oReader );
		m_oOle.m_oResultPic = oNewShape;
	}
	else
		return false;
	return true;
}

bool ParagraphPropDestination::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,RtfAbstractReader& oAbstrReader,CString sCommand, bool hasParameter, int parameter)
{
	m_oReader = &oReader;
	m_bPar = false;

	bool bContinue = false;
	//PARAGRAPH Property
	if( _T("pard") == sCommand )
	{
		oReader.m_oState->m_oParagraphProp.SetDefaultRtf();
		oReader.m_oState->m_oCurOldList.SetDefault();
	}
	else if( _T("trowd") == sCommand )
		oReader.m_oState->m_oRowProperty.SetDefaultRtf();
	else if(  _T("tcelld") == sCommand  )
		oReader.m_oState->m_oCellProperty.SetDefaultRtf();
	else if( _T("par") == sCommand )
	{
		m_bPar = true;
		m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, false, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());

	}
	else if( _T("cell") == sCommand  || _T("nestcell") == sCommand )
	{
		m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, true, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	else if( _T("row" == sCommand || "nestrow") == sCommand)
	{
		m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, false, true );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	else if( _T("nesttableprops") == sCommand )
	{
		oReader.m_oState->m_oRowProperty.SetDefaultRtf();
	}
	else if( _T("nonesttables") == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if( _T("listtext") == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if( _T("pntext") == sCommand )
	{
		//пропускаем списки office 95, если есть списки office 2007
		if( oDocument.m_oListTabel.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			ParagraphReader oParagraphReader( _T("pntext"), oReader );
			oAbstrReader.StartSubReader( oParagraphReader, oDocument, oReader );
			if( NULL != oParagraphReader.m_oParPropDest.m_oTextItems && oParagraphReader.m_oParPropDest.m_oTextItems->GetCount() )
			{
				ITextItemPtr piCurContainer;
				oParagraphReader.m_oParPropDest.m_oTextItems->GetItem( piCurContainer, 0 );
				if( NULL != piCurContainer && TYPE_RTF_PARAGRAPH == piCurContainer->GetType() )
					oReader.m_oState->m_oCurOldList.m_oText = boost::shared_static_cast<RtfParagraph, ITextItem>( piCurContainer ) ;
			}
		}
	}
	else if( _T("pn") == sCommand )
	{
		if( oDocument.m_oListTabel.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			RtfOldList oOldList;
			RtfOldListReader oOldListReader( oOldList );
			oAbstrReader.StartSubReader( oOldListReader, oDocument, oReader );

			oReader.m_oState->m_oCurOldList = oOldList;
			bool bExist = false;
			for( int i = 0; i < (int)oDocument.m_aOldLists.size(); i++ )
				if( oReader.m_oState->m_oCurOldList == *oDocument.m_aOldLists[i] )
				{
					bExist = true;
					oReader.m_oState->m_oCurOldList.m_nLs = oDocument.m_aOldLists[i]->m_nLs;
					oReader.m_oState->m_oCurOldList.m_nIlvl = oDocument.m_aOldLists[i]->m_nIlvl;
					break;
				}
			if( false == bExist )
			{
				oReader.m_oState->m_oCurOldList.m_nLs = oDocument.m_oIdGenerator.Generate_PnId();
				RtfOldListPtr oNewOldList = RtfOldListPtr( new RtfOldList() );
				*oNewOldList = oReader.m_oState->m_oCurOldList;
				oDocument.m_aOldLists.push_back( oNewOldList );
			}
		}
	}
	COMMAND_RTF_BOOL( _T("hyphpar"), oReader.m_oState->m_oParagraphProp.m_bAutoHyphenation, sCommand, hasParameter, parameter )
	else if( _T("intbl") == sCommand )
	{
		if( true == hasParameter && 0 == parameter )
		{
			oReader.m_oState->m_oParagraphProp.m_bInTable = 0;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nItap )
				oReader.m_oState->m_oParagraphProp.m_nItap = 0;
		}
		else
		{
			oReader.m_oState->m_oParagraphProp.m_bInTable = 1;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nItap )
				oReader.m_oState->m_oParagraphProp.m_nItap = 1;
		}
	}
	COMMAND_RTF_BOOL( _T("intbl"), oReader.m_oState->m_oParagraphProp.m_bInTable, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("itap"), oReader.m_oState->m_oParagraphProp.m_nItap, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("keep"), oReader.m_oState->m_oParagraphProp.m_bKeep, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("keepn"), oReader.m_oState->m_oParagraphProp.m_bKeepNext, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("pagebb"), oReader.m_oState->m_oParagraphProp.m_bPageBB, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("outlinelevel"), oReader.m_oState->m_oParagraphProp.m_nOutlinelevel, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("s"), oReader.m_oState->m_oParagraphProp.m_nStyle, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("qc"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qc )
	COMMAND_RTF_INT( _T("qj"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qj )
	COMMAND_RTF_INT( _T("ql"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_ql )
	COMMAND_RTF_INT( _T("qr"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qr )
	COMMAND_RTF_INT( _T("qd"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qd )
	else if( _T("qk0") == sCommand )
	{
		switch( parameter )
		{
			case 0:oReader.m_oState->m_oParagraphProp.m_eAlign = RtfParagraphProperty::pa_qk0;break;
			case 10:oReader.m_oState->m_oParagraphProp.m_eAlign = RtfParagraphProperty::pa_qk10;break;
			case 20:oReader.m_oState->m_oParagraphProp.m_eAlign = RtfParagraphProperty::pa_qk20;break;

		}
	}
	COMMAND_RTF_INT( _T("faauto"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_faauto )
	COMMAND_RTF_INT( _T("fahang"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_fahang )
	COMMAND_RTF_INT( _T("facenter"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_facenter )
	COMMAND_RTF_INT( _T("faroman"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_faroman )
	COMMAND_RTF_INT( _T("favar"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_favar )
	COMMAND_RTF_INT( _T("fafixed"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_fafixed )
	COMMAND_RTF_INT( _T("fi"), oReader.m_oState->m_oParagraphProp.m_nIndFirstLine, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("li"), oReader.m_oState->m_oParagraphProp.m_nIndLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("ri"), oReader.m_oState->m_oParagraphProp.m_nIndRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("lin"), oReader.m_oState->m_oParagraphProp.m_nIndStart, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("rin"), oReader.m_oState->m_oParagraphProp.m_nIndEnd, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("adjustright"), oReader.m_oState->m_oParagraphProp.m_bIndRightAuto, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("indmirror"), oReader.m_oState->m_oParagraphProp.m_bIndMirror, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("sb"), oReader.m_oState->m_oParagraphProp.m_nSpaceBefore, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("sa"), oReader.m_oState->m_oParagraphProp.m_nSpaceAfter, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("sbauto"), oReader.m_oState->m_oParagraphProp.m_nSpaceBeforeAuto, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("saauto"), oReader.m_oState->m_oParagraphProp.m_nSpaceAfterAuto, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("lisb"), oReader.m_oState->m_oParagraphProp.m_nSpaceBeforeLine, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("lisa"), oReader.m_oState->m_oParagraphProp.m_nSpaceAfterLine, sCommand, hasParameter, parameter )
	else if( _T("sl") == sCommand )
	{
		if( true == hasParameter )
		{
			oReader.m_oState->m_oParagraphProp.m_nSpaceBetween = parameter;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nSpaceMultiLine )
				oReader.m_oState->m_oParagraphProp.m_nSpaceMultiLine = 0;
		}
	}
	COMMAND_RTF_INT( _T("slmult"), oReader.m_oState->m_oParagraphProp.m_nSpaceMultiLine, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("contextualspace"), oReader.m_oState->m_oParagraphProp.m_bContextualSpacing, sCommand, hasParameter, parameter )
	else if( _T("rtlpar") == sCommand )
		oReader.m_oState->m_oParagraphProp.m_bRtl = 1;
	else if( _T("ltrpar") == sCommand )
		oReader.m_oState->m_oParagraphProp.m_bRtl = 0;
	COMMAND_RTF_BOOL( _T("nowwrap"), oReader.m_oState->m_oParagraphProp.m_bNoWordWrap, sCommand, hasParameter, parameter )
	else if( _T("nowwrap") == sCommand )
	{
		if( true == hasParameter && 0 == parameter)
			oReader.m_oState->m_oParagraphProp.m_bSnapToGrid = 1;
		else
			oReader.m_oState->m_oParagraphProp.m_bSnapToGrid = 0;
	}
	else if( _T("ls") == sCommand )
	{
		if( true == hasParameter )
		{
			oReader.m_oState->m_oParagraphProp.m_nListId = parameter;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nListLevel )
				oReader.m_oState->m_oParagraphProp.m_nListLevel = 0;
		}
	}
	COMMAND_RTF_INT( _T("ilvl"), oReader.m_oState->m_oParagraphProp.m_nListLevel, sCommand, hasParameter, parameter )

	else if( _T("chbrdr") == sCommand )
		m_eInternalState = is_charBorder;
	else if( _T("brdrt") == sCommand )
		m_eInternalState = is_borderTop;
	else if( _T("brdrb") == sCommand )
		m_eInternalState = is_borderBottom;
	else if( _T("brdrl") == sCommand )
		m_eInternalState = is_borderLeft;
	else if( _T("brdrr") == sCommand )
		m_eInternalState = is_borderRight;
	//else if( _T("brdrbtw") == sCommand )
	//	m_eInternalState = is_borderRight;
	else if( _T("brdrbar") == sCommand )
		m_eInternalState = is_borderBar;
	else if( _T("box") == sCommand )
		m_eInternalState = is_borderBox;
	//Frame
	COMMAND_RTF_INT( _T("absw"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("absh"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHeight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("phmrg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phmrg )
	COMMAND_RTF_INT( _T("phpg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phpg )
	COMMAND_RTF_INT( _T("phcol"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phcol )
	COMMAND_RTF_INT( _T("posx"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posnegx"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posxc"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxc )
	COMMAND_RTF_INT( _T("posxi"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxi )
	COMMAND_RTF_INT( _T("posxo"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxo )
	COMMAND_RTF_INT( _T("posxl"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxl )
	COMMAND_RTF_INT( _T("posxr"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxr )
	COMMAND_RTF_INT( _T("pvmrg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvmrg )
	COMMAND_RTF_INT( _T("pvpg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvpg )
	COMMAND_RTF_INT( _T("pvpara"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvpara )
	COMMAND_RTF_INT( _T("posy"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posnegy"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posyt"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyt )
	COMMAND_RTF_INT( _T("posyil"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyil )
	COMMAND_RTF_INT( _T("posyb"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyb )
	COMMAND_RTF_INT( _T("posyc"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyc )
	COMMAND_RTF_INT( _T("posyin"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyin )
	COMMAND_RTF_INT( _T("posyout"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyout )
	COMMAND_RTF_BOOL( _T("abslock"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_bLockAnchor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("wrapdefault"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wrapdefault )
	COMMAND_RTF_INT( _T("wraparound"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wraparound )
	COMMAND_RTF_INT( _T("wraptight"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wraptight )
	COMMAND_RTF_INT( _T("wrapthrough"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wrapthrough )
	COMMAND_RTF_INT( _T("dropcapt"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_DropcapType, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dropcapli"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_DropcapLines, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dxfrtext"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nAllSpace, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dfrmtxtx"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHorSpace, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dfrmtxty"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nVerSpace, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("absnoovrlp"), oReader.m_oState->m_oParagraphProp.m_bOverlap, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("frmtxlrtb"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxlrtb )
	COMMAND_RTF_INT( _T("frmtxtbrl"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxtbrl )
	COMMAND_RTF_INT( _T("frmtxbtlr"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxbtlr )
	COMMAND_RTF_INT( _T("frmtxlrtbv"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxlrtbv )
	COMMAND_RTF_INT( _T("frmtxtbrlv"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxtbrlv )
	//RtfTab
	
	COMMAND_RTF_INT( _T("tldot"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_dot )
	COMMAND_RTF_INT( _T("tlmdot"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_mdot )
	COMMAND_RTF_INT( _T("tlhyph"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_hyph )
	COMMAND_RTF_INT( _T("tlul"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_ul )
	COMMAND_RTF_INT( _T("tlth"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_ul )
	COMMAND_RTF_INT( _T("tqr"), m_oCurTab.m_eKind , sCommand, true, RtfTab::tk_tqr )
	COMMAND_RTF_INT( _T("tqc"), m_oCurTab.m_eKind , sCommand, true, RtfTab::tk_tqc )
	COMMAND_RTF_INT( _T("tqdec"), m_oCurTab.m_eKind , sCommand, true, RtfTab::tk_tqdec )
	else if( _T("tb") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oCurTab.m_eKind = RtfTab::tk_tqbar;
			m_oCurTab.m_nTab = parameter;
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else if( _T("tx") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oCurTab.m_nTab = parameter;
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//tableStyleProp
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_INT( _T("yts"), oReader.m_oState->m_oParagraphProp.m_nTableStyle, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstrow"), oReader.m_oState->m_oParagraphProp.m_bStyleFirstRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstrow"), oReader.m_oState->m_oCellProperty.m_bStyleFirstRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstrow"), oReader.m_oState->m_oRowProperty.m_bStyleFirstRow, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tsclastrow"), oReader.m_oState->m_oParagraphProp.m_bStyleLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastrow"), oReader.m_oState->m_oCellProperty.m_bStyleLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastrow"), oReader.m_oState->m_oRowProperty.m_bStyleLastRow, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscfirstcol"), oReader.m_oState->m_oParagraphProp.m_bStyleFirstCollumn, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstcol"), oReader.m_oState->m_oCellProperty.m_bStyleFirstCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstcol"), oReader.m_oState->m_oRowProperty.m_bStyleFirstCol, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tsclastcol"), oReader.m_oState->m_oParagraphProp.m_bStyleLastCollumn, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastcol"), oReader.m_oState->m_oCellProperty.m_bStyleLastCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastcol"), oReader.m_oState->m_oRowProperty.m_bStyleLastCol, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandhorzodd"), oReader.m_oState->m_oParagraphProp.m_bStyleOddRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzodd"), oReader.m_oState->m_oCellProperty.m_bStyleOddRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzodd"), oReader.m_oState->m_oRowProperty.m_bStyleOddRowBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandhorzeven"), oReader.m_oState->m_oParagraphProp.m_bStyleEvenRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzeven"), oReader.m_oState->m_oCellProperty.m_bStyleEvenRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzeven"), oReader.m_oState->m_oRowProperty.m_bStyleEvenRowBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandvertodd"), oReader.m_oState->m_oParagraphProp.m_bStyleOddColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandvertodd"), oReader.m_oState->m_oCellProperty.m_bStyleOddColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandvertodd"), oReader.m_oState->m_oRowProperty.m_bStyleOddColBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandverteven"), oReader.m_oState->m_oParagraphProp.m_bStyleEvenColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandverteven"), oReader.m_oState->m_oCellProperty.m_bStyleEvenColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandverteven"), oReader.m_oState->m_oRowProperty.m_bStyleEvenColBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscnwcell"), oReader.m_oState->m_oParagraphProp.m_bStyleNWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnwcell"), oReader.m_oState->m_oCellProperty.m_bStyleNWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnwcell"), oReader.m_oState->m_oRowProperty.m_bStyleNWCell, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscnecell"), oReader.m_oState->m_oParagraphProp.m_bStyleNECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnecell"), oReader.m_oState->m_oCellProperty.m_bStyleNECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnecell"), oReader.m_oState->m_oRowProperty.m_bStyleNECell, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscswcell"), oReader.m_oState->m_oParagraphProp.m_bStyleSWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscswcell"), oReader.m_oState->m_oCellProperty.m_bStyleSWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscswcell"), oReader.m_oState->m_oRowProperty.m_bStyleSWCell, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscsecell"), oReader.m_oState->m_oParagraphProp.m_bStyleSECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscsecell"), oReader.m_oState->m_oCellProperty.m_bStyleSECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscsecell"), oReader.m_oState->m_oRowProperty.m_bStyleSECell, sCommand, hasParameter, parameter )

	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//CELL Property
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_BOOL( _T("clmgf"), oReader.m_oState->m_oCellProperty.m_bMergeFirst, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clmrg"), oReader.m_oState->m_oCellProperty.m_bMerge, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clvmgf"), oReader.m_oState->m_oCellProperty.m_bMergeFirstVertical, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clvmrg"), oReader.m_oState->m_oCellProperty.m_bMergeVertical, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clFitText"), oReader.m_oState->m_oCellProperty.m_bFitText, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clNoWrap"), oReader.m_oState->m_oCellProperty.m_bNoWrap, sCommand, hasParameter, parameter )		

	COMMAND_RTF_INT( _T("clpadfl"), oReader.m_oState->m_oCellProperty.m_nIsPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadl"), oReader.m_oState->m_oCellProperty.m_nPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadft"), oReader.m_oState->m_oCellProperty.m_nIsPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadt"), oReader.m_oState->m_oCellProperty.m_nPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadfr"), oReader.m_oState->m_oCellProperty.m_nIsPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadr"), oReader.m_oState->m_oCellProperty.m_nPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadfb"), oReader.m_oState->m_oCellProperty.m_nIsPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadb"), oReader.m_oState->m_oCellProperty.m_nPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspfl"), oReader.m_oState->m_oCellProperty.m_nIsSpacingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspl"), oReader.m_oState->m_oCellProperty.m_nSpacingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspft"), oReader.m_oState->m_oCellProperty.m_nIsSpacingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspt"), oReader.m_oState->m_oCellProperty.m_nSpacingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspfr"), oReader.m_oState->m_oCellProperty.m_nIsSpacingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspr"), oReader.m_oState->m_oCellProperty.m_nSpacingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspfb"), oReader.m_oState->m_oCellProperty.m_nIsSpacingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspb"), oReader.m_oState->m_oCellProperty.m_nSpacingBottom, sCommand, hasParameter, parameter )
	else if( _T("clftsWidth") == sCommand  )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
				case 0:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_none;break;
				case 1:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_Auto;break;
				case 2:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_Percent;break;
				case 3:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("clwWidth"), oReader.m_oState->m_oCellProperty.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clhidemark"), oReader.m_oState->m_oCellProperty.m_bHideMark, sCommand, hasParameter, parameter )
	else if( _T("cldglu") == sCommand )
		m_eInternalState = is_borderCellLR;
	else if( _T("cldgll") == sCommand )
		m_eInternalState = is_borderCellRL;
	else if( _T("clbrdrl") == sCommand )
		m_eInternalState = is_borderCellLeft;
	else if( _T("clbrdrt") == sCommand )
		m_eInternalState = is_borderCellTop;
	else if( _T("clbrdrr") == sCommand )
		m_eInternalState = is_borderCellRight;
	else if( _T("clbrdrb") == sCommand )
		m_eInternalState = is_borderCellBottom;

	//table Style
	else if( _T("tsbrdrdgl") == sCommand )
		m_eInternalState = is_borderCellLR;
	else if( _T("tsbrdrdgr") == sCommand )
		m_eInternalState = is_borderCellRL;

	COMMAND_RTF_INT( _T("clvertalt"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Top )
	COMMAND_RTF_INT( _T("clvertalc"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Center )
	COMMAND_RTF_INT( _T("clvertalb"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Bottom )
	COMMAND_RTF_INT( _T("cltxlrtb"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_lrtb )
	COMMAND_RTF_INT( _T("cltxtbrl"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_tbrl )
	COMMAND_RTF_INT( _T("cltxbtlr"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_btlr )
	COMMAND_RTF_INT( _T("cltxlrtbv"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_lrtbv )
	COMMAND_RTF_INT( _T("cltxtbrlv"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_tbrlv )
	else if( _T("cellx") == sCommand )
	{
		if( true == hasParameter)
		{
			oReader.m_oState->m_oCellProperty.m_nCellx = parameter;
			oReader.m_oState->m_oRowProperty.AddItem( oReader.m_oState->m_oCellProperty );
			oReader.m_oState->m_oCellProperty.SetDefaultRtf();
		}
	}
	//table style
	COMMAND_RTF_INT( _T("tscellpaddfl"), oReader.m_oState->m_oCellProperty.m_nIsPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddl"), oReader.m_oState->m_oCellProperty.m_nPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddft"), oReader.m_oState->m_oCellProperty.m_nIsPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddt"), oReader.m_oState->m_oCellProperty.m_nPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddfr"), oReader.m_oState->m_oCellProperty.m_nIsPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddr"), oReader.m_oState->m_oCellProperty.m_nPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddfb"), oReader.m_oState->m_oCellProperty.m_nIsPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddb"), oReader.m_oState->m_oCellProperty.m_nPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsnowrap"), oReader.m_oState->m_oCellProperty.m_bNoWrap, sCommand, hasParameter, parameter )		
	COMMAND_RTF_INT( _T("tsvertalt"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Top )
	COMMAND_RTF_INT( _T("tsvertalc"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Center )
	COMMAND_RTF_INT( _T("tsvertalb"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Bottom )

	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//ROW Property
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_INT( _T("irow"), oReader.m_oState->m_oRowProperty.m_nIndex, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("irowband"), oReader.m_oState->m_oRowProperty.m_nBandIndex, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("lastrow"), oReader.m_oState->m_oRowProperty.m_bLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("trhdr"), oReader.m_oState->m_oRowProperty.m_bIsHeader, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("trkeep"), oReader.m_oState->m_oRowProperty.m_bKeep, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("trkeepfollow"), oReader.m_oState->m_oRowProperty.m_bKeep, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("trql"), oReader.m_oState->m_oRowProperty.m_eJust, sCommand, true, RtfRowProperty::rj_trql )
	COMMAND_RTF_INT( _T("trqr"), oReader.m_oState->m_oRowProperty.m_eJust, sCommand, true, RtfRowProperty::rj_trqr )
	COMMAND_RTF_INT( _T("trqc"), oReader.m_oState->m_oRowProperty.m_eJust, sCommand, true, RtfRowProperty::rj_trqc )

	COMMAND_RTF_INT( _T("trrh"), oReader.m_oState->m_oRowProperty.m_nHeight, sCommand, hasParameter, parameter )

	else if( _T("trftsWidth") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_none;break;
			case 1:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Auto;break;
			case 2:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Percent;break;
			case 3:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidth"), oReader.m_oState->m_oRowProperty.m_nWidth, sCommand, hasParameter, parameter )
	else if( _T("trftsWidthB") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_none;break;
			case 1:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_Auto;break;
			case 2:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_Percent;break;
			case 3:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidthB"), oReader.m_oState->m_oRowProperty.m_nWidthStartInvCell, sCommand, hasParameter, parameter )
	else if( _T("trftsWidthA") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_none;break;
			case 1:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_Auto;break;
			case 2:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_Percent;break;
			case 3:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidthA"), oReader.m_oState->m_oRowProperty.m_nWidthEndInvCell, sCommand, hasParameter, parameter )
	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//TABLE property
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_BOOL( _T("taprtl"), oReader.m_oState->m_oRowProperty.m_bBidi, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trautofit"), oReader.m_oState->m_oRowProperty.m_nAutoFit, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trgaph"), oReader.m_oState->m_oRowProperty.m_nGraph, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tblind"), oReader.m_oState->m_oRowProperty.nTableIndent, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tblindtype"), oReader.m_oState->m_oRowProperty.nTableIndentUnits, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tdfrmtxtLeft"), oReader.m_oState->m_oRowProperty.m_nWrapLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tdfrmtxtRight"), oReader.m_oState->m_oRowProperty.m_nWrapRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tdfrmtxtTop"), oReader.m_oState->m_oRowProperty.m_nWrapTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tdfrmtxtBottom"), oReader.m_oState->m_oRowProperty.m_nWrapBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tabsnoovrlp"), oReader.m_oState->m_oRowProperty.m_bOverlap, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tphmrg"), oReader.m_oState->m_oRowProperty.m_eHRef, sCommand, true, RtfTableProperty::hr_phmrg )
	COMMAND_RTF_INT( _T("tphpg"), oReader.m_oState->m_oRowProperty.m_eHRef, sCommand, true, RtfTableProperty::hr_phpg )
	COMMAND_RTF_INT( _T("tphcol"), oReader.m_oState->m_oRowProperty.m_eHRef, sCommand, true, RtfTableProperty::hr_phcol )
	COMMAND_RTF_INT( _T("tposx"), oReader.m_oState->m_oRowProperty.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposnegx"), oReader.m_oState->m_oRowProperty.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposxc"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxc )
	COMMAND_RTF_INT( _T("tposxi"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxi )
	COMMAND_RTF_INT( _T("tposxo"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxo )
	COMMAND_RTF_INT( _T("tposxl"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxl )
	COMMAND_RTF_INT( _T("tposxr"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxr )

	COMMAND_RTF_INT( _T("tpvmrg"), oReader.m_oState->m_oRowProperty.m_eVRef, sCommand, true, RtfTableProperty::vr_pvmrg )
	COMMAND_RTF_INT( _T("tpvpg"), oReader.m_oState->m_oRowProperty.m_eVRef, sCommand, true, RtfTableProperty::vr_pvpg )
	COMMAND_RTF_INT( _T("tpvpara"), oReader.m_oState->m_oRowProperty.m_eVRef, sCommand, true, RtfTableProperty::vr_pvpara )
	COMMAND_RTF_INT( _T("tposy"), oReader.m_oState->m_oRowProperty.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposnegy"), oReader.m_oState->m_oRowProperty.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposyt"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyt )
	COMMAND_RTF_INT( _T("tposyil"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyil )
	COMMAND_RTF_INT( _T("tposyb"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyb )
	COMMAND_RTF_INT( _T("tposyc"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyc )
	COMMAND_RTF_INT( _T("tposyin"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyin )
	COMMAND_RTF_INT( _T("tposyout"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyout )

	else if( _T("trleft") == sCommand )
	{
		if( true == hasParameter )
		{
			oReader.m_oState->m_oRowProperty.m_nLeft = parameter;
			if( PROP_DEF == oReader.m_oState->m_oRowProperty.nTableIndent )
			{
				oReader.m_oState->m_oRowProperty.nTableIndent = parameter;
				oReader.m_oState->m_oRowProperty.nTableIndentUnits = 3;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidth"), oReader.m_oState->m_oRowProperty.m_nWidth, sCommand, hasParameter, parameter )
	else if( _T("trleft") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_none;break;
			case 1: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Auto;break;
			case 2: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Percent;break;
			case 3: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trpaddb"), oReader.m_oState->m_oRowProperty.m_nDefCellMarBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddl"), oReader.m_oState->m_oRowProperty.m_nDefCellMarLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddr"), oReader.m_oState->m_oRowProperty.m_nDefCellMarRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddt"), oReader.m_oState->m_oRowProperty.m_nDefCellMarTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddfb"), oReader.m_oState->m_oRowProperty.m_nDefCellMarBottomUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddfl"), oReader.m_oState->m_oRowProperty.m_nDefCellMarLeftUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddfr"), oReader.m_oState->m_oRowProperty.m_nDefCellMarRightUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddft"), oReader.m_oState->m_oRowProperty.m_nDefCellMarTopUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdb"), oReader.m_oState->m_oRowProperty.m_nDefCellSpBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdl"), oReader.m_oState->m_oRowProperty.m_nDefCellSpLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdr"), oReader.m_oState->m_oRowProperty.m_nDefCellSpRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdt"), oReader.m_oState->m_oRowProperty.m_nDefCellSpTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdfb"), oReader.m_oState->m_oRowProperty.m_nDefCellSpBottomUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdfl"), oReader.m_oState->m_oRowProperty.m_nDefCellSpLeftUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdfr"), oReader.m_oState->m_oRowProperty.m_nDefCellSpRightUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdft"), oReader.m_oState->m_oRowProperty.m_nDefCellSpTopUnits, sCommand, hasParameter, parameter )

	else if( _T("trbrdrl") == sCommand  )
		m_eInternalState = is_borderRowLeft;
	else if( _T("trbrdrr") == sCommand  )
		m_eInternalState = is_borderRowRight;
	else if( _T("trbrdrt") == sCommand  )
		m_eInternalState = is_borderRowTop;
	else if( _T("trbrdrb") == sCommand  )
		m_eInternalState = is_borderRowBottom;
	else if( _T("trbrdrv") == sCommand  )
		m_eInternalState = is_borderRowVer;
	else if( _T("trbrdrh") == sCommand  )
		m_eInternalState = is_borderRowHor;
	//table Style
	else if( _T("tsbrdrh") == sCommand ) //диагональные из свойств cell
		m_eInternalState = is_borderRowHor;
	else if( _T("tsbrdrv") == sCommand )
		m_eInternalState = is_borderRowVer;
	else if( _T("tsbrdrl") == sCommand )
		m_eInternalState = is_borderRowLeft;
	else if( _T("tsbrdrt") == sCommand )
		m_eInternalState = is_borderRowTop;
	else if( _T("tsbrdrr") == sCommand )
		m_eInternalState = is_borderRowRight;
	else if( _T("tsbrdrb") == sCommand )
		m_eInternalState = is_borderRowBottom;

	COMMAND_RTF_INT( _T("ts"), oReader.m_oState->m_oRowProperty.m_nStyle, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tbllkhdrrows"), oReader.m_oState->m_oRowProperty.m_bAutoFirstRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllklastrow"), oReader.m_oState->m_oRowProperty.m_bAutoLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllkhdrcols"), oReader.m_oState->m_oRowProperty.m_bAutoFirstCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllklastcol"), oReader.m_oState->m_oRowProperty.m_bAutoLastCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllknorowband"), oReader.m_oState->m_oRowProperty.m_bAutoNoRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllknocolband"), oReader.m_oState->m_oRowProperty.m_bAutoNoColBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tscbandsh"), oReader.m_oState->m_oRowProperty.m_nRowBandSize, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscbandsv"), oReader.m_oState->m_oRowProperty.m_nColBandSize, sCommand, hasParameter, parameter )

	//Math
	else if( _T("mmath") == sCommand )
	{
		RtfMathPtr oNewMath = RtfMathPtr( new RtfMath() );
		RtfMathReader oMathReader( *oNewMath );
		oAbstrReader.StartSubReader( oMathReader, oDocument, oReader );
		m_oCurParagraph->AddItem( oNewMath );
	}
	else if( _T("shp") == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		ShapeReader oShapeReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("shppict") == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		ShppictReader oShppictReader( *oNewShape );
		oAbstrReader.StartSubReader( oShppictReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("pict") == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		oNewShape->m_nShapeType = 75;
		oNewShape->m_nWrapType = 3; // none
		oNewShape->m_nPositionHRelative = 3;//TCHAR
		oNewShape->m_nPositionVRelative = 3;//line
		oNewShape->m_nPositionH = 0;//absolute
		oNewShape->m_nPositionV = 0;//absolute
		oNewShape->m_oPicture = RtfPicturePtr( new RtfPicture() );
		PictureReader oPictureReader( oReader, *oNewShape);
		oAbstrReader.StartSubReader( oPictureReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("shpgrp") == sCommand )
	{
		RtfShapeGroupPtr oNewShape( new RtfShapeGroup() );
		ShapeGroupReader oShapeGroupReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeGroupReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("nonshppict") == sCommand )
		oAbstrReader.Skip( oDocument, oReader );
	else if( _T("field") == sCommand )
	{
		RtfFieldPtr oNewField = RtfFieldPtr(new RtfField());
		FieldReader oFieldReader( *oNewField );
		oAbstrReader.StartSubReader( oFieldReader, oDocument, oReader );
		if( true == oNewField->IsValid() )
			m_oCurParagraph->AddItem( oNewField );
	}
	else if( _T("bkmkstart") == sCommand )
	{
		RtfBookmarkStartPtr oNewBookmarkStart = RtfBookmarkStartPtr( new RtfBookmarkStart() );
		BookmarkStartReader oBookmarkStartReader( *oNewBookmarkStart );
		oAbstrReader.StartSubReader( oBookmarkStartReader, oDocument, oReader );
		if( true == oNewBookmarkStart->IsValid() )
			m_oCurParagraph->AddItem( oNewBookmarkStart );
	}
	else if( _T("bkmkend") == sCommand )
	{
		RtfBookmarkEndPtr oNewBookmarkEnd = RtfBookmarkEndPtr( new RtfBookmarkEnd() );
		BookmarkEndReader oBookmarkEndReader( *oNewBookmarkEnd );
		oAbstrReader.StartSubReader( oBookmarkEndReader, oDocument, oReader );
		if( true == oNewBookmarkEnd->IsValid() )
			m_oCurParagraph->AddItem( oNewBookmarkEnd );
	}
	else if( _T("footnote") == sCommand )
	{
		RtfFootnotePtr oNewFootnote = RtfFootnotePtr( new RtfFootnote() );
		oNewFootnote->m_oCharProp = oReader.m_oState->m_oCharProp;
		FootnoteReader oFootnoteReader( *oNewFootnote );
		oAbstrReader.StartSubReader( oFootnoteReader, oDocument, oReader );
		if( true == oNewFootnote->IsValid() )
			m_oCurParagraph->AddItem( oNewFootnote );
	}
	else if( _T("chftn") == sCommand )
	{
		if( 1 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			oNewChar->m_eType = RtfCharSpecial::rsc_chftn;
			m_oCurParagraph->AddItem( oNewChar );
		}
		else if( 2 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			oNewChar->m_eType = RtfCharSpecial::rsc_chftnEnd;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	else if( _T("chftnsep") == sCommand || _T("chftnsepc") == sCommand )
	{
		RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->AddItem( oNewChar );
		if( _T("chftnsep") == sCommand )
			oNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
		else if( _T("chftnsepc") == sCommand )
			oNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
	}//specialChars
	else if( _T("page") == sCommand  )
	{
		RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
		oNewChar->m_eType = RtfCharSpecial::rsc_page;
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->AddItem( oNewChar );

		m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, false, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	COMMAND_RTF_SPECIAL_CHAR( _T("column"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_column )
	COMMAND_RTF_SPECIAL_CHAR( _T("line"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_line )
	COMMAND_RTF_SPECIAL_CHAR( _T("line"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_line )
	else if( _T("lbr") == sCommand )
	{
		if( true == hasParameter )
		{
			RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_nTextWrapBreak = parameter;
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( _T("softpage"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softpage )
	COMMAND_RTF_SPECIAL_CHAR( _T("softcol"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softcol )
	COMMAND_RTF_SPECIAL_CHAR( _T("softline"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softline )
	else if( _T("softlheight") == sCommand )
	{
		if( true == hasParameter )
		{
			RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_nSoftHeight = parameter;
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( _T("tab"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_tab )
	else if( _T("emdash") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 151, 0xD0 ); //\bullet Word for Windows - 151	; Apple Macintosh - 0xD0
	else if( _T("endash") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 150, 0xD1 ); //\bullet Word for Windows - 150	; Apple Macintosh - 0xD1
	COMMAND_RTF_SPECIAL_CHAR( _T("emspace"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_emspace )
	COMMAND_RTF_SPECIAL_CHAR( _T("enspace"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_enspace )
	COMMAND_RTF_SPECIAL_CHAR( _T("qmspace"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_qmspace )
	else if( _T("bullet") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 149, 0xA5 ); //\bullet Word for Windows - 149	; Apple Macintosh - 0xA5
	else if( _T("lquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 145, 0xD4 ); //\bullet Word for Windows - 145	; Apple Macintosh - 0xD4
	else if( _T("rquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 146, 0xD5 ); //\bullet Word for Windows - 146	; Apple Macintosh - 0xD5
	else if( _T("ldblquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 147, 0xD2 ); //\bullet Word for Windows - 147	; Apple Macintosh - 0xD2
	else if( _T("rdblquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 148, 0xD3 ); //\bullet Word for Windows - 148	; Apple Macintosh - 0xD3
	COMMAND_RTF_SPECIAL_CHAR( _T("|"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_Formula )
	COMMAND_RTF_SPECIAL_CHAR( _T("~"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrSpace )
	COMMAND_RTF_SPECIAL_CHAR( _T("-"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_OptHyphen )
	COMMAND_RTF_SPECIAL_CHAR( _T("_"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrHyphen )
	COMMAND_RTF_SPECIAL_CHAR( _T(":"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_SubEntry )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwbo"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwbo )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwnbo"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnbo )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwj"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwj )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwnj"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnj )
	else if( _T("object") == sCommand )
	{
		RtfOlePtr oNewOleObject = RtfOlePtr( new RtfOle() );
		OleReader oOleReader( *oNewOleObject );
		oAbstrReader.StartSubReader( oOleReader, oDocument, oReader );
		if( true == oNewOleObject->IsValid() )
			m_oCurParagraph->AddItem( oNewOleObject );
	}
	else
	{
		bool bResult = false;


		bResult = RtfShadingCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oShading );
		if( true == bResult )
			return true;

		if( is_borderBar == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBar );
		else if( is_borderBottom == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBottom );
		else if( is_borderBox == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBox );
		else if( is_borderLeft == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderLeft );
		else if( is_borderRight == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderRight );
		else if( is_borderTop == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderTop );

		if( true == bResult )
			return true;

		if( is_borderCellBottom == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderBottom );
		else if( is_borderCellLeft == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,oReader.m_oState->m_oCellProperty.m_oBorderLeft );
		else if( is_borderCellRight == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderRight );
		else if( is_borderCellTop == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderTop );
		else if( is_borderCellLR == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderDiagonalLR );
		else if( is_borderCellRL == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderDiagonalRL );

		if( true == bResult )
			return true;

		bResult = RtfShadingCellCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oShading );
		if( true == bResult )
			return true;

		if( is_borderRowBottom == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderBottom );
		else if( is_borderRowHor == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,oReader.m_oState->m_oRowProperty.m_oBorderHor );
		else if( is_borderRowLeft == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderLeft );
		else if( is_borderRowRight == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderRight );
		else if( is_borderRowTop == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderTop );
		else if( is_borderRowVer == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderVert );

		if( true == bResult )
			return true;

		bResult = RtfShadingRowCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oShading );
		if( true == bResult )
			return true;

		if( is_charBorder == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,  oReader.m_oState->m_oCharProp.m_poBorder);

		if( true == bResult )
			return true;

		bResult = RtfCharPropCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
		if( true == bResult )
			return true;

		return false;
	}
	return true;
}

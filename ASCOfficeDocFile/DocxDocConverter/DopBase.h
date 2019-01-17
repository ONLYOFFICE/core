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

#include "Constants.h"
#include "Copts60.h"
#include "DTTM.h"

namespace ASCDocFileFormat
{
  class DopBase: public IOperand
  {
    private:
	  bool fFacingPages;
	  bool fPMHMainDoc;
	  Constants::fpc fpc;
	  Constants::rncFtn rncFtn;
	  short nFtn;
	  bool fSplAllDone;
	  bool fSplAllClean;
	  bool fSplHideErrors;
	  bool fGramHideErrors;
	  bool fLabelDoc;
	  bool fHyphCapitals;
	  bool fAutoHyphen;
	  bool fFormNoFields;
	  bool fLinkStyles;
	  bool fRevMarking;
	  bool fExactCWords;
	  bool fPagHidden;
	  bool fPagResults;
	  bool fLockAtn;
	  bool fMirrorMargins;
	  bool fWord97Compat;
	  bool fProtEnabled;
	  bool fDispFormFldSel;
	  bool fRMView;
	  bool fRMPrint;
	  bool fLockVbaProj;
	  bool fLockRev;
	  bool fEmbedFonts;
	  Copts60 copts60;
	  short dxaTab;
	  short cpgWebOpt;
	  short dxaHotZ;
	  short cConsecHypLim;
	  DTTM dttmCreated;
	  DTTM dttmRevised;
	  DTTM dttmLastPrint;
	  short nRevision;
	  int tmEdited;
	  int cWords;
	  int cCh;
	  short cPg;
	  int cParas;
	  Constants::rncEdn rncEdn;
	  short nEdn;
	  Constants::epc epc;
	  bool fPrintFormData;
	  bool fSaveFormData;
	  bool fShadeFormData;
	  bool fShadeMergeFields;
	  bool fIncludeSubdocsInStats;
	  int cLines;
	  int cWordsWithSubdocs;
	  int cChWithSubdocs;
	  short cPgWithSubdocs;
	  int cParasWithSubdocs;
	  int cLinesWithSubdocs;
	  int lKeyProtDoc;
	  Constants::ViewingMode wvkoSaved;
	  unsigned short pctWwdSaved;
	  Constants::ZoomType zkSaved;
	  bool iGutterPos;

	  static const unsigned char SIZE_IN_BYTES = 84;
	  mutable unsigned char bytes[SIZE_IN_BYTES];

	  void SetBytes() const
	  {
	    BitSet bitSet( SIZE_IN_BYTES );
		unsigned int bitOffset = 0;

		bitSet.SetBit( this->fFacingPages, 0 );
		bitSet.SetBit( this->fPMHMainDoc, 2 );
		bitSet.SetBits<Constants::fpc>( this->fpc, 5, 2 );

		memset( this->bytes, 0, SIZE_IN_BYTES );
		memcpy( this->bytes, bitSet.GetBytes(), bitSet.GetSize() );
	  }

    public:
	  DopBase()
	  {
	    this->SetDefaults();
	  }

	  void SetDefaults()
	  {
	  }

	  virtual ~DopBase()
	  {
	  }

	  void SetFacingPages( bool _fFacingPages )
	  {
	    this->fFacingPages = _fFacingPages;
	  }

	  void SetMailMergeMainDocument( bool _fPMHMainDoc )
	  {
	    this->fPMHMainDoc = _fPMHMainDoc; 
	  }

	  void SetFootnotesPlace( Constants::fpc _fpc )
	  {
	    this->fpc = _fpc;
	  }

	  void SetFootnotesNumbering( Constants::rncFtn _rncFtn )
	  {
	    this->rncFtn = _rncFtn;
	  }

	  void SetFootnotesStartingNumber( short _nFtn )
	  {
	    this->nFtn = _nFtn;
	  }

	  void SetSpellCheckingDone( bool _fSplAllDone )
	  {
	    this->fSplAllDone = _fSplAllDone;
	  }

	  void SetSpellingCorrectly( bool _fSplAllClean )
	  {
	    this->fSplAllClean = _fSplAllClean;
	  }

	  void SetSpellingHideError( bool _fSplHideErrors )
	  {
	    this->fSplHideErrors = _fSplHideErrors;
	  }

	  void SetGrammarHideError( bool _fGramHideErrors )
	  {
	    this->fGramHideErrors = _fGramHideErrors;
	  }

	  void SetMailMergeLabelsDocument( bool _fLabelDoc )
	  {
	    this->fLabelDoc = _fLabelDoc;
	  }

      void SetCapitalLettersHyphenated( bool _fHyphCapitals )
	  {
	    this->fHyphCapitals = _fHyphCapitals;
	  }

	  void SetAutoHyphen( bool _fAutoHyphen )
	  {
	    this->fAutoHyphen = _fAutoHyphen;
	  }

	  void SetNoEditableRegions( bool _fFormNoFields )
	  {
	    this->fFormNoFields = _fFormNoFields;
	  }

	  void SetLinkStyles( bool _fLinkStyles )
	  {
	    this->fLinkStyles = _fLinkStyles;
	  }

      void SetRevisionMarking( bool _fRevMarking )
	  {
	    this->fRevMarking = _fRevMarking;
	  }

	  void SetExactCWords( bool _fExactCWords )
	  {
	    this->fExactCWords = _fExactCWords;
	  }

      void SetPagHidden( bool _fPagHidden )
	  {
	    this->fPagHidden = _fPagHidden;
	  }

	  void SetPagResults( bool _fPagResults )
	  {
	    this->fPagResults = _fPagResults;
	  }

	  void SetLockComments( bool _fLockAtn )
	  {
        this->fLockAtn = _fLockAtn;
	  }

	  void SetMirrorMargins( bool _fMirrorMargins )
	  {
	    this->fMirrorMargins = _fMirrorMargins;
	  }

	  void SetWord97CompatibilityMode( bool _fWord97Compat )
	  {
	    this->fWord97Compat = _fWord97Compat;
	  }

	  void SetProtectionEnabled( bool _fProtEnabled )
	  {
	    this->fProtEnabled = _fProtEnabled;
	  }

      void SetDisplayFormFieldSelection( bool _fDispFormFldSel )
	  {
	    this->fDispFormFldSel = _fDispFormFldSel;
	  }

	  void SetShowAnyRevisionMarkup( bool _fRMView )
	  {
	    this->fRMView = _fRMView;
	  }

	  void SetPrintAnyRevisionMarkup( bool _fRMPrint )
	  {
	    this->fRMPrint = _fRMPrint;
	  }

	  void SetLockVBAProject( bool _fLockVbaProj )
	  {
	    this->fLockVbaProj = _fLockVbaProj;
	  }

      void SetLockRevisions( bool _fLockRev )
	  {
	    this->fLockRev = _fLockRev;
	  }

	  void SetTrueTypeFontsEmbedded( bool _fEmbedFonts )
	  {
	    this->fEmbedFonts = _fEmbedFonts;
	  }

      void SetCompatibilityOptions( Copts60 _copts60 )
	  {
	    this->copts60 = _copts60;
	  }

	  void SetTheDefaultTabStopIntervalInTwips( short _dxaTab )
	  {
	    this->dxaTab = _dxaTab;
	  }

      void SetHTMLSavingCodePage( short _cpgWebOpt )
	  {
	    this->cpgWebOpt = _cpgWebOpt;
	  }

	  void SetMaximumAmountOfWhiteSpaceInTwips( short _dxaHotZ )
	  {
	    this->dxaHotZ = _dxaHotZ;
	  }

	  void SetConsecutiveHyphenationLimit( short _cConsecHypLim )
	  {
	    this->cConsecHypLim = _cConsecHypLim;
	  }

	  void SetDocumentCreationDateAndTime( DTTM _dttmCreated )
	  {
	    this->dttmCreated = _dttmCreated;
	  }

	  void SetDocumentLastSaveDateAndTime( DTTM _dttmRevised )
	  {
	    this->dttmRevised = _dttmRevised;
	  }

	  void SetDocumentLastPrintDateAndTime( DTTM _dttmLastPrint )
	  {
	    this->dttmLastPrint = _dttmLastPrint;
	  }
	  
	  void SetRevisionNumber( short _nRevision )
	  {
	    this->nRevision = _nRevision;
	  }

	  void SetEditingNumber( int _tmEdited )
	  {
	    this->tmEdited = _tmEdited;
	  }

	  void SetCountOfWords( int _cWords )
	  {
	    this->cWords = _cWords;
	  }

	  void SetCountOfCharacters( int _cCh )
	  {
	    this->cCh = _cCh;
	  }

	  void SetCountOfPages( short _cPg )
	  {
	    this->cPg = _cPg;
	  }

	  void SetCountOfParagraphs( int _cParas )
	  {
	    this->cParas = _cParas;
	  }

	  void SetEndnoteNumbering( Constants::rncEdn _rncEdn )
	  {
	    this->rncEdn = _rncEdn;
	  }

	  void SetEndnotesStartingNumber( short _nEdn )
	  {
	    this->nEdn = _nEdn;
	  }

	  void SetEndnotesPlace( Constants::epc _epc )
	  {
	    this->epc = _epc;
	  }

	  void SetPrintFormData( bool _fPrintFormData )
	  {
	    this->fPrintFormData = _fPrintFormData;
	  }

	  void SetSaveFormData( bool _fSaveFormData )
	  {
	    this->fSaveFormData = _fSaveFormData;
	  }

      void SetShadeFormData( bool _fShadeFormData )
	  {
	    this->fShadeFormData = _fShadeFormData;
	  }

	  void SetShadeMergeFields( bool _fShadeMergeFields )
	  {
	    this->fShadeMergeFields = _fShadeMergeFields;
	  }

	  void SetIncludeSubdocsInStats( bool _fIncludeSubdocsInStats )
	  {
	    this->fIncludeSubdocsInStats = _fIncludeSubdocsInStats;
	  }

	  void SetCountOfLines( int _cLines )
	  {
	    this->cLines = _cLines;
	  }

	  void SetWordsWithSubdocs( int _cWordsWithSubdocs )
	  {
	    this->cWordsWithSubdocs = _cWordsWithSubdocs;
	  }

	  void SetCharactersWithSubdocs( int _cChWithSubdocs )
	  {
	    this->cChWithSubdocs = _cChWithSubdocs;
	  }

	  void SetPagesWithSubdocs( short _cPgWithSubdocs )
	  {
	    this->cPgWithSubdocs = _cPgWithSubdocs;
	  }

	  void SetParagraphsWithSubdocs( int _cParasWithSubdocs )
	  {
	    this->cParasWithSubdocs = _cParasWithSubdocs;
	  }

	  void SetLinesWithSubdocs( int _cLinesWithSubdocs )
	  {
	    this->cLinesWithSubdocs = _cLinesWithSubdocs;
	  }

	  void SetKeyProtectionDocument( int _lKeyProtDoc )
	  {
	    this->lKeyProtDoc = _lKeyProtDoc;
	  }

	  void SetSavedDocumentViewingMode( Constants::ViewingMode _wvkoSaved )
	  {
	    this->wvkoSaved = _wvkoSaved;
	  }

	  void SetSavedDocumentZoom( unsigned short _pctWwdSaved )
	  {
	    this->pctWwdSaved = _pctWwdSaved;
	  }

	  void SetSavedDocumentZoomType( Constants::ZoomType _zkSaved )
	  {
	    this->zkSaved = _zkSaved;
	  }

	  void SetGutterTopPosition( bool _iGutterPos )
	  {
	    this->iGutterPos = _iGutterPos;
	  }
  
	  virtual operator unsigned char*() const
	  {
	    this->SetBytes();
		  
		return (unsigned char*)this->bytes;
	  }

	  virtual operator const unsigned char*() const
	  {
	    this->SetBytes();
		  
	    return (const unsigned char*)this->bytes;
	  }

	  virtual unsigned int Size() const
	  {
	    return sizeof(this->bytes);
	  }
  };
}
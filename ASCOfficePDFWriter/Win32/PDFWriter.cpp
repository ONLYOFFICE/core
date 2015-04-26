#include "stdafx.h"
#include "PDFWriter.h"

#include "../PdfWriterLib/PdfWriterLib.h"

HRESULT CPDFWriter::FinalConstruct()
{
	m_oPdfWriter = new CPdfWriterLib();
	
	if (m_oPdfWriter == NULL) return S_FALSE;
	return S_OK ;
}

void CPDFWriter::FinalRelease()
{
	RELEASEOBJECT(m_oPdfWriter);
}
HRESULT CPDFWriter::OnlineWordToPdf (BSTR sPathXml, BSTR sDstFile, BSTR sHtmlPlace, LONG nReg)
{
	return m_oPdfWriter->OnlineWordToPdf( std::wstring(sPathXml), std::wstring(sDstFile), nReg);
}


/////////////////////////
//-----------------------------------------------------------------------------------------------------
//
// Интерфейс IPDFWriter
//
//-----------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::CreatePDF ()
{
	return m_oPdfWriter->CreatePDF();
}
HRESULT CPDFWriter::DeletePDF ()
{
	return m_oPdfWriter->DeletePDF();
}
HRESULT CPDFWriter::SaveToFile (BSTR bsPath)
{
	return m_oPdfWriter->SaveToFile(CString(bsPath));
}
HRESULT CPDFWriter::SetPDFPageMode (long nMode)
{
	return m_oPdfWriter->SetPDFPageMode(nMode);
}
HRESULT CPDFWriter::GetPDFPageMode (long *pnMode)
{
	return m_oPdfWriter->GetPDFPageMode(pnMode);
}
HRESULT CPDFWriter::SetPDFPageLayout (long nLayout)
{
	return m_oPdfWriter->SetPDFPageLayout (nLayout);
}
HRESULT CPDFWriter::GetPDFPageLayout (long *pnLayout)
{
	return m_oPdfWriter->GetPDFPageLayout (pnLayout) ;
}
HRESULT CPDFWriter::SetPDFViewerPreference (long nValue )
{
	return m_oPdfWriter->SetPDFViewerPreference (nValue ) ;
}
HRESULT CPDFWriter::GetPDFViewerPreference (long *pnValue)
{
	return m_oPdfWriter->GetPDFViewerPreference (pnValue) ;
}
HRESULT CPDFWriter::SetPDFCompressionMode (long nMode)
{
	return m_oPdfWriter->SetPDFCompressionMode (nMode) ;
}
HRESULT CPDFWriter::SetPDFOpenAction (long nAction)
{
	return m_oPdfWriter->SetPDFOpenAction (nAction) ;
}
HRESULT CPDFWriter::GetLastErrorCode (long *pnErrorCode)
{
	return m_oPdfWriter->GetLastErrorCode (pnErrorCode) ;
}
HRESULT CPDFWriter::GetLastErrorString (BSTR *pbsErrorString)
{
	CString sErrorString;
	m_oPdfWriter->GetLastErrorString (&sErrorString) ;
	*pbsErrorString = sErrorString.AllocSysString();
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::AddNewPage ()
{		
	return m_oPdfWriter->AddNewPage () ;
}

HRESULT CPDFWriter::GetPagesCount (long *pnCount )
{
	return m_oPdfWriter->GetPagesCount (pnCount ) ;		
}
HRESULT CPDFWriter::SetCurrentPage (long nIndex)
{
	return m_oPdfWriter->SetCurrentPage (nIndex) ;
}

HRESULT CPDFWriter::GetCurrentPageIndex (long *pnIndex)
{
	return m_oPdfWriter->GetCurrentPageIndex (pnIndex) ;
}
HRESULT CPDFWriter::SetPageWidth (float fWidth)
{
	return m_oPdfWriter->SetPageWidth ( fWidth) ;
}
HRESULT CPDFWriter::SetPageHeight (float fHeight)
{
	return m_oPdfWriter->SetPageHeight ( fHeight) ;
}
HRESULT CPDFWriter::GetWidth (float *pfWidth)
{
	return m_oPdfWriter->GetWidth ( pfWidth) ;
}
HRESULT CPDFWriter::GetHeight (float *pfHeight)
{
	return m_oPdfWriter->GetHeight ( pfHeight) ;
}
HRESULT CPDFWriter::SetSize (long nSize, long nDirection)
{
	return m_oPdfWriter->SetSize (nSize, nDirection) ;
}
HRESULT CPDFWriter::SetRotate (long nAngle)
{
	return m_oPdfWriter->SetRotate (nAngle) ;
}
HRESULT CPDFWriter::AddLabel (long nPageNum, long nNumStyle, long nFirstPage, BSTR bsPrefix )
{
	return m_oPdfWriter->AddLabel (nPageNum, nNumStyle, nFirstPage,CString(bsPrefix) ) ;
}
//-----------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::SetLineWidth (float fLineWidth)
{
	return m_oPdfWriter->SetLineWidth ( fLineWidth) ;
}
HRESULT CPDFWriter::GetLineWidth (float *pfLineWidth)
{
	return m_oPdfWriter->GetLineWidth ( pfLineWidth) ;
}
HRESULT CPDFWriter::SetLineCap (long nLineCap)
{
	return m_oPdfWriter->SetLineCap (nLineCap) ;
}
HRESULT CPDFWriter::GetLineCap (long *pnLineCap)
{
	return m_oPdfWriter->GetLineCap (pnLineCap) ;
}
HRESULT CPDFWriter::SetLineJoin (long nLineJoin)
{
	return m_oPdfWriter->SetLineJoin (nLineJoin) ;
}
HRESULT CPDFWriter::GetLineJoin (long *pnLineJoin)
{
	return m_oPdfWriter->GetLineJoin (pnLineJoin) ;
}
HRESULT CPDFWriter::SetMiterLimit (float fMiterLimit)
{
	return m_oPdfWriter->SetMiterLimit ( fMiterLimit) ;
}
HRESULT CPDFWriter::GetMiterLimit (float *pfMiterLimit)
{
	return m_oPdfWriter->GetMiterLimit ( pfMiterLimit) ;
}
HRESULT CPDFWriter::SetDash (SAFEARRAY **ppnDashPtn, long   nNumParam, float   fPhase)
{
	return S_OK;
	//return m_oPdfWriter->SetDash(ppnDashPtn,   nNumParam,    fPhase) ;
}

HRESULT CPDFWriter::GetDash (SAFEARRAY **ppnDashPtn, long *pnNumParam, float *pfPhase)
{
	return S_OK;
	//return m_oPdfWriter->GetDash (ppnDashPtn, pnNumParam,  pfPhase) ;
}
HRESULT CPDFWriter::SetFlat (float   fFlatness)
{
	return m_oPdfWriter->SetFlat (   fFlatness) ;
}
HRESULT CPDFWriter::GetFlat (float *pfFlatness)
{
	return m_oPdfWriter->GetFlat ( pfFlatness) ;
}
HRESULT CPDFWriter::NewExtGraphicsState ()
{
	return m_oPdfWriter->NewExtGraphicsState () ;
}
HRESULT CPDFWriter::ExtGraphicsStateSetAlphaStroke (float fValue)
{
	return m_oPdfWriter->ExtGraphicsStateSetAlphaStroke ( fValue) ;
}
HRESULT CPDFWriter::ExtGraphicsStateSetAlphaFill (float fValue)
{
	return m_oPdfWriter->ExtGraphicsStateSetAlphaFill ( fValue) ;
}
HRESULT CPDFWriter::ExtGraphicsStateSetBlendMode (long nBlendMode)
{
	return m_oPdfWriter->ExtGraphicsStateSetBlendMode (nBlendMode) ;
}
HRESULT CPDFWriter::ExtGraphicsStateSetStrokeAdjustment (int nFlag)
{
	return m_oPdfWriter->ExtGraphicsStateSetStrokeAdjustment (nFlag) ;
}
HRESULT CPDFWriter::SetExtGraphicsState ()
{
	return m_oPdfWriter->SetExtGraphicsState () ;
}
HRESULT CPDFWriter::GSave ()
{
	return m_oPdfWriter->GSave () ;
}
HRESULT CPDFWriter::GRestore ()
{
	return m_oPdfWriter->GRestore () ;
}
HRESULT CPDFWriter::GetGStateDepth (long *pnDepth)
{
	return m_oPdfWriter->GetGStateDepth (pnDepth) ;
}
HRESULT CPDFWriter::GetGMode (long *pnGMode)
{
	return m_oPdfWriter->GetGMode (pnGMode) ;
}
HRESULT CPDFWriter::Concat (float fA, float fB, float fC, float fD, float fX, float fY)
{
	return m_oPdfWriter->Concat ( fA,  fB,  fC,  fD,  fX,  fY) ;
}
HRESULT CPDFWriter::MoveTo (float fX, float fY)
{
	return m_oPdfWriter->MoveTo ( fX,  fY) ;
}
HRESULT CPDFWriter::GetCurrentPosition (float *pfX, float *pfY)
{
	return m_oPdfWriter->GetCurrentPosition ( pfX,  pfY) ;
}
HRESULT CPDFWriter::LineTo(float fX, float fY)
{
	return m_oPdfWriter->LineTo( fX,  fY) ;
}
HRESULT CPDFWriter::CurveTo (float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
{
	return m_oPdfWriter->CurveTo( fX1,  fY1,  fX2,  fY2,  fX3,  fY3) ;
}
HRESULT CPDFWriter::CurveTo2 (float fX2, float fY2, float fX3, float fY3)
{
	return m_oPdfWriter->CurveTo2 ( fX2,  fY2,  fX3,  fY3) ;
}
HRESULT CPDFWriter::CurveTo3 (float fX1, float fY1, float fX3, float fY3)
{
	return m_oPdfWriter->CurveTo3 ( fX1,  fY1,  fX3,  fY3) ;
}
HRESULT CPDFWriter::ClosePath ()
{
	return m_oPdfWriter->ClosePath () ;
}
HRESULT CPDFWriter::AppendRectangle (float fX, float fY, float fWidth, float fHeight)
{
	return m_oPdfWriter->AppendRectangle ( fX,  fY,  fWidth,  fHeight) ;
}

HRESULT CPDFWriter::Stroke ()
{
	return m_oPdfWriter->Stroke () ;
}
HRESULT CPDFWriter::GetStrokingColorSpace (long *pnColorSpace)
{
	return m_oPdfWriter->GetStrokingColorSpace (pnColorSpace) ;
}
HRESULT CPDFWriter::ClosePathStroke ()
{
	return m_oPdfWriter->ClosePathStroke () ;
}
HRESULT CPDFWriter::Fill ()
{
	return m_oPdfWriter->Fill () ;
}
HRESULT CPDFWriter::GetFillingColorSpace (long *pnColorSpace)
{
	return m_oPdfWriter->GetFillingColorSpace (pnColorSpace) ;
}
HRESULT CPDFWriter::EoFill ()
{
	return m_oPdfWriter->EoFill () ;
}
HRESULT CPDFWriter::FillStroke ()
{
	return m_oPdfWriter->FillStroke () ;
}
HRESULT CPDFWriter::EoFillStroke ()
{
	return m_oPdfWriter->EoFillStroke () ;
}
HRESULT CPDFWriter::ClosePathFillStroke ()
{
	return m_oPdfWriter->ClosePathFillStroke () ;
}
HRESULT CPDFWriter::ClosePathEoFillStroke ()
{
	return m_oPdfWriter->ClosePathEoFillStroke () ;
}
HRESULT CPDFWriter::EndPath ()
{
	return m_oPdfWriter->EndPath () ;
}

HRESULT CPDFWriter::Clip ()
{
	return m_oPdfWriter->Clip () ;
}
HRESULT CPDFWriter::EoClip ()
{
	return m_oPdfWriter->EoClip () ;
}

HRESULT CPDFWriter::BeginText ()
{
	return m_oPdfWriter->BeginText () ;
}
HRESULT CPDFWriter::EndText ()
{
	return m_oPdfWriter->EndText () ;
}

HRESULT CPDFWriter::SetCharSpace (float fValue)
{
	return m_oPdfWriter->SetCharSpace ( fValue) ;
}
HRESULT CPDFWriter::GetCharSpace (float *pfValue)
{
	return m_oPdfWriter->GetCharSpace ( pfValue) ;
}
HRESULT CPDFWriter::SetWordSpace (float fValue)
{
	return m_oPdfWriter->SetWordSpace ( fValue) ;
}
HRESULT CPDFWriter::GetWordSpace (float *pfValue)
{
	return m_oPdfWriter->GetWordSpace ( pfValue) ;
}
HRESULT CPDFWriter::SetHorizontalScalling (float fValue)
{
	return m_oPdfWriter->SetHorizontalScalling ( fValue) ;
}
HRESULT CPDFWriter::GetHorizontalScalling (float *pfValue)
{
	return m_oPdfWriter->GetHorizontalScalling ( pfValue) ;
}
HRESULT CPDFWriter::SetTextLeading (float fValue)
{
	return m_oPdfWriter->SetTextLeading ( fValue) ;
}
HRESULT CPDFWriter::GetTextLeading (float *pfValue)
{
	return m_oPdfWriter->GetTextLeading ( pfValue) ;
}
HRESULT CPDFWriter::SetFontAndSizeToGState (float fSize)
{
	return m_oPdfWriter->SetFontAndSizeToGState ( fSize) ;
}
HRESULT CPDFWriter::GetCurrentFontFromGState (BSTR *pbsFontName, BSTR *pbsEncodingName)
{
	CString sFontName, sEncodingName;
	m_oPdfWriter->GetCurrentFontFromGState (&sFontName, &sEncodingName) ;
	
	*pbsFontName = sFontName.AllocSysString() ;
	*pbsEncodingName = sEncodingName.AllocSysString() ;
	return S_OK;
}
HRESULT CPDFWriter::GetCurrentFontSizeFromGState (float *pfSize)
{
	return m_oPdfWriter->GetCurrentFontSizeFromGState ( pfSize) ;
}
HRESULT CPDFWriter::SetTextRenderingMode (long nMode)
{
	return m_oPdfWriter->SetTextRenderingMode (nMode) ;
}
HRESULT CPDFWriter::GetTextRenderingMode (long *pnMode)
{
	return m_oPdfWriter->GetTextRenderingMode (pnMode) ;
}
HRESULT CPDFWriter::SetTextRise (float fValue)
{
	return m_oPdfWriter->SetTextRise ( fValue) ;
}
HRESULT CPDFWriter::GetTextRise (float *pfValue)
{
	return m_oPdfWriter->GetTextRise ( pfValue) ;
}
HRESULT CPDFWriter::MoveTextPos (float fX, float fY)
{
	return m_oPdfWriter->MoveTextPos ( fX,  fY) ;
}
HRESULT CPDFWriter::MoveTextPos2 (float fX, float fY)
{
	return m_oPdfWriter->MoveTextPos2 ( fX,  fY) ;
}
HRESULT CPDFWriter::GetCurrentTextPosition (float *pfX, float *pfY)
{
	return m_oPdfWriter->GetCurrentTextPosition ( pfX,  pfY) ;
}
HRESULT CPDFWriter::SetTextMatrix (float   fA, float   fB, float   fC, float   fD, float   fX, float   fY)
{
	return m_oPdfWriter->SetTextMatrix (   fA,    fB,    fC,    fD,    fX,    fY) ;
}	
HRESULT CPDFWriter::GetTextMatrix (float *pfA, float *pfB, float *pfC, float *pfD, float *pfX, float *pfY)
{
	return m_oPdfWriter->GetTextMatrix ( pfA,  pfB,  pfC,  pfD,  pfX,  pfY) ;
}	
HRESULT CPDFWriter::MoveToNextLine ()
{
	return m_oPdfWriter->MoveToNextLine () ;
}	
HRESULT CPDFWriter::ShowText (BSTR bsText)
{
	return m_oPdfWriter->ShowText (CString(bsText)) ;
}	
HRESULT CPDFWriter::ShowTextNextLine (BSTR bsText)
{
	return m_oPdfWriter->ShowTextNextLine (CString(bsText));
}	
HRESULT CPDFWriter::ShowTextNextLineEx (float fWordSpace, float fCharSpace, BSTR bsText)
{
	return m_oPdfWriter->ShowTextNextLineEx ( fWordSpace,  fCharSpace,CString(bsText)) ;
}	
HRESULT CPDFWriter::SetGrayStroke (float fGray)
{
	return m_oPdfWriter->SetGrayStroke ( fGray) ;
}	
HRESULT CPDFWriter::GetGrayStroke (float *pfGray)
{
	return m_oPdfWriter->GetGrayStroke ( pfGray) ;
}	
HRESULT CPDFWriter::SetGrayFill (float fGray)
{
	return m_oPdfWriter->SetGrayFill ( fGray) ;
}	
HRESULT CPDFWriter::GetGrayFill (float *pfGray)
{
	return m_oPdfWriter->GetGrayFill ( pfGray) ;
}
HRESULT CPDFWriter::SetRGBStroke (float   fR, float   fG, float   fB)
{
	return m_oPdfWriter->SetRGBStroke (   fR,    fG,    fB) ;
}	
HRESULT CPDFWriter::GetRGBStroke (float *pfR, float *pfG, float *pfB)
{
	return m_oPdfWriter->GetRGBStroke ( pfR,  pfG,  pfB) ;
}	
HRESULT CPDFWriter::SetRGBFill (float fR, float fG, float fB)
{
	return m_oPdfWriter->SetRGBFill ( fR,  fG,  fB) ;
}	
HRESULT CPDFWriter::GetRGBFill (float *pfR, float *pfG, float *pfB)
{
	return m_oPdfWriter->GetRGBFill ( pfR,  pfG,  pfB) ;
}
HRESULT CPDFWriter::SetCMYKStroke (float   fC, float   fM, float   fY, float   fK)
{
	return m_oPdfWriter->SetCMYKStroke (   fC,    fM,    fY,    fK) ;
}	
HRESULT CPDFWriter::GetCMYKStroke (float *pfC, float *pfM, float *pfY, float *pfK)
{
	return m_oPdfWriter->GetCMYKStroke ( pfC,  pfM,  pfY,  pfK) ;
}	
HRESULT CPDFWriter::SetCMYKFill (float   fC, float   fM, float   fY, float   fK)
{
	return m_oPdfWriter->SetCMYKFill (   fC,    fM,    fY,    fK) ;
}	
HRESULT CPDFWriter::GetCMYKFill (float *pfC, float *pfM, float *pfY, float *pfK)
{
	return m_oPdfWriter->GetCMYKFill ( pfC,  pfM,  pfY,  pfK) ;
}
HRESULT CPDFWriter::ExecuteXObject ()
{
	return m_oPdfWriter->ExecuteXObject () ;
}

HRESULT CPDFWriter::SetExtGraphicsState2 (BSTR bsXmlOptions)
{
	return m_oPdfWriter->SetExtGraphicsState2 (CString(bsXmlOptions)) ;
}
//-----------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::AppendCircle (float fX, float fY, float fRad)
{
	return m_oPdfWriter->AppendCircle ( fX,  fY,  fRad) ;
}
HRESULT CPDFWriter::AppendEllipse (float fX, float fY, float fXRad, float fYRad)
{
	return m_oPdfWriter->AppendEllipse ( fX,  fY,  fXRad,  fYRad) ;
}
HRESULT CPDFWriter::AppendArc (float fX, float fY, float fRad, float fAngle1, float fAngle2)
{
	return m_oPdfWriter->AppendArc ( fX,  fY,  fRad,  fAngle1,  fAngle2) ;
}
HRESULT CPDFWriter::AppendEllipseArc (float fX, float fY, float fXRad, float fYRad, float fAngle1, float fAngle2, BOOL bClockDirection/* = FALSE*/)
{
	return m_oPdfWriter->AppendEllipseArc ( fX,  fY,  fXRad,  fYRad,  fAngle1,  fAngle2, bClockDirection ) ;
}
HRESULT CPDFWriter::TextOut (float fXPos, float fYPos, BSTR bsText)
{
	return m_oPdfWriter->TextOut_ ( fXPos,  fYPos, std::wstring(bsText)) ;
}
HRESULT CPDFWriter::TextRect (float fLeft, float fTop, float fRight, float fBottom, BSTR bsText, long nAlign, unsigned int *pnLen)
{
	return m_oPdfWriter->TextRect ( fLeft,  fTop,  fRight,  fBottom, CString(bsText), nAlign, pnLen) ;
}
HRESULT CPDFWriter::SetSlideShow (long nType, float fDispTime, float fTransTime)
{
	return m_oPdfWriter->SetSlideShow (nType,  fDispTime,  fTransTime) ;
}
HRESULT CPDFWriter::GetTextWidth (BSTR bsText, float *pfResult)
{
	return m_oPdfWriter->GetTextWidth (std::wstring(bsText),  pfResult) ;
}
HRESULT CPDFWriter::MeasureText (BSTR bsText, float fWidth, BOOL bWordWrap, float *pfRealWidth, long *pnLength)
{
	return m_oPdfWriter->MeasureText (CString(bsText),  fWidth, bWordWrap,  pfRealWidth, pnLength) ;
}

//-----------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::LoadT1FFromFile ( BSTR bsAFMFileName, BSTR bsDataFileName, BSTR *bsFontName)
{
	CString sFontName;
	m_oPdfWriter->LoadT1FFromFile ( CString( bsAFMFileName), CString(bsDataFileName), &sFontName) ;
	*bsFontName = sFontName.AllocSysString();
	return S_OK;
}
HRESULT CPDFWriter::LoadTTFFromFile ( BSTR bsFileName, BOOL bEmbedding, BSTR bsEncodingName, BSTR *bsFontName)
{
	CString sFontName;
	m_oPdfWriter->LoadTTFFromFile ( CString( bsFileName),  bEmbedding, CString( bsEncodingName), &sFontName) ;
	*bsFontName = sFontName.AllocSysString();
	return S_OK;

}
HRESULT CPDFWriter::LoadTTCFromFile ( BSTR bsFileName, long nIndex, BOOL bEmbedding, BSTR *bsFontName)
{
	CString sFontName;
	m_oPdfWriter->LoadTTCFromFile ( CString(bsFileName), nIndex,  bEmbedding, &sFontName) ;
	*bsFontName = sFontName.AllocSysString();
	return S_OK;

}
HRESULT CPDFWriter::SetCurrentFont ( BSTR bsFontName, BSTR bsEncodingName, BOOL bNameIsUnicodeArray, BSTR bsToUnicodeName)
{
	return m_oPdfWriter->SetCurrentFont ( CString(bsFontName), CString(bsEncodingName),  bNameIsUnicodeArray, CString(bsToUnicodeName) );
}
HRESULT CPDFWriter::GetCurrentFont ( BSTR *pbsFontName, BSTR *pbsEncodingName )
{
	CString sEncodingName, sFontName;
	m_oPdfWriter->GetCurrentFont ( &sFontName, &sEncodingName );

	*pbsFontName		= sFontName.AllocSysString();
	*pbsEncodingName	= sEncodingName.AllocSysString();
	return S_OK;
}
HRESULT CPDFWriter::GetFontUnicodeWidth (long nCode, long *pnUnicode)
{
	return m_oPdfWriter->GetFontUnicodeWidth (nCode, pnUnicode) ;
}
HRESULT CPDFWriter::GetFontBBox  (float *pfLeft, float *pfBottom, float *pfRight, float *pfTop)
{
	return m_oPdfWriter->GetFontBBox  ( pfLeft,  pfBottom,  pfRight,  pfTop) ;
}
HRESULT CPDFWriter::GetFontAscent (long *pnAscent)	
{
	return m_oPdfWriter->GetFontAscent (pnAscent) ;
}
HRESULT CPDFWriter::GetFontDescent (long *pnDescent)
{
	return m_oPdfWriter->GetFontDescent (pnDescent) ;
}
HRESULT CPDFWriter::GetFontXHeight (long *pnXHeight)
{
	return m_oPdfWriter->GetFontXHeight (pnXHeight) ;
}
HRESULT CPDFWriter::GetFontCapHeight (long *pnCapHeight)
{
	return m_oPdfWriter->GetFontCapHeight (pnCapHeight) ;
}

//-----------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::SetCurrentEncoder ( BSTR bsEncodingName )
{
	return m_oPdfWriter->SetCurrentEncoder ( CString(bsEncodingName) ) ;
}
HRESULT CPDFWriter::GetCurrentEncoder ( BSTR *pbsEncodingName )
{
	CString sEncodingName;
	m_oPdfWriter->GetCurrentEncoder ( &sEncodingName);
	
	*pbsEncodingName = sEncodingName.AllocSysString();
	return S_OK;
}

HRESULT CPDFWriter::GetEncoderUnicode (long nCode, long *pnUnicode)
{
	return m_oPdfWriter->GetEncoderUnicode (nCode, pnUnicode) ;
}
HRESULT CPDFWriter::GetEncoderByteType (BSTR bsText, long nIndex, long *pnByteType)
{
	return m_oPdfWriter->GetEncoderByteType (CString(bsText), nIndex, pnByteType) ;
}
HRESULT CPDFWriter::GetEncoderType (long *pnEncoderType)
{
	return m_oPdfWriter->GetEncoderType (pnEncoderType) ;
}
HRESULT CPDFWriter::GetEncoderWritingMode (long *pnWritingMode)
{
	return m_oPdfWriter->GetEncoderWritingMode (pnWritingMode) ;
}

//-----------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::CreateNewDestination ()
{
	return m_oPdfWriter->CreateNewDestination () ;
}
HRESULT CPDFWriter::DestSetXYZ (float fLeft, float fTop, float fZoom)
{
	return m_oPdfWriter->DestSetXYZ ( fLeft,  fTop,  fZoom) ;
}
HRESULT CPDFWriter::DestSetFit ()
{
	return m_oPdfWriter->DestSetFit () ;
}
HRESULT CPDFWriter::DestSetFitH (float fTop)
{
	return m_oPdfWriter->DestSetFitH ( fTop) ;
}
HRESULT CPDFWriter::DestSetFitV (float fLeft)
{
	return m_oPdfWriter->DestSetFitV (fLeft) ;
}
HRESULT CPDFWriter::DestSetFitR (float fLeft, float fBottom, float fRight, float fTop)
{
	return m_oPdfWriter->DestSetFitR (fLeft, fBottom,fRight, fTop) ;
}
HRESULT CPDFWriter::DestSetFitB ()
{
	return m_oPdfWriter->DestSetFitB () ;
}
HRESULT CPDFWriter::DestSetFitBH (float fTop)
{
	return m_oPdfWriter->DestSetFitBH (fTop) ;
}
HRESULT CPDFWriter::DestSetFitBV (float fLeft)
{
	return m_oPdfWriter->DestSetFitBV (fLeft) ;
}

HRESULT CPDFWriter::SetCurrentDest (long nIndex)
{
	return m_oPdfWriter->SetCurrentDest (nIndex) ;
}

HRESULT CPDFWriter::GetCurrentDestIndex (long *pnIndex)
{
	return m_oPdfWriter->GetCurrentDestIndex (pnIndex) ;
}
//------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::CreateTextAnnot (float fLeft, float fBottom, float fRight, float fTop, BSTR bsText, BSTR bsEncoder)
{
	return m_oPdfWriter->CreateTextAnnot (fLeft, fBottom, fRight, fTop, CString(bsText), CString( bsEncoder)) ;
}
HRESULT CPDFWriter::CreateLinkAnnot (float fLeft, float fBottom, float fRight, float fTop)
{
	return m_oPdfWriter->CreateLinkAnnot (fLeft, fBottom, fRight, fTop) ;
}
HRESULT CPDFWriter::CreateUriLinkAnnot (float fLeft, float fBottom, float fRight, float fTop, BSTR bsUrl)
{
	return m_oPdfWriter->CreateUriLinkAnnot (fLeft, fBottom, fRight, fTop, CString( bsUrl)) ;
}
HRESULT CPDFWriter::ArbAnnotationSetBorderStyle (long nSubtype, float fWidth, long nDashOn, long nDashOff, long nDashPhase)
{
	return m_oPdfWriter->ArbAnnotationSetBorderStyle (nSubtype, fWidth, nDashOn, nDashOff, nDashPhase) ;
}
HRESULT CPDFWriter::LinkAnnotationSetBorderStyle (float fWidth, long nDashOn, long nDashOff )
{
	return m_oPdfWriter->LinkAnnotationSetBorderStyle (fWidth, nDashOn, nDashOff ) ;
}
HRESULT CPDFWriter::LinkAnnotationSetHighlightMode (long nMode)
{
	return m_oPdfWriter->LinkAnnotationSetHighlightMode (nMode) ;
}
HRESULT CPDFWriter::TextAnnotationSetIcon (long nIcon)
{
	return m_oPdfWriter->TextAnnotationSetIcon (nIcon) ;
}
HRESULT CPDFWriter::TextAnnotationSetOpened (BOOL bOpened)
{
	return m_oPdfWriter->TextAnnotationSetOpened ( bOpened) ;
}

HRESULT CPDFWriter::SetCurrentAnnot (long nIndex)
{
	return m_oPdfWriter->SetCurrentAnnot (nIndex) ;
}

HRESULT CPDFWriter::GetCurrentAnnotIndex (long *pnIndex)
{
	return m_oPdfWriter->GetCurrentAnnotIndex (pnIndex) ;
}
HRESULT CPDFWriter::MakeAnnotationFromXml (BSTR bsXML)
{
	return m_oPdfWriter->MakeAnnotationFromXml (CString(bsXML)) ;
}
//------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::CreateNewOutline (long nParentIndex, BSTR bsTitle, BSTR bsEncoder)
{
	return m_oPdfWriter->CreateNewOutline (nParentIndex, CString(bsTitle), CString(bsEncoder));
}
HRESULT CPDFWriter::OutlineSetCurrentDestination ()
{
	return m_oPdfWriter->OutlineSetCurrentDestination () ;
}
HRESULT CPDFWriter::SetOutlineOpened (BOOL bOpened)
{
	return m_oPdfWriter->SetOutlineOpened ( bOpened) ;
}

HRESULT CPDFWriter::SetCurrentOutline (long nIndex)
{
	return m_oPdfWriter->SetCurrentOutline (nIndex) ;
}

HRESULT CPDFWriter::GetCurrentOutlineIndex (long *pnIndex)
{
	return m_oPdfWriter->GetCurrentOutlineIndex (pnIndex) ;
}
HRESULT CPDFWriter::MakeOutlineFromXml (BSTR bsXML)
{
	return m_oPdfWriter->MakeOutlineFromXml (CString( bsXML)) ;
}
//------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::LoadJpegFromFile (BSTR bsFileName)
{
	return m_oPdfWriter->LoadJpegFromFile (CString(bsFileName)) ;
}
HRESULT CPDFWriter::LoadJpxFromFile (BSTR bsFileName, long nFlag)
{
	return m_oPdfWriter->LoadJpxFromFile (CString(bsFileName), nFlag) ;
}
HRESULT CPDFWriter::LoadImageFromFile (BSTR bsFileName, long nWidth, long nHeight, long nColorSpace)
{
	return S_OK;
	//return m_oPdfWriter->LoadImageFromFile (CString( bsFileName), nWidth, nHeight, nColorSpace) ;
}
HRESULT CPDFWriter::LoadImageFromArray (SAFEARRAY **ppImage, long nWidth, long nHeight, long nColorSpace, long nBitsPerComponent)
{
	return S_OK;
	//return m_oPdfWriter->LoadImageFromArray (ppImage, nWidth, nHeight, nColorSpace, nBitsPerComponent) ;
}
HRESULT CPDFWriter::LoadImageFromInterface (IUnknown **pInterface, BOOL bAlpha/* = FALSE*/)
{
	return S_OK;
	//return m_oPdfWriter->LoadImageFromInterface (pInterface,  bAlpha/* = FALSE*/) ;
}
HRESULT CPDFWriter::DrawCurrentImage (float fX, float fY, float fWidth, float fHeight)
{
	return m_oPdfWriter->DrawCurrentImage (fX, fY, fWidth, fHeight) ;
}

HRESULT CPDFWriter::GetImageBitsPerComponent (long *pnBitsPerComponent)
{
	return m_oPdfWriter->GetImageBitsPerComponent (pnBitsPerComponent) ;
}
HRESULT CPDFWriter::GetImageColorSpace (long *pnColorSpace)
{
	return m_oPdfWriter->GetImageColorSpace (pnColorSpace) ;
}
HRESULT CPDFWriter::GetImageHeight (long *pnHeight)
{
	return m_oPdfWriter->GetImageHeight (pnHeight) ;
}
HRESULT CPDFWriter::GetImageWidth (long *pnWidth)
{
	return m_oPdfWriter->GetImageWidth (pnWidth) ;
}
HRESULT CPDFWriter::SetImageColorMask (long nRMin, long nRMax, long nGMin, long nGMax, long nBMin, long nBMax)
{
	return m_oPdfWriter->SetImageColorMask (nRMin, nRMax, nGMin, nGMax, nBMin, nBMax) ;
}
HRESULT CPDFWriter::SetImageMaskImage (long nMaskImageIndex)
{
	return m_oPdfWriter->SetImageMaskImage (nMaskImageIndex) ;
}
HRESULT CPDFWriter::SetCurrentImage (long nIndex)
{
	return m_oPdfWriter->SetCurrentImage (nIndex) ;
}

HRESULT CPDFWriter::GetCurrentImageIndex (long *pnIndex)
{
	return m_oPdfWriter->GetCurrentImageIndex (pnIndex) ;
}

//------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::SetEncryptOn ()
{
	return m_oPdfWriter->SetEncryptOn () ;
}
HRESULT CPDFWriter::SetPDFPassword (BSTR bsOwnerPassword, BSTR bsUserPassword)
{
	return m_oPdfWriter->SetPDFPassword (CString( bsOwnerPassword), CString( bsUserPassword)) ;
}
HRESULT CPDFWriter::SetPDFPermission (long nPermission)
{
	return m_oPdfWriter->SetPDFPermission (nPermission) ;
}
HRESULT CPDFWriter::SetPDFEncryptionMode (long nMode, long nKeyLen)
{
	return m_oPdfWriter->SetPDFEncryptionMode (nMode, nKeyLen) ;
}
HRESULT CPDFWriter::SetEncryptOff ()
{
	return m_oPdfWriter->SetEncryptOff () ;
}

//------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::GetInfoItem (long nDataType, BSTR *pbsItem)
{
	CString sItem;
	m_oPdfWriter->GetInfoItem (nDataType, &sItem) ;
	*pbsItem = sItem.AllocSysString();
	return S_OK;
}
HRESULT CPDFWriter::SetInfoItem (long nDataType, BSTR bsData)
{
	return m_oPdfWriter->SetInfoItem (nDataType, CString(bsData)) ;
}
HRESULT CPDFWriter::SetInfoDateItem (long nType, long nYear, long nMonth, long nDay, long nHour, long nMinutes, long nSeconds, long nInd, long nOffHour, long nOffMinutes)
{
	return m_oPdfWriter->SetInfoDateItem ( nType, nYear, nMonth, nDay, nHour, nMinutes, nSeconds, nInd, nOffHour, nOffMinutes) ;
}
//------------------------------------------------------------------------------------------------
HRESULT CPDFWriter::SetAdditionalParam (BSTR ParamName, VARIANT	ParamValue)
{
	return m_oPdfWriter->SetAdditionalParam (CString(ParamName), ParamValue) ;
}
HRESULT CPDFWriter::GetAdditionalParam (BSTR ParamName, VARIANT *	ParamValue)
{
	return m_oPdfWriter->GetAdditionalParam (CString(ParamName), ParamValue);
}

//-----------------------------------------------------------------------------------------------------
//
// Интерфейс IAVSOfficeRendererTemplate (IPDFRenderer) IAVSRenderer
//
//-----------------------------------------------------------------------------------------------------
//-------- Тип рендерера--------------------------------------------------------------------------
HRESULT CPDFWriter::get_Type (LONG *plType)
{
	return m_oPdfWriter->get_Type (plType) ;
}
//-------- Функции для работы со страницей -------------------------------------------------------
HRESULT CPDFWriter::NewPage ()
{
	return m_oPdfWriter->NewPage () ;
}
HRESULT CPDFWriter::put_Height (double   dHeight)
{
	return m_oPdfWriter->put_Height (  dHeight) ;
}
HRESULT CPDFWriter::get_Height (double *pdHeight)
{
	return m_oPdfWriter->get_Height (pdHeight) ;
}
HRESULT CPDFWriter::put_Width (double dWidth)
{
	return m_oPdfWriter->put_Width (dWidth) ;
}
HRESULT CPDFWriter::get_Width (double *pdWidth)
{
	return m_oPdfWriter->get_Width (pdWidth) ;
}
HRESULT CPDFWriter::get_DpiX (double *pdDpiX)
{
	return m_oPdfWriter->get_DpiX (pdDpiX) ;
}
HRESULT CPDFWriter::get_DpiY (double *pdDpiY)
{
	return m_oPdfWriter->get_DpiY (pdDpiY) ;
}
//-------- Функции для работы с Pen --------------------------------------------------------------
HRESULT CPDFWriter::SetPen (BSTR bsXML)
{
	return S_OK;
	//return m_oPdfWriter->SetPen (CString( bsXML)) ;
}
HRESULT CPDFWriter::get_PenColor (LONG *plColor)
{
	return m_oPdfWriter->get_PenColor (plColor) ;
}
HRESULT CPDFWriter::put_PenColor (LONG lColor)
{
	return m_oPdfWriter->put_PenColor (lColor) ;
}
HRESULT CPDFWriter::get_PenAlpha (LONG *plAlpha)
{
	return m_oPdfWriter->get_PenAlpha (plAlpha) ;
}
HRESULT CPDFWriter::put_PenAlpha (LONG lAlpha)
{
	return m_oPdfWriter->put_PenAlpha (lAlpha) ;
}
HRESULT CPDFWriter::get_PenSize (double *pdSize)
{
	return m_oPdfWriter->get_PenSize (pdSize) ;
}
HRESULT CPDFWriter::put_PenSize (double dSize)
{
	return m_oPdfWriter->put_PenSize (dSize) ;
}
HRESULT CPDFWriter::get_PenDashStyle (BYTE *pnStyle)
{
	return m_oPdfWriter->get_PenDashStyle (pnStyle) ;
}
HRESULT CPDFWriter::put_PenDashStyle (BYTE nStyle)
{
	return m_oPdfWriter->put_PenDashStyle (nStyle) ;
}
HRESULT CPDFWriter::get_PenLineStartCap (BYTE *pnStartCap)
{
	return m_oPdfWriter->get_PenLineStartCap (pnStartCap) ;
}
HRESULT CPDFWriter::put_PenLineStartCap (BYTE nStartCap)
{
	return m_oPdfWriter->put_PenLineStartCap (nStartCap) ;
}
HRESULT CPDFWriter::get_PenLineEndCap (BYTE *pnEndCap)
{
	return m_oPdfWriter->get_PenLineEndCap (pnEndCap) ;
}
HRESULT CPDFWriter::put_PenLineEndCap (BYTE nEndCap)
{
	return m_oPdfWriter->put_PenLineEndCap (nEndCap) ;
}
HRESULT CPDFWriter::get_PenLineJoin (BYTE *pnJointStyle)
{
	return m_oPdfWriter->get_PenLineJoin (pnJointStyle) ;
}
HRESULT CPDFWriter::put_PenLineJoin (BYTE nJointStyle)
{
	return m_oPdfWriter->put_PenLineJoin (nJointStyle) ;
}
HRESULT CPDFWriter::get_PenDashOffset (double *pdOffset)
{
	return m_oPdfWriter->get_PenDashOffset (pdOffset) ;
}
HRESULT CPDFWriter::put_PenDashOffset (double dOffset)
{
	return m_oPdfWriter->put_PenDashOffset (dOffset) ;
}
HRESULT CPDFWriter::get_PenAlign (LONG *plAlign)
{
	return m_oPdfWriter->get_PenAlign (plAlign) ;
}
HRESULT CPDFWriter::put_PenAlign (LONG lAlign)
{
	return m_oPdfWriter->put_PenAlign (lAlign) ;
}
HRESULT CPDFWriter::get_PenMiterLimit (double *pdMiterLimit)
{
	return m_oPdfWriter->get_PenMiterLimit (pdMiterLimit) ;
}
HRESULT CPDFWriter::put_PenMiterLimit (double dMiterLimit)
{
	return m_oPdfWriter->put_PenMiterLimit (dMiterLimit) ;
}
HRESULT CPDFWriter::PenDashPattern (SAFEARRAY* pPattern)
{
	if (pPattern == NULL) return S_OK;
	return m_oPdfWriter->PenDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
}
//-------- Функции для работы с Brush ------------------------------------------------------------
HRESULT CPDFWriter::SetBrush (BSTR bsXML)
{
	return S_OK;
	//return m_oPdfWriter->SetBrush (CString(bsXML)) ;
}
HRESULT CPDFWriter::get_BrushType (LONG *plType)
{
	return m_oPdfWriter->get_BrushType (plType) ;
}
HRESULT CPDFWriter::put_BrushType (LONG lType)
{
	return m_oPdfWriter->put_BrushType (lType) ;
}
HRESULT CPDFWriter::get_BrushColor1 (LONG *plColor)
{
	return m_oPdfWriter->get_BrushColor1 (plColor) ;
}
HRESULT CPDFWriter::put_BrushColor1 (LONG lColor)
{
	return m_oPdfWriter->put_BrushColor1 (lColor) ;
}
HRESULT CPDFWriter::get_BrushAlpha1 (LONG *plAlpha)
{
	return m_oPdfWriter->get_BrushAlpha1 (plAlpha) ;
}
HRESULT CPDFWriter::put_BrushAlpha1 (LONG lAlpha)
{
	return m_oPdfWriter->put_BrushAlpha1 (lAlpha) ;
}
HRESULT CPDFWriter::get_BrushColor2 (LONG *plColor)
{
	return m_oPdfWriter->get_BrushColor2 (plColor) ;
}
HRESULT CPDFWriter::put_BrushColor2 (LONG lColor)
{
	return m_oPdfWriter->put_BrushColor2 (lColor) ;
}
HRESULT CPDFWriter::get_BrushAlpha2 (LONG *plAlpha)
{
	return m_oPdfWriter->get_BrushAlpha2 (plAlpha) ;
}
HRESULT CPDFWriter::put_BrushAlpha2 (LONG lAlpha)
{
	return m_oPdfWriter->put_BrushAlpha2 (lAlpha) ;
}
HRESULT CPDFWriter::get_BrushTexturePath (BSTR *pbsPath)
{
	std::wstring sPath;
	m_oPdfWriter->get_BrushTexturePath (&sPath);
	*pbsPath = std_string2string(sPath).AllocSysString();
	return  S_OK;
}
HRESULT CPDFWriter::put_BrushTexturePath (BSTR bsPath)
{
	return m_oPdfWriter->put_BrushTexturePath (std::wstring(bsPath)) ;
}
HRESULT CPDFWriter::get_BrushTextureMode (LONG *plMode)
{
	return m_oPdfWriter->get_BrushTextureMode (plMode) ;
}
HRESULT CPDFWriter::put_BrushTextureMode (LONG lMode)
{
	return m_oPdfWriter->put_BrushTextureMode (lMode);
}
HRESULT CPDFWriter::get_BrushTextureAlpha (LONG *plTxAlpha)
{
	return m_oPdfWriter->get_BrushTextureAlpha (plTxAlpha) ;
}
HRESULT CPDFWriter::put_BrushTextureAlpha (LONG lTxAlpha)
{
	return m_oPdfWriter->put_BrushTextureAlpha (lTxAlpha) ;
}
HRESULT CPDFWriter::get_BrushLinearAngle (double *pdAngle)
{
	return m_oPdfWriter->get_BrushLinearAngle (pdAngle) ;
}
HRESULT CPDFWriter::put_BrushLinearAngle (double dAngle)
{
	return m_oPdfWriter->put_BrushLinearAngle (dAngle) ;
}
HRESULT CPDFWriter::BrushRect (BOOL bVal, double dLeft, double dTop, double dWidth, double dHeight)
{
	return m_oPdfWriter->BrushRect (bVal, dLeft, dTop, dWidth, dHeight) ;
}
//-------- Функции для работы с Шрифтом ----------------------------------------------------------
HRESULT CPDFWriter::SetFont (BSTR bsXML)
{
	return S_OK;
	//return m_oPdfWriter->SetFont (CString(bsXML)) ;

}
HRESULT CPDFWriter::get_FontName (BSTR *pbsName)
{
	std::wstring sName;
	m_oPdfWriter->get_FontName (&sName) ;
	*pbsName = std_string2string(sName).AllocSysString();
	return S_OK;
}
HRESULT CPDFWriter::put_FontName (BSTR bsName)
{
	return m_oPdfWriter->put_FontName (std::wstring(bsName)) ;
}
HRESULT CPDFWriter::get_FontPath (BSTR *pbsPath)
{
	std::wstring sPath;
	m_oPdfWriter->get_FontPath (&sPath);
	*pbsPath = std_string2string(sPath).AllocSysString();
	return S_OK;
}
HRESULT CPDFWriter::put_FontPath (BSTR bsName)
{		
	return m_oPdfWriter->put_FontPath (std::wstring(bsName)) ;
}
HRESULT CPDFWriter::get_FontSize (double *pdSize)
{
	return m_oPdfWriter->get_FontSize (pdSize) ;
}
HRESULT CPDFWriter::put_FontSize (double dSize)
{
	return m_oPdfWriter->put_FontSize (dSize) ;
}
HRESULT CPDFWriter::get_FontStyle (LONG *plStyle)
{
	return m_oPdfWriter->get_FontStyle (plStyle) ;
}
HRESULT CPDFWriter::put_FontStyle (LONG lStyle)
{
	return m_oPdfWriter->put_FontStyle (lStyle) ;
}
HRESULT CPDFWriter::get_FontStringGID (BOOL *pbGID)
{
	return m_oPdfWriter->get_FontStringGID (pbGID) ;
}
HRESULT CPDFWriter::put_FontStringGID (BOOL bGID)
{
	return m_oPdfWriter->put_FontStringGID (bGID);
}
HRESULT CPDFWriter::get_FontCharSpace (double *pdSpace)
{
	return m_oPdfWriter->get_FontCharSpace (pdSpace) ;
}
HRESULT CPDFWriter::put_FontCharSpace (double dSpace)
{
	return m_oPdfWriter->put_FontCharSpace (dSpace) ;
}

//-------- Функции для работы с Shadow -----------------------------------------------------------
HRESULT CPDFWriter::SetShadow (BSTR bsXML)
{
	return S_OK;
//	return m_oPdfWriter->SetShadow (CString(bsXML)) ;
}
HRESULT CPDFWriter::get_ShadowDistanceX (double *pdX)
{
	return m_oPdfWriter->get_ShadowDistanceX (pdX) ;
}
HRESULT CPDFWriter::put_ShadowDistanceX (double dX)
{
	return m_oPdfWriter->put_ShadowDistanceX (dX) ;
}
HRESULT CPDFWriter::get_ShadowDistanceY (double *pdY)
{
	return m_oPdfWriter->get_ShadowDistanceY (pdY) ;
}
HRESULT CPDFWriter::put_ShadowDistanceY (double dY)
{
	return m_oPdfWriter->put_ShadowDistanceY (dY) ;
}
HRESULT CPDFWriter::get_ShadowBlurSize (double *pdSize)
{
	return m_oPdfWriter->get_ShadowBlurSize (pdSize) ;
}
HRESULT CPDFWriter::put_ShadowBlurSize (double dSize)
{
	return m_oPdfWriter->put_ShadowBlurSize (dSize) ;
}
HRESULT CPDFWriter::get_ShadowColor (LONG *plColor)
{
	return m_oPdfWriter->get_ShadowColor (plColor) ;
}
HRESULT CPDFWriter::put_ShadowColor (LONG lColor)
{
	return m_oPdfWriter->put_ShadowColor (lColor) ;
}
HRESULT CPDFWriter::get_ShadowAlpha (LONG *plAlpha)
{
	return m_oPdfWriter->get_ShadowAlpha (plAlpha) ;
}
HRESULT CPDFWriter::put_ShadowAlpha (LONG lAlpha)
{
	return m_oPdfWriter->put_ShadowAlpha (lAlpha) ;
}
HRESULT CPDFWriter::get_ShadowVisible (BOOL *pbVisible)
{
	bool res;
	return m_oPdfWriter->get_ShadowVisible (&res) ;
	*pbVisible = (BOOL)res;
}
HRESULT CPDFWriter::put_ShadowVisible (BOOL bVisible)
{
	return m_oPdfWriter->put_ShadowVisible ( bVisible );
}
//-------- Функции для работы с Edge Text --------------------------------------------------------
HRESULT CPDFWriter::SetEdgeText (BSTR bsXML)
{
	return S_OK;
	//return m_oPdfWriter->SetEdgeText (CString( bsXML)) ;
}
HRESULT CPDFWriter::get_EdgeVisible (LONG *plVisible)
{
	return m_oPdfWriter->get_EdgeVisible (plVisible) ;
}
HRESULT CPDFWriter::put_EdgeVisible (LONG lVisible)
{
	return m_oPdfWriter->put_EdgeVisible (lVisible) ;
}
HRESULT CPDFWriter::get_EdgeColor (LONG *plColor)
{
	return m_oPdfWriter->get_EdgeColor (plColor) ;
}
HRESULT CPDFWriter::put_EdgeColor (LONG lColor)
{
	return m_oPdfWriter->put_EdgeColor (lColor) ;
}
HRESULT CPDFWriter::get_EdgeAlpha (LONG *plAlpha)
{
	return m_oPdfWriter->get_EdgeAlpha (plAlpha);
}
HRESULT CPDFWriter::put_EdgeAlpha (LONG lAlpha)
{
	return m_oPdfWriter->put_EdgeAlpha (lAlpha) ;
}
HRESULT CPDFWriter::get_EdgeDist (double *pdDist)
{
	return m_oPdfWriter->get_EdgeDist (pdDist) ;
}
HRESULT CPDFWriter::put_EdgeDist (double dDist)
{
	return m_oPdfWriter->put_EdgeDist (dDist) ;
}
//--------- Функции для вывода текста ------------------------------------------------------------
HRESULT CPDFWriter::CommandDrawText (BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return m_oPdfWriter->CommandDrawText (std::wstring(bsText), fX, fY, fWidth, fHeight, fBaseLineOffset) ;
}
HRESULT CPDFWriter::CommandDrawTextEx (BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSrcCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD nFlags)
{
	return m_oPdfWriter->CommandDrawTextEx (std::wstring(bsUnicodeText),std::wstring(bsGidText)/*, std::wstring(bsSrcCodeText)*/, fX, fY, fWidth, fHeight, fBaseLineOffset, nFlags) ;
}
//--------- Маркеры для команд -------------------------------------------------------------------
HRESULT CPDFWriter::BeginCommand (DWORD nFlag)
{
	return m_oPdfWriter->BeginCommand (nFlag) ;
}
HRESULT CPDFWriter::EndCommand (DWORD nFlag)
{
	return m_oPdfWriter->EndCommand (nFlag) ;
}
//--------- Функции для работы с Graphics Path ---------------------------------------------------
HRESULT CPDFWriter::PathCommandStart ()
{
	return m_oPdfWriter->PathCommandStart () ;
}	
HRESULT CPDFWriter::PathCommandEnd ()
{
	return m_oPdfWriter->PathCommandEnd () ;
}
//
HRESULT CPDFWriter::PathCommandMoveTo (double fX,  double fY)
{
	return m_oPdfWriter->PathCommandMoveTo (fX,  fY) ;
}
HRESULT CPDFWriter::PathCommandLineTo (double fX,  double fY)
{
	return m_oPdfWriter->PathCommandLineTo (fX,  fY) ;
}
HRESULT CPDFWriter::PathCommandLinesTo (SAFEARRAY *pPoints)
{
	if (pPoints == NULL) return S_OK;
	return m_oPdfWriter->PathCommandLinesTo ((double*)pPoints->pvData, pPoints->rgsabound[0].cElements) ;
}
HRESULT CPDFWriter::PathCommandCurveTo (double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	return m_oPdfWriter->PathCommandCurveTo (fX1, fY1, fX2, fY2, fX3, fY3) ;
}
HRESULT CPDFWriter::PathCommandCurvesTo (SAFEARRAY *pPoints)
{
	if (pPoints == NULL) return S_OK;
	return m_oPdfWriter->PathCommandCurvesTo ((double*)pPoints->pvData, pPoints->rgsabound[0].cElements) ;
}
HRESULT CPDFWriter::PathCommandArcTo (double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	return m_oPdfWriter->PathCommandArcTo (fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
}
HRESULT CPDFWriter::PathCommandClose ()
{
	return m_oPdfWriter->PathCommandClose ();
}
//
HRESULT CPDFWriter::DrawPath (long nType)
{
	return m_oPdfWriter->DrawPath (nType);
}

HRESULT CPDFWriter::PathCommandGetCurrentPoint (double *pfX, double *pfY)
{
	return m_oPdfWriter->PathCommandGetCurrentPoint (pfX, pfY);
}
HRESULT CPDFWriter::PathCommandText (BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset )
{
	return m_oPdfWriter->PathCommandText (std::wstring(bsText), fX, fY, fWidth, fHeight, fBaseLineOffset );
}

HRESULT CPDFWriter::PathCommandTextEx (BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSrcCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD nFlags )
{
	return m_oPdfWriter->PathCommandTextEx (std::wstring(bsUnicodeText),std::wstring(bsGidText)/*, std::wstring(bsSrcCodeText)*/, fX, fY, fWidth, fHeight, fBaseLineOffset, nFlags );
}

//--------- Функции для вывода изображений -------------------------------------------------------
HRESULT CPDFWriter::DrawImage (IUnknown *pInterface, double fX, double fY, double fWidth, double fHeight)
{
	return m_oPdfWriter->DrawImage ((IGrObject*)pInterface, fX, fY, fWidth, fHeight);
}
HRESULT CPDFWriter::DrawImageFromFile (BSTR bsFilePath, double fX, double fY, double fWidth, double fHeight)
{
	return m_oPdfWriter->DrawImageFromFile (std::wstring(bsFilePath), fX, fY, fWidth, fHeight);
}
//--------- Transform ----------------------------------------------------------------------------
HRESULT CPDFWriter::GetCommandParams (double *pdAngle, double *pdLeft, double *pdTop, double *pdWidth, double *pdHeight, DWORD *pnFlags )
{
	return m_oPdfWriter->GetCommandParams (pdAngle, pdLeft, pdTop, pdWidth, pdHeight, pnFlags );
}
HRESULT CPDFWriter::SetCommandParams (double   dAngle, double   dLeft, double   dTop, double   dWidth, double   dHeight, DWORD   nFlags)
{
	return m_oPdfWriter->SetCommandParams (dAngle, dLeft, dTop, dWidth, dHeight, nFlags);
}

HRESULT CPDFWriter::SetTransform (double   dA, double   dB, double   dC, double   dD, double   dE, double   dF)
{
	return m_oPdfWriter->SetTransform (  dA,   dB,   dC,   dD,   dE,   dF);
}

HRESULT CPDFWriter::GetTransform (double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return m_oPdfWriter->GetTransform (pdA, pdB, pdC, pdD, pdE, pdF);
}
HRESULT CPDFWriter::ResetTransform ()
{
	return m_oPdfWriter->ResetTransform () ;
}

//--------- Clip ---------------------------------------------------------------------------------
HRESULT CPDFWriter::get_ClipMode (LONG* plMode)
{
	return m_oPdfWriter->get_ClipMode (plMode) ;
}
HRESULT CPDFWriter::put_ClipMode (LONG lMode)
{
	return m_oPdfWriter->put_ClipMode (lMode);
}
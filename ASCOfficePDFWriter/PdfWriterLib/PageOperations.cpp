#include "PageOperations.h"


static const float c_fKappa = 0.552F;

static char* QuarterCircleA (char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX - fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX - fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fRad, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}
static char* QuarterCircleB (char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX + fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX + fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX + fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}
static char* QuarterCircleC (char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX + fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX + fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fRad, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}
static char* QuarterCircleD (char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX - fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX - fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX - fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}


static char* QuarterEllipseA(char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fXRad, float fYRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX - fXRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fYRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX - fXRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fYRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fYRad, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}
static char* QuarterEllipseB(char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fXRad, float fYRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX + fXRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fYRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX + fXRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY + fYRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX + fXRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}
static char* QuarterEllipseC(char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fXRad, float fYRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX + fXRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fYRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX + fXRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fYRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fYRad, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}
static char* QuarterEllipseD(char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fXRad, float fYRad)
{
    pBufPointer = UtilsFToA( pBufPointer, fX - fXRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fYRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX - fXRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY - fYRad * c_fKappa, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX - fXRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}


static unsigned long InternalShowTextNextLine(Page pPage, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    unsigned long nRet = OK;

    PageAttr pAttr = (PageAttr)pPage->pAttr;
    FontAttr pFontAttr = (FontAttr)pAttr->pGState->pFont->pAttr;

    if ( FontType0TT == pFontAttr->eType ||  FontType0CID == pFontAttr->eType ) 
	{
        if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "<" ) ) )
            return nRet;

        if ( OK != ( nRet = StreamWriteBinary( pAttr->pStream, (BYTE*)sText, nLen, NULL ) ) )
            return nRet;

        if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, ">" ) ) )
            return nRet;
    } 
	else if ( OK != ( nRet = StreamWriteEscapeText2( pAttr->pStream, sText, nLen ) ) )
        return nRet;

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " \'\012" ) ) )
        return nRet;

    float fTextWidth = PageTextWidth( pPage, sText, nLen, pCIDs, nLenCID );

    pAttr->oTextMatrix.fX -= pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fC;
    pAttr->oTextMatrix.fY -= pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fD;

    pAttr->oTextPos.fX = pAttr->oTextMatrix.fX;
    pAttr->oTextPos.fY = pAttr->oTextMatrix.fY;

    if ( WModeHorizontal == pAttr->pGState->eWritingMode ) 
	{
        pAttr->oTextPos.fX += fTextWidth * pAttr->oTextMatrix.fA;
        pAttr->oTextPos.fY += fTextWidth * pAttr->oTextMatrix.fB;
    } 
	else 
	{
        pAttr->oTextPos.fX -= fTextWidth * pAttr->oTextMatrix.fB;
        pAttr->oTextPos.fY -= fTextWidth * pAttr->oTextMatrix.fA;
    }

    return nRet;
}
static unsigned long InternalWriteText(PageAttr pAttr, const BYTE *sText, unsigned int nLen)
{
	FontAttr pFontAttr = (FontAttr)pAttr->pGState->pFont->pAttr;

	unsigned long nRet = OK;

	if ( FontType0TT  == pFontAttr->eType || FontType0CID == pFontAttr->eType || FontDefCID == pFontAttr->pFontDef->eType ) 
	{
		if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "<" ) ) )
            return nRet;

		if ( OK != ( nRet = StreamWriteBinary( pAttr->pStream, sText, nLen, NULL ) ) )
            return nRet;

		return StreamWriteStr( pAttr->pStream, ">" );
    }
	return StreamWriteEscapeText( pAttr->pStream, sText, nLen );
}
static unsigned long InternalArc      (Page pPage, float fX, float fY, float fRay, float fAng1, float fAng2, BOOL bContFlag)
{
    const float fPIE = 3.14159F;

    char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;
    unsigned long nRet = 0;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    double dDeltaAngle = (90 - (double)(fAng1 + fAng2) / 2) / 180 * fPIE;
    double dNewAngle   = (double)(fAng2 - fAng1) / 2 / 180 * fPIE;

    double dRX0 = fRay * cos(dNewAngle);
    double dRY0 = fRay * sin(dNewAngle);
    double dRX2 = ( fRay * 4.0 - dRX0 ) / 3.0;
    double dRY2 = ( ( fRay * 1.0 - dRX0 ) * ( dRX0 - fRay * 3.0 ) ) / (3.0 * dRY0);
    double dRX1 = dRX2;
    double dRY1 = -dRY2;
    double dRX3 = dRX0;
    double dRY3 = -dRY0;

    double dX0 = dRX0 * cos (dDeltaAngle) - dRY0 * sin (dDeltaAngle) + fX;
    double dY0 = dRX0 * sin (dDeltaAngle) + dRY0 * cos (dDeltaAngle) + fY;
    double dX1 = dRX1 * cos (dDeltaAngle) - dRY1 * sin (dDeltaAngle) + fX;
    double dY1 = dRX1 * sin (dDeltaAngle) + dRY1 * cos (dDeltaAngle) + fY;
    double dX2 = dRX2 * cos (dDeltaAngle) - dRY2 * sin (dDeltaAngle) + fX;
    double dY2 = dRX2 * sin (dDeltaAngle) + dRY2 * cos (dDeltaAngle) + fY;
    double dX3 = dRX3 * cos (dDeltaAngle) - dRY3 * sin (dDeltaAngle) + fX;
    double dY3 = dRX3 * sin (dDeltaAngle) + dRY3 * cos (dDeltaAngle) + fY;

    if ( !bContFlag ) 
	{
        pBufPointer = UtilsFToA( pBufPointer, (float)dX0, pEndBufPointer);
        *pBufPointer++ = ' ';
        pBufPointer = UtilsFToA( pBufPointer, (float)dY0, pEndBufPointer);
        pBufPointer = (char*)UtilsStrCpy( pBufPointer, " m\012", pEndBufPointer);
    }

    pBufPointer = UtilsFToA( pBufPointer, (float)dX1, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, (float)dY1, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, (float)dX2, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, (float)dY2, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, (float)dX3, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, (float)dY3, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = (float)dX3;
	pAttr->oCurPos.fY = (float)dY3;
	pAttr->oStrPos    = pAttr->oCurPos;
	pAttr->nGMode     = GMODE_PATH_OBJECT;

    return nRet;
}



static char* EllipseArc     (char *pBufPointer, char *pEndBufPointer, float fX, float fY, float fXRad, float fYRad, double dAngle1, double dAngle2, float *pfXCur, float *pfYCur, BOOL bClockDirection = FALSE)
{
	// Рассчитаем начальную, конечную и контрольные точки
	float fX1  = 0.0f, fX2  = 0.0f, fY1  = 0.0f, fY2  = 0.0f;
	float fCX1 = 0.0f, fCX2 = 0.0f, fCY1 = 0.0f, fCY2 = 0.0f;

	float fAlpha = sin( dAngle2 - dAngle1 ) * ( sqrt( 4.0 + 3.0 * tan( (dAngle2 - dAngle1) / 2.0 ) * tan( (dAngle2 - dAngle1) / 2.0 ) ) - 1.0 ) / 3.0;

	float fKoef = 1;

	fX1 = fX + fXRad * cos( dAngle1 );
	fY1 = fY + fYRad * sin( dAngle1 );

	fX2 = fX + fXRad * cos( dAngle2 );
	fY2 = fY + fYRad * sin( dAngle2 );

	fCX1 = fX1 - fAlpha * fXRad * sin ( dAngle1 );
	fCY1 = fY1 + fAlpha * fYRad * cos ( dAngle1 );

	fCX2 = fX2 + fAlpha * fXRad * sin ( dAngle2 );
	fCY2 = fY2 - fAlpha * fYRad * cos ( dAngle2 );

	if ( !bClockDirection )
	{
		pBufPointer = UtilsFToA( pBufPointer, fCX1, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fCY1, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fCX2, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fCY2, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fX2, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fY2, pEndBufPointer);
		*pfXCur = fX2; 
		*pfYCur = fY2;
	}
	else
	{
		pBufPointer = UtilsFToA( pBufPointer, fCX2, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fCY2, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fCX1, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fCY1, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fX1, pEndBufPointer);
		*pBufPointer++ = ' ';
		pBufPointer = UtilsFToA( pBufPointer, fY1, pEndBufPointer);
		*pfXCur = fX1; 
		*pfYCur = fY1;
	}
    return (char*)UtilsStrCpy( pBufPointer, " c\012", pEndBufPointer);
}
float AngToEllPrm(float fAngle, float fXRad, float fYRad)
{
	// Функция для перевода реального угла в параметрическое задание эллписа
	// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
	// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
	return atan2( sin( fAngle ) / fYRad,  cos( fAngle ) / fXRad );
}

///////////////////////////////////////////////////////////////////////////////////

//--- General graphics state --------------------------------------------------------------------------

// Operator   : w 
// Descriprion: устанавливаем толщину линии
unsigned long PageSetLineWidth         (Page pPage, float fLineWidth)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( fLineWidth < 0 )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fLineWidth ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " w\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fLineWidth = fLineWidth;

    return nRet;
}

// Operator   : J 
// Descriprion: устанавливаем вид окончания линии (LineCapStyle)
unsigned long PageSetLineCap           (Page pPage, LineCapStyle eLineCap)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);
    if ( OK != nRet )
        return nRet;

    if ( eLineCap < 0 || eLineCap >= LineCapEOF)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, (unsigned long)eLineCap);

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteInt( pAttr->pStream, (unsigned int)eLineCap ) ) )
        return nRet;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " J\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->eLineCap = eLineCap;

    return nRet;
}
// Operator   : j
// Descriprion: устанавливаем вид соединения линий (LineJoinStyle)
unsigned long PageSetLineJoin          (Page pPage, LineJoinStyle eLineJoin)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);
    if ( OK != nRet )
        return nRet;

    if ( eLineJoin < 0 || eLineJoin >= LineJoinEOF )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, (unsigned long)eLineJoin );

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteInt( pAttr->pStream, (unsigned int)eLineJoin ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " j\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->eLineJoin = eLineJoin;

    return nRet;
}

// Operator   : M
// Descriprion: устанавливаем MiterLimit - константа, относящаяся к виду соединения линий
unsigned long PageSetMiterLimit        (Page pPage, float fMiterLimit)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

    if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( fMiterLimit < 1 )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0 );

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fMiterLimit ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " M\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fMiterLimit = fMiterLimit;

    return nRet;
}

// Operator   : d
// Descriprion: устанавливаем вид линий (DashMode)
unsigned long PageSetDash              (Page pPage, const double *pdDashPtn, unsigned int nNumParam, float fPhase)
{
	unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	const double *pdTempDashPtn = pdDashPtn;

	//if ( 1 != nNumParam && (nNumParam / 2) * 2 != nNumParam )
	//	return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_PARAM_COUNT, nNumParam);

    if ( 0 == nNumParam && fPhase > 0)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, fPhase);

    if ( !pdDashPtn && nNumParam > 0)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, nNumParam);

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);
    *pBufPointer++ = '[';

	BOOL bFalseDash = TRUE;
    for ( unsigned int nIndex = 0; nIndex < nNumParam; nIndex++) 
	{
        if ( 0 != pdTempDashPtn[nIndex] )
		{
			bFalseDash = FALSE;
			break;
		}
    }

	if ( !bFalseDash )
	{
		for ( unsigned int nIndex = 0; nIndex < nNumParam; nIndex++) 
		{
			if ( /*0 == *pnTempDashPtn ||*/ *pdTempDashPtn > MAX_DASH_PATTERN )
				return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

			pBufPointer = UtilsFToA( pBufPointer, *pdTempDashPtn, pEndBufPointer);
			*pBufPointer++ = ' ';
			pdTempDashPtn++;
		}
	}
	else
	{
		int k = 10;
	}


    *pBufPointer++ = ']';
    *pBufPointer++ = ' ';

    pBufPointer = UtilsFToA( pBufPointer, fPhase, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " d\012", pEndBufPointer);

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->oDashMode = c_oInitDashMode;
	pAttr->pGState->oDashMode.nNumPoints = nNumParam;
	pAttr->pGState->oDashMode.fPhase     = fPhase;

    pdTempDashPtn = pdDashPtn;

    for ( unsigned int nIndex = 0; nIndex < nNumParam; nIndex++) 
	{
		pAttr->pGState->oDashMode.afPtn[nIndex] = (float)*pdTempDashPtn;
        pdTempDashPtn++;
    }

    return nRet;
}

// TO DO: ri

// Operator   : i
// Descriprion: устанавливаем порог ошибки линии (Flatness tolerance)
unsigned long PageSetFlat              (Page pPage, float fFlatness)
{
	unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( fFlatness > 100 || fFlatness < 0 )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fFlatness ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " i\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fFlatness = fFlatness;

    return nRet;
}

// Operator   : gs
// Description: устанавливаем сразу все настройки данного графического состояния(ExtGState)
unsigned long PageSetExtGState         (Page pPage, ExtGState pExtGState)
{
	unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;


    if ( !ExtGStateValidate( pExtGState ) )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);

	// проверяем принадлежность объекта pExtGState
	if ( pPage->oMMgr != pExtGState->oMMgr )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_EXT_GSTATE, 0);

	PageAttr pAttr = (PageAttr)pPage->pAttr;
	const char *sLocalName = PageGetExtGStateName( pPage, pExtGState);

    if ( !sLocalName )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteEscapeName( pAttr->pStream, sLocalName ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " gs\012" ) ) )
		return CheckError( pPage->oError );

    // меняем класс объекта на readonly
	pExtGState->pHeader.nObjClass = (OSUBCLASS_EXT_GSTATE_R | OCLASS_DICT);

    return nRet;
}

//--- Special graphic state operator ------------------------------------------------------------------

// Operator   : q
// Description: сохраняем текущий GState в графическом стеке
unsigned long PageGSave                (Page pPage)
{
	unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;
	GState pNewGState = GStateNew( pPage->oMMgr, pAttr->pGState );

    if ( !pNewGState )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "q\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState = pNewGState;

    return nRet;
}
// Operator   : Q
// Description: Восстанавливаем GState, удаляя самый последний GState, и делаем данный GState текущим
unsigned long PageGRestore             (Page pPage)
{
	unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	// предыдущего состояния нет :)
	if ( !pAttr->pGState->pPrev )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_RESTORE_GSTATE, 0);

	GState pNewGState = GStateFree( pPage->oMMgr, pAttr->pGState );

	pAttr->pGState = pNewGState;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "Q\012" ) ) )
		return CheckError( pPage->oError );

    return nRet;
}
// Operator   : cm
// Description: меняем матрицу преобразований (CTM - Current Transformation Matrix)
unsigned long PageConcat               (Page pPage, float fA, float fB, float fC, float fD, float fX, float fY)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION);
    if ( OK != nRet )
        return nRet;

    char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fA, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fB, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fC, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fD, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " cm\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	TransMatrix oCTM = pAttr->pGState->oTransMatrix;

	// перемножаем матрицы oCTM(новая)= oCTM(преобразования(которая параметрами задана)) x oCTM(старая)

	// должно быть:
	//pAttr->pGState->oTransMatrix.fA = oCTM.fA * fA + oCTM.fC * fB;
	//pAttr->pGState->oTransMatrix.fB = oCTM.fB * fA + oCTM.fD * fB;
	//pAttr->pGState->oTransMatrix.fC = oCTM.fA * fC + oCTM.fC * fD;
	//pAttr->pGState->oTransMatrix.fD = oCTM.fB * fC + oCTM.fD * fD;
	//pAttr->pGState->oTransMatrix.fX = oCTM.fX + fX * oCTM.fA + fY * oCTM.fC;
	//pAttr->pGState->oTransMatrix.fY = oCTM.fY + fX * oCTM.fB + fY * oCTM.fD;

	pAttr->pGState->oTransMatrix.fA = oCTM.fA * fA + oCTM.fB * fC;
	pAttr->pGState->oTransMatrix.fB = oCTM.fA * fB + oCTM.fB * fD;
	pAttr->pGState->oTransMatrix.fC = oCTM.fC * fA + oCTM.fD * fC;
	pAttr->pGState->oTransMatrix.fD = oCTM.fC * fB + oCTM.fD * fD;
	pAttr->pGState->oTransMatrix.fX = oCTM.fX + fX * oCTM.fA + fY * oCTM.fC;
	pAttr->pGState->oTransMatrix.fY = oCTM.fY + fX * oCTM.fB + fY * oCTM.fD;

    return nRet;
}


//--- Path construction operator ----------------------------------------------------------------------

// Operator   : m
// Description: Начинаем новый subpath, передвигая текущий указатель в точку (x, y)(она же стартовая). 
unsigned long PageMoveTo               (Page pPage, float fX, float fY)
{
	unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT | GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH );
    if ( OK != nRet )
	{
		nRet = OK;
		nRet = PageCheckState( pPage, GMODE_PATH_OBJECT );
		if ( OK != nRet )
			return nRet;
		else 
			ErrorReset( pPage->oError );
	}

    char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " m\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fX;
	pAttr->oCurPos.fY = fY;
	pAttr->oStrPos = pAttr->oCurPos;
	pAttr->nGMode  = GMODE_PATH_OBJECT;

    return nRet;
}
// Operator   : l
// Description: Добавляем линию от текущей точки до точки (x, y). Текущую точку выставляем (х, у).
unsigned long PageLineTo               (Page pPage, float fX, float fY)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT);
    if ( OK != nRet )
        return nRet;

    char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

    PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " l\012", pEndBufPointer);

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
        return CheckError( pPage->oError );

    pAttr->oCurPos.fX = fX;
    pAttr->oCurPos.fY = fY;

    return nRet;
}

// Operator   : c
// Description: Добавляем кривую Безье(кубическую). Начинается кривая в текущей позиции, заканчивается
//              в точке (x3, y3). (x1, y1) и (x2, y2) - контрольные точки. Текущую точку устанавливаем
//              в (х3, у3).
unsigned long PageCurveTo              (Page pPage, float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT);
    if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX1, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY1, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX2, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY2, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX3, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY3, pEndBufPointer);
    UtilsStrCpy (pBufPointer, " c\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fX3;
	pAttr->oCurPos.fY = fY3;

    return nRet;
}
// Operator   : v
// Description: Добавляем кривую Безье(кубическую). Начинается кривая в текущей позиции, заканчивается
//              в точке (x3, y3). (x2, y2) - контрольная точка. Текущую точку устанавливаем в (х3, у3). 
unsigned long PageCurveTo2             (Page pPage, float fX2, float fY2, float fX3, float fY3)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT);
    if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX2, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY2, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX3, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY3, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " v\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fX3;
	pAttr->oCurPos.fY = fY3;

    return nRet;
}

// Operator   : y
// Description: Добавляем кривую Безье(кубическую). Начинается кривая в текущей позиции, заканчивается
//              в точке (x3, y3). (x1, y1) - контрольная точка. Текущую точку устанавливаем в (х3, у3).
unsigned long PageCurveTo3             (Page pPage, float fX1, float fY1, float fX3, float fY3)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT);
    if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX1, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY1, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX3, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY3, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " y\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fX3;
	pAttr->oCurPos.fY = fY3;

    return nRet;
}

// Operator   : h
// Description: Закрываем subpath, соединяя текущую точку с начальной прямой линией. Если subpath
//              уже закрыт, тогда ничего не делаем
unsigned long PageClosePath            (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT);
    if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "h\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos = pAttr->oStrPos;

    return nRet;
}

// Operator   : re
// Description: Добавляем прямоугольник к текущему path, как уже законченый subpath.
unsigned long PageRectangle            (Page pPage, float fX, float fY, float fWidth, float fHeight)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_PATH_OBJECT);

	if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fWidth, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fHeight, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " re\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fX;
	pAttr->oCurPos.fY = fY;
	pAttr->oStrPos = pAttr->oCurPos;
	pAttr->nGMode = GMODE_PATH_OBJECT;

    return nRet;
}


//--- Path painting operator --------------------------------------------------------------------------

// Operator   : S
// Description: Обводим path.
unsigned long PageStroke               (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);
    
    if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "S\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode  = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : s
// Description: Закрываем и обводим path. (Результат должен быть аналогичен последовательному применению
//              двух операторов "h S")
unsigned long PageClosePathStroke      (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);
    
    if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "s\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode  = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : f
// Description: Заливка path по правилу Nonzero Winding Number Rule(см. спецификацию PDF Part1: PDF 1.7 
//              стр. 136, закладка 8.5.3.3.2). 
unsigned long PageFill                 (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "f\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : f*
// Description: Заливка path по правилу Even-Odd Rule(см. спецификацию PDF Part1: PDF 1.7 стр. 137, 
//              закладка 8.5.3.3.3). 
unsigned long PageEoFill               (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "f*\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : B
// Description: Заливка и обоводка path, используя правило для заливки Nonzero Winding Number Rule(см. 
//              спецификацию PDF Part1: PDF 1.7 стр. 136, закладка 8.5.3.3.2). Этот оператор должен 
//              привести к тому же самому результату как строительство двух идентичных объектов path, 
//              применяя к первому оператор f и ко второму - S.
unsigned long PageFillStroke           (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "B\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : B*
// Description: Заливка и обоводка path, используя правило для заливки Even-Odd Rule(см. 
//              спецификацию PDF Part1: PDF 1.7 стр. 137, закладка 8.5.3.3.3). Этот оператор должен 
//              привести к тому же самому результату как строительство двух идентичных объектов path, 
//              применяя к первому оператор f* и ко второму - S.
unsigned long PageEoFillStroke         (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "B*\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : b
// Description: Закрывем, заливаем и обоводим path, используя правило для заливки Nonzero Winding 
//              Number Rule((см. спецификацию PDF Part1: PDF 1.7 стр. 136, закладка 8.5.3.3.2). 
//              Этот оператор должен привести к тому же самому результату как последовательное применение
//              операторов h и B.
unsigned long PageClosePathFillStroke  (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "b\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : b
// Description: Закрывем, заливаем и обоводим path, используя правило для заливки Even-Odd Rule(см. 
//              спецификацию PDF Part1: PDF 1.7 стр. 137, закладка 8.5.3.3.3). Этот оператор должен 
//              привести к тому же самому результату как последовательное применение операторов h и B*.
unsigned long PageClosePathEoFillStroke(Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "b*\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}
// Operator   : n
// Description: Закрываем path, не заливая и не обводя его. Этот оператор используется прежде всего для 
//              изменения текущего path.
unsigned long PageEndPath              (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT | GMODE_CLIPPING_PATH);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "n\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos = c_oInitPosition;

    return nRet;
}

//--- Clipping paths operator -------------------------------------------------------------------------

// Operator   : W
// Description: Изменяем текущий clipping path, пересакая его с текущим path, ипользуя правило Nonzero 
//              Winding Number Rule, для определения какие регионы лежат внутри clipping path.
unsigned long PageClip                 (Page pPage)
{
	unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "W\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_CLIPPING_PATH;

    return nRet;
}
// Operator   : W*
// Description: Изменяем текущий clipping path, пересакая его с текущим path, ипользуя правило Even-Odd 
//              Rule, для определения какие регионы лежат внутри clipping path.
unsigned long PageEoclip               (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PATH_OBJECT);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "W*\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode = GMODE_CLIPPING_PATH;

    return nRet;
}

//--- Text object operator ----------------------------------------------------------------------------

// Operator   : BT
// Description: Начало текста
unsigned long PageBeginText            (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    const TransMatrix c_oDefaultMatrix = {1, 0, 0, 1, 0, 0};

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "BT\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->nGMode      = GMODE_TEXT_OBJECT;
	pAttr->oTextPos    = c_oInitPosition;
	pAttr->oTextMatrix = c_oDefaultMatrix;

    return nRet;
}
// Operator   : ET
// Description: Окончание текста
unsigned long PageEndText              (Page pPage)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "ET\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->oTextPos = c_oInitPosition;
	pAttr->nGMode   = GMODE_PAGE_DESCRIPTION;

    return nRet;
}

//--- Text state --------------------------------------------------------------------------------------

// Operator   : Tc
// Description: Устанавливаем расстояние между буквами
unsigned long PageSetCharSpace         (Page pPage, float fValue)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;
	
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( fValue < MIN_CHARSPACE || fValue > MAX_CHARSPACE)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fValue ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " Tc\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fCharSpace = fValue;

    return nRet;
}

// Operator   : Tw
// Description: Устанавливаем расстояние между словами
unsigned long PageSetWordSpace         (Page pPage, float fValue)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;
	
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( fValue < MIN_WORDSPACE || fValue > MAX_WORDSPACE )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0 );

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fValue ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " Tw\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fWordSpace = fValue;

    return nRet;
}
// Operator   : Tz
// Description: Устанавливаем горизонтальное растяжение/сжатие
unsigned long PageSetHorizontalScalling(Page pPage, float fValue)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;
	
	PageAttr pAttr = (PageAttr)pPage->pAttr;

  //  if ( fValue < MIN_HORIZONTALSCALING || fValue > MAX_HORIZONTALSCALING )
		//return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0 );

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fValue ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " Tz\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fHScalling = fValue;

    return nRet;
}
// Operator   : TL
// Description: Устанавливаем расстояние между строками
unsigned long PageSetTextLeading       (Page pPage, float fValue)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;
	
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( ( nRet = StreamWriteReal( pAttr->pStream, fValue ) ) )
		return CheckError( pPage->oError );

    if ( ( nRet = StreamWriteStr( pAttr->pStream, " TL\012" ) ) )
		return CheckError( pPage->oError );

    pAttr->pGState->fTextLeading = fValue;

    return nRet;
}
// Operator   : Tf
// Description: Устанавливаем фонт и размер фонта
unsigned long PageSetFontAndSize       (Page pPage, FontDict pFont, float fSize)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;
	
    if ( !FontValidate( pFont ) )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT, 0 );

    if ( fSize < 0 || fSize > MAX_FONTSIZE )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT_SIZE, 0);

	// проверяем принадлежность фонта документу
	if ( pPage->oMMgr != pFont->oMMgr )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT, 0);

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    const char *sLocalName = PageGetLocalFontName( pPage, pFont);

    if ( !sLocalName )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT, 0 );

    if ( OK != ( nRet = StreamWriteEscapeName( pAttr->pStream, sLocalName ) ) )
        return CheckError( pPage->oError );

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	UtilsMemSet (sBuffer, 0, TEMP_BUFFER_SIZE);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fSize, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " Tf\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->pFont = pFont;
	pAttr->pGState->fFontSize = fSize;
	pAttr->pGState->eWritingMode = ((FontAttr)pFont->pAttr)->eWritingMode;

    return nRet;
}
// Operator   : Tr
// Description: Устанавливаем тип закрашивания символов (TextRenderingMode)
unsigned long PageSetTextRenderingMode (Page pPage, TextRenderingMode eMode)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

    if ( OK != nRet )
        return nRet;

    if ( eMode < 0 || eMode >= RenderingModeEOF )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, (unsigned long)eMode );

    PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteInt( pAttr->pStream, (int)eMode ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " Tr\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->eRenderingMode = eMode;

    return nRet;
}
// Operator   : Ts
// Description: Устанавливаем поднятие текста в строке
unsigned long PageSetTextRise          (Page pPage, float fValue)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

    if ( OK != nRet )
        return nRet;

    PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fValue ) ) )
		return CheckError( pPage->oError );

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " Ts\012" ) ) )
		return CheckError( pPage->oError );

    pAttr->pGState->fTextRise = fValue;

    return nRet;
}

//--- Text positioning --------------------------------------------------------------------------------

// Operator   : Td
// Description: Переходим к началу следующей линии, сдвигаясь от начала текущей на ( fX, fY ).
unsigned long PageMoveTextPos          (Page pPage, float fX, float fY)
{
    unsigned long nRet = PageCheckState ( pPage, GMODE_TEXT_OBJECT );

    if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " Td\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oTextMatrix.fX += fX * pAttr->oTextMatrix.fA + fY * pAttr->oTextMatrix.fC;
	pAttr->oTextMatrix.fY += fX * pAttr->oTextMatrix.fB + fY * pAttr->oTextMatrix.fD;
	pAttr->oTextPos.fX = pAttr->oTextMatrix.fX;
	pAttr->oTextPos.fY = pAttr->oTextMatrix.fY;

    return nRet;
}
// Operator   : TD
// Description: Переходим к началу следующей линии, сдвигаясь от начала текущей на ( fX, fY ). При
//              этом расстояние между строк (TextLeading) высталяется -fY. Этот оператор должен 
//              быть эквивалентен следующей паре операторов:
//              -fY TL
//              fX fY Td 
unsigned long PageMoveTextPos2         (Page pPage, float fX, float fY)
{
    unsigned long nRet = PageCheckState ( pPage, GMODE_TEXT_OBJECT );

    if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " TD\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oTextMatrix.fX += fX * pAttr->oTextMatrix.fA + fY * pAttr->oTextMatrix.fC;
	pAttr->oTextMatrix.fY += fX * pAttr->oTextMatrix.fB + fY * pAttr->oTextMatrix.fD;
	pAttr->oTextPos.fX = pAttr->oTextMatrix.fX;
	pAttr->oTextPos.fY = pAttr->oTextMatrix.fY;
	pAttr->pGState->fTextLeading = -fY;

    return nRet;
}
// Operator   : Tm
// Description: Устанавливаем матрицу преобразования для текста.
unsigned long PageSetTextMatrix        (Page pPage, float fA, float fB, float fC, float fD, float fX, float fY)
{
    unsigned long nRet = PageCheckState ( pPage, GMODE_TEXT_OBJECT );

    if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

	// правильнее бы здесь сделать проверку на определить матрицы
    if ( ( 0 == fA || 0 == fD ) && ( 0 == fB || 0 == fC ) )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0 );

    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE );

    pBufPointer = UtilsFToA( pBufPointer, fA, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fB, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fC, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fD, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fX, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " Tm\012", pEndBufPointer);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oTextMatrix.fA = fA;
	pAttr->oTextMatrix.fB = fB;
	pAttr->oTextMatrix.fC = fC;
	pAttr->oTextMatrix.fD = fD;
	pAttr->oTextMatrix.fX = fX;
	pAttr->oTextMatrix.fY = fY;
	pAttr->oTextPos.fX = pAttr->oTextMatrix.fX;
	pAttr->oTextPos.fY = pAttr->oTextMatrix.fY;

    return nRet;
}

// Operator   : T*
// Description: Передвигаемся на начало следующей линии. Этот оператор должен быть эквивалентен строке:
//              0 -tL Td, где tL - параметр равный текущему расстоянию между строк(TextLeading)
unsigned long PageMoveToNextLine       (Page pPage)
{
    unsigned long nRet = PageCheckState ( pPage, GMODE_TEXT_OBJECT );

    if ( OK != nRet )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, "T*\012" ) ) )
		return CheckError( pPage->oError );

    // вычисляем точку, в которую нужно перенести текущую позицию

	pAttr->oTextMatrix.fX -= pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fC;
	pAttr->oTextMatrix.fY -= pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fD;

	pAttr->oTextPos.fX = pAttr->oTextMatrix.fX;
	pAttr->oTextPos.fY = pAttr->oTextMatrix.fY;

    return nRet;
}

//--- Text showing ------------------------------------------------------------------------------------

// Operator   : Tj
// Description: Показать текстовую строку.
unsigned long PageShowText             (Page pPage, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT );

    if ( OK != nRet || NULL == sText || 0 == nLen )
        return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( !pAttr->pGState->pFont ) 
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND, 0);

    float fTextWidth = PageTextWidth( pPage, sText, nLen, pCIDs, nLenCID);

	if ( !fTextWidth )
		return nRet;

    if ( OK != ( nRet = InternalWriteText( pAttr, sText, nLen ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " Tj\012" ) ) )
		return CheckError( pPage->oError );

    // Вычисляем текущую точку текста

	if ( WModeHorizontal == pAttr->pGState->eWritingMode ) 
	{
		pAttr->oTextPos.fX += fTextWidth * pAttr->oTextMatrix.fA;
		pAttr->oTextPos.fY += fTextWidth * pAttr->oTextMatrix.fB;
    } 
	else 
	{
		pAttr->oTextPos.fX -= fTextWidth * pAttr->oTextMatrix.fB;
		pAttr->oTextPos.fY -= fTextWidth * pAttr->oTextMatrix.fA;
    }

    return nRet;
}
// Operator   : '
// Description: Переходим на следующую строку и показываем текст. Этот оператор должен быть эквивалентен
//              паре операторов:
//              T*
//              sText Tj
unsigned long PageShowTextNextLine     (Page pPage, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT );

    if ( OK != nRet )
		return nRet;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( !pAttr->pGState->pFont ) 
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND, 0);

    if ( NULL == sText || 0 == nLen )
        return PageMoveToNextLine( pPage );

    if ( OK != ( nRet = InternalWriteText( pAttr, sText, nLen ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " \'\012" ) ) )
		return CheckError( pPage->oError );

    float fTextWidth = PageTextWidth( pPage, sText, nLen, pCIDs, nLenCID );

	pAttr->oTextMatrix.fX -= pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fC;
	pAttr->oTextMatrix.fY -= pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fD;

	pAttr->oTextPos.fX = pAttr->oTextMatrix.fX;
	pAttr->oTextPos.fY = pAttr->oTextMatrix.fY;

	if ( WModeHorizontal == pAttr->pGState->eWritingMode ) 
	{
		pAttr->oTextPos.fX += fTextWidth * pAttr->oTextMatrix.fA;
		pAttr->oTextPos.fY += fTextWidth * pAttr->oTextMatrix.fB;
    } 
	else 
	{
		pAttr->oTextPos.fX -= fTextWidth * pAttr->oTextMatrix.fB;
		pAttr->oTextPos.fY -= fTextWidth * pAttr->oTextMatrix.fA;
    }

    return nRet;
}
// Operator   : "
// Description: Переходим на следующую строку и показываем текст, с расстоянием между словами fWordSpace и 
//              Расстоянием между символами fCharSpace (соответствующие параметры сохраняем в GState). 
//              Этот оператор должен быть эквивалентен последовательности операторов:
//              fWordSpace Tw
//              fCharSpace Tc
//              sText '
unsigned long PageShowTextNextLineEx   (Page pPage, float fWordSpace, float fCharSpace, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;
	
    if ( fWordSpace < MIN_WORDSPACE || fWordSpace > MAX_WORDSPACE)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

    if ( fCharSpace < MIN_CHARSPACE || fCharSpace > MAX_CHARSPACE)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( !pAttr->pGState->pFont ) 
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND, 0);

    if ( NULL == sText || 0 == nLen )
        return PageMoveToNextLine( pPage );

    char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);
    pBufPointer = UtilsFToA( pBufPointer, fWordSpace, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fCharSpace, pEndBufPointer);
    *pBufPointer = ' ';

	if ( OK != ( nRet = InternalWriteText( pAttr, (BYTE *)sBuffer, UtilsStrLen( sBuffer, LIMIT_MAX_NAME_LEN ) ) ) )
		return CheckError( pPage->oError );

    if ( OK != ( nRet = InternalWriteText( pAttr, sText, nLen ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " \"\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fWordSpace = fWordSpace;
	pAttr->pGState->fCharSpace = fCharSpace;

    float fTextWidth = PageTextWidth( pPage, sText, nLen, pCIDs, nLenCID );

	pAttr->oTextMatrix.fX += pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fB;
	pAttr->oTextMatrix.fY -= pAttr->pGState->fTextLeading * pAttr->oTextMatrix.fA;

	pAttr->oTextPos.fX = pAttr->oTextMatrix.fX;
	pAttr->oTextPos.fY = pAttr->oTextMatrix.fY;

	if ( WModeHorizontal == pAttr->pGState->eWritingMode ) 
	{
		pAttr->oTextPos.fX += fTextWidth * pAttr->oTextMatrix.fA;
		pAttr->oTextPos.fY += fTextWidth * pAttr->oTextMatrix.fB;
    } 
	else 
	{
		pAttr->oTextPos.fX -= fTextWidth * pAttr->oTextMatrix.fB;
		pAttr->oTextPos.fY -= fTextWidth * pAttr->oTextMatrix.fA;
    }

    return nRet;
}
//--- Color showing -----------------------------------------------------------------------------------


// TO DO:

// CS cs SC SCN sc scn 

// Operator   : G
// Description: Устанавливаем цветовое пространтсво для обводки в DeviceGray, а уровень серости(GrayLevel)
//              выставляем равным fGray (для обводки).
unsigned long PageSetGrayStroke  (Page pPage, float fGray)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

    if ( OK != nRet )
        return nRet;
	
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( fGray < 0 || fGray > 1 )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0 );

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fGray ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " G\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fGrayStroke = fGray;
	pAttr->pGState->eCSStroke = CSDeviceGray;

    return nRet;
}
// Operator   : g
// Description: Устанавливаем цветовое пространтсво для заливки в DeviceGray, а уровень серости(GrayLevel)
//              выставляем равным fGray (для заливки).
unsigned long PageSetGrayFill    (Page pPage, float fGray)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

    if ( OK != nRet )
        return nRet;
	
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( fGray < 0 || fGray > 1 )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0 );

	if ( OK != ( nRet = StreamWriteReal( pAttr->pStream, fGray ) ) )
		return CheckError( pPage->oError );

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " g\012" ) ) )
		return CheckError( pPage->oError );

	pAttr->pGState->fGrayFill = fGray;
	pAttr->pGState->eCSFill   = CSDeviceGray;

    return nRet;

}
// Operator   : RG
// Description: Устанавливаем цветовое пространтсво для обводки в DeviceRGB и устанавливаем цвет для 
//              операций связанных с обведением фигур.
unsigned long PageSetRGBStroke   (Page pPage, float fR, float fG, float fB)
{
    if ( fR < 0 || fR > 1 || fG < 0 || fG > 1 || fB < 0 || fB > 1)
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_TEXT_OBJECT);

	if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fR, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fG, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fB, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " RG\012", pEndBufPointer);

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
        return CheckError( pPage->oError );

	pAttr->pGState->oRGBStroke.r = fR;
	pAttr->pGState->oRGBStroke.g = fG;
	pAttr->pGState->oRGBStroke.b = fB;
	pAttr->pGState->eCSStroke = CSDeviceRGB;

    return nRet;
}
// Operator   : rg
// Description: Устанавливаем цветовое пространтсво для заливки в DeviceRGB и устанавливаем цвет для 
//              операций связанных с заливкой фигур.
unsigned long PageSetRGBFill     (Page pPage, float fR, float fG, float fB)
{
    if ( fR < 0 || fR > 1 || fG < 0 || fG > 1 || fB < 0 || fB > 1)
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT | GMODE_PAGE_DESCRIPTION);

	if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fR, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fG, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fB, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " rg\012", pEndBufPointer);

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
        return CheckError( pPage->oError );

    pAttr->pGState->oRGBFill.r = fR;
    pAttr->pGState->oRGBFill.g = fG;
    pAttr->pGState->oRGBFill.b = fB;
    pAttr->pGState->eCSFill = CSDeviceRGB;

    return nRet;
}
// Operator   : K
// Description: Устанавливаем цветовое пространтсво для обводки в DeviceCMYK и устанавливаем цвет для 
//              операций связанных с обведением фигур.
unsigned long PageSetCMYKStroke  (Page pPage, float fC, float fM, float fY, float fK)
{
    if ( fC < 0 || fC > 1 || fM < 0 || fM > 1 || fY < 0 || fY > 1 || fK < 0 || fK > 1)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT | GMODE_PAGE_DESCRIPTION);

	if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fC, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fM, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fK, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " K\012", pEndBufPointer);

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
        return CheckError( pPage->oError );

	pAttr->pGState->oCMYKStroke.c = fC;
    pAttr->pGState->oCMYKStroke.m = fM;
    pAttr->pGState->oCMYKStroke.y = fY;
    pAttr->pGState->oCMYKStroke.k = fK;
	pAttr->pGState->eCSStroke = CSDeviceCMYK;

    return nRet;
}
// Operator   : k
// Description: Устанавливаем цветовое пространтсво для заливки в DeviceCMYK и устанавливаем цвет для 
//              операций связанных с заливкой фигур.
unsigned long PageSetCMYKFill    (Page pPage, float fC, float fM, float fY, float fK)
{
    if ( fC < 0 || fC > 1 || fM < 0 || fM > 1 || fY < 0 || fY > 1 || fK < 0 || fK > 1)
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

	unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT | GMODE_PAGE_DESCRIPTION);

	if ( OK != nRet )
        return nRet;

	char sBuffer[TEMP_BUFFER_SIZE];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fC, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fM, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fK, pEndBufPointer);
    UtilsStrCpy( pBufPointer, " K\012", pEndBufPointer);

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
        return CheckError( pPage->oError );

	pAttr->pGState->oCMYKFill.c = fC;
	pAttr->pGState->oCMYKFill.m = fM;
	pAttr->pGState->oCMYKFill.y = fY;
	pAttr->pGState->oCMYKFill.k = fK;
	pAttr->pGState->eCSFill = CSDeviceCMYK;

    return nRet;
}
//--- Shading patterns --------------------------------------------------------------------------------

// TO DO :

// sh 

//--- In-line images ----------------------------------------------------------------------------------

// TO DO :

// BI ID EI 

//--- XObjects ----------------------------------------------------------------------------------------

// Operator   : Do
// Description: Рисуем объект XObject
unsigned long PageExecuteXObject  (Page pPage, XObject pXobj)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION);

	if ( OK != nRet )
        return nRet;

    if ( !pXobj || pXobj->pHeader.nObjClass != (OSUBCLASS_XOBJECT | OCLASS_DICT) )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);

    if ( pPage->oMMgr != pXobj->oMMgr )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_XOBJECT, 0);

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    const char *sLocalName = PageGetXObjectName( pPage, pXobj);

    if ( !sLocalName )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_XOBJECT, 0);

    if ( OK != ( nRet = StreamWriteEscapeName( pAttr->pStream, sLocalName ) ) )
        return CheckError( pPage->oError );

    if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, " Do\012" ) ) )
        return CheckError( pPage->oError );

    return nRet;
}
//--- Marked content ----------------------------------------------------------------------------------

// TO DO :

// MP DP BMC BDC EMC

//--- Compatibility -----------------------------------------------------------------------------------

// TO DO :

// BX EX

//--- Дополнительные функции(не по спецификации) ------------------------------------------------------


unsigned long PageCircle      (Page pPage, float fX, float fY, float fRad)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_PATH_OBJECT);

	if ( OK != nRet )
        return nRet;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

	char sBuffer[TEMP_BUFFER_SIZE * 2];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
    
	UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX - fRad, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    pBufPointer = (char*)UtilsStrCpy( pBufPointer, " m\012", pEndBufPointer);

    pBufPointer = QuarterCircleA( pBufPointer, pEndBufPointer, fX, fY, fRad);
    pBufPointer = QuarterCircleB( pBufPointer, pEndBufPointer, fX, fY, fRad);
    pBufPointer = QuarterCircleC( pBufPointer, pEndBufPointer, fX, fY, fRad);
    QuarterCircleD( pBufPointer, pEndBufPointer, fX, fY, fRad);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fX - fRad;
	pAttr->oCurPos.fY = fY;
	pAttr->oStrPos = pAttr->oCurPos;
	pAttr->nGMode = GMODE_PATH_OBJECT;

    return nRet;
}

unsigned long PageEllipse     (Page pPage, float fX, float fY, float fXRay, float fYRay)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_PATH_OBJECT);

	if ( OK != nRet )
        return nRet;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

	char sBuffer[TEMP_BUFFER_SIZE * 2];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
    
	UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

    pBufPointer = UtilsFToA( pBufPointer, fX - fXRay, pEndBufPointer);
    *pBufPointer++ = ' ';
    pBufPointer = UtilsFToA( pBufPointer, fY, pEndBufPointer);
    pBufPointer = (char*)UtilsStrCpy( pBufPointer, " m\012", pEndBufPointer);

    pBufPointer = QuarterEllipseA( pBufPointer, pEndBufPointer, fX, fY, fXRay, fYRay);
    pBufPointer = QuarterEllipseB( pBufPointer, pEndBufPointer, fX, fY, fXRay, fYRay);
    pBufPointer = QuarterEllipseC( pBufPointer, pEndBufPointer, fX, fY, fXRay, fYRay);
    QuarterEllipseD( pBufPointer, pEndBufPointer, fX, fY, fXRay, fYRay);

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fX - fXRay;
	pAttr->oCurPos.fY = fY;
	pAttr->oStrPos = pAttr->oCurPos;
	pAttr->nGMode = GMODE_PATH_OBJECT;

    return nRet;
}


unsigned long PageEllipseArc  (Page pPage, float fX, float fY, float fXRad, float fYRad, float fAngle1, float fAngle2, BOOL bClockDirection)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_PATH_OBJECT);

    if ( OK != nRet )
        return nRet;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

	char sBuffer[TEMP_BUFFER_SIZE * 2];
    char *pBufPointer = sBuffer;
    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
    
	UtilsMemSet( sBuffer, 0, TEMP_BUFFER_SIZE);

	// переведем углы в радианы

	double dAngle1 = fAngle1 * 3.141592f / 180;
	double dAngle2 = fAngle2 * 3.141592f / 180;

	// Выясним в каких четвертях находятся начальная и конечная точки
    int nFirstPointQuard  = int(fAngle1) / 90 + 1;
    int nSecondPointQuard = int(fAngle2) / 90 + 1;

    nSecondPointQuard = (std::min)( 4, (std::max)( 1, nSecondPointQuard ) );
    nFirstPointQuard  = (std::min)( 4, (std::max)( 1, nFirstPointQuard ) );
	// Проведем линию в начальную точку дуги
	float fStartX = 0.0f, fStartY = 0.0f, fEndX = 0.0f, fEndY = 0.0f;

	fStartX = fX + fXRad * cos( AngToEllPrm( dAngle1, fXRad, fYRad ) );
	fStartY = fY + fYRad * sin( AngToEllPrm( dAngle1, fXRad, fYRad ) );

	pBufPointer = UtilsFToA( pBufPointer, fStartX, pEndBufPointer);
	*pBufPointer++ = ' ';
	pBufPointer = UtilsFToA( pBufPointer, fStartY, pEndBufPointer);
	pBufPointer = (char*)UtilsStrCpy( pBufPointer, " l\012", pEndBufPointer);
	// Дальше рисуем по четверям

	float fCurX = fStartX, fCurY = fStartY;
	double dStartAngle = dAngle1;
	double dEndAngle = 0;

	if ( !bClockDirection )
	{
		for( unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++ ) 
		{
			if ( nIndex == nSecondPointQuard )
				dEndAngle = dAngle2;
			else
				dEndAngle = (90 * (nIndex ) ) * 3.141592f / 180;
			if ( !( nIndex == nFirstPointQuard ) )
				dStartAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;

			pBufPointer = EllipseArc( pBufPointer, pEndBufPointer, fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
		}
	}
	else
	{
		for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- ) 
		{
			if ( nIndex == nFirstPointQuard )
				dStartAngle = dAngle1;
			else
				dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
			if ( !( nIndex == nSecondPointQuard ) )
				dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
			else
				dEndAngle = dAngle2;

			pBufPointer = EllipseArc( pBufPointer, pEndBufPointer, fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
		}
	}

	if ( OK != ( nRet = StreamWriteStr( pAttr->pStream, sBuffer ) ) )
		return CheckError( pPage->oError );

	pAttr->oCurPos.fX = fEndX;
	pAttr->oCurPos.fY = fEndY;
	pAttr->oStrPos = pAttr->oCurPos;
	pAttr->nGMode = GMODE_PATH_OBJECT;

    return nRet;
}
unsigned long PageEllipseArc2 (Page pPage, float fX, float fY, float fXRad, float fYRad, float fAngle1, float fAngle2, BOOL bClockDirection/* = FALSE*/)
{
	unsigned long nRet = OK;

	// Проверяем эллипс на невырожденность
	if ( fXRad < 0.001 || fYRad < 0.001 )
	{
		double dAngle1 = fAngle1 * 3.141592f / 180;
		double dAngle2 = fAngle2 * 3.141592f / 180;

		if ( fXRad < 0.001 && fYRad < 0.001 )
			nRet = PageLineTo( pPage, fX, fY );
		else if ( fXRad < 0.001 )
		{
			nRet += PageLineTo( pPage, fX, fY + sin( dAngle1 ) * fYRad );
			nRet += PageLineTo( pPage, fX, fY + sin( dAngle2 ) * fYRad );
		}
		else // if ( fYRad < 0.001 ) 
		{
			nRet += PageLineTo( pPage, fX + cos( dAngle1 ) * fXRad, fY );
			nRet += PageLineTo( pPage, fX + cos( dAngle2 ) * fXRad, fY );
		}

		return nRet;
	}


	while ( fAngle1 < 0 )
		fAngle1 += 360;

	while ( fAngle1 > 360 )
		fAngle1 -= 360;

	while ( fAngle2 < 0 )
		fAngle2 += 360;

	while ( fAngle2 >= 360 )
		fAngle2 -= 360;

	if ( !bClockDirection )
	{
		if ( fAngle1 <= fAngle2 )
			nRet = PageEllipseArc( pPage, fX, fY, fXRad, fYRad, fAngle1, fAngle2, FALSE );
		else
		{
			nRet += PageEllipseArc( pPage, fX, fY, fXRad, fYRad, fAngle1, 360, FALSE );
			nRet += PageEllipseArc( pPage, fX, fY, fXRad, fYRad, 0, fAngle2, FALSE );
		}
	}
	else
	{
		if ( fAngle1 >= fAngle2 )
			nRet = PageEllipseArc( pPage, fX, fY, fXRad, fYRad, fAngle1, fAngle2, TRUE );
		else
		{
			nRet += PageEllipseArc( pPage, fX, fY, fXRad, fYRad, fAngle1, 0, TRUE );
			nRet += PageEllipseArc( pPage, fX, fY, fXRad, fYRad, 360, fAngle2, TRUE );
		}
	}
    return nRet;
}
//   Математическое описание см. здесь
//   http://www.tinaja.com/glib/bezarc1.pdf 
//   http://www.whizkidtech.redprince.net/bezier/circle/
unsigned long PageArc         (Page pPage, float fX, float fY, float fRay, float fAng1, float fAng2)
{
    BOOL bContFlag = FALSE;

    unsigned long nRet = PageCheckState( pPage, GMODE_PAGE_DESCRIPTION | GMODE_PATH_OBJECT);

    if ( fAng1 >= fAng2 || ( fAng2 - fAng1 ) >= 360 )
		RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE, 0);

    if ( OK != nRet )
        return nRet;

    while ( fAng1 < 0 || fAng2 < 0 ) 
	{
        fAng1 = fAng1 + 360;
        fAng2 = fAng2 + 360;
    }


    for (;;) 
	{
        if ( fAng2 - fAng1 <= 90 )
            return InternalArc( pPage, fX, fY, fRay, fAng1, fAng2, bContFlag);
        else 
		{
            float fTempAngle = fAng1 + 90;

            if ( OK != ( nRet = InternalArc( pPage, fX, fY, fRay, fAng1, fTempAngle, bContFlag ) ) )
                return nRet;

            fAng1 = fTempAngle;
        }

        if ( fAng1 >= fAng2 )
            break;

        bContFlag = TRUE;
    }

    return OK;
}

unsigned long PageDrawImage   (Page pPage, ImageDict pImage, float fX, float fY, float fWidth, float fHeight)
{
    unsigned long nRet = OK;

    if ( OK != ( nRet = PageGSave( pPage ) ) )
        return nRet;

    if ( OK != ( nRet = PageConcat( pPage, fWidth, 0, 0, fHeight, fX, fY ) ) )
        return nRet;

    if ( OK != ( nRet = PageExecuteXObject( pPage, pImage ) ) )
        return nRet;

    return PageGRestore( pPage );
}
unsigned long PageTextOut     (Page pPage, float fXPos, float fYPos, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT );

    if ( OK != nRet )
        return nRet;

	float fX = 0.0f;
	float fY = 0.0f;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( 0 == pAttr->oTextMatrix.fA ) 
	{
		fY = ( fXPos - pAttr->oTextMatrix.fX ) / pAttr->oTextMatrix.fC;
		fX = ( fYPos - pAttr->oTextMatrix.fY - ( fXPos - pAttr->oTextMatrix.fX ) * pAttr->oTextMatrix.fD / pAttr->oTextMatrix.fC ) / pAttr->oTextMatrix.fB;
    } 
	else 
	{
		fY = ( fYPos - pAttr->oTextMatrix.fY - ( fXPos - pAttr->oTextMatrix.fX ) * pAttr->oTextMatrix.fB / pAttr->oTextMatrix.fA ) / ( pAttr->oTextMatrix.fD - pAttr->oTextMatrix.fC * pAttr->oTextMatrix.fB / pAttr->oTextMatrix.fA );
		fX = ( fXPos - pAttr->oTextMatrix.fX - fY * pAttr->oTextMatrix.fC ) / pAttr->oTextMatrix.fA;
    }

    if ( OK != ( nRet = PageMoveTextPos( pPage, fX, fY ) ) )
        return nRet;

    return  PageShowText( pPage, sText, nLen, pCIDs, nLenCID );
}
unsigned long PageTextRect    (Page pPage, float fLeft, float fTop, float fRight, float fBottom, const char *sText, TextAlignment eAlign, unsigned int *pnLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    unsigned long nRet = PageCheckState( pPage, GMODE_TEXT_OBJECT);

    if ( OK != nRet )
        return nRet;
	    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

	const char *ptr = sText;

	float fSaveCharSpace = 0.0f;
    BOOL bIsInsufficientSpace = FALSE;
    BOOL bCharSpaceChanged = FALSE;

	if ( !pAttr->pGState->pFont ) 
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND, 0 );

	Box oBox = FontGetBBox( pAttr->pGState->pFont );

    if ( pnLen )
        *pnLen = 0;

    unsigned int nNumRest = UtilsStrLen( sText, LIMIT_MAX_STRING_LEN + 1 );

    if ( nNumRest > LIMIT_MAX_STRING_LEN ) 
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_STRING_OUT_OF_RANGE, 0 );
    else if ( !nNumRest )
        return OK;

    if ( 0 == pAttr->pGState->fTextLeading )
        PageSetTextLeading( pPage, ( oBox.fTop - oBox.fBottom ) / 1000 * pAttr->pGState->fFontSize );

    fTop    = fTop    - oBox.fTop    / 1000 * pAttr->pGState->fFontSize + pAttr->pGState->fTextLeading;
    fBottom = fBottom - oBox.fBottom / 1000 * pAttr->pGState->fFontSize;

	float fX = 0.0f;
    float fY = 0.0f;

    if ( 0 == pAttr->oTextMatrix.fA ) 
	{
        fY = ( fLeft - pAttr->oTextMatrix.fX ) / pAttr->oTextMatrix.fC;
        fX = ( fTop - pAttr->oTextMatrix.fY - ( fLeft - pAttr->oTextMatrix.fX ) * pAttr->oTextMatrix.fD / pAttr->oTextMatrix.fC ) / pAttr->oTextMatrix.fB;
    } 
	else 
	{
        fY = ( fTop - pAttr->oTextMatrix.fY - ( fLeft - pAttr->oTextMatrix.fX ) * pAttr->oTextMatrix.fB / pAttr->oTextMatrix.fA ) / ( pAttr->oTextMatrix.fD - pAttr->oTextMatrix.fC * pAttr->oTextMatrix.fB / pAttr->oTextMatrix.fA );
        fX = ( fLeft - pAttr->oTextMatrix.fX - fY * pAttr->oTextMatrix.fC ) / pAttr->oTextMatrix.fA;
    }

    if ( OK != ( nRet = PageMoveTextPos( pPage, fX, fY ) ) )
        return nRet;

    if ( TAlignJustify == eAlign ) 
	{
        fSaveCharSpace = pAttr->pGState->fCharSpace;
        pAttr->pGState->fCharSpace = 0;
    }

    for (;;) 
	{
        float fRealWidth = 0.0f;

        float fXAdjust = 0.0f;

        const char *pTempPointer = NULL;
        unsigned int nNumChar = 0;
        ParseTextRec pState;
        EncoderRecPtr pEncoder;
        unsigned int nIndex = 0;

        unsigned int nTempLen = PageMeasureText( pPage, ptr, pCIDs, nLenCID, fRight - fLeft, TRUE, &fRealWidth );
        
		if ( 0 == nTempLen ) 
		{
            bIsInsufficientSpace = TRUE;
            break;
        }

        if ( pnLen )
            *pnLen += nTempLen;

        switch ( eAlign ) 
		{
            case TAlignRight:

                fXAdjust = fRight - fLeft - fRealWidth;

                if ( OK != ( nRet = PageMoveTextPos( pPage, fXAdjust, 0 ) ) )
                    return nRet;

                if ( OK != ( nRet = InternalShowTextNextLine( pPage, (BYTE *)ptr, nTempLen, pCIDs, nLenCID ) ) )
                    return CheckError( pPage->oError );

                if ( OK != ( nRet = PageMoveTextPos( pPage, -fXAdjust, 0 ) ) )
                    return nRet;
                break;
            case TAlignCenter:

                fXAdjust = ( fRight - fLeft - fRealWidth ) / 2;
                
                if ( OK != ( nRet = PageMoveTextPos( pPage, fXAdjust, 0 ) ) )
                    return nRet;

                if ( OK != ( nRet = InternalShowTextNextLine( pPage, (BYTE *)ptr, nTempLen, pCIDs, nLenCID ) ) )
                    return CheckError( pPage->oError );

                if ( OK != ( nRet = PageMoveTextPos( pPage, -fXAdjust, 0 ) ) )
                    return nRet;
                break;
            case TAlignJustify:

                nNumChar = 0;
                pEncoder = ((FontAttr)pAttr->pGState->pFont->pAttr)->pEncoder;
                pTempPointer = ptr;
                EncoderSetParseText( pEncoder, &pState, (BYTE*)pTempPointer, nTempLen );
                while ( *pTempPointer ) 
				{
                    ByteType eByteType = EncoderByteType( pEncoder, &pState);

					if ( ByteTypeTrial != eByteType )
                        nNumChar++;

                    nIndex++;
                    if ( nIndex >= nTempLen )
                        break;

                    pTempPointer++;
                }

                if ( IS_WHITE_SPACE(*pTempPointer) )
                    nNumChar--;

                if ( nNumChar > 1 )
                    fXAdjust = ( fRight - fLeft - fRealWidth ) / ( nNumChar - 1 );
                else
                    fXAdjust = 0;

                if ( nNumRest == nTempLen ) 
				{
                    if ( OK != ( nRet = PageSetCharSpace( pPage, fSaveCharSpace ) ) )
                        return nRet;
                    bCharSpaceChanged = FALSE;
                } 
				else 
				{
                    if ( OK != ( nRet = PageSetCharSpace( pPage, fXAdjust ) ) )
                        return nRet;
                    bCharSpaceChanged = TRUE;
                }

                if ( OK != ( nRet = InternalShowTextNextLine( pPage, (BYTE *)ptr, nTempLen, pCIDs, nLenCID ) ) )
                    return CheckError( pPage->oError );

                pAttr->pGState->fCharSpace = 0;
                break;

			default:
                if ( OK != ( nRet = InternalShowTextNextLine( pPage, (BYTE *)ptr, nTempLen, pCIDs, nLenCID ) ) )
                    return CheckError( pPage->oError );
				break;
        }

        nNumRest -= nTempLen;
        if ( nNumRest <= 0 )
            break;

        if ( pAttr->oTextPos.fY - pAttr->pGState->fTextLeading < fBottom ) 
		{
            bIsInsufficientSpace = TRUE;
            break;
        }

        ptr += nTempLen;
    }

    if ( bCharSpaceChanged ) 
        if ( OK != ( nRet = PageSetCharSpace( pPage, fSaveCharSpace ) ) )
            return nRet;

    if ( bIsInsufficientSpace )
        return AVS_OFFICEPDFWRITER_ERROR_PAGE_INSUFFICIENT_SPACE;
    else
        return OK;
}

unsigned long PageSetSlideShow(Page pPage, TransitionStyle eType, float fDispTime, float fTransTime)
{
    unsigned long nRet = OK;

    if ( !PageValidate( pPage  ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE;

    if ( fDispTime < 0 )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_DISPLAY_TIME, (unsigned long)fDispTime);

    if ( fTransTime < 0 )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_TRANSITION_TIME, (unsigned long)fTransTime);

    Dict pDict = DictNew( pPage->oMMgr );

    if ( !pDict )
        return ErrorGetCode( pPage->oError );

    if ( OK != ( nRet = DictAddName( pDict, "Type", "Trans" ) ) )
	{
	    DictFree( pDict );
		return ErrorGetCode( pPage->oError );
	}

    if ( OK != ( nRet = DictAddReal( pDict, "D", fTransTime ) ) )
	{
	    DictFree( pDict );
		return ErrorGetCode( pPage->oError );
	}

    switch ( eType ) 
	{
        case TSWipeRight:
            nRet += DictAddName( pDict, "S", "Wipe");
            nRet += DictAddNumber( pDict, "Di", 0);
            break;
        case TSWipeUp:
            nRet += DictAddName( pDict, "S", "Wipe");
            nRet += DictAddNumber( pDict, "Di", 90);
            break;
        case TSWipeLeft:
            nRet += DictAddName( pDict, "S", "Wipe");
            nRet += DictAddNumber( pDict, "Di", 180);
            break;
        case TSWipeDown:
            nRet += DictAddName( pDict, "S", "Wipe");
            nRet += DictAddNumber( pDict, "Di", 270);
            break;
        case TSBarnDoorsHorizontalOut:
            nRet += DictAddName( pDict, "S", "Split");
            nRet += DictAddName( pDict, "Dm", "H");
            nRet += DictAddName( pDict, "M", "O");
            break;
        case TSBarnDoorsHorizontalIn:
            nRet += DictAddName( pDict, "S", "Split");
            nRet += DictAddName( pDict, "Dm", "H");
            nRet += DictAddName( pDict, "M", "I");
            break;
        case TSBarnDoorsVerticalOut:
            nRet += DictAddName( pDict, "S", "Split");
            nRet += DictAddName( pDict, "Dm", "V");
            nRet += DictAddName( pDict, "M", "O");
            break;
        case TSBarnDoorsVerticalIn:
            nRet += DictAddName( pDict, "S", "Split");
            nRet += DictAddName( pDict, "Dm", "V");
            nRet += DictAddName( pDict, "M", "I");
            break;
        case TSBoxOut:
            nRet += DictAddName( pDict, "S", "Box");
            nRet += DictAddName( pDict, "M", "O");
            break;
        case TSBoxIn:
            nRet += DictAddName( pDict, "S", "Box");
            nRet += DictAddName( pDict, "M", "I");
            break;
        case TSBlindsHorizontal:
            nRet += DictAddName( pDict, "S", "Blinds");
            nRet += DictAddName( pDict, "Dm", "H");
            break;
        case TSBlindsVertical:
            nRet += DictAddName( pDict, "S", "Blinds");
            nRet += DictAddName( pDict, "Dm", "V");
            break;
        case TSDissolve:
            nRet += DictAddName( pDict, "S", "Dissolve");
            break;
        case TSGlitterRight:
            nRet += DictAddName( pDict, "S", "Glitter");
            nRet += DictAddNumber( pDict, "Di", 0);
            break;
        case TSGlitterDown:
            nRet += DictAddName( pDict, "S", "Glitter");
            nRet += DictAddNumber( pDict, "Di", 270);
            break;
        case TSGlitterTopLeftToBottomRight:
            nRet += DictAddName( pDict, "S", "Glitter");
            nRet += DictAddNumber( pDict, "Di", 315);
            break;
        case TSReplace:
            nRet += DictAddName( pDict, "S", "R");
            break;
        default:
            nRet = AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE_SLIDESHOW_TYPE;
    }

    if ( OK != nRet )
	{
	    DictFree( pDict );
		return ErrorGetCode( pPage->oError );
	}

    if ( OK != ( nRet = DictAddReal( pPage, "Dur", fDispTime ) ) )
	{
	    DictFree( pDict );
		return ErrorGetCode( pPage->oError );
	}

    if ( OK != ( nRet = DictAdd( pPage, "Trans", pDict ) ) )
        return nRet;

    return OK;
}

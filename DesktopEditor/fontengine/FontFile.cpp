#include "FontManager.h"
#include "internal/internal.h"
#include "internal/ftobjs.h"
#include "../common/Types.h"
#include "../common/String.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

CFontFile::CFontFile()
{
	m_pStream = NULL;
	m_pFontManager = NULL;
	m_pDefaultFont = NULL;
	ClearCache();

	memset(m_arrdFontMatrix, 0, 6 * sizeof(double));
	memset(m_arrdTextMatrix, 0, 6 * sizeof(double));

    m_bAntiAliasing = TRUE;
    m_bUseKerning = FALSE;

    m_dSize = 0;
    m_unHorDpi = 0;
    m_unVerDpi = 0;

    m_bNeedDoItalic = FALSE;
    m_bNeedDoBold = FALSE;

    m_dCharSpacing = 0;

    m_nMinX = 0;
    m_nMinY = 0;
    m_nMaxX = 0;
    m_nMaxY = 0;
    
	m_lFaceIndex = 0;
	
	m_pFace = NULL;
	
	m_dUnitsKoef = 0;
	m_nDefaultChar = -1;
	m_nSymbolic = -1;
	m_dTextScale = 1.0;

    m_bStringGID = FALSE;
	
	m_nNum_charmaps = 0;

    m_lAscender = 0;
    m_lDescender = 0;
    m_lLineHeight = 0;
    m_lUnits_Per_Em = 0;

    m_bUseDefaultFont = FALSE;
    m_bIsNeedUpdateMatrix12 = FALSE;

	m_bHintsSupport = TRUE;
}

CFontFile::~CFontFile()
{
	RELEASEINTERFACE(m_pStream);
	ClearCache();
}

void CFontFile::SetDefaultFont(CFontFile* pDefFont)
{
    m_pDefaultFont = pDefFont;
}

void CFontFile::LoadDefaultCharAndSymbolicCmapIndex()
{
    m_nDefaultChar = -1;
    m_nSymbolic = -1;
    
	TT_OS2* pTable = (TT_OS2*)FT_Get_Sfnt_Table( m_pFace, ft_sfnt_os2 );
	if (NULL == pTable)
		return;

    m_nDefaultChar = pTable->usDefaultChar;
		
    // version
    if (0xFFFF == pTable->version)
        return;

	FT_ULong ulCodePageRange1 = pTable->ulCodePageRange1;
    FT_ULong ulCodePageRange2 = pTable->ulCodePageRange2;
    if (!(ulCodePageRange1 & 0x80000000) && !(ulCodePageRange1 == 0 && ulCodePageRange2 == 0))
        return;

	for (int nIndex = 0; nIndex < m_pFace->num_charmaps; ++nIndex)
	{
		// Symbol
		if ( 0 == m_pFace->charmaps[nIndex]->encoding_id && 3 == m_pFace->charmaps[nIndex]->platform_id )
		{
			m_nSymbolic = nIndex;
			break;
		}
	}
}

void CFontFile::ResetFontMatrix()
{
    if (m_pDefaultFont)
        m_pDefaultFont->ResetFontMatrix();

	if ( m_bNeedDoItalic )
	{
		m_arrdFontMatrix[0] = 1;
		m_arrdFontMatrix[1] = 0;
		m_arrdFontMatrix[2] = FONT_ITALIC_ANGLE;
		m_arrdFontMatrix[3] = 1;
		m_arrdFontMatrix[4] = 0;
		m_arrdFontMatrix[5] = 0;
	}
	else
	{
		m_arrdFontMatrix[0] = 1;
		m_arrdFontMatrix[1] = 0;
		m_arrdFontMatrix[2] = 0;
		m_arrdFontMatrix[3] = 1;
		m_arrdFontMatrix[4] = 0;
		m_arrdFontMatrix[5] = 0;
	}

	UpdateMatrix0();
}

void CFontFile::ResetTextMatrix()
{
	m_arrdTextMatrix[0] = 1;
	m_arrdTextMatrix[1] = 0;
	m_arrdTextMatrix[2] = 0;
	m_arrdTextMatrix[3] = 1;
	m_arrdTextMatrix[4] = 0;
	m_arrdTextMatrix[5] = 0;

	CheckTextMatrix();
}

void CFontFile::CheckTextMatrix()
{
    m_bIsNeedUpdateMatrix12 = true;
    if ((m_arrdTextMatrix[0] == 1) && (m_arrdTextMatrix[1] == 0) && (m_arrdTextMatrix[2] == 0) && (m_arrdTextMatrix[3] == 1))
    {
        m_bIsNeedUpdateMatrix12 = false;

        if (m_pDefaultFont)
            m_pDefaultFont->UpdateMatrix1();

        UpdateMatrix1();
    }
}

void CFontFile::UpdateMatrix0()
{
    double m1 = m_arrdTextMatrix[2];
    double m2 = m_arrdTextMatrix[3];
	m_dTextScale = sqrt(m_arrdTextMatrix[2]*m_arrdTextMatrix[2] + m_arrdTextMatrix[3]*m_arrdTextMatrix[3]);
	
	FT_BBox* bbox = &m_pFace->bbox;
	FT_Pos xMin = bbox->xMin;
	FT_Pos yMin = bbox->yMin;
	FT_Pos xMax = bbox->xMax;
	FT_Pos yMax = bbox->yMax;

    if (m_lUnits_Per_Em == 0)
        m_lUnits_Per_Em = m_pFace->units_per_EM = 2048;
	
	int units_per_EM = m_lUnits_Per_Em;
	double dDiv = xMax > 20000 ? 65536 : 1;
    double del = dDiv * units_per_EM;

	int nX = (int)((m_arrdFontMatrix[0] * xMin + m_arrdFontMatrix[2] * yMin) * m_dSize / del);
	m_nMinX = m_nMaxX = nX;

	int nY = (int)((m_arrdFontMatrix[1] * xMin + m_arrdFontMatrix[3] * yMin) * m_dSize / del);
	m_nMinY = m_nMaxY = nY;

	nX = (int)((m_arrdFontMatrix[0] * xMin + m_arrdFontMatrix[2] * yMax) * m_dSize / del);

	if (nX < m_nMinX)
		m_nMinX = nX;
	else if (nX > m_nMaxX)
		m_nMaxX = nX;

	nY = (int)((m_arrdFontMatrix[1] * xMin + m_arrdFontMatrix[3] * yMax) * m_dSize / del);

	if (nY < m_nMinY)
		m_nMinY = nY;
	else if (nY > m_nMaxY)
		m_nMaxY = nY;

	nX = (int)((m_arrdFontMatrix[0] * xMax + m_arrdFontMatrix[2] * yMin) * m_dSize / del);
	if (nX < m_nMinX)
		m_nMinX = nX;
	else if (nX > m_nMaxX)
		m_nMaxX = nX;

	nY = (int)((m_arrdFontMatrix[1] * xMax + m_arrdFontMatrix[3] * yMin) * m_dSize / del);
	if (nY < m_nMinY)
		m_nMinY = nY;
	else if (nY > m_nMaxY)
		m_nMaxY = nY;

	nX = (int)((m_arrdFontMatrix[0] * xMax + m_arrdFontMatrix[2] * yMax) * m_dSize / del);
	if (nX < m_nMinX)
		m_nMinX = nX;
	else if (nX > m_nMaxX)
		m_nMaxX = nX;

	nY = (int)((m_arrdFontMatrix[1] * xMax + m_arrdFontMatrix[3] * yMax) * m_dSize / del);
	if (nY < m_nMinY)
		m_nMinY = nY;
	else if (nY > m_nMaxY)
		m_nMaxY = nY;

	// This is a kludge: some buggy PDF generators embed fonts with  zero bounding boxes.
	if (m_nMaxX == m_nMinX)
	{
		m_nMinX = 0;
		m_nMaxX = (int)(m_dSize);
	}

	if (m_nMaxY == m_nMinY)
	{
		m_nMinY = 0;
		m_nMaxY = (int)((1.2 * m_dSize));
	}

	m_oFontMatrix.xx = (FT_Fixed)(m_arrdFontMatrix[0] * 65536);
	m_oFontMatrix.yx = (FT_Fixed)(m_arrdFontMatrix[1] * 65536);
	m_oFontMatrix.xy = (FT_Fixed)(m_arrdFontMatrix[2] * 65536);
	m_oFontMatrix.yy = (FT_Fixed)(m_arrdFontMatrix[3] * 65536);

	m_oTextMatrix.xx = (FT_Fixed)((m_arrdTextMatrix[0] / m_dTextScale) * 65536);
	m_oTextMatrix.yx = (FT_Fixed)((m_arrdTextMatrix[1] / m_dTextScale) * 65536);
	m_oTextMatrix.xy = (FT_Fixed)((m_arrdTextMatrix[2] / m_dTextScale) * 65536);
	m_oTextMatrix.yy = (FT_Fixed)((m_arrdTextMatrix[3] / m_dTextScale) * 65536);

	FT_Set_Transform( m_pFace, &m_oFontMatrix, NULL );
}

void CFontFile::UpdateMatrix1()
{
    m_oFontMatrix.xx = (FT_Fixed)(m_arrdFontMatrix[0] * 65536);
	m_oFontMatrix.yx = (FT_Fixed)(m_arrdFontMatrix[1] * 65536);
	m_oFontMatrix.xy = (FT_Fixed)(m_arrdFontMatrix[2] * 65536);
	m_oFontMatrix.yy = (FT_Fixed)(m_arrdFontMatrix[3] * 65536);

	FT_Set_Transform( m_pFace, &m_oFontMatrix, NULL );
}

void CFontFile::UpdateMatrix2()
{
	m_oFontMatrix.xx = (FT_Fixed)(( m_arrdFontMatrix[0] * m_arrdTextMatrix[0] + m_arrdFontMatrix[1] * m_arrdTextMatrix[2] ) * 65536);
	m_oFontMatrix.yx = (FT_Fixed)(( m_arrdFontMatrix[0] * m_arrdTextMatrix[1] + m_arrdFontMatrix[1] * m_arrdTextMatrix[3] ) * 65536);
	m_oFontMatrix.xy = (FT_Fixed)(( m_arrdFontMatrix[2] * m_arrdTextMatrix[0] + m_arrdFontMatrix[3] * m_arrdTextMatrix[2] ) * 65536);
	m_oFontMatrix.yy = (FT_Fixed)(( m_arrdFontMatrix[2] * m_arrdTextMatrix[1] + m_arrdFontMatrix[3] * m_arrdTextMatrix[3] ) * 65536);

	FT_Set_Transform( m_pFace, &m_oFontMatrix, NULL );
}

void CFontFile::SetSizeAndDpi(double dSize, UINT unHorDpi, UINT unVerDpi)
{
    if (m_pDefaultFont)
        m_pDefaultFont->SetSizeAndDpi(dSize, unHorDpi, unVerDpi);

	double dOldSize = m_dSize;
	double dNewSize = dSize;
	double fKoef = dNewSize / dOldSize;

	if (fKoef > 1.001 || fKoef < 0.999 || unHorDpi != m_unHorDpi || unVerDpi != m_unVerDpi)
	{
		m_unHorDpi = unHorDpi;
		m_unVerDpi = unVerDpi;

		if (fKoef > 1.001 || fKoef < 0.999)
		{
			m_dSize = dNewSize;
			UpdateMatrix0();
		}

		m_dUnitsKoef = m_unHorDpi / 72.0 * m_dSize;

		FT_Set_Char_Size(m_pFace, 0, (int)(dNewSize * 64), unHorDpi, unVerDpi);		
		ClearCache();
	}
}

void CFontFile::ClearCache()
{
	// TODO: total memory
    ClearCacheNoAttack(true);
}
void CFontFile::ClearCacheNoAttack(bool bIsFree)
{
    m_oCacheSizes.Clear(bIsFree);
	::memset( m_arrCacheSizesIndexs, 0xFF, FONT_CACHE_SIZES_INDEXES_SIZE_2 );
}

void CFontFile::Destroy()
{
	// TODO: total memory
}

bool CFontFile::SetTextMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF)
{
    bool b1 = (m_arrdTextMatrix[0] ==  fA && m_arrdTextMatrix[1] == -fB && m_arrdTextMatrix[2] == -fC && m_arrdTextMatrix[3] ==  fD);
    if (b1 && m_arrdTextMatrix[4] ==  fE && m_arrdTextMatrix[5] ==  fF)
        return false;

    if (m_pDefaultFont)
        m_pDefaultFont->SetTextMatrix(fA, fB, fC, fD, fE, fF);

	m_arrdTextMatrix[0] =  fA;
	m_arrdTextMatrix[1] = -fB;
	m_arrdTextMatrix[2] = -fC;
	m_arrdTextMatrix[3] =  fD;
	m_arrdTextMatrix[4] =  fE;
	m_arrdTextMatrix[5] =  fF;

    if (!b1)
    {
        ClearCache();
    }
    CheckTextMatrix();
	return true;
}

void CFontFile::SetFontMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF)
{
    if (m_pDefaultFont)
        m_pDefaultFont->SetFontMatrix(fA, fB, fC, fD, fE, fF);

    if (m_bNeedDoItalic)
    {
        m_arrdFontMatrix[0] = fA;
        m_arrdFontMatrix[1] = fB;
        m_arrdFontMatrix[2] = fC + fA * FONT_ITALIC_ANGLE;
        m_arrdFontMatrix[3] = fD + fB * FONT_ITALIC_ANGLE;
        m_arrdFontMatrix[4] = fE;
        m_arrdFontMatrix[5] = fF;
    }
    else
    {
        m_arrdFontMatrix[0] = fA;
        m_arrdFontMatrix[1] = fB;
        m_arrdFontMatrix[2] = fC;
        m_arrdFontMatrix[3] = fD;
        m_arrdFontMatrix[4] = fE;
        m_arrdFontMatrix[5] = fF;
    }

    ClearCache();
}

int CFontFile::GetKerning(UINT unPrevGID, UINT unGID)
{
	FT_Vector delta;
    FT_Get_Kerning(m_pFace, unPrevGID, unGID, 0, &delta);
    return (delta.x >> 6);
}

void CFontFile::SetStringGID(const INT& bGID)
{
    if (m_bStringGID == bGID)
        return;

    //ClearCache();
    m_bStringGID = bGID;
}
INT CFontFile::GetStringGID()
{
    return m_bStringGID;
}
void CFontFile::SetUseDefaultFont(const INT& bUse)
{
    m_bUseDefaultFont = bUse;
}
INT CFontFile::GetUseDefaultFont()
{
    return m_bUseDefaultFont;
}
void CFontFile::SetCharSpacing(const double& dCharSpacing)
{
    m_dCharSpacing = dCharSpacing;
}
double CFontFile::GetCharSpacing()
{
    return m_dCharSpacing;
}

std::string CFontFile::GetStyleName()
{
	std::string s(m_pFace->style_name);
	return s;
}

void CFontFile::GetPanose(BYTE* pData)
{
    memset(pData, 0, 10);
    if (!m_pFace)
        return;

    TT_OS2 *pTable = (TT_OS2 *)FT_Get_Sfnt_Table( m_pFace, ft_sfnt_os2 );
    if ( NULL == pTable )
        return;

    memcpy( pData, pTable->panose, 10 );
}

bool CFontFile::IsFixedWidth()
{
    if (!m_pFace)
        return false;
    return FT_IS_FIXED_WIDTH( m_pFace ) != 0;
}

int CFontFile::IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex)
{
    if (!m_pFace)
        return -1;

    TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( m_pFace, ft_sfnt_os2 );
    if ( NULL == pOs2 || 0xFFFF == pOs2->version )
        return -1;

    int nResult = 0;

    unsigned long ulMult = 1;
    for ( unsigned long ulIndex = 0; ulIndex < ulBit; ulIndex++ )
        ulMult <<= 1;

    switch(un4ByteIndex)
    {
    case 0: if ( pOs2->ulUnicodeRange1  & ulMult ) nResult = 1; break;
    case 1: if ( pOs2->ulUnicodeRange2  & ulMult ) nResult = 1; break;
    case 2: if ( pOs2->ulUnicodeRange3  & ulMult ) nResult = 1; break;
    case 3: if ( pOs2->ulUnicodeRange4  & ulMult ) nResult = 1; break;
    case 4: if ( pOs2->ulCodePageRange1 & ulMult ) nResult = 1; break;
    case 5: if ( pOs2->ulCodePageRange2 & ulMult ) nResult = 1; break;
    }

    // Специальная ветка для случаев, когда charset может быть задан не через значения
    // ulCodePageRange, а непосредственно через тип Cmap.

    //  Charset Name       Charset Value(hex)  Codepage number   Platform_ID   Encoding_ID   Description
    //  -------------------------------------------------------------------------------------------------
    //
    //  SYMBOL_CHARSET            2 (x02)                             3            0           Symbol
    //  SHIFTJIS_CHARSET        128 (x80)             932             3            2           ShiftJIS
    //  GB2313_CHARSET          134 (x86)             936             3            3           PRC
    //  CHINESEBIG5_CHARSET     136 (x88)             950             3            4           Big5
    //  HANGEUL_CHARSET         129 (x81)             949             3            5           Wansung
    //  JOHAB_CHARSET	        130 (x82)            1361             3            6           Johab

    if ( 4 == un4ByteIndex && 0 == nResult )
    {
        for( int nIndex = 0; nIndex < m_pFace->num_charmaps; nIndex++ )
        {
            // Symbol
            if ( 31 == ulBit && 0 == m_pFace->charmaps[nIndex]->encoding_id && 3 == m_pFace->charmaps[nIndex]->platform_id )
            {
                nResult = 1;
                break;
            }

            // ShiftJIS
            if ( 17 == ulBit && 2 == m_pFace->charmaps[nIndex]->encoding_id && 3 == m_pFace->charmaps[nIndex]->platform_id )
            {
                nResult = 1;
                break;
            }

            // PRC
            if ( 18 == ulBit && 3 == m_pFace->charmaps[nIndex]->encoding_id && 3 == m_pFace->charmaps[nIndex]->platform_id )
            {
                nResult = 1;
                break;
            }

            // Big5
            if ( 20 == ulBit && 4 == m_pFace->charmaps[nIndex]->encoding_id && 3 == m_pFace->charmaps[nIndex]->platform_id )
            {
                nResult = 1;
                break;
            }

            // Wansung
            if ( 19 == ulBit && 5 == m_pFace->charmaps[nIndex]->encoding_id && 3 == m_pFace->charmaps[nIndex]->platform_id )
            {
                nResult = 1;
                break;
            }

            // Johab
            if ( 21 == ulBit && 6 == m_pFace->charmaps[nIndex]->encoding_id && 3 == m_pFace->charmaps[nIndex]->platform_id )
            {
                nResult = 1;
                break;
            }
        }
    }

    return nResult;
}

void CFontFile::UpdateStyles(const INT& bBold, const INT& bItalic)
{
	std::string sStyle = GetStyleName();

    // Смотрим какой стиль у исходного шрифта
    INT bSrcBold   = (-1 != sStyle.find("Bold"));
    INT bSrcItalic = (-1 != sStyle.find("Italic"));

    if (!bBold) // Нам нужен не жирный шрифт
    {
        m_bNeedDoBold = false;
    }
    else if (bBold) // Нам нужно сделать шрифт жирным
    {
        if (bSrcBold)
        {
            // Исходный шрифт уже жирный, поэтому ничего дополнительного делать не надо
            m_bNeedDoBold = false;
        }
        else
        {
            // Иходный шрифт не жирный, поэтому жирность делаем сами
            m_bNeedDoBold = true;
        }
    }

    if (!bItalic) // Нам нужен не наклонный шрифт
    {
        SetItalic(false);
    }
    else if (bItalic) // Нам нужно сделать наклонный шрифт
    {
        if (bSrcItalic)
        {
            // Исходный шрифт уже наклонный, поэтому ничего дополнительного делать не надо
            SetItalic(false);
        }
        else
        {
            // Иходный шрифт не наклонный, поэтому делаем его наклонным сами
            SetItalic(true);
        }
    }
}

void CFontFile::SetItalic(const INT& value)
{
    if (m_bNeedDoItalic != value)
    {
        ClearCache();
        m_bNeedDoItalic = value;
        ResetFontMatrix();
    }
}
void CFontFile::SetNeedBold(const INT& value)
{
    if (m_bNeedDoBold != value)
        ClearCache();

    m_bNeedDoBold = value;
}

int CFontFile::GetAscender()
{
    return m_lAscender;
}
int CFontFile::GetDescender()
{
    return m_lDescender;
}
int CFontFile::GetHeight()
{
    return m_lLineHeight;
}
int CFontFile::Units_Per_Em()
{
    return m_lUnits_Per_Em;
}

void CFontFile::CheckHintsSupport()
{
    m_bHintsSupport = TRUE;

    if (!m_pFace || !m_pFace->driver || !m_pFace->driver->clazz)
        return;

	std::string sName(m_pFace->driver->clazz->root.module_name); 

    if (sName != "truetype")
    {
        m_bHintsSupport = FALSE;
        return;
    }

	std::string sFamilyName((NULL != m_pFace->family_name) ? m_pFace->family_name : "");

    if (sFamilyName == "MS Mincho" || sFamilyName == "Castellar")
        m_bHintsSupport = FALSE;
}

// glyph methods
int CFontFile::SetCMapForCharCode(long lUnicode, int *pnCMapIndex)
{
	*pnCMapIndex = -1;
	if (!m_pFace)
		return 0;

	if ( m_bStringGID || 0 == m_pFace->num_charmaps )
		return lUnicode;

	int nCharIndex = 0;

	for ( int nIndex = 0; nIndex < m_pFace->num_charmaps; ++nIndex )
	{
		FT_CharMap pCharMap = m_pFace->charmaps[nIndex];

		if ( FT_Set_Charmap( m_pFace, pCharMap ) )
			continue;

		FT_Encoding pEncoding = pCharMap->encoding;

		if ( FT_ENCODING_UNICODE == pEncoding )
		{
			if ( nCharIndex = FT_Get_Char_Index( m_pFace, lUnicode ) )
			{
				*pnCMapIndex = nIndex;
				return nCharIndex;
			}
		}
		else if ( FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding )
		{
#if 0
			FT_ULong  charcode;
			FT_UInt   gindex;

			charcode = FT_Get_First_Char( m_pFace, &gindex );                   
			while ( gindex != 0 )                                            
			{                                                               
				charcode = FT_Get_Next_Char( m_pFace, charcode, &gindex );       
				if ( charcode == lUnicode )
				{
					nCharIndex = gindex;
					*pnCMapIndex = nIndex;
					break;
				}
			}   
#endif

			if ( nCharIndex = FT_Get_Char_Index( m_pFace, lUnicode ) )
			{
				*pnCMapIndex = nIndex;
			}
		}
	}

	return nCharIndex;
}

int CFontFile::SetCMapForCharCode2(long lUnicode)
{
	if (m_bStringGID)
		return lUnicode;

	FT_Int unGID;
	int nCMapIndex = 0;

    int nCacheIndex = 0xFFFF;
    if (lUnicode < FONT_CACHE_SIZES_INDEXES_SIZE) //linux !!!
    {
        nCacheIndex = m_arrCacheSizesIndexs[lUnicode];
    }
	if ( 0xFFFF == nCacheIndex )
	{
		return unGID = SetCMapForCharCode( lUnicode, &nCMapIndex );
	}
	else
	{
		TFontCacheSizes& oSizes = m_oCacheSizes[nCacheIndex];
		unGID = oSizes.ushGID;
		nCMapIndex = oSizes.nCMapIndex;
		if ( 0 != m_pFace->num_charmaps )
		{
			int nCurCMapIndex = FT_Get_Charmap_Index( m_pFace->charmap );
			if ( nCurCMapIndex != nCMapIndex )
			{
				nCMapIndex = max( 0, nCMapIndex );
				FT_Set_Charmap( m_pFace, m_pFace->charmaps[nCMapIndex] );
			}
		}
	}

	return unGID;
}

bool CFontFile::AddToSizesCache(const TFontCacheSizes& oSizes)
{
    if (oSizes.ushUnicode >= FONT_CACHE_SIZES_INDEXES_SIZE) return false;  //oSizes.ushUnicode = 4784212 !!! linux
	m_arrCacheSizesIndexs[oSizes.ushUnicode] = m_oCacheSizes.Add( oSizes );

    return true;
}

TFontCacheSizes CFontFile::GetChar(LONG lUnicode)
{
	FT_Face pFace				= m_pFace;
	FT_GlyphSlot pCurentGliph	= pFace->glyph;
    
    TFontCacheSizes oSizes;
    LONG ushUnicode = lUnicode;

    // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
    if (m_bIsNeedUpdateMatrix12)
    {
        if (m_pDefaultFont)
            m_pDefaultFont->UpdateMatrix1();
        UpdateMatrix1();
    }

    LONG unGID = 0;

    USHORT charSymbolObj = 0xFFFF;

    if (ushUnicode < FONT_CACHE_SIZES_INDEXES_SIZE)     //вылетает под Linux
    {
        charSymbolObj = m_arrCacheSizesIndexs[ushUnicode];
    }
    if (0xFFFF == charSymbolObj)
    {
        int nCMapIndex = 0;
        unGID = SetCMapForCharCode(ushUnicode, &nCMapIndex);

        oSizes.ushUnicode = ushUnicode;
        if (!((unGID > 0) || (-1 != m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = SetCMapForCharCode(ushUnicode + 0xF000, &nCMapIndex)))))
        {
            // Пробуем загрузить через стандартный шрифт
            if ((FALSE == m_bUseDefaultFont) || (NULL == m_pDefaultFont) || (0 >= (unGID = m_pDefaultFont->SetCMapForCharCode(ushUnicode, &nCMapIndex))))
			{
                if (m_nDefaultChar < 0)
                {
                    oSizes.ushGID		= -1;
                    oSizes.eState		= glyphstateMiss;
                    oSizes.fAdvanceX	= (pFace->size->metrics.max_advance >> 6) / 2.0f;
					oSizes.fAdvanceY    = oSizes.fAdvanceX;

                    return oSizes;
                }
                else
                {
                    unGID			= m_nDefaultChar;
                    oSizes.eState	= glyphstateNormal;

                    pFace			= m_pFace;
					pCurentGliph	= pFace->glyph;
                }
            }
            else
            {
                oSizes.eState = glyphstateDeafault;

                pFace = m_pDefaultFont->m_pFace;
				pCurentGliph = pFace->glyph;
            }
        }
        else
        {
            oSizes.eState = glyphstateNormal;
        }

        oSizes.ushGID     = (USHORT)unGID;
        oSizes.nCMapIndex = nCMapIndex;

        FT_Int32 _LOAD_MODE = m_bHintsSupport ? m_pFontManager->m_nLOAD_MODE : 40970;
        if (0 != FT_Load_Glyph(pFace, unGID, _LOAD_MODE))
            return oSizes;

		FT_Glyph pGlyph = NULL;
		if ( FT_Get_Glyph( pFace->glyph, &pGlyph ) )
		{
			return oSizes;
		}

		FT_BBox oBBox;
		FT_Glyph_Get_CBox( pGlyph, ft_glyph_bbox_gridfit, &oBBox );
		FT_Done_Glyph( pGlyph );

		oSizes.fAdvanceX = (float)(pFace->glyph->linearHoriAdvance * m_dUnitsKoef / pFace->units_per_EM);
		oSizes.fAdvanceY = (float)(pFace->glyph->linearVertAdvance * m_dUnitsKoef / pFace->units_per_EM);
		oSizes.oBBox.fMinX = (float)(oBBox.xMin >> 6);
		oSizes.oBBox.fMaxX = (float)(oBBox.xMax >> 6);
		oSizes.oBBox.fMinY = (float)(oBBox.yMin >> 6);
		oSizes.oBBox.fMaxY = (float)(oBBox.yMax >> 6);

		oSizes.oMetrics.fHeight       = (float)(pFace->glyph->metrics.height       >> 6);
		oSizes.oMetrics.fHoriAdvance  = (float)(pFace->glyph->metrics.horiAdvance  >> 6);
		oSizes.oMetrics.fHoriBearingX = (float)(pFace->glyph->metrics.horiBearingX >> 6);
		oSizes.oMetrics.fHoriBearingY = (float)(pFace->glyph->metrics.horiBearingY >> 6);
		oSizes.oMetrics.fVertAdvance  = (float)(pFace->glyph->metrics.vertAdvance  >> 6);
		oSizes.oMetrics.fVertBearingX = (float)(pFace->glyph->metrics.vertBearingX >> 6);
		oSizes.oMetrics.fVertBearingY = (float)(pFace->glyph->metrics.vertBearingY >> 6);
		oSizes.oMetrics.fWidth        = (float)(pFace->glyph->metrics.width        >> 6);

		oSizes.bBitmap = false;
		oSizes.oBitmap.nX        = 0;
		oSizes.oBitmap.nY        = 0;
		oSizes.oBitmap.nHeight   = 0;
		oSizes.oBitmap.nWidth    = 0;
		oSizes.oBitmap.bFreeData = FALSE;
		oSizes.oBitmap.pData     = NULL;
		oSizes.oBitmap.bAA       = FALSE;

		if (m_bNeedDoBold)
            oSizes.fAdvanceX += 1;

        AddToSizesCache( oSizes );
    }
    else
    {
        oSizes = m_oCacheSizes[charSymbolObj];
    }

    if (m_bIsNeedUpdateMatrix12)
    {
        if (m_pDefaultFont)
            m_pDefaultFont->UpdateMatrix2();
       UpdateMatrix2();
    }

    return oSizes;
}

INT CFontFile::GetString(CGlyphString& oString)
{
	int nCountGlyph = oString.GetLength();
	if (nCountGlyph <= 0)
		return TRUE;

	unsigned int unPrevGID = 0;
	float fPenX = 0, fPenY = 0;

	for (int nIndex = 0; nIndex < nCountGlyph; ++nIndex)
	{
        // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
        if (m_bIsNeedUpdateMatrix12)
        {
            if (m_pDefaultFont)
                m_pDefaultFont->UpdateMatrix1();
            UpdateMatrix1();
        }

        FT_Face pFace = m_pFace;
		FT_GlyphSlot pCurentGliph = pFace->glyph;
        
		TGlyph* pCurGlyph = oString.GetAt(nIndex);

		int ushUnicode = pCurGlyph->lUnicode;
		
		int unGID = 0;
        USHORT charSymbolObj = 0xFFFF;

        if (ushUnicode >= 0 && ushUnicode >= 0 && ushUnicode < FONT_CACHE_SIZES_INDEXES_SIZE)     //вылетает под Linux
        {
            charSymbolObj = m_arrCacheSizesIndexs[ushUnicode];
        }
		if (0xFFFF == charSymbolObj)
		{
            int nCMapIndex = 0;
            unGID = SetCMapForCharCode(ushUnicode, &nCMapIndex);

            TFontCacheSizes oSizes;
            oSizes.ushUnicode = ushUnicode;

            if (!((unGID > 0) || (-1 != m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = SetCMapForCharCode(ushUnicode + 0xF000, &nCMapIndex)))))
            {
                // Пробуем загрузить через стандартный шрифт
                if (FALSE == m_bUseDefaultFont || NULL == m_pDefaultFont || 0 >= (unGID = m_pDefaultFont->SetCMapForCharCode(ushUnicode, &nCMapIndex)))
                {
                    if (m_nDefaultChar < 0)
                    {
                        oSizes.ushGID    = -1;
                        oSizes.eState    = glyphstateMiss;
                        oSizes.fAdvanceX = (m_pFace->size->metrics.max_advance >> 6) / 2.0f;

                        return FALSE;
                    }
                    else
                    {
                        unGID = m_nDefaultChar;
                        oSizes.eState = glyphstateNormal;

                        pFace = m_pFace;
                        pCurentGliph = pFace->glyph;
                    }
                }
                else
                {
                    oSizes.eState = glyphstateDeafault;

                    pFace = m_pDefaultFont->m_pFace;
                    pCurentGliph = pFace->glyph;
                }
            }
            else
            {
                oSizes.eState = glyphstateNormal;
            }

            oSizes.ushGID     = unGID;
            oSizes.nCMapIndex = nCMapIndex;

            if (m_bIsNeedUpdateMatrix12)
            {
                if (m_pDefaultFont)
                    m_pDefaultFont->UpdateMatrix2();
                UpdateMatrix2();
            }

			FT_Int32 _LOAD_MODE = m_bHintsSupport ? m_pFontManager->m_nLOAD_MODE : 40970;
            if (0 != FT_Load_Glyph(pFace, unGID, _LOAD_MODE))
                return FALSE;

			FT_Glyph pGlyph = NULL;
			if ( FT_Get_Glyph( pFace->glyph, &pGlyph ) )
                return FALSE;

            FT_BBox oBBox;

            FT_Glyph_Get_CBox(pGlyph, 1, &oBBox);
            FT_Done_Glyph(pGlyph);

            pCurentGliph = pFace->glyph;

            oSizes.fAdvanceX = (float)(pFace->glyph->linearHoriAdvance * m_dUnitsKoef / pFace->units_per_EM);
			oSizes.oBBox.fMinX = (float)(oBBox.xMin >> 6);
			oSizes.oBBox.fMaxX = (float)(oBBox.xMax >> 6);
			oSizes.oBBox.fMinY = (float)(oBBox.yMin >> 6);
			oSizes.oBBox.fMaxY = (float)(oBBox.yMax >> 6);

			oSizes.oMetrics.fHeight       = (float)(pFace->glyph->metrics.height       >> 6);
			oSizes.oMetrics.fHoriAdvance  = (float)(pFace->glyph->metrics.horiAdvance  >> 6);
			oSizes.oMetrics.fHoriBearingX = (float)(pFace->glyph->metrics.horiBearingX >> 6);
			oSizes.oMetrics.fHoriBearingY = (float)(pFace->glyph->metrics.horiBearingY >> 6);
			oSizes.oMetrics.fVertAdvance  = (float)(pFace->glyph->metrics.vertAdvance  >> 6);
			oSizes.oMetrics.fVertBearingX = (float)(pFace->glyph->metrics.vertBearingX >> 6);
			oSizes.oMetrics.fVertBearingY = (float)(pFace->glyph->metrics.vertBearingY >> 6);
			oSizes.oMetrics.fWidth        = (float)(pFace->glyph->metrics.width        >> 6);

            oSizes.bBitmap = false;
			oSizes.oBitmap.nX        = 0;
			oSizes.oBitmap.nY        = 0;
			oSizes.oBitmap.nHeight   = 0;
			oSizes.oBitmap.nWidth    = 0;
			oSizes.oBitmap.bFreeData = FALSE;
			oSizes.oBitmap.pData     = NULL;
			oSizes.oBitmap.bAA       = FALSE;

			if (m_bNeedDoBold)
				oSizes.fAdvanceX += 1;

            if ( AddToSizesCache( oSizes ) == true)
            {
                charSymbolObj = m_arrCacheSizesIndexs[oSizes.ushUnicode];
            }
		}
		if (0xFFFF != charSymbolObj)
		{
			TFontCacheSizes& oSizes = m_oCacheSizes[charSymbolObj];
			int nCMapIndex = oSizes.nCMapIndex;

			if (glyphstateMiss == oSizes.eState)
            {
                oString.SetStartPoint (nIndex, fPenX, fPenY);
                oString.SetBBox(nIndex, 0, 0, 0, 0);
                oString.SetState (nIndex, glyphstateMiss);

				double dAdvanceW = m_arrdFontMatrix[0] * oSizes.fAdvanceX;
				fPenX += (float)(dAdvanceW + m_dCharSpacing);

                unPrevGID = 0;

                continue;
            }
            else if (glyphstateDeafault == oSizes.eState)
            {
                oString.SetState(nIndex, glyphstateDeafault);
            }
            else
            {
                oString.SetState(nIndex, glyphstateNormal);
            }

            if ( 0 != pFace->num_charmaps )
			{
				int nCurCMapIndex = FT_Get_Charmap_Index( pFace->charmap );
				if ( nCurCMapIndex != nCMapIndex )
				{
					nCMapIndex = max( 0, nCMapIndex );
					FT_Set_Charmap( pFace, pFace->charmaps[nCMapIndex] );
				}
			}

            if (m_bUseKerning && unPrevGID && (nIndex >= 0 && oString.GetAt(nIndex)->eState == oString.GetAt(nIndex - 1)->eState))
            {
                fPenX += GetKerning(unPrevGID, unGID);
            }

            float fX = oString.m_fX + fPenX;
            float fY = oString.m_fY + fPenY;

            float fXX = (float)(oString.m_arrCTM[4] + fX * oString.m_arrCTM[0] + fY * oString.m_arrCTM[2] - oString.m_fX);
            float fYY = (float)(oString.m_arrCTM[5] + fX * oString.m_arrCTM[1] + fY * oString.m_arrCTM[3] - oString.m_fY);

            oString.SetStartPoint(nIndex, fXX, fYY);

			oString.GetAt(nIndex)->oMetrics = oSizes.oMetrics;
            oString.SetBBox(nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY);

            double dAdvanceW = m_arrdFontMatrix[0] * oSizes.fAdvanceX;
            fPenX += (float)(dAdvanceW + m_dCharSpacing);

            if (m_bNeedDoBold)
            {
                // Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
                fPenX += 1;
            }

            pCurGlyph->bBitmap = oSizes.bBitmap;
			pCurGlyph->oBitmap = oSizes.oBitmap;
		}
        unPrevGID = unGID;
	}
	
	oString.m_fEndX = fPenX + oString.m_fX;
	oString.m_fEndY = fPenY + oString.m_fY;

    if (m_bIsNeedUpdateMatrix12)
    {
        if (m_pDefaultFont)
            m_pDefaultFont->UpdateMatrix2();
        UpdateMatrix2();
    }

	return TRUE;
}

INT CFontFile::GetString2(CGlyphString& oString)
{
	int nCountGlyph = oString.GetLength();
	if (nCountGlyph <= 0)
		return TRUE;

	//if (1 == nCountGlyph)
	//	return GetString2C(oString);
	
	unsigned int unPrevGID = 0;
	float fPenX = 0, fPenY = 0;

	for (int nIndex = 0; nIndex < nCountGlyph; ++nIndex)
	{
        // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
        if (m_bIsNeedUpdateMatrix12)
        {
            if (m_pDefaultFont)
                m_pDefaultFont->UpdateMatrix1();
            UpdateMatrix1();
        }

        FT_Face pFace = m_pFace;
		FT_GlyphSlot pCurentGliph = pFace->glyph;
        
		TGlyph* pCurGlyph = oString.GetAt(nIndex);

		int ushUnicode = pCurGlyph->lUnicode;
		
		int unGID = 0;

        USHORT charSymbolObj = 0xFFFF;

        if ( ushUnicode >= 0 && ushUnicode < FONT_CACHE_SIZES_INDEXES_SIZE) //linux !!!!
        {
            charSymbolObj = m_arrCacheSizesIndexs[ushUnicode];
        }
		if (0xFFFF == charSymbolObj || m_oCacheSizes[charSymbolObj].bBitmap == false)
		{
            int nCMapIndex = 0;
            unGID = SetCMapForCharCode(ushUnicode, &nCMapIndex);

            TFontCacheSizes oSizes;
            oSizes.ushUnicode = ushUnicode;

            if (!((unGID > 0) || (-1 != m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = SetCMapForCharCode(ushUnicode + 0xF000, &nCMapIndex)))))
            {
                // Пробуем загрузить через стандартный шрифт
                if (FALSE == m_bUseDefaultFont || NULL == m_pDefaultFont || 0 >= (unGID = m_pDefaultFont->SetCMapForCharCode(ushUnicode, &nCMapIndex)))
                {
                    if (m_nDefaultChar < 0)
                    {
                        oSizes.ushGID    = -1;
                        oSizes.eState    = glyphstateMiss;
                        oSizes.fAdvanceX = (m_pFace->size->metrics.max_advance >> 6) / 2.0f;

                        return FALSE;
                    }
                    else
                    {
                        unGID = m_nDefaultChar;
                        oSizes.eState = glyphstateNormal;

                        pFace = m_pFace;
                        pCurentGliph = pFace->glyph;
                    }
                }
                else
                {
                    oSizes.eState = glyphstateDeafault;

                    pFace = m_pDefaultFont->m_pFace;
                    pCurentGliph = pFace->glyph;
                }
            }
            else
            {
                oSizes.eState = glyphstateNormal;
            }

            oSizes.ushGID     = unGID;
            oSizes.nCMapIndex = nCMapIndex;

            if (m_bIsNeedUpdateMatrix12)
            {
                if (m_pDefaultFont)
                    m_pDefaultFont->UpdateMatrix2();
                UpdateMatrix2();
            }

			FT_Int32 _LOAD_MODE = m_bHintsSupport ? m_pFontManager->m_nLOAD_MODE : 40970;
            if (0 != FT_Load_Glyph(pFace, unGID, _LOAD_MODE))
                return FALSE;

			FT_Glyph pGlyph = NULL;
			if ( FT_Get_Glyph( pFace->glyph, &pGlyph ) )
                return FALSE;

            FT_BBox oBBox;

            FT_Glyph_Get_CBox(pGlyph, 1, &oBBox);
            FT_Done_Glyph(pGlyph);

            pCurentGliph = pFace->glyph;

            oSizes.fAdvanceX = (float)(pFace->glyph->linearHoriAdvance * m_dUnitsKoef / pFace->units_per_EM);
			oSizes.oBBox.fMinX = (float)(oBBox.xMin >> 6);
			oSizes.oBBox.fMaxX = (float)(oBBox.xMax >> 6);
			oSizes.oBBox.fMinY = (float)(oBBox.yMin >> 6);
			oSizes.oBBox.fMaxY = (float)(oBBox.yMax >> 6);

			oSizes.oMetrics.fHeight       = (float)(pFace->glyph->metrics.height       >> 6);
			oSizes.oMetrics.fHoriAdvance  = (float)(pFace->glyph->metrics.horiAdvance  >> 6);
			oSizes.oMetrics.fHoriBearingX = (float)(pFace->glyph->metrics.horiBearingX >> 6);
			oSizes.oMetrics.fHoriBearingY = (float)(pFace->glyph->metrics.horiBearingY >> 6);
			oSizes.oMetrics.fVertAdvance  = (float)(pFace->glyph->metrics.vertAdvance  >> 6);
			oSizes.oMetrics.fVertBearingX = (float)(pFace->glyph->metrics.vertBearingX >> 6);
			oSizes.oMetrics.fVertBearingY = (float)(pFace->glyph->metrics.vertBearingY >> 6);
			oSizes.oMetrics.fWidth        = (float)(pFace->glyph->metrics.width        >> 6);

            pCurGlyph->bBitmap  = true;
            if (FT_Render_Glyph(pCurentGliph, REND_MODE))
                return FALSE;

			TGlyphBitmap *pBitmap = &(pCurGlyph->oBitmap);
			pBitmap->nX      = pCurentGliph->bitmap_left;
			pBitmap->nY      = pCurentGliph->bitmap_top;
			pBitmap->nWidth  = pCurentGliph->bitmap.width;
			pBitmap->nHeight = pCurentGliph->bitmap.rows;
			pBitmap->bAA     = m_bAntiAliasing;

			int nRowSize = 0;
			if ( m_bAntiAliasing ) 
			{
				if ( m_bNeedDoBold )
					pBitmap->nWidth++;

				nRowSize = pBitmap->nWidth;
			} 
			else 
			{
				nRowSize = (pBitmap->nWidth + 7) >> 3;
			}

			if (0 != (nRowSize * pBitmap->nHeight))
				pBitmap->pData = (unsigned char *)malloc( nRowSize * pBitmap->nHeight );
			else
				pBitmap->pData = NULL;

			pBitmap->bFreeData = FALSE; // Все удаляется в кэше (во время очистки или замены)

			int nIndex2;
			unsigned char *pDstBuffer, *pSrcBuffer;

			if (NULL != pBitmap->pData)
			{
				//double dKoef = ( 255 + 10 ) / (double)255;
				if ( !m_bNeedDoBold || !m_bAntiAliasing )
				{
					for ( nIndex2 = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pCurentGliph->bitmap.buffer; nIndex2 < pBitmap->nHeight; ++nIndex2, pDstBuffer += nRowSize, pSrcBuffer += pCurentGliph->bitmap.pitch ) 
					{
						memcpy( pDstBuffer, pSrcBuffer, nRowSize );
					}
				}
				else
				{
					int nY, nX;
					for ( nY = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pCurentGliph->bitmap.buffer; nY < pBitmap->nHeight; ++nY, pDstBuffer += nRowSize, pSrcBuffer += pCurentGliph->bitmap.pitch ) 
					{
						for ( nX = pBitmap->nWidth - 1; nX >= 0; nX-- )
						{
							if ( 0 != nX )
							{
								int nFirstByte, nSecondByte;

								if ( pBitmap->nWidth - 1 == nX )
									nFirstByte = 0;
								else
									nFirstByte = pSrcBuffer[nX];

								nSecondByte = pSrcBuffer[nX - 1];

								pDstBuffer[nX] = min( 255, nFirstByte + nSecondByte);

							}
							else
							{
								pDstBuffer[nX] = pSrcBuffer[nX];
							}
						}
					}

				}
			}

			oSizes.bBitmap			 = pCurGlyph->bBitmap;
			oSizes.oBitmap.bAA       = pBitmap->bAA;
			oSizes.oBitmap.bFreeData = pBitmap->bFreeData;
			oSizes.oBitmap.nX        = pBitmap->nX;
			oSizes.oBitmap.nY        = pBitmap->nY;
			oSizes.oBitmap.nWidth    = pBitmap->nWidth;
			oSizes.oBitmap.nHeight   = pBitmap->nHeight;
			oSizes.oBitmap.pData     = pBitmap->pData;

            if ( AddToSizesCache( oSizes ) == true)
            {
                charSymbolObj = m_arrCacheSizesIndexs[oSizes.ushUnicode];
            }
		}
		if (0xFFFF != charSymbolObj)
		{
			TFontCacheSizes& oSizes = m_oCacheSizes[charSymbolObj];
			int nCMapIndex = oSizes.nCMapIndex;

			if (glyphstateMiss == oSizes.eState)
            {
                oString.SetStartPoint (nIndex, fPenX, fPenY);
                oString.SetBBox(nIndex, 0, 0, 0, 0);
                oString.SetState (nIndex, glyphstateMiss);

				double dAdvanceW = m_arrdFontMatrix[0] * oSizes.fAdvanceX;
				fPenX += (float)(dAdvanceW + m_dCharSpacing);
                
                unPrevGID = 0;

                continue;
            }
            else if (glyphstateDeafault == oSizes.eState)
            {
                oString.SetState(nIndex, glyphstateDeafault);
            }
            else
            {
                oString.SetState(nIndex, glyphstateNormal);
            }

            if ( 0 != pFace->num_charmaps )
			{
				int nCurCMapIndex = FT_Get_Charmap_Index( pFace->charmap );
				if ( nCurCMapIndex != nCMapIndex )
				{
					nCMapIndex = max( 0, nCMapIndex );
					FT_Set_Charmap( pFace, pFace->charmaps[nCMapIndex] );
				}
			}

            if (m_bUseKerning && unPrevGID && (nIndex >= 0 && oString.GetAt(nIndex)->eState == oString.GetAt(nIndex - 1)->eState))
            {
                fPenX += GetKerning(unPrevGID, unGID);
            }

            float fX = oString.m_fX + fPenX;
            float fY = oString.m_fY + fPenY;

            float fXX = (float)(oString.m_arrCTM[4] + fX * oString.m_arrCTM[0] + fY * oString.m_arrCTM[2] - oString.m_fX);
            float fYY = (float)(oString.m_arrCTM[5] + fX * oString.m_arrCTM[1] + fY * oString.m_arrCTM[3] - oString.m_fY);

            oString.SetStartPoint(nIndex, fXX, fYY);

			oString.GetAt(nIndex)->oMetrics = oSizes.oMetrics;
            oString.SetBBox(nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY);
			
			double dAdvanceW = m_arrdFontMatrix[0] * oSizes.fAdvanceX;
            fPenX += (float)(dAdvanceW + m_dCharSpacing);
            
            if (m_bNeedDoBold)
            {
                // Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
                fPenX += 1;
            }

            pCurGlyph->bBitmap = oSizes.bBitmap;
			pCurGlyph->oBitmap = oSizes.oBitmap;
		}
        unPrevGID = unGID;
	}
	
	oString.m_fEndX = fPenX + oString.m_fX;
	oString.m_fEndY = fPenY + oString.m_fY;

    if (m_bIsNeedUpdateMatrix12)
    {
        if (m_pDefaultFont)
            m_pDefaultFont->UpdateMatrix2();
        UpdateMatrix2();
    }

	return TRUE;
}

INT CFontFile::GetString2C(CGlyphString& oString)
{
    unsigned int unPrevGID = 0;
	float fPenX = 0, fPenY = 0;

    // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
    if (m_bIsNeedUpdateMatrix12)
    {
        if (m_pDefaultFont)
            m_pDefaultFont->UpdateMatrix1();
        UpdateMatrix1();
    }

	FT_Face pFace = m_pFace;
	FT_GlyphSlot pCurentGliph = pFace->glyph;

	TGlyph* pCurGlyph = oString.GetAt(0);
    int ushUnicode = pCurGlyph->lUnicode;

    int unGID = 0;

    USHORT charSymbolObj = 0xFFFF;

    if (ushUnicode >= 0 && ushUnicode < FONT_CACHE_SIZES_INDEXES_SIZE)     //вылетает под Linux
    {
        charSymbolObj = m_arrCacheSizesIndexs[ushUnicode];
    }
    if (0xFFFF == charSymbolObj || m_oCacheSizes[charSymbolObj].bBitmap == false)
    {
        int nCMapIndex = 0;
        unGID = SetCMapForCharCode(ushUnicode, &nCMapIndex);

        TFontCacheSizes oSizes;
        oSizes.ushUnicode = ushUnicode;

        if (!((unGID > 0) || (-1 != m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = SetCMapForCharCode(ushUnicode + 0xF000, &nCMapIndex)))))
        {
            // Пробуем загрузить через стандартный шрифт
            if (FALSE == m_bUseDefaultFont || NULL == m_pDefaultFont || 0 >= (unGID = m_pDefaultFont->SetCMapForCharCode(ushUnicode, &nCMapIndex)))
            {
                if (m_nDefaultChar < 0)
                {
                    oSizes.ushGID    = -1;
                    oSizes.eState    = glyphstateMiss;
                    oSizes.fAdvanceX = (m_pFace->size->metrics.max_advance >> 6) / 2.0f;

                    return FALSE;
                }
                else
                {
                    unGID = m_nDefaultChar;
                    oSizes.eState = glyphstateNormal;

                    pFace = m_pFace;
                    pCurentGliph = pFace->glyph;
                }
            }
            else
            {
                oSizes.eState = glyphstateDeafault;

                pFace = m_pDefaultFont->m_pFace;
                pCurentGliph = pFace->glyph;
            }
        }
        else
        {
            oSizes.eState = glyphstateNormal;
        }

        oSizes.ushGID     = unGID;
        oSizes.nCMapIndex = nCMapIndex;

        if (m_bIsNeedUpdateMatrix12)
        {
            if (m_pDefaultFont)
                m_pDefaultFont->UpdateMatrix2();
            UpdateMatrix2();
        }

		FT_Int32 _LOAD_MODE = m_bHintsSupport ? m_pFontManager->m_nLOAD_MODE : 40970;
        if (0 != FT_Load_Glyph(pFace, unGID, _LOAD_MODE))
            return FALSE;

		FT_Glyph pGlyph = NULL;
		if ( FT_Get_Glyph( pFace->glyph, &pGlyph ) )
            return FALSE;

        FT_BBox oBBox;

        FT_Glyph_Get_CBox(pGlyph, 1, &oBBox);
        FT_Done_Glyph(pGlyph);

        pCurentGliph = pFace->glyph;

        oSizes.fAdvanceX = (float)(pFace->glyph->linearHoriAdvance * m_dUnitsKoef / pFace->units_per_EM);
		oSizes.oBBox.fMinX = (float)(oBBox.xMin >> 6);
		oSizes.oBBox.fMaxX = (float)(oBBox.xMax >> 6);
		oSizes.oBBox.fMinY = (float)(oBBox.yMin >> 6);
		oSizes.oBBox.fMaxY = (float)(oBBox.yMax >> 6);

		oSizes.oMetrics.fHeight       = (float)(pFace->glyph->metrics.height       >> 6);
		oSizes.oMetrics.fHoriAdvance  = (float)(pFace->glyph->metrics.horiAdvance  >> 6);
		oSizes.oMetrics.fHoriBearingX = (float)(pFace->glyph->metrics.horiBearingX >> 6);
		oSizes.oMetrics.fHoriBearingY = (float)(pFace->glyph->metrics.horiBearingY >> 6);
		oSizes.oMetrics.fVertAdvance  = (float)(pFace->glyph->metrics.vertAdvance  >> 6);
		oSizes.oMetrics.fVertBearingX = (float)(pFace->glyph->metrics.vertBearingX >> 6);
		oSizes.oMetrics.fVertBearingY = (float)(pFace->glyph->metrics.vertBearingY >> 6);
		oSizes.oMetrics.fWidth        = (float)(pFace->glyph->metrics.width        >> 6);

        pCurGlyph->bBitmap  = true;
        if (FT_Render_Glyph(pCurentGliph, REND_MODE))
            return FALSE;

		TGlyphBitmap *pBitmap = &(pCurGlyph->oBitmap);
		pBitmap->nX      = pCurentGliph->bitmap_left;
		pBitmap->nY      = pCurentGliph->bitmap_top;
		pBitmap->nWidth  = pCurentGliph->bitmap.width;
		pBitmap->nHeight = pCurentGliph->bitmap.rows;
		pBitmap->bAA     = m_bAntiAliasing;

		int nRowSize = 0;
		if ( m_bAntiAliasing ) 
		{
			if ( m_bNeedDoBold )
				pBitmap->nWidth++;

			nRowSize = pBitmap->nWidth;
		} 
		else 
		{
			nRowSize = (pBitmap->nWidth + 7) >> 3;
		}

		if (0 != (nRowSize * pBitmap->nHeight))
			pBitmap->pData = (unsigned char *)malloc( nRowSize * pBitmap->nHeight );
		else
			pBitmap->pData = NULL;

		pBitmap->bFreeData = FALSE; // Все удаляется в кэше (во время очистки или замены)

		int nIndex2;
		unsigned char *pDstBuffer, *pSrcBuffer;

		if (NULL != pBitmap->pData)
		{
			//double dKoef = ( 255 + 10 ) / (double)255;
			if ( !m_bNeedDoBold || !m_bAntiAliasing )
			{
				for ( nIndex2 = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pCurentGliph->bitmap.buffer; nIndex2 < pBitmap->nHeight; ++nIndex2, pDstBuffer += nRowSize, pSrcBuffer += pCurentGliph->bitmap.pitch ) 
				{
					memcpy( pDstBuffer, pSrcBuffer, nRowSize );
				}
			}
			else
			{
				int nY, nX;
				for ( nY = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pCurentGliph->bitmap.buffer; nY < pBitmap->nHeight; ++nY, pDstBuffer += nRowSize, pSrcBuffer += pCurentGliph->bitmap.pitch ) 
				{
					for ( nX = pBitmap->nWidth - 1; nX >= 0; nX-- )
					{
						if ( 0 != nX )
						{
							int nFirstByte, nSecondByte;

							if ( pBitmap->nWidth - 1 == nX )
								nFirstByte = 0;
							else
								nFirstByte = pSrcBuffer[nX];

							nSecondByte = pSrcBuffer[nX - 1];

							pDstBuffer[nX] = min( 255, nFirstByte + nSecondByte);

						}
						else
						{
							pDstBuffer[nX] = pSrcBuffer[nX];
						}
					}
				}

			}
		}

		oSizes.bBitmap			 = pCurGlyph->bBitmap;
		oSizes.oBitmap.bAA       = pBitmap->bAA;
		oSizes.oBitmap.bFreeData = pBitmap->bFreeData;
		oSizes.oBitmap.nX        = pBitmap->nX;
		oSizes.oBitmap.nY        = pBitmap->nY;
		oSizes.oBitmap.nWidth    = pBitmap->nWidth;
		oSizes.oBitmap.nHeight   = pBitmap->nHeight;
		oSizes.oBitmap.pData     = pBitmap->pData;

        if (AddToSizesCache( oSizes ) == true)
        {
            charSymbolObj = m_arrCacheSizesIndexs[oSizes.ushUnicode];
        }
    }
    if (0xFFFF != charSymbolObj)
    {
		TFontCacheSizes& oSizes = m_oCacheSizes[charSymbolObj];
		int nCMapIndex = oSizes.nCMapIndex;

		if (glyphstateMiss == oSizes.eState)
        {
            return TRUE;
        }
        else if (glyphstateDeafault == oSizes.eState)
        {
			pCurGlyph->eState = glyphstateDeafault;
        }
        else
        {
			pCurGlyph->eState = glyphstateNormal;
        }

        pCurGlyph->oMetrics = oSizes.oMetrics;

		pCurGlyph->bBitmap = oSizes.bBitmap;
		pCurGlyph->oBitmap = oSizes.oBitmap;
    }
    
        if (true)
        {
            float fX = oString.m_fX + fPenX;
            float fY = oString.m_fY + fPenY;

            pCurGlyph->fX = (float)(oString.m_arrCTM[4] + fX * oString.m_arrCTM[0] + fY * oString.m_arrCTM[2] - oString.m_fX);
            pCurGlyph->fY = (float)(oString.m_arrCTM[5] + fX * oString.m_arrCTM[1] + fY * oString.m_arrCTM[3] - oString.m_fY);
        }

	double dAdvanceW = m_arrdFontMatrix[0] * m_oCacheSizes[charSymbolObj].fAdvanceX;
	fPenX += (float)(dAdvanceW + m_dCharSpacing);
    if (m_bNeedDoBold)
    {
        // Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
        fPenX += m_unHorDpi/72.f;
    }

    oString.m_fEndX = fPenX + oString.m_fX;
	oString.m_fEndY = fPenY + oString.m_fY;

    if (m_bIsNeedUpdateMatrix12)
    {
        if (m_pDefaultFont)
            m_pDefaultFont->UpdateMatrix2();
        UpdateMatrix2();
    }

	return TRUE;
}

std::wstring CFontFile::GetFontFormat() const
{
	if (!m_pFace)
		return L"";

	const char* sFormat = FT_Get_X11_Font_Format(m_pFace);
	return NSStringExt::CConverter::GetUnicodeFromSingleByteString((const unsigned char*)sFormat, strlen(sFormat));
}
unsigned int CFontFile::GetNameIndex(const std::wstring& wsName) const
{
	if (!m_pFace)
		return 0;

	int nLen = wsName.length();
	char* sName = new char[nLen + 1];
	if (!sName)
		return 0;

	sName[nLen] = 0x00;
	for (int nIndex = 0; nIndex < nLen; nIndex++)
	{
		sName[nIndex] = (char)(wsName[nIndex]);
	}
	unsigned int unGID = FT_Get_Name_Index(m_pFace, sName);
	delete[] sName;

	return unGID;
}

////////////////////////////////////////////////////////////////////////////////
static int GlyphPathMoveTo(const FT_Vector *pPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	if ( pGlyphPath->bNeedClose ) 
	{
		pGlyphPath->pPath->Close();
		pGlyphPath->bNeedClose = FALSE;
	}
	pGlyphPath->pPath->MoveTo( (double)pPoint->x / 64.0, (double)pPoint->y / 64.0 );
	return 0;
}

static int GlyphPathLineTo(const FT_Vector *pPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	pGlyphPath->pPath->LineTo( (double)pPoint->x / 64.0, (double)pPoint->y / 64.0 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

static int GlyphPathConicTo(const FT_Vector *pControlPoint, const FT_Vector *pEndPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	double dX0, dY0;

	if ( !pGlyphPath->pPath->GetCurPoint( &dX0, &dY0 ) ) 
	{
		return 0;
	}

	double dXc = (double)pControlPoint->x / 64.0;
	double dYc = (double)pControlPoint->y / 64.0;
	double dX3 = (double)pEndPoint->x / 64.0;
	double dY3 = (double)pEndPoint->y / 64.0;

	// Строим кривую Безье второго порядка, с помощью кривой Безье третего порядка. Если p0, pC, p3 -
	// начальная, контрольная и конечная точки, соответственно, для кривой Безье второго порядка. Тогда 
	// для этой же кривой, рассматриваемой как кривая Безье третьего порядка, точки p0, p1, p2, p3 будут
	// начальной, две контрольные, конечная точки. Где p1 и p2 рассчитываются по следующим формулам:
	//     p1 = (1/3) * (p0 + 2pС)
	//     p2 = (1/3) * (2pС + p3)

	double dX1 = (double)(1.0 / 3.0) * (dX0 + (double)2 * dXc);
	double dY1 = (double)(1.0 / 3.0) * (dY0 + (double)2 * dYc);
	double dX2 = (double)(1.0 / 3.0) * ((double)2 * dXc + dX3);
	double dY2 = (double)(1.0 / 3.0) * ((double)2 * dYc + dY3);

	pGlyphPath->pPath->CurveTo( dX1, dY1, dX2, dY2, dX3, dY3 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

static int GlyphPathCubicTo(const FT_Vector *pFirstControlPoint, const FT_Vector *pSecondControlPoint, const FT_Vector *pEndPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	double dX1 = (double)pFirstControlPoint->x / 64.0;
	double dY1 = (double)pFirstControlPoint->y / 64.0;
	double dX2 = (double)pSecondControlPoint->x / 64.0;
	double dY2 = (double)pSecondControlPoint->y / 64.0;
	double dX3 = (double)pEndPoint->x / 64.0;
	double dY3 = (double)pEndPoint->y / 64.0;

	pGlyphPath->pPath->CurveTo( dX1, dY1, dX2, dY2, dX3, dY3 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

CFontPath* CFontFile::GetGlyphPath(int nCode) 
{ 
	FT_UInt unGID = SetCMapForCharCode2( nCode );
	if (unGID <= 0)
		return NULL;

	static FT_Outline_Funcs pOutlineFuncs = 
	{
		&GlyphPathMoveTo,
		&GlyphPathLineTo,
		&GlyphPathConicTo,
		&GlyphPathCubicTo,
		0, 0
	};

	FT_GlyphSlot oSlot = m_pFace->glyph;

	// TO DO: Пропустить нулевой (".notdef") символ в TrueType
	if ( FT_Load_Glyph( m_pFace, unGID, FT_LOAD_NO_BITMAP ) )
		return NULL;

	FT_Glyph oGlyph;
	if ( FT_Get_Glyph( oSlot, &oGlyph ) ) 
	{
		return NULL;
	}

	TFreeTypeFontPath oGlyphPath;
	oGlyphPath.pPath      = new CFontPath();
	oGlyphPath.bNeedClose = FALSE;

	FT_Outline_Decompose( &((FT_OutlineGlyph)oGlyph)->outline, &pOutlineFuncs, &oGlyphPath );

	if ( oGlyphPath.bNeedClose ) 
	{
		oGlyphPath.pPath->Close();
	}

	FT_Done_Glyph( oGlyph );
	return oGlyphPath.pPath;
};

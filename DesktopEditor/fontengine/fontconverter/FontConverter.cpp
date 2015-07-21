#include "../FontConverter.h"

#include "Consts.h"
#include "Utils.h"
#include FT_XFREE86_H
#include FT_TRUETYPE_TABLES_H
#include "FontFileTrueType.h"
#include "FontFileType1C.h"
#include "FontFileType1.h"

bool CFontConverter::ToOTF(std::wstring sFontIn, std::wstring sFontOut, unsigned int* pSymbols, int nCount, std::wstring sNameW, long nFlag)
{
    FT_Library pLibrary = NULL;
    if ( FT_Init_FreeType( &pLibrary ) )
        return false;

    FT_Face pFace = NULL;

    NSFile::CFileBinary oFileBinary;
    if (!oFileBinary.OpenFile(sFontIn))
        return false;

    FT_Long nFileSize = (FT_Long)oFileBinary.GetFileSize();
    BYTE* pBaseAddress = new BYTE[nFileSize];
    DWORD dwRead = 0;
    oFileBinary.ReadFile(pBaseAddress, (DWORD)nFileSize, dwRead);

    FT_Open_Args oOpenArgs;
    oOpenArgs.flags = FT_OPEN_MEMORY;
    oOpenArgs.memory_base = (BYTE*)pBaseAddress;
    oOpenArgs.memory_size = nFileSize;

    NSFontConverter::CFontFileTrueType* pTTF = NSFontConverter::CFontFileTrueType::LoadFromFile( sFontIn.c_str() );
    FT_Error oerrr;
    if ( oerrr = FT_Open_Face( pLibrary, &oOpenArgs, 0, &pFace ) )
    {
        FT_Done_FreeType( pLibrary );
        RELEASEARRAYOBJECTS(pBaseAddress);
        return false;
    }

    std::string sFontFormat( FT_Get_X11_Font_Format( pFace ) );

    // Проверим флаг конвертации и исходный формат шрифта
    bool bNeedConvert = false;

    if ( nFlag == NSFontConverter::c_lFromAll ||
         ( "TrueType" == sFontFormat && nFlag & NSFontConverter::c_lFromTT ) ||
         ( "CFF" == sFontFormat && nFlag & NSFontConverter::c_lFromCFF ) ||
         ( "Type 1" == sFontFormat && nFlag & NSFontConverter::c_lFromT1 ) )
        bNeedConvert = true;

    bool bIsGids = (NSFontConverter::c_lFlagsGids & nFlag);

    if ( bNeedConvert )
    {
        if ( "CFF" == sFontFormat || "Type 1" == sFontFormat )
        {
            NSFontConverter::TCharBuffer oCFF;
            NSFontConverter::CFontFileType1C *pT1C = NULL;
            if ( "Type 1" == sFontFormat )
            {
                // Сначала сконвертируем Type1 в CFF
                NSFontConverter::CFontFileType1* pT1 = NSFontConverter::CFontFileType1::LoadFromFile( sFontIn.c_str() );
                pT1->ToCFF( &NSFontConverter::CharBufferWrite, &oCFF );
                delete pT1;

                // Конвертируем CFF в OpenTypeCFF
                pT1C = NSFontConverter::CFontFileType1C::LoadFromBuffer( oCFF.sBuffer, oCFF.nLen );
            }
            else
            {
                // FreeType отдает тип шрифта CFF, в случаях когда файл имеет тип OpenType(CFF).
                // Если так оно и есть, тогда нам с файлом ничего делать на надо.
                pT1C = NSFontConverter::CFontFileType1C::LoadFromFile( sFontIn.c_str() );
            }

            if ( pT1C )
            {
                NSFile::CFileBinary oWriteFile;
                oWriteFile.CreateFileW(sFontOut);
                pT1C->ToOpenTypeCFF( &NSFontConverter::FileWrite, oWriteFile.GetFileNative(), pFace );
                oWriteFile.CloseFile();
            }

            delete pT1C;
        }
        else if ( "TrueType" == sFontFormat && ( pSymbols != NULL || !sNameW.empty() ) )
        {
            NSFontConverter::CFontFileTrueType* pTTF = NSFontConverter::CFontFileTrueType::LoadFromFile( sFontIn.c_str() );
            if ( pTTF )
            {
                std::string sName = U_TO_UTF8(sNameW);
                unsigned char *pUseGlyfs = NULL;
                long lGlyfsCount = pFace->num_glyphs;

                if ( pSymbols )
                {
                    // Сначала составим список нужных нами GID
                    unsigned int* pUnicode = pSymbols;
                    unsigned short* pGIDs = new unsigned short[nCount];
                    int nCMapIndex = 0;

                    int nSymbolicIndex = NSFontConverter::GetSymbolicCmapIndex(pFace);

                    if (!bIsGids)
                    {
                        for ( int nIndex = 0; nIndex < nCount; nIndex++ )
                        {
                            pGIDs[nIndex] = NSFontConverter::SetCMapForCharCode( pFace, pUnicode[nIndex], &nCMapIndex  );

                            if ((pGIDs[nIndex] == 0) && (-1 != nSymbolicIndex) && (pUnicode[nIndex] < 0xF000))
                            {
                                pGIDs[nIndex] = NSFontConverter::SetCMapForCharCode( pFace, pUnicode[nIndex] + 0xF000, &nCMapIndex  );
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < nCount; ++i)
                            pGIDs[i] = (unsigned short)pUnicode[i];
                    }

                    pUseGlyfs = new unsigned char[lGlyfsCount];
                    ::memset( pUseGlyfs, 0x00, lGlyfsCount * sizeof(unsigned char) );
                    pUseGlyfs[0] = 1; // нулевой гид всегда записываем
                    for ( int nGID = 1; nGID < lGlyfsCount; nGID++ )
                    {
                        if ( 1 != pUseGlyfs[nGID] )
                        {
                            bool bFound = false;
                            for ( int nIndex = 0; nIndex < nCount; nIndex++ )
                            {
                                if ( nGID == pGIDs[nIndex] )
                                {
                                    bFound = true;
                                    break;
                                }
                            }

                            // Если данный символ составной (CompositeGlyf), тогда мы должны учесть все его дочерные символы (subglyfs)
                            if ( bFound && 0 == FT_Load_Glyph( pFace, nGID, FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE ) )
                            {
                                for ( int nSubIndex = 0; nSubIndex < pFace->glyph->num_subglyphs; nSubIndex++ )
                                {
                                    FT_Int       nSubGID;
                                    FT_UInt      unFlags;
                                    FT_Int       nArg1;
                                    FT_Int       nArg2;
                                    FT_Matrix    oMatrix;
                                    FT_Get_SubGlyph_Info( pFace->glyph, nSubIndex, &nSubGID, &unFlags, &nArg1, &nArg2, &oMatrix );

                                    if ( nSubGID < lGlyfsCount )
                                        pUseGlyfs[nSubGID] = 1;
                                }
                            }

                            if ( bFound )
                                pUseGlyfs[nGID] = 1;
                        }
                    }
                }

                NSFile::CFileBinary oWriteFile;
                oWriteFile.CreateFileW(sFontOut);
                pTTF->WriteTTF( &NSFontConverter::FileWrite, oWriteFile.GetFileNative(), sName.c_str(), NULL, pUseGlyfs, lGlyfsCount );
                oWriteFile.CloseFile();
            }
            else
            {
                // error parse font
                // Просто копируем файл
                NSFile::CFileBinary::Copy(sFontIn, sFontOut);
            }
        }
    }
    else
    {
        // Просто копируем файл
        NSFile::CFileBinary::Copy(sFontIn, sFontOut);
    }

    FT_Done_Face( pFace );
    FT_Done_FreeType( pLibrary );

    RELEASEARRAYOBJECTS(pBaseAddress);

    return true;
}

bool CFontConverter::ToOTF2(std::wstring sFontIn, unsigned int* pSymbols, int nCount, std::wstring sNameW, long nFlag, long lFaceIndex, unsigned char*& pDstData, int& nDstLen)
{
    // функция просто скопирована и немного доработана. это все из-за нехватки времени.

    FT_Library pLibrary = NULL;
    if ( FT_Init_FreeType( &pLibrary ) )
        return false;

    FT_Face pFace = NULL;

    NSFile::CFileBinary oFileBinary;
    if (!oFileBinary.OpenFile(sFontIn))
        return false;

    FT_Long nFileSize = (FT_Long)oFileBinary.GetFileSize();
    BYTE* pBaseAddress = new BYTE[nFileSize];
    DWORD dwRead = 0;
    oFileBinary.ReadFile(pBaseAddress, (DWORD)nFileSize, dwRead);

    FT_Open_Args oOpenArgs;
    oOpenArgs.flags = FT_OPEN_MEMORY;
    oOpenArgs.memory_base = (BYTE*)pBaseAddress;
    oOpenArgs.memory_size = (FT_Long)nFileSize;

    NSFontConverter::CFontFileTrueType* pTTF = NSFontConverter::CFontFileTrueType::LoadFromFile( sFontIn.c_str() );
    FT_Error oerrr;
    if ( oerrr = FT_Open_Face( pLibrary, &oOpenArgs, lFaceIndex, &pFace ) )
    {
        FT_Done_FreeType( pLibrary );
        RELEASEARRAYOBJECTS(pBaseAddress);
        return false;
    }

    std::string sFontFormat( FT_Get_X11_Font_Format( pFace ) );

    // Проверим флаг конвертации и исходный формат шрифта
    bool bNeedConvert = false;

    if ( nFlag == NSFontConverter::c_lFromAll ||
         ( "TrueType" == sFontFormat && nFlag & NSFontConverter::c_lFromTT ) ||
         ( "CFF" == sFontFormat && nFlag & NSFontConverter::c_lFromCFF ) ||
         ( "Type 1" == sFontFormat && nFlag & NSFontConverter::c_lFromT1 ) )
        bNeedConvert = true;

    bool bIsGids = (NSFontConverter::c_lFlagsGids & nFlag);

    NSFontConverter::TCharBuffer* pCharBuffer = NULL;

    if ( bNeedConvert )
    {
        pCharBuffer = new NSFontConverter::TCharBuffer(100000); // ~100Kb
        if ( "CFF" == sFontFormat || "Type 1" == sFontFormat )
        {
            NSFontConverter::TCharBuffer oCFF;
            NSFontConverter::CFontFileType1C *pT1C = NULL;
            if ( "Type 1" == sFontFormat )
            {
                // Сначала сконвертируем Type1 в CFF
                NSFontConverter::CFontFileType1* pT1 = NSFontConverter::CFontFileType1::LoadFromFile( sFontIn.c_str() );
                pT1->ToCFF( &NSFontConverter::CharBufferWrite, &oCFF );
                delete pT1;

                // Конвертируем CFF в OpenTypeCFF
                pT1C = NSFontConverter::CFontFileType1C::LoadFromBuffer( oCFF.sBuffer, oCFF.nLen );
            }
            else
            {
                // FreeType отдает тип шрифта CFF, в случаях когда файл имеет тип OpenType(CFF).
                // Если так оно и есть, тогда нам с файлом ничего делать на надо.
                pT1C = NSFontConverter::CFontFileType1C::LoadFromFile( sFontIn.c_str() );
            }

            if ( pT1C )
            {
                pT1C->ToOpenTypeCFF( &NSFontConverter::CharBufferWrite, pCharBuffer, pFace );
            }

            delete pT1C;
        }
        else if ( "TrueType" == sFontFormat && ( pSymbols != NULL || !sNameW.empty() ) )
        {
            NSFontConverter::CFontFileTrueType* pTTF = NSFontConverter::CFontFileTrueType::LoadFromFile( sFontIn.c_str() );
            if ( pTTF )
            {
                std::string sName = U_TO_UTF8(sNameW);
                unsigned char *pUseGlyfs = NULL;
                long lGlyfsCount = pFace->num_glyphs;

                if ( pSymbols )
                {
                    // Сначала составим список нужных нами GID
                    unsigned int* pUnicode = (unsigned int*)pSymbols;
                    unsigned short* pGIDs = new unsigned short[nCount];
                    int nCMapIndex = 0;

                    int nSymbolicIndex = NSFontConverter::GetSymbolicCmapIndex(pFace);

                    if (!bIsGids)
                    {
                        for ( int nIndex = 0; nIndex < nCount; nIndex++ )
                        {
                            pGIDs[nIndex] = NSFontConverter::SetCMapForCharCode( pFace, pUnicode[nIndex], &nCMapIndex  );

                            if ((pGIDs[nIndex] == 0) && (-1 != nSymbolicIndex) && (pUnicode[nIndex] < 0xF000))
                            {
                                pGIDs[nIndex] = NSFontConverter::SetCMapForCharCode( pFace, pUnicode[nIndex] + 0xF000, &nCMapIndex  );
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < nCount; ++i)
                            pGIDs[i] = (unsigned short)pUnicode[i];
                    }

                    pUseGlyfs = new unsigned char[lGlyfsCount];
                    ::memset( pUseGlyfs, 0x00, lGlyfsCount * sizeof(unsigned char) );
                    pUseGlyfs[0] = 1; // нулевой гид всегда записываем
                    for ( int nGID = 1; nGID < lGlyfsCount; nGID++ )
                    {
                        if ( 1 != pUseGlyfs[nGID] )
                        {
                            bool bFound = false;
                            for ( int nIndex = 0; nIndex < nCount; nIndex++ )
                            {
                                if ( nGID == pGIDs[nIndex] )
                                {
                                    bFound = true;
                                    break;
                                }
                            }

                            // Если данный символ составной (CompositeGlyf), тогда мы должны учесть все его дочерные символы (subglyfs)
                            if ( bFound && 0 == FT_Load_Glyph( pFace, nGID, FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE ) )
                            {
                                for ( int nSubIndex = 0; nSubIndex < pFace->glyph->num_subglyphs; nSubIndex++ )
                                {
                                    FT_Int       nSubGID;
                                    FT_UInt      unFlags;
                                    FT_Int       nArg1;
                                    FT_Int       nArg2;
                                    FT_Matrix    oMatrix;
                                    FT_Get_SubGlyph_Info( pFace->glyph, nSubIndex, &nSubGID, &unFlags, &nArg1, &nArg2, &oMatrix );

                                    if ( nSubGID < lGlyfsCount )
                                        pUseGlyfs[nSubGID] = 1;
                                }
                            }

                            if ( bFound )
                                pUseGlyfs[nGID] = 1;
                        }
                    }
                }

                pTTF->WriteTTF( &NSFontConverter::CharBufferWrite, pCharBuffer, sName.c_str(), NULL, pUseGlyfs, lGlyfsCount );
            }
        }

        LONG nSizeFont = pCharBuffer->nLen;

        nDstLen = (int)nSizeFont;
        pDstData = new BYTE[nDstLen];
        memcpy(pDstData, pCharBuffer->sBuffer, nDstLen);

        RELEASEOBJECT(pCharBuffer);
    }
    else
    {
        nDstLen = (int)nFileSize;
        pDstData = new BYTE[nDstLen];
        memcpy(pDstData, pBaseAddress, nDstLen);
    }

    FT_Done_Face( pFace );
    FT_Done_FreeType( pLibrary );

    RELEASEARRAYOBJECTS(pBaseAddress);

    return true;
}




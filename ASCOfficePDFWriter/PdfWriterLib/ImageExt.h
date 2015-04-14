#pragma once

#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"
namespace NSImageExt
{

	class CImageExt
	{
	public:
		LONG GetImageType(CString strFile)
		{		
			CFile oFile;
			if (S_OK != oFile.OpenFile(strFile))
			{
				oFile.CloseFile();
				return 0;
			}

			DWORD dwSize = (DWORD)oFile.GetFileSize();

			if (44 > dwSize)
			{
				return 0;
			}

			DWORD dwKey = 0;
			oFile.ReadFile((BYTE*)(&dwKey), 4);

			if (0x9AC6CDD7 == dwKey)
			{
				// placeable meta
				oFile.CloseFile();
				return 1;
			}

			if (0x00000001 == dwKey)
			{
				oFile.SetPosition(40);
				oFile.ReadFile((BYTE*)(&dwKey), 4);
				oFile.CloseFile();

				if (0x464D4520 == dwKey)
				{
					// EMF/EMF+
					return 2;
				}
				else
				{
					// 
					return 0;
				}
			}

			oFile.CloseFile();

			if (0x00090001 == dwKey)
			{
				return 1;
			}

			return 0;
		}

#ifdef BUILD_CONFIG_FULL_VERSION
        bool DrawOnRenderer(IRenderer* pRenderer, CString strFile, double dX, double dY, double dW, double dH)
		{
			if (NULL == pRenderer)
                return false;

			LONG lFileType = GetImageType(strFile);

			if (1 == lFileType || 2 == lFileType)
			{
				// Запоминаем все настройки Brush, Font, Pen, Shadow и т.д.
				LONG lBrAlpha1 = 0; pRenderer->get_BrushAlpha1( &lBrAlpha1 );
				LONG lBrAlpha2 = 0; pRenderer->get_BrushAlpha2( &lBrAlpha2 );
				LONG lBrColor1 = 0; pRenderer->get_BrushColor1( &lBrColor1 );
				LONG lBrColor2 = 0; pRenderer->get_BrushColor2( &lBrColor2 );
				double lBrAngle = 0; pRenderer->get_BrushLinearAngle( &lBrAngle );
				LONG lBrTxMode = 0; pRenderer->get_BrushTextureMode( &lBrTxMode );
				LONG lBrTxAlpha = 0; pRenderer->get_BrushTextureAlpha( &lBrTxAlpha );
				std::wstring bsTxPath = NULL; pRenderer->get_BrushTexturePath( &bsTxPath );
				LONG lBrType = 0; pRenderer->get_BrushType( &lBrType );

				std::wstring bsFontPath	= NULL; pRenderer->get_FontPath(&bsFontPath);
				std::wstring bsFontName	= NULL; pRenderer->get_FontName(&bsFontName);
				double dFontSize = 0; pRenderer->get_FontSize(&dFontSize);
				LONG lFontStyle = 0; pRenderer->get_FontStyle(&lFontStyle);
				INT bFontGid = 0; pRenderer->get_FontStringGID( &bFontGid );
				
				LONG lPenAlpha = 0; pRenderer->get_PenAlpha(&lPenAlpha);
				LONG lPenColor = 0; pRenderer->get_PenColor(&lPenColor);
				LONG lPenAlign = 0; pRenderer->get_PenAlign(&lPenAlign);
				double dPenSize = 0; pRenderer->get_PenSize(&dPenSize);
// todooo что тут????
				//AVSGraphics::IAVSImage *pImage = NULL;
				//CoCreateInstance( __uuidof(AVSGraphics::CAVSImage), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSImage), (void**)(&pImage) );

				//pImage->put_FontManager(NULL);

				//pImage->LoadFromFile( strFile );
				//pImage->DrawOnRenderer( pRenderer, dX, dY, dW, dH );

				//RELEASEINTERFACE( pImage );

				// Восстанавливаем параметры

				pRenderer->put_BrushAlpha1( lBrAlpha1 );
				pRenderer->put_BrushAlpha2( lBrAlpha2 );
				pRenderer->put_BrushColor1( lBrColor1 );
				pRenderer->put_BrushColor2( lBrColor2 );
				pRenderer->put_BrushLinearAngle( lBrAngle );
				pRenderer->put_BrushTextureMode( lBrTxMode );
				pRenderer->put_BrushTextureAlpha( lBrTxAlpha );
				pRenderer->put_BrushTexturePath( bsTxPath ); 
				
				pRenderer->put_BrushType( lBrType );

				pRenderer->put_FontName( bsFontName );
				pRenderer->put_FontPath( bsFontPath );
				pRenderer->put_FontSize( dFontSize );
				pRenderer->put_FontStyle( lFontStyle );
				pRenderer->put_FontStringGID( bFontGid );

				pRenderer->put_PenAlign( lPenAlign );
				pRenderer->put_PenAlpha( lPenAlpha );
				pRenderer->put_PenColor( lPenColor );
				pRenderer->put_PenSize(dPenSize);

				// на всякий случай скидываем path
				pRenderer->PathCommandEnd();

                return true;
			}
		
            return false;
		}
#endif
	};
}

#include "CMetafile.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/graphics/Image.h"
#include "../DesktopEditor/graphics/structures.h"
#include "../DesktopEditor/fontengine/GlyphString.h"
#include "CASCImage.h"

namespace NSHtmlRenderer
{
	LONG CImageExt::GetImageType(const std::wstring& strFile)
	{		
		NSFile::CFileBinary oFile;
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
		DWORD dwSizeRead;
		oFile.ReadFile((BYTE*)(&dwKey), 4, dwSizeRead);

		if (0x9AC6CDD7 == dwKey)
		{
			// placeable meta
			oFile.CloseFile();
			return 1;
		}

		if (0x00000001 == dwKey)
		{
			BYTE aTemp[40];
			oFile.ReadFile((BYTE*)&aTemp, 40, dwSizeRead);
			oFile.ReadFile((BYTE*)(&dwKey), 4, dwSizeRead);
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

    void CImageExt::DrawOnRenderer(IRenderer* pRenderer, const std::wstring& strFile, const double& dX, const double& dY, const double& dW, const double& dH, const bool& bIsFromFileUse = true)
	{
		if (NULL == pRenderer)
			return;

		LONG lRendType = 0;
		pRenderer->get_Type(&lRendType);
		if (c_nPDFWriter == lRendType)
		{
			//todo
			//VARIANT var;
			//var.vt = VT_BOOL;
			//var.boolVal = VARIANT_TRUE;
			//pRenderer->SetAdditionalParam(L"WhiteBackImage", var);
		}

		LONG lFileType = 0;

		if (c_nEbookWriter != lRendType)
			lFileType = GetImageType(strFile);

        bool bIsGraphics = false;

		//todo
		//IASCGraphicsRenderer* pGR = NULL;
		//pRenderer->QueryInterface(__uuidof(IASCGraphicsRenderer), (void**)&pGR);

		//if (NULL != pGR)
		//{
		//	bIsGraphics = TRUE;
		//}

		//RELEASEINTERFACE(pGR);

		if (0 == lFileType)
		{
			// картинка
			if (bIsGraphics)
			{
				pRenderer->DrawImageFromFile(strFile, dX, dY, dW, dH);
			}
			else
			{
				Aggplus::CImage oImage(strFile);
				pRenderer->DrawImage(&oImage, dX, dY, dW, dH);
			}
		}
		else if (1 == lFileType || 2 == lFileType)
		{
			// Запоминаем все настройки Brush, Font, Pen, Shadow и т.д.
			NSStructures::CBrush oBrush;
			std::wstring bsBrushTexturePath;
			pRenderer->get_BrushAlpha1( &oBrush.Alpha1 );
			pRenderer->get_BrushAlpha2( &oBrush.Alpha2 );
			pRenderer->get_BrushColor1( &oBrush.Color1 );
			pRenderer->get_BrushColor2( &oBrush.Color2 );
			pRenderer->get_BrushLinearAngle( &oBrush.LinearAngle );
			pRenderer->get_BrushTextureMode( &oBrush.TextureMode );
			pRenderer->get_BrushTextureAlpha( &oBrush.TextureAlpha );
			pRenderer->get_BrushTexturePath( &bsBrushTexturePath );
			pRenderer->get_BrushType( &oBrush.Type );

			NSStructures::CFont oFont;
			std::wstring bsFontPath, bsFontName;
			pRenderer->get_FontName( &bsFontName );
			pRenderer->get_FontPath( &bsFontPath );
			pRenderer->get_FontSize( &oFont.Size );
			LONG lStyle;
			pRenderer->get_FontStyle( &lStyle );
			oFont.Bold      = lStyle & FontConstants::FontStyleBold;
			oFont.Italic    = lStyle & FontConstants::FontStyleItalic;
			oFont.Strikeout = lStyle & FontConstants::FontStyleStrikeout;
			oFont.Underline = lStyle & FontConstants::FontStyleUnderline;
			pRenderer->get_FontStringGID( &oFont.StringGID );

			NSStructures::CPen oPen;
			pRenderer->get_PenAlign( &oPen.Align );
			pRenderer->get_PenAlpha( &oPen.Alpha );
			pRenderer->get_PenColor( &oPen.Color );

            //todo font manager
            CApplicationFonts *pAppFonts = NULL;
            NSHtmlRenderer::CASCImage oImage(pAppFonts);

			oImage.LoadFromFile( strFile );
			oImage.DrawOnRenderer( pRenderer, dX, dY, dW, dH );

			// Восстанавливаем параметры

			pRenderer->put_BrushAlpha1( oBrush.Alpha1 );
			pRenderer->put_BrushAlpha2( oBrush.Alpha2 );
			pRenderer->put_BrushColor1( oBrush.Color1 );
			pRenderer->put_BrushColor2( oBrush.Color2 );
			pRenderer->put_BrushLinearAngle( oBrush.LinearAngle );
			pRenderer->put_BrushTextureMode( oBrush.TextureMode );
			pRenderer->put_BrushTextureAlpha( oBrush.TextureAlpha );
			pRenderer->put_BrushTexturePath( bsBrushTexturePath );
			pRenderer->put_BrushType( oBrush.Type );

			pRenderer->put_FontName( bsFontName );
			pRenderer->put_FontPath( bsFontPath );
			pRenderer->put_FontSize( oFont.Size );
			pRenderer->put_FontStyle( oFont.GetStyle() );
			pRenderer->put_FontStringGID( oFont.StringGID );

			pRenderer->put_PenAlign( oPen.Align );
			pRenderer->put_PenAlpha( oPen.Alpha );
			pRenderer->put_PenColor( oPen.Color );

			// на всякий случай скидываем path
			pRenderer->PathCommandEnd();
		}
	}
}

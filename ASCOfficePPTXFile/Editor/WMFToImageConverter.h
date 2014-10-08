#pragma once
#include "../stdafx.h"
#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../ASCHTMLRenderer/ASCSVGWriter.h"
#include "../../ASCHTMLRenderer/CASCImage.h"

#ifdef BUILD_CONFIG_FULL_VERSION

namespace NSWMFToImageConverter
{
	class CImageExt
	{
	private:
		NSHtmlRenderer::CASCSVGWriter* m_pSVGRenderer;

		void Init()
		{
			if (NULL != m_pSVGRenderer)
			{
				m_pSVGRenderer->ReInit();
			}
			else
			{
				m_pSVGRenderer = new NSHtmlRenderer::CASCSVGWriter();
			}
		}

	public:
		CImageExt()
		{
			m_pSVGRenderer = NULL;
		}
		~CImageExt()
		{
			RELEASEOBJECT(m_pSVGRenderer);
		}		

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

	private:
		void MetaDrawOnRenderer(IRenderer* pRenderer, BSTR strFile, double dW, double dH)
		{
			if (NULL == pRenderer)
				return;

			// Запоминаем все настройки Brush, Font, Pen, Shadow и т.д.
			LONG brush_Alpha1 = 0;
			LONG brush_Alpha2 = 0;
			LONG brush_Color1 = 0;
			LONG brush_Color2 = 0;
			double brush_LinearAngle = 0;
			std::wstring brush_TexturePath;
			LONG brush_TextureMode = 0;
			LONG brush_TextureAlpha = 0;
			LONG brush_Type = 0;

			pRenderer->get_BrushAlpha1( &brush_Alpha1 );
			pRenderer->get_BrushAlpha2( &brush_Alpha2 );
			pRenderer->get_BrushColor1( &brush_Color1 );
			pRenderer->get_BrushColor2( &brush_Color2 );
			pRenderer->get_BrushLinearAngle( &brush_LinearAngle );
			pRenderer->get_BrushTextureMode( &brush_TextureMode );
			pRenderer->get_BrushTextureAlpha( &brush_TextureAlpha );
			pRenderer->get_BrushTexturePath( &brush_TexturePath );
			pRenderer->get_BrushType( &brush_Type );

			std::wstring font_Path;
			std::wstring font_Name;
			double font_Size = 0;
			LONG font_Style = 0;
			BOOL font_GID = 0;

			pRenderer->get_FontName( &font_Name );
			pRenderer->get_FontPath( &font_Path );
			pRenderer->get_FontSize( &font_Size );
			pRenderer->get_FontStyle( &font_Style );
			pRenderer->get_FontStringGID( &font_GID );

			LONG pen_Align = 0;
			LONG pen_Alpha = 0;
			LONG pen_Color = 0;
			pRenderer->get_PenAlign( &pen_Align );
			pRenderer->get_PenAlpha( &pen_Alpha );
			pRenderer->get_PenColor( &pen_Color );

			NSHtmlRenderer::CASCImage oASCImage;
			oASCImage.put_FontManager(NULL);

			oASCImage.LoadFromFile( strFile );

			if (dW <= 0 && dH <= 0)
			{
				LONG _lw = 0;
				LONG _lh = 0;
				oASCImage.get_Width(&_lw);
				oASCImage.get_Height(&_lh);

				LONG lMax = max(_lw, _lh);
				double dKoef = 1000.0 / lMax;
				
				dW = (double)dKoef * _lw * 100;
				dH = (double)dKoef * _lh * 100;

				m_pSVGRenderer->put_Width(dW);
				m_pSVGRenderer->put_Height(dH);
			}

			oASCImage.DrawOnRenderer( pRenderer, 0, 0, dW, dH );

			// Восстанавливаем параметры
			pRenderer->put_BrushAlpha1( brush_Alpha1 );
			pRenderer->put_BrushAlpha2( brush_Alpha2 );
			pRenderer->put_BrushColor1( brush_Color1 );
			pRenderer->put_BrushColor2( brush_Color2 );
			pRenderer->put_BrushLinearAngle( brush_LinearAngle );
			pRenderer->put_BrushTextureMode( brush_TextureMode );
			pRenderer->put_BrushTextureAlpha( brush_TextureAlpha );
			pRenderer->put_BrushTexturePath( brush_TexturePath );
			pRenderer->put_BrushType( brush_Type );

			pRenderer->put_FontName( font_Name );
			pRenderer->put_FontPath( font_Path );
			pRenderer->put_FontSize( font_Size );
			pRenderer->put_FontStyle( font_Style );
			pRenderer->put_FontStringGID( font_GID );

			pRenderer->put_PenAlign( pen_Align );
			pRenderer->put_PenAlpha( pen_Alpha );
			pRenderer->put_PenColor( pen_Color );

			// на всякий случай скидываем path
			pRenderer->PathCommandEnd();
		}
	public:
		bool Convert(CString strPath, LONG lWidth, LONG lHeight, CString strDstPath)
		{
			Init();

			m_pSVGRenderer->put_Width(lWidth);
			m_pSVGRenderer->put_Height(lHeight);

			BSTR bsFilePath = strPath.AllocSysString();
			MetaDrawOnRenderer(m_pSVGRenderer, bsFilePath, (double)lWidth, (double)lHeight);
			SysFreeString(bsFilePath);

			bool bIsRaster = false;
			m_pSVGRenderer->IsRaster(&bIsRaster);
			if (bIsRaster)
				return false;

			m_pSVGRenderer->SaveFile(std::wstring(strDstPath.GetString()));

			return true;
		}
	};
}

#endif
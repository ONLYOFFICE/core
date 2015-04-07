
#pragma once

#include <directx9\dx_core.h>
#include <directx9\dx_shaders.h>
#include <directx9\dx_render.h>


namespace ImageStudio
{
	namespace dx9
	{
		class IEffect
		{
		public:
			enum
			{
				c_nMetricPixels = 0,
				c_nMetricPercents = 1,
				c_nMetricNormalized = 2,
				c_nMetricNormalizedEx = 3,
			};

			
		public:
			static double GetMetric( int metric, int size )
			{
				switch( metric )
				{
				case c_nMetricPercents: return size * 0.01;
				case c_nMetricNormalized: return size;
				case c_nMetricNormalizedEx: return size;
				default: return 1;
				}
			}



			virtual int Prepare( dx9::CRender& render, double dCompleteness ) = 0;
			virtual BOOL Apply( dx9::CRender& render, IDirect3DTexture9* pSource ) const = 0;
		};

		class ISimpleEffect : public IEffect
		{
		protected:
			EPixelShader m_eShader;
			double m_dFrame;
		
		public:
			ISimpleEffect( EPixelShader shader )
			{
				m_eShader = shader;
				m_dFrame = 0;
			}

			virtual int Prepare( dx9::CRender& render, double dCompleteness )
			{
				// return 0 - ok, 1 - not update, -1 - error

				if( render.IsBadPixelShader( m_eShader ) )
					return -1;

				m_dFrame = 0;

				if( dCompleteness <= 0 )
					return 1;
				
				m_dFrame = min(dCompleteness, 1);

				return CheckParams();
			}

			virtual BOOL Apply( dx9::CRender& render, IDirect3DTexture9* pSource ) const
			{
				if( render.IsBadDevice() )
					return FALSE;

				if( !pSource )
					return FALSE;

				D3DSURFACE_DESC desc;
				HRESULT hr = pSource->GetLevelDesc( 0, &desc );
				if( render.CheckError(hr) )
					return FALSE;

				if( !desc.Width || !desc.Height || desc.Format != D3DFMT_A8R8G8B8 )
					return FALSE;

				if( !render.LockDevice() )
					return FALSE;

				pSource->AddRef();

				BOOL bSuccess = Update( render, pSource, desc.Width, desc.Height );

				pSource->Release();

				render.UnlockDevice();

				return bSuccess;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const = 0;

			virtual int CheckParams()
			{
				// return 0 - ok, 1 - not update, -1 - error
				return 0;
			}
		};

		class EffectRipple : public ISimpleEffect
		{
		public:
			double m_dOmega;
			double m_dOffset;

		public:
			EffectRipple() :
				ISimpleEffect( c_nPS_Ripple )
			{
				m_dOmega = 0;
				m_dOffset = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				PS_Ripple shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_Ripple ) ) )
					return FALSE;

				//Test::Test_PS_Ripple shader( _T("d:\\ps_ripple.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				double size = sqrt(double(nWidth * nWidth + nHeight * nHeight));

				shader.m_fOmega   = float(m_dOmega * IPP_2PI / size);
				shader.m_fDisp    = float(m_dOffset * IPP_2PI);
				shader.m_fAmpl    = float(size * m_dFrame);
				shader.m_fWidth   = float(nWidth);
				shader.m_fHeight  = float(nHeight);
				shader.m_pTexture = pSource;
				shader.m_eFilter  = render.GetFilter();

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}

		};
		class EffectTwirl : public ISimpleEffect
		{
		public:
			double m_dAngle;
			double m_dDegree;
			double m_dZoom;
			
			D3DCOLOR m_nBackColor;

		public:
			EffectTwirl() :
				ISimpleEffect( c_nPS_Twirl )
			{
				m_dAngle     = 0;
				m_dDegree    = 1; 
				m_dZoom      = 1;
				m_nBackColor = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				PS_Twirl shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_Twirl ) ) )
					return FALSE;
				
				//Test::Test_PS_Twirl shader( _T("d:\\ps_twirl.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				shader.m_fAngle   = float(m_dAngle * m_dFrame * IPP_PI180);
				shader.m_fDegree  = float(m_dDegree);
				shader.m_fZoom    = float(1 + (m_dZoom - 1) * m_dFrame);
				shader.m_fWidth   = float(nWidth);
				shader.m_fHeight  = float(nHeight);
				shader.m_nBackColor = m_nBackColor;
				shader.m_pTexture = pSource;
				shader.m_eFilter  = render.GetFilter();
				
				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}

		};

		class EffectZoom : public ISimpleEffect
		{
		public:
			double m_dZoomX;
			double m_dZoomY;
			double m_dDispX;
			double m_dDispY;
			
			int m_nMetric;
			D3DCOLOR m_nBackColor;
			BOOL m_bResample;

		public:
			EffectZoom() :
				ISimpleEffect( c_nPS_Zoom )
			{
				m_dZoomX = 1;
				m_dZoomY = 1; 
				m_dDispX = 0;
				m_dDispY = 0;
				
				m_nMetric = 0;
				m_nBackColor = 0;
				m_bResample = FALSE;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				PS_Zoom shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_Zoom ) ) )
					return FALSE;
				
				//Test::Test_PS_Zoom shader( _T("d:\\ps_zoom.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				double dMetricX = GetMetric( m_nMetric, nWidth );
				double dMetricY = GetMetric( m_nMetric, nHeight );

				if( m_dFrame < 1 )
				{
					shader.m_fZoomX = float(1 + (m_dZoomX - 1) * m_dFrame);
					shader.m_fZoomY = float(1 + (m_dZoomY - 1) * m_dFrame);
					shader.m_fDispX = float(m_dDispX * dMetricX * m_dFrame);
					shader.m_fDispY = float(m_dDispY * dMetricY * m_dFrame);
				}
				else
				{
					// коррекция, что бы не было дробных координат (расплывчатых границ)
					shader.m_fZoomX = float(Round(m_dZoomX * nWidth)  / double(nWidth));
					shader.m_fZoomY = float(Round(m_dZoomY * nHeight) / double(nHeight));
					shader.m_fDispX = float(Round(m_dDispX * dMetricX));
					shader.m_fDispY = float(Round(m_dDispY * dMetricY));
				}

				shader.m_fWidth  = float(nWidth);
				shader.m_fHeight = float(nHeight);

				shader.m_nBackColor = m_nBackColor;
				shader.m_pTexture   = pSource;
				shader.m_eFilter    = render.GetFilter();
				shader.m_bResample  = m_bResample;
				
				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};

		class EffectRotate : public ISimpleEffect
		{
		public:
			double m_dAngle;
			
			D3DCOLOR m_nBackColor;

			int m_nType;

		public:
			EffectRotate() :
				ISimpleEffect( c_nPS_LinearTransform )
			{
				m_dAngle = 0;
				m_nBackColor = 0;
				m_nType = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				PS_LinearTransform shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_LinearTransform ) ) )
					return FALSE;
				
				//Test::Test_PS_LinearTransform shader( _T("d:\\ps_mul_matrix3x2.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				shader.m_nBackColor = m_nBackColor;
				shader.m_eFilter = render.GetFilter();

				double dSin = sin(m_dAngle * IPP_PI180 * m_dFrame );
				double dCos = cos(m_dAngle * IPP_PI180 * m_dFrame );

				int nTargetW = nWidth;
				int nTargetH = nHeight;
				int nSourceW = nWidth;
				int nSourceH = nHeight;

				double dp[4][2] = {
					0, 0,
					nWidth, 0,
					nWidth, nHeight,
					0, nHeight
				};

				double dHalfW = nWidth  / 2.0;
				double dHalfH = nHeight / 2.0;

				// перенос в центр и поворот
				for( int i = 0; i < 4; i++ )
				{
					double x = dp[i][0] - dHalfW;
					double y = dp[i][1] - dHalfH;

					dp[i][0] = x * dCos - y * dSin;
					dp[i][1] = x * dSin + y * dCos;
				}

				double dMinX = 0;
				double dMaxX = 0;
				double dMinY = 0;
				double dMaxY = 0;

				for( int i = 0; i < 4; i++ )
				{
					if( dp[i][0] < dMinX ) dMinX = dp[i][0]; else
					if( dp[i][0] > dMaxX ) dMaxX = dp[i][0];
					if( dp[i][1] < dMinY ) dMinY = dp[i][1]; else
					if( dp[i][1] > dMaxY ) dMaxY = dp[i][1];
				}

				if( m_nType != 1 && m_nType != 2 )
				{
					nTargetW = Ceil( dMaxX - dMinX );
					nTargetH = Ceil( dMaxY - dMinY );
				}

				if( 1 == m_nType )
				{
					// масштабируем картинку, что бы она вписалась в наше исходное изображение
					double dw = dMaxX - dMinX;
					double dh = dMaxY - dMinY;

					double dFactor = ((dw - nWidth) > (dh - nHeight)) ? dw / nWidth : dh / nHeight;
					if( dFactor < 0.0001 )
						dFactor = 0.0001;

					nSourceW = Round( nWidth  / dFactor );
					nSourceH = Round( nHeight / dFactor );
					
					//if( nSourceW != desc.Width || nSourceH != desc.Height )
					//{
					//	IDirect3DTexture9* pResizeSource = render.ResizeTexture( pSource, nSourceW, nSourceH );
					//	if( !pResizeSource )
					//		break;

					//	pSource->Release();
					//	pSource = pResizeSource;
					//}
				}

				shader.m_matrix[0][0] = float( dCos * nTargetW / nSourceW);
				shader.m_matrix[0][1] = float( dSin * nTargetW / nSourceH);
				shader.m_matrix[1][0] = float(-dSin * nTargetH / nSourceW);
				shader.m_matrix[1][1] = float( dCos * nTargetH / nSourceH);
				shader.m_matrix[2][0] = float((nSourceW - dCos * nTargetW + dSin * nTargetH) / (2 * nSourceW));
				shader.m_matrix[2][1] = float((nSourceH - dCos * nTargetH - dSin * nTargetW) / (2 * nSourceH));
				shader.m_pTexture = pSource;

				if( !render.CreateTarget( nTargetW, nTargetH ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};

		class EffectSkew : public ISimpleEffect
		{
		public:
			double m_dAngle;
			BOOL   m_bVertical;
			
			D3DCOLOR m_nBackColor;

		public:
			EffectSkew() :
				ISimpleEffect( c_nPS_LinearTransform )
			{
				m_dAngle = 0;
				m_bVertical = FALSE;
				m_nBackColor = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				double dAngle = m_dAngle * m_dFrame;
				
				dAngle -= int(dAngle / 360) * 360;
				
				if( dAngle < -180 )	dAngle += 360; else
				if( dAngle >  180 ) dAngle -= 360;
				
				double dFactorX = abs(dAngle) / 90;
				double dFactorY = abs(dAngle) / 90;
				
				if( m_bVertical )
					dFactorY = 1 - abs(dFactorY - 1);
				else
					dFactorX = 1 - abs(dFactorX - 1);

				double dX1 = 0.5 * dFactorX;
				double dY1 = 0.5 * dFactorY;
				
				if( 1 == m_dFrame )
				{
					dX1 = double(Round( dX1 * nWidth  )) / nWidth;
					dY1 = double(Round( dY1 * nHeight )) / nHeight;
				}

				double dX2 = 1 - dX1;
				double dY2 = 1 - dY1;

				TPoint quad[4]; // {(0,0),(1,0),(1,1),(0,1)};

				// check for transform direction
				if( m_bVertical )
				{
					quad[0].x = quad[3].x = dX1;
					quad[1].x = quad[2].x = dX2;
					
					if( dAngle > 0 )
					{
						quad[0].y = dY1;
						quad[1].y = 0;
						quad[2].y = dY2;
						quad[3].y = 1;
					}
					else
					{
						quad[0].y = 0;
						quad[1].y = dY1;
						quad[2].y = 1;
						quad[3].y = dY2;
					}
				}
				else
				{
					quad[0].y = quad[1].y = dY1;
					quad[2].y = quad[3].y = dY2;
					
					if( dAngle > 0 )
					{
						quad[0].x = dX1;
						quad[1].x = 1;
						quad[2].x = dX2;
						quad[3].x = 0;
					}
					else
					{
						quad[0].x = 0;
						quad[1].x = dX2;
						quad[2].x = 1;
						quad[3].x = dX1;
					}
				}

				PS_LinearTransform shader;

				CMatrix3x2 matrix;
				BOOL bDraw = matrix.Build2( quad );
				if( bDraw )
				{
					if( !shader.Set( render.GetPixelShader( c_nPS_LinearTransform ) ) )
						return FALSE;
					
					//Test::Test_PS_LinearTransform shader( _T("d:\\ps_mul_matrix3x2.txt") );
					//if( !shader.Create( render.GetDevice() ) )
					//	return FALSE;

					shader.m_nBackColor = m_nBackColor;
					shader.m_eFilter    = render.GetFilter();
					shader.m_pTexture   = pSource;

					matrix.Get( shader.m_matrix );
				}

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( bDraw )
				{
					if( !render.Draw( &shader ) )
						return FALSE;
				}
				else
				{
					if( !render.FillTarget( m_nBackColor ) )
						return FALSE;
				}

				return TRUE;
			}
		};

		class EffectSphere : public ISimpleEffect
		{
		public:
			double m_dCenterX;
			double m_dCenterY;
			double m_dRadiusX;
			double m_dRadiusY;
			double m_dDegree;
			
			int m_nMetric;
			D3DCOLOR m_nBackColor;

		public:
			EffectSphere() :
				ISimpleEffect( c_nPS_Sphere )
			{
				m_dCenterX = 0;
				m_dCenterY = 0;
				m_dRadiusX = 0;
				m_dRadiusY = 0;
				m_dDegree  = 1;
				
				m_nMetric = 0;
				m_nBackColor = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				PS_Sphere shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_Sphere ) ) )
					return FALSE;
				
				//Test::Test_PS_Sphere shader( _T("d:\\ps_sphere.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				double dMetricX = GetMetric( m_nMetric, nWidth );
				double dMetricY = GetMetric( m_nMetric, nHeight );

				double dDegree  = min(max(-10, m_dDegree), 30) / 10;

				shader.m_fCenterX = float(m_dCenterX * dMetricX);
				shader.m_fCenterY = float(m_dCenterY * dMetricY);
				shader.m_fRadiusX = float(m_dRadiusX * dMetricX * m_dFrame);
				shader.m_fRadiusY = float(m_dRadiusY * dMetricY * m_dFrame);
				shader.m_fDegree  = float(dDegree /** dFrame*/);

				shader.m_fWidth   = float(nWidth);
				shader.m_fHeight  = float(nHeight);

				shader.m_nBackColor = m_nBackColor;
				shader.m_pTexture   = pSource;
				shader.m_eFilter    = render.GetFilter();
				
				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};

		class EffectCylinder : public ISimpleEffect
		{
		public:
			double m_dCenter;
			double m_dRadius;
			double m_dDegree;
			BOOL m_bVertical;
			
			int m_nMetric;
			D3DCOLOR m_nBackColor;

		public:
			EffectCylinder() :
				ISimpleEffect( c_nPS_CylinderVer )
			{
				m_dCenter = 0;
				m_dRadius = 0;
				m_dDegree = 1;
				m_bVertical = FALSE;
				
				m_nMetric = 0;
				m_nBackColor = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				PS_CylinderVer shader1;
				PS_CylinderHor shader2;

				double dDegree = min(max(-10, m_dDegree), 30) / 10;

				if( m_bVertical )
				{
					if( !shader1.Set( render.GetPixelShader( c_nPS_CylinderVer ) ) )
						return FALSE;

					double dMetric = GetMetric( m_nMetric, nWidth );

					shader1.m_fCenter = float(m_dCenter * dMetric);
					shader1.m_fRadius = float(m_dRadius * dMetric * m_dFrame);
					shader1.m_fDegree = float(dDegree /** dFrame*/);
					shader1.m_fWidth  = float(nWidth);

					shader1.m_nBackColor = m_nBackColor;
					shader1.m_pTexture   = pSource;
					shader1.m_eFilter    = render.GetFilter();
				}
				else
				{
					if( !shader2.Set( render.GetPixelShader( c_nPS_CylinderHor ) ) )
						return FALSE;

					double dMetric = GetMetric( m_nMetric, nHeight );

					shader2.m_fCenter = float(m_dCenter * dMetric);
					shader2.m_fRadius = float(m_dRadius * dMetric * m_dFrame);
					shader2.m_fDegree = float(dDegree /** dFrame*/);
					shader2.m_fHeight = float(nHeight);

					shader2.m_nBackColor = m_nBackColor;
					shader2.m_pTexture   = pSource;
					shader2.m_eFilter    = render.GetFilter();
				}

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( m_bVertical )
				{
					if( !render.Draw( &shader1 ) )
						return FALSE;
				}
				else
				{
					if( !render.Draw( &shader2 ) )
						return FALSE;
				}

				return TRUE;
			}
		};

		class EffectWave : public ISimpleEffect
		{
		public:
			double m_dAmplitude;
			double m_dPhase;
			double m_dOffset;
			BOOL m_bVertical;
			
			int m_nMetric;

		public:
			EffectWave() :
				ISimpleEffect( c_nPS_WaveVer )
			{
				m_dAmplitude = 0;
				m_dPhase = 0;
				m_dOffset = 0;
				m_bVertical = FALSE;
				
				m_nMetric = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				PS_WaveVer shader1;
				PS_WaveHor shader2;

				if( m_bVertical )
				{
					if( !shader1.Set( render.GetPixelShader( c_nPS_WaveVer ) ) )
						return FALSE;

					double dMetric = GetMetric( m_nMetric, nWidth );
					double dAmplitude = m_dAmplitude * dMetric * m_dFrame;

					double w = (nWidth - 1) / 2.0;
					if( dAmplitude > w )
						dAmplitude = w;
					if( dAmplitude < -w )
						dAmplitude = -w;

					shader1.m_fAmpl  = float(dAmplitude);
					shader1.m_fPhase = float(m_dPhase);
					shader1.m_fDisp  = float(m_dOffset);
					shader1.m_fWidth = float(nWidth);

					shader1.m_pTexture = pSource;
					shader1.m_eFilter  = render.GetFilter();
				}
				else
				{
					if( !shader2.Set( render.GetPixelShader( c_nPS_WaveHor ) ) )
						return FALSE;

					double dMetric = GetMetric( m_nMetric, nHeight );
					double dAmplitude = m_dAmplitude * dMetric * m_dFrame;

					double h = (nHeight - 1) / 2.0;
					if( dAmplitude > h )
						dAmplitude = h;
					if( dAmplitude < -h )
						dAmplitude = -h;

					shader2.m_fAmpl  = float(dAmplitude);
					shader2.m_fPhase = float(m_dPhase);
					shader2.m_fDisp  = float(m_dOffset);
					shader2.m_fHeight = float(nHeight);

					shader2.m_pTexture = pSource;
					shader2.m_eFilter  = render.GetFilter();
				}

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( m_bVertical )
				{
					if( !render.Draw( &shader1 ) )
						return FALSE;
				}
				else
				{
					if( !render.Draw( &shader2 ) )
						return FALSE;
				}

				return TRUE;
			}
		};

		class EffectHLS : public ISimpleEffect
		{
		public:
			double m_dShiftHueAngle;
			double m_dShiftSaturation;
			double m_dFactorSaturation;

		public:
			EffectHLS() :
				ISimpleEffect( c_nPS_HLS )
			{
				m_dShiftHueAngle = 0;
				m_dShiftSaturation = 0;
				m_dFactorSaturation = 1;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_HLS shader( _T("d:\\ps_hls.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;
				
				PS_HLS shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_HLS ) ) )
					return FALSE;

				double dFactor = max(1, m_dFactorSaturation);
				double dSat = m_dShiftSaturation / 100 * m_dFrame + 1;
				if( dSat < 0 ) dSat = 0;
				if( dSat > 1 ) dSat = (dSat - 1) * dFactor + 1;

				shader.m_fShiftSat = float(dSat);
				shader.m_fShiftHue = float(m_dShiftHueAngle * m_dFrame);
				shader.m_pTexture = pSource;

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};

		class EffectSharpen : public ISimpleEffect
		{
		public:
			double m_dLevel;

		public:
			EffectSharpen() :
				ISimpleEffect( c_nPS_FilterCenter3x3 )
			{
				m_dLevel = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_Filter3x3 shader( _T("d:\\ps_filter_3x3.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;
				
				PS_FilterCenter3x3 shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_FilterCenter3x3 ) ) )
					return FALSE;

				double level  = sqrt(Clamp(m_dLevel / 100));
				double factor = m_dFrame / (23 * (1 - level) + 1);

				//shader.m_matrix[0][0] = float(-factor);
				//shader.m_matrix[0][1] = float(-factor);
				//shader.m_matrix[0][2] = float(-factor);
				//shader.m_matrix[1][0] = float(-factor);
				//shader.m_matrix[1][1] = float(factor * 8 + 1);
				//shader.m_matrix[1][2] = float(-factor);
				//shader.m_matrix[2][0] = float(-factor);
				//shader.m_matrix[2][1] = float(-factor);
				//shader.m_matrix[2][2] = float(-factor);

				shader.m_fCenter    = float(factor * 8 + 1);
				shader.m_fBorder    = float(-factor);
				shader.m_fSumFactor = 1;
				shader.m_fSumOffset = 0.5f / 255;
				//shader.m_factor = 1;
				//shader.m_offset = 0.5f / 255;

				shader.m_fWidth     = float(nWidth);
				shader.m_fHeight    = float(nHeight);
				shader.m_pTexture   = pSource;

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};

		class EffectBlur : public ISimpleEffect
		{
		public:
			double m_dLevel;

		public:
			EffectBlur() :
				ISimpleEffect( c_nPS_FilterCenter3x3 )
			{
				m_dLevel = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_Filter3x3 shader( _T("d:\\ps_filter_3x3.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;
				
				PS_FilterCenter3x3 shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_FilterCenter3x3 ) ) )
					return FALSE;

				double level  = min(100, max(0, m_dLevel));
				double factor = m_dFrame / (20 * (1 - m_dFrame * level / 100) + 9);

				//shader.m_matrix[0][0] = float(factor);
				//shader.m_matrix[0][1] = float(factor);
				//shader.m_matrix[0][2] = float(factor);
				//shader.m_matrix[1][0] = float(factor);
				//shader.m_matrix[1][1] = float(1 - 8 * factor);
				//shader.m_matrix[1][2] = float(factor);
				//shader.m_matrix[2][0] = float(factor);
				//shader.m_matrix[2][1] = float(factor);
				//shader.m_matrix[2][2] = float(factor);

				shader.m_fCenter    = float(1 - 8 * factor);
				shader.m_fBorder    = float(factor);
				shader.m_fSumFactor = 1;
				shader.m_fSumOffset = 0.5f / 255;
				//shader.m_factor = 1;
				//shader.m_offset = 0.5f / 255;

				shader.m_fWidth     = float(nWidth);
				shader.m_fHeight    = float(nHeight);
				shader.m_pTexture   = pSource;

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};


		class EffectPerspective : public ISimpleEffect
		{
		public:
			double m_dAngle;
			BOOL   m_bVertical;
			
			BOOL   m_bAlternative;
			TPoint m_oPoints[4];
			
			D3DCOLOR m_nBackColor;

		public:
			EffectPerspective() :
				ISimpleEffect( c_nPS_PerspectiveTransform )
			{
				m_dAngle = 0;
				m_bVertical = FALSE;

				m_bAlternative = FALSE;
				m_oPoints[0].x = 0;
				m_oPoints[0].y = 0;
				m_oPoints[1].x = 1;
				m_oPoints[1].y = 0;
				m_oPoints[2].x = 1;
				m_oPoints[2].y = 1;
				m_oPoints[3].x = 0;
				m_oPoints[3].y = 1;

				m_nBackColor = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				TPoint quad[4];

				if( !m_bAlternative )
				{
					double dAngle = m_dFrame * m_dAngle;

					dAngle -= int(dAngle / 360) * 360;
					
					if( dAngle < -180 )	dAngle += 360; else
					if( dAngle >  180 ) dAngle -= 360;
					
					double dFactorX = abs(dAngle) / 90;
					double dFactorY = abs(dAngle) / 90;
					
					if( m_bVertical )
						dFactorY = 1 - abs(dFactorY - 1);
					else
						dFactorX = 1 - abs(dFactorX - 1);

					double x1 = dFactorX * (m_bVertical ? 0.50 : 0.25);
					double y1 = dFactorY * (m_bVertical ? 0.25 : 0.50);

					if( 1 == m_dFrame )
					{
						x1 = double(Round(x1 * nWidth))  / nWidth;
						y1 = double(Round(y1 * nHeight)) / nHeight;
					}

					double x2 = 1 - x1;
					double y2 = 1 - y1;

					// check for transform center line (vertical or horizontal)
					if( m_bVertical)
					{
						quad[0].x = quad[3].x = x1;
						quad[1].x = quad[2].x = x2;
						
						if( dAngle < 0)
						{
							quad[0].y = 0;
							quad[3].y = 1;
							quad[1].y = y1;
							quad[2].y = y2;
						}
						else
						{
							quad[0].y = y1;
							quad[3].y = y2;
							quad[1].y = 0;
							quad[2].y = 1;
						}
					}
					else
					{
						quad[0].y = quad[1].y = y1;
						quad[2].y = quad[3].y = y2;
						
						if( dAngle > 0 )
						{
							quad[0].x = x1;
							quad[1].x = x2;
							quad[2].x = 1;
							quad[3].x = 0;
						}
						else
						{
							quad[0].x = 0;
							quad[1].x = 1;
							quad[2].x = x2;
							quad[3].x = x1;
						}
					}
				}
				else
				{
					quad[0].x = (m_oPoints[0].x) * m_dFrame;
					quad[0].y = (m_oPoints[0].y) * m_dFrame;
					quad[1].x = (m_oPoints[1].x - 1) * m_dFrame + 1;
					quad[1].y = (m_oPoints[1].y) * m_dFrame;
					quad[2].x = (m_oPoints[2].x - 1) * m_dFrame + 1;
					quad[2].y = (m_oPoints[2].y - 1) * m_dFrame + 1;
					quad[3].x = (m_oPoints[3].x) * m_dFrame;
					quad[3].y = (m_oPoints[3].y - 1) * m_dFrame + 1;
				}

				//Test::Test_PS_PerspectiveTransform shader( _T("d:\\ps_perspective.txt") );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;
				
				PS_PerspectiveTransform shader;

				BOOL bDraw = CMatrix3x3::Build2( quad, shader.m_matrix );
				if( bDraw )
				{
					if( !shader.Set( render.GetPixelShader( c_nPS_PerspectiveTransform ) ) )
						return FALSE;

					shader.m_width    = float(nWidth);
					shader.m_height   = float(nHeight);
					shader.m_pTexture = pSource;
					shader.m_nBackColor = m_nBackColor;
				}

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( bDraw )
				{
					if( !render.Draw( &shader ) )
						return FALSE;
				}
				else
				{
					if( !render.FillTarget( m_nBackColor ) )
						return FALSE;
				}

				return TRUE;
			}
		};

		class EffectMinimal : public ISimpleEffect
		{
		public:
			int m_nSize;

		public:
			EffectMinimal() :
				ISimpleEffect( c_nPS_Minimal )
			{
				m_nSize = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_Minimal shader( _T("d:\\ps_minimal.txt"), "ps_3_0" );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				int nSize = Round(m_nSize * m_dFrame);
				if( nSize <= 0 )
					nSize = 0;

				if( nSize > 255 )
					nSize = 255;

				PS_Minimal shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_Minimal ) ) )
					return FALSE;

				shader.m_count = nSize;
				shader.m_pTexture = pSource;
				shader.m_dx = float(1.0 / nWidth);
				shader.m_dy = 0;
				
				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				IDirect3DTexture9* pTexture = render.GetRenderTargetTexture();
				if( !pTexture )
					return FALSE;

				shader.m_pTexture = pTexture;
				shader.m_dx = 0;
				shader.m_dy = float(1.0 / nHeight);

				if( !render.CreateTarget( nWidth, nHeight ) )
				{
					pTexture->Release();
					return FALSE;
				}

				if( !render.Draw( &shader ) )
				{
					pTexture->Release();
					return FALSE;
				}

				pTexture->Release();

				return TRUE;
			}
		};

		class EffectMaximal : public ISimpleEffect
		{
		public:
			int m_nSize;

		public:
			EffectMaximal() :
				ISimpleEffect( c_nPS_Maximal )
			{
				m_nSize = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_Maximal shader( _T("d:\\ps_maximal.txt"), "ps_3_0" );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				int nSize = Round(m_nSize * m_dFrame);
				if( nSize <= 0 )
					nSize = 0;

				if( nSize > 255 )
					nSize = 255;

				PS_Maximal shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_Maximal ) ) )
					return FALSE;

				shader.m_count = nSize;
				shader.m_pTexture = pSource;
				shader.m_dx = float(1.0 / nWidth);
				shader.m_dy = 0;
				
				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				IDirect3DTexture9* pTexture = render.GetRenderTargetTexture();
				if( !pTexture )
					return FALSE;

				shader.m_pTexture = pTexture;
				shader.m_dx = 0;
				shader.m_dy = float(1.0 / nHeight);

				if( !render.CreateTarget( nWidth, nHeight ) )
				{
					pTexture->Release();
					return FALSE;
				}

				if( !render.Draw( &shader ) )
				{
					pTexture->Release();
					return FALSE;
				}

				pTexture->Release();

				return TRUE;
			}
		};

		class EffectMotionBlur : public ISimpleEffect
		{
		public:
			double m_dDistance;
			double m_dAngle;

		public:
			EffectMotionBlur() :
				ISimpleEffect( c_nPS_MotionBlur )
			{
				m_dDistance = 0;
				m_dAngle = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_MotionBlur shader( _T("d:\\ps_motion_blur.txt"), "ps_3_0" );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				PS_MotionBlur shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_MotionBlur ) ) )
					return FALSE;

				double angle = m_dAngle * IPP_PI180;
				double dCos = cos(angle);
				double dSin = sin(angle);

				int distance = Round(m_dDistance * m_dFrame);
				if( distance < 0 )
				{
					distance = -distance;
					dCos = -dCos;
					dSin = -dSin;
				}

				shader.m_count = distance;
				shader.m_dx = float(dCos / nWidth);
				shader.m_dy = float(dSin / nHeight);
				shader.m_pTexture = pSource;
				
				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};

		class EffectGaussinBlur : public ISimpleEffect
		{
		public:
			int m_nSize;

		public:
			EffectGaussinBlur() :
				ISimpleEffect( c_nPS_GaussinBlur )
			{
				m_nSize = 0;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_GaussinBlur shader( _T("d:\\ps_blur.txt"), "ps_3_0" );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				PS_GaussinBlur shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_GaussinBlur ) ) )
					return FALSE;

				int count = Round(m_nSize * m_dFrame);
				if( count < 0)
					count = 0;

				if( count > 100 )
					count = 100;

				shader.m_count = count;
				shader.m_dx = float(1.0 / nWidth);
				shader.m_dy = 0;
				shader.m_pTexture = pSource;
				
				IDirect3DTexture9* pBuffer = NULL;
				BOOL bSuccess = FALSE;

				do
				{
					if( !render.CreateTarget( nWidth, nHeight ) )
						break;

					if( !render.Draw( &shader ) )
						break;

					pBuffer = render.GetRenderTargetTexture();
					if( !pBuffer )
						break;

					shader.m_pTexture = pBuffer;
					shader.m_dx = 0;
					shader.m_dy = float(1.0 / nHeight);

					if( !render.CreateTarget( nWidth, nHeight ) )
						break;

					if( !render.Draw( &shader ) )
						break;

					RELEASEINTERFACE(pBuffer);
					pBuffer = render.GetRenderTargetTexture();
					if( !pBuffer )
						break;

					shader.m_pTexture = pBuffer;
					shader.m_dx = float(1.0 / nWidth);
					shader.m_dy = 0;

					if( !render.CreateTarget( nWidth, nHeight ) )
						break;

					if( !render.Draw( &shader ) )
						break;


					RELEASEINTERFACE(pBuffer);
					pBuffer = render.GetRenderTargetTexture();
					if( !pBuffer )
						break;

					shader.m_pTexture = pBuffer;
					shader.m_dx = 0;
					shader.m_dy = float(1.0 / nHeight);

					if( !render.CreateTarget( nWidth, nHeight ) )
						break;

					if( !render.Draw( &shader ) )
						break;

					bSuccess = TRUE;
				}while( 0 );


				RELEASEINTERFACE( pBuffer );
				
				return bSuccess;
			}
		};

		class EffectGlassMosaic : public ISimpleEffect
		{
		public:
			double m_dSizeX;
			double m_dSizeY;
			double m_dOffsetX;
			double m_dOffsetY;
			double m_dCurvature;
			double m_dScale;
			double m_dAngle;
			double m_dSmooth;
			double m_dMaxFactor;
			int    m_eQuality;


		public:
			EffectGlassMosaic() :
				ISimpleEffect( c_nPS_GlassMosaic )
			{
				m_dSizeX     = 1;
				m_dSizeY     = 1;
				m_dOffsetX   = 0;
				m_dOffsetY   = 0;
				m_dCurvature = 1;
				m_dScale     = 1;
				m_dAngle     = 0;
				m_dSmooth    = 1;
				m_dMaxFactor = 4;
				m_eQuality   = 2;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_GlassMosaic shader( _T("d:\\ps_glass_mosaic.txt"), "ps_3_0" );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				PS_GlassMosaic shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_GlassMosaic ) ) )
					return FALSE;

				double angle  = m_dAngle * m_dFrame * IPP_PI180;
				double sinVal = sin(angle);
				double cosVal = cos(angle);
				double dSizeX = /*IPP_PI*/ 1.0 / (m_dSizeX > 1 ? m_dSizeX : 1);
				double dSizeY = /*IPP_PI*/ 1.0 / (m_dSizeY > 1 ? m_dSizeY : 1);
			
				double Curvature = max(-100, min(100, m_dCurvature));
				
				Curvature *= abs(Curvature) * 0.1 * m_dFrame;
				double dLength = abs(Curvature) / 1000 * max(0, m_dMaxFactor) + 2;

				double dMaxFactorX = abs(Curvature) < IPP_EPS23 ? 1 : atan( (nWidth  * dLength) / abs(Curvature) ) / IPP_PI2;
				double dMaxFactorY = abs(Curvature) < IPP_EPS23 ? 1 : atan( (nHeight * dLength) / abs(Curvature) ) / IPP_PI2;

				DWORD nLevel = 4;
				switch( m_eQuality )
				{
				case 0: nLevel = 2; break;
				case 1: nLevel = 3; break;
				case 2: nLevel = 4; break;
				case 3: nLevel = 5; break;
				case 4: nLevel = 6; break;
				}

				shader.m_samples   = nLevel * nLevel + 1;
				shader.m_level     = float(nLevel);
				shader.m_smooth    = float(m_dSmooth);
				shader.m_curvature = float(Curvature);
				shader.m_rescale   = float((m_dScale - 1) * m_dFrame + 1);
				shader.m_factorX   = float(dMaxFactorX);
				shader.m_factorY   = float(dMaxFactorY);
				shader.m_scaleX    = float(dSizeX);
				shader.m_scaleY    = float(dSizeY);
				shader.m_dispX     = float(m_dOffsetX * m_dFrame);
				shader.m_dispY     = float(m_dOffsetY * m_dFrame);
				shader.m_width     = float(nWidth);
				shader.m_height    = float(nHeight);
				shader.m_sin       = float(sinVal);
				shader.m_cos       = float(cosVal);
				shader.m_pTexture  = pSource;

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				return TRUE;
			}
		};

		class EffectNewsprint : public ISimpleEffect
		{
		public:
			double m_dLevel;

		public:
			EffectNewsprint() :
				ISimpleEffect( c_nPS_Newsprint )
			{
				m_dLevel = 100;
			}

			virtual BOOL Update( dx9::CRender& render, IDirect3DTexture9* pSource, int nWidth, int nHeight ) const
			{
				//Test::Test_PS_Newsprint shader( _T("d:\\ps_newsprint.txt"), "ps_2_0" );
				//if( !shader.Create( render.GetDevice() ) )
				//	return FALSE;

				PS_Newsprint shader;
				if( !shader.Set( render.GetPixelShader( c_nPS_Newsprint ) ) )
					return FALSE;

				shader.m_level  = float(Clamp( (m_dLevel * 254 / 255) / 100 ));
				shader.m_alpha  = float(m_dFrame);
				shader.m_width  = float(nWidth);
				shader.m_height = float(nHeight);
				shader.m_pTexture = pSource;

				if( !render.CreateTarget( nWidth, nHeight ) )
					return FALSE;

				if( !render.Draw( &shader ) )
					return FALSE;

				IDirect3DTexture9* pTexture = render.GetRenderTargetTexture();
				if( pTexture )
				{
					EffectBlur blur;
					blur.m_dLevel = 1;
					
					BOOL bSuccess = TRUE;

					if( !blur.Prepare( render, 1 ) )
					{
						bSuccess = blur.Update( render, pTexture, nWidth, nHeight );
					}
					
					pTexture->Release();

					return bSuccess;
				}

				return TRUE;
			}
		};


	}
}
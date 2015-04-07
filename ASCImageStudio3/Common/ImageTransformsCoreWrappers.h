#pragma once

#include "ImageTransformsCoreConstants.h"

namespace ImageStudio
{
	namespace Transforms
	{
		namespace Core
		{
			namespace Wrappers
			{
				class MediaTransform
				{
				public:

					MediaTransform()
					{
						m_pFormat = NULL;
						m_pTransform = NULL;
						m_bInterfacesValid = TRUE;
					}
					virtual ~MediaTransform()
					{
						Destroy();
					}
					
					BOOL ConvertToBGRA(IUnknown* pSource, IUnknown*& pResult)
					{
						pResult = NULL;

						if (!pSource)
							return FALSE;
						if (!Create())
							return FALSE;

						MediaCore::IAVSUncompressedVideoFrame* pSourceFrame = NULL;
						pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pSourceFrame));
						if (NULL == pSourceFrame)
							return FALSE;

						// настраиваем формат для конвертации
						LONG lWidth = 0; pSourceFrame->get_Width(&lWidth);
						LONG lHeight = 0; pSourceFrame->get_Height(&lHeight);
						LONG lAspectX = 0; pSourceFrame->get_AspectRatioX(&lAspectX);
						LONG lAspectY = 0; pSourceFrame->get_AspectRatioY(&lAspectY);
						LONG lColorSpace = 0; pSourceFrame->get_ColorSpace(&lColorSpace);
						
						m_pFormat->put_Width(lWidth);
						m_pFormat->put_Height(lHeight);
						m_pFormat->put_AspectRatioX(lAspectX);
						m_pFormat->put_AspectRatioY(lAspectY);

						// преобразуем к BGRA32 формату и создаем дубликат (всегда!)
						m_pTransform->SetVideoFormat(m_pFormat);
						m_pTransform->TransformFrame(pSourceFrame, &pResult);

						// если трансформ не сделал дупликата - делаем его руками
						if (pSourceFrame == pResult) // здесь (pResult != NULL)
						{
							pResult->Release();
							pResult = NULL;
							
							pSourceFrame->CreateDuplicate(DUBLICATE_TYPE_COPY, (MediaCore::IAVSMediaData**)(&pResult));
						}

						// если исходный формат был не BGRA (т.е. не содержал alpha канал), то его надо заполнить руками значением 255
						if ((lColorSpace != CSP_BGRA) && (lColorSpace != (CSP_BGRA | CSP_VFLIP)) && (NULL != pResult))
						{
							BYTE* pResultBuffer = NULL; ((MediaCore::IAVSMediaData*)pResult)->get_Buffer(&pResultBuffer);

							if (NULL != pResultBuffer)
							{
								BYTE* pResultAlpha = pResultBuffer + 3;
								for (int index = 4*lWidth*lHeight - 1; index >= 0; index -= 4, pResultAlpha += 4)
									*pResultAlpha = 255;
							}
						}

						pSourceFrame->Release();

						return (NULL != pResult);
					}
				
				protected:
					
					BOOL IsValid()
					{
						if (NULL == m_pFormat || NULL == m_pTransform)
							return FALSE;

						return TRUE;
					}
					void Destroy()
					{
						if (NULL != m_pFormat)
							m_pFormat->Release();
						if (NULL != m_pTransform)
							m_pTransform->Release();

						m_pTransform = NULL;
					}
					BOOL Create()
					{
						if (NULL != m_pTransform && NULL != m_pFormat)
							return TRUE;
						if (!m_bInterfacesValid)
							return FALSE;

						// пытаемся создать интерфейс трансформера
						if (NULL == m_pTransform)
						{
							if (S_OK != CoCreateInstance(MediaCore::CLSID_CAVSVideoFrameTransform, NULL, CLSCTX_ALL, MediaCore::IID_IAVSVideoFrameTransform, (void**)(&m_pTransform)))
							{
								m_pTransform = NULL;
								m_bInterfacesValid = FALSE;
							}
						}

						// пытаемся создать интерфейс формата
						if (NULL == m_pFormat)
						{
							if (S_OK != CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)(&m_pFormat)))
							{
								m_pFormat = NULL;
								m_bInterfacesValid = FALSE;
							}
							else
							{
								m_pFormat->SetDefaultProperties();
								m_pFormat->put_ColorSpace(CSP_BGRA | CSP_VFLIP);
							}
						}

						return IsValid();
					}
					
				protected:
					
					MediaFormat::IAVSVideoFormat* m_pFormat;			// интерфейс, хранящий настройки формата BGR32
					MediaCore::IAVSVideoFrameTransform* m_pTransform;	// интерфейс, необходимый для конвертации media data в формат BGR32
					BOOL m_bInterfacesValid;							// флаг, обозначающий - производилась ли попытка создать интерфейс трансформера или нет
				};
			}
		}


		namespace NonLinearTransforms
		{

			//========================================================================
			template<class T> void swap_arrays(T* a1, T* a2, unsigned n)
			{
				unsigned i;
				for(i = 0; i < n; i++)
				{
					T tmp = *a1;
					*a1++ = *a2;
					*a2++ = tmp;
				}
			}
			//========================================================================
			template<unsigned Rows, unsigned Cols>
			struct matrix_rot
			{
				static int rot(double m[Rows][Cols], unsigned row)
				{
					int k = int(row);
					double max_val, tmp;

					max_val = -1.0;
					unsigned i;
					for(i = row; i < Rows; i++)
					{
						if((tmp = fabs(m[i][row])) > max_val && tmp != 0.0)
						{
							max_val = tmp;
							k = i;
						}
					}

					if(m[k][row] == 0.0)
					{
						return -1;
					}

					if(k != int(row))
					{
						swap_arrays(m[k], m[row], Cols);
						return k;
					}
					return 0;
				}
			};
			//========================================================================
			template<unsigned Size, unsigned RightCols>
			struct simul_eq
			{
				static bool solve(const double left[Size][Size], 
					const double right[Size][RightCols],
					double result[Size][RightCols])
				{
					unsigned i, j, k;
					double a1;

					double tmp[Size][Size + RightCols];

					for(i = 0; i < Size; i++)
					{
						for(j = 0; j < Size; j++)
						{
							tmp[i][j] = left[i][j];
						} 
						for(j = 0; j < RightCols; j++)
						{
							tmp[i][Size + j] = right[i][j];
						}
					}

					for(k = 0; k < Size; k++)
					{
						if(matrix_rot<Size, Size + RightCols>::rot(tmp, k) < 0)
						{
							return false; 
						}

						a1 = tmp[k][k];

						for(j = k; j < Size + RightCols; j++)
						{
							tmp[k][j] /= a1;
						}

						for(i = k + 1; i < Size; i++)
						{
							a1 = tmp[i][k];
							for (j = k; j < Size + RightCols; j++)
							{
								tmp[i][j] -= a1 * tmp[k][j];
							}
						}
					}


					for(k = 0; k < RightCols; k++)
					{
						int m;
						for(m = int(Size - 1); m >= 0; m--)
						{
							result[m][k] = tmp[m][Size + k];
							for(j = m + 1; j < Size; j++)
							{
								result[m][k] -= tmp[m][j] * result[j][k];
							}
						}
					}
					return true;
				}

			};

			class PerspectiveTransformation
			{
			public:
				//--------------------------------------------------------------------
				PerspectiveTransformation() : m_valid(false) {}
				PerspectiveTransformation(const double* src, const double* dst) 
				{
					quad_to_quad(src, dst);
				}
				PerspectiveTransformation(double x1, double y1, double x2, double y2, 
					const double* quad)
				{
					rect_to_quad(x1, y1, x2, y2, quad);
				}
				PerspectiveTransformation(const double* quad, 
					double x1, double y1, double x2, double y2)
				{
					quad_to_rect(quad, x1, y1, x2, y2);
				}
				void quad_to_quad(const double* src, const double* dst)
				{

					double left[8][8];
					double right[8][1];

					unsigned i;
					for (i = 0; i < 4; i++)
					{
						unsigned ix = i * 2;
						unsigned iy = ix + 1;

						left[ix][0]  =  1.0;
						left[ix][1]  =  src[ix];
						left[ix][2]  =  src[iy];
						left[ix][3]  =  0.0;
						left[ix][4]  =  0.0;
						left[ix][5]  =  0.0;
						left[ix][6]  = -src[ix] * dst[ix];
						left[ix][7]  = -src[iy] * dst[ix];
						right[ix][0] =  dst[ix];

						left[iy][0]  =  0.0;
						left[iy][1]  =  0.0;
						left[iy][2]  =  0.0;
						left[iy][3]  =  1.0;
						left[iy][4]  =  src[ix];
						left[iy][5]  =  src[iy];
						left[iy][6]  = -src[ix] * dst[iy];
						left[iy][7]  = -src[iy] * dst[iy];
						right[iy][0] =  dst[iy];
					}
					m_valid = simul_eq<8, 1>::solve(left, right, m_mtx);
				}
				void rect_to_quad(double x1, double y1, double x2, double y2, 
					const double* quad)
				{
					double src[8];
					src[0] = src[6] = x1;
					src[2] = src[4] = x2;
					src[1] = src[3] = y1;
					src[5] = src[7] = y2;
					quad_to_quad(src, quad);
				}
				void quad_to_rect(const double* quad, 
					double x1, double y1, double x2, double y2)
				{
					double dst[8];
					dst[0] = dst[6] = x1;
					dst[2] = dst[4] = x2;
					dst[1] = dst[3] = y1;
					dst[5] = dst[7] = y2;
					quad_to_quad(quad, dst);
				}
				bool is_valid() const { return m_valid; }
				void transform(double* x, double* y) const
				{
					double tx = *x;
					double ty = *y;
					double d = 1.0 / (m_mtx[6][0] * tx + m_mtx[7][0] * ty + 1.0);
					*x = (m_mtx[0][0] + m_mtx[1][0] * tx + m_mtx[2][0] * ty) * d;
					*y = (m_mtx[3][0] + m_mtx[4][0] * tx + m_mtx[5][0] * ty) * d;
				}

			private:
				double m_mtx[8][1];
				bool   m_valid;
			};



			//========================================================================
			class BilinearTransformation
			{
			public:
				//--------------------------------------------------------------------
				BilinearTransformation() : m_valid(false) {}
				BilinearTransformation(const double* src, const double* dst) 
				{
					quad_to_quad(src, dst);
				}
				BilinearTransformation(double x1, double y1, double x2, double y2, 
					const double* quad)
				{
					rect_to_quad(x1, y1, x2, y2, quad);
				}
				BilinearTransformation(const double* quad, 
					double x1, double y1, double x2, double y2)
				{
					quad_to_rect(quad, x1, y1, x2, y2);
				}

				void quad_to_quad(const double* src, const double* dst)
				{
					double left[4][4];
					double right[4][2];

					unsigned i;
					for(i = 0; i < 4; i++)
					{
						unsigned ix = i * 2;
						unsigned iy = ix + 1;
						left[i][0] = 1.0;
						left[i][1] = src[ix] * src[iy];
						left[i][2] = src[ix];
						left[i][3] = src[iy];

						right[i][0] = dst[ix];
						right[i][1] = dst[iy];
					}
					m_valid = simul_eq<4, 2>::solve(left, right, m_mtx);
				}
				void rect_to_quad(double x1, double y1, double x2, double y2, 
					const double* quad)
				{
					double src[8];
					src[0] = src[6] = x1;
					src[2] = src[4] = x2;
					src[1] = src[3] = y1;
					src[5] = src[7] = y2;
					quad_to_quad(src, quad);
				}
				void quad_to_rect(const double* quad, 
					double x1, double y1, double x2, double y2)
				{
					double dst[8];
					dst[0] = dst[6] = x1;
					dst[2] = dst[4] = x2;
					dst[1] = dst[3] = y1;
					dst[5] = dst[7] = y2;
					quad_to_quad(quad, dst);
				}
				bool is_valid() const { return m_valid; }

				void transform(double* x, double* y) const
				{
					double tx = *x;
					double ty = *y;
					double xy = tx * ty;
					*x = m_mtx[0][0] + m_mtx[1][0] * xy + m_mtx[2][0] * tx + m_mtx[3][0] * ty;
					*y = m_mtx[0][1] + m_mtx[1][1] * xy + m_mtx[2][1] * tx + m_mtx[3][1] * ty;
				}

			private:
				double m_mtx[4][2];
				bool   m_valid;
			};
		}
	}
}


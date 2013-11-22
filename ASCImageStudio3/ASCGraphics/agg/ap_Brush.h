#if !defined(Aggplus_BRUSH_H__5B90925A_5FCA_48E5_9455_10AFD798E52E__INCLUDED_)
#define Aggplus_BRUSH_H__5B90925A_5FCA_48E5_9455_10AFD798E52E__INCLUDED_

#include "ap_AggPlusTypes.h"
#include "ap_AggPlusEnums.h"
#include "ap_Color.h"
#include "ap_Matrix.h"
#include "ap_Image.h"

namespace Aggplus
{


		class Brush  
		{
			friend class Graphics;

			protected:
				Brush(BrushType bType) : m_bType(bType) { }
			public:
				virtual ~Brush() { ; }
				virtual Brush *Clone() const = 0;
				inline BrushType GetType() const { return(m_bType); }

			protected:
				BrushType m_bType;
		};

		class BrushSolid : public Brush
		{
			public:
				BrushSolid(Color dwColor) : Brush(BrushTypeSolidColor), m_dwColor(dwColor) { }
				virtual ~BrushSolid() { ; }
				virtual Brush *Clone() const { return new BrushSolid(m_dwColor); }

				void GetColor(Color* color) const { *color = m_dwColor; }
				void SetColor(const Color &color) { m_dwColor = color; }
			protected:
				Color m_dwColor;
		};


		class BrushLinearGradient : public Brush
		{
			friend class Graphics;
			public:
				BrushLinearGradient(/*const RectF &rect,*/ const Color &color1, const Color &color2, REAL angle/*, BOOL isAngleScalable=false*/);
				virtual ~BrushLinearGradient() { }
				
				virtual Brush *Clone() const;

				//Status GetLinearColors(Color* colors) const { colors[0]=m_dwColorA; colors[1]=m_dwColorB; return Ok; }
				//Status GetRectangle(RectF* rect ) const  { *rect = m_fRct; return Ok; }
				//Status ResetTransform() { m_mtx.Reset(); return Ok; }

				float GetRotate() const {return m_rotate_angle_radians;};

				void  AddColor(const Color &clr);

				unsigned size() const{ return (unsigned)(m_colors.size()); }
				const agg::rgba8 operator [] (unsigned v) const
				{ 
					return m_colors[(size_t)v].GetAggColor(); 
				}
				
			protected:
				std::vector<Color> m_colors;
				
				float m_rotate_angle_radians;
		};

		class BrushTexture : public Brush
		{
			friend class Graphics;
			protected:
				BrushTexture() : Brush(BrushTypeTextureFill) { }
			public:
				BrushTexture(const Image *pImage, WrapMode wrapMode=WrapModeTile);
				virtual ~BrushTexture() {};
				virtual Brush *Clone() const;

				void TranslateTransform(float dX, float dY, MatrixOrder order=MatrixOrderPrepend) { m_mtx.Translate(dX, dY, order); }
				void ScaleTransform(float dX, float dY, MatrixOrder order=MatrixOrderPrepend) { m_mtx.Scale(dX, dY, order); }
				void RotateTransform(float angle, MatrixOrder order=MatrixOrderPrepend) { m_mtx.Rotate(angle, order); }
				void GetTransform(Matrix* matrix) const { *matrix = m_mtx; }
				void SetTransform(const Matrix* matrix) { m_mtx = *matrix; }

				void SetWrapMode(WrapMode wMode) { m_wrapMode=wMode; }
				WrapMode GetWrapMode() const { return(m_wrapMode); }

			 
			protected:

				void *PatternFinalize()				{	return(m_Image.m_pImgData);			}
				DWORD PatternGetWidth()				{	return(m_Image.m_dwWidth);			}
				DWORD PatternGetHeight()			{	return(m_Image.m_dwHeight);			}
				int   PatternGetStride()			{	return(m_Image.m_nStride);			}

				protected:
				Image m_Image;
				
				WrapMode m_wrapMode;
				Matrix m_mtx;
		};

		typedef BrushSolid SolidBrush;
		typedef BrushTexture TextureBrush;
		typedef BrushLinearGradient LinearGradientBrush;

}
#endif // !defined(Aggplus_BRUSH_H__5B90925A_5FCA_48E5_9455_10AFD798E52E__INCLUDED_)

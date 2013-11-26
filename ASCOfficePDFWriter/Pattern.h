#pragma once

#include <float.h>
#include <atlstr.h>
#include <atlcoll.h>
#include "../Common/atldefine.h"

#include "Utils.h"
#include "Objects.h"
#include "Common.h"

#ifdef _DEBUG
	#define TAB			src += _T("\012");
	#define TAB_X		src += _T("\012   ");
	#define TAB_XX		src += _T("\012      ");
	#define TAB_XXX		src += _T("\012         ");
	#define TAB_XXXX	src += _T("\012            ");
#else
	//#define TAB		
	//#define TAB_X	
	//#define TAB_XX	
	//#define TAB_XXX 
	//#define TAB_XXXX 

	#define TAB			src += _T("\012");
	#define TAB_X		src += _T("\012   ");
	#define TAB_XX		src += _T("\012      ");
	#define TAB_XXX		src += _T("\012         ");
	#define TAB_XXXX	src += _T("\012            ");

#endif

namespace PDF
{
	enum FunctionType
	{
		Sampled							=	0,
		ExponentialInterpolation		=	2,
		Stitching						=	3,
		PostScriptCalculator			=	4,

		FunctionInvalid					=	5
	};
	enum PatternType
	{
		TilingPatternType				=	1,
		ShadingPatternType				=	2,

		PatternInvalid					=	3
	};
	enum ShadingType
	{
		FunctionBasedShading			=	1,
		AxialShadingType				=	2,
		RadialShadingType				=	3, 
		FreeForm						=	4,
		LatticeForm						=	5, 
		CoonsPatch						=	6,
		TensorProduct					=	7,

		ShadingInvalid					=	8
	};
	enum PaintType
	{
		ColouredTilingPattern			=	0,
		UncolouredTilingPattern			=	1
	};
	enum TilingType
	{
		ConstantSpacing					=	0,
		NoDistortion					=	1,
		ConstantSpacingAndFsterTiling	=	2
	};
}

namespace PDF
{
	struct float2
	{
		float2(float n0 = 0.0f, float n1 = 0.0f) : f1(n0), f2(n1)
		{

		}

		float f1;
		float f2;
	};
	struct float3
	{
		float3(float m0 = 0.0f, float m1 = 0.0f, float m2 = 0.0f) : f1(m0), f2(m1), f3(m2)
		{

		}

		float f1;
		float f2;
		float f3;
	};
	struct float4
	{
		float4(float m0 = 0.0f, float m1 = 0.0f, float m2 = 0.0f, float m3 = 0.0f) : f1(m0), f2(m1), f3(m2), f4(m3)
		{

		}

		float f1;
		float f2;
		float f3;
		float f4;
	};

	struct InterpolationValues
	{
	public:
		InterpolationValues()
		{

		}
		InterpolationValues(const InterpolationValues& val)
		{
			numbers.Copy(val.numbers);
		}
		InterpolationValues& operator=(const InterpolationValues& val)
		{
			numbers.Copy(val.numbers);
		}

		inline const float& operator[] (int index) const
		{
			return numbers[index];
		}
		inline size_t Count() const
		{
			return numbers.GetCount();
		}
		inline size_t Add(float f) 
		{
			return numbers.Add(f);
		}
	public:
		CAtlArray<float> numbers;
	};

	class Point
	{
	public:
		Point (double dX = 0.0, double dY = 0.0) : X(dX), Y(dY)
		{

		}
		
		inline Point& operator+= (const Point& point)
		{
			X += point.X;
			Y += point.Y;

			return (*this);
		}
		inline Point& operator-= (const Point& point)
		{
			X -= point.X;
			Y -= point.Y;

			return (*this);
		}

	public:
		double X;
		double Y;
	};
	class Matrix
	{
	public:

		Matrix()
		{
			Identity();
		}
		Matrix(double mat[9])
		{
			memcpy (m_fMat, mat, 9 * sizeof(double));
		}
		Matrix(double m1, double m2, double m3, double m4, double m5, double m6, double m7, double m8, double m9) 
		{
			m_fMat[0] = m1;
			m_fMat[1] = m2;
			m_fMat[2] = m3;

			m_fMat[3] = m4;
			m_fMat[4] = m5;
			m_fMat[5] = m6;

			m_fMat[6] = m7;
			m_fMat[7] = m8;
			m_fMat[8] = m9;
		}

		inline double operator [] (int i) const
		{
			return m_fMat[i]; 
		}
		inline Matrix& operator*= (const Matrix& transform)
		{
			double mat[9];

			for (int i = 0 ; i < 3; ++i)
			{
				for(int k = 0; k < 3; ++k)
				{
					mat [i*3 + k] = 0;

					for (int j = 0; j < 3; ++j)
					{
						mat[i*3 + k] += m_fMat[i * 3 + j] * transform.m_fMat [j*3 + k];
					}
				}
			}

			*this = Matrix(mat);

			return (*this);
		}

		inline Matrix& Translate(double x, double y)
		{
			(*this) *= Matrix(1, 0, x,  0, 1, y,  0, 0, 1);
			return (*this);
		}
		inline Matrix& Scale(double x, double y)
		{
			(*this) *= Matrix(x, 0, 0,  0, y, 0,  0, 0, 1);
			return (*this);
		}
		inline Matrix& Translate(Point point)
		{
			(*this) *= Matrix(1, 0, point.X,  0, 1, point.Y,  0, 0, 1);
			return (*this);
		}
		inline Matrix& Scale(Point point)
		{
			(*this) *= Matrix(point.X, 0, 0,  0, point.Y, 0,  0, 0, 1);
			return (*this);
		}
		inline Matrix& Rotate(double angle)
		{
			(*this) *= Matrix(cos(angle), -sin(angle), 0,  sin(angle), cos(angle), 0,  0, 0, 1);
			return (*this);
		}

		inline void Set(int i, double f)
		{
			m_fMat[i] = f;
		}

		inline Point TranformPoint (const Point& point)
		{
			return Point(point.X * m_fMat[0] + point.Y * m_fMat[3] + m_fMat[6] - m_fMat[2], point.X * m_fMat[1] + point.Y * m_fMat[4] + m_fMat[7] - m_fMat[5]);
		}
		inline Point RotatePoint (const Point& point) 
		{
			return Point(point.X * m_fMat[0] + point.Y * m_fMat[3], point.X * m_fMat[1] + point.Y * m_fMat[4]);
		}

	private:

		inline void Identity()
		{
			m_fMat[0] = 1;
			m_fMat[1] = 0;
			m_fMat[2] = 0;

			m_fMat[3] = 0;
			m_fMat[4] = 1;
			m_fMat[5] = 0;

			m_fMat[6] = 0;
			m_fMat[7] = 0;
			m_fMat[8] = 1;
		}

	private:

		double	m_fMat[9];
	};
	class Rect
	{
	public:
		Rect ()
		{
			X		=	0.0;
			Y		=	0.0;
			Width	=	0.0;
			Height	=	0.0;
		}
		Rect (float fX, float fY, float fWidth, float fHeight) : X(fX), Y(fY), Width(fWidth), Height(fHeight)
		{

		}

		inline float CenterX() const
		{
			return X + Width * 0.5;
		}
		inline float CenterY() const
		{
			return Y + Height * 0.5;
		}
		inline float GetRight() const
		{
			return X + Width;
		}
		inline float GetBottom() const
		{
			return Y + Height;
		}

	public:

		float X;
		float Y;
		float Width;
		float Height;
	};
}

namespace PDF
{
	// functions

	class Function
	{
	public:
		Function()
		{
			m_nId	=	-1;
			m_Type	=	FunctionInvalid;
		}
		Function(FunctionType type) : m_Type (type)
		{
			m_nId	=	-1;
		}
		Function(const Function& func)
		{
			*this = func;
		}
		virtual ~Function()
		{

		}
		Function& operator=(const Function& func)
		{
			m_nId	=	func.m_nId;
			m_Type	=	func.m_Type;

			m_Domain.Copy(func.m_Domain);
			m_Range.Copy(func.m_Range);

			return *this;
		}

		inline void SetId(int id)
		{
			m_nId	=	id;
		}
		inline void SetType(FunctionType type)
		{
			m_Type	=	type;
		}

		inline int GetId() const
		{
			return m_nId;
		}
		inline FunctionType GetType() const
		{
			return m_Type;
		}

		inline void AddDomain(float2 domain)
		{
			m_Domain.Add(domain);
		}
		inline void AddRange(float2 range)
		{
			m_Range.Add(range);
		}

		virtual CString Define() const
		{
			return CString(_T("<<")) + Function::InternalObj() + CString(_T(">>"));
		}

	protected:

		inline CString InternalObj() const
		{
			CString src;
			CString val;
			
			src.Format(_T(" /FunctionType %d"), m_Type);							

			if (m_Domain.GetCount())
			{
				TAB_X
				src += _T("/Domain [");

				for (size_t i = 0; i < m_Domain.GetCount(); ++i)
				{
					val.Format (_T(" %f %f"), m_Domain.GetAt(i).f1, m_Domain.GetAt(i).f2);
					src += val;						
				}

				src += _T(" ]");																
			}

			if (m_Range.GetCount())
			{
				TAB_X
				src += _T("/Range [");

				for (size_t i = 0; i < m_Range.GetCount(); ++i)
				{
					val.Format (_T(" %f %f"), m_Range.GetAt(i).f1, m_Range.GetAt(i).f2);
					src += val;						
				}

				src += _T(" ]");														
			}

			return src;			
		}

	protected:

		int					m_nId;

		FunctionType		m_Type;				//	Required
		CAtlArray<float2>	m_Domain;			//	Required
		CAtlArray<float2>	m_Range;			//	Required for type 0 and type 4 functions, optional otherwise; see below
	};
	class SampledFunction : public Function
	{
	public:
		SampledFunction ()
		{
			m_Type = Sampled;
		}
	};
	class ExponentialInterpolationFunction : public Function
	{
	public:
		ExponentialInterpolationFunction()
		{
			m_Type	=	ExponentialInterpolation;
		}
		ExponentialInterpolationFunction& operator=(const ExponentialInterpolationFunction& func)
		{
			m_nId	=	func.m_nId;
			m_Type	=	func.m_Type;

			m_N.Copy(func.m_N);
			m_offsets.Copy(func.m_offsets);
			
			m_C.Copy(func.m_C);

			return *this;
		}

		inline void AddC(int count, ...)
		{
			va_list arg;
			va_start(arg, count);

			InterpolationValues numbers;

			for (int i = 0; i < count; ++i)
				numbers.Add(va_arg(arg, double));

			va_end(arg);	

			m_C.Add(numbers);
		}
		inline void AddN(int n = 1)
		{
			m_N.Add(n);
		}
		inline void AddOffset (float2 offset)
		{
			m_offsets.Add(offset);
		}

		inline float2 GetOffset(int index = 0)
		{
			if (m_offsets.GetCount())
			{
				return float2(m_offsets[index].f1, m_offsets[index].f2);
			}

			return float2 (0.0f, 1.0f);
		}

		virtual CString Define() const
		{
			return CString(_T("<<")) +
				Function::InternalObj() +
				ExponentialInterpolationFunction::InternalObj() +
				CString(_T(">>"));
		}

	protected:

		inline CString InternalObj() const
		{
			CString src;
			CString val;

			if (m_C.GetCount())
			{
				for (size_t j = 0; j < m_C.GetCount(); ++j)
				{
					TAB_X
					val.Format(_T("/C%d ["), j);
					src += val;

					for (size_t i = 0; i < m_C[j].Count(); ++i) 
					{
						val.Format (_T(" %f"), m_C[j][i]);
						src += val;					
					}

					src += _T(" ]");					
				}
			}

			if (m_N.GetCount())
			{
				TAB_X
				src += _T("/N");

				for (size_t i = 0; i < m_N.GetCount(); ++i)
				{
					val.Format (_T(" %d"), m_N.GetAt(i));
					src += val;						
				}	
				TAB
			}

			return src;			
		}

	protected:


		CAtlArray<InterpolationValues>	m_C;		//	C0, C1, ... Cn
		CAtlArray<int>					m_N;		//	Required

		// work
		CAtlArray<float2>				m_offsets;
	};
	class StitchFunction : public Function
	{
	public:
		StitchFunction()
		{
			m_Type	=	Stitching;
		}

		inline void AddFunc(Function* function)
		{
			m_Functions.Add(function);
		}
		inline void AddBounds(float bounds)
		{
			m_Bounds.Add(bounds);
		}
		inline void AddEncode(float2 encode)
		{
			m_Encode.Add(encode);
		}

		inline BOOL IsValid()
		{
			if (m_Functions.GetCount() < 2)
				return FALSE;

			return TRUE;
		}

		virtual CString Define() const
		{
			return CString(_T("<<")) +				
				StitchFunction::InternalObj() +
				CString(_T(">>"));
		}

	protected:

		inline CString InternalObj() const
		{
			CString src;
			CString val;

			src.Format(_T(" /FunctionType %d"), m_Type);							

			if (m_Domain.GetCount())
			{
				TAB_X
				src += _T("/Domain [");

				for (size_t i = 0; i < m_Domain.GetCount(); ++i)
				{
					val.Format (_T(" %f %f"), m_Domain.GetAt(i).f1, m_Domain.GetAt(i).f2);
					src += val;						
				}

				src += _T(" ]");																
			}

			if (m_Bounds.GetCount())
			{
				TAB_X
				src += _T("/Bounds [");

				for (size_t i = 0; i < m_Bounds.GetCount(); ++i)
				{
					val.Format (_T(" %f"), m_Bounds.GetAt(i));
					src += val;						
				}

				src += _T(" ]");
			}

			if (m_Encode.GetCount())
			{
				TAB_X
				src += _T("/Encode [");

				for (size_t i = 0; i < m_Encode.GetCount(); ++i)
				{
					val.Format (_T(" %f %f"), m_Encode.GetAt(i).f1, m_Encode.GetAt(i).f2);
					src += val;						
				}

				src += _T(" ]");
			}

			if (m_Functions.GetCount())
			{
				TAB_X
				src += _T("/Functions [");

				for (size_t i = 0; i < m_Functions.GetCount(); ++i)
				{
					val.Format (_T(" %d 0 R"), m_Functions.GetAt(i)->GetId());
					src += val;						
				}

				src += _T(" ]");
				TAB
			}

			return src;			
		}

	protected:

		CAtlArray<Function*>	m_Functions;	//	Required	// храним только ссылки
		CAtlArray<float>		m_Bounds;		//	Required
		CAtlArray<float2>		m_Encode;		//	Required
	};

	// shadings

	class Shading
	{
	public:
		Shading()
		{
			m_Type		=	ShadingInvalid;
			m_FuncTop	=	NULL;
		}
		Shading(const Shading& shading)
		{
			*this = shading;
		}
		Shading& operator=(const Shading& shading)
		{
			m_Type	=	shading.m_Type;

			m_Functions.Copy(shading.m_Functions);

			return *this;
		}		
		virtual ~Shading()
		{
			for (size_t i = 0; i < m_Functions.GetCount(); ++i)
			{
				RELEASEOBJECT(m_Functions[i]);
			}
		}

		inline void AddFunc(Function* function)
		{
			if (function)
			{
				m_FuncTop	=	function;
				m_Functions.Add(function);
			}
		}

		virtual void CalculateCoords(Rect bounds, float pageHeight)
		{

		}

		virtual void SetTransform(const Matrix& matrix)
		{

		}

		inline ShadingType GetType() const
		{
			return m_Type;
		}
		virtual CString Define() const
		{
#ifdef _DEBUG
			ATLTRACE(L"Shading - NOT IMPLEMENTED");
#endif
			return CString(_T(""));
		}

	protected:

		ShadingType				m_Type;
		CAtlArray<Function*>	m_Functions;

		// work
		Function*				m_FuncTop;
	};

	class AxialShading : public Shading
	{
	public:
		AxialShading()
		{
			m_Type			=	AxialShadingType;
			m_line			=	float4(0.0f, 0.0f, 1.0f, 0.0f);		//	horizontal
			m_normalized	=	TRUE;
			
			m_ColorSpace	=	L"DeviceRGB";

			m_Extend.Add(TRUE);
			m_Extend.Add(TRUE);
		}
		AxialShading(const AxialShading& shading)
		{
			*this = shading;
		}
		AxialShading& operator=(const AxialShading& shading)
		{
			m_Type			=	shading.m_Type;
			m_Coords		=	shading.m_Coords;
			m_Domain		=	shading.m_Domain;
			m_Matrix		=	shading.m_Matrix;
			m_line			=	shading.m_line;
			m_normalized	=	shading.m_normalized;
			m_transform		=	shading.m_transform;
			m_mirror		=	shading.m_mirror;

			m_Extend.Copy(shading.m_Extend);
			m_Functions.Copy(shading.m_Functions);

			return *this;
		}	

		virtual void CalculateCoords(Rect bounds, float pageHeight)
		{
			if (!m_normalized)
			{
				Point size		=	Point ((m_line.f3 - m_line.f1) * 0.5,  (m_line.f4 - m_line.f2) * 0.5);
				Point center	=	Point (bounds.Width * 0.5 + bounds.X, bounds.Height * 0.5 + bounds.Y);
				Point offset	=	m_transform.RotatePoint(Point(0, bounds.Height));

				m_Coords.f1		=	center.X - size.X;
				m_Coords.f3		=	center.X + size.X;
				m_Coords.f2		=	center.Y + size.Y;
				m_Coords.f4		=	center.Y - size.Y;

				m_Matrix		*=	m_transform;

				m_Matrix.Set (2, m_Matrix[2] - offset.X);
				m_Matrix.Set (5, pageHeight - m_Matrix[5] - offset.Y);

				return;
			}

			// актуально только для svg

			CalcGradientVector(bounds);

			float maxw			=	bounds.Height * 0.5;	
			float maxh			=	bounds.Width * 0.5;	  

			float w				=	bounds.Width * 0.5;	  
			float h				=	bounds.Height * 0.5;

			float offW			=	0.0f;
			float moveCX		=	0.0f;		//	смещение для центра 

			if (m_Functions.GetCount())
			{
				float2 offset;

				if (ExponentialInterpolation == m_Functions[0]->GetType())
				{
					offset		=	static_cast<ExponentialInterpolationFunction*>(m_Functions[0])->GetOffset();
				}

				for (size_t i = m_Functions.GetCount() - 1; i > 0; --i)
				{
					if (ExponentialInterpolation == m_Functions[i]->GetType())
					{
						ExponentialInterpolationFunction* function = static_cast<ExponentialInterpolationFunction*>(m_Functions[i]);
						if (function)
						{
							offset.f1	=	__min(function->GetOffset().f1, offset.f1);
							offset.f2	=	__max(function->GetOffset().f2, offset.f2);
						}
					}
				}

				offW			=	(offset.f2 - offset.f1);
				moveCX			=	(offset.f2 + offset.f1) * 0.5 - 0.5;

				if (fabs(offW) < 0.0001)	// есть совпадения смещений
				{
					offW		=	0.0001;
				}

				w	*=	offW;
				h	*=	offW;
			}

			float cx			=	bounds.X + bounds.Width * 0.5;
			float cy			=	pageHeight - (bounds.Y + bounds.Height * 0.5);

			// натягиваем "градиент" как обычную текстуру

			if (bounds.Width < bounds.Height) 
			{
				cx				=	bounds.X + bounds.Width * 0.5;
				cy				=	pageHeight - (bounds.Y + bounds.Height * 0.5);

				float basecx	=	bounds.X + bounds.Height * 0.5;
				float basecy	=	pageHeight - (bounds.Y + bounds.Height * 0.5);

				m_Coords.f1		=	basecx - h;
				m_Coords.f3		=	basecx + h;
				m_Coords.f2		=	basecy + h;
				m_Coords.f4		=	basecy - h;

				m_Matrix.Translate(cx, cy);
				m_Matrix.Translate(-bounds.Width * m_centerBB.f1 + moveCX * bounds.Width, bounds.Height * m_centerBB.f2 - moveCX * bounds.Height);
				
				m_Matrix		*=	m_transform;
				m_Matrix		*=	m_mirror;

				m_Matrix.Scale(w / h * m_mapBB.f1, 1.0 * m_mapBB.f2);
				m_Matrix.Translate(-basecx, -basecy);
			} 
			else if (bounds.Width > bounds.Height) 
			{
				cx				=	bounds.X + bounds.Width * 0.5;
				cy				=	pageHeight - (bounds.Y + bounds.Height * 0.5);

				float basecx	=	bounds.X + bounds.Width * 0.5;
				float basecy	=	pageHeight - (bounds.Y + bounds.Width * 0.5);

				m_Coords.f1		=	basecx - w;
				m_Coords.f3		=	basecx + w;
				m_Coords.f2		=	basecy + w;
				m_Coords.f4		=	basecy - w;

				m_Matrix.Translate(cx, cy);
				m_Matrix.Translate(-bounds.Width * m_centerBB.f1 + moveCX * bounds.Width, bounds.Height * m_centerBB.f2 - moveCX * bounds.Height);
				
				m_Matrix		*=	m_transform;
				m_Matrix		*=	m_mirror;
				
				m_Matrix.Scale(1.0 * m_mapBB.f1, h / w * m_mapBB.f2);	
				m_Matrix.Translate(-basecx, -basecy);
			} 
			else
			{
				m_Coords.f1		=	cx - w;
				m_Coords.f3		=	cx + w;
				m_Coords.f2		=	cy + h;
				m_Coords.f4		=	cy - h;

				m_Matrix.Translate(cx, cy);
				m_Matrix.Translate(-bounds.Width * m_centerBB.f1 + moveCX * bounds.Width, bounds.Height * m_centerBB.f2 - moveCX * bounds.Height);
				
				m_Matrix		*=	m_transform;
				m_Matrix		*=	m_mirror;

				m_Matrix.Scale(1.0 * m_mapBB.f1, 1.0* m_mapBB.f2);						
				m_Matrix.Translate(-cx, -cy);
			}

			// возможно вертикальный
			if (fabs(m_line.f1 - m_line.f3) < DBL_EPSILON)
			{
				m_Coords.f1		=	0.0f;
				m_Coords.f3		=	0.0f;
			}

			// возможно горизонтальный
			if (fabs(m_line.f2 - m_line.f4) < DBL_EPSILON)
			{
				m_Coords.f2		=	0.0f;
				m_Coords.f4		=	0.0f;
			}
		}

		inline void SetGradientVector(float4 line, BOOL normalized = TRUE)		
		{
			m_line			=	line;
			m_normalized	=	normalized;
		}
		virtual void SetTransform(const Matrix& matrix) 
		{
			m_transform = matrix;
		}
		inline void SetColorSpace (const CString& colorSpace)
		{
			m_ColorSpace	=	colorSpace;
		}

		virtual CString Define() const
		{
			return AxialShading::InternalObj();
		}

	private:

		inline void CalcGradientVector(const Rect& bounds)
		{
			m_transform.Set(2, m_transform[2] * bounds.Width);
			m_transform.Set(5, m_transform[5] * bounds.Height);

			m_mapBB				=	float2(fabs(m_line.f1 - m_line.f3), fabs(m_line.f2 - m_line.f4));

			if (m_mapBB.f1 < DBL_EPSILON)
				m_mapBB.f1		=	1.0f;
			if (m_mapBB.f2 < DBL_EPSILON)
				m_mapBB.f2		=	1.0f;

			m_centerBB			=	float2((m_line.f1 + m_line.f3) * 0.5, (m_line.f2 + m_line.f4) * 0.5);

			if (fabs(m_centerBB.f1) > DBL_EPSILON)
				m_centerBB.f1	=	0.5f - m_centerBB.f1;
			if (fabs(m_centerBB.f2) > DBL_EPSILON)
				m_centerBB.f2	=	0.5f - m_centerBB.f2;
		}
		inline float2 CalculateBaseDomain()
		{
			float off		=	1.0f;
			float move		=	0.0;

			if (m_Functions.GetCount())
			{
				float2 offset;

				if (ExponentialInterpolation == m_Functions[0]->GetType())
				{
					offset		=	static_cast<ExponentialInterpolationFunction*>(m_Functions[0])->GetOffset();
				}

				for (size_t i = m_Functions.GetCount() - 1; i > 0; --i)
				{
					if (ExponentialInterpolation == m_Functions[i]->GetType())
					{
						ExponentialInterpolationFunction* function = static_cast<ExponentialInterpolationFunction*>(m_Functions[i]);
						if (function)
						{
							offset.f1	=	__min(function->GetOffset().f1, offset.f1);
							offset.f2	=	__max(function->GetOffset().f2, offset.f2);
						}
					}
				}

				off			=	(offset.f2 - offset.f1);
				move		=	(offset.f2 + offset.f1) * 0.5 - 0.5;

				if (fabs(off) < 0.0001)	// есть совпадения смещений
				{
					off		=	0.0001;
				}
			}

			return float2(off, move);
		}

		inline CString InternalObj() const
		{
			CString src;
			CString val;
			
			TAB_X
			val.Format(_T("/Matrix [ %f %f %f %f %f %f ] "), m_Matrix[0], m_Matrix[1], m_Matrix[3], m_Matrix[4], m_Matrix[2], m_Matrix[5]);
			src += val;
			TAB_X

			src += CString(_T("/Shading"));

			val.Format (_T("<< /ShadingType %d"), m_Type);	
			TAB_XX
			src += val;										
			TAB_XXX	

			val.Format (_T("/ColorSpace /%s "), m_ColorSpace);			
			src += val;	

			val.Format (_T("/Coords [ %f %f %f %f ]"), m_Coords.f1, m_Coords.f2, m_Coords.f3, m_Coords.f4);
			TAB_XXX
			src += val;	

			if (2 == m_Extend.GetCount())
			{
				TAB_XXX
				src += CString(_T("/Extend [ "));

				(m_Extend[0]) ? src += CString(_T("true ")) : src += CString(_T("false "));
				(m_Extend[1]) ? src += CString(_T("true"))  : src += CString(_T("false"));

				src += CString(_T(" ]"));
			}

			if (m_Functions.GetCount())
			{
				TAB_XXX
				src += _T("/Function");

				val.Format (_T(" %d 0 R"), m_Functions.GetAt(m_Functions.GetCount()-1)->GetId());
				src += val;	
			}

			TAB_XX
			src += CString(_T(">>\012"));

			return src;			
		}
	private:

		float4					m_Coords;			//	Required
		float2					m_Domain;			//	Optional
		Matrix					m_Matrix;			//	Optional

		CAtlArray<BOOL>			m_Extend;			//	Optional
		CString					m_ColorSpace;		

		// work
	
		float4					m_line;
		BOOL					m_normalized;
		float2					m_mapBB;
		float2					m_centerBB;
		Matrix					m_transform;		//	дополнительное преобразование
		Matrix					m_mirror;
	};
	class RadialShading : public Shading
	{
	public:
		RadialShading()
		{
			m_Type			=	RadialShadingType;
			m_normalized	=	TRUE;
			
			m_ColorSpace	=	L"DeviceRGB";
		
			m_Extend.Add(TRUE);
			m_Extend.Add(TRUE);

			m_Radius		=	float2(0.5, 1.0);
		}
		RadialShading(const RadialShading& shading)
		{
			*this = shading;
		}
		RadialShading& operator=(const RadialShading& shading)
		{
			m_Type			=	shading.m_Type;
			m_Coords		=	shading.m_Coords;
			m_Radius		=	shading.m_Radius;
			m_Domain		=	shading.m_Domain;
			m_Matrix		=	shading.m_Matrix;
			m_normalized	=	shading.m_normalized;
			m_c0			=	shading.m_c0;
			m_c1			=	shading.m_c1;
			m_r				=	shading.m_r;
			m_transform		=	shading.m_transform;

			m_Extend.Copy(shading.m_Extend);
			m_Functions.Copy(shading.m_Functions);

			return *this;
		}		
			
		virtual void CalculateCoords(Rect bounds, float pageHeight)
		{			
			//m_c0			=	float2(m_c0.f1, pageHeight - m_c0.f2);
			//m_c1			=	float2(m_c1.f1, pageHeight - m_c1.f2);

			Point center	=	Point (bounds.Width * 0.5 + bounds.X, bounds.Height * 0.5 + bounds.Y);

			Point offset	=	m_transform.RotatePoint(Point(0, bounds.Height));
			m_transform.Set (2, m_transform[2] - offset.X);
			m_transform.Set (5, pageHeight - m_transform[5] - offset.Y);

			//if (!m_normalized)
			//{
				//m_transform.Set (2, m_Matrix[2]);
			//	m_transform.Set (5, pageHeight - m_transform[5]);

				//m_Matrix.Set (2, m_Matrix[2] - offset.X);
				//m_Matrix.Set (5, pageHeight - m_Matrix[5] - offset.Y);
			//}

		}
		virtual void SetTransform(const Matrix& matrix) 
		{
			m_transform		=	matrix;
		}
	
		inline void SetGradientCircles(float2 c0, float2 c1, float2 r, BOOL normalized = TRUE)		
		{
			m_c0			=	c0;
			m_c1			=	c1;
			m_r				=	r;

			m_normalized	=	normalized;
		}
		inline void SetColorSpace (const CString& colorSpace)
		{
			m_ColorSpace	=	colorSpace;
		}

		inline float2 GetC0() const
		{
			return m_c0;
		}
		inline float2 GetC1() const
		{
			return m_c1;
		}
		inline float2 GetR() const
		{
			return m_r;
		}

		virtual CString Define() const
		{
			return RadialShading::InternalObj();
		}

	private:

		inline CString InternalObj() const
		{
			CString src;
			CString val;
			
			TAB_X
			val.Format(_T("/Matrix [ %f %f %f %f %f %f ]"), m_transform[0], m_transform[1], m_transform[3], m_transform[4], m_transform[2], m_transform[5]);	
			src += val;										
			TAB_X
						
			src += CString(_T("/Shading"));

			val.Format (_T("<< /ShadingType %d"), m_Type);	
			TAB_XX
			src += val;										
			TAB_XXX	

			val.Format (_T("/ColorSpace /%s "), m_ColorSpace);			
			src += val;	

			//	[ x0 y0 r0 x1 y1 r1 ]

			val.Format (_T("/Coords [ %f %f %f %f %f %f]"), m_c0.f1, m_c0.f2, m_r.f1, m_c1.f1, m_c1.f2, m_r.f2); 
			TAB_XXX
			src += val;	

			if (2 == m_Extend.GetCount())
			{
				TAB_XXX
				src += CString(_T("/Extend [ "));

				(m_Extend[0]) ? src += CString(_T("true ")) : src += CString(_T("false "));
				(m_Extend[1]) ? src += CString(_T("true"))  : src += CString(_T("false"));

				src += CString(_T(" ]"));
			}

			if (m_Functions.GetCount())
			{
				TAB_XXX
				src += _T("/Function ");

				val.Format (_T(" %d 0 R"), m_Functions.GetAt(m_Functions.GetCount()-1)->GetId());
				src += val;	
			}

			TAB_XX
			src += CString(_T(">>\012"));

			return src;			
		}

	private:

		float4				m_Coords;			//	Required
		float2				m_Radius;
		float2				m_Domain;			//	Optional
		Matrix				m_Matrix;			//	Optional

		CAtlArray<BOOL>		m_Extend;			//	Optional
		CString				m_ColorSpace;		

		// work
		
		Matrix				m_transform;		//	дополнительное преобразование
		BOOL				m_normalized;
		float2				m_c0;
		float2				m_c1;
		float2				m_r;
	};
	
	// patterns

	class Pattern
	{
	public:
		Pattern()
		{
			m_Type			=	PatternInvalid;
			m_nId			=	-1;
			m_nRefIndex		=	-1;

			m_AlphaPattern	=	NULL;
		}
		virtual ~Pattern()
		{
		}
		Pattern(const Pattern& pattern)
		{
			*this = pattern;
		}
		Pattern& operator=(const Pattern& pattern)
		{
			m_nId	=	pattern.m_nId;
			m_Type	=	pattern.m_Type;

			return *this;
		}

		inline void SetType(PatternType type)
		{
			m_Type	=	type;
		}
		inline void SetId(int id)
		{
			m_nId	=	id;
		}
		inline void SetRefIndex(int index)
		{
			m_nRefIndex	= index;
		}
		inline int GetRefIndex() const
		{
			return m_nRefIndex;
		}
		inline void SetAlphaPattern(Pattern* pattern)
		{
			m_AlphaPattern	=	pattern;
		}

		inline Pattern* GetAlphaPattern()
		{
			return m_AlphaPattern;
		}

		//
		inline PatternType GetType() const
		{
			return m_Type;
		}
		inline int GetId() const
		{
			return m_nId;
		}

		//
		virtual CString Define()
		{
			return CString(_T(""));
		}

	protected:

		int						m_nId;
		int						m_nRefIndex;

		PatternType				m_Type;				//	Required

		Pattern*				m_AlphaPattern;
	};
	class ShadingPattern : public Pattern
	{
	public:
		ShadingPattern()
		{
			m_Type			=	ShadingPatternType;
			m_topShading	=	NULL;
		}
		virtual ~ShadingPattern()
		{
			for (size_t i = 0; i < m_Shadings.GetCount(); ++i)
			{
				RELEASEOBJECT(m_Shadings[i]);
			}
		}

		ShadingPattern& operator=(const ShadingPattern& pattern)
		{
			m_Type	=	pattern.m_Type;
			m_topShading	=	pattern.m_topShading;		

			m_Shadings.Copy(pattern.m_Shadings);

			return *this;
		}
		inline void AddShading(Shading* shading)
		{
			if (shading)
			{
				m_Shadings.Add(shading);

				m_topShading	=	shading;
			}
		}
		inline Shading* GetShading(int index) 
		{
			if (index >= 0 && index < (int)m_Shadings.GetCount())
			{
				return m_Shadings[index];
			}

			return NULL;
		}
		inline Shading* GetTopShading()
		{
			return m_topShading;
		}

		//
		virtual CString Define()
		{
			return CString(_T("<<")) + ShadingPattern::InternalObj() + CString(_T(">>"));
		}

	protected:

		inline CString InternalObj() const
		{
			CString src;	src.Format(_T(" /PatternType %d"), m_Type);

			for (size_t i = 0; i < m_Shadings.GetCount(); ++i)
			{
				src += m_Shadings[i]->Define();
			}

			return src;			
		}

	protected:

		CAtlArray<Shading*>	m_Shadings;

		// work
		Shading*			m_topShading;
	};
	
	// в foxit если паттерн под наклоном, то по краям идут полоски (видимо это бага просмотрщика)

	class TilePattern : public Pattern
	{
	public:
		TilePattern()
		{
			m_Type			=	TilingPatternType;
			m_PaintType		=	UncolouredTilingPattern;
			m_TilingType	=	NoDistortion;

			m_nRefIdX		=	-1;

			m_XStep			=	0;
			m_YStep			=	0;
		}
		virtual ~TilePattern()
		{

		}
		
		TilePattern& operator=(const TilePattern& pattern)
		{
			m_Type			=	pattern.m_Type;
			m_PaintType		=	pattern.m_PaintType;
			m_TilingType	=	pattern.m_TilingType;
		
			m_XStep			=	pattern.m_XStep;
			m_YStep			=	pattern.m_YStep;
			m_BBox   		=	pattern.m_BBox;

			m_nRefIdX		=	pattern.m_nRefIdX;
			m_bound			=	pattern.m_bound;
			m_transform		=	pattern.m_transform;

			return *this;
		}

		inline void SetBox(const Rect& box)
		{
			m_BBox			=	box;
			
			m_XStep			=	m_BBox.Width;
			m_YStep			=	m_BBox.Height;
		}
		inline void SetBound(const Rect& bound)
		{
			m_bound			=	bound;
		}
		inline void SetRefIdX(int index)
		{
			m_nRefIdX		=	index;
		}
		inline void SetTransform(const Matrix& matrix) 
		{
			m_transform		=	matrix;
		}

		inline void CalculateCoords(const Rect& bounds, float pageHeight)
		{		
			Point texture	=	Point( bounds.Width - ((long)(bounds.Width / m_BBox.Width)) * m_BBox.Width, 
				bounds.Height - ((long)(bounds.Height / m_BBox.Height)) * m_BBox.Height);
			texture			=	m_transform.RotatePoint(Point(0, texture.Y));
			
			Point center	=	Point (bounds.Width * 0.5, bounds.Height * 0.5);

			Point offset	=	m_transform.RotatePoint(Point(0, bounds.Height));
			
			m_transform.Set (2, m_transform[2] - offset.X + texture.X);
			m_transform.Set (5, pageHeight - m_transform[5] - offset.Y + texture.Y);
		}

		//
		virtual CString Define()
		{
			return CString(_T("<< ")) + TilePattern::InternalObj() + CString(_T(">>")) + TilePattern::StreamObj();
		}

	protected:

		inline CString InternalObj() const
		{
			CString src;
			CString val;

			val.Format(_T("/PatternType %d "), m_Type);
			src += val;
			
			TAB_X
			val.Format(_T("/PaintType %d "), m_PaintType);
			src += val;
			
			TAB_X
			val.Format(_T("/TilingType %d "), m_TilingType);
			src += val;
			
			TAB_X
			val.Format (_T("/BBox [ %d %d %d %d ] "), (int)m_BBox.X, (int)m_BBox.Y, (int)(m_BBox.X + m_BBox.Width), (int)(m_BBox.Y + m_BBox.Height));
			src += val;
		
			TAB_X
			val.Format(_T("/Matrix [ %f %f %f %f %f %f ] "), m_transform[0], m_transform[1], m_transform[3], m_transform[4], m_transform[2], m_transform[5]);
			src += val;
		
			TAB_X
			val.Format(_T("/XStep %d "), m_XStep);
			src += val;

			TAB_X			
			val.Format(_T("/YStep %d "), m_YStep);
			src += val;

			TAB_X
			val.Format(_T("/Resources << /XObject << /x%d %d 0 R >> >>"), m_nRefIdX, m_nRefIdX);
			src += val;
			TAB

			return src;			
		}
		
		inline CString StreamObj() const 
		{
			// draw commands (TODO : compress)
		
			CString src;
			CString val;
			
			TAB
			src += _T("stream");
			//TAB
			//src += _T("q");
			TAB
			src += _T("q");
			TAB
			val.Format(_T("%d 0 0 %d 0 0 cm"), (int)m_BBox.Width, (int)m_BBox.Height);
			src += val;
			TAB
			val.Format(_T("/x%d Do"), m_nRefIdX);
			src += val;
			TAB
			src += _T("Q");
			//TAB
			//src += _T("Q");
			TAB
			src += _T("endstream");

			return src;
		}

	protected:

		PaintType				m_PaintType;				//	Required
		TilingType				m_TilingType;				//	Required
		Rect					m_BBox;						//	Required
		int						m_XStep;					//	Required
		int						m_YStep;					//	Required

		//CAtlArray<CString>	m_Resources;				//	Required
		Matrix					m_Matrix;					//	Required

		// work

		int						m_nRefIdX;
		Rect					m_bound;
		Matrix					m_transform;
	};

	class SoftMask
	{
	public:
		SoftMask()
		{
			m_nId			=	-1;
			m_Type			=	L"Mask";
			m_SubType		=	L"Luminosity";
			m_GroupId		=	-1;
			m_StateId		=	-1;
		}

		inline void SetId(int i)
		{
			m_nId			=	i;
		}
		inline void SetGroupId(int i)
		{
			m_GroupId		=	i;
		}
		inline void SetStateId(int i)
		{
			m_StateId		=	i;
		}
		
		inline int GetId() const
		{
			return m_nId;
		}
		inline int GetGroupId() const
		{
			return m_GroupId;
		}
		inline int GetStateId() const
		{
			return m_StateId;
		}
		
		inline CString Define()
		{
			CString src;
			CString val;

			val.Format(_T("<< /Type /%s"), m_Type);
			src += val;
			
			TAB_X
			val.Format(_T("/S /%s"), m_SubType);
			src += val;

			TAB_X
			val.Format (_T("/G %d 0 R"), m_GroupId);
			src += val;
			
			TAB
			src += _T(">>");

			return src;
		}

	private:
		
		int			m_nId;
		CString		m_Type;
		CString		m_SubType;
		int			m_GroupId;
		int			m_StateId;
	};
	
	// прозрачность паттерна - привязанная форма с маской в бесцветом режиме 
		
	class PatternAlphaGroup	//	TODO : to xForm
	{
	public:
		PatternAlphaGroup()
		{
			m_nId			=	-1;
			m_FillId		=	-1;
			m_FormId		=	-1;
			m_FormType		=	1;

			m_SoftMask		=	NULL;
		}
		~PatternAlphaGroup()
		{
			RELEASEOBJECT(m_SoftMask);
		}
	
		inline void SetId(int id)
		{
			m_nId	=	id;
		}
		inline void SetBound(Rect bounds)
		{
			m_Bounds	=	bounds;
		}
		inline void SetFillId(int index) 
		{
			m_FillId	=	index;
		}
		inline void SetFormId(int id)
		{
			m_FormId = id;
		}

		//
		inline int GetId() const
		{
			return m_nId;
		}
		inline int GetFormId() const 
		{
			return m_FormId;
		}

		inline SoftMask* GetMask()
		{
			if (NULL == m_SoftMask)
			{
				m_SoftMask = new SoftMask();
			}

			return m_SoftMask;
		}

		inline CString Define()
		{
			CString src;
			CString val;

			val.Format(_T("<< /Type /%s"), L"XObject");
			src += val;
			
			TAB_X
			val.Format(_T("/Subtype /%s "), L"Form");
			src += val;
		
			if (-1 != m_FormType)
			{
				TAB_X
				val.Format(_T("/FormType %d "), m_FormType);
				src += val;
			}
		
			TAB_X
			val.Format (_T("/BBox [ %f %f %f %f ] "), m_Bounds.X, m_Bounds.Y, m_Bounds.X + m_Bounds.Width, m_Bounds.Y + m_Bounds.Height);
			src += val;

			TAB_X
			src += _T("/Resources");
			
			TAB_XX
			src += _T("<< /ExtGState");
			
			TAB_XXXX
			val.Format (_T("<< /a%d << /ca %d >> >>"), 0, 1);
			src += val;
			
			TAB_XXX
			src += _T("/Pattern");
			
			TAB_XXXX
			val.Format (_T("<< /p%d %d 0 R >>"), m_FillId, m_FillId);
			src += val;
			
			TAB_XX
			src += _T(">>");
		
			TAB_X
			src += _T("/Group");
		
			TAB_XX
			src += _T("<< /Type /Group");
			
			TAB_XXX
			src += _T("/S /Transparency");

			TAB_XXX
			src += _T("/CS /DeviceGray");
			
			TAB_XX
			src += _T(">>");

			TAB
			src += _T(">>");

			src += StreamObj();

			return src;
		}

	private:

		inline CString StreamObj() const 
		{
			// draw commands (TODO : compress)
		
			CString src;
			CString val;
			
			TAB
			src += _T("stream");
			TAB
			src += _T("q");
			TAB
			src += _T("/a0 gs");
			TAB
			val.Format(_T("/Pattern cs /p%d scn"), m_FillId);
			src += val;
			TAB
			val.Format(_T("%d %d %d %d re"), (int)m_Bounds.X, (int)m_Bounds.Y, (int)(m_Bounds.X + m_Bounds.Width), (int)(m_Bounds.Y + m_Bounds.Height));
			src += val;
			TAB
			src += _T("f");
			TAB
			src += _T("Q");
			TAB
			src += _T("endstream");

			return src;
		}

	private:
		
		int			m_nId;
		Rect		m_Bounds;
		int			m_FillId;
		int			m_FormId;
		int			m_FormType;

		SoftMask*	m_SoftMask;
	};
	
	class XForm
	{
	public:
		XForm()
		{
			m_nId			=	-1;
			m_Type			=	L"XObject";
			m_SubType		=	L"Form";
		}

		inline void SetId(int i)
		{
			m_nId			=	i;
		}
		inline void SetBound(Rect bounds)
		{
			m_Bounds		=	bounds;
		}
		inline void SetStream(const CString& source)
		{
			m_Stream		=	source;
		}

		inline void AddPattern(Pattern* pattern)
		{
			m_Patterns.Add(pattern);
		}

		inline int GetId() const
		{
			return m_nId;
		}

		inline CString Define()
		{
			CString src;
			CString val;

			val.Format(_T("<< /Type /%s"), m_Type);
			src += val;

			TAB_X
			val.Format(_T("/Subtype /%s"), m_SubType);
			src += val;

			TAB_X
			val.Format(_T("/BBox [ %f %f %f %f ]"), m_Bounds.X, m_Bounds.Y, m_Bounds.X + m_Bounds.Width, m_Bounds.Y + m_Bounds.Height);
			src += val;
			
			if (m_Patterns.GetCount())
			{
				TAB_X
				src += _T("/Resources");
				src += CString(_T(" << "));
				src += CString(_T("/Pattern"));
				src += CString(_T(" << "));

				for (size_t i = 0; i < m_Patterns.GetCount(); ++i)
				{
					val.Format(_T("/P%d %d 0 R "), m_Patterns[i]->GetId(), m_Patterns[i]->GetId());
					src += val;
				}

				src += CString(_T(">>"));
				src += CString(_T(">>"));
			}

			TAB
			src += _T(">>");

			//------------------------------------------------------------------------------------------------

			// TODO: compress commands

			TAB
			src += _T("stream");			
			TAB
			src += m_Stream;			
			TAB
			src += _T("endstream");

			return src;
		}

	public:

		int			m_nId;
		CString		m_Type;
		CString		m_SubType;
		Rect		m_Bounds;
		CString		m_Stream;

		CAtlArray<Pattern*>	m_Patterns;		//	 Только ссылки
	};

	// хранит все объекты для паттернов

	class PatternStorage
	{
	public:
		PatternStorage()
		{
			m_top		=	NULL;
			m_topIndex	=	-1;
		}		
		~PatternStorage()
		{
			for (size_t i = 0; i < m_Patterns.GetCount(); ++i)
			{
				RELEASEOBJECT(m_Patterns[i]);
			}

			for (size_t i = 0; i < m_Masks.GetCount(); ++i)
			{
				RELEASEOBJECT(m_Masks[i]);
			}
			
			for (size_t i = 0; i < m_XForms.GetCount(); ++i)
			{
				RELEASEOBJECT(m_XForms[i]);
			}			
		}

		inline void Add(Pattern* pattern)
		{
			if (pattern)
			{
				m_Patterns.Add(pattern);

				m_top		=	pattern;
				m_topIndex	=	(int)m_Patterns.GetCount();
				pattern->SetRefIndex(m_topIndex);
			}
		}
		inline void AddMask(PatternAlphaGroup* mask)
		{
			m_Masks.Add(mask);
		}
		inline void AddXForm(XForm* form)
		{
			m_XForms.Add(form);
		}

		inline Pattern* GetPattern(int index)
		{
			if (index >= 0 && index < (int)m_Patterns.GetCount())
			{
				return m_Patterns[index];
			}

			return NULL;
		}
		inline Pattern* GetTop()
		{
			return m_top;
		}

		inline PatternAlphaGroup* GetAlphaGroupTop()
		{
			if (m_Masks.GetCount())
			{
				return m_Masks[m_Masks.GetCount() - 1];
			}

			return NULL;
		}
		inline int GetTopIndex() const
		{
			return m_topIndex;
		}

		inline int GetPatternsCount() const
		{
			return (int)m_Patterns.GetCount();
		}

		virtual CString Defines()
		{
			CString src;
			CString val;

			src += CString(_T("<<"));
			TAB

			for (size_t i = 0; i < m_Patterns.GetCount(); ++i)
			{
				val.Format(_T("/P%d %d 0 R\012"), (int)(i + 1), m_Patterns[i]->GetId());
				src += val;
			}

			src += CString(_T(">>"));			
			
			return src;
		}

	protected:

		CAtlArray<Pattern*>					m_Patterns;
		Pattern*							m_top;
		int									m_topIndex;

		CAtlArray<PatternAlphaGroup*>		m_Masks;
		CAtlArray<XForm*>					m_XForms;
	};
}

namespace PDF
{
	class PaintState
	{
	public:
		PaintState()
		{
			m_fill		=	NULL;
			m_stroke	=	NULL;
			m_tiling	=	NULL;

			m_TileMode	=	FALSE;
		}

		inline Pattern* GetFill()
		{
			return m_fill;
		}
		inline Pattern* GetStroke()
		{
			return m_stroke;
		}
		inline Pattern* GetTiling()
		{
			return m_tiling;
		}

		inline void SetFill(Pattern* pattern)
		{
			m_fill	=	pattern;	
		}
		inline void SetStroke(Pattern* pattern)
		{
			m_stroke	=	pattern;
		}
		inline void SetTiling(Pattern* pattern)
		{
			m_tiling	=	pattern;
		}
		inline void SetTileMode(BOOL mode)
		{
			m_TileMode = mode;
		}

		inline void Reset()
		{
			m_fill		=	NULL;
			m_stroke	=	NULL;
			m_tiling	=	NULL;

			m_TileMode	=	FALSE;
		}

		inline BOOL IsUse()
		{
			return m_TileMode || (NULL != m_fill) || (NULL != m_stroke) || (NULL != m_tiling);
		}

		inline BOOL IsFill() const
		{
			return (NULL != m_fill);
		}
		inline BOOL IsStroke() const
		{
			return (NULL != m_stroke);
		}
		inline BOOL IsTiling() const
		{
			return (NULL != m_tiling);
		}

	private:

		Pattern*	m_fill;
		Pattern*	m_stroke;
		Pattern*	m_tiling;

		BOOL		m_TileMode;
	};
}

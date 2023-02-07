#ifndef SVGTYPES_H
#define SVGTYPES_H

#include <cstring>
#include <cmath>

#include "../../../Common/3dParty/html/css/src/StyleProperties.h"

namespace SVG
{
    #define MapCI std::map<std::wstring, std::wstring>::const_iterator

    #define SvgDigit  NSCSS::NSProperties::CDigit
    #define SvgString NSCSS::NSProperties::CString
    #define SvgColor  NSCSS::NSProperties::CColor
    #define SvgMatrix NSCSS::NSProperties::CMatrix

    #define SvgFont   NSCSS::NSProperties::CFont

	struct CStroke
	{
		SvgColor m_oColor;
		SvgDigit m_oWidth;
	};

	struct Point
	{
		double dX;
		double dY;

		Point& operator+=(const Point& oPoint)
		{
			dX += oPoint.dX;
			dY += oPoint.dY;

			return *this;
		}

		Point& operator-=(const Point& oPoint)
		{
			dX -= oPoint.dX;
			dY -= oPoint.dY;

			return *this;
		}

		Point operator+(const Point& oPoint)
		{
			return Point{dX + oPoint.dX, dY + oPoint.dY};
		}

		Point operator-(const Point& oPoint)
		{
			return Point{dX - oPoint.dX, dY - oPoint.dY};
		}

		void Rotate(double dAngle, const Point& oPoint)
		{
			double dOldX = dX;
			double dOldY = dY;

			dAngle *= 3.1415926535 / 180;

			dX = dOldX + cos(dAngle) * (oPoint.dX - dOldX) - sin(dAngle) * (oPoint.dY - dOldY);
			dY = dOldY + sin(dAngle) * (oPoint.dX - dOldX) + cos(dAngle) * (oPoint.dY - dOldY);
		}
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

		inline Matrix Copy() const
		{
			return (*this);
		}

		inline const double& operator [] (int i) const
		{
			return m_fMat[i];
		}

		inline Matrix& operator *= (const Matrix& transform)
		{
			double mat[9];

			for (int i = 0 ; i < 3; ++i)
			{
				for( int k = 0; k < 3; ++k)
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

		inline Matrix& Move(double fX, double fY)
		{
			m_fMat[2] += fX;
			m_fMat[5] += fY;

			return (*this);
		}

		inline Matrix& Translate(double dX, double dY)
		{
			m_fMat[2] = dX;
			m_fMat[5] = dY;

			return (*this);
		}

		//
		inline static Matrix ScaleTransform(double dX, double dY)
		{
			Matrix mat;

			mat.m_fMat[0] = dX;
			mat.m_fMat[4] = dY;

			return mat;
		}

		inline static Matrix RotateTransform(double angle)
		{
			Matrix mat;

			mat.m_fMat[0] =	cos(angle);
			mat.m_fMat[1] =	-sin(angle);

			mat.m_fMat[3] = sin(angle);
			mat.m_fMat[4] = cos(angle);

			return mat;
		}

		inline static Matrix TranslateTransform(double dX, double dY)
		{
			Matrix mat;

			mat.m_fMat[2] = dX;
			mat.m_fMat[5] = dY;

			return mat;
		}

		inline static Matrix ApplyAt(double dX, double dY, const Matrix& transform)
		{
			Matrix mat;

			mat.m_fMat[2] = -dX;
			mat.m_fMat[5] = -dY;

			mat	*=	transform;

			mat.m_fMat[2] += dX;
			mat.m_fMat[5] += dY;

			return mat;
		}

		static Matrix Rotate(const Point& point, const double& angle)
		{
			Matrix mat;

			mat.m_fMat[6] = point.dX;
			mat.m_fMat[7] = point.dY;

			mat *= Matrix::RotateTransform (angle);

			mat.m_fMat[6] += point.dX;
			mat.m_fMat[7] += point.dY;

			return mat;
		}

		//
		Point Transform (const Point& point)
		{
			return Point{point.dX * m_fMat[0] + point.dY * m_fMat [3] + m_fMat[6], point.dX * m_fMat[1] + point.dY * m_fMat [4] + m_fMat[7]};
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
//		inline bool FromString(const std::wstring& sTransforms, UnitSystem& us)
//		{
//			std::vector<std::wstring> arrMatrix;
//			if (GetTranforms(sTransforms, arrMatrix))
//			{
//				Matrix matrix;

//				for (size_t i = 0; i < arrMatrix.size(); ++i)
//					matrix *= Matrix::ReadMatrix(arrMatrix[i], us);

//				*this = matrix;

//				return true;
//			}

//			return false;
//		}
//		inline static std::wstring GetTransform(const std::wstring& strMat, std::wstring::size_type& index)
//		{
//			std::wstring strText = StringHelpers::Tokenize(strMat, L"(", index);
//			if (std::wstring::npos == index)
//				return L"";

//			StrUtils::RemoveSpaces(strText);
//			return strText;
//		}
//		inline static Matrix ReadMatrix(const std::wstring& sSrc, UnitSystem& us)
//		{
//			std::wstring::size_type index = 0;
//			std::wstring matType = GetTransform(sSrc, index);
//			if (matType.empty() || std::wstring::npos == index)
//				return Matrix();

//			std::wstring sMat = sSrc.substr(index, sSrc.length() - index);
//			if (sMat.empty())
//				return Matrix();

//			std::vector<double> mat;
//			if (StrUtils::DoubleValues(sMat, mat))
//			{
//				size_t count = mat.size();
//				if (L"matrix" == matType)
//				{
//					Matrix oTransform;

//					/*

//					oTransform.m_fMat [ 0 ]	=	mat [ 0 ];														//	a
//					oTransform.m_fMat [ 1 ]	=	mat [ 2 ];														//	c
//					oTransform.m_fMat [ 2 ]	=	mat [ 4 ];		//	e	//	tx
//					oTransform.m_fMat [ 3 ]	=	mat [ 1 ];														//	b
//					oTransform.m_fMat [ 4 ]	=	mat [ 3 ];														//	d
//					oTransform.m_fMat [ 5 ]	=	mat [ 5 ];		//	f	//	ty
//					oTransform.m_fMat [ 6 ]	=	0.0;
//					oTransform.m_fMat [ 7 ]	=	0.0;
//					oTransform.m_fMat [ 8 ]	=	1.0;

//					*/

//					//matrix(0<a> 1<b> 2<c> 3<d> 4<e> 5<f>)
//					oTransform.m_fMat[0] = mat[0];														//	a
//					oTransform.m_fMat[1] = mat[2];														//	c
//					oTransform.m_fMat[2] = us.Convert(mat[4], UNDEFINED, c_dirHorizontal);		//	e	//	tx
//					oTransform.m_fMat[3] = mat[1];														//	b
//					oTransform.m_fMat[4] = mat[3];														//	d
//					oTransform.m_fMat[5] = us.Convert(mat[5], UNDEFINED, c_dirVertical);		//	f	//	ty
//					oTransform.m_fMat[6] = 0.0;
//					oTransform.m_fMat[7] = 0.0;
//					oTransform.m_fMat[8] = 1.0;

//					return oTransform;
//				}
//				else if (L"translate" == matType)
//				{
//					if (1 == count)	return	Matrix::TranslateTransform(us.Convert(mat[0], UNDEFINED, c_dirHorizontal), 0.0);
//					if (2 == count)	return	Matrix::TranslateTransform(us.Convert(mat[0], UNDEFINED, c_dirHorizontal), us.Convert(mat[1], UNDEFINED, c_dirVertical));
//				}
//				else if (L"scale" == matType)
//				{
//					if (1 == count)	return Matrix::ScaleTransform(mat[0], mat[0]);
//					if (2 == count)	return Matrix::ScaleTransform(mat[0], mat[1]);
//				}
//				else if (L"rotate" == matType)
//				{
//					if (1 == count)	return	Matrix::RotateTransform(mat[0] * M_PI / 180.0);
//					if (3 == count)
//					{
//						Matrix oTransform = Matrix::TranslateTransform(mat[1], mat[2]);

//						oTransform	*=	Matrix::RotateTransform(mat[0] * M_PI / 180);
//						oTransform	*=	Matrix::TranslateTransform(-mat[1], -mat[2]);

//						return	oTransform;
//					}
//				}
//				else if (L"skewX" == matType)
//				{
//					Matrix oTransform;
//					oTransform.m_fMat[1] = tan(mat[0] * M_PI / 180.0);		//	c

//					return oTransform;
//				}
//				else if (L"skewY" == matType)
//				{
//					Matrix oTransform;
//					oTransform.m_fMat[3] = tan(mat[0] * M_PI / 180.0);		//	b

//					return oTransform;
//				}
//			}

//			return Matrix();
//		}
//		inline static bool GetTranforms(const std::wstring& strMat, std::vector<std::wstring>& refTransforms)
//		{
//			std::wstring::size_type index = 0;
//			while (std::wstring::npos != index)
//			{
//				std::wstring sToken = StringHelpers::Tokenize(strMat, L")", index);
//				if (!sToken.empty())
//					refTransforms.push_back(sToken);
//			}

//			return (0 != refTransforms.size());
//		}

	private:

		double	m_fMat[9];
	};

}

#endif // SVGTYPES_H

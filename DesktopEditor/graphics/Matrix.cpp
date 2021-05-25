/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Matrix_private.h"

namespace Aggplus
{
    CMatrix::CMatrix(double m11, double m12, double m21, double m22, double dx, double dy)
	{
        m_internal = new CMatrix_private();
        m_internal->m_agg_mtx.sx = m11;
        m_internal->m_agg_mtx.shy = m12;
        m_internal->m_agg_mtx.shx = m21;
        m_internal->m_agg_mtx.sy = m22;
        m_internal->m_agg_mtx.tx = dx;
        m_internal->m_agg_mtx.ty = dy;
	}

    CMatrix::CMatrix(const CMatrix& oSrc)
    {
        m_internal = new CMatrix_private();
        m_internal->m_agg_mtx = oSrc.m_internal->m_agg_mtx;
    }

    CMatrix::CMatrix()
	{
        m_internal = new CMatrix_private();
	}
	
	CMatrix::~CMatrix()
	{
        RELEASEOBJECT(m_internal);
	}
	
	void CMatrix::Translate(double offsetX, double offsetY, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
            m_internal->m_agg_mtx.premultiply(agg::trans_affine_translation(offsetX, offsetY));
		}
		else                            
		{ 
            m_internal->m_agg_mtx.multiply(agg::trans_affine_translation(offsetX, offsetY));
		}
	}

	void CMatrix::Scale(double scaleX, double scaleY, MatrixOrder order)
	{	
		if (order == MatrixOrderPrepend) 
		{ 
            m_internal->m_agg_mtx.premultiply(agg::trans_affine_scaling(scaleX, scaleY));
		}
		else                            
		{ 
            m_internal->m_agg_mtx.multiply(agg::trans_affine_scaling(scaleX, scaleY));
		}
	}

	void CMatrix::Shear(double shearX, double shearY, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
            m_internal->m_agg_mtx.premultiply(agg::trans_affine_skewing(shearX, shearY));
		}
		else                            
		{ 
            m_internal->m_agg_mtx.multiply(agg::trans_affine_skewing(shearX, shearY));
		}
	}

	void CMatrix::TransformVectors(PointF* pts, int count)
	{
		// Store matrix to an array [6] of double
        double M[6]; m_internal->m_agg_mtx.store_to(M);

        for (int i = 0; i < count; ++i)
		{
            double x = pts[i].X;
            double y = pts[i].Y;
            m_internal->m_agg_mtx.transform(&x, &y);
            pts[i].X = (float)(x-M[4]);
            pts[i].Y = (float)(y-M[5]);
		}
	}

	void CMatrix::TransformPoints(PointF* pts, int count)
	{
        for (int i = 0; i < count; ++i)
		{
			double x = pts[i].X;
			double y = pts[i].Y;
            m_internal->m_agg_mtx.transform(&x, &y);
			pts[i].X = (float)x;
			pts[i].Y = (float)y;
		}
	}

	void CMatrix::TransformPoint(double& x, double& y)
	{
        m_internal->m_agg_mtx.transform(&x, &y);
	}

	void CMatrix::Rotate(double angle, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
            m_internal->m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle)));
		}
		else
		{ 
            m_internal->m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle)));
		}
	}

	void CMatrix::RotateAt(double angle, const PointF &center, MatrixOrder order)
	{
		Translate(-center.X, -center.Y, order);
		if(order == MatrixOrderPrepend) 
		{ 
            m_internal->m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle)));
		}
		else                            
		{ 
            m_internal->m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle)));
		}
		Translate(center.X, center.Y, order);
		return;
	}

	void CMatrix::RotateAt(double angle, double x, double y, MatrixOrder order)
	{
		Translate(-x, -y, order);
		if (order == MatrixOrderPrepend) 
		{ 
            m_internal->m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle)));
		}
		else                            
		{ 
            m_internal->m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle)));
		}
		Translate(x, y, order);
	}

	void CMatrix::Multiply(const CMatrix* matrix, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
            m_internal->m_agg_mtx.premultiply(matrix->m_internal->m_agg_mtx);
		}
		else
		{ 
            m_internal->m_agg_mtx.multiply(matrix->m_internal->m_agg_mtx);
		}
	}

	double CMatrix::OffsetX() const
	{
        return m_internal->m_agg_mtx.tx;
	}
	double CMatrix::OffsetY() const
	{
        return m_internal->m_agg_mtx.ty;
	}

    double CMatrix::sx() const
    {
        return m_internal->m_agg_mtx.sx;
    }
    double CMatrix::sy() const
    {
        return m_internal->m_agg_mtx.sy;
    }
    double CMatrix::shx() const
    {
        return m_internal->m_agg_mtx.shx;
    }
    double CMatrix::shy() const
    {
        return m_internal->m_agg_mtx.shy;
    }
    double CMatrix::tx() const
    {
        return m_internal->m_agg_mtx.tx;
    }
    double CMatrix::ty() const
    {
        return m_internal->m_agg_mtx.ty;
    }        
    double CMatrix::rotation()
    {
        return m_internal->m_agg_mtx.rotation();
    }


    void CMatrix::SetElements(const double& sx, const double& shy, const double& shx, const double& sy, const double& tx, const double& ty)
    {
        m_internal->m_agg_mtx.sx = sx;
        m_internal->m_agg_mtx.shy = shy;
        m_internal->m_agg_mtx.shx = shx;
        m_internal->m_agg_mtx.sy = sy;
        m_internal->m_agg_mtx.tx = tx;
        m_internal->m_agg_mtx.ty = ty;
    }
	
	Status CMatrix::GetElements(REAL* m) const
	{
        double M[6]; m_internal->m_agg_mtx.store_to(M);
		m[0]=(REAL)M[0];
		m[1]=(REAL)M[1];
		m[2]=(REAL)M[2];
		m[3]=(REAL)M[3];
		m[4]=(REAL)M[4];
		m[5]=(REAL)M[5];
		return Ok;
	}

	Status CMatrix::GetElements(double* m) const
	{
        m_internal->m_agg_mtx.store_to(m);
		return Ok;
	}

	void CMatrix::Reset()
	{
        m_internal->m_agg_mtx.reset();
	}

	double CMatrix::Determinant() const
	{
        return m_internal->m_agg_mtx.determinant();
    }

	const CMatrix& CMatrix::operator=(const CMatrix& Src)
	{
        m_internal->m_agg_mtx = Src.m_internal->m_agg_mtx;
        return *this;
	}

	Status CMatrix::Invert()
	{
        double d = m_internal->m_agg_mtx.determinant();
        if (0.0001 >= fabs(d))
			return InvalidParameter;

        m_internal->m_agg_mtx.invert();
		return Ok;
	}

	//Temp
	//Used in X_BrushLinearGradient constructor
	double CMatrix::z_Rotation() const 
	{ 
        return agg::rad2deg(m_internal->m_agg_mtx.rotation());
	}

    void CMatrix::TransformPoints( PointF* dst, const PointF* src, int count )
	{
        agg::trans_affine& m = m_internal->m_agg_mtx;
		for(int i = 0; i < count; ++i)
		{
            double x = src[i].X * m.sx + src[i].Y * m.shx + m.tx;
            double y = src[i].Y * m.sy + src[i].X * m.shy + m.ty;
			
			dst[i].X = (float)x;
			dst[i].Y = (float)y;
		}
	}

    bool CMatrix::IsIdentity(const double& eps) const
	{
        return m_internal->m_agg_mtx.is_identity(eps);
	}
    bool CMatrix::IsIdentity2(const double& eps) const
	{
        agg::trans_affine& m = m_internal->m_agg_mtx;
        return agg::is_equal_eps(m.sx,  1.0, eps) &&
               agg::is_equal_eps(m.shy, 0.0, eps) &&
               agg::is_equal_eps(m.shx, 0.0, eps) &&
               agg::is_equal_eps(m.sy,  1.0, eps);
	}

    bool CMatrix::IsEqual(const CMatrix* mm1, const CMatrix* mm2, const double& eps, bool bIsOnlyMain)
    {
        agg::trans_affine& m1 = mm1->m_internal->m_agg_mtx;
        agg::trans_affine& m2 = mm2->m_internal->m_agg_mtx;

        bool bMain = (fabs(m1.sx  - m2.sx) < eps &&
                      fabs(m1.sy  - m2.sy) < eps &&
                      fabs(m1.shx - m2.shx) < eps &&
                      fabs(m1.shy - m2.shy) < eps) ? true : false;

        if (!bMain || bIsOnlyMain)
            return bMain;

        return (fabs(m1.tx - m2.tx) < eps && fabs(m1.ty - m2.ty) < eps) ? true : false;
    }
}

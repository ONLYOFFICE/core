/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _BUILD_MATRIX_H_
#define _BUILD_MATRIX_H_

#include "AggPlusEnums.h"
#include "../agg-2.4/include/agg_trans_affine.h"

namespace Aggplus
{

class CMatrix  
{
	friend class CGraphics;
	friend class CGraphicsPath;

public:
	CMatrix(double m11, double m12, double m21, double m22, double dx, double dy);
	CMatrix();
    CMatrix(const CMatrix& oSrc);
	
	~CMatrix();
	
	void Translate(double offsetX, double offsetY, MatrixOrder order = MatrixOrderPrepend);
	void Scale(double scaleX, double scaleY, MatrixOrder order = MatrixOrderPrepend);
	void Shear(double shearX, double shearY, MatrixOrder order = MatrixOrderPrepend);

	void TransformVectors(PointF* pts, int count);
	void TransformPoints(PointF* pts, int count);
	void TransformPoint(double& x, double& y);

	void Rotate(double angle, MatrixOrder order = MatrixOrderPrepend);
	void RotateAt(double angle, const PointF &center, MatrixOrder order = MatrixOrderPrepend);
	void RotateAt(double angle, double x, double y, MatrixOrder order = MatrixOrderPrepend);

	void Multiply(const CMatrix* matrix, MatrixOrder order = MatrixOrderPrepend);

	double OffsetX() const;
	double OffsetY() const;
	
	Status GetElements(float* m) const;
	Status GetElements(double* m) const;

	void Reset();
	bool IsIdentity() const;
	bool IsIdentity2() const;

	const CMatrix& operator=(const CMatrix& Src);

	Status Invert();

	//Temp
	//Used in X_BrushLinearGradient constructor
	double z_Rotation() const;

	void TransformPoints( PointF* dst, const PointF* src, int count ) const;

public:
	agg::trans_affine m_agg_mtx;
};

}

#endif // _BUILD_MATRIX_H_
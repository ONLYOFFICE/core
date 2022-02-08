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
#ifndef _BUILD_MATRIX_H_
#define _BUILD_MATRIX_H_

#include "AggPlusEnums.h"
#include "./config.h"

namespace Aggplus
{
    class CMatrix_private;
    class GRAPHICS_DECL CMatrix
    {
    public:
        CMatrix(double m11, double m12, double m21, double m22, double dx, double dy);
        CMatrix();
        CMatrix(const CMatrix& oSrc);

        ~CMatrix();

        void Translate(double offsetX, double offsetY, MatrixOrder order = MatrixOrderPrepend);
        void Scale(double scaleX, double scaleY, MatrixOrder order = MatrixOrderPrepend);
        void Shear(double shearX, double shearY, MatrixOrder order = MatrixOrderPrepend);
        double Determinant() const;

        void TransformVectors(PointF* pts, int count);
        void TransformPoints(PointF* pts, int count);
        void TransformPoint(double& x, double& y);
        void TransformPoints(PointF* dst, const PointF* src, int count);

        void Rotate(double angle, MatrixOrder order = MatrixOrderPrepend);
        void RotateAt(double angle, const PointF &center, MatrixOrder order = MatrixOrderPrepend);
        void RotateAt(double angle, double x, double y, MatrixOrder order = MatrixOrderPrepend);

        void Multiply(const CMatrix* matrix, MatrixOrder order = MatrixOrderPrepend);

        double OffsetX() const;
        double OffsetY() const;

        double sx() const;
        double sy() const;
        double shx() const;
        double shy() const;
        double tx() const;
        double ty() const;
        double rotation();
        void SetElements(const double& sx, const double& shy, const double& shx, const double& sy, const double& tx = 0, const double& ty = 0);

        Status GetElements(float* m) const;
        Status GetElements(double* m) const;

        void Reset();
        bool IsIdentity(const double& eps = 0.00001) const;
        bool IsIdentity2(const double& eps = 0.00001) const;

        static bool IsEqual(const CMatrix* m1, const CMatrix* m2, const double& eps = 0.001, bool bIsOnlyMain = false);

        const CMatrix& operator=(const CMatrix& Src);

        Status Invert();

        double z_Rotation() const;

    public:
        CMatrix_private* m_internal;
    };
}

#endif // _BUILD_MATRIX_H_

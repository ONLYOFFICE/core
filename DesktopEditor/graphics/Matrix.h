/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

        void TransformVectors(PointF* pts, int count) const;
        void TransformPoints(PointF* pts, int count) const;
        void TransformPoint(double& x, double& y) const;
        void TransformPoints(PointF* dst, const PointF* src, int count) const;

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

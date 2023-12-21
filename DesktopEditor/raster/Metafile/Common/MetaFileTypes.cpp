/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "MetaFileTypes.h"

namespace MetaFile
{
	TRGBA::TRGBA() : r(0), g(0), b(0), a(0)
	{}

	TRGBA::TRGBA(const TRGBA &oRGB)
		: r(oRGB.r), g(oRGB.g), b(oRGB.b), a(oRGB.a)
	{}

	TRGBA::TRGBA(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
		: r(_r), g(_g), b(_b), a(_a)
	{}

	TRGBA::TRGBA(int nValue) : r((nValue >> 0)  & 0xFF), g((nValue >> 8)  & 0xFF), b((nValue >> 16)  & 0xFF), a((nValue >> 24)  & 0xFF)
	{}

	void TRGBA::Set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
	{
		r = _r;
		g = _g;
		b = _b;
	}

	void TRGBA::Copy(const TRGBA &oRGBA)
	{
		r = oRGBA.r;
		g = oRGBA.g;
		b = oRGBA.b;
		a = oRGBA.a;
	}

	int TRGBA::ToInt()
	{
		return METAFILE_RGBA(r, g, b, a);
	}

	void TRGBA::SwapToBGR()
	{
		unsigned char t = r;
		r = b;
		b = t;
	}

	TXForm::TXForm() : M11(1), M12(0), M21(0), M22(1), Dx(0), Dy(0)
	{}

	TXForm::TXForm(const TXForm &oXForm) 
		: M11(oXForm.M11), M12(oXForm.M12), M21(oXForm.M21), M22(oXForm.M22), Dx(oXForm.Dx), Dy(oXForm.Dy)
	{}

	TXForm::TXForm(double m11, double m12, double m21, double m22, double dx, double dy)
		: M11(m11), M12(m12), M21(m21), M22(m22), Dx(dx), Dy(dy)
	{}

	void TXForm::Init()
	{
		M11 = 1;
		M12 = 0;
		M21 = 0;
		M22 = 1;
		Dx  = 0;
		Dy  = 0;
	}

	void TXForm::Copy(const TXForm *pOther)
	{
		if (NULL == pOther)
			return;

		M11 = pOther->M11;
		M12	= pOther->M12;
		M21	= pOther->M21;
		M22	= pOther->M22;
		Dx	= pOther->Dx;
		Dy	= pOther->Dy;
	}

	void TXForm::Multiply(TXForm &oOther, unsigned int ulMode)
	{
		if (MWT_IDENTITY == ulMode)
			Init();
		else if (MWT_LEFTMULTIPLY == ulMode)
		{
			// oOther слева, текущая матрица справа
			double dM11 = oOther.M11 * M11 + oOther.M12 * M21;
			double dM12 = oOther.M11 * M12 + oOther.M12 * M22;
			double dM21 = oOther.M21 * M11 + oOther.M22 * M21;
			double dM22 = oOther.M21 * M12 + oOther.M22 * M22;

			double dDx = oOther.Dx * M11 + oOther.Dy * M21 + Dx;
			double dDy = oOther.Dx * M12 + oOther.Dy * M22 + Dy;

			M11 = dM11;
			M12 = dM12;
			M21 = dM21;
			M22 = dM22;
			Dx  = dDx;
			Dy  = dDy;
		}
		else if (MWT_RIGHTMULTIPLY == ulMode)
		{
			// oOther справа, текущая матрица слева
			double dM11 = M11 * oOther.M11 + M12 * oOther.M21;
			double dM12 = M11 * oOther.M12 + M12 * oOther.M22;
			double dM21 = M21 * oOther.M11 + M22 * oOther.M21;
			double dM22 = M21 * oOther.M12 + M22 * oOther.M22;

			double dDx = Dx * oOther.M11 + Dy * oOther.M21 + oOther.Dx;
			double dDy = Dx * oOther.M12 + Dy * oOther.M22 + oOther.Dy;

			M11 = dM11;
			M12 = dM12;
			M21 = dM21;
			M22 = dM22;
			Dx  = dDx;
			Dy  = dDy;
		}
		else //if (MWT_SET == ulMode)
		{
			Copy(&oOther);
		}
	}

	void TXForm::Apply(double &dX, double &dY)
	{
		double _dX = dX;
		double _dY = dY;

		dX = _dX * M11 + _dY * M21 + Dx;
		dY = _dX * M12 + _dY * M22 + Dy;
	}

	template<>
	bool TPoint<double>::operator==(const TPoint<double>& oPoint) const
	{
		return std::abs(X - oPoint.X) <= DBL_EPSILON && std::abs(Y - oPoint.Y) <= DBL_EPSILON;
	}

	template<>
	bool TPoint<double>::operator!=(const TPoint<double>& oPoint) const
	{
		return std::abs(X - oPoint.X) > DBL_EPSILON || std::abs(Y - oPoint.Y) > DBL_EPSILON;
	}

	template<>
	bool TRect<double>::operator==(const TRect<double>& oRect) const
	{
		return std::abs(Left   - oRect.Right)  <= DBL_EPSILON &&
			   std::abs(Top    - oRect.Top)    <= DBL_EPSILON &&
			   std::abs(Right  - oRect.Right)  <= DBL_EPSILON &&
			   std::abs(Bottom - oRect.Bottom) <= DBL_EPSILON;
	}

	template<>
	bool TRect<double>::operator!=(const TRect<double>& oRect) const
	{
		return std::abs(Left   - oRect.Right)  > DBL_EPSILON ||
			   std::abs(Top    - oRect.Top)    > DBL_EPSILON ||
			   std::abs(Right  - oRect.Right)  > DBL_EPSILON ||
			   std::abs(Bottom - oRect.Bottom) > DBL_EPSILON;
	}
}

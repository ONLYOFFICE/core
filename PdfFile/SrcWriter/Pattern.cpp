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
#include "Pattern.h"
#include "Image.h"
#include "Streams.h"

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CPattern
	//----------------------------------------------------------------------------------------
	CPattern::CPattern(CXref* pXref) : CDictObject(pXref)
	{
	}
	//----------------------------------------------------------------------------------------
	// CImageTilePattern
	//----------------------------------------------------------------------------------------
	CImageTilePattern::CImageTilePattern(CXref* pXref, const double& dW, const double& dH, CImageDict* pImageDict, CMatrix* pMatrix, EImageTilePatternType eType, double dXStepSpacing, double dYStepSpacing) : CPattern(pXref)
	{
		Add("Type", "Pattern");
		Add("PatternType", 1);
		Add("PaintType", 1);    // Uncolored
		Add("TilingType", 1);   // No distortion

		if (pMatrix)
		{
			CArrayObject* pMatrixArray = new CArrayObject();
			if (!pMatrixArray)
				return;

			pMatrixArray->Add(pMatrix->m11);
			pMatrixArray->Add(pMatrix->m12);
			pMatrixArray->Add(pMatrix->m21);
			pMatrixArray->Add(pMatrix->m22);
			pMatrixArray->Add(pMatrix->x);
			pMatrixArray->Add(pMatrix->y);
			Add("Matrix", pMatrixArray);
		}

		CDictObject* pResources = new CDictObject();
		if (!pResources)
			return;

		Add("Resources", pResources);
		CDictObject* pXObject = new CDictObject();
		if (!pXObject)
			return;

		pResources->Add("XObject", pXObject);
		pXObject->Add("X1", pImageDict);

		CStream* pStream = GetStream();
		if (!pStream)
			return;

		if (imagetilepatterntype_Default == eType)
		{
			if (dXStepSpacing > 0.01 && dYStepSpacing > 0.01)
				Add("BBox", CArrayObject::CreateBox(-dXStepSpacing / 2, -dYStepSpacing / 2, dW + dXStepSpacing / 2, dH + dYStepSpacing / 2));
			else
				Add("BBox", CArrayObject::CreateBox(0, 0, dW, dH));

			Add("XStep", dW + dXStepSpacing);
			Add("YStep", dH + dYStepSpacing);

			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" 0 0 cm\12");
			pStream->WriteStr("/X1 Do\12");
		}
		else if (imagetilepatterntype_InverseX == eType)
		{
			Add("BBox", CArrayObject::CreateBox(0, 0, 2 * dW, dH));
			Add("XStep", 2 * dW + dXStepSpacing);
			Add("YStep", dH + dYStepSpacing);

			pStream->WriteStr("q\12");
			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" 0 0 cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteReal(-dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" ");
			pStream->WriteReal(2 * dW);
			pStream->WriteStr(" 0 cm\12");
			pStream->WriteStr("/X1 Do\12");
		}
		else if (imagetilepatterntype_InverseY == eType)
		{
			Add("BBox", CArrayObject::CreateBox(0, 0, dW, 2 * dH));
			Add("XStep", dW + dXStepSpacing);
			Add("YStep", 2 * dH + dYStepSpacing);

			pStream->WriteStr("q\12");
			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" 0 0 cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(-dH);
			pStream->WriteStr(" 0 ");
			pStream->WriteReal(2 * dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
		}
		else if (imagetilepatterntype_InverseXY == eType)
		{
			Add("BBox", CArrayObject::CreateBox(0, 0, 2 * dW, 2 * dH));
			Add("XStep", 2 * dW + dXStepSpacing);
			Add("YStep", 2 * dH + dYStepSpacing);

			pStream->WriteStr("q\12");
			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" 0 0 cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteStr("q\12");
			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(-dH);
			pStream->WriteStr(" 0 ");
			pStream->WriteReal(2 * dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteStr("q\12");
			pStream->WriteReal(-dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" ");
			pStream->WriteReal(2 * dW);
			pStream->WriteStr(" 0 cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteReal(-dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(-dH);
			pStream->WriteStr(" ");
			pStream->WriteReal(2 * dW);
			pStream->WriteStr(" ");
			pStream->WriteReal(2 * dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
		}
	}
}

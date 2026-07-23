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
			pStream->WriteStr(" 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(-dH);
			pStream->WriteStr(" 0 ");
			pStream->WriteReal(dH);
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
			pStream->WriteStr(" 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteStr("q\12");
			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(-dH);
			pStream->WriteStr(" 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteStr("q\12");
			pStream->WriteReal(-dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" ");
			pStream->WriteReal(2 * dW);
			pStream->WriteStr(" ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
			pStream->WriteStr("Q\12");

			pStream->WriteReal(-dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(-dH);
			pStream->WriteStr(" ");
			pStream->WriteReal(2 * dW);
			pStream->WriteStr(" ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" cm\12");
			pStream->WriteStr("/X1 Do\12");
		}
	}
}

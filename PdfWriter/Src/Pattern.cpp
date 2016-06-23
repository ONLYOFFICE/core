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
	CImageTilePattern::CImageTilePattern(CXref* pXref, const double& dW, const double& dH, CImageDict* pImageDict, CMatrix* pMatrix, EImageTilePatternType eType) : CPattern(pXref)
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
			Add("BBox", CArrayObject::CreateBox(0, 0, dW, dH));
			Add("XStep", dW);
			Add("YStep", dH);

			pStream->WriteReal(dW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dH);
			pStream->WriteStr(" 0 0 cm\12");
			pStream->WriteStr("/X1 Do\12");
		}
		else if (imagetilepatterntype_InverseX == eType)
		{
			Add("BBox", CArrayObject::CreateBox(0, 0, 2 * dW, dH));
			Add("XStep", 2 * dW);
			Add("YStep", dH);

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
			Add("XStep", dW);
			Add("YStep", 2 * dH);

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
			Add("XStep", 2 * dW);
			Add("YStep", 2 * dH);

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
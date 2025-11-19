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

#include "RedactOutputDev.h"
#include "Types.h"
#include "Streams.h"
#include "Utils.h"
#include "Image.h"
#include "ResourcesDictionary.h"

#include "../lib/xpdf/Gfx.h"
#include "../lib/xpdf/GfxFont.h"
#include "../lib/xpdf/XRef.h"
#include "../lib/xpdf/Page.h"

#include "../../DesktopEditor/graphics/GraphicsPath.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"

namespace PdfWriter
{
void Transform(double* pMatrix, double dUserX, double dUserY, double* pdDeviceX, double* pdDeviceY)
{
	*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
	*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
}
bool InvertMatrix(const double* matrix, double* inverse)
{
	double det = matrix[0] * matrix[3] - matrix[1] * matrix[2];
	if (fabs(det) < 1e-10)
		return false;

	double invDet = 1.0 / det;
	inverse[0] =  matrix[3] * invDet;
	inverse[1] = -matrix[1] * invDet;
	inverse[2] = -matrix[2] * invDet;
	inverse[3] =  matrix[0] * invDet;
	inverse[4] = (matrix[2] * matrix[5] - matrix[3] * matrix[4]) * invDet;
	inverse[5] = (matrix[1] * matrix[4] - matrix[0] * matrix[5]) * invDet;

	return true;
}
std::vector<CPoint> GetImagePolygon(double* pMatrix)
{
	double dX1 = 0, dY1 = 0, dX2 = 0, dY2 = 1, dX3 = 1, dY3 = 1, dX4 = 1, dY4 = 0;
	Transform(pMatrix, dX1, dY1, &dX1, &dY1);
	Transform(pMatrix, dX2, dY2, &dX2, &dY2);
	Transform(pMatrix, dX3, dY3, &dX3, &dY3);
	Transform(pMatrix, dX4, dY4, &dX4, &dY4);
	return { CPoint(dX1, dY1), CPoint(dX2, dY2), CPoint(dX3, dY3), CPoint(dX4, dY4) };
}
bool CheckImageRedact(const std::vector<double>& arrQuadPoints, const std::vector<CPoint>& imagePolygon)
{
	for (int j = 0; j < arrQuadPoints.size(); j += 8)
	{
		std::vector<CPoint> redactPolygon =
		{
			CPoint(arrQuadPoints[j + 0], arrQuadPoints[j + 1]),
			CPoint(arrQuadPoints[j + 2], arrQuadPoints[j + 3]),
			CPoint(arrQuadPoints[j + 4], arrQuadPoints[j + 5]),
			CPoint(arrQuadPoints[j + 6], arrQuadPoints[j + 7])
		};

		if (PdfWriter::SAT(redactPolygon, imagePolygon))
			return true;
	}
	return false;
}
BYTE* DecodeImageToRGBA(Stream* pStream, int nWidth, int nHeight, GfxImageColorMap* pColorMap, GfxState* pGState)
{
	BYTE* pBufferPtr = new(std::nothrow) BYTE[4 * nWidth * nHeight];
	if (!pBufferPtr)
		return nullptr;

	int nComponentsCount = pColorMap->getNumPixelComps();
	ImageStream* pImageStream = new ImageStream(pStream, nWidth, nComponentsCount, pColorMap->getBits());
	pImageStream->reset();

	int nComps = pImageStream->getComps();
	int nCheckWidth = std::min(nWidth, pImageStream->getVals() / nComps);
	GfxRenderingIntent intent = pGState->getRenderingIntent();

	int nColorMapType = pColorMap->getFillType();
	GfxColorComp** pColorMapLookup = pColorMap->getLookup();
	if (!pColorMapLookup)
		nColorMapType = 0;

	for (int nY = 0; nY < nHeight; ++nY)
	{
		BYTE* pLine = pImageStream->getLine();
		BYTE* pLineDst = pBufferPtr + 4 * nWidth * nY;

		if (!pLine) {
			memset(pLineDst, 0, 4 * nWidth);
			continue;
		}

		for (int nX = 0; nX < nCheckWidth; ++nX)
		{
			if (2 == nColorMapType)
			{
				pLineDst[0] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
				pLineDst[1] = colToByte(clip01(pColorMapLookup[1][pLine[1]]));
				pLineDst[2] = colToByte(clip01(pColorMapLookup[2][pLine[2]]));
				pLineDst[3] = 255;
			}
			else if (1 == nColorMapType)
			{
				pLineDst[0] = pLineDst[1] = pLineDst[2] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
				pLineDst[3] = 255;
			}
			else if (3 == nColorMapType)
			{
				pLineDst[0] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
				pLineDst[1] = colToByte(clip01(pColorMapLookup[1][pLine[1]]));
				pLineDst[2] = colToByte(clip01(pColorMapLookup[2][pLine[2]]));
				pLineDst[3] = colToByte(clip01(pColorMapLookup[3][pLine[3]]));
			}
			else
			{
				GfxRGB oRGB;
				pColorMap->getRGB(pLine, &oRGB, intent);
				pLineDst[0] = colToByte(oRGB.r);
				pLineDst[1] = colToByte(oRGB.g);
				pLineDst[2] = colToByte(oRGB.b);
				pLineDst[3] = 255;
			}

			pLine += nComps;
			pLineDst += 4;
		}
	}

	pImageStream->close();
	delete pImageStream;
	return pBufferPtr;
}
BYTE* DecodeImageToGray(Stream* pStream, int nWidth, int nHeight, GfxImageColorMap* pColorMap, GfxState* pGState)
{
	BYTE* pBufferPtr = new(std::nothrow) BYTE[nWidth * nHeight];
	if (!pBufferPtr)
		return nullptr;

	int nComponentsCount = pColorMap->getNumPixelComps();
	ImageStream* pImageStream = new ImageStream(pStream, nWidth, nComponentsCount, pColorMap->getBits());
	pImageStream->reset();

	int nComps = pImageStream->getComps();
	int nCheckWidth = std::min(nWidth, pImageStream->getVals() / nComps);
	GfxRenderingIntent intent = pGState->getRenderingIntent();

	int nColorMapType = pColorMap->getFillType();
	GfxColorComp** pColorMapLookup = pColorMap->getLookup();
	if (!pColorMapLookup)
		nColorMapType = 0;

	for (int nY = 0; nY < nHeight; ++nY)
	{
		BYTE* pLine = pImageStream->getLine();
		BYTE* pLineDst = pBufferPtr + nWidth * nY;

		if (!pLine) {
			memset(pLineDst, 0, nWidth);
			continue;
		}

		for (int nX = 0; nX < nCheckWidth; ++nX)
		{
			if (1 == nColorMapType)
				pLineDst[nX] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
			else if (2 == nColorMapType || 3 == nColorMapType)
			{
				pLineDst[nX] = colToByte(clip01(
							   0.3  * pColorMapLookup[0][pLine[0]] +
							   0.59 * pColorMapLookup[1][pLine[1]] +
							   0.11 * pColorMapLookup[2][pLine[2]] + 0.5));
			}
			else
			{
				GfxGray oGray;
				pColorMap->getGray(pLine, &oGray, intent);
				pLineDst[nX] = colToByte(oGray);
			}

			pLine += nComps;
		}
	}

	pImageStream->close();
	delete pImageStream;
	return pBufferPtr;
}
BYTE* DecodeImageMaskToBits(Stream* pStream, int nWidth, int nHeight, GBool bInvert)
{
	BYTE* pBits = new BYTE[nWidth * nHeight];
	if (!pBits)
		return nullptr;

	ImageStream* pImageStream = new ImageStream(pStream, nWidth, 1, 1);
	pImageStream->reset();

	for (int y = 0; y < nHeight; ++y)
	{
		BYTE* pLine = pImageStream->getLine();
		if (!pLine)
		{
			memset(pBits + y * nWidth, bInvert ? 1 : 0, nWidth);
			continue;
		}

		for (int x = 0; x < nWidth; ++x)
		{
			BYTE pixel = pLine[x];
			if (bInvert)
				pixel = pixel ? 0 : 1;
			pBits[y * nWidth + x] = pixel;
		}
	}

	pImageStream->close();
	delete pImageStream;
	return pBits;
}
void SaveRGBAToStream(CDocument* pDocument, CDictObject* pDictObj, BYTE* pRGBA, int nWidth, int nHeight)
{
	CBgraFrame oFrame;
	oFrame.put_Data(pRGBA);
	oFrame.put_Width(nWidth);
	oFrame.put_Height(nHeight);
	oFrame.put_Stride(4 * nWidth);
	oFrame.put_IsRGBA(true);

	oFrame.SetJpegQuality(85.0);
	BYTE* pBuffer = NULL;
	int nBufferSize = 0;
	if (!oFrame.Encode(pBuffer, nBufferSize, _CXIMAGE_FORMAT_JPG))
		return;

	pDictObj->ClearStream();
	pDictObj->SetFilter(STREAM_FILTER_DCT_DECODE);
	CObjectBase* pLength = pDictObj->Get("Length");
	if (!pLength->IsIndirect())
	{
		pLength = new CNumberObject(0);
		pDocument->AddObject(pLength);
		pDictObj->Add("Length", pLength);
	}
	pDictObj->Add("Type", "XObject");
	pDictObj->Add("Subtype", "Image");
	pDictObj->Add("ColorSpace", "DeviceRGB");
	pDictObj->Add("Width", nWidth);
	pDictObj->Add("Height", nHeight);
	pDictObj->Add("BitsPerComponent", 8);

	pDictObj->Remove("DecodeParms");
	pDictObj->Remove("Decode");

	CStream* pStreamForm = pDictObj->GetStream();
	if (!pStreamForm)
	{
		pStreamForm = new PdfWriter::CMemoryStream(nBufferSize);
		pDictObj->SetStream(pStreamForm);
	}
	pStreamForm->Write(pBuffer, nBufferSize);

	delete[] pBuffer;
}
void SaveGrayToSMask(CDocument* pDocument, CDictObject* pDictObj, BYTE* pMaskGray, int nWidth, int nHeight)
{
	CObjectBase* pObj = pDictObj->Get("SMask");
	if (!pObj || pObj->GetType() != object_type_DICT)
	{
		pObj = new CDictObject();
		pDocument->AddObject(pObj);
		pDictObj->Add("SMask", pObj);
	}
	CDictObject* pSMask = (CDictObject*)pObj;

	pSMask->ClearStream();
	pSMask->SetFilter(STREAM_FILTER_FLATE_DECODE);
	CObjectBase* pLength = pDictObj->Get("Length");
	if (!pLength->IsIndirect())
	{
		pLength = new CNumberObject(0);
		pDocument->AddObject(pLength);
		pDictObj->Add("Length", pLength);
	}
	pSMask->Add("Type", "XObject");
	pSMask->Add("Subtype", "Image");
	pSMask->Add("ColorSpace", "DeviceGray");
	pSMask->Add("Width", nWidth);
	pSMask->Add("Height", nHeight);
	pSMask->Add("BitsPerComponent", 8);

	pSMask->Remove("DecodeParms");
	pDictObj->Remove("Decode");

	int nBufferSize = nWidth * nHeight;
	CStream* pStreamForm = pSMask->GetStream();
	if (!pStreamForm)
	{
		pStreamForm = new PdfWriter::CMemoryStream(nBufferSize);
		pDictObj->SetStream(pStreamForm);
	}
	pStreamForm->Write(pMaskGray, nBufferSize);
}
void SaveImageMaskToStream(CDocument* pDocument, CDictObject* pDictObj, BYTE* pMaskBits, int nWidth, int nHeight)
{
	pDictObj->ClearStream();
	pDictObj->SetFilter(STREAM_FILTER_FLATE_DECODE);
	CObjectBase* pLength = pDictObj->Get("Length");
	if (!pLength->IsIndirect())
	{
		pLength = new CNumberObject(0);
		pDocument->AddObject(pLength);
		pDictObj->Add("Length", pLength);
	}
	pDictObj->Add("Type", "XObject");
	pDictObj->Add("Subtype", "Image");
	pDictObj->Add("ImageMask", true);
	pDictObj->Add("Width", nWidth);
	pDictObj->Add("Height", nHeight);
	pDictObj->Add("BitsPerComponent", 1);

	pDictObj->Remove("DecodeParms");
	pDictObj->Remove("ColorSpace");

	int nRowSize = (nWidth + 7) / 8;
	int nTotalSize = nRowSize * nHeight;
	BYTE* pPackedBits = new BYTE[nTotalSize];
	memset(pPackedBits, 0, nTotalSize);

	for (int y = 0; y < nHeight; ++y)
	{
		for (int x = 0; x < nWidth; ++x)
		{
			if (pMaskBits[y * nWidth + x])
			{
				int byteIndex = y * nRowSize + (x / 8);
				int bitIndex = 7 - (x % 8);
				pPackedBits[byteIndex] |= (1 << bitIndex);
			}
		}
	}

	CStream* pStream = pDictObj->GetStream();
	if (!pStream)
	{
		pStream = new PdfWriter::CMemoryStream(nTotalSize);
		pDictObj->SetStream(pStream);
	}
	pStream->Write(pPackedBits, nTotalSize);

	delete[] pPackedBits;
}
void ApplyRedactToRGBA(const std::vector<double>& arrQuadPoints, BYTE* pImage, int nWidth, int nHeight, const std::vector<CPoint>& imagePolygon)
{
	// Находим все области редоктирования, которые пересекаются с изображением
	std::vector<std::vector<CPoint>> imageSpaceRedacts;

	for (int j = 0; j < arrQuadPoints.size(); j += 8)
	{
		std::vector<CPoint> redactPolygon =
		{
			CPoint(arrQuadPoints[j + 0], arrQuadPoints[j + 1]),
			CPoint(arrQuadPoints[j + 2], arrQuadPoints[j + 3]),
			CPoint(arrQuadPoints[j + 4], arrQuadPoints[j + 5]),
			CPoint(arrQuadPoints[j + 6], arrQuadPoints[j + 7])
		};

		// Преобразуем в координаты изображения
		std::vector<CPoint> imageSpaceRedact;
		for (const CPoint& point : redactPolygon)
		{
			double x = (point.x - imagePolygon[0].x) / (imagePolygon[2].x - imagePolygon[0].x) * nWidth;
			double y = (point.y - imagePolygon[0].y) / (imagePolygon[2].y - imagePolygon[0].y) * nHeight;
			imageSpaceRedact.push_back(CPoint(x, y));
		}

		// Проверяем, что область хоть частично внутри изображения
		if (PdfWriter::SAT(imageSpaceRedact, {CPoint(0, 0), CPoint(0, nHeight), CPoint(nWidth, nHeight), CPoint(nWidth, 0)}))
			imageSpaceRedacts.push_back(imageSpaceRedact);
	}

	// Закрашиваем области редоктирования черным цветом
	for (const auto& redact : imageSpaceRedacts)
	{
		// Находим bounding box области редоктирования в координатах изображения
		double minX = nWidth, minY = nHeight, maxX = 0, maxY = 0;
		for (const CPoint& p : redact)
		{
			if (p.x < minX) minX = p.x;
			if (p.y < minY) minY = p.y;
			if (p.x > maxX) maxX = p.x;
			if (p.y > maxY) maxY = p.y;
		}

		// Ограничиваем bounding box размерами изображения
		int startX = std::max(0, (int)minX);
		int startY = std::max(0, (int)minY);
		int endX = std::min(nWidth  - 1, (int)maxX);
		int endY = std::min(nHeight - 1, (int)maxY);

		// Закрашиваем прямоугольную область
		for (int y = startY; y <= endY; ++y)
		{
			for (int x = startX; x <= endX; ++x)
			{
				// Проверяем, что пиксель внутри полигона редоктирования
				if (PdfWriter::isPointInQuad(x, y, redact[0].x, redact[0].y, redact[1].x, redact[1].y, redact[2].x, redact[2].y, redact[3].x, redact[3].y))
				{
					int nIndex = ((nHeight - 1 - y) * nWidth + x) * 4;
					pImage[nIndex + 0] = 0; // R
					pImage[nIndex + 1] = 0; // G
					pImage[nIndex + 2] = 0; // B
					// Alpha оставляем 255
				}
			}
		}
	}
}
void ApplyRedactToGray(const std::vector<double>& arrQuadPoints, BYTE* pImage, int nWidth, int nHeight, const std::vector<CPoint>& imagePolygon)
{
	// Преобразуем области редоктирования в координаты маски
	std::vector<std::vector<CPoint>> maskSpaceRedacts;

	for (int j = 0; j < arrQuadPoints.size(); j += 8)
	{
		std::vector<CPoint> redactPolygon =
		{
			CPoint(arrQuadPoints[j + 0], arrQuadPoints[j + 1]),
			CPoint(arrQuadPoints[j + 2], arrQuadPoints[j + 3]),
			CPoint(arrQuadPoints[j + 4], arrQuadPoints[j + 5]),
			CPoint(arrQuadPoints[j + 6], arrQuadPoints[j + 7])
		};

		// Преобразуем в координаты маски
		std::vector<CPoint> maskSpaceRedact;
		for (const CPoint& point : redactPolygon)
		{
			double x = (point.x - imagePolygon[0].x) / (imagePolygon[2].x - imagePolygon[0].x) * nWidth;
			double y = (point.y - imagePolygon[0].y) / (imagePolygon[2].y - imagePolygon[0].y) * nHeight;
			maskSpaceRedact.push_back(CPoint(x, y));
		}

		if (PdfWriter::SAT(maskSpaceRedact, {CPoint(0, 0), CPoint(0, nHeight), CPoint(nWidth, nHeight), CPoint(nWidth, 0)}))
			maskSpaceRedacts.push_back(maskSpaceRedact);
	}

	for (const auto& redact : maskSpaceRedacts)
	{
		// Находим bounding box
		double minX = nWidth, minY = nHeight, maxX = 0, maxY = 0;
		for (const CPoint& p : redact)
		{
			if (p.x < minX) minX = p.x;
			if (p.y < minY) minY = p.y;
			if (p.x > maxX) maxX = p.x;
			if (p.y > maxY) maxY = p.y;
		}

		int startX = std::max(0, (int)minX);
		int startY = std::max(0, (int)minY);
		int endX = std::min(nWidth  - 1, (int)maxX);
		int endY = std::min(nHeight - 1, (int)maxY);

		for (int y = startY; y <= endY; ++y)
			for (int x = startX; x <= endX; ++x)
				if (PdfWriter::isPointInQuad(x, y, redact[0].x, redact[0].y, redact[1].x, redact[1].y, redact[2].x, redact[2].y, redact[3].x, redact[3].y))
					pImage[(nHeight - 1 - y) * nWidth + x] = 1;
	}
}

//----- constructor/destructor
RedactOutputDev::RedactOutputDev(CPdfWriter* pRenderer, CObjectsManager* pObjMng)
{
	m_pXref = NULL;

	m_pRenderer = pRenderer;
	m_mObjManager = pObjMng;
	m_pDoc = m_pRenderer->GetDocument();
	m_pPage = NULL;

	m_bUpdateAll = false;
}
RedactOutputDev::~RedactOutputDev()
{
	m_pRenderer = NULL;
	m_pDoc = NULL;
	m_pPage = NULL;
}
void RedactOutputDev::SetRedact(const std::vector<double>& arrQuadPoints)
{
	m_arrQuadPoints = arrQuadPoints;
	for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
	{
		m_oPathRedact.StartFigure();
		m_oPathRedact.MoveTo(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]);
		m_oPathRedact.LineTo(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]);
		m_oPathRedact.LineTo(m_arrQuadPoints[i + 4], m_arrQuadPoints[i + 5]);
		m_oPathRedact.LineTo(m_arrQuadPoints[i + 6], m_arrQuadPoints[i + 7]);
		m_oPathRedact.CloseFigure();
	}
}
void RedactOutputDev::NewPDF(XRef* pXref)
{
	m_pXref = pXref;
}
//----- initialization and control
void RedactOutputDev::setDefaultCTM(double *ctm)
{
	memcpy(m_arrMatrix, ctm, 6 * sizeof(double));
	if (m_pPage)
		m_pPage->StartTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
}
void RedactOutputDev::startPage(int nPageIndex, GfxState *pGState)
{
	m_pPage = m_pDoc->GetEditPage(nPageIndex);
	m_pRenderer->EditPage(m_pPage);
	m_pDoc->SetCurPage(m_pPage);
	m_pDoc->ClearPageFull();
}
void RedactOutputDev::endPage()
{
	m_pRenderer->m_oCommandManager.Flush();
	m_pPage = NULL;
}
//----- save/restore graphics state
void RedactOutputDev::saveState(GfxState *pGState)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();
	m_pPage->GrSave();
	m_sStates.push_back(GfxRedactState());
	updateAll(pGState);
}
void RedactOutputDev::restoreState(GfxState *pGState)
{
	m_pRenderer->m_oCommandManager.Flush();
	m_pPage->GrRestore();
	updateAll(pGState);

	if (m_sStates.empty())
		return; // Несбалансированный q/Q - сломанный файл
	m_sStates.pop_back();
}
//----- update graphics state
void RedactOutputDev::updateAll(GfxState *pGState)
{
	m_bUpdateAll = true;
	updateLineDash(pGState);
	updateFlatness(pGState);
	updateLineJoin(pGState);
	updateLineCap(pGState);
	updateMiterLimit(pGState);
	updateLineWidth(pGState);
	// updateStrokeAdjust(pGState);
	// updateFillColorSpace(pGState);
	// updateFillColor(pGState);
	// updateStrokeColorSpace(pGState);
	// updateStrokeColor(pGState);
	updateRenderingIntent(pGState);
	// updateBlendMode(pGState);
	// updateFillOpacity(pGState);
	// updateStrokeOpacity(pGState);
	updateFont(pGState);
	updateCharSpace(pGState);
	updateRender(pGState);
	updateRise(pGState);
	updateWordSpace(pGState);
	updateHorizScaling(pGState);
	m_bUpdateAll = false;
}
void RedactOutputDev::updateLineDash(GfxState *pGState)
{
	double* pDash  = NULL;
	int     nSize  = 0;
	double  dStart = 0;
	pGState->getLineDash(&pDash, &nSize, &dStart);

	if (0 == nSize) // Solid
	{
		m_pRenderer->put_PenDashOffset(0);
		m_pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
	}
	else
	{
		m_pRenderer->m_oPen.SetDashPattern(pDash, nSize);
		m_pRenderer->m_oPen.SetDashOffset(dStart);
		m_pRenderer->m_oPen.SetDashStyle(Aggplus::DashStyleCustom);
	}

	if (m_bUpdateAll || m_sStates.empty())
		return;
	std::string sOp = "[ ";
	for (int i = 0; i < nSize; ++i)
		sOp += (std::to_string(pDash[i]) + " ");
	sOp += "] " + std::to_string(dStart);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "d"));
}
void RedactOutputDev::updateFlatness(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetFlatness(pGState->getFlatness());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getFlatness()), "i"));
}
void RedactOutputDev::updateLineJoin(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetJoinStyle(pGState->getLineJoin());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getLineJoin()), "j"));
}
void RedactOutputDev::updateLineCap(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetStartCapStyle(pGState->getLineCap());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getLineCap()), "J"));
}
void RedactOutputDev::updateMiterLimit(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetMiter(pGState->getMiterLimit());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getMiterLimit()), "M"));
}
void RedactOutputDev::updateLineWidth(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetSize(pGState->getLineWidth());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getLineWidth()), "w"));
}
void RedactOutputDev::updateFillColor(GfxState *pGState)
{
	if (m_sStates.empty())
		return;
	GfxColorSpace* pColorSpace = pGState->getFillColorSpace();
	GfxColorSpaceMode eMode = pColorSpace->getMode();
	GfxColor* pColor = pGState->getFillColor();
	std::string sOp;
	std::string sOp2;
	switch (eMode)
	{
	case csDeviceGray:
	{
		sOp = std::to_string(colToDbl(pColor->c[0]));
		sOp2 = "g";
		break;
	}
	case csDeviceRGB:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2]));
		sOp2 = "rg";
		break;
	}
	case csDeviceCMYK:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2])) + " " + std::to_string(colToDbl(pColor->c[3]));
		sOp2 = "k";
		break;
	}
	default:
		break;
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, sOp2));
}
void RedactOutputDev::updateStrokeColor(GfxState *pGState)
{
	if (m_sStates.empty())
		return;
	GfxColorSpace* pColorSpace = pGState->getStrokeColorSpace();
	GfxColorSpaceMode eMode = pColorSpace->getMode();
	GfxColor* pColor = pGState->getStrokeColor();
	std::string sOp;
	std::string sOp2;
	switch (eMode)
	{
	case csDeviceGray:
	{
		sOp = std::to_string(colToDbl(pColor->c[0]));
		sOp2 = "G";
		break;
	}
	case csDeviceRGB:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2]));
		sOp2 = "RG";
		break;
	}
	case csDeviceCMYK:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2])) + " " + std::to_string(colToDbl(pColor->c[3]));
		sOp2 = "K";
		break;
	}
	default:
		break;
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, sOp2));
}
void RedactOutputDev::updateRenderingIntent(GfxState *pGState)
{
	if (m_bUpdateAll || m_sStates.empty())
		return;
	std::string sOp = "/";

	GfxRenderingIntent eRI = pGState->getRenderingIntent();
	switch (eRI)
	{
	case GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric:
		sOp += "AbsoluteColorimetric";
		break;
	case GfxRenderingIntent::gfxRenderingIntentRelativeColorimetric:
		sOp += "RelativeColorimetric";
		break;
	case GfxRenderingIntent::gfxRenderingIntentSaturation:
		sOp += "Saturation";
		break;
	case GfxRenderingIntent::gfxRenderingIntentPerceptual:
	default:
		sOp += "Perceptual";
		break;
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "ri"));
}
//----- update text state
void RedactOutputDev::updateFont(GfxState *pGState)
{
	GfxFont* pFont = pGState->getFont();
	if (pFont)
	{
		m_pRenderer->m_oFont.SetSize(pGState->getFontSize());
		m_pRenderer->put_FontName(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pFont->getTag()->getCString(), pFont->getTag()->getLength()));
	}
	else
		m_pRenderer->put_FontName(L"");
}
void RedactOutputDev::updateCharSpace(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetCharSpace(pGState->getCharSpace());
}
void RedactOutputDev::updateRender(GfxState *pGState)
{
	int nRender = pGState->getRender();
	m_pRenderer->m_oFont.SetRenderMode(nRender);
}
void RedactOutputDev::updateRise(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetRise(pGState->getRise());
}
void RedactOutputDev::updateWordSpace(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetWordSpace(pGState->getWordSpace());
}
void RedactOutputDev::updateHorizScaling(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetHorizontalScaling(pGState->getHorizScaling() * 100);
}
//----- path painting
void RedactOutputDev::stroke(GfxState *pGState)
{
	DoPathRedact(pGState, pGState->getPath(), pGState->getCTM(), true);
	DrawPath(c_nStroke);
}
void RedactOutputDev::fill(GfxState *pGState)
{
	DoPathRedact(pGState, pGState->getPath(), pGState->getCTM());
	DrawPath(c_nWindingFillMode);
}
void RedactOutputDev::eoFill(GfxState *pGState)
{
	DoPathRedact(pGState, pGState->getPath(), pGState->getCTM(), false, true);
	DrawPath(c_nEvenOddFillMode);
}
void RedactOutputDev::tilingPatternFill(GfxState *pGState, Gfx *gfx, Object *pStream, int nPaintType, int nTilingType, Dict *pResourcesDict,
										double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep)
{
}
//----- path clipping
void RedactOutputDev::clip(GfxState *pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeWinding;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
	AddClip(pGState, &m_sStates.back(), m_sStates.back().m_pClip->GetPathNum() - 1);
}
void RedactOutputDev::eoClip(GfxState *pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeEvenOdd;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
	AddClip(pGState, &m_sStates.back(), m_sStates.back().m_pClip->GetPathNum() - 1);
}
void RedactOutputDev::clipToStrokePath(GfxState *pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeWinding | c_nClipToStrokePath;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
	AddClip(pGState, &m_sStates.back(), m_sStates.back().m_pClip->GetPathNum() - 1);
}
//----- text drawing
void RedactOutputDev::beginStringOp(GfxState *pGState)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();
}
void RedactOutputDev::drawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY,
							   CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen)
{
	double* pCTM = pGState->getCTM();
	double* pTm = pGState->getTextMat();
	double pNewTm[6], arrMatrix[6];

	double dTextScale = std::min(sqrt(pTm[2] * pTm[2] + pTm[3] * pTm[3]), sqrt(pTm[0] * pTm[0] + pTm[1] * pTm[1]));
	double dITextScale = 1 / dTextScale;
	double dOldSize = 10.0;
	m_pRenderer->get_FontSize(&dOldSize);
	if (dOldSize * dTextScale > 0)
	{
		m_pRenderer->put_FontSize(dOldSize * dTextScale);

		pNewTm[0] =  pTm[0] * dITextScale * pGState->getHorizScaling();
		pNewTm[1] =  pTm[1] * dITextScale * pGState->getHorizScaling();
		pNewTm[2] =  pTm[2] * dITextScale;
		pNewTm[3] =  pTm[3] * dITextScale;
		pNewTm[4] =  dX - dOriginX;
		pNewTm[5] =  dY - dOriginY;
	}
	else
	{
		m_pRenderer->put_FontSize(-dOldSize * dTextScale);

		pNewTm[0] = pTm[0] * dITextScale * pGState->getHorizScaling();
		pNewTm[1] = pTm[1] * dITextScale * pGState->getHorizScaling();
		pNewTm[2] = pTm[2] * dITextScale;
		pNewTm[3] = pTm[3] * dITextScale;
		pNewTm[4] = dX;
		pNewTm[5] = dY;
	}

	arrMatrix[0] = pNewTm[0] * pCTM[0] + pNewTm[1] * pCTM[2];
	arrMatrix[1] = pNewTm[0] * pCTM[1] + pNewTm[1] * pCTM[3];
	arrMatrix[2] = pNewTm[2] * pCTM[0] + pNewTm[3] * pCTM[2];
	arrMatrix[3] = pNewTm[2] * pCTM[1] + pNewTm[3] * pCTM[3];
	arrMatrix[4] = pNewTm[4] * pCTM[0] + pNewTm[5] * pCTM[2] + pCTM[4];
	arrMatrix[5] = pNewTm[4] * pCTM[1] + pNewTm[5] * pCTM[3] + pCTM[5];

	double dSize = 1;
	if (true)
	{
		double dNorma = std::min(sqrt(arrMatrix[0] * arrMatrix[0] + arrMatrix[1] * arrMatrix[1]), sqrt(arrMatrix[2] * arrMatrix[2] + arrMatrix[3] * arrMatrix[3]));
		if (dNorma > 0 && dNorma != 1)
		{
			arrMatrix[0] /= dNorma;
			arrMatrix[1] /= dNorma;
			arrMatrix[2] /= dNorma;
			arrMatrix[3] /= dNorma;

			m_pRenderer->get_FontSize(&dSize);
			dSize *= dNorma;
			m_pRenderer->put_FontSize(dSize);
		}
	}

	double dShiftX = 0, dShiftY = 0;
	DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

	double startX, startY, endX, endY;
	pGState->transform(dX, dY, &startX, &startY);
	pGState->transform(dX + dDx, dY + dDy, &endX, &endY);
	double dCenterX = (startX + endX) / 2;
	double dCenterY = (startY + endY) / 2;
	for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
	{
		double x1 = m_arrQuadPoints[i + 0];
		double y1 = m_arrQuadPoints[i + 1];
		double x2 = m_arrQuadPoints[i + 2];
		double y2 = m_arrQuadPoints[i + 3];
		double x3 = m_arrQuadPoints[i + 4];
		double y3 = m_arrQuadPoints[i + 5];
		double x4 = m_arrQuadPoints[i + 6];
		double y4 = m_arrQuadPoints[i + 7];

		if (isPointInQuad(dCenterX, dCenterY, x1, y1, x2, y2, x3, y3, x4, y4))
		{
			m_pRenderer->put_FontSize(dOldSize);
			return;
		}
	}

	BYTE* pCodes = new BYTE[2];
	pCodes[0] = (nCode >> 8) & 0xFF;
	pCodes[1] = nCode & 0xFF;

	m_pRenderer->m_oCommandManager.SetTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);

	CRendererTextCommand* pText = m_pRenderer->m_oCommandManager.AddText(pCodes, 2, dShiftX, dShiftY);
	pText->SetName(m_pRenderer->m_oFont.GetName());
	pText->SetSize(m_pRenderer->m_oFont.GetSize());
	pText->SetType((EFontType)pGState->getFont()->getType());
	pText->SetCharSpace(m_pRenderer->m_oFont.GetCharSpace());
	pText->SetMode(m_pRenderer->m_oFont.GetRenderMode());
	pText->SetRise(m_pRenderer->m_oFont.GetRise());
	pText->SetWordSpace(m_pRenderer->m_oFont.GetWordSpace());
	pText->SetHorScaling(m_pRenderer->m_oFont.GetHorizontalScaling());
	pText->SetWidth(dDx);

	m_pRenderer->put_FontSize(dOldSize);
}
//----- additional
GBool RedactOutputDev::useNameOp()
{
	return gTrue;
}
void RedactOutputDev::setExtGState(const char* name)
{
	if (m_sStates.empty())
		return;
	std::string sOp = "/" + std::string(name);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "gs"));
}
void RedactOutputDev::setFillColorSpace(const char* name)
{
	if (m_sStates.empty())
		return;
	std::string sOp = "/" + std::string(name);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "cs"));
}
void RedactOutputDev::setFillColor(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "sc"));
}
void RedactOutputDev::setFillColorN(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "scn"));
}
void RedactOutputDev::setStrokeColorSpace(const char* name)
{
	if (m_sStates.empty())
		return;
	std::string sOp = "/" + std::string(name);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "CS"));
}
void RedactOutputDev::setStrokeColor(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "SC"));
}
void RedactOutputDev::setStrokeColorN(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "SCN"));
}
void RedactOutputDev::setShading(GfxState *pGState, const char* name)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	// TODO Нужно проверять Shading на отсечение?

	m_pPage->GrSave();
	UpdateTransform();
	DoStateOp();
	CStream* pStream = m_pPage->GetStream();
	pStream->WriteEscapeName(name);
	pStream->WriteChar(' ');
	pStream->WriteStr("sh");
	pStream->WriteStr("\012");
	m_pPage->GrRestore();
}
//----- image drawing
void RedactOutputDev::drawImageMask(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	PdfWriter::CDocument* pDocument = m_pRenderer->GetDocument();
	PdfWriter::CObjectBase* pObj = NULL;
	if (!pRef || bInlineImage)
		pObj = CreateImage(gfx, nWidth, nHeight, STREAM_FILTER_FLATE_DECODE, 1, NULL);
	else
	{
		pObj = m_mObjManager->GetObj(pRef->getRefNum());

		CDictObject* pXObject = pDocument->GetXObject(pRef->getRefNum());
		if (pXObject)
		{
			DrawXObject(m_sImageName.c_str());
			return;
		}
	}

	if (!pObj || pObj->GetType() != object_type_DICT)
		return;

	std::vector<CPoint> imagePolygon = GetImagePolygon(m_arrMatrix);
	bool bHasRedact = CheckImageRedact(m_arrQuadPoints, imagePolygon);
	if (!bHasRedact && pRef && !bInlineImage)
	{
		DrawXObject(m_sImageName.c_str());
		return;
	}

	BYTE* pBufferPtr = DecodeImageMaskToBits(pStream, nWidth, nHeight, bInvert);
	if (!pBufferPtr)
		return;

	if (bHasRedact)
		ApplyRedactToGray(m_arrQuadPoints, pBufferPtr, nWidth, nHeight, imagePolygon);

	SaveImageMaskToStream(pDocument, (CDictObject*)pObj, pBufferPtr, nWidth, nHeight);
	delete[] pBufferPtr;

	DrawXObject(m_sImageName.c_str());
}
void RedactOutputDev::drawImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	PdfWriter::CDocument* pDocument = m_pRenderer->GetDocument();
	PdfWriter::CObjectBase* pObj = NULL;
	if (!pRef || bInlineImg)
		pObj = CreateImage(gfx, nWidth, nHeight, STREAM_FILTER_DCT_DECODE, 8, "DeviceRGB");
	else
	{
		pObj = m_mObjManager->GetObj(pRef->getRefNum());

		CDictObject* pXObject = pDocument->GetXObject(pRef->getRefNum());
		if (pXObject)
		{
			DrawXObject(m_sImageName.c_str());
			return;
		}
	}

	if (!pObj || pObj->GetType() != object_type_DICT)
		return;

	std::vector<CPoint> imagePolygon = GetImagePolygon(m_arrMatrix);
	bool bHasRedact = CheckImageRedact(m_arrQuadPoints, imagePolygon);
	if (!bHasRedact && pRef && !bInlineImg)
	{
		DrawXObject(m_sImageName.c_str());
		return;
	}

	BYTE* pBufferPtr = DecodeImageToRGBA(pStream, nWidth, nHeight, pColorMap, pGState);
	if (!pBufferPtr)
		return;

	if (bHasRedact)
		ApplyRedactToRGBA(m_arrQuadPoints, pBufferPtr, nWidth, nHeight, imagePolygon);

	SaveRGBAToStream(pDocument, (CDictObject*)pObj, pBufferPtr, nWidth, nHeight);

	DrawXObject(m_sImageName.c_str());
}
void RedactOutputDev::drawMaskedImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
							 Object* pMaskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate)
{

}
void RedactOutputDev::drawSoftMaskedImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
								 Object *maskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap *pMaskColorMap, double *pMatte, GBool interpolate)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	PdfWriter::CDocument* pDocument = m_pRenderer->GetDocument();
	PdfWriter::CObjectBase* pObj = NULL;
	if (!pRef)
		pObj = CreateImage(gfx, nWidth, nHeight, STREAM_FILTER_DCT_DECODE, 8, "DeviceRGB");
	else
	{
		pObj = m_mObjManager->GetObj(pRef->getRefNum());

		CDictObject* pXObject = pDocument->GetXObject(pRef->getRefNum());
		if (pXObject)
		{
			DrawXObject(m_sImageName.c_str());
			return;
		}
	}

	if (!pObj || pObj->GetType() != object_type_DICT)
		return;

	std::vector<CPoint> imagePolygon = GetImagePolygon(m_arrMatrix);
	if (!CheckImageRedact(m_arrQuadPoints, imagePolygon))
	{
		DrawXObject(m_sImageName.c_str());
		return;
	}

	BYTE* pBufferPtr = DecodeImageToRGBA(pStream, nWidth, nHeight, pColorMap, pGState);
	if (!pBufferPtr)
		return;

	ApplyRedactToRGBA(m_arrQuadPoints, pBufferPtr, nWidth, nHeight, imagePolygon);
	CDictObject* pDictObj = (CDictObject*)pObj;
	SaveRGBAToStream(pDocument, pDictObj, pBufferPtr, nWidth, nHeight);

	BYTE* pMaskBufferPtr = DecodeImageToGray(pMaskStream, nMaskWidth, nMaskHeight, pMaskColorMap, pGState);
	if (pMaskBufferPtr)
	{
		if (nWidth != nMaskWidth || nHeight != nMaskHeight)
		{
			// Простое масштабирование (ближайший сосед)
			BYTE* pDstMask = new BYTE[nWidth * nHeight];
			if (!pDstMask)
			{
				delete[] pMaskBufferPtr;
				return;
			}

			double dScaleX = (double)nMaskWidth  / nWidth;
			double dScaleY = (double)nMaskHeight / nHeight;

			for (int dstY = 0; dstY < nHeight; ++dstY)
			{
				for (int dstX = 0; dstX < nWidth; ++dstX)
				{
					int srcX = (int)(dstX * dScaleX);
					int srcY = (int)(dstY * dScaleY);

					// Ограничиваем координаты
					srcX = std::min(srcX, nMaskWidth  - 1);
					srcY = std::min(srcY, nMaskHeight - 1);

					pDstMask[dstY * nWidth + dstX] = pMaskBufferPtr[srcY * nMaskWidth + srcX];
				}
			}

			delete[] pMaskBufferPtr;
			pMaskBufferPtr = pDstMask;
		}

		ApplyRedactToGray(m_arrQuadPoints, pMaskBufferPtr, nWidth, nHeight, imagePolygon);
		SaveGrayToSMask(pDocument, pDictObj, pMaskBufferPtr, nWidth, nHeight);
		delete[] pMaskBufferPtr;
	}

	DrawXObject(m_sImageName.c_str());
}
//----- Type 3 font operators
void RedactOutputDev::type3D0(GfxState *pGState, double wx, double wy)
{

}
void RedactOutputDev::type3D1(GfxState *pGState, double wx, double wy, double llx, double lly, double urx, double ury)
{

}
//----- form XObjects
void RedactOutputDev::drawForm(GfxState *pGState, Gfx *gfx, Ref id, const char* name)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	PdfWriter::CObjectBase* pObj = m_mObjManager->GetObj(id.num);
	if (!pObj || pObj->GetType() != object_type_DICT)
		return;

	PdfWriter::CDocument* pDocument = m_pRenderer->GetDocument();
	CDictObject* pXObject = pDocument->GetXObject(id.num);
	if (pXObject)
	{
		DrawXObject(name);
		return;
	}

	Object oForm;
	if (!m_pXref->fetch(id.num, id.gen, &oForm)->isStream())
	{
		oForm.free();
		return;
	}

	double dXmin = 0, dYmin = 0, dXmax = 0, dYmax = 0;
	double dX1 = 0, dY1 = 0, dX2 = 0, dY2 = 1, dX3 = 1, dY3 = 1, dX4 = 1, dY4 = 0;
	Object oObj;
	if (oForm.streamGetDict()->lookup("BBox", &oObj)->isArray() && oObj.arrayGetLength() == 4)
	{
		Object oNum;
		if (oObj.arrayGet(0, &oNum)->isNum())
			dXmin = oNum.getNum();
		oNum.free();
		if (oObj.arrayGet(1, &oNum)->isNum())
			dYmin = oNum.getNum();
		oNum.free();
		if (oObj.arrayGet(2, &oNum)->isNum())
			dXmax = oNum.getNum();
		oNum.free();
		if (oObj.arrayGet(3, &oNum)->isNum())
			dYmax = oNum.getNum();
		oNum.free(); oObj.free();

		dX1 = dXmin, dY1 = dYmin;
		dX2 = dXmin, dY2 = dYmax;
		dX3 = dXmax, dY3 = dYmax;
		dX4 = dXmax, dY4 = dYmin;

		double oMatrix[6] = { 1, 0, 0, 1, 0, 0 };
		if (oForm.streamGetDict()->lookup("Matrix", &oObj)->isArray() && oObj.arrayGetLength() == 6)
		{
			Object oObj2;
			for (int i = 0; i < 6; ++i)
			{
				oMatrix[i] = oObj.arrayGet(i, &oObj2)->getNum();
				oObj2.free();
			}
		}

		Transform(oMatrix, dX1, dY1, &dX1, &dY1);
		Transform(oMatrix, dX2, dY2, &dX2, &dY2);
		Transform(oMatrix, dX3, dY3, &dX3, &dY3);
		Transform(oMatrix, dX4, dY4, &dX4, &dY4);

		Transform(m_arrMatrix, dX1, dY1, &dX1, &dY1);
		Transform(m_arrMatrix, dX2, dY2, &dX2, &dY2);
		Transform(m_arrMatrix, dX3, dY3, &dX3, &dY3);
		Transform(m_arrMatrix, dX4, dY4, &dX4, &dY4);
	}
	oObj.free();

	bool bFullyRedacted = false;
	std::vector<CPoint> formPolygon =
	{
		CPoint(dX1, dY1),
		CPoint(dX2, dY2),
		CPoint(dX3, dY3),
		CPoint(dX4, dY4)
	};
	for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
	{
		std::vector<CPoint> redactPolygon =
		{
			CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]),
			CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]),
			CPoint(m_arrQuadPoints[i + 4], m_arrQuadPoints[i + 5]),
			CPoint(m_arrQuadPoints[i + 6], m_arrQuadPoints[i + 7])
		};
		if (PdfWriter::isPolygonInsidePolygon(formPolygon, redactPolygon))
		{
			bFullyRedacted = true;
			break;
		}
	}

	if (bFullyRedacted)
	{
		oForm.free();
		return;
	}

	CDictObject* pDictObj = (CDictObject*)pObj;
	pDictObj->ClearStream();
	CNumberObject* pLength = new CNumberObject(0);
	pDocument->AddObject(pLength);
	pDictObj->Add("Length", pLength);
#ifndef FILTER_FLATE_DECODE_DISABLED
	pDictObj->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif

	PdfWriter::CPage* pCurPage = m_pRenderer->GetPage();
	PdfWriter::CPage* pFakePage = new PdfWriter::CPage(pDocument);
	m_pRenderer->SetPage(pFakePage);
	pDocument->SetCurPage(pFakePage);
	pFakePage->SetStream(pDictObj->GetStream());

	RedactOutputDev* pFormOutputDev = new RedactOutputDev(m_pRenderer, m_mObjManager);
	pFormOutputDev->NewPDF(m_pXref);
	pFormOutputDev->m_pPage = pFakePage;
	std::vector<double> arrFormQuadPoints;
	double dInvMatrix[6] = { 1, 0, 0, 1, 0, 0 };
	InvertMatrix(m_arrMatrix, dInvMatrix);
	for (int i = 0; i < m_arrQuadPoints.size(); i += 2)
	{
		double x = m_arrQuadPoints[i];
		double y = m_arrQuadPoints[i + 1];
		Transform(dInvMatrix, x, y, &x, &y);
		arrFormQuadPoints.push_back(x);
		arrFormQuadPoints.push_back(y);
	}
	pFormOutputDev->SetRedact(arrFormQuadPoints);

	Object resourcesObj;
	oForm.streamGetDict()->lookup("Resources", &resourcesObj);

	PDFRectangle pBBox = { 0, 0, dXmax - dXmin, dYmax - dYmin };
	Gfx* _gfx = new Gfx(gfx->getDoc(), pFormOutputDev, resourcesObj.isDict() ? resourcesObj.getDict() : NULL, &pBBox, NULL);
	_gfx->takeContentStreamStack(gfx);
	Object oFormRef;
	oFormRef.initRef(id.num, id.gen);
	_gfx->saveState();
	_gfx->display(&oFormRef);
	_gfx->endOfPage();
	oFormRef.free();

	RELEASEOBJECT(_gfx);
	RELEASEOBJECT(pFormOutputDev);
	resourcesObj.free(); oForm.free();

	m_pRenderer->SetPage(pCurPage);
	pDocument->SetCurPage(pCurPage);
	pDocument->AddXObject(id.num, pDictObj);

	RELEASEOBJECT(pFakePage);

	DrawXObject(name);
}
void RedactOutputDev::drawImage(GfxState *pGState, Gfx *gfx, Ref id, const char* name)
{
	m_sImageName = std::string(name);
}

bool SkipPath(const std::vector<CSegment>& arrForStroke, const CPoint& P1, const CPoint& P2)
{
	for (int i = 0; i < arrForStroke.size(); ++i)
	{
		CPoint P3 = arrForStroke[i].start;
		CPoint P4 = arrForStroke[i].end;
		// Вычисляем коэффициенты A, B, C для уравнения прямой P3P4: Ax + By + C = 0
		double A = P4.y - P3.y;
		double B = P3.x - P4.x;
		double C = P4.x * P3.y - P3.x * P4.y;

		// Проверяем, лежит ли точка P1 на прямой P3P4
		double check1 = A * P1.x + B * P1.y + C;

		// Проверяем, лежит ли точка P2 на прямой P3P4
		double check2 = A * P2.x + B * P2.y + C;

		// Если обе проверки близки к нулю (в пределах эпсилон), то лежит
		if ((std::abs(check1) < 0.006) && (std::abs(check2) < 0.006))
			return true;
	}
	return false;
}
void RedactOutputDev::DrawPathRedact(Aggplus::CGraphicsPath* oPath, bool bStroke, double& dXEnd, double& dYEnd, const std::vector<CSegment>& arrForStroke)
{
	CMatrix oMatrix(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
	CMatrix oInverse = oMatrix.Inverse();

	size_t length = oPath->GetPointCount(), compound = oPath->GetCloseCount();
	std::vector<Aggplus::PointD> points = oPath->GetPoints(0, length + compound);

	double dXStart = -1, dYStart = -1, dXCur = -1, dYCur = -1;
	bool bBreak = false;
	for (size_t i = 0; i < length + compound; i++)
	{
		if (oPath->IsCurvePoint(i))
		{
			double dX = points[i].X;
			double dY = points[i].Y;
			oInverse.Apply(dX, dY);
			double dX2 = points[i + 1].X;
			double dY2 = points[i + 1].Y;
			oInverse.Apply(dX2, dY2);
			double dX3 = points[i + 2].X;
			double dY3 = points[i + 2].Y;
			if (bBreak)
			{
				bBreak = false;
				double dXCI = dXCur, dYCI = dYCur;
				oInverse.Apply(dXCI, dYCI);
				if (dXEnd != dXCI || dYEnd != dYCI)
					m_pRenderer->m_oPath.MoveTo(dXCI, dYCI);
			}
			dXCur = dX3; dYCur = dY3;
			oInverse.Apply(dX3, dY3);
			m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
			dXEnd = dX3, dYEnd = dY3;

			i += 2;
		}
		else if (oPath->IsMovePoint(i))
		{
			double dX = points[i].X, dY = points[i].Y;
			dXStart = dX; dYStart = dY; dXCur = dX; dYCur = dY;
			if (bStroke)
				bBreak = true;
			else
			{
				oInverse.Apply(dX, dY);
				m_pRenderer->m_oPath.MoveTo(dX, dY);
			}
		}
		else if (oPath->IsLinePoint(i))
		{
			double dX = points[i].X, dY = points[i].Y;
			if (bStroke && SkipPath(arrForStroke, CPoint(dXCur, dYCur), CPoint(dX, dY)))
			{
				dXCur = dX; dYCur = dY;
				bBreak = true;
				continue;
			}
			if (bBreak)
			{
				bBreak = false;
				double dXCI = dXCur, dYCI = dYCur;
				oInverse.Apply(dXCI, dYCI);
				if (dXEnd != dXCI || dYEnd != dYCI)
					m_pRenderer->m_oPath.MoveTo(dXCI, dYCI);
			}
			dXCur = dX; dYCur = dY;
			oInverse.Apply(dX, dY);
			m_pRenderer->m_oPath.LineTo(dX, dY);
			dXEnd = dX, dYEnd = dY;
		}
		else if (oPath->IsClosePoint(i))
		{
			if (bStroke && (std::abs(dXCur - dXStart) > EPS || std::abs(dYCur - dYStart) > EPS) && SkipPath(arrForStroke, CPoint(dXCur, dYCur), CPoint(dXStart, dYStart)))
			{
				dXCur = dXStart; dYCur = dYStart;
				bBreak = true;
				continue;
			}
			if (bStroke || bBreak)
			{
				if (std::abs(dXCur - dXStart) > EPS || std::abs(dYCur - dYStart) > EPS)
				{
					bBreak = false;
					double dXCI = dXCur, dYCI = dYCur;
					oInverse.Apply(dXCI, dYCI);
					double dXSI = dXStart, dYSI = dYStart;
					oInverse.Apply(dXSI, dYSI);
					if (dXEnd != dXCI || dYEnd != dYCI)
						m_pRenderer->m_oPath.MoveTo(dXCI, dYCI);
					m_pRenderer->m_oPath.LineTo(dXSI, dYSI);
					dXEnd = dXSI, dYEnd = dYSI;
				}
			}
			else
				m_pRenderer->m_oPath.Close();
		}
	}
}
void RedactOutputDev::DoPathRedact(GfxState* pGState, GfxPath* pPath, double* pCTM, bool bStroke, bool bEoFill)
{
	double arrMatrix[6];
	arrMatrix[0] = pCTM[0];
	arrMatrix[1] = pCTM[1];
	arrMatrix[2] = pCTM[2];
	arrMatrix[3] = pCTM[3];
	arrMatrix[4] = pCTM[4];
	arrMatrix[5] = pCTM[5];

	double dShiftX = 0, dShiftY = 0;
	DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
	m_pRenderer->m_oPath.Clear();

	CMatrix oMatrix(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
	CMatrix oInverse = oMatrix.Inverse();

	std::vector<CSegment> arrForStroke;
	Aggplus::CGraphicsPath oPath, oPathResult;
	if (bEoFill)
		oPath.SetRuler(true);

	if (bStroke)
	{
		std::vector<std::vector<CPoint>> rectangles;
		for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
		{
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]), CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3])));
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]), CPoint(m_arrQuadPoints[i + 4], m_arrQuadPoints[i + 5])));
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 4], m_arrQuadPoints[i + 5]), CPoint(m_arrQuadPoints[i + 6], m_arrQuadPoints[i + 7])));
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 6], m_arrQuadPoints[i + 7]), CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1])));

			std::vector<CPoint> rectangle = { CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]), CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]),
											  CPoint(m_arrQuadPoints[i + 4], m_arrQuadPoints[i + 5]), CPoint(m_arrQuadPoints[i + 6], m_arrQuadPoints[i + 7]) };
			rectangles.push_back(rectangle);
		}

		for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
		{
			GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
			int nPointsCount = pSubpath->getNumPoints();

			double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
			oMatrix.Apply(dX, dY);
			double dXStart = dX, dYStart = dY, dXCur = dX, dYCur = dY, dXEnd = dX + 1, dYEnd = dY + 1;

			int nCurPointIndex = 1;
			while (nCurPointIndex < nPointsCount)
			{
				if (pSubpath->getCurve(nCurPointIndex))
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					double dX2 = pSubpath->getX(nCurPointIndex + 1);
					double dY2 = pSubpath->getY(nCurPointIndex + 1);
					oMatrix.Apply(dX2, dY2);
					double dX3 = pSubpath->getX(nCurPointIndex + 2);
					double dY3 = pSubpath->getY(nCurPointIndex + 2);
					oMatrix.Apply(dX3, dY3);
					nCurPointIndex += 3;

					oPath.StartFigure();
					oPath.MoveTo(dXCur, dYCur);
					oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
					oPath.CloseFigure();

					arrForStroke.push_back(CSegment(CPoint(dXCur, dYCur), CPoint(dX3, dY3)));
					dXCur = dX3, dYCur = dY3;

					oPathResult = Aggplus::CalcBooleanOperation(oPath, m_oPathRedact, Aggplus::BooleanOpType::Subtraction);
					DrawPathRedact(&oPathResult, bStroke, dXEnd, dYEnd, arrForStroke);
					oPathResult.Reset(); oPath.Reset();
				}
				else
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					++nCurPointIndex;

					CSegment line(CPoint(dXCur, dYCur), CPoint(dX, dY));
					dXCur = dX; dYCur = dY;
					auto visibleSegments = RectangleIntersection::findSegmentsOutsideRectangles(line, rectangles);

					for (int i = 0; i < visibleSegments.size(); ++i)
					{
						double dX1 = visibleSegments[i].start.x, dY1 = visibleSegments[i].start.y;
						double dX2 = visibleSegments[i].end.x,   dY2 = visibleSegments[i].end.y;
						oInverse.Apply(dX1, dY1);
						if (dXEnd != dX1 || dYEnd != dY1)
							m_pRenderer->m_oPath.MoveTo(dX1, dY1);
						oInverse.Apply(dX2, dY2);
						m_pRenderer->m_oPath.LineTo(dX2, dY2);
						dXEnd = dX2, dYEnd = dY2;
					}
				}
			}
			if (pSubpath->isClosed())
			{
				CSegment line(CPoint(dXCur, dYCur), CPoint(dXStart, dYStart));
				auto visibleSegments = RectangleIntersection::findSegmentsOutsideRectangles(line, rectangles);
				for (int i = 0; i < visibleSegments.size(); ++i)
				{
					double dX1 = visibleSegments[i].start.x, dY1 = visibleSegments[i].start.y;
					double dX2 = visibleSegments[i].end.x,   dY2 = visibleSegments[i].end.y;
					oInverse.Apply(dX1, dY1);
					m_pRenderer->m_oPath.MoveTo(dX1, dY1);
					oInverse.Apply(dX2, dY2);
					m_pRenderer->m_oPath.LineTo(dX2, dY2);
				}
			}
		}
	}
	else
	{
		for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
		{
			GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
			int nPointsCount = pSubpath->getNumPoints();

			oPath.StartFigure();

			double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
			oMatrix.Apply(dX, dY);
			oPath.MoveTo(dX, dY);

			int nCurPointIndex = 1;
			while (nCurPointIndex < nPointsCount)
			{
				if (pSubpath->getCurve(nCurPointIndex))
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					double dX2 = pSubpath->getX(nCurPointIndex + 1);
					double dY2 = pSubpath->getY(nCurPointIndex + 1);
					oMatrix.Apply(dX2, dY2);
					double dX3 = pSubpath->getX(nCurPointIndex + 2);
					double dY3 = pSubpath->getY(nCurPointIndex + 2);
					oMatrix.Apply(dX3, dY3);
					oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
					nCurPointIndex += 3;
				}
				else
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					oPath.LineTo(dX, dY);
					++nCurPointIndex;
				}
			}
			// if (pSubpath->isClosed()) Принудительное замыкание фигур для CGraphicsPath
				oPath.CloseFigure();
		}

		oPathResult = Aggplus::CalcBooleanOperation(oPath, m_oPathRedact, Aggplus::BooleanOpType::Subtraction);
		double dXEnd = -1, dYEnd = -1;
		DrawPathRedact(&oPathResult, bStroke, dXEnd, dYEnd);
	}
}
void RedactOutputDev::DoPath(GfxState* pGState, GfxPath* pPath, double* pCTM)
{
	double arrMatrix[6];
	arrMatrix[0] = pCTM[0];
	arrMatrix[1] = pCTM[1];
	arrMatrix[2] = pCTM[2];
	arrMatrix[3] = pCTM[3];
	arrMatrix[4] = pCTM[4];
	arrMatrix[5] = pCTM[5];

	double dShiftX = 0, dShiftY = 0;
	DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

	m_pRenderer->m_oPath.Clear();

	for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
	{
		GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
		int nPointsCount = pSubpath->getNumPoints();

		double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
		m_pRenderer->m_oPath.MoveTo(dX, dY);

		int nCurPointIndex = 1;
		while (nCurPointIndex < nPointsCount)
		{
			if (pSubpath->getCurve(nCurPointIndex))
			{
				dX = pSubpath->getX(nCurPointIndex);
				dY = pSubpath->getY(nCurPointIndex);
				double dX2 = pSubpath->getX(nCurPointIndex + 1);
				double dY2 = pSubpath->getY(nCurPointIndex + 1);
				double dX3 = pSubpath->getX(nCurPointIndex + 2);
				double dY3 = pSubpath->getY(nCurPointIndex + 2);
				m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
				nCurPointIndex += 3;
			}
			else
			{
				dX = pSubpath->getX(nCurPointIndex);
				dY = pSubpath->getY(nCurPointIndex);
				m_pRenderer->m_oPath.LineTo(dX, dY);
				++nCurPointIndex;
			}
		}
		if (pSubpath->isClosed())
			m_pRenderer->m_oPath.Close();
	}
}
void RedactOutputDev::DoTransform(double* pMatrix, double* pdShiftX, double* pdShiftY, bool bActual)
{
	if (1 == pMatrix[0] && 0 == pMatrix[1] && 0 == pMatrix[2] && 1 == pMatrix[3] && !bActual)
	{
		if (pMatrix[4] || pMatrix[5])
		{
			*pdShiftX = pMatrix[4];
			*pdShiftY = pMatrix[5];
		}
		m_pRenderer->ResetTransform();
		m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
		m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
		m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;
	}
	else if (m_arrMatrix[0] == pMatrix[0] && m_arrMatrix[1] == pMatrix[1] && m_arrMatrix[2] == pMatrix[2] && m_arrMatrix[3] == pMatrix[3]
			 && m_arrMatrix[4] == pMatrix[4] && m_arrMatrix[5] == pMatrix[5] && !bActual)
	{
		double dIDet = 1 / (pMatrix[0] * pMatrix[3] - pMatrix[1] * pMatrix[2]);
		*pdShiftX = ((pMatrix[4] - m_arrMatrix[4]) * m_arrMatrix[3] - (pMatrix[5] - m_arrMatrix[5]) * m_arrMatrix[1]) * dIDet;
		*pdShiftY = ((pMatrix[5] - m_arrMatrix[5]) * m_arrMatrix[0] - (pMatrix[4] - m_arrMatrix[4]) * m_arrMatrix[2]) * dIDet;
	}
	else
	{
		m_pRenderer->SetTransform(pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], pMatrix[4], pMatrix[5]);
		m_arrMatrix[0] = pMatrix[0]; m_arrMatrix[1] = pMatrix[1];
		m_arrMatrix[2] = pMatrix[2]; m_arrMatrix[3] = pMatrix[3];
		m_arrMatrix[4] = pMatrix[4]; m_arrMatrix[5] = pMatrix[5];
	}
}
void RedactOutputDev::DrawPath(const LONG& lType)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	if (!m_pRenderer->m_oPath.m_bIsMoveTo)
		return;

	bool bStroke = lType & c_nStroke;
	bool bFill   = lType & c_nWindingFillMode;
	bool bEoFill = lType & c_nEvenOddFillMode;

	m_pPage->GrSave();
	UpdateTransform();
	m_pRenderer->m_oPath.Draw(m_pPage, bStroke, bFill, bEoFill);
	m_pPage->GrRestore();
}
void RedactOutputDev::UpdateTransform()
{
	m_pPage->SetTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
}
void RedactOutputDev::AddClip(GfxState* pGState, GfxRedactState* pState, int nIndex)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	GfxClip* pClip = pState->m_pClip;
	GfxPath* pPath = pClip->GetPath(nIndex);
	int  nClipFlag = pClip->GetClipFlag(nIndex);;

	m_pRenderer->m_lClipMode = nClipFlag;
	GfxClipMatrix oMatrix = pClip->m_vMatrix[nIndex];
	double pMatrix[6] = { oMatrix.dA, oMatrix.dB, oMatrix.dC, oMatrix.dD, oMatrix.dE, oMatrix.dF };
	DoPath(pGState, pPath, pMatrix);
	UpdateTransform();
	m_pRenderer->m_oPath.Clip(m_pPage, c_nClipRegionTypeEvenOdd & m_pRenderer->m_lClipMode);
	m_pRenderer->m_oPath.Clear();
}
void RedactOutputDev::DoStateOp()
{
	if (m_sStates.empty() || m_sStates.back().m_arrOp.empty())
		return;

	CStream* pStream = m_pPage->GetStream();
	for (int i = 0; i < m_sStates.back().m_arrOp.size(); ++i)
	{
		auto arrOp = m_sStates.back().m_arrOp;
		pStream->WriteStr(arrOp[i].first.c_str());
		pStream->WriteChar(' ');
		pStream->WriteStr(arrOp[i].second.c_str());
		pStream->WriteStr("\012");
	}
	m_sStates.back().m_arrOp.clear();
}
void RedactOutputDev::DrawXObject(const char* name)
{
	m_pPage->GrSave();
	UpdateTransform();
	m_pPage->ExecuteXObject(name);
	m_pPage->GrRestore();
}
CObjectBase* RedactOutputDev::CreateImage(Gfx *gfx, int nWidth, int nHeight, unsigned int nFilter, int nBPC, const char* sCS)
{
	PdfWriter::CDocument* pDocument = m_pRenderer->GetDocument();
	PdfWriter::CObjectBase* pObj = pDocument->CreateImage();
	pDocument->AddObject(pObj);

	CDictObject* pDictObj = (CDictObject*)pObj;
	pDictObj->SetFilter(nFilter);
	CNumberObject* pLength = new CNumberObject(0);
	pDocument->AddObject(pLength);
	pDictObj->Add("Length", pLength);
	pDictObj->Add("Type", "XObject");
	pDictObj->Add("Subtype", "Image");
	pDictObj->Add("Width", nWidth);
	pDictObj->Add("Height", nHeight);
	pDictObj->Add("BitsPerComponent", nBPC);
	if (sCS)
		pDictObj->Add("ColorSpace", sCS);

	pDictObj->Remove("DecodeParms");

	PdfWriter::CResourcesDict* pResources = NULL;
	Object* pContent = gfx->getTopContentStreamStack();
	if (pContent && pContent->isRef())
	{
		PdfWriter::CObjectBase* pForm = m_mObjManager->GetObj(pContent->getRefNum());
		if (pForm && pForm->GetType() == object_type_DICT)
		{
			PdfWriter::CDictObject* pDictForm = (PdfWriter::CDictObject*)pForm;
			PdfWriter::CObjectBase* pResourcesForm = pDictForm->Get("Resources");
			if (pResourcesForm && pResourcesForm->GetType() == object_type_DICT)
				pResources = (PdfWriter::CResourcesDict*)pResourcesForm;
			else
			{
				pResources = new PdfWriter::CResourcesDict(NULL, true, false);
				pDictForm->Add("Resources", pResources);
			}
		}
	}
	else
	{
		pResources = m_pPage->GetResourcesItem();
		if (!pResources)
		{
			m_pPage->AddResource();
			pResources = m_pPage->GetResourcesItem();
		}
	}

	if (pResources)
	{
		const char* sXObjectName = pResources->GetXObjectName(pObj);
		m_sImageName = std::string(sXObjectName);
	}

	return pObj;
}
}

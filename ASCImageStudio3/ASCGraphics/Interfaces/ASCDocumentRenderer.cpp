#pragma once
#include "stdafx.h"
#include "ASCDocumentRenderer.h"
#include "ImageSerializeObjects2.h"

void CAVSDocumentRenderer::DrawWatermark(LONG lPageNumber, IUnknown* punkRenderer)
{
	if (NULL == punkRenderer)
		return;

	IAVSRenderer* pRenderer = NULL;
	punkRenderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRenderer);

	if (NULL == pRenderer)
		return;

	NSDocumentViewer::CDoubleRect oRect;
	oRect.left = 0;
	oRect.top = 0;

	pRenderer->get_Width(&oRect.right);
	pRenderer->get_Height(&oRect.bottom);

	m_oWatermark.DrawOnRenderer(pRenderer, oRect);

	RELEASEINTERFACE(pRenderer);
}
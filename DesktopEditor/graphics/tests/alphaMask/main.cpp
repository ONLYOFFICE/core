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

#include "../../pro/Graphics.h"
#include "../../../raster/BgraFrame.h"
#include "../../../common/Directory.h"
#include "../../AlphaMask.h"

int main(int argc, char *argv[])
{
	NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();

	unsigned int unWidth  = 1000;
	unsigned int unHeight = 1000;

	// Create main image
	BYTE* pData = new BYTE[4 * unWidth * unHeight];

	for (unsigned long unIndex = 0; unIndex < unWidth * unHeight; ++unIndex)
		((unsigned int*)pData)[unIndex] = 0xffffff;

	CBgraFrame oFrame;
	oFrame.put_Data(pData);
	oFrame.put_Width(unWidth);
	oFrame.put_Height(unHeight);
	oFrame.put_Stride(4 * unWidth);

	pRasterRenderer->CreateFromBgraFrame(&oFrame);
	pRasterRenderer->SetSwapRGB(false);

	double dW_MM = unWidth;
	double dH_MM = unHeight;

	pRasterRenderer->put_Width(dW_MM);
	pRasterRenderer->put_Height(dH_MM);

	enum
	{
		GenerationMask,
		LoadMaskFromBuffer,
		LoadMaskFromFile
	} enMode = LoadMaskFromBuffer;

	switch (enMode)
	{
		case GenerationMask:
		{
			// Generate mask
			pRasterRenderer->BeginCommand(c_nMaskType);
			pRasterRenderer->PathCommandStart();

			pRasterRenderer->PathCommandMoveTo(0,    300);
			pRasterRenderer->PathCommandLineTo(1000, 300);
			pRasterRenderer->PathCommandLineTo(1000, 700);
			pRasterRenderer->PathCommandLineTo(0,    700);
			pRasterRenderer->PathCommandClose();

			// Set values for mask pen and brush
			pRasterRenderer->put_PenSize(100);
			pRasterRenderer->put_PenColor(16777215); // white 16777215

			pRasterRenderer->put_BrushAlpha1(255);
			pRasterRenderer->put_BrushColor1(8355711); // gray

			pRasterRenderer->Stroke();
			pRasterRenderer->Fill();

			pRasterRenderer->EndCommand(c_nMaskType);
			pRasterRenderer->PathCommandEnd();

			break;
		}
		case LoadMaskFromBuffer:
		{
			Aggplus::CAlphaMask* pAlphaMask = new Aggplus::CAlphaMask();

			pAlphaMask->Create(unWidth, unHeight, Aggplus::EMaskDataType::AlphaBuffer);
			
			BYTE* pAlphaBuffer = pAlphaMask->GetBuffer();

			BYTE uchAlphaValue = 0;

			for (UINT unRow = 0; unRow < unHeight; ++unRow)
			{
				for (UINT unColumn = 0; unColumn < unWidth; ++unColumn)
					pAlphaBuffer[unRow * unWidth + unColumn] = uchAlphaValue;

				if (0 != unRow && 0 == unRow % 100)
					uchAlphaValue += 25;
			}

			pRasterRenderer->SetAlphaMask(pAlphaMask);

			pAlphaMask->Release();
			break;
		}
		case LoadMaskFromFile:
		{
			break;
		}
	}

	// Draw test object
	pRasterRenderer->BeginCommand(c_nPathType);
	pRasterRenderer->PathCommandStart();

//	pRasterRenderer->PathCommandMoveTo(400, 0);
//	pRasterRenderer->PathCommandLineTo(600, 0);
//	pRasterRenderer->PathCommandLineTo(600, 1000);
//	pRasterRenderer->PathCommandLineTo(400, 1000);
//	pRasterRenderer->PathCommandClose();

	pRasterRenderer->PathCommandMoveTo(300, 100);
	pRasterRenderer->PathCommandLineTo(700, 100);
	pRasterRenderer->PathCommandLineTo(900, 300);
	pRasterRenderer->PathCommandLineTo(900, 700);
	pRasterRenderer->PathCommandLineTo(700, 900);
	pRasterRenderer->PathCommandLineTo(300, 900);
	pRasterRenderer->PathCommandLineTo(100, 700);
	pRasterRenderer->PathCommandLineTo(100, 300);
	pRasterRenderer->PathCommandClose();

	// Set values for pen and brush
	pRasterRenderer->put_PenSize(100);
	pRasterRenderer->put_PenColor(255); // red

	pRasterRenderer->put_BrushAlpha1(255);
	pRasterRenderer->put_BrushColor1(65280); // green

	pRasterRenderer->Stroke();
	pRasterRenderer->Fill();

	// Reset mask
	pRasterRenderer->BeginCommand(c_nResetMaskType);
	pRasterRenderer->EndCommand(c_nResetMaskType);

	pRasterRenderer->PathCommandEnd();
	pRasterRenderer->EndCommand(c_nPathType);

	oFrame.SaveFile(L"testAlphaMask.png", 4);

	RELEASEINTERFACE(pRasterRenderer);

	return 0;
}

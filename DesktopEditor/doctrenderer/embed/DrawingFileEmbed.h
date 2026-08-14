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

#ifndef _BUILD_DRAWING_EMBED_H_
#define _BUILD_DRAWING_EMBED_H_

#include "../js_internal/js_base.h"
#include "../../graphics/pro/officedrawingfile.h"

class CDrawingFile;

using namespace NSJSBase;
class JS_DECL CDrawingFileEmbed : public CJSEmbedObject
{
public:
	CDrawingFile* m_pFile;

public:
	CDrawingFileEmbed();
	~CDrawingFileEmbed();

	virtual void* getObject() override { return (void*)m_pFile; }

public:
	JSSmart<CJSValue> OpenFile(JSSmart<CJSValue> sFile, JSSmart<CJSValue> sPassword);
	JSSmart<CJSValue> CloseFile();

	JSSmart<CJSValue> GetType();
	JSSmart<CJSValue> GetErrorCode();

	JSSmart<CJSValue> GetInfo();

	JSSmart<CJSValue> GetPixmap(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor);
	JSSmart<CJSValue> DestroyPixmap(JSSmart<CJSValue> typedArray);

	JSSmart<CJSValue> GetLinks(JSSmart<CJSValue> nPageIndex);
	JSSmart<CJSValue> GetStructure();
	JSSmart<CJSValue> GetMacros();
	JSSmart<CJSValue> GetInteractiveFormsInfo();
	JSSmart<CJSValue> GetInteractiveFormsFonts(JSSmart<CJSValue> nTypeFonts);

	JSSmart<CJSValue> GetInteractiveFormsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nWidget, JSSmart<CJSValue> nView, JSSmart<CJSValue> nButtonView);
	JSSmart<CJSValue> GetButtonIcons(JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> bBase64, JSSmart<CJSValue> nButtonWidget, JSSmart<CJSValue> nIconView);
	JSSmart<CJSValue> GetAnnotationsInfo(JSSmart<CJSValue> nPageIndex);
	JSSmart<CJSValue> GetAnnotationsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nAnnot, JSSmart<CJSValue> nView);

	JSSmart<CJSValue> GetFontBinary(JSSmart<CJSValue> Id);
	JSSmart<CJSValue> GetGlyphs(JSSmart<CJSValue> nPageIndex);
	JSSmart<CJSValue> DestroyTextInfo();
	JSSmart<CJSValue> GetGIDByUnicode(JSSmart<CJSValue> sId);

	JSSmart<CJSValue> IsNeedCMap();
	JSSmart<CJSValue> ScanPage(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> mode);
	JSSmart<CJSValue> SetScanPageFonts(JSSmart<CJSValue> arrPageIndexes);

	JSSmart<CJSValue> GetImageBase64(JSSmart<CJSValue> rId);

	JSSmart<CJSValue> FreeWasmData(JSSmart<CJSValue> typedArray);

	JSSmart<CJSValue> SplitPages(JSSmart<CJSValue> arrPageIndexes, JSSmart<CJSValue> data);
	JSSmart<CJSValue> MergePages(JSSmart<CJSValue> data, JSSmart<CJSValue> nMaxID, JSSmart<CJSValue> sPrefixForm);
	JSSmart<CJSValue> UnmergePages();
	JSSmart<CJSValue> RedactPage(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> arrRedactBox, JSSmart<CJSValue> dataFiller);
	JSSmart<CJSValue> UndoRedact();
	JSSmart<CJSValue> BuildPdf(JSSmart<CJSValue> data, JSSmart<CJSValue> sPassword);

	JSSmart<CJSValue> CheckOwnerPassword(JSSmart<CJSValue> sPassword);
	JSSmart<CJSValue> CheckPerm(JSSmart<CJSValue> nPerm);

	DECLARE_EMBED_METHODS
};

bool EmbedDrawingFile(JSSmart<NSJSBase::CJSContext>& context, IOfficeDrawingFile* pFile);

#endif // _BUILD_NATIVE_ZIP_EMBED_H_

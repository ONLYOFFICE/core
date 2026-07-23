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

#ifndef _BUILD_NATIVE_ZIP_EMBED_H_
#define _BUILD_NATIVE_ZIP_EMBED_H_

#include "../../../OfficeUtils/src/ZipFolder.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class JS_DECL CZipEmbed : public CJSEmbedObject
{
public:
	IFolder* m_pFolder;

public:
	CZipEmbed()
	{
		m_pFolder = NULL;
	}
	~CZipEmbed()
	{
		RELEASEOBJECT(m_pFolder);
	}

	virtual void* getObject() override { return (void*)m_pFolder; }

public:
	JSSmart<CJSValue> open(JSSmart<CJSValue> typedArray_or_Folder);
	JSSmart<CJSValue> create();
	JSSmart<CJSValue> save();
	JSSmart<CJSValue> getFile(JSSmart<CJSValue> filePath);
	JSSmart<CJSValue> addFile(JSSmart<CJSValue> filePath, JSSmart<CJSValue> typedArray);
	JSSmart<CJSValue> removeFile(JSSmart<CJSValue> filePath);
	JSSmart<CJSValue> close();
	JSSmart<CJSValue> getPaths();

	JSSmart<CJSValue> decodeImage(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> isRgba);
	JSSmart<CJSValue> encodeImageData(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> stride, JSSmart<CJSValue> format, JSSmart<CJSValue> isRgba);
	JSSmart<CJSValue> encodeImage(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> format);
	JSSmart<CJSValue> getImageType(JSSmart<CJSValue> typedArray);
	JSSmart<CJSValue> getImageBuffer(JSSmart<CJSValue> path);

	DECLARE_EMBED_METHODS
};

#endif // _BUILD_NATIVE_ZIP_EMBED_H_

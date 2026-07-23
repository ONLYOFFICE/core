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
#ifndef _BUILD_NATIVE_BUILDER_EMBED_H_
#define _BUILD_NATIVE_BUILDER_EMBED_H_

#include "../docbuilder.h"
#include "../js_internal/js_base.h"

// For windows fileapi
#ifdef CreateFile
#undef CreateFile
#endif

using namespace NSJSBase;
class CBuilderEmbed : public CJSEmbedObject
{
public:
	NSDoctRenderer::CDocBuilder* m_pBuilder;

	CBuilderEmbed() : m_pBuilder(NULL) {}
	~CBuilderEmbed() { if(m_pBuilder && !m_isExternalize) RELEASEOBJECT(m_pBuilder); }

	virtual void* getObject() { return (void*)m_pBuilder; }
	NSDoctRenderer::CDocBuilder_Private* GetPrivate() { return m_pBuilder->GetPrivate(); }

public:
	JSSmart<CJSValue> OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams);
	JSSmart<CJSValue> CreateFile(JSSmart<CJSValue> type);
	JSSmart<CJSValue> SetTmpFolder(JSSmart<CJSValue> path);
	JSSmart<CJSValue> SaveFile(JSSmart<CJSValue> type, JSSmart<CJSValue> path, JSSmart<CJSValue> params);
	JSSmart<CJSValue> CloseFile();
	JSSmart<CJSValue> OpenTmpFile(JSSmart<CJSValue> path, JSSmart<CJSValue> params);

	DECLARE_EMBED_METHODS
};

void builder_CreateNative(const std::string& name, JSSmart<CJSContext> context, NSDoctRenderer::CDocBuilder* builder);

#endif // _BUILD_NATIVE_BUILDER_EMBED_H_

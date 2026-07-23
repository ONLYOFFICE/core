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

#ifndef _BUILD_NATIVE_CONTROL_JS_EMBED_H_
#define _BUILD_NATIVE_CONTROL_JS_EMBED_H_

#include "./js_base.h"

#ifdef JS_ENGINE_JAVASCRIPTCORE

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

@protocol JSEmbedObjectProtocol
-(void*) getNative;
-(void) freeNative;
@end

#if __has_feature(objc_arc)
#define SUPER_DEALLOC
#else
#define SUPER_DEALLOC [super dealloc]
#endif

#define EMBED_OBJECT_WRAPPER_METHODS(CLASS)     \
-(id) init                                      \
{                                               \
	self = [super init];                        \
	if (self)                                   \
		m_internal = new CLASS;			        \
	return self;                                \
}                                               \
-(id) init : (CLASS*)pNativeObj					\
{                                               \
	self = [super init];                        \
	if (self)                                   \
		m_internal = pNativeObj;				\
	return self;                                \
}                                               \
-(void) dealloc                                 \
{                                               \
	RELEASEOBJECT(m_internal);                  \
	SUPER_DEALLOC;								\
}                                               \
-(void*) getNative								\
{                                               \
	return m_internal;                          \
}                                               \
-(void) freeNative                              \
{                                               \
	RELEASEOBJECT(m_internal);                  \
}

namespace NSJSBase
{
	class JS_DECL CJSEmbedObjectAdapterJSC : public CJSEmbedObjectAdapterBase
	{
	public:
		CJSEmbedObjectAdapterJSC() = default;
		virtual ~CJSEmbedObjectAdapterJSC() = default;

		virtual id getExportedObject(CJSEmbedObject* pNative) = 0;

		static JSSmart<CJSValue> Native2Value(JSValue* value);
		static JSValue* Value2Native(JSSmart<CJSValue> value);
	};
}

#else

namespace NSJSBase
{
	class JS_DECL CJSEmbedObjectAdapterV8 : public CJSEmbedObjectAdapterBase
	{
	public:
		using EmbedFunctionType = std::function<JSSmart<CJSValue>(CJSFunctionArguments*)>;

	public:
		CJSEmbedObjectAdapterV8() = default;
		virtual ~CJSEmbedObjectAdapterV8() = default;

		virtual std::vector<std::string> getMethodNames() = 0;
		virtual void initFunctions(CJSEmbedObject* pNativeObjBase) = 0;

	public:
		JSSmart<CJSValue> Call(const int& index, CJSFunctionArguments* args)
		{
			return m_functions[index](args);
		}

	protected:
		std::vector<EmbedFunctionType> m_functions;
	};
}

#endif

#endif // _BUILD_NATIVE_CONTROL_JS_EMBED_H_

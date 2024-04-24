/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#ifndef _BUILD_NATIVE_CONTROL_JS_EMBED_H_
#define _BUILD_NATIVE_CONTROL_JS_EMBED_H_

#include "./js_base.h"

#ifdef JS_ENGINE_JAVASCRIPTCORE

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

@protocol JSEmbedObjectProtocol
-(void*) getNative;
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

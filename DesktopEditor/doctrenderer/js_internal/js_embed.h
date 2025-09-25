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

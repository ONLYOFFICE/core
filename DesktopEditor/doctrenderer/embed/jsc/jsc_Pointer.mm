#include "../PointerEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCPointer <JSExport>
@end

@interface CJSCPointer : NSObject<IJSCPointer, JSEmbedObjectProtocol>
{
@public
    CPointerEmbedObject* m_internal;
}
@end

@implementation CJSCPointer

-(id) init
{
    self = [super init];
    return self;
}
-(void) dealloc
{
    RELEASEOBJECT(m_internal);

#if !__has_feature(objc_arc)
    [super dealloc];
#endif
}
-(void*) getNative
{
    return m_internal;
}

-(id) initWithPointer:(CPointerEmbedObject*) pointer
{
    self = [super init];
    if (self)
        m_internal = pointer;
    return self;
} 

@end

JSSmart<CJSValue> CPointerEmbedObject::createObject()
{
    CJSValueJSC* pRet = new CJSValueJSC();
    pRet->value = [JSValue valueWithObject:(id)[[CJSCPointer alloc] initWithPointer:this] inContext:NSJSBase::CJSContextPrivate::GetCurrentContext()];
    return pRet;
}

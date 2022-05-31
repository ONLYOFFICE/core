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

EMBED_OBJECT_WRAPPER_METHODS(CPointerEmbedObject)

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
    return [[CJSCPointer alloc] initWithPointer:this];
}

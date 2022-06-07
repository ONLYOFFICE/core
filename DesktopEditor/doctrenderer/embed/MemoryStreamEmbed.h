#ifndef _BUILD_NATIVE_MEMORYSTREAM_EMBED_H_
#define _BUILD_NATIVE_MEMORYSTREAM_EMBED_H_

#include "../../fontengine/MemoryStream.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class CMemoryStreamEmbed : public CJSEmbedObject
{
public:
    NSMemoryStream::CMemoryStream* m_pInternal;

public:
    CMemoryStreamEmbed() : m_pInternal(new NSMemoryStream::CMemoryStream()) {}
    ~CMemoryStreamEmbed() { RELEASEOBJECT(m_pInternal); }

    virtual void* getObject() override { return (void*)m_pInternal; }

public:
    JSSmart<CJSValue> Copy(JSSmart<CJSValue> stream, JSSmart<CJSValue> pos, JSSmart<CJSValue> len);
    JSSmart<CJSValue> ClearNoAttack();

    JSSmart<CJSValue> WriteByte(JSSmart<CJSValue> value);
    JSSmart<CJSValue> WriteBool(JSSmart<CJSValue> value);
    JSSmart<CJSValue> WriteLong(JSSmart<CJSValue> value);
    JSSmart<CJSValue> WriteDouble(JSSmart<CJSValue> value);
    JSSmart<CJSValue> WriteDouble2(JSSmart<CJSValue> value);
    JSSmart<CJSValue> WriteStringA(JSSmart<CJSValue> value);
    JSSmart<CJSValue> WriteString(JSSmart<CJSValue> value);
    JSSmart<CJSValue> WriteString2(JSSmart<CJSValue> value);

    static void CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context);
};

#endif // _BUILD_NATIVE_MEMORYSTREAM_EMBED_H_

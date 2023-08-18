#ifndef _BUILD_NATIVE_HASH_EMBED_H_
#define _BUILD_NATIVE_HASH_EMBED_H_

#include "../js_internal/js_base.h"
#include "../hash.h"

using namespace NSJSBase;
class JS_DECL CHashEmbed : public CJSEmbedObject
{
public:
	CHash* m_pHash;

	CHashEmbed() : m_pHash(new CHash(NSJSBase::NSAllocator::Alloc))
	{
	}

	~CHashEmbed()
	{
		RELEASEOBJECT(m_pHash);
	}

	virtual void* getObject() override { return (void*)m_pHash; }

	JSSmart<CJSValue> hash(JSSmart<CJSValue> data, JSSmart<CJSValue> size, JSSmart<CJSValue> alg);
	JSSmart<CJSValue> hash2(JSSmart<CJSValue> password, JSSmart<CJSValue> salt, JSSmart<CJSValue> spinCount, JSSmart<CJSValue> alg);

	DECLARE_EMBED_METHODS
};

#endif // _BUILD_NATIVE_HASH_EMBED_H_

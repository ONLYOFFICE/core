#include "HashEmbed.h"

JSSmart<CJSValue> CZipEmbed::hash(JSSmart<CJSValue> data, JSSmart<CJSValue> size, JSSmart<CJSValue> alg)
{
	std::wstring _data = data->toStringW();
	int _size = size->toInt32();
	int _alg = size->toInt32();
	unsigned char* pRes = m_pHash->hash(reinterpret_cast<const unsigned char*>(_data.c_str()), _size, _alg);
	std::string sRes(reinterpret_cast<char*>(pRes));
	return CJSContext::createString(sRes);
}

JSSmart<CJSValue> CZipEmbed::hash2(JSSmart<CJSValue> password, JSSmart<CJSValue> salt, JSSmart<CJSValue> spinCount, JSSmart<CJSValue> alg)
{
	return CJSContext::createUndefined();
}

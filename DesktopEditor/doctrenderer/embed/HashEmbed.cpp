#include "HashEmbed.h"

JSSmart<CJSValue> CHashEmbed::hash(JSSmart<CJSValue> data, JSSmart<CJSValue> size, JSSmart<CJSValue> alg)
{
	std::string _data = data->toStringA();
	int _size = size->toInt32();
	int _alg = alg->toInt32();
	unsigned char* pRes = m_pHash->hash(reinterpret_cast<const unsigned char*>(_data.c_str()), _size, _alg);
	return CJSContext::createUint8Array(pRes, NSHash::getDigestLength(static_cast<NSHash::HashAlgs>(_alg)));
}

JSSmart<CJSValue> CHashEmbed::hash2(JSSmart<CJSValue> password, JSSmart<CJSValue> salt, JSSmart<CJSValue> spinCount, JSSmart<CJSValue> alg)
{
	std::string _password = password->toStringA();
	std::string _salt = salt->toStringA();
	int _spinCount = spinCount->toInt32();
	int _alg = alg->toInt32();
	unsigned char* pRes = m_pHash->hash2(reinterpret_cast<const char*>(_password.c_str()), reinterpret_cast<const char*>(_salt.c_str()), _spinCount, _alg);
	return CJSContext::createUint8Array(pRes, NSHash::getDigestLength(static_cast<NSHash::HashAlgs>(_alg)));
}

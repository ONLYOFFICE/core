#ifndef _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H
#define _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H

#include "Objects.h"

namespace PdfWriter
{
	class CEncrypt;
	class CInfoDict;

	class CEncryptDict : public CDictObject
	{
	public:
		CEncryptDict(CXref* pXref);
		~CEncryptDict();
		EDictType GetDictType() const
		{
			return dict_type_ENCRYPT;
		}

		void CreateId(CInfoDict* pInfo, CXref* pXref);
		void SetPassword(const char* sOwnerPassword, const char* sUserPassword);
		void Prepare(CInfoDict* pInfo, CXref* pXref);
		CEncrypt* GetEncrypt() const
		{
			return m_pEncrypt;
		}
	private:
		CEncrypt* m_pEncrypt;
	};
}
#endif // _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H

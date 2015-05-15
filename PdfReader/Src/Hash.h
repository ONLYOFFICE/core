#ifndef _PDF_READER_HASH_H
#define _PDF_READER_HASH_H

namespace PdfReader
{
	class  StringExt;
	struct THashBucket;
	struct THashIter;

	//------------------------------------------------------------------------
	// CHash
	//------------------------------------------------------------------------

	class CHash
	{

	public:

		CHash(bool bDeleteKeys = false);
		~CHash();

		void Add(StringExt *seKey, void *pValue);
		void Add(StringExt *seKey, int   nValue);

		void Replace(StringExt *seKey, void *pValue);
		void Replace(StringExt *seKey, int   nValue);

		void *Lookup(StringExt *seKey);
		void *Lookup(char *sKey);
		int   LookupInt(StringExt *seKey);
		int   LookupInt(char *sKey);

		void *Remove(StringExt *seKey);
		void *Remove(char *seKey);
		int   RemoveInt(StringExt *seKey);
		int   RemoveInt(char *sKey);

		int GetLength()
		{
			return m_nLength;
		}

		void StartIter(THashIter **ppIter);

		bool GetNext(THashIter **ppIter, StringExt **pseKey, void **ppValue);
		bool GetNext(THashIter **ppIter, StringExt **pseKey, int   *pnValue);

		void DeleteIter(THashIter **ppIter);

	private:

		void Expand();
		THashBucket *Find(StringExt *seKey, int *pnHashIndex);
		THashBucket *Find(char       *sKey, int *pnHashIndex);

		int Hash(StringExt *seKey);
		int Hash(char *sKey);

	private:

		bool          m_bDeleteKeys;   // Будем ли удалять имена? set if key strings should be deleted
		int           m_nBucketsCount; // Количество наборов
		int           m_nLength;       // Количество вхождений
		THashBucket **m_ppTable;
	};

#define DeleteCHash(hash, T)                     \
  do                                             \
  {                                              \
	CHash *_hash = (hash);                       \
    {                                            \
      THashIter *_iter;                          \
      StringExt *_key;                           \
      void *_p;                                  \
      _hash->StartIter(&_iter);                  \
	  while (_hash->GetNext(&_iter, &_key, &_p)) \
	  {                                          \
        delete (T*)_p;                           \
	  }                                          \
      delete _hash;                              \
	}                                            \
  } while(0);

}

#endif // _PDF_READER_HASH_H

/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#ifndef _ASC_FONTCONVERTER_HASH_H_
#define _ASC_FONTCONVERTER_HASH_H_

namespace NSFontConverter
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

      bool GetNext(THashIter **ppIter, StringExt **pseKey, void **ppValue );
      bool GetNext(THashIter **ppIter, StringExt **pseKey, int   *pnValue );

      void DeleteIter(THashIter **ppIter);

    private:

      void Expand();
      THashBucket *Find(StringExt *seKey, int *pnHashIndex );
      THashBucket *Find(char       *sKey, int *pnHashIndex );

      int Hash(StringExt *seKey);
      int Hash(char *sKey);

    private:

      bool          m_bDeleteKeys;   // Будем ли удалять имена? set if key strings should be deleted
      int           m_nBucketsCount; // Количество наборов
      int           m_nLength;       // Количество вхождений
      THashBucket **m_ppTable;
    };

#define DeleteCHash(hash, T)                       \
  do {                                             \
    CHash *_hash = (hash);                         \
    {                                              \
      THashIter *_iter;                            \
      StringExt *_key;                               \
      void *_p;                                    \
      _hash->StartIter(&_iter);                    \
      while (_hash->GetNext(&_iter, &_key, &_p)) { \
        delete (T*)_p;                             \
      }                                            \
      delete _hash;                                \
    }                                              \
  } while(0)

}

#endif /* _ASC_FONTCONVERTER_HASH_H_ */

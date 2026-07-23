/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

      bool          m_bDeleteKeys;   // Whether to delete names? set if key strings should be deleted
      int           m_nBucketsCount; // Number of buckets
      int           m_nLength;       // Number of entries
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

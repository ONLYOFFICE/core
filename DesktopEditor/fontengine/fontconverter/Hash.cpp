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
#include "MemoryUtils.h"
#include "StringExt.h"
#include "Hash.h"
#include "Utils.h"

namespace NSFontConverter
{
    //------------------------------------------------------------------------

    struct THashBucket
    {
      StringExt   *seKey;
      union
      {
        void *p;
        int   i;
      } uValue;
      THashBucket *pNext;
    };

    struct THashIter
    {
      int          nHashIndex;
      THashBucket *pBucket;
    };

    //------------------------------------------------------------------------

    CHash::CHash(bool bDeleteKeys)
    {
        m_bDeleteKeys = bDeleteKeys;
        m_nBucketsCount = 7;
        m_ppTable = (THashBucket **)MemUtilsMallocArray( m_nBucketsCount, sizeof(THashBucket *));
        for ( int nHashIndex = 0; nHashIndex < m_nBucketsCount; ++nHashIndex )
            m_ppTable[ nHashIndex ] = NULL;
        m_nLength = 0;
    }

    CHash::~CHash()
    {
        THashBucket *pBucket = NULL;

        for ( int nHashIndex = 0; nHashIndex < m_nBucketsCount; ++nHashIndex )
        {
            while ( m_ppTable[ nHashIndex ] )
            {
                pBucket = m_ppTable[ nHashIndex ];
                m_ppTable[ nHashIndex ] = pBucket->pNext;
                if (m_bDeleteKeys)
                {
                    delete pBucket->seKey;
                }
                delete pBucket;
            }
        }
        MemUtilsFree( m_ppTable );
    }

    void CHash::Add(StringExt *seKey, void *pValue)
    {
        if ( m_nLength >= m_nBucketsCount )
            Expand();

        THashBucket *pBucket = new THashBucket;
        pBucket->seKey = seKey;
        pBucket->uValue.p = pValue;
        int nHashIndex = Hash( seKey );
        pBucket->pNext = m_ppTable[ nHashIndex ];
        m_ppTable[ nHashIndex ] = pBucket;
        ++m_nLength;
    }

    void CHash::Add(StringExt *seKey, int nValue)
    {
        if ( m_nLength >= m_nBucketsCount )
            Expand();

        THashBucket *pBucket = new THashBucket;
        pBucket->seKey = seKey;
        pBucket->uValue.i = nValue;
        int nHashIndex = Hash( seKey );
        pBucket->pNext = m_ppTable[ nHashIndex ];
        m_ppTable[ nHashIndex ] = pBucket;
        ++m_nLength;
    }

    void CHash::Replace(StringExt *seKey, void *pValue)
    {
        THashBucket *pBucket = NULL;
        int nHashIndex = 0;

        if ( ( pBucket = Find( seKey, &nHashIndex ) ) )
        {
            pBucket->uValue.p = pValue;
            delete seKey;
        }
        else
            Add( seKey, pValue);
    }

    void CHash::Replace(StringExt *seKey, int nValue)
    {
        THashBucket *pBucket = NULL;
        int nHashIndex = 0;

        if ( ( pBucket = Find( seKey, &nHashIndex ) ) )
        {
            pBucket->uValue.i = nValue;
            delete seKey;
        }
        else
            Add( seKey, nValue);
    }

    void *CHash::Lookup(StringExt *seKey)
    {
        THashBucket *pBucket = NULL;
        int nHashIndex = 0;

        if ( !( pBucket = Find( seKey, &nHashIndex ) ) )
            return NULL;
        return pBucket->uValue.p;
    }

    int CHash::LookupInt(StringExt *seKey)
    {
        THashBucket *pBucket = NULL;
        int nHashIndex = 0;

        if ( !( pBucket = Find( seKey, &nHashIndex ) ) )
            return 0;

        return pBucket->uValue.i;
    }

    void *CHash::Lookup(char *sKey)
    {
        THashBucket *pBucket = NULL;
        int nHashIndex = 0;

        if ( !( pBucket = Find( sKey, &nHashIndex ) ) )
            return NULL;
        return pBucket->uValue.p;
    }

    int CHash::LookupInt(char *sKey)
    {
        THashBucket *pBucket = NULL;
        int nHashIndex = 0;

        if ( !( pBucket = Find( sKey, &nHashIndex ) ) )
            return 0;

        return pBucket->uValue.i;
    }

    void *CHash::Remove(StringExt *seKey)
    {
        THashBucket *pBucket;
        THashBucket **ppBucket;
        int nHashIndex = 0;

        if ( !( pBucket = Find( seKey, &nHashIndex ) ) )
            return NULL;

        ppBucket = &m_ppTable[ nHashIndex ];
        while ( *ppBucket != pBucket )
            ppBucket = &((*ppBucket)->pNext);

        *ppBucket = pBucket->pNext;
        if ( m_bDeleteKeys )
            delete pBucket->seKey;
        void *pValue = pBucket->uValue.p;
        delete pBucket;
        --m_nLength;
        return pValue;
    }

    int CHash::RemoveInt(StringExt *seKey)
    {
        THashBucket *pBucket;
        THashBucket **ppBucket;
        int nHashIndex = 0;

        if ( !( pBucket = Find( seKey, &nHashIndex) ) )
            return 0;

        ppBucket = &m_ppTable[ nHashIndex ];
        while ( *ppBucket != pBucket )
            ppBucket = &((*ppBucket)->pNext);

        *ppBucket = pBucket->pNext;
        if (m_bDeleteKeys)
            delete pBucket->seKey;

        int nValue = pBucket->uValue.i;
        delete pBucket;
        --m_nLength;
        return nValue;
    }

    void *CHash::Remove(char *sKey)
    {
        THashBucket *pBucket;
        THashBucket **ppBucket;
        int nHashIndex = 0;

        if ( !( pBucket = Find( sKey, &nHashIndex ) ) )
            return NULL;
        ppBucket = &m_ppTable[ nHashIndex ];
        while ( *ppBucket != pBucket )
            ppBucket = &((*ppBucket)->pNext);

        *ppBucket = pBucket->pNext;

        if (m_bDeleteKeys)
            delete pBucket->pNext;

        void *pValue = pBucket->uValue.p;
        delete pBucket;
        --m_nLength;
        return pValue;
    }

    int CHash::RemoveInt(char *sKey)
    {
        THashBucket *pBucket;
        THashBucket **ppBucket;
        int nHashIndex = 0;

        if ( !( pBucket = Find( sKey, &nHashIndex ) ) )
            return 0;

        ppBucket = &m_ppTable[ nHashIndex ];
        while (*ppBucket != pBucket )
            ppBucket = &((*ppBucket)->pNext);

        *ppBucket = pBucket->pNext;
        if ( m_bDeleteKeys )
            delete pBucket->seKey;

        int nValue = pBucket->uValue.i;
        delete pBucket;
        --m_nLength;
        return nValue;
    }

    void CHash::StartIter(THashIter **ppIter)
    {
        *ppIter = new THashIter;
        (*ppIter)->nHashIndex = -1;
        (*ppIter)->pBucket = NULL;
    }

    bool CHash::GetNext(THashIter **ppIter, StringExt **pseKey, void **ppValue)
    {
        if (!*ppIter)
            return false;

        if ((*ppIter)->pBucket)
            (*ppIter)->pBucket = (*ppIter)->pBucket->pNext;

        while (!(*ppIter)->pBucket)
        {
            if (++(*ppIter)->nHashIndex == m_nBucketsCount)
            {
                delete *ppIter;
                *ppIter = NULL;
                return false;
            }
            (*ppIter)->pBucket = m_ppTable[(*ppIter)->nHashIndex];
        }
        *pseKey = (*ppIter)->pBucket->seKey;
        *ppValue = (*ppIter)->pBucket->uValue.p;
        return true;
    }

    bool CHash::GetNext(THashIter **ppIter, StringExt **pseKey, int   *pnValue)
    {
        if (!*ppIter)
            return false;

        if ((*ppIter)->pBucket)
            (*ppIter)->pBucket = (*ppIter)->pBucket->pNext;

        while (!(*ppIter)->pBucket)
        {
            if (++(*ppIter)->nHashIndex == m_nBucketsCount)
            {
                delete *ppIter;
                *ppIter = NULL;
                return false;
            }
            (*ppIter)->pBucket = m_ppTable[(*ppIter)->nHashIndex ];
        }
        *pseKey = (*ppIter)->pBucket->seKey;
        *pnValue = (*ppIter)->pBucket->uValue.i;
        return true;
    }

    void CHash::DeleteIter(THashIter **ppIter)
    {
        delete *ppIter;
        *ppIter = NULL;
    }

    void CHash::Expand()
    {
        int nHashIndex = 0;

        int nOldSize = m_nBucketsCount;
        THashBucket **ppOldTab = m_ppTable;
        m_nBucketsCount = 2 * m_nBucketsCount + 1;
        m_ppTable = (THashBucket **)MemUtilsMallocArray( m_nBucketsCount, sizeof(THashBucket *) );
        for ( nHashIndex = 0; nHashIndex < m_nBucketsCount; ++nHashIndex )
            m_ppTable[ nHashIndex ] = NULL;

        for (int nIndex = 0; nIndex < nOldSize; ++nIndex )
        {
            while ( ppOldTab[nIndex] )
            {
                THashBucket *pBucket = ppOldTab[ nIndex ];
                ppOldTab[ nIndex ] = ppOldTab[ nIndex ]->pNext;
                nHashIndex = Hash( pBucket->seKey );
                pBucket->pNext = m_ppTable[ nHashIndex ];
                m_ppTable[ nHashIndex ] = pBucket;
            }
        }
        MemUtilsFree( ppOldTab );
    }

    THashBucket *CHash::Find(StringExt *seKey, int *pnHashIndex)
    {
        THashBucket *pBucket;

        *pnHashIndex = Hash( seKey );
        for ( pBucket = m_ppTable[ *pnHashIndex ]; pBucket; pBucket = pBucket->pNext )
        {
            if ( !pBucket->seKey->Compare( seKey ) )
                return pBucket;
        }
        return NULL;
    }
    THashBucket *CHash::Find(char       *sKey, int *pnHashIndex)
    {
        THashBucket *pBucket;

        *pnHashIndex = Hash( sKey );
        for ( pBucket = m_ppTable[ *pnHashIndex ]; pBucket; pBucket = pBucket->pNext )
        {
            if ( !pBucket->seKey->Compare( sKey ) )
                return pBucket;
        }
        return NULL;
    }

    int CHash::Hash(StringExt *seKey)
    {
        char *sTemp;
        unsigned int nHash = 0;
        int nIndex;

        for ( sTemp = seKey->GetBuffer(), nIndex = 0; nIndex < seKey->GetLength(); ++sTemp, ++nIndex )
            nHash = 17 * nHash + (int)(*sTemp & 0xff);

        return (int)(nHash % m_nBucketsCount);
    }

    int CHash::Hash(char *sKey)
    {
        char *sTemp;
        unsigned int nHash = 0;

        for ( sTemp = sKey; *sTemp; ++sTemp )
            nHash = 17 * nHash + (int)(*sTemp & 0xff);

        return (int)(nHash % m_nBucketsCount);
    }
}

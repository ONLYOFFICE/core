#include <stdlib.h>
#include <string>

class CPaths
{
protected:
    char*	m_pData;
    size_t	m_lSize;

    char*	m_pDataCur;
    size_t	m_lSizeCur;

public:
    CPaths()
    {
        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = m_lSize;
    }
    CPaths(size_t nLen)
    {
        m_lSize = nLen;
        m_pData = (char*)malloc(m_lSize * sizeof(char));

        m_lSizeCur = 0;
        m_pDataCur = m_pData;
    }
    virtual ~CPaths()
    {
        Clear();
    }

    inline void AddSize(size_t nSize)
    {
        if (NULL == m_pData)
        {
            m_lSize = 1000;
            if (nSize > m_lSize)
                m_lSize = nSize;

            m_pData = (char*)malloc(m_lSize * sizeof(char));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
            return;
        }

        if ((m_lSizeCur + nSize) > m_lSize)
        {
            while ((m_lSizeCur + nSize) > m_lSize)
                m_lSize *= 2;

            char* pRealloc = (char*)realloc(m_pData, m_lSize * sizeof(char));
            if (NULL != pRealloc)
            {
                m_pData    = pRealloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
            else
            {
                char* pMalloc = (char*)malloc(m_lSize * sizeof(char));
                memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(char));

                free(m_pData);
                m_pData    = pMalloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
        }
    }

public:
    void AddInt(unsigned int value)
    {
        AddSize(4);
        memcpy(m_pDataCur, &value, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
    }
    void WriteString(const char* value, unsigned int len)
    {
        AddSize(len + 4);
        memcpy(m_pDataCur, &len, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
        memcpy(m_pDataCur, value, len);
        m_pDataCur += len;
        m_lSizeCur += len;
    }
    unsigned char* GetBuffer()
    {
        return (unsigned char*)m_pData;
    }

    void Clear()
    {
        if (m_pData) free(m_pData);

        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    void ClearNoAttack()
    {
        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    unsigned int GetSize()
    {
        return (unsigned int)m_lSizeCur;
    }

    void SkipLen()
    {
        AddInt(0);
    }
    void WriteLen()
    {
        unsigned int len = (unsigned int)m_lSizeCur;
        memcpy(m_pData, &len, sizeof(unsigned int));
    }
};

class CFile
{
protected:
    unsigned char* m_pData;
    size_t m_lSize;

    unsigned char* m_pDataCur;
    size_t m_lSizeCur;

public:
    CFile()
    {
        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = m_lSize;
    }
    CFile(size_t nLen)
    {
        m_lSize = nLen;
        m_pData = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));

        m_lSizeCur = 0;
        m_pDataCur = m_pData;
    }
    virtual ~CFile()
    {
        Clear();
    }

    inline void AddSize(size_t nSize)
    {
        if (NULL == m_pData)
        {
            m_lSize = 1000;
            if (nSize > m_lSize)
                m_lSize = nSize;

            m_pData = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
            return;
        }

        if ((m_lSizeCur + nSize) > m_lSize)
        {
            while ((m_lSizeCur + nSize) > m_lSize)
                m_lSize *= 2;

            unsigned char* pRealloc = (unsigned char*)realloc(m_pData, m_lSize * sizeof(unsigned char));
            if (NULL != pRealloc)
            {
                m_pData    = pRealloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
            else
            {
                unsigned char* pMalloc = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));
                memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(unsigned char));

                free(m_pData);
                m_pData    = pMalloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
        }
    }

public:
    void AddInt(unsigned int value)
    {
        AddSize(4);
        memcpy(m_pDataCur, &value, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
    }
    void WriteString(unsigned char* value, unsigned int len)
    {
        AddSize(len + 4);
        memcpy(m_pDataCur, &len, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
        memcpy(m_pDataCur, value, len);
        m_pDataCur += len;
        m_lSizeCur += len;
    }
    unsigned char* GetBuffer()
    {
        return m_pData;
    }

    void Clear()
    {
        if (m_pData) free(m_pData);

        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    void ClearNoAttack()
    {
        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    unsigned int GetSize()
    {
        return (unsigned int)m_lSizeCur;
    }

    void SkipLen()
    {
        AddInt(0);
    }
    void WriteLen()
    {
        unsigned int len = (unsigned int)m_lSizeCur;
        memcpy(m_pData, &len, sizeof(unsigned int));
    }
};

class Zlib
{
public:
    unsigned char* buffer;
    unsigned long  size;

    CPaths m_oPaths;
    CFile  m_oFile;

public:
    Zlib()
    {
        buffer = NULL;
        size   = -1;
    }
    Zlib(unsigned char* _buffer, unsigned long _size)
    {
        buffer = _buffer;
        size   = _size;
    }
    ~Zlib()
    {
        if (buffer) delete[] buffer;
    }

    bool isInit()
    {
        return buffer != NULL;
    }
};

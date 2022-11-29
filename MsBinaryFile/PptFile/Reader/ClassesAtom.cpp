#include "ClassesAtom.h"


CUserEdit::CUserEdit()
{
    m_nOffsetLastEdit			= 0;
    m_nOffsetPersistDirectory	= 0;
    m_nDocumentRef				= 0;
    m_nMaxPersistWritten		= 0;
    m_nEncryptRef				= 0;
}

CUserEdit::~CUserEdit()
{
}

CUserEdit &CUserEdit::operator =(const CUserEdit &oSrc)
{
    m_nOffsetLastEdit			= oSrc.m_nOffsetLastEdit;
    m_nOffsetPersistDirectory	= oSrc.m_nOffsetPersistDirectory;
    m_nDocumentRef				= oSrc.m_nDocumentRef;
    m_nMaxPersistWritten		= oSrc.m_nMaxPersistWritten;
    m_nEncryptRef				= oSrc.m_nEncryptRef;
    return (*this);
}

void CUserEdit::FromAtom(CRecordUserEditAtom *pAtom)
{
    m_nOffsetLastEdit			= pAtom->m_nOffsetLastEdit;
    m_nOffsetPersistDirectory	= pAtom->m_nOffsetPersistDirectory;
    m_nDocumentRef				= pAtom->m_nOffsetDocPersistIdRef;
    m_nMaxPersistWritten		= pAtom->m_nPersistIdSeed;
    m_nEncryptRef				= pAtom->m_nEncryptSessionPersistIdRef;
}

CCurrentUser::CCurrentUser()
{
    m_bIsEncrypt			= false;
    m_nOffsetToCurrentEdit	= 0;
}

CCurrentUser::~CCurrentUser()
{
}

CCurrentUser &CCurrentUser::operator =(const CCurrentUser &oSrc)
{
    m_bIsEncrypt			= oSrc.m_bIsEncrypt;
    m_nOffsetToCurrentEdit	= oSrc.m_nOffsetToCurrentEdit;
    m_sName					= oSrc.m_sName;
    return (*this);
}

void CCurrentUser::FromAtom(CRecordCurrentUserAtom *pAtom)
{
    m_bIsEncrypt			= (ENCRYPT == pAtom->m_nToken);

    m_nOffsetToCurrentEdit	= pAtom->m_nOffsetToCurEdit;
    m_sName					= pAtom->m_strUNICODEUserName;
}

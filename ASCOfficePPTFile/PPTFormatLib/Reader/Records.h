#pragma once

#include "PPTFileDefines.h"
#include "../Reader/ReadStructures.h"
#include "../../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/Enums.h"

using namespace NSPresentationEditor;

struct SRecordHeader 
{ 
	BYTE			RecVersion;                
	USHORT			RecInstance;  
	USHORT			RecType; 
	UINT	        RecLen; 
	
	SRecordHeader()
	{
		RecVersion = RecInstance = RecType = RecLen = 0;
	}

    bool ReadFromStream(POLE::Stream * pStream)
	{
		RecVersion = RecInstance = RecType = RecLen = 0;

		POLE::uint64 nRd = 0; 
		
		unsigned short rec =0;
		nRd = pStream->read((unsigned char*)&(rec), 2);

		if (nRd != 2) return FALSE;

		//RecVersion = rec & 0xFF0F;
		RecInstance = rec >> 4;
		RecVersion = rec - (RecInstance<<4);

		nRd = pStream->read((unsigned char*)&(RecType), 2);
	
		nRd = pStream->read((unsigned char*)&(RecLen), 4);

		long sz = pStream->size()-pStream->tell();

		if (RecLen > sz )
		{
			RecLen = sz;
		}

        return true;
	}

    bool IsContainer()
	{
		/*if ((RecVersion == PSFLAG_CONTAINER) || ((RecVersion & 0x0F) == 0x0F))
		{
			return TRUE;
		}*/
		if (1064 == RecType)
            return false;
		
		if (RecVersion == 0x0F)
		{
            return true;
		}
        return false;
	}

	SRecordHeader& operator =(const SRecordHeader& oSrc)
	{
		RecVersion	= oSrc.RecVersion;
		RecInstance = oSrc.RecInstance;
		RecType		= oSrc.RecType;
		RecLen		= oSrc.RecLen;
		return (*this);
	}

	void ToXmlWriter(XmlUtils::CXmlWriter* pWriter)
	{
		CString str = GetRecordName((DWORD)RecType);
		
		pWriter->WriteNodeBegin(str, TRUE);
		pWriter->WriteAttribute(_T("length"), CDirectory::ToString(RecLen));
		pWriter->WriteAttribute(_T("type"), CDirectory::ToString(RecType));
		pWriter->WriteAttribute(_T("instance"), CDirectory::ToString(RecInstance));

		pWriter->WriteNodeEnd(str, TRUE);
	}

	void ToXmlWriter(XmlUtils::CXmlWriter* pWriter, POLE::Stream* pStream);

	static CString ToString(SRecordHeader oHeader)
	{
		CString str = GetRecordName((DWORD)oHeader.RecType);
		
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(str, TRUE);
		
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(oHeader.RecType));
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(oHeader.RecLen));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(oHeader.RecInstance));

		oWriter.WriteNodeEnd(str, TRUE);
		return oWriter.GetXmlString();
	}

	CString ToString()
	{
		return ToString(*this);
	}
};

class IRecord
{
public:
	SRecordHeader m_oHeader;

public:
	// читаем из файла
	virtual ~IRecord(){}
	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) = 0;
	virtual CString ToString() = 0;
};

class CUnknownRecord : public IRecord
{
	// этот класс - просто для того, чтобы нигде не проверять, 
	// реализована ли у нас такая запись

public:
	CUnknownRecord()
	{
	}

	~CUnknownRecord()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		StreamUtils::StreamSkip((long)m_oHeader.RecLen, pStream);
	}
	virtual CString ToString()
	{
		return m_oHeader.ToString();
	}
};

IRecord* CreateByType(SRecordHeader oHeader);

class CRecordsContainer : public IRecord
{
protected:
	std::vector<IRecord*> m_arRecords;

public:

	CRecordsContainer() : m_arRecords()
	{
	}

	virtual ~CRecordsContainer()
	{
		Clear();
	}

	void Clear()
	{
		size_t nCount = m_arRecords.size();
		while(0 != nCount) 
		{ 
			if (NULL != m_arRecords[nCount-1])
				delete m_arRecords[nCount-1]; 
			
			m_arRecords.pop_back();
			--nCount;
		} 
	}
	
	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream);

	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		if (0 == m_arRecords.size())
		{
			oWriter.WriteNodeEnd(strName, TRUE);
		}
		else
		{
			oWriter.WriteNodeEnd(strName, TRUE, FALSE);

			for (int nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
			{
				oWriter.WriteString(m_arRecords[nIndex]->ToString());
			}

			oWriter.WriteNodeEnd(strName);
		}	
		
		return oWriter.GetXmlString();
	}

	template <typename T>
	void GetRecordsByType(std::vector<T>* pArray, bool bIsChild, bool bOnlyFirst = false)
	{
		if (NULL == pArray)
			return;

		// возвращаем указатели, их не удалять наверху!!!!
		for (int nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
		{
			T pRec = dynamic_cast<T>(m_arRecords[nIndex]);
			if (NULL != pRec)
			{
				pArray->push_back(pRec);
				if (bOnlyFirst)
				{
					return;
				}
			}
			else if ((bIsChild) && (m_arRecords[nIndex]->m_oHeader.IsContainer()))
			{
				CRecordsContainer* pContainer = dynamic_cast<CRecordsContainer*>(m_arRecords[nIndex]);
				if (NULL != pContainer)
				{				
					pContainer->GetRecordsByType(pArray, bIsChild, bOnlyFirst);
				}
			}
		}
	}
};

/**************************************************************
	контейнеры... (по идее они не нужны, потом можно убрать)
**************************************************************/

/*
class CRecordDocument					: public CRecordsContainer {};
class CRecordEnvironment				: public CRecordsContainer {};
class CRecordExAviMovie					: public CRecordsContainer {};
class CRecordExCDAudio					: public CRecordsContainer {};
class CRecordExControl					: public CRecordsContainer {};
class CRecordExEmbed					: public CRecordsContainer {};
class CRecordExHyperlink				: public CRecordsContainer {};
class CRecordExHyperlink9				: public CRecordsContainer {};
class CRecordExLink						: public CRecordsContainer {};
class CRecordExMCIMovie					: public CRecordsContainer {};
class CRecordExMIDIAudio				: public CRecordsContainer {};
class CRecordExObjList					: public CRecordsContainer {};
class CRecordExOleObjStg				: public CRecordsContainer {};
class CRecordExQuickTimeMovie			: public CRecordsContainer {};
class CRecordExVideo					: public CRecordsContainer {};
class CRecordExWAVAudioEmbedded			: public CRecordsContainer {};
class CRecordExWAVAudioLink				: public CRecordsContainer {};
class CRecordFontCollection				: public CRecordsContainer {};
class CRecordFontCollection10			: public CRecordsContainer {};
class CRecordHandout					: public CRecordsContainer {};
class CRecordHeadersFooters				: public CRecordsContainer {};
class CRecordHTMLPublishInfo			: public CRecordsContainer {};
class CRecordInteractiveInfo			: public CRecordsContainer {};
class CRecordList						: public CRecordsContainer {};
class CRecordMainMaster					: public CRecordsContainer {};
class CRecordMsofbtClientData			: public CRecordsContainer {};
class CRecordNamedShow					: public CRecordsContainer {};
class CRecordNamedShows					: public CRecordsContainer {};
class CRecordNotes						: public CRecordsContainer {};
class CRecordNotesTextViewInfo			: public CRecordsContainer {};
class CRecordOutlineTextProps9			: public CRecordsContainer {};
class CRecordOutlineTextProps10			: public CRecordsContainer {};
class CRecordOutlineTextProps11			: public CRecordsContainer {};
class CRecordOutlineViewInfo			: public CRecordsContainer {};
class CRecordParaBuild					: public CRecordsContainer {};
class CRecordProgBinaryTag				: public CRecordsContainer {};
class CRecordProgStringTag				: public CRecordsContainer {};
class CRecordProgTags					: public CRecordsContainer {};
class CRecordSlide						: public CRecordsContainer {};
class CRecordSlideListTable10			: public CRecordsContainer {};
class CRecordSlideListWithText			: public CRecordsContainer {};
class CRecordSlideSyncInfo12			: public CRecordsContainer {};
class CRecordSlideViewInfo				: public CRecordsContainer {};
class CRecordSorterViewInfo				: public CRecordsContainer {};
class CRecordSound						: public CRecordsContainer {};
class CRecordSoundCollection			: public CRecordsContainer {};
class CRecordSrKinsoku					: public CRecordsContainer {};
class CRecordSummary					: public CRecordsContainer {};
class CRecordVBAInfo					: public CRecordsContainer {};
*/

/********************************************************************************
		создаем по типу (остальные будем добавлять по мере необходимости)
********************************************************************************/

//-------------------------------------------------------------------------------
#define CREATE_BY_TYPE(RECORD_TYPE, CLASS_RECORD_NAME)							\
	case RECORD_TYPE: { pRecord = new CLASS_RECORD_NAME(); break; }				\
//-------------------------------------------------------------------------------

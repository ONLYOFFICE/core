#ifndef HWPRECORDBINDATA_H
#define HWPRECORDBINDATA_H

#include "HWPRecord.h"
#include "../HWPStream.h"
#include "../HWPDocInfo.h"
#include "../Common/XMLReader.h"

namespace HWP
{
enum class ECompressed
{
	FOLLOW_STORAGE = 0x00,
	COMPRESS = 0x10,
	NO_COMPRESS = 0x20,
};

enum class EType
{
	LINK = 0x0,
	EMBEDDING = 0x1,
	STORAGE = 0x2
};

enum class EState
{
	NEVER_ACCESSED = 0x000,
	FOUND_FILE_BY_ACCESS = 0x100,
	ACCESS_FAILED = 0x200,
	LINK_ACCESS_IGNORED = 0x400
};

class CHWPRecordBinData : public CHWPRecord
{
	EType m_eType;
	ECompressed m_eCompressed;
	EState m_eState;

	HWP_STRING m_sAPath;
	HWP_STRING m_sRPath;
	short m_shBinDataID;
	HWP_STRING m_sFormat;

	HWP_STRING m_sItemID;
public:
	CHWPRecordBinData(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordBinData(CXMLReader& oReader, int nVersion);

	HWP_STRING GetPath() const;
	HWP_STRING GetItemID() const;
	HWP_STRING GetFormat() const;

	EType GetType() const;
	ECompressed GetCompressed() const;

	short GetBinDataID() const;
};
}

#endif // HWPRECORDBINDATA_H

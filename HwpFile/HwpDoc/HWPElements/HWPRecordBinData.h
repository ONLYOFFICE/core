#ifndef HWPRECORDBINDATA_H
#define HWPRECORDBINDATA_H

#include "HWPRecord.h"
#include "../HWPStream.h"
#include "../HWPDocInfo.h"

namespace HWP
{
class CHWPRecordBinData : public CHWPRecord
{
	enum class EType
	{
		LINK = 0x0,
		EMBEDDING = 0x1,
		STORAGE = 0x2
	} m_eType;

	enum class ECompressed
	{
		FOLLOW_STORAGE = 0x00,
		COMPRESS = 0x10,
		NO_COMPRESS = 0x20
	} m_eCompressed;

	enum class EState
	{
		NEVER_ACCESSED = 0x000,
		FOUND_FILE_BY_ACCESS = 0x100,
		ACCESS_FAILED = 0x200,
		LINK_ACCESS_IGNORED = 0x400
	} m_eState;


	std::string m_sAPath;
	short m_shBinDataID;
	std::string m_sFormat;

	std::string m_sItemID;
public:
	CHWPRecordBinData(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	std::string GetItemID() const;
};
}

#endif // HWPRECORDBINDATA_H

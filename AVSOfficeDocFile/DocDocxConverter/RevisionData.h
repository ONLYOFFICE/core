#pragma once

#include "DateAndTime.h"
#include "CharacterPropertyExceptions.h"

namespace DocFileFormat
{
	typedef enum _RevisionType
	{
		NoRevision,
		Inserted,
		Deleted,
		Changed
	} RevisionType;

	class RevisionData
	{
		friend class CharacterPropertiesMapping;
		friend class DocumentMapping;
		friend class ParagraphPropertiesMapping;
		friend class StyleSheetMapping;
		friend class TableRowPropertiesMapping;

	public:
		virtual ~RevisionData();
		RevisionData();
		/// Collects the revision data of a CHPX
		RevisionData(CharacterPropertyExceptions* chpx);

	private:
		DateAndTime Dttm;
		short Isbt;
		RevisionType Type;
		list<SinglePropertyModifier>* Changes;
		int RsidDel;
		int RsidProp;
		int Rsid;
	};
}
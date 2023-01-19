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

#include "TxO.h"

#include "Font.h"

#include "../../../../../OOXML/Base/Unit.h"

#include "../../../Converter/XlsConverter.h"
#include "../../../Converter/xlsx_conversion_context.h"


namespace XLS
{

TxO::~TxO()
{
}


BaseObjectPtr TxO::clone()
{
	return BaseObjectPtr(new TxO(*this));
}


void TxO::readFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	unsigned short flags;
	
	if (global_info->Version < 0x0600)
	{
		short mnLinkSize;
		short mnButtonFlags;
		short mnShortcut;
		short mnShortcutEA;	
		
		record >> cchText;
		record.skipNunBytes( 2 );
		record >> cbRuns >> ifntEmpty;
		record.skipNunBytes( 2 );
		record >> flags >> rot;
		record.skipNunBytes( 2 );
		record >> mnLinkSize;
		record.skipNunBytes( 2 );
		record >> mnButtonFlags >> mnShortcut >> mnShortcutEA;

		if (nameLength > 0)
		{
			record >> name;
		}

		bool bAutoSize = flags & 0x0080;

		if (record.getRdPtr() % 2 != 0/*name.value().empty() == false*/)
			record.skipNunBytes(1);

		record.skipNunBytes(macrosSize);

		LPAnsiStringNoCch text(cchText);
		record >> text;
		
		rawText = text;

		record.skipNunBytes(mnLinkSize ); 
		
		if (record.getRdPtr() % 2 != 0)
		{
			record.skipNunBytes( 1 );
		}

		TxOruns.m_runCount = cbRuns / 8 - 1;
		TxOruns.load(record);	
	}
	else
	{
		record >> flags;
		record >> rot;
		record.skipNunBytes(6);	// reserved4 + reserved5
		
		//record >> controlInfo;	// The field MUST exist if and only if the value of cmo.ot in the preceding Obj record is 0, 5, 7, 11, 12, or 14.
		record >> cchText;

		if (cchText != 0)
			record >> cbRuns;
		
		record >> ifntEmpty;
		fmla.load(record);
		
		std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

		int sz = cchText;
		if ( cbRuns )
		{	
			std::wstring result;
			
			if(record.getDataSize() - record.getRdPtr() > 0)
			{
				XLUnicodeStringNoCch val;
				
				unsigned char fHighByte;	//todooo втащить в BufferString
				record >> fHighByte;
				bool is_wide = ((fHighByte & 1) != 0);

				size_t cch = record.getDataSize() - record.getRdPtr();
				if (is_wide) cch = cch >> 1;

				val.load(record, cch, is_wide);
				result += val.value();
			}

			while (result.length() < cchText && !recs.empty())
			{
				XLUnicodeStringNoCch val;
				
				unsigned char fHighByte;
				(*recs.front()) >> fHighByte;
				bool is_wide = ((fHighByte & 1) != 0);

				size_t cch = recs.front()->getDataSize() - 1;
				if (is_wide) cch = cch >> 1;

				val.load(*recs.front(), cch, is_wide);
				result += val.value();

				recs.pop_front();
			}

			rawText = result;

			TxOruns.set_records(&recs);

			TxOruns.m_runCount = cbRuns / 8 - 1;
			TxOruns.load(record);
		}

		while( !recs.empty() )		
		{
			sp_enabled	= true;
			mso_drawing_->storeRecordAndDecideProceeding(recs.front());
			recs.pop_front();
		}
	}
	
	hAlignment	= GETBITS(flags, 1, 3);
	vAlignment	= GETBITS(flags, 4, 6);	// reserved2 (2 bits)	
	
	fLockText	= GETBIT(flags, 9);	// reserved3 (4 bits)	
	fJustLast	= GETBIT(flags, 14);
	fSecretEdit = GETBIT(flags, 15);

	//if (false == record.isEOF())
	//{
	//	int sz = record.getDataSize() - record.getRdPtr();
	//	BYTE *last = new BYTE[sz];
	//	memcpy(last, record.getCurData<BYTE>(), sz);
	//	delete []last;
	//}
}

int TxO::serialize_vml (std::wostream & _stream)
{
	std::wstring str_ = rawText.value();
	int str_size = str_.size();

	int iFnt = 0; 
	
	CP_XML_WRITER(_stream)    
	{
		for (size_t i = 0; i < TxOruns.rgTxoRuns.size(); i++)
		{
			Run *run = dynamic_cast<Run*>(TxOruns.rgTxoRuns[i].get());
			if (run == NULL) continue;

			int end_string = str_size;

			if ( i < TxOruns.rgTxoRuns.size() - 1)
			{
				Run *run_next = dynamic_cast<Run*>(TxOruns.rgTxoRuns[i+1].get());
				if (run_next)
					end_string = run_next->formatRun.ich;
			}

			CP_XML_NODE(L"div")
			{
				//style='text-align:left'
				CP_XML_NODE(L"font")
				{
					iFnt = run->formatRun.ifnt;
					Font *font = NULL;
					if (iFnt >= 0 && iFnt < global_info->m_arFonts.size())
					{
						font = dynamic_cast<Font *>(global_info->m_arFonts[iFnt].get());
					}
					if (font)
					{
						if (font->dyHeight > 0)	CP_XML_ATTR(L"size", font->dyHeight);
						if (font->bls == 700)	CP_XML_ATTR(L"bold", true);
						if (font->fItalic)		CP_XML_ATTR(L"italic", true);
                        if (!font->fontName.empty())
						{
                            CP_XML_ATTR(L"face", font->fontName);
						}
						if ( font->icv < 0x7fff )
						{
						}
						else CP_XML_ATTR(L"color", L"auto");

					}

					if (run->formatRun.ich > str_.length())
					{
						//ошибка
						run->formatRun.ich = 0;
					}

					std::wstring str_part = str_.substr( run->formatRun.ich, end_string - run->formatRun.ich);

					CP_XML_STREAM() << XmlUtils::EncodeXmlString(str_part);
				}
			}
		}
	}
	return 0;
}

int TxO::serialize (std::wostream & _stream)
{
	std::wstring str_ = rawText.value();
	int str_size = str_.size();

	int Fmt = 0; 
	
	std::wstring namespace_ = L"a:";
	oox::external_items::Type type = global_info->xls_converter->xlsx_context->get_drawing_context().getType();

	if (type == oox::external_items::typeComment) 
		namespace_.clear();
	
	CP_XML_WRITER(_stream)    
	{
		for (size_t i = 0 ; i < TxOruns.rgTxoRuns.size(); i++)
		{
			Run *run = dynamic_cast<Run*>(TxOruns.rgTxoRuns[i].get());
			if (run == NULL) continue;

			int end_string = str_size;

			if ( i < TxOruns.rgTxoRuns.size() - 1)
			{
				Run *run_next = dynamic_cast<Run*>(TxOruns.rgTxoRuns[i+1].get());
				if (run_next)
					end_string = run_next->formatRun.ich;
			}

			CP_XML_NODE(namespace_ + L"r")
			{
				Fmt = run->formatRun.ifnt;
				
				serialize_rPr(CP_XML_STREAM(), Fmt, namespace_);

				CP_XML_NODE(namespace_ + L"t")
				{
					if (preserve_enabled)
					{
						CP_XML_ATTR(L"xml:space", L"preserve");
					}

					if (run->formatRun.ich > str_.length())
					{
						//ошибка
						run->formatRun.ich = 0;
					}

					std::wstring str_part = str_.substr( run->formatRun.ich, end_string - run->formatRun.ich);

					CP_XML_STREAM() << XmlUtils::EncodeXmlString(str_part);
				}
			}
		}
	}
	
	return 0;
}
int TxO::serialize_rPr	(std::wostream & _stream, int iFmt, std::wstring namespace_)
{
	if (!global_info)			return 0;

	int sz = global_info->m_arFonts.size();
	if (iFmt - 1 >= sz || iFmt < 1) return 0;

	Font * font = dynamic_cast<Font*>(global_info->m_arFonts[iFmt-1].get());

	if (!font) return 0;

	if (namespace_.empty())
	{
		CP_XML_WRITER(_stream)    
		{	
			CP_XML_NODE( L"rPr")
			{
				font->serialize_properties(CP_XML_STREAM(), true);
			}
		}
	}
	else
		font->serialize_rPr(_stream, false, false, namespace_);
	
	return 0;
}

} // namespace XLS


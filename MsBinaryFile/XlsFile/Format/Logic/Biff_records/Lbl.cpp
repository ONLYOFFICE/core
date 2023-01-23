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

#include "Lbl.h"

namespace XLS
{

const wchar_t* const AutoFilterDefineNames[] =
{
    L"_FilterDatabase",			// English
    L"_FilterDatenbank",		// German
    L"_ФильтрБазыДанных"		// Russian

};

Lbl::Lbl() : rgce(false), fGrp(0)
{
}
Lbl::~Lbl()
{
}
BaseObjectPtr Lbl::clone()
{
	return BaseObjectPtr(new Lbl(*this));
}
void Lbl::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        unsigned short flags;
        record >> flags >> chKey;

        fHidden			= GETBIT(flags, 0);
        fFunc			= GETBIT(flags, 1);
        fOB				= GETBIT(flags, 2);
        fProc			= GETBIT(flags, 3);
        fCalcExp		= GETBIT(flags, 4);
        fBuiltin		= GETBIT(flags, 5);
        fGrp			= GETBITS(flags, 6, 11);
        fPublished		= GETBIT(flags, 13);
        fWorkbookParam	= GETBIT(flags, 14);

        unsigned char cch;
        unsigned short cce;
        record >> cch >> cce;

		_UINT16	itab_2b = 0;
		if (record.getGlobalWorkbookInfo()->Version == 0x500)
		{
			_UINT16	ixti_2b = 0;
			record >> itab_2b;
			record >> ixti_2b;
			
			itab = itab_2b;
		}
		else
		{
			record.skipNunBytes(2);
			record >> itab_2b;
			
			itab = itab_2b;
		}
        record.skipNunBytes(4); 

        if (record.getGlobalWorkbookInfo()->Version < 0x600)
        {
            LPAnsiStringNoCch s;
            s.setSize(cch);
            record >> s;
            Name_bin = s;
        }
        else
        {
            Name_bin.setSize(cch); // this is to process built-in string values
            record >> Name_bin;
        }

        rgce.load(record, cce);

        if(!fBuiltin)
        {
            Name = Name_bin.getEscaped_ST_Xstring();

            for (int i = 0 ; i < 3; i++)
            {
                int res = Name_bin.value().find(AutoFilterDefineNames[i]);
                if (res >=0)
                {
                    Name = Name_bin = (L"_xlnm._FilterDatabase");
                    break;
                }
            }
        }
        else
        {
            unsigned char index = (static_cast<std::wstring>(Name_bin)).c_str()[0];
            switch(index)
            {
                case 0x00:
                    Name =  (L"_xlnm.Consolidate_Area");
                    break;
                case 0x01:
                    Name =  (L"_xlnm.Auto_Open");
                    break;
                case 0x02:
                    Name =  (L"_xlnm.Auto_Close");
                    break;
                case 0x03:
                    Name =  (L"_xlnm.Extract");
                    break;
                case 0x04:
                    Name =  (L"_xlnm.Database");
                    break;
                case 0x05:
                    Name =  (L"_xlnm.Criteria");
                    break;
                case 0x06:
                    Name =  (L"_xlnm.Print_Area");
                    break;
                case 0x07:
                    Name =  (L"_xlnm.Print_Titles");
                    break;
                case 0x08:
                    Name =  (L"_xlnm.Recorder");
                    break;
                case 0x09:
                    Name =  (L"_xlnm.Data_Form");
                    break;
                case 0x0A:
                    Name =  (L"_xlnm.Auto_Activate");
                    break;
                case 0x0B:
                    Name =  (L"_xlnm.Auto_Deactivate");
                    break;
                case 0x0C:
                    Name =  (L"_xlnm.Sheet_Title");
                    break;
                case 0x0D:
                    Name =  (L"_xlnm._FilterDatabase");
                    break;
                default:
                    Name;
                    break;
                    // EXCEPT::RT::WrongBiffRecord(L"Unsupported value of built-in name.", record.getTypeString());
            }
        }
    }
    else
    {
        unsigned int flags;
        record >> flags;

        fHidden			= GETBIT(flags, 0);
        fFunc			= GETBIT(flags, 1);
        fOB				= GETBIT(flags, 2);
        fProc			= GETBIT(flags, 3);
        fCalcExp		= GETBIT(flags, 4);
        fBuiltin		= GETBIT(flags, 5);
        fGrp			= GETBITS(flags, 6, 14);
        fPublished		= GETBIT(flags, 15);
        fWorkbookParam	= GETBIT(flags, 16);
        fFutureFunction	= GETBIT(flags, 17);

        record >> chKey >> itab;
        record >> name;
        rgce.load(record);
        record >> comment;
        if(fProc)
        {
            record >> unusedstring1;
            record >> description;
            record >> helpTopic;
            record >> unusedstring2;
        }
    }
}

Lbl_BIFF34::Lbl_BIFF34()
{
}

Lbl_BIFF34::~Lbl_BIFF34()
{
}
BaseObjectPtr Lbl_BIFF34::clone()
{
	return BaseObjectPtr(new Lbl_BIFF34(*this));
}
} // namespace XLS


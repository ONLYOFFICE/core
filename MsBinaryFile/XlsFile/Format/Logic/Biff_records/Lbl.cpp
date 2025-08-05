/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
        record.skipNunBytes(2);
        _UINT16	itab_2b;
        record >> itab_2b;
        itab = itab_2b;

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
		_UINT32 flags;
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

void Lbl::writeFields(CFRecord& record)
{
	unsigned short flags;

	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
        unsigned short flags = 0;

        SETBIT(flags, 0, fHidden)
        SETBIT(flags, 1, fFunc)
        SETBIT(flags, 2, fOB)
        SETBIT(flags, 3, fProc)
        SETBIT(flags, 4, fCalcExp)
        SETBIT(flags, 5, fBuiltin)
        SETBITS(flags, 6, 11, fGrp)
        SETBIT(flags, 13, fPublished)
        SETBIT(flags, 14, fWorkbookParam)

        unsigned char cch = Name_bin.getSize();
		if(!cch)
			Name_bin = L"";
        record << flags << chKey << cch;
        auto ccePos = record.getRdPtr();
        record.reserveNunBytes(4);

        _UINT16	itab_2b = itab;
        record << itab_2b;
        record.reserveNunBytes(4);

		record << Name_bin;
        auto rgceStart = record.getRdPtr();
        rgce.save(record);

        unsigned short cce = record.getRdPtr() - rgceStart;
        record.RollRdPtrBack(record.getRdPtr() - ccePos);
        record << cce;
        record.skipNunBytes((rgceStart - record.getRdPtr()) + cce);
	}

	else
	{
		_UINT32 flags = 0;

		SETBIT(flags, 0, fHidden)
		SETBIT(flags, 1, fFunc)
		SETBIT(flags, 2, fOB)
		SETBIT(flags, 3, fProc)
		SETBIT(flags, 4, fCalcExp)
		SETBIT(flags, 5, fBuiltin)
		SETBITS(flags, 6, 14, fGrp)
		SETBIT(flags, 15, fPublished)
		SETBIT(flags, 16, fWorkbookParam)
		SETBIT(flags, 17, fFutureFunction)

		record << flags;

		record << chKey << itab;
		record << name;


       /* std::vector<std::wstring>inputs {
                L"1E+10+3+5",
                L"3 * 4 + 5",
                L"50",
                L"1+1",
                L"SUM(1,2,3,4)",
                L"IF(P5=1.0,\"NA\",IF(P5=2.0,\"A\",IF(P5=3.0,\"B\",IF(P5=4.0,\"C\",IF(P5=5.0,\"D\",IF(P5=6.0,\"E\",IF(P5=7.0,\"F\",IF(P5=8.0,\"G\"))))))))",
                L"SUM(123 + SUM(456) + (45DATE(2002,1,6),0,IF(ISERROR(R[41]C[2]),0,IF(R13C3>=R[41]C[2],0, IF(AND(R[23]C[11]>=55,R[24]C[11]>=20),R53C3,0))))",
                L"$A1",
                L"$B$2",
                L"SUM(B5:B15)",
                L"SUM(B5:B15,D5:D15)",
                L"SUM(B5:B15 A7:D7)",
                L"SUM(sheet1!$A$1:$B$2)",
                L"[data.xls]sheet1!$A$1",
                L"SUM((A:A 1:1))",
                L"SUM((A:A,1:1))",
                L"SUM((A:A A1:B1))",
                L"SUM(D9:D11,E9:E11,F9:F11)",
                L"SUM((D9:D11,(E9:E11,F9:F11)))",
                L"{SUM(B2:D2*B3:D3)}",
                L"IF(R[39]C[11]>65,R[25]C[42],ROUND((R[11]C[11]*IF(OR(AND(R[39]C[11]>=55, "
                    "(R[40]C[11]>=20),AND(R[40]C[11]>=20,R11C3=\"YES\")),R[44]C[11],R[43]C[11]))+(R[14]C[11] "
                    "*IF(OR(AND(R[39]C[11]>=55,R[40]C[11]>=20),AND(R[40]C[11]>=20,R11C3=\"YES\")), "
                    "R[45]C[11],R[43]C[11])),0))"
            };
            std::vector<std::wstring> outputs;
            for (const auto& input: inputs)
            {
                NameParsedFormula rg(false);
                rg.parseStringFormula(input, L"");
                auto assem = rg.getAssembledFormula();
                outputs.push_back(assem);

            }

*/
		//NameParsedFormula example(false);
		//example = std::wstring(L"3 * 4 + 5");
		//example.save(record);

        rgce.save(record);
		record << comment;
		if (fProc)
		{
			record << unusedstring1;
			record << description;
			record << helpTopic;
			record << unusedstring2;
		}
	}
}


} // namespace XLS


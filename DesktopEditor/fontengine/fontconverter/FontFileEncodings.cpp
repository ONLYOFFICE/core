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
#include "FontFileEncodings.h"
#include "../../common/File.h"

namespace NSFontConverter
{
    static int Type1NameToUnicode_A(CWCharWrapper &wsName)
    {
        if      ( L"A" == wsName ) return 0x0041;
        else if ( L"AE" == wsName ) return 0x00C6;
        else if ( L"AEacute" == wsName ) return 0x01FC;
        else if ( L"AEmacron" == wsName ) return 0x01E2;
        else if ( L"AEsmall" == wsName ) return 0xF7E6;
        else if ( L"Aacute" == wsName ) return 0x00C1;
        else if ( L"Aacutesmall" == wsName ) return 0xF7E1;
        else if ( L"Abreve" == wsName ) return 0x0102;
        else if ( L"Abreveacute" == wsName ) return 0x1EAE;
        else if ( L"Abrevecyrillic" == wsName ) return 0x04D0;
        else if ( L"Abrevedotbelow" == wsName ) return 0x1EB6;
        else if ( L"Abrevegrave" == wsName ) return 0x1EB0;
        else if ( L"Abrevehookabove" == wsName ) return 0x1EB2;
        else if ( L"Abrevetilde" == wsName ) return 0x1EB4;
        else if ( L"Acaron" == wsName ) return 0x01CD;
        else if ( L"Acircle" == wsName ) return 0x24B6;
        else if ( L"Acircumflex" == wsName ) return 0x00C2;
        else if ( L"Acircumflexacute" == wsName ) return 0x1EA4;
        else if ( L"Acircumflexdotbelow" == wsName ) return 0x1EAC;
        else if ( L"Acircumflexgrave" == wsName ) return 0x1EA6;
        else if ( L"Acircumflexhookabove" == wsName ) return 0x1EA8;
        else if ( L"Acircumflexsmall" == wsName ) return 0xF7E2;
        else if ( L"Acircumflextilde" == wsName ) return 0x1EAA;
        else if ( L"Acute" == wsName ) return 0xF6C9;
        else if ( L"Acutesmall" == wsName ) return 0xF7B4;
        else if ( L"Acyrillic" == wsName ) return 0x0410;
        else if ( L"Adblgrave" == wsName ) return 0x0200;
        else if ( L"Adieresis" == wsName ) return 0x00C4;
        else if ( L"Adieresiscyrillic" == wsName ) return 0x04D2;
        else if ( L"Adieresismacron" == wsName ) return 0x01DE;
        else if ( L"Adieresissmall" == wsName ) return 0xF7E4;
        else if ( L"Adotbelow" == wsName ) return 0x1EA0;
        else if ( L"Adotmacron" == wsName ) return 0x01E0;
        else if ( L"Agrave" == wsName ) return 0x00C0;
        else if ( L"Agravesmall" == wsName ) return 0xF7E0;
        else if ( L"Ahookabove" == wsName ) return 0x1EA2;
        else if ( L"Aiecyrillic" == wsName ) return 0x04D4;
        else if ( L"Ainvertedbreve" == wsName ) return 0x0202;
        else if ( L"Alpha" == wsName ) return 0x0391;
        else if ( L"Alphatonos" == wsName ) return 0x0386;
        else if ( L"Amacron" == wsName ) return 0x0100;
        else if ( L"Amonospace" == wsName ) return 0xFF21;
        else if ( L"Aogonek" == wsName ) return 0x0104;
        else if ( L"Aring" == wsName ) return 0x00C5;
        else if ( L"Aringacute" == wsName ) return 0x01FA;
        else if ( L"Aringbelow" == wsName ) return 0x1E00;
        else if ( L"Aringsmall" == wsName ) return 0xF7E5;
        else if ( L"Asmall" == wsName ) return 0xF761;
        else if ( L"Atilde" == wsName ) return 0x00C3;
        else if ( L"Atildesmall" == wsName ) return 0xF7E3;
        else if ( L"Aybarmenian" == wsName ) return 0x0531;

        return 0;
    }

    static int Type1NameToUnicode_B(CWCharWrapper &wsName)
    {
        if      ( L"B" == wsName ) return 0x0042;
        else if ( L"Bcircle" == wsName ) return 0x24B7;
        else if ( L"Bdotaccent" == wsName ) return 0x1E02;
        else if ( L"Bdotbelow" == wsName ) return 0x1E04;
        else if ( L"Becyrillic" == wsName ) return 0x0411;
        else if ( L"Benarmenian" == wsName ) return 0x0532;
        else if ( L"Beta" == wsName ) return 0x0392;
        else if ( L"Bhook" == wsName ) return 0x0181;
        else if ( L"Blinebelow" == wsName ) return 0x1E06;
        else if ( L"Bmonospace" == wsName ) return 0xFF22;
        else if ( L"Brevesmall" == wsName ) return 0xF6F4;
        else if ( L"Bsmall" == wsName ) return 0xF762;
        else if ( L"Btopbar" == wsName ) return 0x0182;

        return 0;
    }
    static int Type1NameToUnicode_C(CWCharWrapper &wsName)
    {
        if      ( L"C" == wsName ) return 0x0043;
        else if ( L"Caarmenian" == wsName ) return 0x053E;
        else if ( L"Cacute" == wsName ) return 0x0106;
        else if ( L"Caron" == wsName ) return 0xF6CA;
        else if ( L"Caronsmall" == wsName ) return 0xF6F5;
        else if ( L"Ccaron" == wsName ) return 0x010C;
        else if ( L"Ccedilla" == wsName ) return 0x00C7;
        else if ( L"Ccedillaacute" == wsName ) return 0x1E08;
        else if ( L"Ccedillasmall" == wsName ) return 0xF7E7;
        else if ( L"Ccircle" == wsName ) return 0x24B8;
        else if ( L"Ccircumflex" == wsName ) return 0x0108;
        else if ( L"Cdot" == wsName ) return 0x010A;
        else if ( L"Cdotaccent" == wsName ) return 0x010A;
        else if ( L"Cedillasmall" == wsName ) return 0xF7B8;
        else if ( L"Chaarmenian" == wsName ) return 0x0549;
        else if ( L"Cheabkhasiancyrillic" == wsName ) return 0x04BC;
        else if ( L"Checyrillic" == wsName ) return 0x0427;
        else if ( L"Chedescenderabkhasiancyrillic" == wsName ) return 0x04BE;
        else if ( L"Chedescendercyrillic" == wsName ) return 0x04B6;
        else if ( L"Chedieresiscyrillic" == wsName ) return 0x04F4;
        else if ( L"Cheharmenian" == wsName ) return 0x0543;
        else if ( L"Chekhakassiancyrillic" == wsName ) return 0x04CB;
        else if ( L"Cheverticalstrokecyrillic" == wsName ) return 0x04B8;
        else if ( L"Chi" == wsName ) return 0x03A7;
        else if ( L"Chook" == wsName ) return 0x0187;
        else if ( L"Circumflexsmall" == wsName ) return 0xF6F6;
        else if ( L"Cmonospace" == wsName ) return 0xFF23;
        else if ( L"Coarmenian" == wsName ) return 0x0551;
        else if ( L"Csmall" == wsName ) return 0xF763;

        return 0;
    }

    static int Type1NameToUnicode_D(CWCharWrapper &wsName)
    {
        if      ( L"D" == wsName ) return 0x0044;
        else if ( L"DZ" == wsName ) return 0x01F1;
        else if ( L"DZcaron" == wsName ) return 0x01C4;
        else if ( L"Daarmenian" == wsName ) return 0x0534;
        else if ( L"Dafrican" == wsName ) return 0x0189;
        else if ( L"Dcaron" == wsName ) return 0x010E;
        else if ( L"Dcedilla" == wsName ) return 0x1E10;
        else if ( L"Dcircle" == wsName ) return 0x24B9;
        else if ( L"Dcircumflexbelow" == wsName ) return 0x1E12;
        else if ( L"Dcroat" == wsName ) return 0x0110;
        else if ( L"Ddotaccent" == wsName ) return 0x1E0A;
        else if ( L"Ddotbelow" == wsName ) return 0x1E0C;
        else if ( L"Decyrillic" == wsName ) return 0x0414;
        else if ( L"Deicoptic" == wsName ) return 0x03EE;
        else if ( L"Delta" == wsName ) return 0x2206;
        else if ( L"Deltagreek" == wsName ) return 0x0394;
        else if ( L"Dhook" == wsName ) return 0x018A;
        else if ( L"Dieresis" == wsName ) return 0xF6CB;
        else if ( L"DieresisAcute" == wsName ) return 0xF6CC;
        else if ( L"DieresisGrave" == wsName ) return 0xF6CD;
        else if ( L"Dieresissmall" == wsName ) return 0xF7A8;
        else if ( L"Digammagreek" == wsName ) return 0x03DC;
        else if ( L"Djecyrillic" == wsName ) return 0x0402;
        else if ( L"Dlinebelow" == wsName ) return 0x1E0E;
        else if ( L"Dmonospace" == wsName ) return 0xFF24;
        else if ( L"Dotaccentsmall" == wsName ) return 0xF6F7;
        else if ( L"Dslash" == wsName ) return 0x0110;
        else if ( L"Dsmall" == wsName ) return 0xF764;
        else if ( L"Dtopbar" == wsName ) return 0x018B;
        else if ( L"Dz" == wsName ) return 0x01F2;
        else if ( L"Dzcaron" == wsName ) return 0x01C5;
        else if ( L"Dzeabkhasiancyrillic" == wsName ) return 0x04E0;
        else if ( L"Dzecyrillic" == wsName ) return 0x0405;
        else if ( L"Dzhecyrillic" == wsName ) return 0x040F;

        return 0;
    }
    static int Type1NameToUnicode_E(CWCharWrapper &wsName)
    {
        if      ( L"E" == wsName ) return 0x0045;
        else if ( L"Eacute" == wsName ) return 0x00C9;
        else if ( L"Eacutesmall" == wsName ) return 0xF7E9;
        else if ( L"Ebreve" == wsName ) return 0x0114;
        else if ( L"Ecaron" == wsName ) return 0x011A;
        else if ( L"Ecedillabreve" == wsName ) return 0x1E1C;
        else if ( L"Echarmenian" == wsName ) return 0x0535;
        else if ( L"Ecircle" == wsName ) return 0x24BA;
        else if ( L"Ecircumflex" == wsName ) return 0x00CA;
        else if ( L"Ecircumflexacute" == wsName ) return 0x1EBE;
        else if ( L"Ecircumflexbelow" == wsName ) return 0x1E18;
        else if ( L"Ecircumflexdotbelow" == wsName ) return 0x1EC6;
        else if ( L"Ecircumflexgrave" == wsName ) return 0x1EC0;
        else if ( L"Ecircumflexhookabove" == wsName ) return 0x1EC2;
        else if ( L"Ecircumflexsmall" == wsName ) return 0xF7EA;
        else if ( L"Ecircumflextilde" == wsName ) return 0x1EC4;
        else if ( L"Ecyrillic" == wsName ) return 0x0404;
        else if ( L"Edblgrave" == wsName ) return 0x0204;
        else if ( L"Edieresis" == wsName ) return 0x00CB;
        else if ( L"Edieresissmall" == wsName ) return 0xF7EB;
        else if ( L"Edot" == wsName ) return 0x0116;
        else if ( L"Edotaccent" == wsName ) return 0x0116;
        else if ( L"Edotbelow" == wsName ) return 0x1EB8;
        else if ( L"Efcyrillic" == wsName ) return 0x0424;
        else if ( L"Egrave" == wsName ) return 0x00C8;
        else if ( L"Egravesmall" == wsName ) return 0xF7E8;
        else if ( L"Eharmenian" == wsName ) return 0x0537;
        else if ( L"Ehookabove" == wsName ) return 0x1EBA;
        else if ( L"Eightroman" == wsName ) return 0x2167;
        else if ( L"Einvertedbreve" == wsName ) return 0x0206;
        else if ( L"Eiotifiedcyrillic" == wsName ) return 0x0464;
        else if ( L"Elcyrillic" == wsName ) return 0x041B;
        else if ( L"Elevenroman" == wsName ) return 0x216A;
        else if ( L"Emacron" == wsName ) return 0x0112;
        else if ( L"Emacronacute" == wsName ) return 0x1E16;
        else if ( L"Emacrongrave" == wsName ) return 0x1E14;
        else if ( L"Emcyrillic" == wsName ) return 0x041C;
        else if ( L"Emonospace" == wsName ) return 0xFF25;
        else if ( L"Encyrillic" == wsName ) return 0x041D;
        else if ( L"Endescendercyrillic" == wsName ) return 0x04A2;
        else if ( L"Eng" == wsName ) return 0x014A;
        else if ( L"Enghecyrillic" == wsName ) return 0x04A4;
        else if ( L"Enhookcyrillic" == wsName ) return 0x04C7;
        else if ( L"Eogonek" == wsName ) return 0x0118;
        else if ( L"Eopen" == wsName ) return 0x0190;
        else if ( L"Epsilon" == wsName ) return 0x0395;
        else if ( L"Epsilontonos" == wsName ) return 0x0388;
        else if ( L"Ercyrillic" == wsName ) return 0x0420;
        else if ( L"Ereversed" == wsName ) return 0x018E;
        else if ( L"Ereversedcyrillic" == wsName ) return 0x042D;
        else if ( L"Escyrillic" == wsName ) return 0x0421;
        else if ( L"Esdescendercyrillic" == wsName ) return 0x04AA;
        else if ( L"Esh" == wsName ) return 0x01A9;
        else if ( L"Esmall" == wsName ) return 0xF765;
        else if ( L"Eta" == wsName ) return 0x0397;
        else if ( L"Etarmenian" == wsName ) return 0x0538;
        else if ( L"Etatonos" == wsName ) return 0x0389;
        else if ( L"Eth" == wsName ) return 0x00D0;
        else if ( L"Ethsmall" == wsName ) return 0xF7F0;
        else if ( L"Etilde" == wsName ) return 0x1EBC;
        else if ( L"Etildebelow" == wsName ) return 0x1E1A;
        else if ( L"Euro" == wsName ) return 0x20AC;
        else if ( L"Ezh" == wsName ) return 0x01B7;
        else if ( L"Ezhcaron" == wsName ) return 0x01EE;
        else if ( L"Ezhreversed" == wsName ) return 0x01B8;

        return 0;
    }
    static int Type1NameToUnicode_F(CWCharWrapper &wsName)
    {
        if      ( L"F" == wsName ) return 0x0046;
        else if ( L"Fcircle" == wsName ) return 0x24BB;
        else if ( L"Fdotaccent" == wsName ) return 0x1E1E;
        else if ( L"Feharmenian" == wsName ) return 0x0556;
        else if ( L"Feicoptic" == wsName ) return 0x03E4;
        else if ( L"Fhook" == wsName ) return 0x0191;
        else if ( L"Fitacyrillic" == wsName ) return 0x0472;
        else if ( L"Fiveroman" == wsName ) return 0x2164;
        else if ( L"Fmonospace" == wsName ) return 0xFF26;
        else if ( L"Fourroman" == wsName ) return 0x2163;
        else if ( L"Fsmall" == wsName ) return 0xF766;

        return 0;
    }
    static int Type1NameToUnicode_G(CWCharWrapper &wsName)
    {
        if      ( L"G" == wsName ) return 0x0047;
        else if ( L"GBsquare" == wsName ) return 0x3387;
        else if ( L"Gacute" == wsName ) return 0x01F4;
        else if ( L"Gamma" == wsName ) return 0x0393;
        else if ( L"Gammaafrican" == wsName ) return 0x0194;
        else if ( L"Gangiacoptic" == wsName ) return 0x03EA;
        else if ( L"Gbreve" == wsName ) return 0x011E;
        else if ( L"Gcaron" == wsName ) return 0x01E6;
        else if ( L"Gcedilla" == wsName ) return 0x0122;
        else if ( L"Gcircle" == wsName ) return 0x24BC;
        else if ( L"Gcircumflex" == wsName ) return 0x011C;
        else if ( L"Gcommaaccent" == wsName ) return 0x0122;
        else if ( L"Gdot" == wsName ) return 0x0120;
        else if ( L"Gdotaccent" == wsName ) return 0x0120;
        else if ( L"Gecyrillic" == wsName ) return 0x0413;
        else if ( L"Ghadarmenian" == wsName ) return 0x0542;
        else if ( L"Ghemiddlehookcyrillic" == wsName ) return 0x0494;
        else if ( L"Ghestrokecyrillic" == wsName ) return 0x0492;
        else if ( L"Gheupturncyrillic" == wsName ) return 0x0490;
        else if ( L"Ghook" == wsName ) return 0x0193;
        else if ( L"Gimarmenian" == wsName ) return 0x0533;
        else if ( L"Gjecyrillic" == wsName ) return 0x0403;
        else if ( L"Gmacron" == wsName ) return 0x1E20;
        else if ( L"Gmonospace" == wsName ) return 0xFF27;
        else if ( L"Grave" == wsName ) return 0xF6CE;
        else if ( L"Gravesmall" == wsName ) return 0xF760;
        else if ( L"Gsmall" == wsName ) return 0xF767;
        else if ( L"Gsmallhook" == wsName ) return 0x029B;
        else if ( L"Gstroke" == wsName ) return 0x01E4;

        return 0;
    }
    static int Type1NameToUnicode_H(CWCharWrapper &wsName)
    {
        if      ( L"H" == wsName ) return 0x0048;
        else if ( L"H18533" == wsName ) return 0x25CF;
        else if ( L"H18543" == wsName ) return 0x25AA;
        else if ( L"H18551" == wsName ) return 0x25AB;
        else if ( L"H22073" == wsName ) return 0x25A1;
        else if ( L"HPsquare" == wsName ) return 0x33CB;
        else if ( L"Haabkhasiancyrillic" == wsName ) return 0x04A8;
        else if ( L"Hadescendercyrillic" == wsName ) return 0x04B2;
        else if ( L"Hardsigncyrillic" == wsName ) return 0x042A;
        else if ( L"Hbar" == wsName ) return 0x0126;
        else if ( L"Hbrevebelow" == wsName ) return 0x1E2A;
        else if ( L"Hcedilla" == wsName ) return 0x1E28;
        else if ( L"Hcircle" == wsName ) return 0x24BD;
        else if ( L"Hcircumflex" == wsName ) return 0x0124;
        else if ( L"Hdieresis" == wsName ) return 0x1E26;
        else if ( L"Hdotaccent" == wsName ) return 0x1E22;
        else if ( L"Hdotbelow" == wsName ) return 0x1E24;
        else if ( L"Hmonospace" == wsName ) return 0xFF28;
        else if ( L"Hoarmenian" == wsName ) return 0x0540;
        else if ( L"Horicoptic" == wsName ) return 0x03E8;
        else if ( L"Hsmall" == wsName ) return 0xF768;
        else if ( L"Hungarumlaut" == wsName ) return 0xF6CF;
        else if ( L"Hungarumlautsmall" == wsName ) return 0xF6F8;
        else if ( L"Hzsquare" == wsName ) return 0x3390;

        return 0;
    }
    static int Type1NameToUnicode_I(CWCharWrapper &wsName)
    {
        if      ( L"I" == wsName ) return 0x0049;
        else if ( L"IAcyrillic" == wsName ) return 0x042F;
        else if ( L"IJ" == wsName ) return 0x0132;
        else if ( L"IUcyrillic" == wsName ) return 0x042E;
        else if ( L"Iacute" == wsName ) return 0x00CD;
        else if ( L"Iacutesmall" == wsName ) return 0xF7ED;
        else if ( L"Ibreve" == wsName ) return 0x012C;
        else if ( L"Icaron" == wsName ) return 0x01CF;
        else if ( L"Icircle" == wsName ) return 0x24BE;
        else if ( L"Icircumflex" == wsName ) return 0x00CE;
        else if ( L"Icircumflexsmall" == wsName ) return 0xF7EE;
        else if ( L"Icyrillic" == wsName ) return 0x0406;
        else if ( L"Idblgrave" == wsName ) return 0x0208;
        else if ( L"Idieresis" == wsName ) return 0x00CF;
        else if ( L"Idieresisacute" == wsName ) return 0x1E2E;
        else if ( L"Idieresiscyrillic" == wsName ) return 0x04E4;
        else if ( L"Idieresissmall" == wsName ) return 0xF7EF;
        else if ( L"Idot" == wsName ) return 0x0130;
        else if ( L"Idotaccent" == wsName ) return 0x0130;
        else if ( L"Idotbelow" == wsName ) return 0x1ECA;
        else if ( L"Iebrevecyrillic" == wsName ) return 0x04D6;
        else if ( L"Iecyrillic" == wsName ) return 0x0415;
        else if ( L"Ifraktur" == wsName ) return 0x2111;
        else if ( L"Igrave" == wsName ) return 0x00CC;
        else if ( L"Igravesmall" == wsName ) return 0xF7EC;
        else if ( L"Ihookabove" == wsName ) return 0x1EC8;
        else if ( L"Iicyrillic" == wsName ) return 0x0418;
        else if ( L"Iinvertedbreve" == wsName ) return 0x020A;
        else if ( L"Iishortcyrillic" == wsName ) return 0x0419;
        else if ( L"Imacron" == wsName ) return 0x012A;
        else if ( L"Imacroncyrillic" == wsName ) return 0x04E2;
        else if ( L"Imonospace" == wsName ) return 0xFF29;
        else if ( L"Iniarmenian" == wsName ) return 0x053B;
        else if ( L"Iocyrillic" == wsName ) return 0x0401;
        else if ( L"Iogonek" == wsName ) return 0x012E;
        else if ( L"Iota" == wsName ) return 0x0399;
        else if ( L"Iotaafrican" == wsName ) return 0x0196;
        else if ( L"Iotadieresis" == wsName ) return 0x03AA;
        else if ( L"Iotatonos" == wsName ) return 0x038A;
        else if ( L"Ismall" == wsName ) return 0xF769;
        else if ( L"Istroke" == wsName ) return 0x0197;
        else if ( L"Itilde" == wsName ) return 0x0128;
        else if ( L"Itildebelow" == wsName ) return 0x1E2C;
        else if ( L"Izhitsacyrillic" == wsName ) return 0x0474;
        else if ( L"Izhitsadblgravecyrillic" == wsName ) return 0x0476;

        return 0;
    }
    static int Type1NameToUnicode_J(CWCharWrapper &wsName)
    {
        if      ( L"J" == wsName ) return 0x004A;
        else if ( L"Jaarmenian" == wsName ) return 0x0541;
        else if ( L"Jcircle" == wsName ) return 0x24BF;
        else if ( L"Jcircumflex" == wsName ) return 0x0134;
        else if ( L"Jecyrillic" == wsName ) return 0x0408;
        else if ( L"Jheharmenian" == wsName ) return 0x054B;
        else if ( L"Jmonospace" == wsName ) return 0xFF2A;
        else if ( L"Jsmall" == wsName ) return 0xF76A;

        return 0;
    }
    static int Type1NameToUnicode_K(CWCharWrapper &wsName)
    {
        if      ( L"K" == wsName ) return 0x004B;
        else if ( L"KBsquare" == wsName ) return 0x3385;
        else if ( L"KKsquare" == wsName ) return 0x33CD;
        else if ( L"Kabashkircyrillic" == wsName ) return 0x04A0;
        else if ( L"Kacute" == wsName ) return 0x1E30;
        else if ( L"Kacyrillic" == wsName ) return 0x041A;
        else if ( L"Kadescendercyrillic" == wsName ) return 0x049A;
        else if ( L"Kahookcyrillic" == wsName ) return 0x04C3;
        else if ( L"Kappa" == wsName ) return 0x039A;
        else if ( L"Kastrokecyrillic" == wsName ) return 0x049E;
        else if ( L"Kaverticalstrokecyrillic" == wsName ) return 0x049C;
        else if ( L"Kcaron" == wsName ) return 0x01E8;
        else if ( L"Kcedilla" == wsName ) return 0x0136;
        else if ( L"Kcircle" == wsName ) return 0x24C0;
        else if ( L"Kcommaaccent" == wsName ) return 0x0136;
        else if ( L"Kdotbelow" == wsName ) return 0x1E32;
        else if ( L"Keharmenian" == wsName ) return 0x0554;
        else if ( L"Kenarmenian" == wsName ) return 0x053F;
        else if ( L"Khacyrillic" == wsName ) return 0x0425;
        else if ( L"Kheicoptic" == wsName ) return 0x03E6;
        else if ( L"Khook" == wsName ) return 0x0198;
        else if ( L"Kjecyrillic" == wsName ) return 0x040C;
        else if ( L"Klinebelow" == wsName ) return 0x1E34;
        else if ( L"Kmonospace" == wsName ) return 0xFF2B;
        else if ( L"Koppacyrillic" == wsName ) return 0x0480;
        else if ( L"Koppagreek" == wsName ) return 0x03DE;
        else if ( L"Ksicyrillic" == wsName ) return 0x046E;
        else if ( L"Ksmall" == wsName ) return 0xF76B;

        return 0;
    }
    static int Type1NameToUnicode_L(CWCharWrapper &wsName)
    {
        if      ( L"L" == wsName ) return 0x004C;
        else if ( L"LJ" == wsName ) return 0x01C7;
        else if ( L"LL" == wsName ) return 0xF6BF;
        else if ( L"Lacute" == wsName ) return 0x0139;
        else if ( L"Lambda" == wsName ) return 0x039B;
        else if ( L"Lcaron" == wsName ) return 0x013D;
        else if ( L"Lcedilla" == wsName ) return 0x013B;
        else if ( L"Lcircle" == wsName ) return 0x24C1;
        else if ( L"Lcircumflexbelow" == wsName ) return 0x1E3C;
        else if ( L"Lcommaaccent" == wsName ) return 0x013B;
        else if ( L"Ldot" == wsName ) return 0x013F;
        else if ( L"Ldotaccent" == wsName ) return 0x013F;
        else if ( L"Ldotbelow" == wsName ) return 0x1E36;
        else if ( L"Ldotbelowmacron" == wsName ) return 0x1E38;
        else if ( L"Liwnarmenian" == wsName ) return 0x053C;
        else if ( L"Lj" == wsName ) return 0x01C8;
        else if ( L"Ljecyrillic" == wsName ) return 0x0409;
        else if ( L"Llinebelow" == wsName ) return 0x1E3A;
        else if ( L"Lmonospace" == wsName ) return 0xFF2C;
        else if ( L"Lslash" == wsName ) return 0x0141;
        else if ( L"Lslashsmall" == wsName ) return 0xF6F9;
        else if ( L"Lsmall" == wsName ) return 0xF76C;

        return 0;
    }
    static int Type1NameToUnicode_M(CWCharWrapper &wsName)
    {
        if      ( L"M" == wsName ) return 0x004D;
        else if ( L"MBsquare" == wsName ) return 0x3386;
        else if ( L"Macron" == wsName ) return 0xF6D0;
        else if ( L"Macronsmall" == wsName ) return 0xF7AF;
        else if ( L"Macute" == wsName ) return 0x1E3E;
        else if ( L"Mcircle" == wsName ) return 0x24C2;
        else if ( L"Mdotaccent" == wsName ) return 0x1E40;
        else if ( L"Mdotbelow" == wsName ) return 0x1E42;
        else if ( L"Menarmenian" == wsName ) return 0x0544;
        else if ( L"Mmonospace" == wsName ) return 0xFF2D;
        else if ( L"Msmall" == wsName ) return 0xF76D;
        else if ( L"Mturned" == wsName ) return 0x019C;
        else if ( L"Mu" == wsName ) return 0x039C;

        return 0;
    }
    static int Type1NameToUnicode_N(CWCharWrapper &wsName)
    {
        if      ( L"N" == wsName ) return 0x004E;
        else if ( L"NJ" == wsName ) return 0x01CA;
        else if ( L"Nacute" == wsName ) return 0x0143;
        else if ( L"Ncaron" == wsName ) return 0x0147;
        else if ( L"Ncedilla" == wsName ) return 0x0145;
        else if ( L"Ncircle" == wsName ) return 0x24C3;
        else if ( L"Ncircumflexbelow" == wsName ) return 0x1E4A;
        else if ( L"Ncommaaccent" == wsName ) return 0x0145;
        else if ( L"Ndotaccent" == wsName ) return 0x1E44;
        else if ( L"Ndotbelow" == wsName ) return 0x1E46;
        else if ( L"Nhookleft" == wsName ) return 0x019D;
        else if ( L"Nineroman" == wsName ) return 0x2168;
        else if ( L"Nj" == wsName ) return 0x01CB;
        else if ( L"Njecyrillic" == wsName ) return 0x040A;
        else if ( L"Nlinebelow" == wsName ) return 0x1E48;
        else if ( L"Nmonospace" == wsName ) return 0xFF2E;
        else if ( L"Nowarmenian" == wsName ) return 0x0546;
        else if ( L"Nsmall" == wsName ) return 0xF76E;
        else if ( L"Ntilde" == wsName ) return 0x00D1;
        else if ( L"Ntildesmall" == wsName ) return 0xF7F1;
        else if ( L"Nu" == wsName ) return 0x039D;

        return 0;
    }
    static int Type1NameToUnicode_O(CWCharWrapper &wsName)
    {
        if      ( L"O" == wsName ) return 0x004F;
        else if ( L"OE" == wsName ) return 0x0152;
        else if ( L"OEsmall" == wsName ) return 0xF6FA;
        else if ( L"Oacute" == wsName ) return 0x00D3;
        else if ( L"Oacutesmall" == wsName ) return 0xF7F3;
        else if ( L"Obarredcyrillic" == wsName ) return 0x04E8;
        else if ( L"Obarreddieresiscyrillic" == wsName ) return 0x04EA;
        else if ( L"Obreve" == wsName ) return 0x014E;
        else if ( L"Ocaron" == wsName ) return 0x01D1;
        else if ( L"Ocenteredtilde" == wsName ) return 0x019F;
        else if ( L"Ocircle" == wsName ) return 0x24C4;
        else if ( L"Ocircumflex" == wsName ) return 0x00D4;
        else if ( L"Ocircumflexacute" == wsName ) return 0x1ED0;
        else if ( L"Ocircumflexdotbelow" == wsName ) return 0x1ED8;
        else if ( L"Ocircumflexgrave" == wsName ) return 0x1ED2;
        else if ( L"Ocircumflexhookabove" == wsName ) return 0x1ED4;
        else if ( L"Ocircumflexsmall" == wsName ) return 0xF7F4;
        else if ( L"Ocircumflextilde" == wsName ) return 0x1ED6;
        else if ( L"Ocyrillic" == wsName ) return 0x041E;
        else if ( L"Odblacute" == wsName ) return 0x0150;
        else if ( L"Odblgrave" == wsName ) return 0x020C;
        else if ( L"Odieresis" == wsName ) return 0x00D6;
        else if ( L"Odieresiscyrillic" == wsName ) return 0x04E6;
        else if ( L"Odieresissmall" == wsName ) return 0xF7F6;
        else if ( L"Odotbelow" == wsName ) return 0x1ECC;
        else if ( L"Ogoneksmall" == wsName ) return 0xF6FB;
        else if ( L"Ograve" == wsName ) return 0x00D2;
        else if ( L"Ogravesmall" == wsName ) return 0xF7F2;
        else if ( L"Oharmenian" == wsName ) return 0x0555;
        else if ( L"Ohm" == wsName ) return 0x2126;
        else if ( L"Ohookabove" == wsName ) return 0x1ECE;
        else if ( L"Ohorn" == wsName ) return 0x01A0;
        else if ( L"Ohornacute" == wsName ) return 0x1EDA;
        else if ( L"Ohorndotbelow" == wsName ) return 0x1EE2;
        else if ( L"Ohorngrave" == wsName ) return 0x1EDC;
        else if ( L"Ohornhookabove" == wsName ) return 0x1EDE;
        else if ( L"Ohorntilde" == wsName ) return 0x1EE0;
        else if ( L"Ohungarumlaut" == wsName ) return 0x0150;
        else if ( L"Oi" == wsName ) return 0x01A2;
        else if ( L"Oinvertedbreve" == wsName ) return 0x020E;
        else if ( L"Omacron" == wsName ) return 0x014C;
        else if ( L"Omacronacute" == wsName ) return 0x1E52;
        else if ( L"Omacrongrave" == wsName ) return 0x1E50;
        else if ( L"Omega" == wsName ) return 0x2126;
        else if ( L"Omegacyrillic" == wsName ) return 0x0460;
        else if ( L"Omegagreek" == wsName ) return 0x03A9;
        else if ( L"Omegaroundcyrillic" == wsName ) return 0x047A;
        else if ( L"Omegatitlocyrillic" == wsName ) return 0x047C;
        else if ( L"Omegatonos" == wsName ) return 0x038F;
        else if ( L"Omicron" == wsName ) return 0x039F;
        else if ( L"Omicrontonos" == wsName ) return 0x038C;
        else if ( L"Omonospace" == wsName ) return 0xFF2F;
        else if ( L"Oneroman" == wsName ) return 0x2160;
        else if ( L"Oogonek" == wsName ) return 0x01EA;
        else if ( L"Oogonekmacron" == wsName ) return 0x01EC;
        else if ( L"Oopen" == wsName ) return 0x0186;
        else if ( L"Oslash" == wsName ) return 0x00D8;
        else if ( L"Oslashacute" == wsName ) return 0x01FE;
        else if ( L"Oslashsmall" == wsName ) return 0xF7F8;
        else if ( L"Osmall" == wsName ) return 0xF76F;
        else if ( L"Ostrokeacute" == wsName ) return 0x01FE;
        else if ( L"Otcyrillic" == wsName ) return 0x047E;
        else if ( L"Otilde" == wsName ) return 0x00D5;
        else if ( L"Otildeacute" == wsName ) return 0x1E4C;
        else if ( L"Otildedieresis" == wsName ) return 0x1E4E;
        else if ( L"Otildesmall" == wsName ) return 0xF7F5;

        return 0;
    }
    static int Type1NameToUnicode_P(CWCharWrapper &wsName)
    {
        if      ( L"P" == wsName ) return 0x0050;
        else if ( L"Pacute" == wsName ) return 0x1E54;
        else if ( L"Pcircle" == wsName ) return 0x24C5;
        else if ( L"Pdotaccent" == wsName ) return 0x1E56;
        else if ( L"Pecyrillic" == wsName ) return 0x041F;
        else if ( L"Peharmenian" == wsName ) return 0x054A;
        else if ( L"Pemiddlehookcyrillic" == wsName ) return 0x04A6;
        else if ( L"Phi" == wsName ) return 0x03A6;
        else if ( L"Phook" == wsName ) return 0x01A4;
        else if ( L"Pi" == wsName ) return 0x03A0;
        else if ( L"Piwrarmenian" == wsName ) return 0x0553;
        else if ( L"Pmonospace" == wsName ) return 0xFF30;
        else if ( L"Psi" == wsName ) return 0x03A8;
        else if ( L"Psicyrillic" == wsName ) return 0x0470;
        else if ( L"Psmall" == wsName ) return 0xF770;

        return 0;
    }
    static int Type1NameToUnicode_Q(CWCharWrapper &wsName)
    {
        if      ( L"Q" == wsName ) return 0x0051;
        else if ( L"Qcircle" == wsName ) return 0x24C6;
        else if ( L"Qmonospace" == wsName ) return 0xFF31;
        else if ( L"Qsmall" == wsName ) return 0xF771;

        return 0;
    }
    static int Type1NameToUnicode_R(CWCharWrapper &wsName)
    {
        if      ( L"R" == wsName ) return 0x0052;
        else if ( L"Raarmenian" == wsName ) return 0x054C;
        else if ( L"Racute" == wsName ) return 0x0154;
        else if ( L"Rcaron" == wsName ) return 0x0158;
        else if ( L"Rcedilla" == wsName ) return 0x0156;
        else if ( L"Rcircle" == wsName ) return 0x24C7;
        else if ( L"Rcommaaccent" == wsName ) return 0x0156;
        else if ( L"Rdblgrave" == wsName ) return 0x0210;
        else if ( L"Rdotaccent" == wsName ) return 0x1E58;
        else if ( L"Rdotbelow" == wsName ) return 0x1E5A;
        else if ( L"Rdotbelowmacron" == wsName ) return 0x1E5C;
        else if ( L"Reharmenian" == wsName ) return 0x0550;
        else if ( L"Rfraktur" == wsName ) return 0x211C;
        else if ( L"Rho" == wsName ) return 0x03A1;
        else if ( L"Ringsmall" == wsName ) return 0xF6FC;
        else if ( L"Rinvertedbreve" == wsName ) return 0x0212;
        else if ( L"Rlinebelow" == wsName ) return 0x1E5E;
        else if ( L"Rmonospace" == wsName ) return 0xFF32;
        else if ( L"Rsmall" == wsName ) return 0xF772;
        else if ( L"Rsmallinverted" == wsName ) return 0x0281;
        else if ( L"Rsmallinvertedsuperior" == wsName ) return 0x02B6;

        return 0;
    }
    static int Type1NameToUnicode_S(CWCharWrapper &wsName)
    {
        if      ( L"S" == wsName ) return 0x0053;
        else if ( L"SF010000" == wsName ) return 0x250C;
        else if ( L"SF020000" == wsName ) return 0x2514;
        else if ( L"SF030000" == wsName ) return 0x2510;
        else if ( L"SF040000" == wsName ) return 0x2518;
        else if ( L"SF050000" == wsName ) return 0x253C;
        else if ( L"SF060000" == wsName ) return 0x252C;
        else if ( L"SF070000" == wsName ) return 0x2534;
        else if ( L"SF080000" == wsName ) return 0x251C;
        else if ( L"SF090000" == wsName ) return 0x2524;
        else if ( L"SF100000" == wsName ) return 0x2500;
        else if ( L"SF110000" == wsName ) return 0x2502;
        else if ( L"SF190000" == wsName ) return 0x2561;
        else if ( L"SF200000" == wsName ) return 0x2562;
        else if ( L"SF210000" == wsName ) return 0x2556;
        else if ( L"SF220000" == wsName ) return 0x2555;
        else if ( L"SF230000" == wsName ) return 0x2563;
        else if ( L"SF240000" == wsName ) return 0x2551;
        else if ( L"SF250000" == wsName ) return 0x2557;
        else if ( L"SF260000" == wsName ) return 0x255D;
        else if ( L"SF270000" == wsName ) return 0x255C;
        else if ( L"SF280000" == wsName ) return 0x255B;
        else if ( L"SF360000" == wsName ) return 0x255E;
        else if ( L"SF370000" == wsName ) return 0x255F;
        else if ( L"SF380000" == wsName ) return 0x255A;
        else if ( L"SF390000" == wsName ) return 0x2554;
        else if ( L"SF400000" == wsName ) return 0x2569;
        else if ( L"SF410000" == wsName ) return 0x2566;
        else if ( L"SF420000" == wsName ) return 0x2560;
        else if ( L"SF430000" == wsName ) return 0x2550;
        else if ( L"SF440000" == wsName ) return 0x256C;
        else if ( L"SF450000" == wsName ) return 0x2567;
        else if ( L"SF460000" == wsName ) return 0x2568;
        else if ( L"SF470000" == wsName ) return 0x2564;
        else if ( L"SF480000" == wsName ) return 0x2565;
        else if ( L"SF490000" == wsName ) return 0x2559;
        else if ( L"SF500000" == wsName ) return 0x2558;
        else if ( L"SF510000" == wsName ) return 0x2552;
        else if ( L"SF520000" == wsName ) return 0x2553;
        else if ( L"SF530000" == wsName ) return 0x256B;
        else if ( L"SF540000" == wsName ) return 0x256A;
        else if ( L"Sacute" == wsName ) return 0x015A;
        else if ( L"Sacutedotaccent" == wsName ) return 0x1E64;
        else if ( L"Sampigreek" == wsName ) return 0x03E0;
        else if ( L"Scaron" == wsName ) return 0x0160;
        else if ( L"Scarondotaccent" == wsName ) return 0x1E66;
        else if ( L"Scaronsmall" == wsName ) return 0xF6FD;
        else if ( L"Scedilla" == wsName ) return 0x015E;
        else if ( L"Schwa" == wsName ) return 0x018F;
        else if ( L"Schwacyrillic" == wsName ) return 0x04D8;
        else if ( L"Schwadieresiscyrillic" == wsName ) return 0x04DA;
        else if ( L"Scircle" == wsName ) return 0x24C8;
        else if ( L"Scircumflex" == wsName ) return 0x015C;
        else if ( L"Scommaaccent" == wsName ) return 0x0218;
        else if ( L"Sdotaccent" == wsName ) return 0x1E60;
        else if ( L"Sdotbelow" == wsName ) return 0x1E62;
        else if ( L"Sdotbelowdotaccent" == wsName ) return 0x1E68;
        else if ( L"Seharmenian" == wsName ) return 0x054D;
        else if ( L"Sevenroman" == wsName ) return 0x2166;
        else if ( L"Shaarmenian" == wsName ) return 0x0547;
        else if ( L"Shacyrillic" == wsName ) return 0x0428;
        else if ( L"Shchacyrillic" == wsName ) return 0x0429;
        else if ( L"Sheicoptic" == wsName ) return 0x03E2;
        else if ( L"Shhacyrillic" == wsName ) return 0x04BA;
        else if ( L"Shimacoptic" == wsName ) return 0x03EC;
        else if ( L"Sigma" == wsName ) return 0x03A3;
        else if ( L"Sixroman" == wsName ) return 0x2165;
        else if ( L"Smonospace" == wsName ) return 0xFF33;
        else if ( L"Softsigncyrillic" == wsName ) return 0x042C;
        else if ( L"Ssmall" == wsName ) return 0xF773;
        else if ( L"Stigmagreek" == wsName ) return 0x03DA;

        return 0;
    }
    static int Type1NameToUnicode_T(CWCharWrapper &wsName)
    {
        if      ( L"T" == wsName ) return 0x0054;
        else if ( L"Tau" == wsName ) return 0x03A4;
        else if ( L"Tbar" == wsName ) return 0x0166;
        else if ( L"Tcaron" == wsName ) return 0x0164;
        else if ( L"Tcedilla" == wsName ) return 0x0162;
        else if ( L"Tcircle" == wsName ) return 0x24C9;
        else if ( L"Tcircumflexbelow" == wsName ) return 0x1E70;
        else if ( L"Tcommaaccent" == wsName ) return 0x0162;
        else if ( L"Tdotaccent" == wsName ) return 0x1E6A;
        else if ( L"Tdotbelow" == wsName ) return 0x1E6C;
        else if ( L"Tecyrillic" == wsName ) return 0x0422;
        else if ( L"Tedescendercyrillic" == wsName ) return 0x04AC;
        else if ( L"Tenroman" == wsName ) return 0x2169;
        else if ( L"Tetsecyrillic" == wsName ) return 0x04B4;
        else if ( L"Theta" == wsName ) return 0x0398;
        else if ( L"Thook" == wsName ) return 0x01AC;
        else if ( L"Thorn" == wsName ) return 0x00DE;
        else if ( L"Thornsmall" == wsName ) return 0xF7FE;
        else if ( L"Threeroman" == wsName ) return 0x2162;
        else if ( L"Tildesmall" == wsName ) return 0xF6FE;
        else if ( L"Tiwnarmenian" == wsName ) return 0x054F;
        else if ( L"Tlinebelow" == wsName ) return 0x1E6E;
        else if ( L"Tmonospace" == wsName ) return 0xFF34;
        else if ( L"Toarmenian" == wsName ) return 0x0539;
        else if ( L"Tonefive" == wsName ) return 0x01BC;
        else if ( L"Tonesix" == wsName ) return 0x0184;
        else if ( L"Tonetwo" == wsName ) return 0x01A7;
        else if ( L"Tretroflexhook" == wsName ) return 0x01AE;
        else if ( L"Tsecyrillic" == wsName ) return 0x0426;
        else if ( L"Tshecyrillic" == wsName ) return 0x040B;
        else if ( L"Tsmall" == wsName ) return 0xF774;
        else if ( L"Twelveroman" == wsName ) return 0x216B;
        else if ( L"Tworoman" == wsName ) return 0x2161;

        return 0;
    }
    static int Type1NameToUnicode_U(CWCharWrapper &wsName)
    {
        if      ( L"U" == wsName ) return 0x0055;
        else if ( L"Uacute" == wsName ) return 0x00DA;
        else if ( L"Uacutesmall" == wsName ) return 0xF7FA;
        else if ( L"Ubreve" == wsName ) return 0x016C;
        else if ( L"Ucaron" == wsName ) return 0x01D3;
        else if ( L"Ucircle" == wsName ) return 0x24CA;
        else if ( L"Ucircumflex" == wsName ) return 0x00DB;
        else if ( L"Ucircumflexbelow" == wsName ) return 0x1E76;
        else if ( L"Ucircumflexsmall" == wsName ) return 0xF7FB;
        else if ( L"Ucyrillic" == wsName ) return 0x0423;
        else if ( L"Udblacute" == wsName ) return 0x0170;
        else if ( L"Udblgrave" == wsName ) return 0x0214;
        else if ( L"Udieresis" == wsName ) return 0x00DC;
        else if ( L"Udieresisacute" == wsName ) return 0x01D7;
        else if ( L"Udieresisbelow" == wsName ) return 0x1E72;
        else if ( L"Udieresiscaron" == wsName ) return 0x01D9;
        else if ( L"Udieresiscyrillic" == wsName ) return 0x04F0;
        else if ( L"Udieresisgrave" == wsName ) return 0x01DB;
        else if ( L"Udieresismacron" == wsName ) return 0x01D5;
        else if ( L"Udieresissmall" == wsName ) return 0xF7FC;
        else if ( L"Udotbelow" == wsName ) return 0x1EE4;
        else if ( L"Ugrave" == wsName ) return 0x00D9;
        else if ( L"Ugravesmall" == wsName ) return 0xF7F9;
        else if ( L"Uhookabove" == wsName ) return 0x1EE6;
        else if ( L"Uhorn" == wsName ) return 0x01AF;
        else if ( L"Uhornacute" == wsName ) return 0x1EE8;
        else if ( L"Uhorndotbelow" == wsName ) return 0x1EF0;
        else if ( L"Uhorngrave" == wsName ) return 0x1EEA;
        else if ( L"Uhornhookabove" == wsName ) return 0x1EEC;
        else if ( L"Uhorntilde" == wsName ) return 0x1EEE;
        else if ( L"Uhungarumlaut" == wsName ) return 0x0170;
        else if ( L"Uhungarumlautcyrillic" == wsName ) return 0x04F2;
        else if ( L"Uinvertedbreve" == wsName ) return 0x0216;
        else if ( L"Ukcyrillic" == wsName ) return 0x0478;
        else if ( L"Umacron" == wsName ) return 0x016A;
        else if ( L"Umacroncyrillic" == wsName ) return 0x04EE;
        else if ( L"Umacrondieresis" == wsName ) return 0x1E7A;
        else if ( L"Umonospace" == wsName ) return 0xFF35;
        else if ( L"Uogonek" == wsName ) return 0x0172;
        else if ( L"Upsilon" == wsName ) return 0x03A5;
        else if ( L"Upsilon1" == wsName ) return 0x03D2;
        else if ( L"Upsilonacutehooksymbolgreek" == wsName ) return 0x03D3;
        else if ( L"Upsilonafrican" == wsName ) return 0x01B1;
        else if ( L"Upsilondieresis" == wsName ) return 0x03AB;
        else if ( L"Upsilondieresishooksymbolgreek" == wsName ) return 0x03D4;
        else if ( L"Upsilonhooksymbol" == wsName ) return 0x03D2;
        else if ( L"Upsilontonos" == wsName ) return 0x038E;
        else if ( L"Uring" == wsName ) return 0x016E;
        else if ( L"Ushortcyrillic" == wsName ) return 0x040E;
        else if ( L"Usmall" == wsName ) return 0xF775;
        else if ( L"Ustraightcyrillic" == wsName ) return 0x04AE;
        else if ( L"Ustraightstrokecyrillic" == wsName ) return 0x04B0;
        else if ( L"Utilde" == wsName ) return 0x0168;
        else if ( L"Utildeacute" == wsName ) return 0x1E78;
        else if ( L"Utildebelow" == wsName ) return 0x1E74;

        return 0;
    }
    static int Type1NameToUnicode_V(CWCharWrapper &wsName)
    {
        if      ( L"V" == wsName ) return 0x0056;
        else if ( L"Vcircle" == wsName ) return 0x24CB;
        else if ( L"Vdotbelow" == wsName ) return 0x1E7E;
        else if ( L"Vecyrillic" == wsName ) return 0x0412;
        else if ( L"Vewarmenian" == wsName ) return 0x054E;
        else if ( L"Vhook" == wsName ) return 0x01B2;
        else if ( L"Vmonospace" == wsName ) return 0xFF36;
        else if ( L"Voarmenian" == wsName ) return 0x0548;
        else if ( L"Vsmall" == wsName ) return 0xF776;
        else if ( L"Vtilde" == wsName ) return 0x1E7C;

        return 0;
    }
    static int Type1NameToUnicode_W(CWCharWrapper &wsName)
    {
        if      ( L"W" == wsName ) return 0x0057;
        else if ( L"Wacute" == wsName ) return 0x1E82;
        else if ( L"Wcircle" == wsName ) return 0x24CC;
        else if ( L"Wcircumflex" == wsName ) return 0x0174;
        else if ( L"Wdieresis" == wsName ) return 0x1E84;
        else if ( L"Wdotaccent" == wsName ) return 0x1E86;
        else if ( L"Wdotbelow" == wsName ) return 0x1E88;
        else if ( L"Wgrave" == wsName ) return 0x1E80;
        else if ( L"Wmonospace" == wsName ) return 0xFF37;
        else if ( L"Wsmall" == wsName ) return 0xF777;

        return 0;
    }
    static int Type1NameToUnicode_X(CWCharWrapper &wsName)
    {
        if      ( L"X" == wsName ) return 0x0058;
        else if ( L"Xcircle" == wsName ) return 0x24CD;
        else if ( L"Xdieresis" == wsName ) return 0x1E8C;
        else if ( L"Xdotaccent" == wsName ) return 0x1E8A;
        else if ( L"Xeharmenian" == wsName ) return 0x053D;
        else if ( L"Xi" == wsName ) return 0x039E;
        else if ( L"Xmonospace" == wsName ) return 0xFF38;
        else if ( L"Xsmall" == wsName ) return 0xF778;

        return 0;
    }
    static int Type1NameToUnicode_Y(CWCharWrapper &wsName)
    {
        if      ( L"Y" == wsName ) return 0x0059;
        else if ( L"Yacute" == wsName ) return 0x00DD;
        else if ( L"Yacutesmall" == wsName ) return 0xF7FD;
        else if ( L"Yatcyrillic" == wsName ) return 0x0462;
        else if ( L"Ycircle" == wsName ) return 0x24CE;
        else if ( L"Ycircumflex" == wsName ) return 0x0176;
        else if ( L"Ydieresis" == wsName ) return 0x0178;
        else if ( L"Ydieresissmall" == wsName ) return 0xF7FF;
        else if ( L"Ydotaccent" == wsName ) return 0x1E8E;
        else if ( L"Ydotbelow" == wsName ) return 0x1EF4;
        else if ( L"Yericyrillic" == wsName ) return 0x042B;
        else if ( L"Yerudieresiscyrillic" == wsName ) return 0x04F8;
        else if ( L"Ygrave" == wsName ) return 0x1EF2;
        else if ( L"Yhook" == wsName ) return 0x01B3;
        else if ( L"Yhookabove" == wsName ) return 0x1EF6;
        else if ( L"Yiarmenian" == wsName ) return 0x0545;
        else if ( L"Yicyrillic" == wsName ) return 0x0407;
        else if ( L"Yiwnarmenian" == wsName ) return 0x0552;
        else if ( L"Ymonospace" == wsName ) return 0xFF39;
        else if ( L"Ysmall" == wsName ) return 0xF779;
        else if ( L"Ytilde" == wsName ) return 0x1EF8;
        else if ( L"Yusbigcyrillic" == wsName ) return 0x046A;
        else if ( L"Yusbigiotifiedcyrillic" == wsName ) return 0x046C;
        else if ( L"Yuslittlecyrillic" == wsName ) return 0x0466;
        else if ( L"Yuslittleiotifiedcyrillic" == wsName ) return 0x0468;

        return 0;
    }
    static int Type1NameToUnicode_Z(CWCharWrapper &wsName)
    {
        if      ( L"Z" == wsName ) return 0x005A;
        else if ( L"Zaarmenian" == wsName ) return 0x0536;
        else if ( L"Zacute" == wsName ) return 0x0179;
        else if ( L"Zcaron" == wsName ) return 0x017D;
        else if ( L"Zcaronsmall" == wsName ) return 0xF6FF;
        else if ( L"Zcircle" == wsName ) return 0x24CF;
        else if ( L"Zcircumflex" == wsName ) return 0x1E90;
        else if ( L"Zdot" == wsName ) return 0x017B;
        else if ( L"Zdotaccent" == wsName ) return 0x017B;
        else if ( L"Zdotbelow" == wsName ) return 0x1E92;
        else if ( L"Zecyrillic" == wsName ) return 0x0417;
        else if ( L"Zedescendercyrillic" == wsName ) return 0x0498;
        else if ( L"Zedieresiscyrillic" == wsName ) return 0x04DE;
        else if ( L"Zeta" == wsName ) return 0x0396;
        else if ( L"Zhearmenian" == wsName ) return 0x053A;
        else if ( L"Zhebrevecyrillic" == wsName ) return 0x04C1;
        else if ( L"Zhecyrillic" == wsName ) return 0x0416;
        else if ( L"Zhedescendercyrillic" == wsName ) return 0x0496;
        else if ( L"Zhedieresiscyrillic" == wsName ) return 0x04DC;
        else if ( L"Zlinebelow" == wsName ) return 0x1E94;
        else if ( L"Zmonospace" == wsName ) return 0xFF3A;
        else if ( L"Zsmall" == wsName ) return 0xF77A;
        else if ( L"Zstroke" == wsName ) return 0x01B5;

        return 0;
    }
    static int Type1NameToUnicode_a(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch (wChar1)
        {
        case 0:
            {
                if      ( L"a" == wsName ) return 0x0061;
                break;
            }
        case 'a':
            {
                if      ( L"aabengali" == wsName ) return 0x0986;
                else if ( L"aacute" == wsName ) return 0x00E1;
                else if ( L"aadeva" == wsName ) return 0x0906;
                else if ( L"aagujarati" == wsName ) return 0x0A86;
                else if ( L"aagurmukhi" == wsName ) return 0x0A06;
                else if ( L"aamatragurmukhi" == wsName ) return 0x0A3E;
                else if ( L"aarusquare" == wsName ) return 0x3303;
                else if ( L"aavowelsignbengali" == wsName ) return 0x09BE;
                else if ( L"aavowelsigndeva" == wsName ) return 0x093E;
                else if ( L"aavowelsigngujarati" == wsName ) return 0x0ABE;
                break;
            }
        case 'b':
            {
                if      ( L"abbreviationmarkarmenian" == wsName ) return 0x055F;
                else if ( L"abbreviationsigndeva" == wsName ) return 0x0970;
                else if ( L"abengali" == wsName ) return 0x0985;
                else if ( L"abopomofo" == wsName ) return 0x311A;
                else if ( L"abreve" == wsName ) return 0x0103;
                else if ( L"abreveacute" == wsName ) return 0x1EAF;
                else if ( L"abrevecyrillic" == wsName ) return 0x04D1;
                else if ( L"abrevedotbelow" == wsName ) return 0x1EB7;
                else if ( L"abrevegrave" == wsName ) return 0x1EB1;
                else if ( L"abrevehookabove" == wsName ) return 0x1EB3;
                else if ( L"abrevetilde" == wsName ) return 0x1EB5;
                break;
            }
        case 'c':
            {
                if      ( L"acaron" == wsName ) return 0x01CE;
                else if ( L"acircle" == wsName ) return 0x24D0;
                else if ( L"acircumflex" == wsName ) return 0x00E2;
                else if ( L"acircumflexacute" == wsName ) return 0x1EA5;
                else if ( L"acircumflexdotbelow" == wsName ) return 0x1EAD;
                else if ( L"acircumflexgrave" == wsName ) return 0x1EA7;
                else if ( L"acircumflexhookabove" == wsName ) return 0x1EA9;
                else if ( L"acircumflextilde" == wsName ) return 0x1EAB;
                else if ( L"acute" == wsName ) return 0x00B4;
                else if ( L"acutebelowcmb" == wsName ) return 0x0317;
                else if ( L"acutecmb" == wsName ) return 0x0301;
                else if ( L"acutecomb" == wsName ) return 0x0301;
                else if ( L"acutedeva" == wsName ) return 0x0954;
                else if ( L"acutelowmod" == wsName ) return 0x02CF;
                else if ( L"acutetonecmb" == wsName ) return 0x0341;
                else if ( L"acyrillic" == wsName ) return 0x0430;
                break;
            }
        case 'd':
            {
                if      ( L"adblgrave" == wsName ) return 0x0201;
                else if ( L"addakgurmukhi" == wsName ) return 0x0A71;
                else if ( L"adeva" == wsName ) return 0x0905;
                else if ( L"adieresis" == wsName ) return 0x00E4;
                else if ( L"adieresiscyrillic" == wsName ) return 0x04D3;
                else if ( L"adieresismacron" == wsName ) return 0x01DF;
                else if ( L"adotbelow" == wsName ) return 0x1EA1;
                else if ( L"adotmacron" == wsName ) return 0x01E1;
                break;
            }
        case 'e':
            {
                if      ( L"ae" == wsName ) return 0x00E6;
                else if ( L"aeacute" == wsName ) return 0x01FD;
                else if ( L"aekorean" == wsName ) return 0x3150;
                else if ( L"aemacron" == wsName ) return 0x01E3;
                break;
            }
        case 'f':
            {
                wchar_t wChar4 = wsName[4];
                switch( wChar4 )
                {
                case '0':
                    {
                        if      ( L"afii00208" == wsName ) return 0x2015;
                        else if ( L"afii08941" == wsName ) return 0x20A4;
                        break;
                    }
                case '1':
                    {
                        if      ( L"afii10017" == wsName ) return 0x0410;
                        else if ( L"afii10018" == wsName ) return 0x0411;
                        else if ( L"afii10019" == wsName ) return 0x0412;
                        else if ( L"afii10020" == wsName ) return 0x0413;
                        else if ( L"afii10021" == wsName ) return 0x0414;
                        else if ( L"afii10022" == wsName ) return 0x0415;
                        else if ( L"afii10023" == wsName ) return 0x0401;
                        else if ( L"afii10024" == wsName ) return 0x0416;
                        else if ( L"afii10025" == wsName ) return 0x0417;
                        else if ( L"afii10026" == wsName ) return 0x0418;
                        else if ( L"afii10027" == wsName ) return 0x0419;
                        else if ( L"afii10028" == wsName ) return 0x041A;
                        else if ( L"afii10029" == wsName ) return 0x041B;
                        else if ( L"afii10030" == wsName ) return 0x041C;
                        else if ( L"afii10031" == wsName ) return 0x041D;
                        else if ( L"afii10032" == wsName ) return 0x041E;
                        else if ( L"afii10033" == wsName ) return 0x041F;
                        else if ( L"afii10034" == wsName ) return 0x0420;
                        else if ( L"afii10035" == wsName ) return 0x0421;
                        else if ( L"afii10036" == wsName ) return 0x0422;
                        else if ( L"afii10037" == wsName ) return 0x0423;
                        else if ( L"afii10038" == wsName ) return 0x0424;
                        else if ( L"afii10039" == wsName ) return 0x0425;
                        else if ( L"afii10040" == wsName ) return 0x0426;
                        else if ( L"afii10041" == wsName ) return 0x0427;
                        else if ( L"afii10042" == wsName ) return 0x0428;
                        else if ( L"afii10043" == wsName ) return 0x0429;
                        else if ( L"afii10044" == wsName ) return 0x042A;
                        else if ( L"afii10045" == wsName ) return 0x042B;
                        else if ( L"afii10046" == wsName ) return 0x042C;
                        else if ( L"afii10047" == wsName ) return 0x042D;
                        else if ( L"afii10048" == wsName ) return 0x042E;
                        else if ( L"afii10049" == wsName ) return 0x042F;
                        else if ( L"afii10050" == wsName ) return 0x0490;
                        else if ( L"afii10051" == wsName ) return 0x0402;
                        else if ( L"afii10052" == wsName ) return 0x0403;
                        else if ( L"afii10053" == wsName ) return 0x0404;
                        else if ( L"afii10054" == wsName ) return 0x0405;
                        else if ( L"afii10055" == wsName ) return 0x0406;
                        else if ( L"afii10056" == wsName ) return 0x0407;
                        else if ( L"afii10057" == wsName ) return 0x0408;
                        else if ( L"afii10058" == wsName ) return 0x0409;
                        else if ( L"afii10059" == wsName ) return 0x040A;
                        else if ( L"afii10060" == wsName ) return 0x040B;
                        else if ( L"afii10061" == wsName ) return 0x040C;
                        else if ( L"afii10062" == wsName ) return 0x040E;
                        else if ( L"afii10063" == wsName ) return 0xF6C4;
                        else if ( L"afii10064" == wsName ) return 0xF6C5;
                        else if ( L"afii10065" == wsName ) return 0x0430;
                        else if ( L"afii10066" == wsName ) return 0x0431;
                        else if ( L"afii10067" == wsName ) return 0x0432;
                        else if ( L"afii10068" == wsName ) return 0x0433;
                        else if ( L"afii10069" == wsName ) return 0x0434;
                        else if ( L"afii10070" == wsName ) return 0x0435;
                        else if ( L"afii10071" == wsName ) return 0x0451;
                        else if ( L"afii10072" == wsName ) return 0x0436;
                        else if ( L"afii10073" == wsName ) return 0x0437;
                        else if ( L"afii10074" == wsName ) return 0x0438;
                        else if ( L"afii10075" == wsName ) return 0x0439;
                        else if ( L"afii10076" == wsName ) return 0x043A;
                        else if ( L"afii10077" == wsName ) return 0x043B;
                        else if ( L"afii10078" == wsName ) return 0x043C;
                        else if ( L"afii10079" == wsName ) return 0x043D;
                        else if ( L"afii10080" == wsName ) return 0x043E;
                        else if ( L"afii10081" == wsName ) return 0x043F;
                        else if ( L"afii10082" == wsName ) return 0x0440;
                        else if ( L"afii10083" == wsName ) return 0x0441;
                        else if ( L"afii10084" == wsName ) return 0x0442;
                        else if ( L"afii10085" == wsName ) return 0x0443;
                        else if ( L"afii10086" == wsName ) return 0x0444;
                        else if ( L"afii10087" == wsName ) return 0x0445;
                        else if ( L"afii10088" == wsName ) return 0x0446;
                        else if ( L"afii10089" == wsName ) return 0x0447;
                        else if ( L"afii10090" == wsName ) return 0x0448;
                        else if ( L"afii10091" == wsName ) return 0x0449;
                        else if ( L"afii10092" == wsName ) return 0x044A;
                        else if ( L"afii10093" == wsName ) return 0x044B;
                        else if ( L"afii10094" == wsName ) return 0x044C;
                        else if ( L"afii10095" == wsName ) return 0x044D;
                        else if ( L"afii10096" == wsName ) return 0x044E;
                        else if ( L"afii10097" == wsName ) return 0x044F;
                        else if ( L"afii10098" == wsName ) return 0x0491;
                        else if ( L"afii10099" == wsName ) return 0x0452;
                        else if ( L"afii10100" == wsName ) return 0x0453;
                        else if ( L"afii10101" == wsName ) return 0x0454;
                        else if ( L"afii10102" == wsName ) return 0x0455;
                        else if ( L"afii10103" == wsName ) return 0x0456;
                        else if ( L"afii10104" == wsName ) return 0x0457;
                        else if ( L"afii10105" == wsName ) return 0x0458;
                        else if ( L"afii10106" == wsName ) return 0x0459;
                        else if ( L"afii10107" == wsName ) return 0x045A;
                        else if ( L"afii10108" == wsName ) return 0x045B;
                        else if ( L"afii10109" == wsName ) return 0x045C;
                        else if ( L"afii10110" == wsName ) return 0x045E;
                        else if ( L"afii10145" == wsName ) return 0x040F;
                        else if ( L"afii10146" == wsName ) return 0x0462;
                        else if ( L"afii10147" == wsName ) return 0x0472;
                        else if ( L"afii10148" == wsName ) return 0x0474;
                        else if ( L"afii10192" == wsName ) return 0xF6C6;
                        else if ( L"afii10193" == wsName ) return 0x045F;
                        else if ( L"afii10194" == wsName ) return 0x0463;
                        else if ( L"afii10195" == wsName ) return 0x0473;
                        else if ( L"afii10196" == wsName ) return 0x0475;
                        else if ( L"afii10831" == wsName ) return 0xF6C7;
                        else if ( L"afii10832" == wsName ) return 0xF6C8;
                        else if ( L"afii10846" == wsName ) return 0x04D9;
                        break;
                    }
                case '2':
                    {
                        if      ( L"afii299" == wsName ) return 0x200E;
                        break;
                    }
                case '3':
                    {
                        if      ( L"afii300" == wsName ) return 0x200F;
                        else if ( L"afii301" == wsName ) return 0x200D;
                        break;
                    }
                case '5':
                    {
                        wchar_t wChar6 = wsName[6];
                        switch ( wChar6 )
                        {
                        case '3':
                            {
                                if      ( L"afii57381" == wsName ) return 0x066A;
                                else if ( L"afii57388" == wsName ) return 0x060C;
                                else if ( L"afii57392" == wsName ) return 0x0660;
                                else if ( L"afii57393" == wsName ) return 0x0661;
                                else if ( L"afii57394" == wsName ) return 0x0662;
                                else if ( L"afii57395" == wsName ) return 0x0663;
                                else if ( L"afii57396" == wsName ) return 0x0664;
                                else if ( L"afii57397" == wsName ) return 0x0665;
                                else if ( L"afii57398" == wsName ) return 0x0666;
                                else if ( L"afii57399" == wsName ) return 0x0667;
                                break;
                            }
                        case '4':
                            {
                                if      ( L"afii57400" == wsName ) return 0x0668;
                                else if ( L"afii57401" == wsName ) return 0x0669;
                                else if ( L"afii57403" == wsName ) return 0x061B;
                                else if ( L"afii57407" == wsName ) return 0x061F;
                                else if ( L"afii57409" == wsName ) return 0x0621;
                                else if ( L"afii57410" == wsName ) return 0x0622;
                                else if ( L"afii57411" == wsName ) return 0x0623;
                                else if ( L"afii57412" == wsName ) return 0x0624;
                                else if ( L"afii57413" == wsName ) return 0x0625;
                                else if ( L"afii57414" == wsName ) return 0x0626;
                                else if ( L"afii57415" == wsName ) return 0x0627;
                                else if ( L"afii57416" == wsName ) return 0x0628;
                                else if ( L"afii57417" == wsName ) return 0x0629;
                                else if ( L"afii57418" == wsName ) return 0x062A;
                                else if ( L"afii57419" == wsName ) return 0x062B;
                                else if ( L"afii57420" == wsName ) return 0x062C;
                                else if ( L"afii57421" == wsName ) return 0x062D;
                                else if ( L"afii57422" == wsName ) return 0x062E;
                                else if ( L"afii57423" == wsName ) return 0x062F;
                                else if ( L"afii57424" == wsName ) return 0x0630;
                                else if ( L"afii57425" == wsName ) return 0x0631;
                                else if ( L"afii57426" == wsName ) return 0x0632;
                                else if ( L"afii57427" == wsName ) return 0x0633;
                                else if ( L"afii57428" == wsName ) return 0x0634;
                                else if ( L"afii57429" == wsName ) return 0x0635;
                                else if ( L"afii57430" == wsName ) return 0x0636;
                                else if ( L"afii57431" == wsName ) return 0x0637;
                                else if ( L"afii57432" == wsName ) return 0x0638;
                                else if ( L"afii57433" == wsName ) return 0x0639;
                                else if ( L"afii57434" == wsName ) return 0x063A;
                                else if ( L"afii57440" == wsName ) return 0x0640;
                                else if ( L"afii57441" == wsName ) return 0x0641;
                                else if ( L"afii57442" == wsName ) return 0x0642;
                                else if ( L"afii57443" == wsName ) return 0x0643;
                                else if ( L"afii57444" == wsName ) return 0x0644;
                                else if ( L"afii57445" == wsName ) return 0x0645;
                                else if ( L"afii57446" == wsName ) return 0x0646;
                                else if ( L"afii57448" == wsName ) return 0x0648;
                                else if ( L"afii57449" == wsName ) return 0x0649;
                                else if ( L"afii57450" == wsName ) return 0x064A;
                                else if ( L"afii57451" == wsName ) return 0x064B;
                                else if ( L"afii57452" == wsName ) return 0x064C;
                                else if ( L"afii57453" == wsName ) return 0x064D;
                                else if ( L"afii57454" == wsName ) return 0x064E;
                                else if ( L"afii57455" == wsName ) return 0x064F;
                                else if ( L"afii57456" == wsName ) return 0x0650;
                                else if ( L"afii57457" == wsName ) return 0x0651;
                                else if ( L"afii57458" == wsName ) return 0x0652;
                                else if ( L"afii57470" == wsName ) return 0x0647;
                                break;
                            }
                        case '5':
                            {
                                if      ( L"afii57505" == wsName ) return 0x06A4;
                                else if ( L"afii57506" == wsName ) return 0x067E;
                                else if ( L"afii57507" == wsName ) return 0x0686;
                                else if ( L"afii57508" == wsName ) return 0x0698;
                                else if ( L"afii57509" == wsName ) return 0x06AF;
                                else if ( L"afii57511" == wsName ) return 0x0679;
                                else if ( L"afii57512" == wsName ) return 0x0688;
                                else if ( L"afii57513" == wsName ) return 0x0691;
                                else if ( L"afii57514" == wsName ) return 0x06BA;
                                else if ( L"afii57519" == wsName ) return 0x06D2;
                                else if ( L"afii57534" == wsName ) return 0x06D5;
                                break;
                            }
                        case '6':
                            {
                                if      ( L"afii57636" == wsName ) return 0x20AA;
                                else if ( L"afii57645" == wsName ) return 0x05BE;
                                else if ( L"afii57658" == wsName ) return 0x05C3;
                                else if ( L"afii57664" == wsName ) return 0x05D0;
                                else if ( L"afii57665" == wsName ) return 0x05D1;
                                else if ( L"afii57666" == wsName ) return 0x05D2;
                                else if ( L"afii57667" == wsName ) return 0x05D3;
                                else if ( L"afii57668" == wsName ) return 0x05D4;
                                else if ( L"afii57669" == wsName ) return 0x05D5;
                                else if ( L"afii57670" == wsName ) return 0x05D6;
                                else if ( L"afii57671" == wsName ) return 0x05D7;
                                else if ( L"afii57672" == wsName ) return 0x05D8;
                                else if ( L"afii57673" == wsName ) return 0x05D9;
                                else if ( L"afii57674" == wsName ) return 0x05DA;
                                else if ( L"afii57675" == wsName ) return 0x05DB;
                                else if ( L"afii57676" == wsName ) return 0x05DC;
                                else if ( L"afii57677" == wsName ) return 0x05DD;
                                else if ( L"afii57678" == wsName ) return 0x05DE;
                                else if ( L"afii57679" == wsName ) return 0x05DF;
                                else if ( L"afii57680" == wsName ) return 0x05E0;
                                else if ( L"afii57681" == wsName ) return 0x05E1;
                                else if ( L"afii57682" == wsName ) return 0x05E2;
                                else if ( L"afii57683" == wsName ) return 0x05E3;
                                else if ( L"afii57684" == wsName ) return 0x05E4;
                                else if ( L"afii57685" == wsName ) return 0x05E5;
                                else if ( L"afii57686" == wsName ) return 0x05E6;
                                else if ( L"afii57687" == wsName ) return 0x05E7;
                                else if ( L"afii57688" == wsName ) return 0x05E8;
                                else if ( L"afii57689" == wsName ) return 0x05E9;
                                else if ( L"afii57690" == wsName ) return 0x05EA;
                                else if ( L"afii57694" == wsName ) return 0xFB2A;
                                else if ( L"afii57695" == wsName ) return 0xFB2B;
                                break;
                            }
                        case '7':
                            {
                                if      ( L"afii57700" == wsName ) return 0xFB4B;
                                else if ( L"afii57705" == wsName ) return 0xFB1F;
                                else if ( L"afii57716" == wsName ) return 0x05F0;
                                else if ( L"afii57717" == wsName ) return 0x05F1;
                                else if ( L"afii57718" == wsName ) return 0x05F2;
                                else if ( L"afii57723" == wsName ) return 0xFB35;
                                else if ( L"afii57793" == wsName ) return 0x05B4;
                                else if ( L"afii57794" == wsName ) return 0x05B5;
                                else if ( L"afii57795" == wsName ) return 0x05B6;
                                else if ( L"afii57796" == wsName ) return 0x05BB;
                                else if ( L"afii57797" == wsName ) return 0x05B8;
                                else if ( L"afii57798" == wsName ) return 0x05B7;
                                else if ( L"afii57799" == wsName ) return 0x05B0;
                                break;
                            }
                        case '8':
                            {
                                if      ( L"afii57800" == wsName ) return 0x05B2;
                                else if ( L"afii57801" == wsName ) return 0x05B1;
                                else if ( L"afii57802" == wsName ) return 0x05B3;
                                else if ( L"afii57803" == wsName ) return 0x05C2;
                                else if ( L"afii57804" == wsName ) return 0x05C1;
                                else if ( L"afii57806" == wsName ) return 0x05B9;
                                else if ( L"afii57807" == wsName ) return 0x05BC;
                                else if ( L"afii57839" == wsName ) return 0x05BD;
                                else if ( L"afii57841" == wsName ) return 0x05BF;
                                else if ( L"afii57842" == wsName ) return 0x05C0;
                                break;
                            }
                        case '9':
                            {
                                if      ( L"afii57929" == wsName ) return 0x02BC;
                                break;
                            }
                        }
                        break;
                    }
                case '6':
                    {
                        if      ( L"afii61248" == wsName ) return 0x2105;
                        else if ( L"afii61289" == wsName ) return 0x2113;
                        else if ( L"afii61352" == wsName ) return 0x2116;
                        else if ( L"afii61573" == wsName ) return 0x202C;
                        else if ( L"afii61574" == wsName ) return 0x202D;
                        else if ( L"afii61575" == wsName ) return 0x202E;
                        else if ( L"afii61664" == wsName ) return 0x200C;
                        else if ( L"afii63167" == wsName ) return 0x066D;
                        else if ( L"afii64937" == wsName ) return 0x02BD;
                        break;
                    }
                }
                break;
            }
        case 'g':
            {
                if      ( L"agrave" == wsName ) return 0x00E0;
                else if ( L"agujarati" == wsName ) return 0x0A85;
                else if ( L"agurmukhi" == wsName ) return 0x0A05;
                break;
            }
        case 'h':
            {
                if      ( L"ahiragana" == wsName ) return 0x3042;
                else if ( L"ahookabove" == wsName ) return 0x1EA3;
                break;
            }
        case 'i':
            {
                if      ( L"aibengali" == wsName ) return 0x0990;
                else if ( L"aibopomofo" == wsName ) return 0x311E;
                else if ( L"aideva" == wsName ) return 0x0910;
                else if ( L"aiecyrillic" == wsName ) return 0x04D5;
                else if ( L"aigujarati" == wsName ) return 0x0A90;
                else if ( L"aigurmukhi" == wsName ) return 0x0A10;
                else if ( L"aimatragurmukhi" == wsName ) return 0x0A48;
                else if ( L"ainarabic" == wsName ) return 0x0639;
                else if ( L"ainfinalarabic" == wsName ) return 0xFECA;
                else if ( L"aininitialarabic" == wsName ) return 0xFECB;
                else if ( L"ainmedialarabic" == wsName ) return 0xFECC;
                else if ( L"ainvertedbreve" == wsName ) return 0x0203;
                else if ( L"aivowelsignbengali" == wsName ) return 0x09C8;
                else if ( L"aivowelsigndeva" == wsName ) return 0x0948;
                else if ( L"aivowelsigngujarati" == wsName ) return 0x0AC8;
                break;
            }
        case 'k':
            {
                if      ( L"akatakana" == wsName ) return 0x30A2;
                else if ( L"akatakanahalfwidth" == wsName ) return 0xFF71;
                else if ( L"akorean" == wsName ) return 0x314F;
                break;
            }
        case 'l':
            {
                if      ( L"alef" == wsName ) return 0x05D0;
                else if ( L"alefarabic" == wsName ) return 0x0627;
                else if ( L"alefdageshhebrew" == wsName ) return 0xFB30;
                else if ( L"aleffinalarabic" == wsName ) return 0xFE8E;
                else if ( L"alefhamzaabovearabic" == wsName ) return 0x0623;
                else if ( L"alefhamzaabovefinalarabic" == wsName ) return 0xFE84;
                else if ( L"alefhamzabelowarabic" == wsName ) return 0x0625;
                else if ( L"alefhamzabelowfinalarabic" == wsName ) return 0xFE88;
                else if ( L"alefhebrew" == wsName ) return 0x05D0;
                else if ( L"aleflamedhebrew" == wsName ) return 0xFB4F;
                else if ( L"alefmaddaabovearabic" == wsName ) return 0x0622;
                else if ( L"alefmaddaabovefinalarabic" == wsName ) return 0xFE82;
                else if ( L"alefmaksuraarabic" == wsName ) return 0x0649;
                else if ( L"alefmaksurafinalarabic" == wsName ) return 0xFEF0;
                else if ( L"alefmaksurainitialarabic" == wsName ) return 0xFEF3;
                else if ( L"alefmaksuramedialarabic" == wsName ) return 0xFEF4;
                else if ( L"alefpatahhebrew" == wsName ) return 0xFB2E;
                else if ( L"alefqamatshebrew" == wsName ) return 0xFB2F;
                else if ( L"aleph" == wsName ) return 0x2135;
                else if ( L"allequal" == wsName ) return 0x224C;
                else if ( L"alpha" == wsName ) return 0x03B1;
                else if ( L"alphatonos" == wsName ) return 0x03AC;
                break;
            }
        case 'm':
            {
                if      ( L"amacron" == wsName ) return 0x0101;
                else if ( L"amonospace" == wsName ) return 0xFF41;
                else if ( L"ampersand" == wsName ) return 0x0026;
                else if ( L"ampersandmonospace" == wsName ) return 0xFF06;
                else if ( L"ampersandsmall" == wsName ) return 0xF726;
                else if ( L"amsquare" == wsName ) return 0x33C2;
                break;
            }
        case 'n':
            {
                if      ( L"anbopomofo" == wsName ) return 0x3122;
                else if ( L"angbopomofo" == wsName ) return 0x3124;
                else if ( L"angbracketleft" == wsName ) return 0x3008;
                else if ( L"angbracketright" == wsName ) return 0x3009;
                else if ( L"angkhankhuthai" == wsName ) return 0x0E5A;
                else if ( L"angle" == wsName ) return 0x2220;
                else if ( L"anglebracketleft" == wsName ) return 0x3008;
                else if ( L"anglebracketleftvertical" == wsName ) return 0xFE3F;
                else if ( L"anglebracketright" == wsName ) return 0x3009;
                else if ( L"anglebracketrightvertical" == wsName ) return 0xFE40;
                else if ( L"angleleft" == wsName ) return 0x2329;
                else if ( L"angleright" == wsName ) return 0x232A;
                else if ( L"angstrom" == wsName ) return 0x212B;
                else if ( L"anoteleia" == wsName ) return 0x0387;
                else if ( L"anudattadeva" == wsName ) return 0x0952;
                else if ( L"anusvarabengali" == wsName ) return 0x0982;
                else if ( L"anusvaradeva" == wsName ) return 0x0902;
                else if ( L"anusvaragujarati" == wsName ) return 0x0A82;
                break;
            }
        case 'o':
            {
                if      ( L"aogonek" == wsName ) return 0x0105;
                break;
            }
        case 'p':
            {
                if      ( L"apaatosquare" == wsName ) return 0x3300;
                else if ( L"aparen" == wsName ) return 0x249C;
                else if ( L"apostrophearmenian" == wsName ) return 0x055A;
                else if ( L"apostrophemod" == wsName ) return 0x02BC;
                else if ( L"apple" == wsName ) return 0xF8FF;
                else if ( L"approaches" == wsName ) return 0x2250;
                else if ( L"approxequal" == wsName ) return 0x2248;
                else if ( L"approxequalorimage" == wsName ) return 0x2252;
                else if ( L"approximatelyequal" == wsName ) return 0x2245;
                break;
            }
        case 'r':
            {
                if      ( L"araeaekorean" == wsName ) return 0x318E;
                else if ( L"araeakorean" == wsName ) return 0x318D;
                else if ( L"arc" == wsName ) return 0x2312;
                else if ( L"arighthalfring" == wsName ) return 0x1E9A;
                else if ( L"aring" == wsName ) return 0x00E5;
                else if ( L"aringacute" == wsName ) return 0x01FB;
                else if ( L"aringbelow" == wsName ) return 0x1E01;
                else if ( L"arrowboth" == wsName ) return 0x2194;
                else if ( L"arrowdashdown" == wsName ) return 0x21E3;
                else if ( L"arrowdashleft" == wsName ) return 0x21E0;
                else if ( L"arrowdashright" == wsName ) return 0x21E2;
                else if ( L"arrowdashup" == wsName ) return 0x21E1;
                else if ( L"arrowdblboth" == wsName ) return 0x21D4;
                else if ( L"arrowdbldown" == wsName ) return 0x21D3;
                else if ( L"arrowdblleft" == wsName ) return 0x21D0;
                else if ( L"arrowdblright" == wsName ) return 0x21D2;
                else if ( L"arrowdblup" == wsName ) return 0x21D1;
                else if ( L"arrowdown" == wsName ) return 0x2193;
                else if ( L"arrowdownleft" == wsName ) return 0x2199;
                else if ( L"arrowdownright" == wsName ) return 0x2198;
                else if ( L"arrowdownwhite" == wsName ) return 0x21E9;
                else if ( L"arrowheaddownmod" == wsName ) return 0x02C5;
                else if ( L"arrowheadleftmod" == wsName ) return 0x02C2;
                else if ( L"arrowheadrightmod" == wsName ) return 0x02C3;
                else if ( L"arrowheadupmod" == wsName ) return 0x02C4;
                else if ( L"arrowhorizex" == wsName ) return 0xF8E7;
                else if ( L"arrowleft" == wsName ) return 0x2190;
                else if ( L"arrowleftdbl" == wsName ) return 0x21D0;
                else if ( L"arrowleftdblstroke" == wsName ) return 0x21CD;
                else if ( L"arrowleftoverright" == wsName ) return 0x21C6;
                else if ( L"arrowleftwhite" == wsName ) return 0x21E6;
                else if ( L"arrowright" == wsName ) return 0x2192;
                else if ( L"arrowrightdblstroke" == wsName ) return 0x21CF;
                else if ( L"arrowrightheavy" == wsName ) return 0x279E;
                else if ( L"arrowrightoverleft" == wsName ) return 0x21C4;
                else if ( L"arrowrightwhite" == wsName ) return 0x21E8;
                else if ( L"arrowtableft" == wsName ) return 0x21E4;
                else if ( L"arrowtabright" == wsName ) return 0x21E5;
                else if ( L"arrowup" == wsName ) return 0x2191;
                else if ( L"arrowupdn" == wsName ) return 0x2195;
                else if ( L"arrowupdnbse" == wsName ) return 0x21A8;
                else if ( L"arrowupdownbase" == wsName ) return 0x21A8;
                else if ( L"arrowupleft" == wsName ) return 0x2196;
                else if ( L"arrowupleftofdown" == wsName ) return 0x21C5;
                else if ( L"arrowupright" == wsName ) return 0x2197;
                else if ( L"arrowupwhite" == wsName ) return 0x21E7;
                else if ( L"arrowvertex" == wsName ) return 0xF8E6;
                break;
            }
        case 's':
            {
                if      ( L"asciicircum" == wsName ) return 0x005E;
                else if ( L"asciicircummonospace" == wsName ) return 0xFF3E;
                else if ( L"asciitilde" == wsName ) return 0x007E;
                else if ( L"asciitildemonospace" == wsName ) return 0xFF5E;
                else if ( L"ascript" == wsName ) return 0x0251;
                else if ( L"ascriptturned" == wsName ) return 0x0252;
                else if ( L"asmallhiragana" == wsName ) return 0x3041;
                else if ( L"asmallkatakana" == wsName ) return 0x30A1;
                else if ( L"asmallkatakanahalfwidth" == wsName ) return 0xFF67;
                else if ( L"asterisk" == wsName ) return 0x002A;
                else if ( L"asteriskaltonearabic" == wsName ) return 0x066D;
                else if ( L"asteriskarabic" == wsName ) return 0x066D;
                else if ( L"asteriskmath" == wsName ) return 0x2217;
                else if ( L"asteriskmonospace" == wsName ) return 0xFF0A;
                else if ( L"asterisksmall" == wsName ) return 0xFE61;
                else if ( L"asterism" == wsName ) return 0x2042;
                else if ( L"asuperior" == wsName ) return 0xF6E9;
                else if ( L"asymptoticallyequal" == wsName ) return 0x2243;
                break;
            }
        case 't':
            {
                if      ( L"at" == wsName ) return 0x0040;
                else if ( L"atilde" == wsName ) return 0x00E3;
                else if ( L"atmonospace" == wsName ) return 0xFF20;
                else if ( L"atsmall" == wsName ) return 0xFE6B;
                else if ( L"aturned" == wsName ) return 0x0250;
                break;
            }
        case 'u':
            {
                if      ( L"aubengali" == wsName ) return 0x0994;
                else if ( L"aubopomofo" == wsName ) return 0x3120;
                else if ( L"audeva" == wsName ) return 0x0914;
                else if ( L"augujarati" == wsName ) return 0x0A94;
                else if ( L"augurmukhi" == wsName ) return 0x0A14;
                else if ( L"aulengthmarkbengali" == wsName ) return 0x09D7;
                else if ( L"aumatragurmukhi" == wsName ) return 0x0A4C;
                else if ( L"auvowelsignbengali" == wsName ) return 0x09CC;
                else if ( L"auvowelsigndeva" == wsName ) return 0x094C;
                else if ( L"auvowelsigngujarati" == wsName ) return 0x0ACC;
                break;
            }
        case 'v':
            {
                if      ( L"avagrahadeva" == wsName ) return 0x093D;
                break;
            }
        case 'y':
            {
                if      ( L"aybarmenian" == wsName ) return 0x0561;
                else if ( L"ayin" == wsName ) return 0x05E2;
                else if ( L"ayinaltonehebrew" == wsName ) return 0xFB20;
                else if ( L"ayinhebrew" == wsName ) return 0x05E2;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_b(CWCharWrapper &wsName)
    {
        if      ( L"b" == wsName ) return 0x0062;
        else if ( L"babengali" == wsName ) return 0x09AC;
        else if ( L"backslash" == wsName ) return 0x005C;
        else if ( L"backslashmonospace" == wsName ) return 0xFF3C;
        else if ( L"badeva" == wsName ) return 0x092C;
        else if ( L"bagujarati" == wsName ) return 0x0AAC;
        else if ( L"bagurmukhi" == wsName ) return 0x0A2C;
        else if ( L"bahiragana" == wsName ) return 0x3070;
        else if ( L"bahtthai" == wsName ) return 0x0E3F;
        else if ( L"bakatakana" == wsName ) return 0x30D0;
        else if ( L"bar" == wsName ) return 0x007C;
        else if ( L"barmonospace" == wsName ) return 0xFF5C;
        else if ( L"bbopomofo" == wsName ) return 0x3105;
        else if ( L"bcircle" == wsName ) return 0x24D1;
        else if ( L"bdotaccent" == wsName ) return 0x1E03;
        else if ( L"bdotbelow" == wsName ) return 0x1E05;
        else if ( L"beamedsixteenthnotes" == wsName ) return 0x266C;
        else if ( L"because" == wsName ) return 0x2235;
        else if ( L"becyrillic" == wsName ) return 0x0431;
        else if ( L"beharabic" == wsName ) return 0x0628;
        else if ( L"behfinalarabic" == wsName ) return 0xFE90;
        else if ( L"behinitialarabic" == wsName ) return 0xFE91;
        else if ( L"behiragana" == wsName ) return 0x3079;
        else if ( L"behmedialarabic" == wsName ) return 0xFE92;
        else if ( L"behmeeminitialarabic" == wsName ) return 0xFC9F;
        else if ( L"behmeemisolatedarabic" == wsName ) return 0xFC08;
        else if ( L"behnoonfinalarabic" == wsName ) return 0xFC6D;
        else if ( L"bekatakana" == wsName ) return 0x30D9;
        else if ( L"benarmenian" == wsName ) return 0x0562;
        else if ( L"bet" == wsName ) return 0x05D1;
        else if ( L"beta" == wsName ) return 0x03B2;
        else if ( L"betasymbolgreek" == wsName ) return 0x03D0;
        else if ( L"betdagesh" == wsName ) return 0xFB31;
        else if ( L"betdageshhebrew" == wsName ) return 0xFB31;
        else if ( L"bethebrew" == wsName ) return 0x05D1;
        else if ( L"betrafehebrew" == wsName ) return 0xFB4C;
        else if ( L"bhabengali" == wsName ) return 0x09AD;
        else if ( L"bhadeva" == wsName ) return 0x092D;
        else if ( L"bhagujarati" == wsName ) return 0x0AAD;
        else if ( L"bhagurmukhi" == wsName ) return 0x0A2D;
        else if ( L"bhook" == wsName ) return 0x0253;
        else if ( L"bihiragana" == wsName ) return 0x3073;
        else if ( L"bikatakana" == wsName ) return 0x30D3;
        else if ( L"bilabialclick" == wsName ) return 0x0298;
        else if ( L"bindigurmukhi" == wsName ) return 0x0A02;
        else if ( L"birusquare" == wsName ) return 0x3331;
        else if ( L"blackcircle" == wsName ) return 0x25CF;
        else if ( L"blackdiamond" == wsName ) return 0x25C6;
        else if ( L"blackdownpointingtriangle" == wsName ) return 0x25BC;
        else if ( L"blackleftpointingpointer" == wsName ) return 0x25C4;
        else if ( L"blackleftpointingtriangle" == wsName ) return 0x25C0;
        else if ( L"blacklenticularbracketleft" == wsName ) return 0x3010;
        else if ( L"blacklenticularbracketleftvertical" == wsName ) return 0xFE3B;
        else if ( L"blacklenticularbracketright" == wsName ) return 0x3011;
        else if ( L"blacklenticularbracketrightvertical" == wsName ) return 0xFE3C;
        else if ( L"blacklowerlefttriangle" == wsName ) return 0x25E3;
        else if ( L"blacklowerrighttriangle" == wsName ) return 0x25E2;
        else if ( L"blackrectangle" == wsName ) return 0x25AC;
        else if ( L"blackrightpointingpointer" == wsName ) return 0x25BA;
        else if ( L"blackrightpointingtriangle" == wsName ) return 0x25B6;
        else if ( L"blacksmallsquare" == wsName ) return 0x25AA;
        else if ( L"blacksmilingface" == wsName ) return 0x263B;
        else if ( L"blacksquare" == wsName ) return 0x25A0;
        else if ( L"blackstar" == wsName ) return 0x2605;
        else if ( L"blackupperlefttriangle" == wsName ) return 0x25E4;
        else if ( L"blackupperrighttriangle" == wsName ) return 0x25E5;
        else if ( L"blackuppointingsmalltriangle" == wsName ) return 0x25B4;
        else if ( L"blackuppointingtriangle" == wsName ) return 0x25B2;
        else if ( L"blank" == wsName ) return 0x2423;
        else if ( L"blinebelow" == wsName ) return 0x1E07;
        else if ( L"block" == wsName ) return 0x2588;
        else if ( L"bmonospace" == wsName ) return 0xFF42;
        else if ( L"bobaimaithai" == wsName ) return 0x0E1A;
        else if ( L"bohiragana" == wsName ) return 0x307C;
        else if ( L"bokatakana" == wsName ) return 0x30DC;
        else if ( L"bparen" == wsName ) return 0x249D;
        else if ( L"bqsquare" == wsName ) return 0x33C3;
        else if ( L"braceex" == wsName ) return 0xF8F4;
        else if ( L"braceleft" == wsName ) return 0x007B;
        else if ( L"braceleftbt" == wsName ) return 0xF8F3;
        else if ( L"braceleftmid" == wsName ) return 0xF8F2;
        else if ( L"braceleftmonospace" == wsName ) return 0xFF5B;
        else if ( L"braceleftsmall" == wsName ) return 0xFE5B;
        else if ( L"bracelefttp" == wsName ) return 0xF8F1;
        else if ( L"braceleftvertical" == wsName ) return 0xFE37;
        else if ( L"braceright" == wsName ) return 0x007D;
        else if ( L"bracerightbt" == wsName ) return 0xF8FE;
        else if ( L"bracerightmid" == wsName ) return 0xF8FD;
        else if ( L"bracerightmonospace" == wsName ) return 0xFF5D;
        else if ( L"bracerightsmall" == wsName ) return 0xFE5C;
        else if ( L"bracerighttp" == wsName ) return 0xF8FC;
        else if ( L"bracerightvertical" == wsName ) return 0xFE38;
        else if ( L"bracketleft" == wsName ) return 0x005B;
        else if ( L"bracketleftbt" == wsName ) return 0xF8F0;
        else if ( L"bracketleftex" == wsName ) return 0xF8EF;
        else if ( L"bracketleftmonospace" == wsName ) return 0xFF3B;
        else if ( L"bracketlefttp" == wsName ) return 0xF8EE;
        else if ( L"bracketright" == wsName ) return 0x005D;
        else if ( L"bracketrightbt" == wsName ) return 0xF8FB;
        else if ( L"bracketrightex" == wsName ) return 0xF8FA;
        else if ( L"bracketrightmonospace" == wsName ) return 0xFF3D;
        else if ( L"bracketrighttp" == wsName ) return 0xF8F9;
        else if ( L"breve" == wsName ) return 0x02D8;
        else if ( L"brevebelowcmb" == wsName ) return 0x032E;
        else if ( L"brevecmb" == wsName ) return 0x0306;
        else if ( L"breveinvertedbelowcmb" == wsName ) return 0x032F;
        else if ( L"breveinvertedcmb" == wsName ) return 0x0311;
        else if ( L"breveinverteddoublecmb" == wsName ) return 0x0361;
        else if ( L"bridgebelowcmb" == wsName ) return 0x032A;
        else if ( L"bridgeinvertedbelowcmb" == wsName ) return 0x033A;
        else if ( L"brokenbar" == wsName ) return 0x00A6;
        else if ( L"bstroke" == wsName ) return 0x0180;
        else if ( L"bsuperior" == wsName ) return 0xF6EA;
        else if ( L"btopbar" == wsName ) return 0x0183;
        else if ( L"buhiragana" == wsName ) return 0x3076;
        else if ( L"bukatakana" == wsName ) return 0x30D6;
        else if ( L"bullet" == wsName ) return 0x2022;
        else if ( L"bulletinverse" == wsName ) return 0x25D8;
        else if ( L"bulletoperator" == wsName ) return 0x2219;
        else if ( L"bullseye" == wsName ) return 0x25CE;

        return 0;
    }
    static int Type1NameToUnicode_c(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch (wChar1)
        {
        case 0:
            {
                if      ( L"c" == wsName ) return 0x0063;
                break;
            }
        case 'a':
            {
                if      ( L"caarmenian" == wsName ) return 0x056E;
                else if ( L"cabengali" == wsName ) return 0x099A;
                else if ( L"cacute" == wsName ) return 0x0107;
                else if ( L"cadeva" == wsName ) return 0x091A;
                else if ( L"cagujarati" == wsName ) return 0x0A9A;
                else if ( L"cagurmukhi" == wsName ) return 0x0A1A;
                else if ( L"calsquare" == wsName ) return 0x3388;
                else if ( L"candrabindubengali" == wsName ) return 0x0981;
                else if ( L"candrabinducmb" == wsName ) return 0x0310;
                else if ( L"candrabindudeva" == wsName ) return 0x0901;
                else if ( L"candrabindugujarati" == wsName ) return 0x0A81;
                else if ( L"capslock" == wsName ) return 0x21EA;
                else if ( L"careof" == wsName ) return 0x2105;
                else if ( L"caron" == wsName ) return 0x02C7;
                else if ( L"caronbelowcmb" == wsName ) return 0x032C;
                else if ( L"caroncmb" == wsName ) return 0x030C;
                else if ( L"carriagereturn" == wsName ) return 0x21B5;
                break;
            }
        case 'b':
            {
                if      ( L"cbopomofo" == wsName ) return 0x3118;
                break;
            }
        case 'c':
            {
                if      ( L"ccaron" == wsName ) return 0x010D;
                else if ( L"ccedilla" == wsName ) return 0x00E7;
                else if ( L"ccedillaacute" == wsName ) return 0x1E09;
                else if ( L"ccircle" == wsName ) return 0x24D2;
                else if ( L"ccircumflex" == wsName ) return 0x0109;
                else if ( L"ccurl" == wsName ) return 0x0255;
                break;
            }
        case 'd':
            {
                if      ( L"cdot" == wsName ) return 0x010B;
                else if ( L"cdotaccent" == wsName ) return 0x010B;
                else if ( L"cdsquare" == wsName ) return 0x33C5;
                break;
            }
        case 'e':
            {
                if      ( L"cedilla" == wsName ) return 0x00B8;
                else if ( L"cedillacmb" == wsName ) return 0x0327;
                else if ( L"cent" == wsName ) return 0x00A2;
                else if ( L"centigrade" == wsName ) return 0x2103;
                else if ( L"centinferior" == wsName ) return 0xF6DF;
                else if ( L"centmonospace" == wsName ) return 0xFFE0;
                else if ( L"centoldstyle" == wsName ) return 0xF7A2;
                else if ( L"centsuperior" == wsName ) return 0xF6E0;
                break;
            }
        case 'h':
            {
                if      ( L"chaarmenian" == wsName ) return 0x0579;
                else if ( L"chabengali" == wsName ) return 0x099B;
                else if ( L"chadeva" == wsName ) return 0x091B;
                else if ( L"chagujarati" == wsName ) return 0x0A9B;
                else if ( L"chagurmukhi" == wsName ) return 0x0A1B;
                else if ( L"chbopomofo" == wsName ) return 0x3114;
                else if ( L"cheabkhasiancyrillic" == wsName ) return 0x04BD;
                else if ( L"checkmark" == wsName ) return 0x2713;
                else if ( L"checyrillic" == wsName ) return 0x0447;
                else if ( L"chedescenderabkhasiancyrillic" == wsName ) return 0x04BF;
                else if ( L"chedescendercyrillic" == wsName ) return 0x04B7;
                else if ( L"chedieresiscyrillic" == wsName ) return 0x04F5;
                else if ( L"cheharmenian" == wsName ) return 0x0573;
                else if ( L"chekhakassiancyrillic" == wsName ) return 0x04CC;
                else if ( L"cheverticalstrokecyrillic" == wsName ) return 0x04B9;
                else if ( L"chi" == wsName ) return 0x03C7;
                else if ( L"chieuchacirclekorean" == wsName ) return 0x3277;
                else if ( L"chieuchaparenkorean" == wsName ) return 0x3217;
                else if ( L"chieuchcirclekorean" == wsName ) return 0x3269;
                else if ( L"chieuchkorean" == wsName ) return 0x314A;
                else if ( L"chieuchparenkorean" == wsName ) return 0x3209;
                else if ( L"chochangthai" == wsName ) return 0x0E0A;
                else if ( L"chochanthai" == wsName ) return 0x0E08;
                else if ( L"chochingthai" == wsName ) return 0x0E09;
                else if ( L"chochoethai" == wsName ) return 0x0E0C;
                else if ( L"chook" == wsName ) return 0x0188;
                break;
            }
        case 'i':
            {
                if      ( L"cieucacirclekorean" == wsName ) return 0x3276;
                else if ( L"cieucaparenkorean" == wsName ) return 0x3216;
                else if ( L"cieuccirclekorean" == wsName ) return 0x3268;
                else if ( L"cieuckorean" == wsName ) return 0x3148;
                else if ( L"cieucparenkorean" == wsName ) return 0x3208;
                else if ( L"cieucuparenkorean" == wsName ) return 0x321C;
                else if ( L"circle" == wsName ) return 0x25CB;
                else if ( L"circlecopyrt" == wsName ) return 0x00A9;
                else if ( L"circlemultiply" == wsName ) return 0x2297;
                else if ( L"circleot" == wsName ) return 0x2299;
                else if ( L"circleplus" == wsName ) return 0x2295;
                else if ( L"circlepostalmark" == wsName ) return 0x3036;
                else if ( L"circlewithlefthalfblack" == wsName ) return 0x25D0;
                else if ( L"circlewithrighthalfblack" == wsName ) return 0x25D1;
                else if ( L"circumflex" == wsName ) return 0x02C6;
                else if ( L"circumflexbelowcmb" == wsName ) return 0x032D;
                else if ( L"circumflexcmb" == wsName ) return 0x0302;
                break;
            }
        case 'l':
            {
                if      ( L"clear" == wsName ) return 0x2327;
                else if ( L"clickalveolar" == wsName ) return 0x01C2;
                else if ( L"clickdental" == wsName ) return 0x01C0;
                else if ( L"clicklateral" == wsName ) return 0x01C1;
                else if ( L"clickretroflex" == wsName ) return 0x01C3;
                else if ( L"club" == wsName ) return 0x2663;
                else if ( L"clubsuitblack" == wsName ) return 0x2663;
                else if ( L"clubsuitwhite" == wsName ) return 0x2667;
                break;
            }
        case 'm':
            {
                if      ( L"cmcubedsquare" == wsName ) return 0x33A4;
                else if ( L"cmonospace" == wsName ) return 0xFF43;
                else if ( L"cmsquaredsquare" == wsName ) return 0x33A0;
                break;
            }
        case 'o':
            {
                if      ( L"coarmenian" == wsName ) return 0x0581;
                else if ( L"colon" == wsName ) return 0x003A;
                else if ( L"colonmonetary" == wsName ) return 0x20A1;
                else if ( L"colonmonospace" == wsName ) return 0xFF1A;
                else if ( L"colonsign" == wsName ) return 0x20A1;
                else if ( L"colonsmall" == wsName ) return 0xFE55;
                else if ( L"colontriangularhalfmod" == wsName ) return 0x02D1;
                else if ( L"colontriangularmod" == wsName ) return 0x02D0;
                else if ( L"comma" == wsName ) return 0x002C;
                else if ( L"commaabovecmb" == wsName ) return 0x0313;
                else if ( L"commaaboverightcmb" == wsName ) return 0x0315;
                else if ( L"commaaccent" == wsName ) return 0xF6C3;
                else if ( L"commaarabic" == wsName ) return 0x060C;
                else if ( L"commaarmenian" == wsName ) return 0x055D;
                else if ( L"commainferior" == wsName ) return 0xF6E1;
                else if ( L"commamonospace" == wsName ) return 0xFF0C;
                else if ( L"commareversedabovecmb" == wsName ) return 0x0314;
                else if ( L"commareversedmod" == wsName ) return 0x02BD;
                else if ( L"commasmall" == wsName ) return 0xFE50;
                else if ( L"commasuperior" == wsName ) return 0xF6E2;
                else if ( L"commaturnedabovecmb" == wsName ) return 0x0312;
                else if ( L"commaturnedmod" == wsName ) return 0x02BB;
                else if ( L"compass" == wsName ) return 0x263C;
                else if ( L"congruent" == wsName ) return 0x2245;
                else if ( L"contourintegral" == wsName ) return 0x222E;
                else if ( L"control" == wsName ) return 0x2303;
                else if ( L"controlACK" == wsName ) return 0x0006;
                else if ( L"controlBEL" == wsName ) return 0x0007;
                else if ( L"controlBS" == wsName ) return 0x0008;
                else if ( L"controlCAN" == wsName ) return 0x0018;
                else if ( L"controlCR" == wsName ) return 0x000D;
                else if ( L"controlDC1" == wsName ) return 0x0011;
                else if ( L"controlDC2" == wsName ) return 0x0012;
                else if ( L"controlDC3" == wsName ) return 0x0013;
                else if ( L"controlDC4" == wsName ) return 0x0014;
                else if ( L"controlDEL" == wsName ) return 0x007F;
                else if ( L"controlDLE" == wsName ) return 0x0010;
                else if ( L"controlEM" == wsName ) return 0x0019;
                else if ( L"controlENQ" == wsName ) return 0x0005;
                else if ( L"controlEOT" == wsName ) return 0x0004;
                else if ( L"controlESC" == wsName ) return 0x001B;
                else if ( L"controlETB" == wsName ) return 0x0017;
                else if ( L"controlETX" == wsName ) return 0x0003;
                else if ( L"controlFF" == wsName ) return 0x000C;
                else if ( L"controlFS" == wsName ) return 0x001C;
                else if ( L"controlGS" == wsName ) return 0x001D;
                else if ( L"controlHT" == wsName ) return 0x0009;
                else if ( L"controlLF" == wsName ) return 0x000A;
                else if ( L"controlNAK" == wsName ) return 0x0015;
                else if ( L"controlRS" == wsName ) return 0x001E;
                else if ( L"controlSI" == wsName ) return 0x000F;
                else if ( L"controlSO" == wsName ) return 0x000E;
                else if ( L"controlSOT" == wsName ) return 0x0002;
                else if ( L"controlSTX" == wsName ) return 0x0001;
                else if ( L"controlSUB" == wsName ) return 0x001A;
                else if ( L"controlSYN" == wsName ) return 0x0016;
                else if ( L"controlUS" == wsName ) return 0x001F;
                else if ( L"controlVT" == wsName ) return 0x000B;
                else if ( L"copyright" == wsName ) return 0x00A9;
                else if ( L"copyrightsans" == wsName ) return 0xF8E9;
                else if ( L"copyrightserif" == wsName ) return 0xF6D9;
                else if ( L"cornerbracketleft" == wsName ) return 0x300C;
                else if ( L"cornerbracketlefthalfwidth" == wsName ) return 0xFF62;
                else if ( L"cornerbracketleftvertical" == wsName ) return 0xFE41;
                else if ( L"cornerbracketright" == wsName ) return 0x300D;
                else if ( L"cornerbracketrighthalfwidth" == wsName ) return 0xFF63;
                else if ( L"cornerbracketrightvertical" == wsName ) return 0xFE42;
                else if ( L"corporationsquare" == wsName ) return 0x337F;
                else if ( L"cosquare" == wsName ) return 0x33C7;
                else if ( L"coverkgsquare" == wsName ) return 0x33C6;
                break;
            }
        case 'p':
            {
                if      ( L"cparen" == wsName ) return 0x249E;
                break;
            }
        case 'r':
            {
                if      ( L"cruzeiro" == wsName ) return 0x20A2;
                break;
            }
        case 's':
            {
                if      ( L"cstretched" == wsName ) return 0x0297;
                break;
            }
        case 'u':
            {
                if      ( L"curlyand" == wsName ) return 0x22CF;
                else if ( L"curlyor" == wsName ) return 0x22CE;
                else if ( L"currency" == wsName ) return 0x00A4;
                break;
            }
        case 'y':
            {
                if      ( L"cyrBreve" == wsName ) return 0xF6D1;
                else if ( L"cyrFlex" == wsName ) return 0xF6D2;
                else if ( L"cyrbreve" == wsName ) return 0xF6D4;
                else if ( L"cyrflex" == wsName ) return 0xF6D5;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_d(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch( wChar1 )
        {
        case 0:
            {
                if      ( L"d" == wsName ) return 0x0064;
                break;
            }
        case 'a':
            {
                if      ( L"daarmenian" == wsName ) return 0x0564;
                else if ( L"dabengali" == wsName ) return 0x09A6;
                else if ( L"dadarabic" == wsName ) return 0x0636;
                else if ( L"dadeva" == wsName ) return 0x0926;
                else if ( L"dadfinalarabic" == wsName ) return 0xFEBE;
                else if ( L"dadinitialarabic" == wsName ) return 0xFEBF;
                else if ( L"dadmedialarabic" == wsName ) return 0xFEC0;
                else if ( L"dagesh" == wsName ) return 0x05BC;
                else if ( L"dageshhebrew" == wsName ) return 0x05BC;
                else if ( L"dagger" == wsName ) return 0x2020;
                else if ( L"daggerdbl" == wsName ) return 0x2021;
                else if ( L"dagujarati" == wsName ) return 0x0AA6;
                else if ( L"dagurmukhi" == wsName ) return 0x0A26;
                else if ( L"dahiragana" == wsName ) return 0x3060;
                else if ( L"dakatakana" == wsName ) return 0x30C0;
                else if ( L"dalarabic" == wsName ) return 0x062F;
                else if ( L"dalet" == wsName ) return 0x05D3;
                else if ( L"daletdagesh" == wsName ) return 0xFB33;
                else if ( L"daletdageshhebrew" == wsName ) return 0xFB33;
                else if ( L"dalethatafpatah" == wsName ) return 0x05D305B2;
                else if ( L"dalethatafpatahhebrew" == wsName ) return 0x05D305B2;
                else if ( L"dalethatafsegol" == wsName ) return 0x05D305B1;
                else if ( L"dalethatafsegolhebrew" == wsName ) return 0x05D305B1;
                else if ( L"dalethebrew" == wsName ) return 0x05D3;
                else if ( L"dalethiriq" == wsName ) return 0x05D305B4;
                else if ( L"dalethiriqhebrew" == wsName ) return 0x05D305B4;
                else if ( L"daletholam" == wsName ) return 0x05D305B9;
                else if ( L"daletholamhebrew" == wsName ) return 0x05D305B9;
                else if ( L"daletpatah" == wsName ) return 0x05D305B7;
                else if ( L"daletpatahhebrew" == wsName ) return 0x05D305B7;
                else if ( L"daletqamats" == wsName ) return 0x05D305B8;
                else if ( L"daletqamatshebrew" == wsName ) return 0x05D305B8;
                else if ( L"daletqubuts" == wsName ) return 0x05D305BB;
                else if ( L"daletqubutshebrew" == wsName ) return 0x05D305BB;
                else if ( L"daletsegol" == wsName ) return 0x05D305B6;
                else if ( L"daletsegolhebrew" == wsName ) return 0x05D305B6;
                else if ( L"daletsheva" == wsName ) return 0x05D305B0;
                else if ( L"daletshevahebrew" == wsName ) return 0x05D305B0;
                else if ( L"dalettsere" == wsName ) return 0x05D305B5;
                else if ( L"dalettserehebrew" == wsName ) return 0x05D305B5;
                else if ( L"dalfinalarabic" == wsName ) return 0xFEAA;
                else if ( L"dammaarabic" == wsName ) return 0x064F;
                else if ( L"dammalowarabic" == wsName ) return 0x064F;
                else if ( L"dammatanaltonearabic" == wsName ) return 0x064C;
                else if ( L"dammatanarabic" == wsName ) return 0x064C;
                else if ( L"danda" == wsName ) return 0x0964;
                else if ( L"dargahebrew" == wsName ) return 0x05A7;
                else if ( L"dargalefthebrew" == wsName ) return 0x05A7;
                else if ( L"dasiapneumatacyrilliccmb" == wsName ) return 0x0485;
                break;
            }
        case 'b':
            {
                if      ( L"dblGrave" == wsName ) return 0xF6D3;
                else if ( L"dblanglebracketleft" == wsName ) return 0x300A;
                else if ( L"dblanglebracketleftvertical" == wsName ) return 0xFE3D;
                else if ( L"dblanglebracketright" == wsName ) return 0x300B;
                else if ( L"dblanglebracketrightvertical" == wsName ) return 0xFE3E;
                else if ( L"dblarchinvertedbelowcmb" == wsName ) return 0x032B;
                else if ( L"dblarrowleft" == wsName ) return 0x21D4;
                else if ( L"dblarrowright" == wsName ) return 0x21D2;
                else if ( L"dbldanda" == wsName ) return 0x0965;
                else if ( L"dblgrave" == wsName ) return 0xF6D6;
                else if ( L"dblgravecmb" == wsName ) return 0x030F;
                else if ( L"dblintegral" == wsName ) return 0x222C;
                else if ( L"dbllowline" == wsName ) return 0x2017;
                else if ( L"dbllowlinecmb" == wsName ) return 0x0333;
                else if ( L"dbloverlinecmb" == wsName ) return 0x033F;
                else if ( L"dblprimemod" == wsName ) return 0x02BA;
                else if ( L"dblverticalbar" == wsName ) return 0x2016;
                else if ( L"dblverticallineabovecmb" == wsName ) return 0x030E;
                else if ( L"dbopomofo" == wsName ) return 0x3109;
                else if ( L"dbsquare" == wsName ) return 0x33C8;
                break;
            }
        case 'c':
            {
                if      ( L"dcaron" == wsName ) return 0x010F;
                else if ( L"dcedilla" == wsName ) return 0x1E11;
                else if ( L"dcircle" == wsName ) return 0x24D3;
                else if ( L"dcircumflexbelow" == wsName ) return 0x1E13;
                else if ( L"dcroat" == wsName ) return 0x0111;
                break;
            }
        case 'd':
            {
                if      ( L"ddabengali" == wsName ) return 0x09A1;
                else if ( L"ddadeva" == wsName ) return 0x0921;
                else if ( L"ddagujarati" == wsName ) return 0x0AA1;
                else if ( L"ddagurmukhi" == wsName ) return 0x0A21;
                else if ( L"ddalarabic" == wsName ) return 0x0688;
                else if ( L"ddalfinalarabic" == wsName ) return 0xFB89;
                else if ( L"dddhadeva" == wsName ) return 0x095C;
                else if ( L"ddhabengali" == wsName ) return 0x09A2;
                else if ( L"ddhadeva" == wsName ) return 0x0922;
                else if ( L"ddhagujarati" == wsName ) return 0x0AA2;
                else if ( L"ddhagurmukhi" == wsName ) return 0x0A22;
                else if ( L"ddotaccent" == wsName ) return 0x1E0B;
                else if ( L"ddotbelow" == wsName ) return 0x1E0D;
                break;
            }
        case 'e':
            {
                if      ( L"decimalseparatorarabic" == wsName ) return 0x066B;
                else if ( L"decimalseparatorpersian" == wsName ) return 0x066B;
                else if ( L"decyrillic" == wsName ) return 0x0434;
                else if ( L"degree" == wsName ) return 0x00B0;
                else if ( L"dehihebrew" == wsName ) return 0x05AD;
                else if ( L"dehiragana" == wsName ) return 0x3067;
                else if ( L"deicoptic" == wsName ) return 0x03EF;
                else if ( L"dekatakana" == wsName ) return 0x30C7;
                else if ( L"deleteleft" == wsName ) return 0x232B;
                else if ( L"deleteright" == wsName ) return 0x2326;
                else if ( L"delta" == wsName ) return 0x03B4;
                else if ( L"deltaturned" == wsName ) return 0x018D;
                else if ( L"denominatorminusonenumeratorbengali" == wsName ) return 0x09F8;
                else if ( L"dezh" == wsName ) return 0x02A4;
                break;
            }
        case 'h':
            {
                if      ( L"dhabengali" == wsName ) return 0x09A7;
                else if ( L"dhadeva" == wsName ) return 0x0927;
                else if ( L"dhagujarati" == wsName ) return 0x0AA7;
                else if ( L"dhagurmukhi" == wsName ) return 0x0A27;
                else if ( L"dhook" == wsName ) return 0x0257;
                break;
            }
        case 'i':
            {
                if      ( L"dialytikatonos" == wsName ) return 0x0385;
                else if ( L"dialytikatonoscmb" == wsName ) return 0x0344;
                else if ( L"diamond" == wsName ) return 0x2666;
                else if ( L"diamondsuitwhite" == wsName ) return 0x2662;
                else if ( L"dieresis" == wsName ) return 0x00A8;
                else if ( L"dieresisacute" == wsName ) return 0xF6D7;
                else if ( L"dieresisbelowcmb" == wsName ) return 0x0324;
                else if ( L"dieresiscmb" == wsName ) return 0x0308;
                else if ( L"dieresisgrave" == wsName ) return 0xF6D8;
                else if ( L"dieresistonos" == wsName ) return 0x0385;
                else if ( L"dihiragana" == wsName ) return 0x3062;
                else if ( L"dikatakana" == wsName ) return 0x30C2;
                else if ( L"dittomark" == wsName ) return 0x3003;
                else if ( L"divide" == wsName ) return 0x00F7;
                else if ( L"divides" == wsName ) return 0x2223;
                else if ( L"divisionslash" == wsName ) return 0x2215;
                break;
            }
        case 'j':
            {
                if      ( L"djecyrillic" == wsName ) return 0x0452;
                break;
            }
        case 'k':
            {
                if      ( L"dkshade" == wsName ) return 0x2593;
                break;
            }
        case 'l':
            {
                if      ( L"dlinebelow" == wsName ) return 0x1E0F;
                else if ( L"dlsquare" == wsName ) return 0x3397;
                break;
            }
        case 'm':
            {
                if      ( L"dmacron" == wsName ) return 0x0111;
                else if ( L"dmonospace" == wsName ) return 0xFF44;
                break;
            }
        case 'n':
            {
                if      ( L"dnblock" == wsName ) return 0x2584;
                break;
            }
        case 'o':
            {
                if      ( L"dochadathai" == wsName ) return 0x0E0E;
                else if ( L"dodekthai" == wsName ) return 0x0E14;
                else if ( L"dohiragana" == wsName ) return 0x3069;
                else if ( L"dokatakana" == wsName ) return 0x30C9;
                else if ( L"dollar" == wsName ) return 0x0024;
                else if ( L"dollarinferior" == wsName ) return 0xF6E3;
                else if ( L"dollarmonospace" == wsName ) return 0xFF04;
                else if ( L"dollaroldstyle" == wsName ) return 0xF724;
                else if ( L"dollarsmall" == wsName ) return 0xFE69;
                else if ( L"dollarsuperior" == wsName ) return 0xF6E4;
                else if ( L"dong" == wsName ) return 0x20AB;
                else if ( L"dorusquare" == wsName ) return 0x3326;
                else if ( L"dotaccent" == wsName ) return 0x02D9;
                else if ( L"dotaccentcmb" == wsName ) return 0x0307;
                else if ( L"dotbelowcmb" == wsName ) return 0x0323;
                else if ( L"dotbelowcomb" == wsName ) return 0x0323;
                else if ( L"dotkatakana" == wsName ) return 0x30FB;
                else if ( L"dotlessi" == wsName ) return 0x0131;
                else if ( L"dotlessj" == wsName ) return 0xF6BE;
                else if ( L"dotlessjstrokehook" == wsName ) return 0x0284;
                else if ( L"dotmath" == wsName ) return 0x22C5;
                else if ( L"dottedcircle" == wsName ) return 0x25CC;
                else if ( L"doubleyodpatah" == wsName ) return 0xFB1F;
                else if ( L"doubleyodpatahhebrew" == wsName ) return 0xFB1F;
                else if ( L"downtackbelowcmb" == wsName ) return 0x031E;
                else if ( L"downtackmod" == wsName ) return 0x02D5;
                break;
            }
        case 'p':
            {
                if      ( L"dparen" == wsName ) return 0x249F;
                break;
            }
        case 's':
            {
                if      ( L"dsuperior" == wsName ) return 0xF6EB;
                break;
            }
        case 't':
            {
                if      ( L"dtail" == wsName ) return 0x0256;
                else if ( L"dtopbar" == wsName ) return 0x018C;
                break;
            }
        case 'u':
            {
                if      ( L"duhiragana" == wsName ) return 0x3065;
                else if ( L"dukatakana" == wsName ) return 0x30C5;
                break;
            }
        case 'z':
            {
                if      ( L"dz" == wsName ) return 0x01F3;
                else if ( L"dzaltone" == wsName ) return 0x02A3;
                else if ( L"dzcaron" == wsName ) return 0x01C6;
                else if ( L"dzcurl" == wsName ) return 0x02A5;
                else if ( L"dzeabkhasiancyrillic" == wsName ) return 0x04E1;
                else if ( L"dzecyrillic" == wsName ) return 0x0455;
                else if ( L"dzhecyrillic" == wsName ) return 0x045F;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_e(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch ( wChar1 )
        {
        case 0:
            {
                if      ( L"e" == wsName ) return 0x0065;
                break;
            }
        case 'a':
            {
                if      ( L"eacute" == wsName ) return 0x00E9;
                else if ( L"earth" == wsName ) return 0x2641;
                break;
            }
        case 'b':
            {
                if      ( L"ebengali" == wsName ) return 0x098F;
                else if ( L"ebopomofo" == wsName ) return 0x311C;
                else if ( L"ebreve" == wsName ) return 0x0115;
                break;
            }
        case 'c':
            {
                if      ( L"ecandradeva" == wsName ) return 0x090D;
                else if ( L"ecandragujarati" == wsName ) return 0x0A8D;
                else if ( L"ecandravowelsigndeva" == wsName ) return 0x0945;
                else if ( L"ecandravowelsigngujarati" == wsName ) return 0x0AC5;
                else if ( L"ecaron" == wsName ) return 0x011B;
                else if ( L"ecedillabreve" == wsName ) return 0x1E1D;
                else if ( L"echarmenian" == wsName ) return 0x0565;
                else if ( L"echyiwnarmenian" == wsName ) return 0x0587;
                else if ( L"ecircle" == wsName ) return 0x24D4;
                else if ( L"ecircumflex" == wsName ) return 0x00EA;
                else if ( L"ecircumflexacute" == wsName ) return 0x1EBF;
                else if ( L"ecircumflexbelow" == wsName ) return 0x1E19;
                else if ( L"ecircumflexdotbelow" == wsName ) return 0x1EC7;
                else if ( L"ecircumflexgrave" == wsName ) return 0x1EC1;
                else if ( L"ecircumflexhookabove" == wsName ) return 0x1EC3;
                else if ( L"ecircumflextilde" == wsName ) return 0x1EC5;
                else if ( L"ecyrillic" == wsName ) return 0x0454;
                break;
            }
        case 'd':
            {
                if      ( L"edblgrave" == wsName ) return 0x0205;
                else if ( L"edeva" == wsName ) return 0x090F;
                else if ( L"edieresis" == wsName ) return 0x00EB;
                else if ( L"edot" == wsName ) return 0x0117;
                else if ( L"edotaccent" == wsName ) return 0x0117;
                else if ( L"edotbelow" == wsName ) return 0x1EB9;
                break;
            }
        case 'e':
            {
                if      ( L"eegurmukhi" == wsName ) return 0x0A0F;
                else if ( L"eematragurmukhi" == wsName ) return 0x0A47;
                break;
            }
        case 'f':
            {
                if      ( L"efcyrillic" == wsName ) return 0x0444;
                break;
            }
        case 'g':
            {
                if      ( L"egrave" == wsName ) return 0x00E8;
                else if ( L"egujarati" == wsName ) return 0x0A8F;
                break;
            }
        case 'h':
            {
                if      ( L"eharmenian" == wsName ) return 0x0567;
                else if ( L"ehbopomofo" == wsName ) return 0x311D;
                else if ( L"ehiragana" == wsName ) return 0x3048;
                else if ( L"ehookabove" == wsName ) return 0x1EBB;
                break;
            }
        case 'i':
            {
                if      ( L"eibopomofo" == wsName ) return 0x311F;
                else if ( L"eight" == wsName ) return 0x0038;
                else if ( L"eightarabic" == wsName ) return 0x0668;
                else if ( L"eightbengali" == wsName ) return 0x09EE;
                else if ( L"eightcircle" == wsName ) return 0x2467;
                else if ( L"eightcircleinversesansserif" == wsName ) return 0x2791;
                else if ( L"eightdeva" == wsName ) return 0x096E;
                else if ( L"eighteencircle" == wsName ) return 0x2471;
                else if ( L"eighteenparen" == wsName ) return 0x2485;
                else if ( L"eighteenperiod" == wsName ) return 0x2499;
                else if ( L"eightgujarati" == wsName ) return 0x0AEE;
                else if ( L"eightgurmukhi" == wsName ) return 0x0A6E;
                else if ( L"eighthackarabic" == wsName ) return 0x0668;
                else if ( L"eighthangzhou" == wsName ) return 0x3028;
                else if ( L"eighthnotebeamed" == wsName ) return 0x266B;
                else if ( L"eightideographicparen" == wsName ) return 0x3227;
                else if ( L"eightinferior" == wsName ) return 0x2088;
                else if ( L"eightmonospace" == wsName ) return 0xFF18;
                else if ( L"eightoldstyle" == wsName ) return 0xF738;
                else if ( L"eightparen" == wsName ) return 0x247B;
                else if ( L"eightperiod" == wsName ) return 0x248F;
                else if ( L"eightpersian" == wsName ) return 0x06F8;
                else if ( L"eightroman" == wsName ) return 0x2177;
                else if ( L"eightsuperior" == wsName ) return 0x2078;
                else if ( L"eightthai" == wsName ) return 0x0E58;
                else if ( L"einvertedbreve" == wsName ) return 0x0207;
                else if ( L"eiotifiedcyrillic" == wsName ) return 0x0465;
                break;
            }
        case 'k':
            {
                if      ( L"ekatakana" == wsName ) return 0x30A8;
                else if ( L"ekatakanahalfwidth" == wsName ) return 0xFF74;
                else if ( L"ekonkargurmukhi" == wsName ) return 0x0A74;
                else if ( L"ekorean" == wsName ) return 0x3154;
                break;
            }
        case 'l':
            {
                if      ( L"elcyrillic" == wsName ) return 0x043B;
                else if ( L"element" == wsName ) return 0x2208;
                else if ( L"elevencircle" == wsName ) return 0x246A;
                else if ( L"elevenparen" == wsName ) return 0x247E;
                else if ( L"elevenperiod" == wsName ) return 0x2492;
                else if ( L"elevenroman" == wsName ) return 0x217A;
                else if ( L"ellipsis" == wsName ) return 0x2026;
                else if ( L"ellipsisvertical" == wsName ) return 0x22EE;
                break;
            }
        case 'm':
            {
                if      ( L"emacron" == wsName ) return 0x0113;
                else if ( L"emacronacute" == wsName ) return 0x1E17;
                else if ( L"emacrongrave" == wsName ) return 0x1E15;
                else if ( L"emcyrillic" == wsName ) return 0x043C;
                else if ( L"emdash" == wsName ) return 0x2014;
                else if ( L"emdashvertical" == wsName ) return 0xFE31;
                else if ( L"emonospace" == wsName ) return 0xFF45;
                else if ( L"emphasismarkarmenian" == wsName ) return 0x055B;
                else if ( L"emptyset" == wsName ) return 0x2205;
                break;
            }
        case 'n':
            {
                if      ( L"enbopomofo" == wsName ) return 0x3123;
                else if ( L"encyrillic" == wsName ) return 0x043D;
                else if ( L"endash" == wsName ) return 0x2013;
                else if ( L"endashvertical" == wsName ) return 0xFE32;
                else if ( L"endescendercyrillic" == wsName ) return 0x04A3;
                else if ( L"eng" == wsName ) return 0x014B;
                else if ( L"engbopomofo" == wsName ) return 0x3125;
                else if ( L"enghecyrillic" == wsName ) return 0x04A5;
                else if ( L"enhookcyrillic" == wsName ) return 0x04C8;
                else if ( L"enspace" == wsName ) return 0x2002;
                break;
            }
        case 'o':
            {
                if      ( L"eogonek" == wsName ) return 0x0119;
                else if ( L"eokorean" == wsName ) return 0x3153;
                else if ( L"eopen" == wsName ) return 0x025B;
                else if ( L"eopenclosed" == wsName ) return 0x029A;
                else if ( L"eopenreversed" == wsName ) return 0x025C;
                else if ( L"eopenreversedclosed" == wsName ) return 0x025E;
                else if ( L"eopenreversedhook" == wsName ) return 0x025D;
                break;
            }
        case 'p':
            {
                if      ( L"eparen" == wsName ) return 0x24A0;
                else if ( L"epsilon" == wsName ) return 0x03B5;
                else if ( L"epsilontonos" == wsName ) return 0x03AD;
                break;
            }
        case 'q':
            {
                if      ( L"equal" == wsName ) return 0x003D;
                else if ( L"equalmonospace" == wsName ) return 0xFF1D;
                else if ( L"equalsmall" == wsName ) return 0xFE66;
                else if ( L"equalsuperior" == wsName ) return 0x207C;
                else if ( L"equivalence" == wsName ) return 0x2261;
                break;
            }
        case 'r':
            {
                if      ( L"erbopomofo" == wsName ) return 0x3126;
                else if ( L"ercyrillic" == wsName ) return 0x0440;
                else if ( L"ereversed" == wsName ) return 0x0258;
                else if ( L"ereversedcyrillic" == wsName ) return 0x044D;
                break;
            }
        case 's':
            {
                if      ( L"escyrillic" == wsName ) return 0x0441;
                else if ( L"esdescendercyrillic" == wsName ) return 0x04AB;
                else if ( L"esh" == wsName ) return 0x0283;
                else if ( L"eshcurl" == wsName ) return 0x0286;
                else if ( L"eshortdeva" == wsName ) return 0x090E;
                else if ( L"eshortvowelsigndeva" == wsName ) return 0x0946;
                else if ( L"eshreversedloop" == wsName ) return 0x01AA;
                else if ( L"eshsquatreversed" == wsName ) return 0x0285;
                else if ( L"esmallhiragana" == wsName ) return 0x3047;
                else if ( L"esmallkatakana" == wsName ) return 0x30A7;
                else if ( L"esmallkatakanahalfwidth" == wsName ) return 0xFF6A;
                else if ( L"estimated" == wsName ) return 0x212E;
                else if ( L"esuperior" == wsName ) return 0xF6EC;
                break;
            }
        case 't':
            {
                if      ( L"eta" == wsName ) return 0x03B7;
                else if ( L"etarmenian" == wsName ) return 0x0568;
                else if ( L"etatonos" == wsName ) return 0x03AE;
                else if ( L"eth" == wsName ) return 0x00F0;
                else if ( L"etilde" == wsName ) return 0x1EBD;
                else if ( L"etildebelow" == wsName ) return 0x1E1B;
                else if ( L"etnahtafoukhhebrew" == wsName ) return 0x0591;
                else if ( L"etnahtafoukhlefthebrew" == wsName ) return 0x0591;
                else if ( L"etnahtahebrew" == wsName ) return 0x0591;
                else if ( L"etnahtalefthebrew" == wsName ) return 0x0591;
                else if ( L"eturned" == wsName ) return 0x01DD;
                break;
            }
        case 'u':
            {
                if      ( L"eukorean" == wsName ) return 0x3161;
                else if ( L"euro" == wsName ) return 0x20AC;
                break;
            }
        case 'v':
            {
                if      ( L"evowelsignbengali" == wsName ) return 0x09C7;
                else if ( L"evowelsigndeva" == wsName ) return 0x0947;
                else if ( L"evowelsigngujarati" == wsName ) return 0x0AC7;
                break;
            }
        case 'x':
            {
                if      ( L"exclam" == wsName ) return 0x0021;
                else if ( L"exclamarmenian" == wsName ) return 0x055C;
                else if ( L"exclamdbl" == wsName ) return 0x203C;
                else if ( L"exclamdown" == wsName ) return 0x00A1;
                else if ( L"exclamdownsmall" == wsName ) return 0xF7A1;
                else if ( L"exclammonospace" == wsName ) return 0xFF01;
                else if ( L"exclamsmall" == wsName ) return 0xF721;
                else if ( L"existential" == wsName ) return 0x2203;
                break;
            }
        case 'z':
            {
                if      ( L"ezh" == wsName ) return 0x0292;
                else if ( L"ezhcaron" == wsName ) return 0x01EF;
                else if ( L"ezhcurl" == wsName ) return 0x0293;
                else if ( L"ezhreversed" == wsName ) return 0x01B9;
                else if ( L"ezhtail" == wsName ) return 0x01BA;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_f(CWCharWrapper &wsName)
    {
        if      ( L"f" == wsName ) return 0x0066;
        else if ( L"fadeva" == wsName ) return 0x095E;
        else if ( L"fagurmukhi" == wsName ) return 0x0A5E;
        else if ( L"fahrenheit" == wsName ) return 0x2109;
        else if ( L"fathaarabic" == wsName ) return 0x064E;
        else if ( L"fathalowarabic" == wsName ) return 0x064E;
        else if ( L"fathatanarabic" == wsName ) return 0x064B;
        else if ( L"fbopomofo" == wsName ) return 0x3108;
        else if ( L"fcircle" == wsName ) return 0x24D5;
        else if ( L"fdotaccent" == wsName ) return 0x1E1F;
        else if ( L"feharabic" == wsName ) return 0x0641;
        else if ( L"feharmenian" == wsName ) return 0x0586;
        else if ( L"fehfinalarabic" == wsName ) return 0xFED2;
        else if ( L"fehinitialarabic" == wsName ) return 0xFED3;
        else if ( L"fehmedialarabic" == wsName ) return 0xFED4;
        else if ( L"feicoptic" == wsName ) return 0x03E5;
        else if ( L"female" == wsName ) return 0x2640;
        else if ( L"ff" == wsName ) return 0xFB00;
        else if ( L"ffi" == wsName ) return 0xFB03;
        else if ( L"ffl" == wsName ) return 0xFB04;
        else if ( L"fi" == wsName ) return 0xFB01;
        else if ( L"fifteencircle" == wsName ) return 0x246E;
        else if ( L"fifteenparen" == wsName ) return 0x2482;
        else if ( L"fifteenperiod" == wsName ) return 0x2496;
        else if ( L"figuredash" == wsName ) return 0x2012;
        else if ( L"filledbox" == wsName ) return 0x25A0;
        else if ( L"filledrect" == wsName ) return 0x25AC;
        else if ( L"finalkaf" == wsName ) return 0x05DA;
        else if ( L"finalkafdagesh" == wsName ) return 0xFB3A;
        else if ( L"finalkafdageshhebrew" == wsName ) return 0xFB3A;
        else if ( L"finalkafhebrew" == wsName ) return 0x05DA;
        else if ( L"finalkafqamats" == wsName ) return 0x05DA05B8;
        else if ( L"finalkafqamatshebrew" == wsName ) return 0x05DA05B8;
        else if ( L"finalkafsheva" == wsName ) return 0x05DA05B0;
        else if ( L"finalkafshevahebrew" == wsName ) return 0x05DA05B0;
        else if ( L"finalmem" == wsName ) return 0x05DD;
        else if ( L"finalmemhebrew" == wsName ) return 0x05DD;
        else if ( L"finalnun" == wsName ) return 0x05DF;
        else if ( L"finalnunhebrew" == wsName ) return 0x05DF;
        else if ( L"finalpe" == wsName ) return 0x05E3;
        else if ( L"finalpehebrew" == wsName ) return 0x05E3;
        else if ( L"finaltsadi" == wsName ) return 0x05E5;
        else if ( L"finaltsadihebrew" == wsName ) return 0x05E5;
        else if ( L"firsttonechinese" == wsName ) return 0x02C9;
        else if ( L"fisheye" == wsName ) return 0x25C9;
        else if ( L"fitacyrillic" == wsName ) return 0x0473;
        else if ( L"five" == wsName ) return 0x0035;
        else if ( L"fivearabic" == wsName ) return 0x0665;
        else if ( L"fivebengali" == wsName ) return 0x09EB;
        else if ( L"fivecircle" == wsName ) return 0x2464;
        else if ( L"fivecircleinversesansserif" == wsName ) return 0x278E;
        else if ( L"fivedeva" == wsName ) return 0x096B;
        else if ( L"fiveeighths" == wsName ) return 0x215D;
        else if ( L"fivegujarati" == wsName ) return 0x0AEB;
        else if ( L"fivegurmukhi" == wsName ) return 0x0A6B;
        else if ( L"fivehackarabic" == wsName ) return 0x0665;
        else if ( L"fivehangzhou" == wsName ) return 0x3025;
        else if ( L"fiveideographicparen" == wsName ) return 0x3224;
        else if ( L"fiveinferior" == wsName ) return 0x2085;
        else if ( L"fivemonospace" == wsName ) return 0xFF15;
        else if ( L"fiveoldstyle" == wsName ) return 0xF735;
        else if ( L"fiveparen" == wsName ) return 0x2478;
        else if ( L"fiveperiod" == wsName ) return 0x248C;
        else if ( L"fivepersian" == wsName ) return 0x06F5;
        else if ( L"fiveroman" == wsName ) return 0x2174;
        else if ( L"fivesuperior" == wsName ) return 0x2075;
        else if ( L"fivethai" == wsName ) return 0x0E55;
        else if ( L"fl" == wsName ) return 0xFB02;
        else if ( L"florin" == wsName ) return 0x0192;
        else if ( L"fmonospace" == wsName ) return 0xFF46;
        else if ( L"fmsquare" == wsName ) return 0x3399;
        else if ( L"fofanthai" == wsName ) return 0x0E1F;
        else if ( L"fofathai" == wsName ) return 0x0E1D;
        else if ( L"fongmanthai" == wsName ) return 0x0E4F;
        else if ( L"forall" == wsName ) return 0x2200;
        else if ( L"four" == wsName ) return 0x0034;
        else if ( L"fourarabic" == wsName ) return 0x0664;
        else if ( L"fourbengali" == wsName ) return 0x09EA;
        else if ( L"fourcircle" == wsName ) return 0x2463;
        else if ( L"fourcircleinversesansserif" == wsName ) return 0x278D;
        else if ( L"fourdeva" == wsName ) return 0x096A;
        else if ( L"fourgujarati" == wsName ) return 0x0AEA;
        else if ( L"fourgurmukhi" == wsName ) return 0x0A6A;
        else if ( L"fourhackarabic" == wsName ) return 0x0664;
        else if ( L"fourhangzhou" == wsName ) return 0x3024;
        else if ( L"fourideographicparen" == wsName ) return 0x3223;
        else if ( L"fourinferior" == wsName ) return 0x2084;
        else if ( L"fourmonospace" == wsName ) return 0xFF14;
        else if ( L"fournumeratorbengali" == wsName ) return 0x09F7;
        else if ( L"fouroldstyle" == wsName ) return 0xF734;
        else if ( L"fourparen" == wsName ) return 0x2477;
        else if ( L"fourperiod" == wsName ) return 0x248B;
        else if ( L"fourpersian" == wsName ) return 0x06F4;
        else if ( L"fourroman" == wsName ) return 0x2173;
        else if ( L"foursuperior" == wsName ) return 0x2074;
        else if ( L"fourteencircle" == wsName ) return 0x246D;
        else if ( L"fourteenparen" == wsName ) return 0x2481;
        else if ( L"fourteenperiod" == wsName ) return 0x2495;
        else if ( L"fourthai" == wsName ) return 0x0E54;
        else if ( L"fourthtonechinese" == wsName ) return 0x02CB;
        else if ( L"fparen" == wsName ) return 0x24A1;
        else if ( L"fraction" == wsName ) return 0x2044;
        else if ( L"franc" == wsName ) return 0x20A3;

        return 0;
    }
    static int Type1NameToUnicode_g(CWCharWrapper &wsName)
    {
        if      ( L"g" == wsName ) return 0x0067;
        else if ( L"gabengali" == wsName ) return 0x0997;
        else if ( L"gacute" == wsName ) return 0x01F5;
        else if ( L"gadeva" == wsName ) return 0x0917;
        else if ( L"gafarabic" == wsName ) return 0x06AF;
        else if ( L"gaffinalarabic" == wsName ) return 0xFB93;
        else if ( L"gafinitialarabic" == wsName ) return 0xFB94;
        else if ( L"gafmedialarabic" == wsName ) return 0xFB95;
        else if ( L"gagujarati" == wsName ) return 0x0A97;
        else if ( L"gagurmukhi" == wsName ) return 0x0A17;
        else if ( L"gahiragana" == wsName ) return 0x304C;
        else if ( L"gakatakana" == wsName ) return 0x30AC;
        else if ( L"gamma" == wsName ) return 0x03B3;
        else if ( L"gammalatinsmall" == wsName ) return 0x0263;
        else if ( L"gammasuperior" == wsName ) return 0x02E0;
        else if ( L"gangiacoptic" == wsName ) return 0x03EB;
        else if ( L"gbopomofo" == wsName ) return 0x310D;
        else if ( L"gbreve" == wsName ) return 0x011F;
        else if ( L"gcaron" == wsName ) return 0x01E7;
        else if ( L"gcedilla" == wsName ) return 0x0123;
        else if ( L"gcircle" == wsName ) return 0x24D6;
        else if ( L"gcircumflex" == wsName ) return 0x011D;
        else if ( L"gcommaaccent" == wsName ) return 0x0123;
        else if ( L"gdot" == wsName ) return 0x0121;
        else if ( L"gdotaccent" == wsName ) return 0x0121;
        else if ( L"gecyrillic" == wsName ) return 0x0433;
        else if ( L"gehiragana" == wsName ) return 0x3052;
        else if ( L"gekatakana" == wsName ) return 0x30B2;
        else if ( L"geometricallyequal" == wsName ) return 0x2251;
        else if ( L"gereshaccenthebrew" == wsName ) return 0x059C;
        else if ( L"gereshhebrew" == wsName ) return 0x05F3;
        else if ( L"gereshmuqdamhebrew" == wsName ) return 0x059D;
        else if ( L"germandbls" == wsName ) return 0x00DF;
        else if ( L"gershayimaccenthebrew" == wsName ) return 0x059E;
        else if ( L"gershayimhebrew" == wsName ) return 0x05F4;
        else if ( L"getamark" == wsName ) return 0x3013;
        else if ( L"ghabengali" == wsName ) return 0x0998;
        else if ( L"ghadarmenian" == wsName ) return 0x0572;
        else if ( L"ghadeva" == wsName ) return 0x0918;
        else if ( L"ghagujarati" == wsName ) return 0x0A98;
        else if ( L"ghagurmukhi" == wsName ) return 0x0A18;
        else if ( L"ghainarabic" == wsName ) return 0x063A;
        else if ( L"ghainfinalarabic" == wsName ) return 0xFECE;
        else if ( L"ghaininitialarabic" == wsName ) return 0xFECF;
        else if ( L"ghainmedialarabic" == wsName ) return 0xFED0;
        else if ( L"ghemiddlehookcyrillic" == wsName ) return 0x0495;
        else if ( L"ghestrokecyrillic" == wsName ) return 0x0493;
        else if ( L"gheupturncyrillic" == wsName ) return 0x0491;
        else if ( L"ghhadeva" == wsName ) return 0x095A;
        else if ( L"ghhagurmukhi" == wsName ) return 0x0A5A;
        else if ( L"ghook" == wsName ) return 0x0260;
        else if ( L"ghzsquare" == wsName ) return 0x3393;
        else if ( L"gihiragana" == wsName ) return 0x304E;
        else if ( L"gikatakana" == wsName ) return 0x30AE;
        else if ( L"gimarmenian" == wsName ) return 0x0563;
        else if ( L"gimel" == wsName ) return 0x05D2;
        else if ( L"gimeldagesh" == wsName ) return 0xFB32;
        else if ( L"gimeldageshhebrew" == wsName ) return 0xFB32;
        else if ( L"gimelhebrew" == wsName ) return 0x05D2;
        else if ( L"gjecyrillic" == wsName ) return 0x0453;
        else if ( L"glottalinvertedstroke" == wsName ) return 0x01BE;
        else if ( L"glottalstop" == wsName ) return 0x0294;
        else if ( L"glottalstopinverted" == wsName ) return 0x0296;
        else if ( L"glottalstopmod" == wsName ) return 0x02C0;
        else if ( L"glottalstopreversed" == wsName ) return 0x0295;
        else if ( L"glottalstopreversedmod" == wsName ) return 0x02C1;
        else if ( L"glottalstopreversedsuperior" == wsName ) return 0x02E4;
        else if ( L"glottalstopstroke" == wsName ) return 0x02A1;
        else if ( L"glottalstopstrokereversed" == wsName ) return 0x02A2;
        else if ( L"gmacron" == wsName ) return 0x1E21;
        else if ( L"gmonospace" == wsName ) return 0xFF47;
        else if ( L"gohiragana" == wsName ) return 0x3054;
        else if ( L"gokatakana" == wsName ) return 0x30B4;
        else if ( L"gparen" == wsName ) return 0x24A2;
        else if ( L"gpasquare" == wsName ) return 0x33AC;
        else if ( L"gradient" == wsName ) return 0x2207;
        else if ( L"grave" == wsName ) return 0x0060;
        else if ( L"gravebelowcmb" == wsName ) return 0x0316;
        else if ( L"gravecmb" == wsName ) return 0x0300;
        else if ( L"gravecomb" == wsName ) return 0x0300;
        else if ( L"gravedeva" == wsName ) return 0x0953;
        else if ( L"gravelowmod" == wsName ) return 0x02CE;
        else if ( L"gravemonospace" == wsName ) return 0xFF40;
        else if ( L"gravetonecmb" == wsName ) return 0x0340;
        else if ( L"greater" == wsName ) return 0x003E;
        else if ( L"greaterequal" == wsName ) return 0x2265;
        else if ( L"greaterequalorless" == wsName ) return 0x22DB;
        else if ( L"greatermonospace" == wsName ) return 0xFF1E;
        else if ( L"greaterorequivalent" == wsName ) return 0x2273;
        else if ( L"greaterorless" == wsName ) return 0x2277;
        else if ( L"greateroverequal" == wsName ) return 0x2267;
        else if ( L"greatersmall" == wsName ) return 0xFE65;
        else if ( L"gscript" == wsName ) return 0x0261;
        else if ( L"gstroke" == wsName ) return 0x01E5;
        else if ( L"guhiragana" == wsName ) return 0x3050;
        else if ( L"guillemotleft" == wsName ) return 0x00AB;
        else if ( L"guillemotright" == wsName ) return 0x00BB;
        else if ( L"guilsinglleft" == wsName ) return 0x2039;
        else if ( L"guilsinglright" == wsName ) return 0x203A;
        else if ( L"gukatakana" == wsName ) return 0x30B0;
        else if ( L"guramusquare" == wsName ) return 0x3318;
        else if ( L"gysquare" == wsName ) return 0x33C9;

        return 0;
    }
    static int Type1NameToUnicode_h(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch( wChar1 )
        {
        case 0:
            {
                if      ( L"h" == wsName ) return 0x0068;
                break;
            }
        case 'a':
            {
                if      ( L"haabkhasiancyrillic" == wsName ) return 0x04A9;
                else if ( L"haaltonearabic" == wsName ) return 0x06C1;
                else if ( L"habengali" == wsName ) return 0x09B9;
                else if ( L"hadescendercyrillic" == wsName ) return 0x04B3;
                else if ( L"hadeva" == wsName ) return 0x0939;
                else if ( L"hagujarati" == wsName ) return 0x0AB9;
                else if ( L"hagurmukhi" == wsName ) return 0x0A39;
                else if ( L"haharabic" == wsName ) return 0x062D;
                else if ( L"hahfinalarabic" == wsName ) return 0xFEA2;
                else if ( L"hahinitialarabic" == wsName ) return 0xFEA3;
                else if ( L"hahiragana" == wsName ) return 0x306F;
                else if ( L"hahmedialarabic" == wsName ) return 0xFEA4;
                else if ( L"haitusquare" == wsName ) return 0x332A;
                else if ( L"hakatakana" == wsName ) return 0x30CF;
                else if ( L"hakatakanahalfwidth" == wsName ) return 0xFF8A;
                else if ( L"halantgurmukhi" == wsName ) return 0x0A4D;
                else if ( L"hamzaarabic" == wsName ) return 0x0621;
                else if ( L"hamzadammaarabic" == wsName ) return 0x0621064F;
                else if ( L"hamzadammatanarabic" == wsName ) return 0x0621064C;
                else if ( L"hamzafathaarabic" == wsName ) return 0x0621064E;
                else if ( L"hamzafathatanarabic" == wsName ) return 0x0621064B;
                else if ( L"hamzalowarabic" == wsName ) return 0x0621;
                else if ( L"hamzalowkasraarabic" == wsName ) return 0x06210650;
                else if ( L"hamzalowkasratanarabic" == wsName ) return 0x0621064D;
                else if ( L"hamzasukunarabic" == wsName ) return 0x06210652;
                else if ( L"hangulfiller" == wsName ) return 0x3164;
                else if ( L"hardsigncyrillic" == wsName ) return 0x044A;
                else if ( L"harpoonleftbarbup" == wsName ) return 0x21BC;
                else if ( L"harpoonrightbarbup" == wsName ) return 0x21C0;
                else if ( L"hasquare" == wsName ) return 0x33CA;
                else if ( L"hatafpatah" == wsName ) return 0x05B2;
                else if ( L"hatafpatah16" == wsName ) return 0x05B2;
                else if ( L"hatafpatah23" == wsName ) return 0x05B2;
                else if ( L"hatafpatah2f" == wsName ) return 0x05B2;
                else if ( L"hatafpatahhebrew" == wsName ) return 0x05B2;
                else if ( L"hatafpatahnarrowhebrew" == wsName ) return 0x05B2;
                else if ( L"hatafpatahquarterhebrew" == wsName ) return 0x05B2;
                else if ( L"hatafpatahwidehebrew" == wsName ) return 0x05B2;
                else if ( L"hatafqamats" == wsName ) return 0x05B3;
                else if ( L"hatafqamats1b" == wsName ) return 0x05B3;
                else if ( L"hatafqamats28" == wsName ) return 0x05B3;
                else if ( L"hatafqamats34" == wsName ) return 0x05B3;
                else if ( L"hatafqamatshebrew" == wsName ) return 0x05B3;
                else if ( L"hatafqamatsnarrowhebrew" == wsName ) return 0x05B3;
                else if ( L"hatafqamatsquarterhebrew" == wsName ) return 0x05B3;
                else if ( L"hatafqamatswidehebrew" == wsName ) return 0x05B3;
                else if ( L"hatafsegol" == wsName ) return 0x05B1;
                else if ( L"hatafsegol17" == wsName ) return 0x05B1;
                else if ( L"hatafsegol24" == wsName ) return 0x05B1;
                else if ( L"hatafsegol30" == wsName ) return 0x05B1;
                else if ( L"hatafsegolhebrew" == wsName ) return 0x05B1;
                else if ( L"hatafsegolnarrowhebrew" == wsName ) return 0x05B1;
                else if ( L"hatafsegolquarterhebrew" == wsName ) return 0x05B1;
                else if ( L"hatafsegolwidehebrew" == wsName ) return 0x05B1;
                break;
            }
        case 'b':
            {
                if      ( L"hbar" == wsName ) return 0x0127;
                else if ( L"hbopomofo" == wsName ) return 0x310F;
                else if ( L"hbrevebelow" == wsName ) return 0x1E2B;
                break;
            }
        case 'c':
            {
                if      ( L"hcedilla" == wsName ) return 0x1E29;
                else if ( L"hcircle" == wsName ) return 0x24D7;
                else if ( L"hcircumflex" == wsName ) return 0x0125;
                break;
            }
        case 'd':
            {
                if      ( L"hdieresis" == wsName ) return 0x1E27;
                else if ( L"hdotaccent" == wsName ) return 0x1E23;
                else if ( L"hdotbelow" == wsName ) return 0x1E25;
                break;
            }
        case 'e':
            {
                if      ( L"he" == wsName ) return 0x05D4;
                else if ( L"heart" == wsName ) return 0x2665;
                else if ( L"heartsuitblack" == wsName ) return 0x2665;
                else if ( L"heartsuitwhite" == wsName ) return 0x2661;
                else if ( L"hedagesh" == wsName ) return 0xFB34;
                else if ( L"hedageshhebrew" == wsName ) return 0xFB34;
                else if ( L"hehaltonearabic" == wsName ) return 0x06C1;
                else if ( L"heharabic" == wsName ) return 0x0647;
                else if ( L"hehebrew" == wsName ) return 0x05D4;
                else if ( L"hehfinalaltonearabic" == wsName ) return 0xFBA7;
                else if ( L"hehfinalalttwoarabic" == wsName ) return 0xFEEA;
                else if ( L"hehfinalarabic" == wsName ) return 0xFEEA;
                else if ( L"hehhamzaabovefinalarabic" == wsName ) return 0xFBA5;
                else if ( L"hehhamzaaboveisolatedarabic" == wsName ) return 0xFBA4;
                else if ( L"hehinitialaltonearabic" == wsName ) return 0xFBA8;
                else if ( L"hehinitialarabic" == wsName ) return 0xFEEB;
                else if ( L"hehiragana" == wsName ) return 0x3078;
                else if ( L"hehmedialaltonearabic" == wsName ) return 0xFBA9;
                else if ( L"hehmedialarabic" == wsName ) return 0xFEEC;
                else if ( L"heiseierasquare" == wsName ) return 0x337B;
                else if ( L"hekatakana" == wsName ) return 0x30D8;
                else if ( L"hekatakanahalfwidth" == wsName ) return 0xFF8D;
                else if ( L"hekutaarusquare" == wsName ) return 0x3336;
                else if ( L"henghook" == wsName ) return 0x0267;
                else if ( L"herutusquare" == wsName ) return 0x3339;
                else if ( L"het" == wsName ) return 0x05D7;
                else if ( L"hethebrew" == wsName ) return 0x05D7;
                break;
            }
        case 'h':
            {
                if      ( L"hhook" == wsName ) return 0x0266;
                else if ( L"hhooksuperior" == wsName ) return 0x02B1;
                break;
            }
        case 'i':
            {
                if      ( L"hieuhacirclekorean" == wsName ) return 0x327B;
                else if ( L"hieuhaparenkorean" == wsName ) return 0x321B;
                else if ( L"hieuhcirclekorean" == wsName ) return 0x326D;
                else if ( L"hieuhkorean" == wsName ) return 0x314E;
                else if ( L"hieuhparenkorean" == wsName ) return 0x320D;
                else if ( L"hihiragana" == wsName ) return 0x3072;
                else if ( L"hikatakana" == wsName ) return 0x30D2;
                else if ( L"hikatakanahalfwidth" == wsName ) return 0xFF8B;
                else if ( L"hiriq" == wsName ) return 0x05B4;
                else if ( L"hiriq14" == wsName ) return 0x05B4;
                else if ( L"hiriq21" == wsName ) return 0x05B4;
                else if ( L"hiriq2d" == wsName ) return 0x05B4;
                else if ( L"hiriqhebrew" == wsName ) return 0x05B4;
                else if ( L"hiriqnarrowhebrew" == wsName ) return 0x05B4;
                else if ( L"hiriqquarterhebrew" == wsName ) return 0x05B4;
                else if ( L"hiriqwidehebrew" == wsName ) return 0x05B4;
                break;
            }
        case 'l':
            {
                if      ( L"hlinebelow" == wsName ) return 0x1E96;
                break;
            }
        case 'm':
            {
                if      ( L"hmonospace" == wsName ) return 0xFF48;
                break;
            }
        case 'o':
            {
                if      ( L"hoarmenian" == wsName ) return 0x0570;
                else if ( L"hohipthai" == wsName ) return 0x0E2B;
                else if ( L"hohiragana" == wsName ) return 0x307B;
                else if ( L"hokatakana" == wsName ) return 0x30DB;
                else if ( L"hokatakanahalfwidth" == wsName ) return 0xFF8E;
                else if ( L"holam" == wsName ) return 0x05B9;
                else if ( L"holam19" == wsName ) return 0x05B9;
                else if ( L"holam26" == wsName ) return 0x05B9;
                else if ( L"holam32" == wsName ) return 0x05B9;
                else if ( L"holamhebrew" == wsName ) return 0x05B9;
                else if ( L"holamnarrowhebrew" == wsName ) return 0x05B9;
                else if ( L"holamquarterhebrew" == wsName ) return 0x05B9;
                else if ( L"holamwidehebrew" == wsName ) return 0x05B9;
                else if ( L"honokhukthai" == wsName ) return 0x0E2E;
                else if ( L"hookabovecomb" == wsName ) return 0x0309;
                else if ( L"hookcmb" == wsName ) return 0x0309;
                else if ( L"hookpalatalizedbelowcmb" == wsName ) return 0x0321;
                else if ( L"hookretroflexbelowcmb" == wsName ) return 0x0322;
                else if ( L"hoonsquare" == wsName ) return 0x3342;
                else if ( L"horicoptic" == wsName ) return 0x03E9;
                else if ( L"horizontalbar" == wsName ) return 0x2015;
                else if ( L"horncmb" == wsName ) return 0x031B;
                else if ( L"hotsprings" == wsName ) return 0x2668;
                else if ( L"house" == wsName ) return 0x2302;
                break;
            }
        case 'p':
            {
                if      ( L"hparen" == wsName ) return 0x24A3;
                break;
            }
        case 's':
            {
                if      ( L"hsuperior" == wsName ) return 0x02B0;
                break;
            }
        case 't':
            {
                if      ( L"hturned" == wsName ) return 0x0265;
                break;
            }
        case 'u':
            {
                if      ( L"huhiragana" == wsName ) return 0x3075;
                else if ( L"huiitosquare" == wsName ) return 0x3333;
                else if ( L"hukatakana" == wsName ) return 0x30D5;
                else if ( L"hukatakanahalfwidth" == wsName ) return 0xFF8C;
                else if ( L"hungarumlaut" == wsName ) return 0x02DD;
                else if ( L"hungarumlautcmb" == wsName ) return 0x030B;
                break;
            }
        case 'v':
            {
                if      ( L"hv" == wsName ) return 0x0195;
                break;
            }
        case 'y':
            {
                if      ( L"hyphen" == wsName ) return 0x002D;
                else if ( L"hypheninferior" == wsName ) return 0xF6E5;
                else if ( L"hyphenmonospace" == wsName ) return 0xFF0D;
                else if ( L"hyphensmall" == wsName ) return 0xFE63;
                else if ( L"hyphensuperior" == wsName ) return 0xF6E6;
                else if ( L"hyphentwo" == wsName ) return 0x2010;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_i(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch( wChar1 )
        {
        case 0:
            {
                if      ( L"i" == wsName ) return 0x0069;
                break;
            }
        case 'a':
            {
                if      ( L"iacute" == wsName ) return 0x00ED;
                else if ( L"iacyrillic" == wsName ) return 0x044F;
                break;
            }
        case 'b':
            {
                if      ( L"ibengali" == wsName ) return 0x0987;
                else if ( L"ibopomofo" == wsName ) return 0x3127;
                else if ( L"ibreve" == wsName ) return 0x012D;
                break;
            }
        case 'c':
            {
                if      ( L"icaron" == wsName ) return 0x01D0;
                else if ( L"icircle" == wsName ) return 0x24D8;
                else if ( L"icircumflex" == wsName ) return 0x00EE;
                else if ( L"icyrillic" == wsName ) return 0x0456;
                break;
            }
        case 'd':
            {
                if      ( L"idblgrave" == wsName ) return 0x0209;
                else if ( L"ideographearthcircle" == wsName ) return 0x328F;
                else if ( L"ideographfirecircle" == wsName ) return 0x328B;
                else if ( L"ideographicallianceparen" == wsName ) return 0x323F;
                else if ( L"ideographiccallparen" == wsName ) return 0x323A;
                else if ( L"ideographiccentrecircle" == wsName ) return 0x32A5;
                else if ( L"ideographicclose" == wsName ) return 0x3006;
                else if ( L"ideographiccomma" == wsName ) return 0x3001;
                else if ( L"ideographiccommaleft" == wsName ) return 0xFF64;
                else if ( L"ideographiccongratulationparen" == wsName ) return 0x3237;
                else if ( L"ideographiccorrectcircle" == wsName ) return 0x32A3;
                else if ( L"ideographicearthparen" == wsName ) return 0x322F;
                else if ( L"ideographicenterpriseparen" == wsName ) return 0x323D;
                else if ( L"ideographicexcellentcircle" == wsName ) return 0x329D;
                else if ( L"ideographicfestivalparen" == wsName ) return 0x3240;
                else if ( L"ideographicfinancialcircle" == wsName ) return 0x3296;
                else if ( L"ideographicfinancialparen" == wsName ) return 0x3236;
                else if ( L"ideographicfireparen" == wsName ) return 0x322B;
                else if ( L"ideographichaveparen" == wsName ) return 0x3232;
                else if ( L"ideographichighcircle" == wsName ) return 0x32A4;
                else if ( L"ideographiciterationmark" == wsName ) return 0x3005;
                else if ( L"ideographiclaborcircle" == wsName ) return 0x3298;
                else if ( L"ideographiclaborparen" == wsName ) return 0x3238;
                else if ( L"ideographicleftcircle" == wsName ) return 0x32A7;
                else if ( L"ideographiclowcircle" == wsName ) return 0x32A6;
                else if ( L"ideographicmedicinecircle" == wsName ) return 0x32A9;
                else if ( L"ideographicmetalparen" == wsName ) return 0x322E;
                else if ( L"ideographicmoonparen" == wsName ) return 0x322A;
                else if ( L"ideographicnameparen" == wsName ) return 0x3234;
                else if ( L"ideographicperiod" == wsName ) return 0x3002;
                else if ( L"ideographicprintcircle" == wsName ) return 0x329E;
                else if ( L"ideographicreachparen" == wsName ) return 0x3243;
                else if ( L"ideographicrepresentparen" == wsName ) return 0x3239;
                else if ( L"ideographicresourceparen" == wsName ) return 0x323E;
                else if ( L"ideographicrightcircle" == wsName ) return 0x32A8;
                else if ( L"ideographicsecretcircle" == wsName ) return 0x3299;
                else if ( L"ideographicselfparen" == wsName ) return 0x3242;
                else if ( L"ideographicsocietyparen" == wsName ) return 0x3233;
                else if ( L"ideographicspace" == wsName ) return 0x3000;
                else if ( L"ideographicspecialparen" == wsName ) return 0x3235;
                else if ( L"ideographicstockparen" == wsName ) return 0x3231;
                else if ( L"ideographicstudyparen" == wsName ) return 0x323B;
                else if ( L"ideographicsunparen" == wsName ) return 0x3230;
                else if ( L"ideographicsuperviseparen" == wsName ) return 0x323C;
                else if ( L"ideographicwaterparen" == wsName ) return 0x322C;
                else if ( L"ideographicwoodparen" == wsName ) return 0x322D;
                else if ( L"ideographiczero" == wsName ) return 0x3007;
                else if ( L"ideographmetalcircle" == wsName ) return 0x328E;
                else if ( L"ideographmooncircle" == wsName ) return 0x328A;
                else if ( L"ideographnamecircle" == wsName ) return 0x3294;
                else if ( L"ideographsuncircle" == wsName ) return 0x3290;
                else if ( L"ideographwatercircle" == wsName ) return 0x328C;
                else if ( L"ideographwoodcircle" == wsName ) return 0x328D;
                else if ( L"ideva" == wsName ) return 0x0907;
                else if ( L"idieresis" == wsName ) return 0x00EF;
                else if ( L"idieresisacute" == wsName ) return 0x1E2F;
                else if ( L"idieresiscyrillic" == wsName ) return 0x04E5;
                else if ( L"idotbelow" == wsName ) return 0x1ECB;
                break;
            }
        case 'e':
            {
                if      ( L"iebrevecyrillic" == wsName ) return 0x04D7;
                else if ( L"iecyrillic" == wsName ) return 0x0435;
                else if ( L"ieungacirclekorean" == wsName ) return 0x3275;
                else if ( L"ieungaparenkorean" == wsName ) return 0x3215;
                else if ( L"ieungcirclekorean" == wsName ) return 0x3267;
                else if ( L"ieungkorean" == wsName ) return 0x3147;
                else if ( L"ieungparenkorean" == wsName ) return 0x3207;
                break;
            }
        case 'g':
            {
                if      ( L"igrave" == wsName ) return 0x00EC;
                else if ( L"igujarati" == wsName ) return 0x0A87;
                else if ( L"igurmukhi" == wsName ) return 0x0A07;
                break;
            }
        case 'h':
            {
                if      ( L"ihiragana" == wsName ) return 0x3044;
                else if ( L"ihookabove" == wsName ) return 0x1EC9;
                break;
            }
        case 'i':
            {
                if      ( L"iibengali" == wsName ) return 0x0988;
                else if ( L"iicyrillic" == wsName ) return 0x0438;
                else if ( L"iideva" == wsName ) return 0x0908;
                else if ( L"iigujarati" == wsName ) return 0x0A88;
                else if ( L"iigurmukhi" == wsName ) return 0x0A08;
                else if ( L"iimatragurmukhi" == wsName ) return 0x0A40;
                else if ( L"iinvertedbreve" == wsName ) return 0x020B;
                else if ( L"iishortcyrillic" == wsName ) return 0x0439;
                else if ( L"iivowelsignbengali" == wsName ) return 0x09C0;
                else if ( L"iivowelsigndeva" == wsName ) return 0x0940;
                else if ( L"iivowelsigngujarati" == wsName ) return 0x0AC0;
                break;
            }
        case 'j':
            {
                if      ( L"ij" == wsName ) return 0x0133;
                break;
            }
        case 'k':
            {
                if      ( L"ikatakana" == wsName ) return 0x30A4;
                else if ( L"ikatakanahalfwidth" == wsName ) return 0xFF72;
                else if ( L"ikorean" == wsName ) return 0x3163;
                break;
            }
        case 'l':
            {
                if      ( L"ilde" == wsName ) return 0x02DC;
                else if ( L"iluyhebrew" == wsName ) return 0x05AC;
                break;
            }
        case 'm':
            {
                if      ( L"imacron" == wsName ) return 0x012B;
                else if ( L"imacroncyrillic" == wsName ) return 0x04E3;
                else if ( L"imageorapproximatelyequal" == wsName ) return 0x2253;
                else if ( L"imatragurmukhi" == wsName ) return 0x0A3F;
                else if ( L"imonospace" == wsName ) return 0xFF49;
                break;
            }
        case 'n':
            {
                if      ( L"increment" == wsName ) return 0x2206;
                else if ( L"infinity" == wsName ) return 0x221E;
                else if ( L"iniarmenian" == wsName ) return 0x056B;
                else if ( L"integral" == wsName ) return 0x222B;
                else if ( L"integralbottom" == wsName ) return 0x2321;
                else if ( L"integralbt" == wsName ) return 0x2321;
                else if ( L"integralex" == wsName ) return 0xF8F5;
                else if ( L"integraltop" == wsName ) return 0x2320;
                else if ( L"integraltp" == wsName ) return 0x2320;
                else if ( L"intersection" == wsName ) return 0x2229;
                else if ( L"intisquare" == wsName ) return 0x3305;
                else if ( L"invbullet" == wsName ) return 0x25D8;
                else if ( L"invcircle" == wsName ) return 0x25D9;
                else if ( L"invsmileface" == wsName ) return 0x263B;
                break;
            }
        case 'o':
            {
                if      ( L"iocyrillic" == wsName ) return 0x0451;
                else if ( L"iogonek" == wsName ) return 0x012F;
                else if ( L"iota" == wsName ) return 0x03B9;
                else if ( L"iotadieresis" == wsName ) return 0x03CA;
                else if ( L"iotadieresistonos" == wsName ) return 0x0390;
                else if ( L"iotalatin" == wsName ) return 0x0269;
                else if ( L"iotatonos" == wsName ) return 0x03AF;
                break;
            }
        case 'p':
            {
                if      ( L"iparen" == wsName ) return 0x24A4;
                break;
            }
        case 'r':
            {
                if      ( L"irigurmukhi" == wsName ) return 0x0A72;
                break;
            }
        case 's':
            {
                if      ( L"ismallhiragana" == wsName ) return 0x3043;
                else if ( L"ismallkatakana" == wsName ) return 0x30A3;
                else if ( L"ismallkatakanahalfwidth" == wsName ) return 0xFF68;
                else if ( L"issharbengali" == wsName ) return 0x09FA;
                else if ( L"istroke" == wsName ) return 0x0268;
                else if ( L"isuperior" == wsName ) return 0xF6ED;
                break;
            }
        case 't':
            {
                if      ( L"iterationhiragana" == wsName ) return 0x309D;
                else if ( L"iterationkatakana" == wsName ) return 0x30FD;
                else if ( L"itilde" == wsName ) return 0x0129;
                else if ( L"itildebelow" == wsName ) return 0x1E2D;
                break;
            }
        case 'u':
            {
                if      ( L"iubopomofo" == wsName ) return 0x3129;
                else if ( L"iucyrillic" == wsName ) return 0x044E;
                break;
            }
        case 'v':
            {
                if      ( L"ivowelsignbengali" == wsName ) return 0x09BF;
                else if ( L"ivowelsigndeva" == wsName ) return 0x093F;
                else if ( L"ivowelsigngujarati" == wsName ) return 0x0ABF;
                break;
            }
        case 'z':
            {
                if      ( L"izhitsacyrillic" == wsName ) return 0x0475;
                else if ( L"izhitsadblgravecyrillic" == wsName ) return 0x0477;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_j(CWCharWrapper &wsName)
    {
        if      ( L"j" == wsName ) return 0x006A;
        else if ( L"jaarmenian" == wsName ) return 0x0571;
        else if ( L"jabengali" == wsName ) return 0x099C;
        else if ( L"jadeva" == wsName ) return 0x091C;
        else if ( L"jagujarati" == wsName ) return 0x0A9C;
        else if ( L"jagurmukhi" == wsName ) return 0x0A1C;
        else if ( L"jbopomofo" == wsName ) return 0x3110;
        else if ( L"jcaron" == wsName ) return 0x01F0;
        else if ( L"jcircle" == wsName ) return 0x24D9;
        else if ( L"jcircumflex" == wsName ) return 0x0135;
        else if ( L"jcrossedtail" == wsName ) return 0x029D;
        else if ( L"jdotlessstroke" == wsName ) return 0x025F;
        else if ( L"jecyrillic" == wsName ) return 0x0458;
        else if ( L"jeemarabic" == wsName ) return 0x062C;
        else if ( L"jeemfinalarabic" == wsName ) return 0xFE9E;
        else if ( L"jeeminitialarabic" == wsName ) return 0xFE9F;
        else if ( L"jeemmedialarabic" == wsName ) return 0xFEA0;
        else if ( L"jeharabic" == wsName ) return 0x0698;
        else if ( L"jehfinalarabic" == wsName ) return 0xFB8B;
        else if ( L"jhabengali" == wsName ) return 0x099D;
        else if ( L"jhadeva" == wsName ) return 0x091D;
        else if ( L"jhagujarati" == wsName ) return 0x0A9D;
        else if ( L"jhagurmukhi" == wsName ) return 0x0A1D;
        else if ( L"jheharmenian" == wsName ) return 0x057B;
        else if ( L"jis" == wsName ) return 0x3004;
        else if ( L"jmonospace" == wsName ) return 0xFF4A;
        else if ( L"jparen" == wsName ) return 0x24A5;
        else if ( L"jsuperior" == wsName ) return 0x02B2;

        return 0;
    }
    static int Type1NameToUnicode_k(CWCharWrapper &wsName)
    {
        if      ( L"k" == wsName ) return 0x006B;
        else if ( L"kabashkircyrillic" == wsName ) return 0x04A1;
        else if ( L"kabengali" == wsName ) return 0x0995;
        else if ( L"kacute" == wsName ) return 0x1E31;
        else if ( L"kacyrillic" == wsName ) return 0x043A;
        else if ( L"kadescendercyrillic" == wsName ) return 0x049B;
        else if ( L"kadeva" == wsName ) return 0x0915;
        else if ( L"kaf" == wsName ) return 0x05DB;
        else if ( L"kafarabic" == wsName ) return 0x0643;
        else if ( L"kafdagesh" == wsName ) return 0xFB3B;
        else if ( L"kafdageshhebrew" == wsName ) return 0xFB3B;
        else if ( L"kaffinalarabic" == wsName ) return 0xFEDA;
        else if ( L"kafhebrew" == wsName ) return 0x05DB;
        else if ( L"kafinitialarabic" == wsName ) return 0xFEDB;
        else if ( L"kafmedialarabic" == wsName ) return 0xFEDC;
        else if ( L"kafrafehebrew" == wsName ) return 0xFB4D;
        else if ( L"kagujarati" == wsName ) return 0x0A95;
        else if ( L"kagurmukhi" == wsName ) return 0x0A15;
        else if ( L"kahiragana" == wsName ) return 0x304B;
        else if ( L"kahookcyrillic" == wsName ) return 0x04C4;
        else if ( L"kakatakana" == wsName ) return 0x30AB;
        else if ( L"kakatakanahalfwidth" == wsName ) return 0xFF76;
        else if ( L"kappa" == wsName ) return 0x03BA;
        else if ( L"kappasymbolgreek" == wsName ) return 0x03F0;
        else if ( L"kapyeounmieumkorean" == wsName ) return 0x3171;
        else if ( L"kapyeounphieuphkorean" == wsName ) return 0x3184;
        else if ( L"kapyeounpieupkorean" == wsName ) return 0x3178;
        else if ( L"kapyeounssangpieupkorean" == wsName ) return 0x3179;
        else if ( L"karoriisquare" == wsName ) return 0x330D;
        else if ( L"kashidaautoarabic" == wsName ) return 0x0640;
        else if ( L"kashidaautonosidebearingarabic" == wsName ) return 0x0640;
        else if ( L"kasmallkatakana" == wsName ) return 0x30F5;
        else if ( L"kasquare" == wsName ) return 0x3384;
        else if ( L"kasraarabic" == wsName ) return 0x0650;
        else if ( L"kasratanarabic" == wsName ) return 0x064D;
        else if ( L"kastrokecyrillic" == wsName ) return 0x049F;
        else if ( L"katahiraprolongmarkhalfwidth" == wsName ) return 0xFF70;
        else if ( L"kaverticalstrokecyrillic" == wsName ) return 0x049D;
        else if ( L"kbopomofo" == wsName ) return 0x310E;
        else if ( L"kcalsquare" == wsName ) return 0x3389;
        else if ( L"kcaron" == wsName ) return 0x01E9;
        else if ( L"kcedilla" == wsName ) return 0x0137;
        else if ( L"kcircle" == wsName ) return 0x24DA;
        else if ( L"kcommaaccent" == wsName ) return 0x0137;
        else if ( L"kdotbelow" == wsName ) return 0x1E33;
        else if ( L"keharmenian" == wsName ) return 0x0584;
        else if ( L"kehiragana" == wsName ) return 0x3051;
        else if ( L"kekatakana" == wsName ) return 0x30B1;
        else if ( L"kekatakanahalfwidth" == wsName ) return 0xFF79;
        else if ( L"kenarmenian" == wsName ) return 0x056F;
        else if ( L"kesmallkatakana" == wsName ) return 0x30F6;
        else if ( L"kgreenlandic" == wsName ) return 0x0138;
        else if ( L"khabengali" == wsName ) return 0x0996;
        else if ( L"khacyrillic" == wsName ) return 0x0445;
        else if ( L"khadeva" == wsName ) return 0x0916;
        else if ( L"khagujarati" == wsName ) return 0x0A96;
        else if ( L"khagurmukhi" == wsName ) return 0x0A16;
        else if ( L"khaharabic" == wsName ) return 0x062E;
        else if ( L"khahfinalarabic" == wsName ) return 0xFEA6;
        else if ( L"khahinitialarabic" == wsName ) return 0xFEA7;
        else if ( L"khahmedialarabic" == wsName ) return 0xFEA8;
        else if ( L"kheicoptic" == wsName ) return 0x03E7;
        else if ( L"khhadeva" == wsName ) return 0x0959;
        else if ( L"khhagurmukhi" == wsName ) return 0x0A59;
        else if ( L"khieukhacirclekorean" == wsName ) return 0x3278;
        else if ( L"khieukhaparenkorean" == wsName ) return 0x3218;
        else if ( L"khieukhcirclekorean" == wsName ) return 0x326A;
        else if ( L"khieukhkorean" == wsName ) return 0x314B;
        else if ( L"khieukhparenkorean" == wsName ) return 0x320A;
        else if ( L"khokhaithai" == wsName ) return 0x0E02;
        else if ( L"khokhonthai" == wsName ) return 0x0E05;
        else if ( L"khokhuatthai" == wsName ) return 0x0E03;
        else if ( L"khokhwaithai" == wsName ) return 0x0E04;
        else if ( L"khomutthai" == wsName ) return 0x0E5B;
        else if ( L"khook" == wsName ) return 0x0199;
        else if ( L"khorakhangthai" == wsName ) return 0x0E06;
        else if ( L"khzsquare" == wsName ) return 0x3391;
        else if ( L"kihiragana" == wsName ) return 0x304D;
        else if ( L"kikatakana" == wsName ) return 0x30AD;
        else if ( L"kikatakanahalfwidth" == wsName ) return 0xFF77;
        else if ( L"kiroguramusquare" == wsName ) return 0x3315;
        else if ( L"kiromeetorusquare" == wsName ) return 0x3316;
        else if ( L"kirosquare" == wsName ) return 0x3314;
        else if ( L"kiyeokacirclekorean" == wsName ) return 0x326E;
        else if ( L"kiyeokaparenkorean" == wsName ) return 0x320E;
        else if ( L"kiyeokcirclekorean" == wsName ) return 0x3260;
        else if ( L"kiyeokkorean" == wsName ) return 0x3131;
        else if ( L"kiyeokparenkorean" == wsName ) return 0x3200;
        else if ( L"kiyeoksioskorean" == wsName ) return 0x3133;
        else if ( L"kjecyrillic" == wsName ) return 0x045C;
        else if ( L"klinebelow" == wsName ) return 0x1E35;
        else if ( L"klsquare" == wsName ) return 0x3398;
        else if ( L"kmcubedsquare" == wsName ) return 0x33A6;
        else if ( L"kmonospace" == wsName ) return 0xFF4B;
        else if ( L"kmsquaredsquare" == wsName ) return 0x33A2;
        else if ( L"kohiragana" == wsName ) return 0x3053;
        else if ( L"kohmsquare" == wsName ) return 0x33C0;
        else if ( L"kokaithai" == wsName ) return 0x0E01;
        else if ( L"kokatakana" == wsName ) return 0x30B3;
        else if ( L"kokatakanahalfwidth" == wsName ) return 0xFF7A;
        else if ( L"kooposquare" == wsName ) return 0x331E;
        else if ( L"koppacyrillic" == wsName ) return 0x0481;
        else if ( L"koreanstandardsymbol" == wsName ) return 0x327F;
        else if ( L"koroniscmb" == wsName ) return 0x0343;
        else if ( L"kparen" == wsName ) return 0x24A6;
        else if ( L"kpasquare" == wsName ) return 0x33AA;
        else if ( L"ksicyrillic" == wsName ) return 0x046F;
        else if ( L"ktsquare" == wsName ) return 0x33CF;
        else if ( L"kturned" == wsName ) return 0x029E;
        else if ( L"kuhiragana" == wsName ) return 0x304F;
        else if ( L"kukatakana" == wsName ) return 0x30AF;
        else if ( L"kukatakanahalfwidth" == wsName ) return 0xFF78;
        else if ( L"kvsquare" == wsName ) return 0x33B8;
        else if ( L"kwsquare" == wsName ) return 0x33BE;

        return 0;
    }
    static int Type1NameToUnicode_l(CWCharWrapper &wsName)
    {
        if      ( L"l" == wsName ) return 0x006C;
        else if ( L"labengali" == wsName ) return 0x09B2;
        else if ( L"lacute" == wsName ) return 0x013A;
        else if ( L"ladeva" == wsName ) return 0x0932;
        else if ( L"lagujarati" == wsName ) return 0x0AB2;
        else if ( L"lagurmukhi" == wsName ) return 0x0A32;
        else if ( L"lakkhangyaothai" == wsName ) return 0x0E45;
        else if ( L"lamaleffinalarabic" == wsName ) return 0xFEFC;
        else if ( L"lamalefhamzaabovefinalarabic" == wsName ) return 0xFEF8;
        else if ( L"lamalefhamzaaboveisolatedarabic" == wsName ) return 0xFEF7;
        else if ( L"lamalefhamzabelowfinalarabic" == wsName ) return 0xFEFA;
        else if ( L"lamalefhamzabelowisolatedarabic" == wsName ) return 0xFEF9;
        else if ( L"lamalefisolatedarabic" == wsName ) return 0xFEFB;
        else if ( L"lamalefmaddaabovefinalarabic" == wsName ) return 0xFEF6;
        else if ( L"lamalefmaddaaboveisolatedarabic" == wsName ) return 0xFEF5;
        else if ( L"lamarabic" == wsName ) return 0x0644;
        else if ( L"lambda" == wsName ) return 0x03BB;
        else if ( L"lambdastroke" == wsName ) return 0x019B;
        else if ( L"lamed" == wsName ) return 0x05DC;
        else if ( L"lameddagesh" == wsName ) return 0xFB3C;
        else if ( L"lameddageshhebrew" == wsName ) return 0xFB3C;
        else if ( L"lamedhebrew" == wsName ) return 0x05DC;
        else if ( L"lamedholam" == wsName ) return 0x05DC05B9;
        // "lamedholamdagesh"  "05DC 05B9 05BC";
        // "lamedholamdageshhebrew"  "05DC 05B9 05BC";
        else if ( L"lamedholamhebrew" == wsName ) return 0x05DC05B9;
        else if ( L"lamfinalarabic" == wsName ) return 0xFEDE;
        else if ( L"lamhahinitialarabic" == wsName ) return 0xFCCA;
        else if ( L"laminitialarabic" == wsName ) return 0xFEDF;
        else if ( L"lamjeeminitialarabic" == wsName ) return 0xFCC9;
        else if ( L"lamkhahinitialarabic" == wsName ) return 0xFCCB;
        else if ( L"lamlamhehisolatedarabic" == wsName ) return 0xFDF2;
        else if ( L"lammedialarabic" == wsName ) return 0xFEE0;
        else if ( L"lammeemhahinitialarabic" == wsName ) return 0xFD88;
        else if ( L"lammeeminitialarabic" == wsName ) return 0xFCCC;
        // "lammeemjeeminitialarabic" "FEDF FEE4 FEA0";
        // "lammeemkhahinitialarabic" "FEDF FEE4 FEA8";
        else if ( L"largecircle" == wsName ) return 0x25EF;
        else if ( L"lbar" == wsName ) return 0x019A;
        else if ( L"lbelt" == wsName ) return 0x026C;
        else if ( L"lbopomofo" == wsName ) return 0x310C;
        else if ( L"lcaron" == wsName ) return 0x013E;
        else if ( L"lcedilla" == wsName ) return 0x013C;
        else if ( L"lcircle" == wsName ) return 0x24DB;
        else if ( L"lcircumflexbelow" == wsName ) return 0x1E3D;
        else if ( L"lcommaaccent" == wsName ) return 0x013C;
        else if ( L"ldot" == wsName ) return 0x0140;
        else if ( L"ldotaccent" == wsName ) return 0x0140;
        else if ( L"ldotbelow" == wsName ) return 0x1E37;
        else if ( L"ldotbelowmacron" == wsName ) return 0x1E39;
        else if ( L"leftangleabovecmb" == wsName ) return 0x031A;
        else if ( L"lefttackbelowcmb" == wsName ) return 0x0318;
        else if ( L"less" == wsName ) return 0x003C;
        else if ( L"lessequal" == wsName ) return 0x2264;
        else if ( L"lessequalorgreater" == wsName ) return 0x22DA;
        else if ( L"lessmonospace" == wsName ) return 0xFF1C;
        else if ( L"lessorequivalent" == wsName ) return 0x2272;
        else if ( L"lessorgreater" == wsName ) return 0x2276;
        else if ( L"lessoverequal" == wsName ) return 0x2266;
        else if ( L"lesssmall" == wsName ) return 0xFE64;
        else if ( L"lezh" == wsName ) return 0x026E;
        else if ( L"lfblock" == wsName ) return 0x258C;
        else if ( L"lhookretroflex" == wsName ) return 0x026D;
        else if ( L"lira" == wsName ) return 0x20A4;
        else if ( L"liwnarmenian" == wsName ) return 0x056C;
        else if ( L"lj" == wsName ) return 0x01C9;
        else if ( L"ljecyrillic" == wsName ) return 0x0459;
        else if ( L"ll" == wsName ) return 0xF6C0;
        else if ( L"lladeva" == wsName ) return 0x0933;
        else if ( L"llagujarati" == wsName ) return 0x0AB3;
        else if ( L"llinebelow" == wsName ) return 0x1E3B;
        else if ( L"llladeva" == wsName ) return 0x0934;
        else if ( L"llvocalicbengali" == wsName ) return 0x09E1;
        else if ( L"llvocalicdeva" == wsName ) return 0x0961;
        else if ( L"llvocalicvowelsignbengali" == wsName ) return 0x09E3;
        else if ( L"llvocalicvowelsigndeva" == wsName ) return 0x0963;
        else if ( L"lmiddletilde" == wsName ) return 0x026B;
        else if ( L"lmonospace" == wsName ) return 0xFF4C;
        else if ( L"lmsquare" == wsName ) return 0x33D0;
        else if ( L"lochulathai" == wsName ) return 0x0E2C;
        else if ( L"logicaland" == wsName ) return 0x2227;
        else if ( L"logicalnot" == wsName ) return 0x00AC;
        else if ( L"logicalnotreversed" == wsName ) return 0x2310;
        else if ( L"logicalor" == wsName ) return 0x2228;
        else if ( L"lolingthai" == wsName ) return 0x0E25;
        else if ( L"longs" == wsName ) return 0x017F;
        else if ( L"lowlinecenterline" == wsName ) return 0xFE4E;
        else if ( L"lowlinecmb" == wsName ) return 0x0332;
        else if ( L"lowlinedashed" == wsName ) return 0xFE4D;
        else if ( L"lozenge" == wsName ) return 0x25CA;
        else if ( L"lparen" == wsName ) return 0x24A7;
        else if ( L"lslash" == wsName ) return 0x0142;
        else if ( L"lsquare" == wsName ) return 0x2113;
        else if ( L"lsuperior" == wsName ) return 0xF6EE;
        else if ( L"ltshade" == wsName ) return 0x2591;
        else if ( L"luthai" == wsName ) return 0x0E26;
        else if ( L"lvocalicbengali" == wsName ) return 0x098C;
        else if ( L"lvocalicdeva" == wsName ) return 0x090C;
        else if ( L"lvocalicvowelsignbengali" == wsName ) return 0x09E2;
        else if ( L"lvocalicvowelsigndeva" == wsName ) return 0x0962;
        else if ( L"lxsquare" == wsName ) return 0x33D3;

        return 0;
    }
    static int Type1NameToUnicode_m(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch( wChar1 )
        {
        case 0:
            {
                if      ( L"m" == wsName ) return 0x006D;
                break;
            }
        case 'a':
            {
                if      ( L"mabengali" == wsName ) return 0x09AE;
                else if ( L"macron" == wsName ) return 0x00AF;
                else if ( L"macronbelowcmb" == wsName ) return 0x0331;
                else if ( L"macroncmb" == wsName ) return 0x0304;
                else if ( L"macronlowmod" == wsName ) return 0x02CD;
                else if ( L"macronmonospace" == wsName ) return 0xFFE3;
                else if ( L"macute" == wsName ) return 0x1E3F;
                else if ( L"madeva" == wsName ) return 0x092E;
                else if ( L"magujarati" == wsName ) return 0x0AAE;
                else if ( L"magurmukhi" == wsName ) return 0x0A2E;
                else if ( L"mahapakhhebrew" == wsName ) return 0x05A4;
                else if ( L"mahapakhlefthebrew" == wsName ) return 0x05A4;
                else if ( L"mahiragana" == wsName ) return 0x307E;
                else if ( L"maichattawalowleftthai" == wsName ) return 0xF895;
                else if ( L"maichattawalowrightthai" == wsName ) return 0xF894;
                else if ( L"maichattawathai" == wsName ) return 0x0E4B;
                else if ( L"maichattawaupperleftthai" == wsName ) return 0xF893;
                else if ( L"maieklowleftthai" == wsName ) return 0xF88C;
                else if ( L"maieklowrightthai" == wsName ) return 0xF88B;
                else if ( L"maiekthai" == wsName ) return 0x0E48;
                else if ( L"maiekupperleftthai" == wsName ) return 0xF88A;
                else if ( L"maihanakatleftthai" == wsName ) return 0xF884;
                else if ( L"maihanakatthai" == wsName ) return 0x0E31;
                else if ( L"maitaikhuleftthai" == wsName ) return 0xF889;
                else if ( L"maitaikhuthai" == wsName ) return 0x0E47;
                else if ( L"maitholowleftthai" == wsName ) return 0xF88F;
                else if ( L"maitholowrightthai" == wsName ) return 0xF88E;
                else if ( L"maithothai" == wsName ) return 0x0E49;
                else if ( L"maithoupperleftthai" == wsName ) return 0xF88D;
                else if ( L"maitrilowleftthai" == wsName ) return 0xF892;
                else if ( L"maitrilowrightthai" == wsName ) return 0xF891;
                else if ( L"maitrithai" == wsName ) return 0x0E4A;
                else if ( L"maitriupperleftthai" == wsName ) return 0xF890;
                else if ( L"maiyamokthai" == wsName ) return 0x0E46;
                else if ( L"makatakana" == wsName ) return 0x30DE;
                else if ( L"makatakanahalfwidth" == wsName ) return 0xFF8F;
                else if ( L"male" == wsName ) return 0x2642;
                else if ( L"mansyonsquare" == wsName ) return 0x3347;
                else if ( L"maqafhebrew" == wsName ) return 0x05BE;
                else if ( L"mars" == wsName ) return 0x2642;
                else if ( L"masoracirclehebrew" == wsName ) return 0x05AF;
                else if ( L"masquare" == wsName ) return 0x3383;
                break;
            }
        case 'b':
            {
                if      ( L"mbopomofo" == wsName ) return 0x3107;
                else if ( L"mbsquare" == wsName ) return 0x33D4;
                break;
            }
        case 'c':
            {
                if      ( L"mcircle" == wsName ) return 0x24DC;
                else if ( L"mcubedsquare" == wsName ) return 0x33A5;
                break;
            }
        case 'd':
            {
                if      ( L"mdotaccent" == wsName ) return 0x1E41;
                else if ( L"mdotbelow" == wsName ) return 0x1E43;
                break;
            }
        case 'e':
            {
                if      ( L"meemarabic" == wsName ) return 0x0645;
                else if ( L"meemfinalarabic" == wsName ) return 0xFEE2;
                else if ( L"meeminitialarabic" == wsName ) return 0xFEE3;
                else if ( L"meemmedialarabic" == wsName ) return 0xFEE4;
                else if ( L"meemmeeminitialarabic" == wsName ) return 0xFCD1;
                else if ( L"meemmeemisolatedarabic" == wsName ) return 0xFC48;
                else if ( L"meetorusquare" == wsName ) return 0x334D;
                else if ( L"mehiragana" == wsName ) return 0x3081;
                else if ( L"meizierasquare" == wsName ) return 0x337E;
                else if ( L"mekatakana" == wsName ) return 0x30E1;
                else if ( L"mekatakanahalfwidth" == wsName ) return 0xFF92;
                else if ( L"mem" == wsName ) return 0x05DE;
                else if ( L"memdagesh" == wsName ) return 0xFB3E;
                else if ( L"memdageshhebrew" == wsName ) return 0xFB3E;
                else if ( L"memhebrew" == wsName ) return 0x05DE;
                else if ( L"menarmenian" == wsName ) return 0x0574;
                else if ( L"merkhahebrew" == wsName ) return 0x05A5;
                else if ( L"merkhakefulahebrew" == wsName ) return 0x05A6;
                else if ( L"merkhakefulalefthebrew" == wsName ) return 0x05A6;
                else if ( L"merkhalefthebrew" == wsName ) return 0x05A5;
                break;
            }
        case 'h':
            {
                if      ( L"mhook" == wsName ) return 0x0271;
                else if ( L"mhzsquare" == wsName ) return 0x3392;
                break;
            }
        case 'i':
            {
                if      ( L"middledotkatakanahalfwidth" == wsName ) return 0xFF65;
                else if ( L"middot" == wsName ) return 0x00B7;
                else if ( L"mieumacirclekorean" == wsName ) return 0x3272;
                else if ( L"mieumaparenkorean" == wsName ) return 0x3212;
                else if ( L"mieumcirclekorean" == wsName ) return 0x3264;
                else if ( L"mieumkorean" == wsName ) return 0x3141;
                else if ( L"mieumpansioskorean" == wsName ) return 0x3170;
                else if ( L"mieumparenkorean" == wsName ) return 0x3204;
                else if ( L"mieumpieupkorean" == wsName ) return 0x316E;
                else if ( L"mieumsioskorean" == wsName ) return 0x316F;
                else if ( L"mihiragana" == wsName ) return 0x307F;
                else if ( L"mikatakana" == wsName ) return 0x30DF;
                else if ( L"mikatakanahalfwidth" == wsName ) return 0xFF90;
                else if ( L"minus" == wsName ) return 0x2212;
                else if ( L"minusbelowcmb" == wsName ) return 0x0320;
                else if ( L"minuscircle" == wsName ) return 0x2296;
                else if ( L"minusmod" == wsName ) return 0x02D7;
                else if ( L"minusplus" == wsName ) return 0x2213;
                else if ( L"minute" == wsName ) return 0x2032;
                else if ( L"miribaarusquare" == wsName ) return 0x334A;
                else if ( L"mirisquare" == wsName ) return 0x3349;
                break;
            }
        case 'l':
            {
                if      ( L"mlonglegturned" == wsName ) return 0x0270;
                else if ( L"mlsquare" == wsName ) return 0x3396;
                break;
            }
        case 'm':
            {
                if      ( L"mmcubedsquare" == wsName ) return 0x33A3;
                else if ( L"mmonospace" == wsName ) return 0xFF4D;
                else if ( L"mmsquaredsquare" == wsName ) return 0x339F;
                break;
            }
        case 'o':
            {
                if      ( L"mohiragana" == wsName ) return 0x3082;
                else if ( L"mohmsquare" == wsName ) return 0x33C1;
                else if ( L"mokatakana" == wsName ) return 0x30E2;
                else if ( L"mokatakanahalfwidth" == wsName ) return 0xFF93;
                else if ( L"molsquare" == wsName ) return 0x33D6;
                else if ( L"momathai" == wsName ) return 0x0E21;
                else if ( L"moverssquare" == wsName ) return 0x33A7;
                else if ( L"moverssquaredsquare" == wsName ) return 0x33A8;
                break;
            }
        case 'p':
            {
                if      ( L"mparen" == wsName ) return 0x24A8;
                else if ( L"mpasquare" == wsName ) return 0x33AB;
                break;
            }
        case 's':
            {
                if      ( L"mssquare" == wsName ) return 0x33B3;
                else if ( L"msuperior" == wsName ) return 0xF6EF;
                break;
            }
        case 't':
            {
                if      ( L"mturned" == wsName ) return 0x026F;
                break;
            }
        case 'u':
            {
                if      ( L"mu" == wsName ) return 0x00B5;
                else if ( L"mu1" == wsName ) return 0x00B5;
                else if ( L"muasquare" == wsName ) return 0x3382;
                else if ( L"muchgreater" == wsName ) return 0x226B;
                else if ( L"muchless" == wsName ) return 0x226A;
                else if ( L"mufsquare" == wsName ) return 0x338C;
                else if ( L"mugreek" == wsName ) return 0x03BC;
                else if ( L"mugsquare" == wsName ) return 0x338D;
                else if ( L"muhiragana" == wsName ) return 0x3080;
                else if ( L"mukatakana" == wsName ) return 0x30E0;
                else if ( L"mukatakanahalfwidth" == wsName ) return 0xFF91;
                else if ( L"mulsquare" == wsName ) return 0x3395;
                else if ( L"multiply" == wsName ) return 0x00D7;
                else if ( L"mumsquare" == wsName ) return 0x339B;
                else if ( L"munahhebrew" == wsName ) return 0x05A3;
                else if ( L"munahlefthebrew" == wsName ) return 0x05A3;
                else if ( L"musicalnote" == wsName ) return 0x266A;
                else if ( L"musicalnotedbl" == wsName ) return 0x266B;
                else if ( L"musicflatsign" == wsName ) return 0x266D;
                else if ( L"musicsharpsign" == wsName ) return 0x266F;
                else if ( L"mussquare" == wsName ) return 0x33B2;
                else if ( L"muvsquare" == wsName ) return 0x33B6;
                else if ( L"muwsquare" == wsName ) return 0x33BC;
                break;
            }
        case 'v':
            {
                if      ( L"mvmegasquare" == wsName ) return 0x33B9;
                else if ( L"mvsquare" == wsName ) return 0x33B7;
                break;
            }
        case 'w':
            {
                if      ( L"mwmegasquare" == wsName ) return 0x33BF;
                else if ( L"mwsquare" == wsName ) return 0x33BD;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_n(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch( wChar1 )
        {
        case 0:
            {
                if      ( L"n" == wsName ) return 0x006E;
                break;
            }
        case 'a':
            {
                if      ( L"nabengali" == wsName ) return 0x09A8;
                else if ( L"nabla" == wsName ) return 0x2207;
                else if ( L"nacute" == wsName ) return 0x0144;
                else if ( L"nadeva" == wsName ) return 0x0928;
                else if ( L"nagujarati" == wsName ) return 0x0AA8;
                else if ( L"nagurmukhi" == wsName ) return 0x0A28;
                else if ( L"nahiragana" == wsName ) return 0x306A;
                else if ( L"nakatakana" == wsName ) return 0x30CA;
                else if ( L"nakatakanahalfwidth" == wsName ) return 0xFF85;
                else if ( L"napostrophe" == wsName ) return 0x0149;
                else if ( L"nasquare" == wsName ) return 0x3381;
                break;
            }
        case 'b':
            {
                if      ( L"nbopomofo" == wsName ) return 0x310B;
                else if ( L"nbspace" == wsName ) return 0x00A0;
                break;
            }
        case 'c':
            {
                if      ( L"ncaron" == wsName ) return 0x0148;
                else if ( L"ncedilla" == wsName ) return 0x0146;
                else if ( L"ncircle" == wsName ) return 0x24DD;
                else if ( L"ncircumflexbelow" == wsName ) return 0x1E4B;
                else if ( L"ncommaaccent" == wsName ) return 0x0146;
                break;
            }
        case 'd':
            {
                if      ( L"ndotaccent" == wsName ) return 0x1E45;
                else if ( L"ndotbelow" == wsName ) return 0x1E47;
                break;
            }
        case 'e':
            {
                if      ( L"nehiragana" == wsName ) return 0x306D;
                else if ( L"nekatakana" == wsName ) return 0x30CD;
                else if ( L"nekatakanahalfwidth" == wsName ) return 0xFF88;
                else if ( L"newsheqelsign" == wsName ) return 0x20AA;
                break;
            }
        case 'f':
            {
                if      ( L"nfsquare" == wsName ) return 0x338B;
                break;
            }
        case 'g':
            {
                if      ( L"ngabengali" == wsName ) return 0x0999;
                else if ( L"ngadeva" == wsName ) return 0x0919;
                else if ( L"ngagujarati" == wsName ) return 0x0A99;
                else if ( L"ngagurmukhi" == wsName ) return 0x0A19;
                else if ( L"ngonguthai" == wsName ) return 0x0E07;
                break;
            }
        case 'h':
            {
                if      ( L"nhiragana" == wsName ) return 0x3093;
                else if ( L"nhookleft" == wsName ) return 0x0272;
                else if ( L"nhookretroflex" == wsName ) return 0x0273;
                break;
            }
        case 'i':
            {
                if      ( L"nieunacirclekorean" == wsName ) return 0x326F;
                else if ( L"nieunaparenkorean" == wsName ) return 0x320F;
                else if ( L"nieuncieuckorean" == wsName ) return 0x3135;
                else if ( L"nieuncirclekorean" == wsName ) return 0x3261;
                else if ( L"nieunhieuhkorean" == wsName ) return 0x3136;
                else if ( L"nieunkorean" == wsName ) return 0x3134;
                else if ( L"nieunpansioskorean" == wsName ) return 0x3168;
                else if ( L"nieunparenkorean" == wsName ) return 0x3201;
                else if ( L"nieunsioskorean" == wsName ) return 0x3167;
                else if ( L"nieuntikeutkorean" == wsName ) return 0x3166;
                else if ( L"nihiragana" == wsName ) return 0x306B;
                else if ( L"nikatakana" == wsName ) return 0x30CB;
                else if ( L"nikatakanahalfwidth" == wsName ) return 0xFF86;
                else if ( L"nikhahitleftthai" == wsName ) return 0xF899;
                else if ( L"nikhahitthai" == wsName ) return 0x0E4D;
                else if ( L"nine" == wsName ) return 0x0039;
                else if ( L"ninearabic" == wsName ) return 0x0669;
                else if ( L"ninebengali" == wsName ) return 0x09EF;
                else if ( L"ninecircle" == wsName ) return 0x2468;
                else if ( L"ninecircleinversesansserif" == wsName ) return 0x2792;
                else if ( L"ninedeva" == wsName ) return 0x096F;
                else if ( L"ninegujarati" == wsName ) return 0x0AEF;
                else if ( L"ninegurmukhi" == wsName ) return 0x0A6F;
                else if ( L"ninehackarabic" == wsName ) return 0x0669;
                else if ( L"ninehangzhou" == wsName ) return 0x3029;
                else if ( L"nineideographicparen" == wsName ) return 0x3228;
                else if ( L"nineinferior" == wsName ) return 0x2089;
                else if ( L"ninemonospace" == wsName ) return 0xFF19;
                else if ( L"nineoldstyle" == wsName ) return 0xF739;
                else if ( L"nineparen" == wsName ) return 0x247C;
                else if ( L"nineperiod" == wsName ) return 0x2490;
                else if ( L"ninepersian" == wsName ) return 0x06F9;
                else if ( L"nineroman" == wsName ) return 0x2178;
                else if ( L"ninesuperior" == wsName ) return 0x2079;
                else if ( L"nineteencircle" == wsName ) return 0x2472;
                else if ( L"nineteenparen" == wsName ) return 0x2486;
                else if ( L"nineteenperiod" == wsName ) return 0x249A;
                else if ( L"ninethai" == wsName ) return 0x0E59;
                break;
            }
        case 'j':
            {
                if      ( L"nj" == wsName ) return 0x01CC;
                else if ( L"njecyrillic" == wsName ) return 0x045A;
                break;
            }
        case 'k':
            {
                if      ( L"nkatakana" == wsName ) return 0x30F3;
                else if ( L"nkatakanahalfwidth" == wsName ) return 0xFF9D;
                break;
            }
        case 'l':
            {
                if      ( L"nlegrightlong" == wsName ) return 0x019E;
                else if ( L"nlinebelow" == wsName ) return 0x1E49;
                break;
            }
        case 'm':
            {
                if      ( L"nmonospace" == wsName ) return 0xFF4E;
                else if ( L"nmsquare" == wsName ) return 0x339A;
                break;
            }
        case 'n':
            {
                if      ( L"nnabengali" == wsName ) return 0x09A3;
                else if ( L"nnadeva" == wsName ) return 0x0923;
                else if ( L"nnagujarati" == wsName ) return 0x0AA3;
                else if ( L"nnagurmukhi" == wsName ) return 0x0A23;
                else if ( L"nnnadeva" == wsName ) return 0x0929;
                break;
            }
        case 'o':
            {
                if      ( L"nohiragana" == wsName ) return 0x306E;
                else if ( L"nokatakana" == wsName ) return 0x30CE;
                else if ( L"nokatakanahalfwidth" == wsName ) return 0xFF89;
                else if ( L"nonbreakingspace" == wsName ) return 0x00A0;
                else if ( L"nonenthai" == wsName ) return 0x0E13;
                else if ( L"nonuthai" == wsName ) return 0x0E19;
                else if ( L"noonarabic" == wsName ) return 0x0646;
                else if ( L"noonfinalarabic" == wsName ) return 0xFEE6;
                else if ( L"noonghunnaarabic" == wsName ) return 0x06BA;
                else if ( L"noonghunnafinalarabic" == wsName ) return 0xFB9F;
                else if ( L"noonhehinitialarabic" == wsName ) return 0xFEE7FEEC;
                else if ( L"nooninitialarabic" == wsName ) return 0xFEE7;
                else if ( L"noonjeeminitialarabic" == wsName ) return 0xFCD2;
                else if ( L"noonjeemisolatedarabic" == wsName ) return 0xFC4B;
                else if ( L"noonmedialarabic" == wsName ) return 0xFEE8;
                else if ( L"noonmeeminitialarabic" == wsName ) return 0xFCD5;
                else if ( L"noonmeemisolatedarabic" == wsName ) return 0xFC4E;
                else if ( L"noonnoonfinalarabic" == wsName ) return 0xFC8D;
                else if ( L"notcontains" == wsName ) return 0x220C;
                else if ( L"notelement" == wsName ) return 0x2209;
                else if ( L"notelementof" == wsName ) return 0x2209;
                else if ( L"notequal" == wsName ) return 0x2260;
                else if ( L"notgreater" == wsName ) return 0x226F;
                else if ( L"notgreaternorequal" == wsName ) return 0x2271;
                else if ( L"notgreaternorless" == wsName ) return 0x2279;
                else if ( L"notidentical" == wsName ) return 0x2262;
                else if ( L"notless" == wsName ) return 0x226E;
                else if ( L"notlessnorequal" == wsName ) return 0x2270;
                else if ( L"notparallel" == wsName ) return 0x2226;
                else if ( L"notprecedes" == wsName ) return 0x2280;
                else if ( L"notsubset" == wsName ) return 0x2284;
                else if ( L"notsucceeds" == wsName ) return 0x2281;
                else if ( L"notsuperset" == wsName ) return 0x2285;
                else if ( L"nowarmenian" == wsName ) return 0x0576;
                break;
            }
        case 'p':
            {
                if      ( L"nparen" == wsName ) return 0x24A9;
                break;
            }
        case 's':
            {
                if      ( L"nssquare" == wsName ) return 0x33B1;
                else if ( L"nsuperior" == wsName ) return 0x207F;
                break;
            }
        case 't':
            {
                if      ( L"ntilde" == wsName ) return 0x00F1;
                break;
            }
        case 'u':
            {
                if      ( L"nu" == wsName ) return 0x03BD;
                else if ( L"nuhiragana" == wsName ) return 0x306C;
                else if ( L"nukatakana" == wsName ) return 0x30CC;
                else if ( L"nukatakanahalfwidth" == wsName ) return 0xFF87;
                else if ( L"nuktabengali" == wsName ) return 0x09BC;
                else if ( L"nuktadeva" == wsName ) return 0x093C;
                else if ( L"nuktagujarati" == wsName ) return 0x0ABC;
                else if ( L"nuktagurmukhi" == wsName ) return 0x0A3C;
                else if ( L"numbersign" == wsName ) return 0x0023;
                else if ( L"numbersignmonospace" == wsName ) return 0xFF03;
                else if ( L"numbersignsmall" == wsName ) return 0xFE5F;
                else if ( L"numeralsigngreek" == wsName ) return 0x0374;
                else if ( L"numeralsignlowergreek" == wsName ) return 0x0375;
                else if ( L"numero" == wsName ) return 0x2116;
                else if ( L"nun" == wsName ) return 0x05E0;
                else if ( L"nundagesh" == wsName ) return 0xFB40;
                else if ( L"nundageshhebrew" == wsName ) return 0xFB40;
                else if ( L"nunhebrew" == wsName ) return 0x05E0;
                break;
            }
        case 'v':
            {
                if      ( L"nvsquare" == wsName ) return 0x33B5;
                break;
            }
        case 'w':
            {
                if      ( L"nwsquare" == wsName ) return 0x33BB;
                break;
            }
        case 'y':
            {
                if      ( L"nyabengali" == wsName ) return 0x099E;
                else if ( L"nyadeva" == wsName ) return 0x091E;
                else if ( L"nyagujarati" == wsName ) return 0x0A9E;
                else if ( L"nyagurmukhi" == wsName ) return 0x0A1E;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_o(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch ( wChar1 )
        {
        case 0:
            {
                if      ( L"o" == wsName ) return 0x006F;
                break;
            }
        case 'a':
            {
                if      ( L"oacute" == wsName ) return 0x00F3;
                else if ( L"oangthai" == wsName ) return 0x0E2D;
                break;
            }
        case 'b':
            {
                if      ( L"obarred" == wsName ) return 0x0275;
                else if ( L"obarredcyrillic" == wsName ) return 0x04E9;
                else if ( L"obarreddieresiscyrillic" == wsName ) return 0x04EB;
                else if ( L"obengali" == wsName ) return 0x0993;
                else if ( L"obopomofo" == wsName ) return 0x311B;
                else if ( L"obreve" == wsName ) return 0x014F;
                break;
            }
        case 'c':
            {
                if      ( L"ocandradeva" == wsName ) return 0x0911;
                else if ( L"ocandragujarati" == wsName ) return 0x0A91;
                else if ( L"ocandravowelsigndeva" == wsName ) return 0x0949;
                else if ( L"ocandravowelsigngujarati" == wsName ) return 0x0AC9;
                else if ( L"ocaron" == wsName ) return 0x01D2;
                else if ( L"ocircle" == wsName ) return 0x24DE;
                else if ( L"ocircumflex" == wsName ) return 0x00F4;
                else if ( L"ocircumflexacute" == wsName ) return 0x1ED1;
                else if ( L"ocircumflexdotbelow" == wsName ) return 0x1ED9;
                else if ( L"ocircumflexgrave" == wsName ) return 0x1ED3;
                else if ( L"ocircumflexhookabove" == wsName ) return 0x1ED5;
                else if ( L"ocircumflextilde" == wsName ) return 0x1ED7;
                else if ( L"ocyrillic" == wsName ) return 0x043E;
                break;
            }
        case 'd':
            {
                if      ( L"odblacute" == wsName ) return 0x0151;
                else if ( L"odblgrave" == wsName ) return 0x020D;
                else if ( L"odeva" == wsName ) return 0x0913;
                else if ( L"odieresis" == wsName ) return 0x00F6;
                else if ( L"odieresiscyrillic" == wsName ) return 0x04E7;
                else if ( L"odotbelow" == wsName ) return 0x1ECD;
                break;
            }
        case 'e':
            {
                if      ( L"oe" == wsName ) return 0x0153;
                else if ( L"oekorean" == wsName ) return 0x315A;
                break;
            }
        case 'g':
            {
                if      ( L"ogonek" == wsName ) return 0x02DB;
                else if ( L"ogonekcmb" == wsName ) return 0x0328;
                else if ( L"ograve" == wsName ) return 0x00F2;
                else if ( L"ogujarati" == wsName ) return 0x0A93;
                break;
            }
        case 'h':
            {
                if      ( L"oharmenian" == wsName ) return 0x0585;
                else if ( L"ohiragana" == wsName ) return 0x304A;
                else if ( L"ohookabove" == wsName ) return 0x1ECF;
                else if ( L"ohorn" == wsName ) return 0x01A1;
                else if ( L"ohornacute" == wsName ) return 0x1EDB;
                else if ( L"ohorndotbelow" == wsName ) return 0x1EE3;
                else if ( L"ohorngrave" == wsName ) return 0x1EDD;
                else if ( L"ohornhookabove" == wsName ) return 0x1EDF;
                else if ( L"ohorntilde" == wsName ) return 0x1EE1;
                else if ( L"ohungarumlaut" == wsName ) return 0x0151;
                break;
            }
        case 'i':
            {
                if      ( L"oi" == wsName ) return 0x01A3;
                else if ( L"oinvertedbreve" == wsName ) return 0x020F;
                break;
            }
        case 'k':
            {
                if      ( L"okatakana" == wsName ) return 0x30AA;
                else if ( L"okatakanahalfwidth" == wsName ) return 0xFF75;
                else if ( L"okorean" == wsName ) return 0x3157;
                break;
            }
        case 'l':
            {
                if      ( L"olehebrew" == wsName ) return 0x05AB;
                break;
            }
        case 'm':
            {
                if      ( L"omacron" == wsName ) return 0x014D;
                else if ( L"omacronacute" == wsName ) return 0x1E53;
                else if ( L"omacrongrave" == wsName ) return 0x1E51;
                else if ( L"omdeva" == wsName ) return 0x0950;
                else if ( L"omega" == wsName ) return 0x03C9;
                else if ( L"omega1" == wsName ) return 0x03D6;
                else if ( L"omegacyrillic" == wsName ) return 0x0461;
                else if ( L"omegalatinclosed" == wsName ) return 0x0277;
                else if ( L"omegaroundcyrillic" == wsName ) return 0x047B;
                else if ( L"omegatitlocyrillic" == wsName ) return 0x047D;
                else if ( L"omegatonos" == wsName ) return 0x03CE;
                else if ( L"omgujarati" == wsName ) return 0x0AD0;
                else if ( L"omicron" == wsName ) return 0x03BF;
                else if ( L"omicrontonos" == wsName ) return 0x03CC;
                else if ( L"omonospace" == wsName ) return 0xFF4F;
                break;
            }
        case 'n':
            {
                if      ( L"one" == wsName ) return 0x0031;
                else if ( L"onearabic" == wsName ) return 0x0661;
                else if ( L"onebengali" == wsName ) return 0x09E7;
                else if ( L"onecircle" == wsName ) return 0x2460;
                else if ( L"onecircleinversesansserif" == wsName ) return 0x278A;
                else if ( L"onedeva" == wsName ) return 0x0967;
                else if ( L"onedotenleader" == wsName ) return 0x2024;
                else if ( L"oneeighth" == wsName ) return 0x215B;
                else if ( L"onefitted" == wsName ) return 0xF6DC;
                else if ( L"onegujarati" == wsName ) return 0x0AE7;
                else if ( L"onegurmukhi" == wsName ) return 0x0A67;
                else if ( L"onehackarabic" == wsName ) return 0x0661;
                else if ( L"onehalf" == wsName ) return 0x00BD;
                else if ( L"onehangzhou" == wsName ) return 0x3021;
                else if ( L"oneideographicparen" == wsName ) return 0x3220;
                else if ( L"oneinferior" == wsName ) return 0x2081;
                else if ( L"onemonospace" == wsName ) return 0xFF11;
                else if ( L"onenumeratorbengali" == wsName ) return 0x09F4;
                else if ( L"oneoldstyle" == wsName ) return 0xF731;
                else if ( L"oneparen" == wsName ) return 0x2474;
                else if ( L"oneperiod" == wsName ) return 0x2488;
                else if ( L"onepersian" == wsName ) return 0x06F1;
                else if ( L"onequarter" == wsName ) return 0x00BC;
                else if ( L"oneroman" == wsName ) return 0x2170;
                else if ( L"onesuperior" == wsName ) return 0x00B9;
                else if ( L"onethai" == wsName ) return 0x0E51;
                else if ( L"onethird" == wsName ) return 0x2153;
                break;
            }
        case 'o':
            {
                if      ( L"oogonek" == wsName ) return 0x01EB;
                else if ( L"oogonekmacron" == wsName ) return 0x01ED;
                else if ( L"oogurmukhi" == wsName ) return 0x0A13;
                else if ( L"oomatragurmukhi" == wsName ) return 0x0A4B;
                else if ( L"oopen" == wsName ) return 0x0254;
                break;
            }
        case 'p':
            {
                if      ( L"oparen" == wsName ) return 0x24AA;
                else if ( L"openbullet" == wsName ) return 0x25E6;
                else if ( L"option" == wsName ) return 0x2325;
                break;
            }
        case 'r':
            {
                if      ( L"ordfeminine" == wsName ) return 0x00AA;
                else if ( L"ordmasculine" == wsName ) return 0x00BA;
                else if ( L"orthogonal" == wsName ) return 0x221F;
                break;
            }
        case 's':
            {
                if      ( L"oshortdeva" == wsName ) return 0x0912;
                else if ( L"oshortvowelsigndeva" == wsName ) return 0x094A;
                else if ( L"oslash" == wsName ) return 0x00F8;
                else if ( L"oslashacute" == wsName ) return 0x01FF;
                else if ( L"osmallhiragana" == wsName ) return 0x3049;
                else if ( L"osmallkatakana" == wsName ) return 0x30A9;
                else if ( L"osmallkatakanahalfwidth" == wsName ) return 0xFF6B;
                else if ( L"ostrokeacute" == wsName ) return 0x01FF;
                else if ( L"osuperior" == wsName ) return 0xF6F0;
                break;
            }
        case 't':
            {
                if      ( L"otcyrillic" == wsName ) return 0x047F;
                else if ( L"otilde" == wsName ) return 0x00F5;
                else if ( L"otildeacute" == wsName ) return 0x1E4D;
                else if ( L"otildedieresis" == wsName ) return 0x1E4F;
                break;
            }
        case 'u':
            {
                if      ( L"oubopomofo" == wsName ) return 0x3121;
                break;
            }
        case 'v':
            {
                if      ( L"overline" == wsName ) return 0x203E;
                else if ( L"overlinecenterline" == wsName ) return 0xFE4A;
                else if ( L"overlinecmb" == wsName ) return 0x0305;
                else if ( L"overlinedashed" == wsName ) return 0xFE49;
                else if ( L"overlinedblwavy" == wsName ) return 0xFE4C;
                else if ( L"overlinewavy" == wsName ) return 0xFE4B;
                else if ( L"overscore" == wsName ) return 0x00AF;
                else if ( L"ovowelsignbengali" == wsName ) return 0x09CB;
                else if ( L"ovowelsigndeva" == wsName ) return 0x094B;
                else if ( L"ovowelsigngujarati" == wsName ) return 0x0ACB;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_p(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch ( wChar1 )
        {
        case 0:
            {
                if      ( L"p" == wsName ) return 0x0070;
                break;
            }
        case 'a':
            {
                if      ( L"paampssquare" == wsName ) return 0x3380;
                else if ( L"paasentosquare" == wsName ) return 0x332B;
                else if ( L"pabengali" == wsName ) return 0x09AA;
                else if ( L"pacute" == wsName ) return 0x1E55;
                else if ( L"padeva" == wsName ) return 0x092A;
                else if ( L"pagedown" == wsName ) return 0x21DF;
                else if ( L"pageup" == wsName ) return 0x21DE;
                else if ( L"pagujarati" == wsName ) return 0x0AAA;
                else if ( L"pagurmukhi" == wsName ) return 0x0A2A;
                else if ( L"pahiragana" == wsName ) return 0x3071;
                else if ( L"paiyannoithai" == wsName ) return 0x0E2F;
                else if ( L"pakatakana" == wsName ) return 0x30D1;
                else if ( L"palatalizationcyrilliccmb" == wsName ) return 0x0484;
                else if ( L"palochkacyrillic" == wsName ) return 0x04C0;
                else if ( L"pansioskorean" == wsName ) return 0x317F;
                else if ( L"paragraph" == wsName ) return 0x00B6;
                else if ( L"parallel" == wsName ) return 0x2225;
                else if ( L"parenleft" == wsName ) return 0x0028;
                else if ( L"parenleftaltonearabic" == wsName ) return 0xFD3E;
                else if ( L"parenleftbt" == wsName ) return 0xF8ED;
                else if ( L"parenleftex" == wsName ) return 0xF8EC;
                else if ( L"parenleftinferior" == wsName ) return 0x208D;
                else if ( L"parenleftmonospace" == wsName ) return 0xFF08;
                else if ( L"parenleftsmall" == wsName ) return 0xFE59;
                else if ( L"parenleftsuperior" == wsName ) return 0x207D;
                else if ( L"parenlefttp" == wsName ) return 0xF8EB;
                else if ( L"parenleftvertical" == wsName ) return 0xFE35;
                else if ( L"parenright" == wsName ) return 0x0029;
                else if ( L"parenrightaltonearabic" == wsName ) return 0xFD3F;
                else if ( L"parenrightbt" == wsName ) return 0xF8F8;
                else if ( L"parenrightex" == wsName ) return 0xF8F7;
                else if ( L"parenrightinferior" == wsName ) return 0x208E;
                else if ( L"parenrightmonospace" == wsName ) return 0xFF09;
                else if ( L"parenrightsmall" == wsName ) return 0xFE5A;
                else if ( L"parenrightsuperior" == wsName ) return 0x207E;
                else if ( L"parenrighttp" == wsName ) return 0xF8F6;
                else if ( L"parenrightvertical" == wsName ) return 0xFE36;
                else if ( L"partialdiff" == wsName ) return 0x2202;
                else if ( L"paseqhebrew" == wsName ) return 0x05C0;
                else if ( L"pashtahebrew" == wsName ) return 0x0599;
                else if ( L"pasquare" == wsName ) return 0x33A9;
                else if ( L"patah" == wsName ) return 0x05B7;
                else if ( L"patah11" == wsName ) return 0x05B7;
                else if ( L"patah1d" == wsName ) return 0x05B7;
                else if ( L"patah2a" == wsName ) return 0x05B7;
                else if ( L"patahhebrew" == wsName ) return 0x05B7;
                else if ( L"patahnarrowhebrew" == wsName ) return 0x05B7;
                else if ( L"patahquarterhebrew" == wsName ) return 0x05B7;
                else if ( L"patahwidehebrew" == wsName ) return 0x05B7;
                else if ( L"pazerhebrew" == wsName ) return 0x05A1;
                break;
            }
        case 'b':
            {
                if      ( L"pbopomofo" == wsName ) return 0x3106;
                break;
            }
        case 'c':
            {
                if      ( L"pcircle" == wsName ) return 0x24DF;
                break;
            }
        case 'd':
            {
                if      ( L"pdotaccent" == wsName ) return 0x1E57;
                break;
            }
        case 'e':
            {
                if      ( L"pe" == wsName ) return 0x05E4;
                else if ( L"pecyrillic" == wsName ) return 0x043F;
                else if ( L"pedagesh" == wsName ) return 0xFB44;
                else if ( L"pedageshhebrew" == wsName ) return 0xFB44;
                else if ( L"peezisquare" == wsName ) return 0x333B;
                else if ( L"pefinaldageshhebrew" == wsName ) return 0xFB43;
                else if ( L"peharabic" == wsName ) return 0x067E;
                else if ( L"peharmenian" == wsName ) return 0x057A;
                else if ( L"pehebrew" == wsName ) return 0x05E4;
                else if ( L"pehfinalarabic" == wsName ) return 0xFB57;
                else if ( L"pehinitialarabic" == wsName ) return 0xFB58;
                else if ( L"pehiragana" == wsName ) return 0x307A;
                else if ( L"pehmedialarabic" == wsName ) return 0xFB59;
                else if ( L"pekatakana" == wsName ) return 0x30DA;
                else if ( L"pemiddlehookcyrillic" == wsName ) return 0x04A7;
                else if ( L"perafehebrew" == wsName ) return 0xFB4E;
                else if ( L"percent" == wsName ) return 0x0025;
                else if ( L"percentarabic" == wsName ) return 0x066A;
                else if ( L"percentmonospace" == wsName ) return 0xFF05;
                else if ( L"percentsmall" == wsName ) return 0xFE6A;
                else if ( L"period" == wsName ) return 0x002E;
                else if ( L"periodarmenian" == wsName ) return 0x0589;
                else if ( L"periodcentered" == wsName ) return 0x00B7;
                else if ( L"periodhalfwidth" == wsName ) return 0xFF61;
                else if ( L"periodinferior" == wsName ) return 0xF6E7;
                else if ( L"periodmonospace" == wsName ) return 0xFF0E;
                else if ( L"periodsmall" == wsName ) return 0xFE52;
                else if ( L"periodsuperior" == wsName ) return 0xF6E8;
                else if ( L"perispomenigreekcmb" == wsName ) return 0x0342;
                else if ( L"perpendicular" == wsName ) return 0x22A5;
                else if ( L"perthousand" == wsName ) return 0x2030;
                else if ( L"peseta" == wsName ) return 0x20A7;
                break;
            }
        case 'f':
            {
                if      ( L"pfsquare" == wsName ) return 0x338A;
                break;
            }
        case 'h':
            {
                if      ( L"phabengali" == wsName ) return 0x09AB;
                else if ( L"phadeva" == wsName ) return 0x092B;
                else if ( L"phagujarati" == wsName ) return 0x0AAB;
                else if ( L"phagurmukhi" == wsName ) return 0x0A2B;
                else if ( L"phi" == wsName ) return 0x03C6;
                else if ( L"phi1" == wsName ) return 0x03D5;
                else if ( L"phieuphacirclekorean" == wsName ) return 0x327A;
                else if ( L"phieuphaparenkorean" == wsName ) return 0x321A;
                else if ( L"phieuphcirclekorean" == wsName ) return 0x326C;
                else if ( L"phieuphkorean" == wsName ) return 0x314D;
                else if ( L"phieuphparenkorean" == wsName ) return 0x320C;
                else if ( L"philatin" == wsName ) return 0x0278;
                else if ( L"phinthuthai" == wsName ) return 0x0E3A;
                else if ( L"phisymbolgreek" == wsName ) return 0x03D5;
                else if ( L"phook" == wsName ) return 0x01A5;
                else if ( L"phophanthai" == wsName ) return 0x0E1E;
                else if ( L"phophungthai" == wsName ) return 0x0E1C;
                else if ( L"phosamphaothai" == wsName ) return 0x0E20;
                break;
            }
        case 'i':
            {
                if      ( L"pi" == wsName ) return 0x03C0;
                else if ( L"pieupacirclekorean" == wsName ) return 0x3273;
                else if ( L"pieupaparenkorean" == wsName ) return 0x3213;
                else if ( L"pieupcieuckorean" == wsName ) return 0x3176;
                else if ( L"pieupcirclekorean" == wsName ) return 0x3265;
                else if ( L"pieupkiyeokkorean" == wsName ) return 0x3172;
                else if ( L"pieupkorean" == wsName ) return 0x3142;
                else if ( L"pieupparenkorean" == wsName ) return 0x3205;
                else if ( L"pieupsioskiyeokkorean" == wsName ) return 0x3174;
                else if ( L"pieupsioskorean" == wsName ) return 0x3144;
                else if ( L"pieupsiostikeutkorean" == wsName ) return 0x3175;
                else if ( L"pieupthieuthkorean" == wsName ) return 0x3177;
                else if ( L"pieuptikeutkorean" == wsName ) return 0x3173;
                else if ( L"pihiragana" == wsName ) return 0x3074;
                else if ( L"pikatakana" == wsName ) return 0x30D4;
                else if ( L"pisymbolgreek" == wsName ) return 0x03D6;
                else if ( L"piwrarmenian" == wsName ) return 0x0583;
                break;
            }
        case 'l':
            {
                if      ( L"plus" == wsName ) return 0x002B;
                else if ( L"plusbelowcmb" == wsName ) return 0x031F;
                else if ( L"pluscircle" == wsName ) return 0x2295;
                else if ( L"plusminus" == wsName ) return 0x00B1;
                else if ( L"plusmod" == wsName ) return 0x02D6;
                else if ( L"plusmonospace" == wsName ) return 0xFF0B;
                else if ( L"plussmall" == wsName ) return 0xFE62;
                else if ( L"plussuperior" == wsName ) return 0x207A;
                break;
            }
        case 'm':
            {
                if      ( L"pmonospace" == wsName ) return 0xFF50;
                else if ( L"pmsquare" == wsName ) return 0x33D8;
                break;
            }
        case 'o':
            {
                if      ( L"pohiragana" == wsName ) return 0x307D;
                else if ( L"pointingindexdownwhite" == wsName ) return 0x261F;
                else if ( L"pointingindexleftwhite" == wsName ) return 0x261C;
                else if ( L"pointingindexrightwhite" == wsName ) return 0x261E;
                else if ( L"pointingindexupwhite" == wsName ) return 0x261D;
                else if ( L"pokatakana" == wsName ) return 0x30DD;
                else if ( L"poplathai" == wsName ) return 0x0E1B;
                else if ( L"postalmark" == wsName ) return 0x3012;
                else if ( L"postalmarkface" == wsName ) return 0x3020;
                break;
            }
        case 'p':
            {
                if      ( L"pparen" == wsName ) return 0x24AB;
                break;
            }
        case 'r':
            {
                if      ( L"precedes" == wsName ) return 0x227A;
                else if ( L"prescription" == wsName ) return 0x211E;
                else if ( L"primemod" == wsName ) return 0x02B9;
                else if ( L"primereversed" == wsName ) return 0x2035;
                else if ( L"product" == wsName ) return 0x220F;
                else if ( L"projective" == wsName ) return 0x2305;
                else if ( L"prolongedkana" == wsName ) return 0x30FC;
                else if ( L"propellor" == wsName ) return 0x2318;
                else if ( L"propersubset" == wsName ) return 0x2282;
                else if ( L"propersuperset" == wsName ) return 0x2283;
                else if ( L"proportion" == wsName ) return 0x2237;
                else if ( L"proportional" == wsName ) return 0x221D;
                break;
            }
        case 's':
            {
                if      ( L"psi" == wsName ) return 0x03C8;
                else if ( L"psicyrillic" == wsName ) return 0x0471;
                else if ( L"psilipneumatacyrilliccmb" == wsName ) return 0x0486;
                else if ( L"pssquare" == wsName ) return 0x33B0;
                break;
            }
        case 'u':
            {
                if      ( L"puhiragana" == wsName ) return 0x3077;
                else if ( L"pukatakana" == wsName ) return 0x30D7;
                break;
            }
        case 'v':
            {
                if      ( L"pvsquare" == wsName ) return 0x33B4;
                break;
            }
        case 'w':
            {
                if      ( L"pwsquare" == wsName ) return 0x33BA;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_q(CWCharWrapper &wsName)
    {
        if      ( L"q" == wsName ) return 0x0071;
        else if ( L"qadeva" == wsName ) return 0x0958;
        else if ( L"qadmahebrew" == wsName ) return 0x05A8;
        else if ( L"qafarabic" == wsName ) return 0x0642;
        else if ( L"qaffinalarabic" == wsName ) return 0xFED6;
        else if ( L"qafinitialarabic" == wsName ) return 0xFED7;
        else if ( L"qafmedialarabic" == wsName ) return 0xFED8;
        else if ( L"qamats" == wsName ) return 0x05B8;
        else if ( L"qamats10" == wsName ) return 0x05B8;
        else if ( L"qamats1a" == wsName ) return 0x05B8;
        else if ( L"qamats1c" == wsName ) return 0x05B8;
        else if ( L"qamats27" == wsName ) return 0x05B8;
        else if ( L"qamats29" == wsName ) return 0x05B8;
        else if ( L"qamats33" == wsName ) return 0x05B8;
        else if ( L"qamatsde" == wsName ) return 0x05B8;
        else if ( L"qamatshebrew" == wsName ) return 0x05B8;
        else if ( L"qamatsnarrowhebrew" == wsName ) return 0x05B8;
        else if ( L"qamatsqatanhebrew" == wsName ) return 0x05B8;
        else if ( L"qamatsqatannarrowhebrew" == wsName ) return 0x05B8;
        else if ( L"qamatsqatanquarterhebrew" == wsName ) return 0x05B8;
        else if ( L"qamatsqatanwidehebrew" == wsName ) return 0x05B8;
        else if ( L"qamatsquarterhebrew" == wsName ) return 0x05B8;
        else if ( L"qamatswidehebrew" == wsName ) return 0x05B8;
        else if ( L"qarneyparahebrew" == wsName ) return 0x059F;
        else if ( L"qbopomofo" == wsName ) return 0x3111;
        else if ( L"qcircle" == wsName ) return 0x24E0;
        else if ( L"qhook" == wsName ) return 0x02A0;
        else if ( L"qmonospace" == wsName ) return 0xFF51;
        else if ( L"qof" == wsName ) return 0x05E7;
        else if ( L"qofdagesh" == wsName ) return 0xFB47;
        else if ( L"qofdageshhebrew" == wsName ) return 0xFB47;
        else if ( L"qofhatafpatah" == wsName ) return 0x05E705B2;
        else if ( L"qofhatafpatahhebrew" == wsName ) return 0x05E705B2;
        else if ( L"qofhatafsegol" == wsName ) return 0x05E705B1;
        else if ( L"qofhatafsegolhebrew" == wsName ) return 0x05E705B1;
        else if ( L"qofhebrew" == wsName ) return 0x05E7;
        else if ( L"qofhiriq" == wsName ) return 0x05E705B4;
        else if ( L"qofhiriqhebrew" == wsName ) return 0x05E705B4;
        else if ( L"qofholam" == wsName ) return 0x05E705B9;
        else if ( L"qofholamhebrew" == wsName ) return 0x05E705B9;
        else if ( L"qofpatah" == wsName ) return 0x05E705B7;
        else if ( L"qofpatahhebrew" == wsName ) return 0x05E705B7;
        else if ( L"qofqamats" == wsName ) return 0x05E705B8;
        else if ( L"qofqamatshebrew" == wsName ) return 0x05E705B8;
        else if ( L"qofqubuts" == wsName ) return 0x05E705BB;
        else if ( L"qofqubutshebrew" == wsName ) return 0x05E705BB;
        else if ( L"qofsegol" == wsName ) return 0x05E705B6;
        else if ( L"qofsegolhebrew" == wsName ) return 0x05E705B6;
        else if ( L"qofsheva" == wsName ) return 0x05E705B0;
        else if ( L"qofshevahebrew" == wsName ) return 0x05E705B0;
        else if ( L"qoftsere" == wsName ) return 0x05E705B5;
        else if ( L"qoftserehebrew" == wsName ) return 0x05E705B5;
        else if ( L"qparen" == wsName ) return 0x24AC;
        else if ( L"quarternote" == wsName ) return 0x2669;
        else if ( L"qubuts" == wsName ) return 0x05BB;
        else if ( L"qubuts18" == wsName ) return 0x05BB;
        else if ( L"qubuts25" == wsName ) return 0x05BB;
        else if ( L"qubuts31" == wsName ) return 0x05BB;
        else if ( L"qubutshebrew" == wsName ) return 0x05BB;
        else if ( L"qubutsnarrowhebrew" == wsName ) return 0x05BB;
        else if ( L"qubutsquarterhebrew" == wsName ) return 0x05BB;
        else if ( L"qubutswidehebrew" == wsName ) return 0x05BB;
        else if ( L"question" == wsName ) return 0x003F;
        else if ( L"questionarabic" == wsName ) return 0x061F;
        else if ( L"questionarmenian" == wsName ) return 0x055E;
        else if ( L"questiondown" == wsName ) return 0x00BF;
        else if ( L"questiondownsmall" == wsName ) return 0xF7BF;
        else if ( L"questiongreek" == wsName ) return 0x037E;
        else if ( L"questionmonospace" == wsName ) return 0xFF1F;
        else if ( L"questionsmall" == wsName ) return 0xF73F;
        else if ( L"quotedbl" == wsName ) return 0x0022;
        else if ( L"quotedblbase" == wsName ) return 0x201E;
        else if ( L"quotedblleft" == wsName ) return 0x201C;
        else if ( L"quotedblmonospace" == wsName ) return 0xFF02;
        else if ( L"quotedblprime" == wsName ) return 0x301E;
        else if ( L"quotedblprimereversed" == wsName ) return 0x301D;
        else if ( L"quotedblright" == wsName ) return 0x201D;
        else if ( L"quoteleft" == wsName ) return 0x2018;
        else if ( L"quoteleftreversed" == wsName ) return 0x201B;
        else if ( L"quotereversed" == wsName ) return 0x201B;
        else if ( L"quoteright" == wsName ) return 0x2019;
        else if ( L"quoterightn" == wsName ) return 0x0149;
        else if ( L"quotesinglbase" == wsName ) return 0x201A;
        else if ( L"quotesingle" == wsName ) return 0x0027;
        else if ( L"quotesinglemonospace" == wsName ) return 0xFF07;

        return 0;
    }
    static int Type1NameToUnicode_r(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch ( wChar1 )
        {
        case 0:
            {
                if      ( L"r" == wsName ) return 0x0072;
                break;
            }
        case 'a':
            {
                if      ( L"raarmenian" == wsName ) return 0x057C;
                else if ( L"rabengali" == wsName ) return 0x09B0;
                else if ( L"racute" == wsName ) return 0x0155;
                else if ( L"radeva" == wsName ) return 0x0930;
                else if ( L"radical" == wsName ) return 0x221A;
                else if ( L"radicalex" == wsName ) return 0xF8E5;
                else if ( L"radoverssquare" == wsName ) return 0x33AE;
                else if ( L"radoverssquaredsquare" == wsName ) return 0x33AF;
                else if ( L"radsquare" == wsName ) return 0x33AD;
                else if ( L"rafe" == wsName ) return 0x05BF;
                else if ( L"rafehebrew" == wsName ) return 0x05BF;
                else if ( L"ragujarati" == wsName ) return 0x0AB0;
                else if ( L"ragurmukhi" == wsName ) return 0x0A30;
                else if ( L"rahiragana" == wsName ) return 0x3089;
                else if ( L"rakatakana" == wsName ) return 0x30E9;
                else if ( L"rakatakanahalfwidth" == wsName ) return 0xFF97;
                else if ( L"ralowerdiagonalbengali" == wsName ) return 0x09F1;
                else if ( L"ramiddlediagonalbengali" == wsName ) return 0x09F0;
                else if ( L"ramshorn" == wsName ) return 0x0264;
                else if ( L"ratio" == wsName ) return 0x2236;
                break;
            }
        case 'b':
            {
                if      ( L"rbopomofo" == wsName ) return 0x3116;
                break;
            }
        case 'c':
            {
                if      ( L"rcaron" == wsName ) return 0x0159;
                else if ( L"rcedilla" == wsName ) return 0x0157;
                else if ( L"rcircle" == wsName ) return 0x24E1;
                else if ( L"rcommaaccent" == wsName ) return 0x0157;
                break;
            }
        case 'd':
            {
                if      ( L"rdblgrave" == wsName ) return 0x0211;
                else if ( L"rdotaccent" == wsName ) return 0x1E59;
                else if ( L"rdotbelow" == wsName ) return 0x1E5B;
                else if ( L"rdotbelowmacron" == wsName ) return 0x1E5D;
                break;
            }
        case 'e':
            {
                if      ( L"referencemark" == wsName ) return 0x203B;
                else if ( L"reflexsubset" == wsName ) return 0x2286;
                else if ( L"reflexsuperset" == wsName ) return 0x2287;
                else if ( L"registered" == wsName ) return 0x00AE;
                else if ( L"registersans" == wsName ) return 0xF8E8;
                else if ( L"registerserif" == wsName ) return 0xF6DA;
                else if ( L"reharabic" == wsName ) return 0x0631;
                else if ( L"reharmenian" == wsName ) return 0x0580;
                else if ( L"rehfinalarabic" == wsName ) return 0xFEAE;
                else if ( L"rehiragana" == wsName ) return 0x308C;
                //{ "rehyehaleflamarabic" == wsName ) return "0631 FEF3 FE8E 0644";
                else if ( L"rekatakana" == wsName ) return 0x30EC;
                else if ( L"rekatakanahalfwidth" == wsName ) return 0xFF9A;
                else if ( L"resh" == wsName ) return 0x05E8;
                else if ( L"reshdageshhebrew" == wsName ) return 0xFB48;
                else if ( L"reshhatafpatah" == wsName ) return 0x05E805B2;
                else if ( L"reshhatafpatahhebrew" == wsName ) return 0x05E805B2;
                else if ( L"reshhatafsegol" == wsName ) return 0x05E805B1;
                else if ( L"reshhatafsegolhebrew" == wsName ) return 0x05E805B1;
                else if ( L"reshhebrew" == wsName ) return 0x05E8;
                else if ( L"reshhiriq" == wsName ) return 0x05E805B4;
                else if ( L"reshhiriqhebrew" == wsName ) return 0x05E805B4;
                else if ( L"reshholam" == wsName ) return 0x05E805B9;
                else if ( L"reshholamhebrew" == wsName ) return 0x05E805B9;
                else if ( L"reshpatah" == wsName ) return 0x05E805B7;
                else if ( L"reshpatahhebrew" == wsName ) return 0x05E805B7;
                else if ( L"reshqamats" == wsName ) return 0x05E805B8;
                else if ( L"reshqamatshebrew" == wsName ) return 0x05E805B8;
                else if ( L"reshqubuts" == wsName ) return 0x05E805BB;
                else if ( L"reshqubutshebrew" == wsName ) return 0x05E805BB;
                else if ( L"reshsegol" == wsName ) return 0x05E805B6;
                else if ( L"reshsegolhebrew" == wsName ) return 0x05E805B6;
                else if ( L"reshsheva" == wsName ) return 0x05E805B0;
                else if ( L"reshshevahebrew" == wsName ) return 0x05E805B0;
                else if ( L"reshtsere" == wsName ) return 0x05E805B5;
                else if ( L"reshtserehebrew" == wsName ) return 0x05E805B5;
                else if ( L"reversedtilde" == wsName ) return 0x223D;
                else if ( L"reviahebrew" == wsName ) return 0x0597;
                else if ( L"reviamugrashhebrew" == wsName ) return 0x0597;
                else if ( L"revlogicalnot" == wsName ) return 0x2310;
                break;
            }
        case 'f':
            {
                if      ( L"rfishhook" == wsName ) return 0x027E;
                else if ( L"rfishhookreversed" == wsName ) return 0x027F;
                break;
            }
        case 'h':
            {
                if      ( L"rhabengali" == wsName ) return 0x09DD;
                else if ( L"rhadeva" == wsName ) return 0x095D;
                else if ( L"rho" == wsName ) return 0x03C1;
                else if ( L"rhook" == wsName ) return 0x027D;
                else if ( L"rhookturned" == wsName ) return 0x027B;
                else if ( L"rhookturnedsuperior" == wsName ) return 0x02B5;
                else if ( L"rhosymbolgreek" == wsName ) return 0x03F1;
                else if ( L"rhotichookmod" == wsName ) return 0x02DE;
                break;
            }
        case 'i':
            {
                if      ( L"rieulacirclekorean" == wsName ) return 0x3271;
                else if ( L"rieulaparenkorean" == wsName ) return 0x3211;
                else if ( L"rieulcirclekorean" == wsName ) return 0x3263;
                else if ( L"rieulhieuhkorean" == wsName ) return 0x3140;
                else if ( L"rieulkiyeokkorean" == wsName ) return 0x313A;
                else if ( L"rieulkiyeoksioskorean" == wsName ) return 0x3169;
                else if ( L"rieulkorean" == wsName ) return 0x3139;
                else if ( L"rieulmieumkorean" == wsName ) return 0x313B;
                else if ( L"rieulpansioskorean" == wsName ) return 0x316C;
                else if ( L"rieulparenkorean" == wsName ) return 0x3203;
                else if ( L"rieulphieuphkorean" == wsName ) return 0x313F;
                else if ( L"rieulpieupkorean" == wsName ) return 0x313C;
                else if ( L"rieulpieupsioskorean" == wsName ) return 0x316B;
                else if ( L"rieulsioskorean" == wsName ) return 0x313D;
                else if ( L"rieulthieuthkorean" == wsName ) return 0x313E;
                else if ( L"rieultikeutkorean" == wsName ) return 0x316A;
                else if ( L"rieulyeorinhieuhkorean" == wsName ) return 0x316D;
                else if ( L"rightangle" == wsName ) return 0x221F;
                else if ( L"righttackbelowcmb" == wsName ) return 0x0319;
                else if ( L"righttriangle" == wsName ) return 0x22BF;
                else if ( L"rihiragana" == wsName ) return 0x308A;
                else if ( L"rikatakana" == wsName ) return 0x30EA;
                else if ( L"rikatakanahalfwidth" == wsName ) return 0xFF98;
                else if ( L"ring" == wsName ) return 0x02DA;
                else if ( L"ringbelowcmb" == wsName ) return 0x0325;
                else if ( L"ringcmb" == wsName ) return 0x030A;
                else if ( L"ringhalfleft" == wsName ) return 0x02BF;
                else if ( L"ringhalfleftarmenian" == wsName ) return 0x0559;
                else if ( L"ringhalfleftbelowcmb" == wsName ) return 0x031C;
                else if ( L"ringhalfleftcentered" == wsName ) return 0x02D3;
                else if ( L"ringhalfright" == wsName ) return 0x02BE;
                else if ( L"ringhalfrightbelowcmb" == wsName ) return 0x0339;
                else if ( L"ringhalfrightcentered" == wsName ) return 0x02D2;
                else if ( L"rinvertedbreve" == wsName ) return 0x0213;
                else if ( L"rittorusquare" == wsName ) return 0x3351;
                break;
            }
        case 'l':
            {
                if      ( L"rlinebelow" == wsName ) return 0x1E5F;
                else if ( L"rlongleg" == wsName ) return 0x027C;
                else if ( L"rlonglegturned" == wsName ) return 0x027A;
                break;
            }
        case 'm':
            {
                if      ( L"rmonospace" == wsName ) return 0xFF52;
                break;
            }
        case 'o':
            {
                if      ( L"rohiragana" == wsName ) return 0x308D;
                else if ( L"rokatakana" == wsName ) return 0x30ED;
                else if ( L"rokatakanahalfwidth" == wsName ) return 0xFF9B;
                else if ( L"roruathai" == wsName ) return 0x0E23;
                break;
            }
        case 'p':
            {
                if      ( L"rparen" == wsName ) return 0x24AD;
                break;
            }
        case 'r':
            {
                if      ( L"rrabengali" == wsName ) return 0x09DC;
                else if ( L"rradeva" == wsName ) return 0x0931;
                else if ( L"rragurmukhi" == wsName ) return 0x0A5C;
                else if ( L"rreharabic" == wsName ) return 0x0691;
                else if ( L"rrehfinalarabic" == wsName ) return 0xFB8D;
                else if ( L"rrvocalicbengali" == wsName ) return 0x09E0;
                else if ( L"rrvocalicdeva" == wsName ) return 0x0960;
                else if ( L"rrvocalicgujarati" == wsName ) return 0x0AE0;
                else if ( L"rrvocalicvowelsignbengali" == wsName ) return 0x09C4;
                else if ( L"rrvocalicvowelsigndeva" == wsName ) return 0x0944;
                else if ( L"rrvocalicvowelsigngujarati" == wsName ) return 0x0AC4;
                break;
            }
        case 's':
            {
                if      ( L"rsuperior" == wsName ) return 0xF6F1;
                break;
            }
        case 't':
            {
                if      ( L"rtblock" == wsName ) return 0x2590;
                else if ( L"rturned" == wsName ) return 0x0279;
                else if ( L"rturnedsuperior" == wsName ) return 0x02B4;
                break;
            }
        case 'u':
            {
                if      ( L"ruhiragana" == wsName ) return 0x308B;
                else if ( L"rukatakana" == wsName ) return 0x30EB;
                else if ( L"rukatakanahalfwidth" == wsName ) return 0xFF99;
                else if ( L"rupeemarkbengali" == wsName ) return 0x09F2;
                else if ( L"rupeesignbengali" == wsName ) return 0x09F3;
                else if ( L"rupiah" == wsName ) return 0xF6DD;
                else if ( L"ruthai" == wsName ) return 0x0E24;
                break;
            }
        case 'v':
            {
                if      ( L"rvocalicbengali" == wsName ) return 0x098B;
                else if ( L"rvocalicdeva" == wsName ) return 0x090B;
                else if ( L"rvocalicgujarati" == wsName ) return 0x0A8B;
                else if ( L"rvocalicvowelsignbengali" == wsName ) return 0x09C3;
                else if ( L"rvocalicvowelsigndeva" == wsName ) return 0x0943;
                else if ( L"rvocalicvowelsigngujarati" == wsName ) return 0x0AC3;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_s(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch ( wChar1 )
        {
        case 0:
            {
                if      ( L"s" == wsName ) return 0x0073;
                break;
            }
        case 'a':
            {
                if      ( L"sabengali" == wsName ) return 0x09B8;
                else if ( L"sacute" == wsName ) return 0x015B;
                else if ( L"sacutedotaccent" == wsName ) return 0x1E65;
                else if ( L"sadarabic" == wsName ) return 0x0635;
                else if ( L"sadeva" == wsName ) return 0x0938;
                else if ( L"sadfinalarabic" == wsName ) return 0xFEBA;
                else if ( L"sadinitialarabic" == wsName ) return 0xFEBB;
                else if ( L"sadmedialarabic" == wsName ) return 0xFEBC;
                else if ( L"sagujarati" == wsName ) return 0x0AB8;
                else if ( L"sagurmukhi" == wsName ) return 0x0A38;
                else if ( L"sahiragana" == wsName ) return 0x3055;
                else if ( L"sakatakana" == wsName ) return 0x30B5;
                else if ( L"sakatakanahalfwidth" == wsName ) return 0xFF7B;
                else if ( L"sallallahoualayhewasallamarabic" == wsName ) return 0xFDFA;
                else if ( L"samekh" == wsName ) return 0x05E1;
                else if ( L"samekhdagesh" == wsName ) return 0xFB41;
                else if ( L"samekhdageshhebrew" == wsName ) return 0xFB41;
                else if ( L"samekhhebrew" == wsName ) return 0x05E1;
                else if ( L"saraaathai" == wsName ) return 0x0E32;
                else if ( L"saraaethai" == wsName ) return 0x0E41;
                else if ( L"saraaimaimalaithai" == wsName ) return 0x0E44;
                else if ( L"saraaimaimuanthai" == wsName ) return 0x0E43;
                else if ( L"saraamthai" == wsName ) return 0x0E33;
                else if ( L"saraathai" == wsName ) return 0x0E30;
                else if ( L"saraethai" == wsName ) return 0x0E40;
                else if ( L"saraiileftthai" == wsName ) return 0xF886;
                else if ( L"saraiithai" == wsName ) return 0x0E35;
                else if ( L"saraileftthai" == wsName ) return 0xF885;
                else if ( L"saraithai" == wsName ) return 0x0E34;
                else if ( L"saraothai" == wsName ) return 0x0E42;
                else if ( L"saraueeleftthai" == wsName ) return 0xF888;
                else if ( L"saraueethai" == wsName ) return 0x0E37;
                else if ( L"saraueleftthai" == wsName ) return 0xF887;
                else if ( L"sarauethai" == wsName ) return 0x0E36;
                else if ( L"sarauthai" == wsName ) return 0x0E38;
                else if ( L"sarauuthai" == wsName ) return 0x0E39;
                break;
            }
        case 'b':
            {
                if      ( L"sbopomofo" == wsName ) return 0x3119;
                break;
            }
        case 'c':
            {
                if      ( L"scaron" == wsName ) return 0x0161;
                else if ( L"scarondotaccent" == wsName ) return 0x1E67;
                else if ( L"scedilla" == wsName ) return 0x015F;
                else if ( L"schwa" == wsName ) return 0x0259;
                else if ( L"schwacyrillic" == wsName ) return 0x04D9;
                else if ( L"schwadieresiscyrillic" == wsName ) return 0x04DB;
                else if ( L"schwahook" == wsName ) return 0x025A;
                else if ( L"scircle" == wsName ) return 0x24E2;
                else if ( L"scircumflex" == wsName ) return 0x015D;
                else if ( L"scommaaccent" == wsName ) return 0x0219;
                break;
            }
        case 'd':
            {
                if      ( L"sdotaccent" == wsName ) return 0x1E61;
                else if ( L"sdotbelow" == wsName ) return 0x1E63;
                else if ( L"sdotbelowdotaccent" == wsName ) return 0x1E69;
                break;
            }
        case 'e':
            {
                if      ( L"seagullbelowcmb" == wsName ) return 0x033C;
                else if ( L"second" == wsName ) return 0x2033;
                else if ( L"secondtonechinese" == wsName ) return 0x02CA;
                else if ( L"section" == wsName ) return 0x00A7;
                else if ( L"seenarabic" == wsName ) return 0x0633;
                else if ( L"seenfinalarabic" == wsName ) return 0xFEB2;
                else if ( L"seeninitialarabic" == wsName ) return 0xFEB3;
                else if ( L"seenmedialarabic" == wsName ) return 0xFEB4;
                else if ( L"segol" == wsName ) return 0x05B6;
                else if ( L"segol13" == wsName ) return 0x05B6;
                else if ( L"segol1f" == wsName ) return 0x05B6;
                else if ( L"segol2c" == wsName ) return 0x05B6;
                else if ( L"segolhebrew" == wsName ) return 0x05B6;
                else if ( L"segolnarrowhebrew" == wsName ) return 0x05B6;
                else if ( L"segolquarterhebrew" == wsName ) return 0x05B6;
                else if ( L"segoltahebrew" == wsName ) return 0x0592;
                else if ( L"segolwidehebrew" == wsName ) return 0x05B6;
                else if ( L"seharmenian" == wsName ) return 0x057D;
                else if ( L"sehiragana" == wsName ) return 0x305B;
                else if ( L"sekatakana" == wsName ) return 0x30BB;
                else if ( L"sekatakanahalfwidth" == wsName ) return 0xFF7E;
                else if ( L"semicolon" == wsName ) return 0x003B;
                else if ( L"semicolonarabic" == wsName ) return 0x061B;
                else if ( L"semicolonmonospace" == wsName ) return 0xFF1B;
                else if ( L"semicolonsmall" == wsName ) return 0xFE54;
                else if ( L"semivoicedmarkkana" == wsName ) return 0x309C;
                else if ( L"semivoicedmarkkanahalfwidth" == wsName ) return 0xFF9F;
                else if ( L"sentisquare" == wsName ) return 0x3322;
                else if ( L"sentosquare" == wsName ) return 0x3323;
                else if ( L"seven" == wsName ) return 0x0037;
                else if ( L"sevenarabic" == wsName ) return 0x0667;
                else if ( L"sevenbengali" == wsName ) return 0x09ED;
                else if ( L"sevencircle" == wsName ) return 0x2466;
                else if ( L"sevencircleinversesansserif" == wsName ) return 0x2790;
                else if ( L"sevendeva" == wsName ) return 0x096D;
                else if ( L"seveneighths" == wsName ) return 0x215E;
                else if ( L"sevengujarati" == wsName ) return 0x0AED;
                else if ( L"sevengurmukhi" == wsName ) return 0x0A6D;
                else if ( L"sevenhackarabic" == wsName ) return 0x0667;
                else if ( L"sevenhangzhou" == wsName ) return 0x3027;
                else if ( L"sevenideographicparen" == wsName ) return 0x3226;
                else if ( L"seveninferior" == wsName ) return 0x2087;
                else if ( L"sevenmonospace" == wsName ) return 0xFF17;
                else if ( L"sevenoldstyle" == wsName ) return 0xF737;
                else if ( L"sevenparen" == wsName ) return 0x247A;
                else if ( L"sevenperiod" == wsName ) return 0x248E;
                else if ( L"sevenpersian" == wsName ) return 0x06F7;
                else if ( L"sevenroman" == wsName ) return 0x2176;
                else if ( L"sevensuperior" == wsName ) return 0x2077;
                else if ( L"seventeencircle" == wsName ) return 0x2470;
                else if ( L"seventeenparen" == wsName ) return 0x2484;
                else if ( L"seventeenperiod" == wsName ) return 0x2498;
                else if ( L"seventhai" == wsName ) return 0x0E57;
                break;
            }
        case 'f':
            {
                if      ( L"sfthyphen" == wsName ) return 0x00AD;
                break;
            }
        case 'h':
            {
                if      ( L"shaarmenian" == wsName ) return 0x0577;
                else if ( L"shabengali" == wsName ) return 0x09B6;
                else if ( L"shacyrillic" == wsName ) return 0x0448;
                else if ( L"shaddaarabic" == wsName ) return 0x0651;
                else if ( L"shaddadammaarabic" == wsName ) return 0xFC61;
                else if ( L"shaddadammatanarabic" == wsName ) return 0xFC5E;
                else if ( L"shaddafathaarabic" == wsName ) return 0xFC60;
                else if ( L"shaddafathatanarabic" == wsName ) return 0x0651064B;
                else if ( L"shaddakasraarabic" == wsName ) return 0xFC62;
                else if ( L"shaddakasratanarabic" == wsName ) return 0xFC5F;
                else if ( L"shade" == wsName ) return 0x2592;
                else if ( L"shadedark" == wsName ) return 0x2593;
                else if ( L"shadelight" == wsName ) return 0x2591;
                else if ( L"shademedium" == wsName ) return 0x2592;
                else if ( L"shadeva" == wsName ) return 0x0936;
                else if ( L"shagujarati" == wsName ) return 0x0AB6;
                else if ( L"shagurmukhi" == wsName ) return 0x0A36;
                else if ( L"shalshelethebrew" == wsName ) return 0x0593;
                else if ( L"shbopomofo" == wsName ) return 0x3115;
                else if ( L"shchacyrillic" == wsName ) return 0x0449;
                else if ( L"sheenarabic" == wsName ) return 0x0634;
                else if ( L"sheenfinalarabic" == wsName ) return 0xFEB6;
                else if ( L"sheeninitialarabic" == wsName ) return 0xFEB7;
                else if ( L"sheenmedialarabic" == wsName ) return 0xFEB8;
                else if ( L"sheicoptic" == wsName ) return 0x03E3;
                else if ( L"sheqel" == wsName ) return 0x20AA;
                else if ( L"sheqelhebrew" == wsName ) return 0x20AA;
                else if ( L"sheva" == wsName ) return 0x05B0;
                else if ( L"sheva115" == wsName ) return 0x05B0;
                else if ( L"sheva15" == wsName ) return 0x05B0;
                else if ( L"sheva22" == wsName ) return 0x05B0;
                else if ( L"sheva2e" == wsName ) return 0x05B0;
                else if ( L"shevahebrew" == wsName ) return 0x05B0;
                else if ( L"shevanarrowhebrew" == wsName ) return 0x05B0;
                else if ( L"shevaquarterhebrew" == wsName ) return 0x05B0;
                else if ( L"shevawidehebrew" == wsName ) return 0x05B0;
                else if ( L"shhacyrillic" == wsName ) return 0x04BB;
                else if ( L"shimacoptic" == wsName ) return 0x03ED;
                else if ( L"shin" == wsName ) return 0x05E9;
                else if ( L"shindagesh" == wsName ) return 0xFB49;
                else if ( L"shindageshhebrew" == wsName ) return 0xFB49;
                else if ( L"shindageshshindot" == wsName ) return 0xFB2C;
                else if ( L"shindageshshindothebrew" == wsName ) return 0xFB2C;
                else if ( L"shindageshsindot" == wsName ) return 0xFB2D;
                else if ( L"shindageshsindothebrew" == wsName ) return 0xFB2D;
                else if ( L"shindothebrew" == wsName ) return 0x05C1;
                else if ( L"shinhebrew" == wsName ) return 0x05E9;
                else if ( L"shinshindot" == wsName ) return 0xFB2A;
                else if ( L"shinshindothebrew" == wsName ) return 0xFB2A;
                else if ( L"shinsindot" == wsName ) return 0xFB2B;
                else if ( L"shinsindothebrew" == wsName ) return 0xFB2B;
                else if ( L"shook" == wsName ) return 0x0282;
                break;
            }
        case 'i':
            {
                if      ( L"sigma" == wsName ) return 0x03C3;
                else if ( L"sigma1" == wsName ) return 0x03C2;
                else if ( L"sigmafinal" == wsName ) return 0x03C2;
                else if ( L"sigmalunatesymbolgreek" == wsName ) return 0x03F2;
                else if ( L"sihiragana" == wsName ) return 0x3057;
                else if ( L"sikatakana" == wsName ) return 0x30B7;
                else if ( L"sikatakanahalfwidth" == wsName ) return 0xFF7C;
                else if ( L"siluqhebrew" == wsName ) return 0x05BD;
                else if ( L"siluqlefthebrew" == wsName ) return 0x05BD;
                else if ( L"similar" == wsName ) return 0x223C;
                else if ( L"sindothebrew" == wsName ) return 0x05C2;
                else if ( L"siosacirclekorean" == wsName ) return 0x3274;
                else if ( L"siosaparenkorean" == wsName ) return 0x3214;
                else if ( L"sioscieuckorean" == wsName ) return 0x317E;
                else if ( L"sioscirclekorean" == wsName ) return 0x3266;
                else if ( L"sioskiyeokkorean" == wsName ) return 0x317A;
                else if ( L"sioskorean" == wsName ) return 0x3145;
                else if ( L"siosnieunkorean" == wsName ) return 0x317B;
                else if ( L"siosparenkorean" == wsName ) return 0x3206;
                else if ( L"siospieupkorean" == wsName ) return 0x317D;
                else if ( L"siostikeutkorean" == wsName ) return 0x317C;
                else if ( L"six" == wsName ) return 0x0036;
                else if ( L"sixarabic" == wsName ) return 0x0666;
                else if ( L"sixbengali" == wsName ) return 0x09EC;
                else if ( L"sixcircle" == wsName ) return 0x2465;
                else if ( L"sixcircleinversesansserif" == wsName ) return 0x278F;
                else if ( L"sixdeva" == wsName ) return 0x096C;
                else if ( L"sixgujarati" == wsName ) return 0x0AEC;
                else if ( L"sixgurmukhi" == wsName ) return 0x0A6C;
                else if ( L"sixhackarabic" == wsName ) return 0x0666;
                else if ( L"sixhangzhou" == wsName ) return 0x3026;
                else if ( L"sixideographicparen" == wsName ) return 0x3225;
                else if ( L"sixinferior" == wsName ) return 0x2086;
                else if ( L"sixmonospace" == wsName ) return 0xFF16;
                else if ( L"sixoldstyle" == wsName ) return 0xF736;
                else if ( L"sixparen" == wsName ) return 0x2479;
                else if ( L"sixperiod" == wsName ) return 0x248D;
                else if ( L"sixpersian" == wsName ) return 0x06F6;
                else if ( L"sixroman" == wsName ) return 0x2175;
                else if ( L"sixsuperior" == wsName ) return 0x2076;
                else if ( L"sixteencircle" == wsName ) return 0x246F;
                else if ( L"sixteencurrencydenominatorbengali" == wsName ) return 0x09F9;
                else if ( L"sixteenparen" == wsName ) return 0x2483;
                else if ( L"sixteenperiod" == wsName ) return 0x2497;
                else if ( L"sixthai" == wsName ) return 0x0E56;
                break;
            }
        case 'l':
            {
                if      ( L"slash" == wsName ) return 0x002F;
                else if ( L"slashmonospace" == wsName ) return 0xFF0F;
                else if ( L"slong" == wsName ) return 0x017F;
                else if ( L"slongdotaccent" == wsName ) return 0x1E9B;
                break;
            }
        case 'm':
            {
                if      ( L"smileface" == wsName ) return 0x263A;
                else if ( L"smonospace" == wsName ) return 0xFF53;
                break;
            }
        case 'o':
            {
                if      ( L"sofpasuqhebrew" == wsName ) return 0x05C3;
                else if ( L"softhyphen" == wsName ) return 0x00AD;
                else if ( L"softsigncyrillic" == wsName ) return 0x044C;
                else if ( L"sohiragana" == wsName ) return 0x305D;
                else if ( L"sokatakana" == wsName ) return 0x30BD;
                else if ( L"sokatakanahalfwidth" == wsName ) return 0xFF7F;
                else if ( L"soliduslongoverlaycmb" == wsName ) return 0x0338;
                else if ( L"solidusshortoverlaycmb" == wsName ) return 0x0337;
                else if ( L"sorusithai" == wsName ) return 0x0E29;
                else if ( L"sosalathai" == wsName ) return 0x0E28;
                else if ( L"sosothai" == wsName ) return 0x0E0B;
                else if ( L"sosuathai" == wsName ) return 0x0E2A;
                break;
            }
        case 'p':
            {
                if      ( L"space" == wsName ) return 0x0020;
                else if ( L"spacehackarabic" == wsName ) return 0x0020;
                else if ( L"spade" == wsName ) return 0x2660;
                else if ( L"spadesuitblack" == wsName ) return 0x2660;
                else if ( L"spadesuitwhite" == wsName ) return 0x2664;
                else if ( L"sparen" == wsName ) return 0x24AE;
                break;
            }
        case 'q':
            {
                if      ( L"squarebelowcmb" == wsName ) return 0x033B;
                else if ( L"squarecc" == wsName ) return 0x33C4;
                else if ( L"squarecm" == wsName ) return 0x339D;
                else if ( L"squarediagonalcrosshatchfill" == wsName ) return 0x25A9;
                else if ( L"squarehorizontalfill" == wsName ) return 0x25A4;
                else if ( L"squarekg" == wsName ) return 0x338F;
                else if ( L"squarekm" == wsName ) return 0x339E;
                else if ( L"squarekmcapital" == wsName ) return 0x33CE;
                else if ( L"squareln" == wsName ) return 0x33D1;
                else if ( L"squarelog" == wsName ) return 0x33D2;
                else if ( L"squaremg" == wsName ) return 0x338E;
                else if ( L"squaremil" == wsName ) return 0x33D5;
                else if ( L"squaremm" == wsName ) return 0x339C;
                else if ( L"squaremsquared" == wsName ) return 0x33A1;
                else if ( L"squareorthogonalcrosshatchfill" == wsName ) return 0x25A6;
                else if ( L"squareupperlefttolowerrightfill" == wsName ) return 0x25A7;
                else if ( L"squareupperrighttolowerleftfill" == wsName ) return 0x25A8;
                else if ( L"squareverticalfill" == wsName ) return 0x25A5;
                else if ( L"squarewhitewithsmallblack" == wsName ) return 0x25A3;
                break;
            }
        case 'r':
            {
                if      ( L"srsquare" == wsName ) return 0x33DB;
                break;
            }
        case 's':
            {
                if      ( L"ssabengali" == wsName ) return 0x09B7;
                else if ( L"ssadeva" == wsName ) return 0x0937;
                else if ( L"ssagujarati" == wsName ) return 0x0AB7;
                else if ( L"ssangcieuckorean" == wsName ) return 0x3149;
                else if ( L"ssanghieuhkorean" == wsName ) return 0x3185;
                else if ( L"ssangieungkorean" == wsName ) return 0x3180;
                else if ( L"ssangkiyeokkorean" == wsName ) return 0x3132;
                else if ( L"ssangnieunkorean" == wsName ) return 0x3165;
                else if ( L"ssangpieupkorean" == wsName ) return 0x3143;
                else if ( L"ssangsioskorean" == wsName ) return 0x3146;
                else if ( L"ssangtikeutkorean" == wsName ) return 0x3138;
                else if ( L"ssuperior" == wsName ) return 0xF6F2;
                break;
            }
        case 't':
            {
                if      ( L"sterling" == wsName ) return 0x00A3;
                else if ( L"sterlingmonospace" == wsName ) return 0xFFE1;
                else if ( L"strokelongoverlaycmb" == wsName ) return 0x0336;
                else if ( L"strokeshortoverlaycmb" == wsName ) return 0x0335;
                break;
            }
        case 'u':
            {
                if      ( L"subset" == wsName ) return 0x2282;
                else if ( L"subsetnotequal" == wsName ) return 0x228A;
                else if ( L"subsetorequal" == wsName ) return 0x2286;
                else if ( L"succeeds" == wsName ) return 0x227B;
                else if ( L"suchthat" == wsName ) return 0x220B;
                else if ( L"suhiragana" == wsName ) return 0x3059;
                else if ( L"sukatakana" == wsName ) return 0x30B9;
                else if ( L"sukatakanahalfwidth" == wsName ) return 0xFF7D;
                else if ( L"sukunarabic" == wsName ) return 0x0652;
                else if ( L"summation" == wsName ) return 0x2211;
                else if ( L"sun" == wsName ) return 0x263C;
                else if ( L"superset" == wsName ) return 0x2283;
                else if ( L"supersetnotequal" == wsName ) return 0x228B;
                else if ( L"supersetorequal" == wsName ) return 0x2287;
                break;
            }
        case 'v':
            {
                if      ( L"svsquare" == wsName ) return 0x33DC;
                break;
            }
        case 'y':
            {
                if      ( L"syouwaerasquare" == wsName ) return 0x337C;
                break;
            }
        }


        return 0;
    }
    static int Type1NameToUnicode_t(CWCharWrapper &wsName)
    {
        wchar_t wChar1 = wsName[1];
        switch ( wChar1 )
        {
        case 0:
            {
                if      ( L"t" == wsName ) return 0x0074;
                break;
            }
        case 'a':
            {
                if      ( L"tabengali" == wsName ) return 0x09A4;
                else if ( L"tackdown" == wsName ) return 0x22A4;
                else if ( L"tackleft" == wsName ) return 0x22A3;
                else if ( L"tadeva" == wsName ) return 0x0924;
                else if ( L"tagujarati" == wsName ) return 0x0AA4;
                else if ( L"tagurmukhi" == wsName ) return 0x0A24;
                else if ( L"taharabic" == wsName ) return 0x0637;
                else if ( L"tahfinalarabic" == wsName ) return 0xFEC2;
                else if ( L"tahinitialarabic" == wsName ) return 0xFEC3;
                else if ( L"tahiragana" == wsName ) return 0x305F;
                else if ( L"tahmedialarabic" == wsName ) return 0xFEC4;
                else if ( L"taisyouerasquare" == wsName ) return 0x337D;
                else if ( L"takatakana" == wsName ) return 0x30BF;
                else if ( L"takatakanahalfwidth" == wsName ) return 0xFF80;
                else if ( L"tatweelarabic" == wsName ) return 0x0640;
                else if ( L"tau" == wsName ) return 0x03C4;
                else if ( L"tav" == wsName ) return 0x05EA;
                else if ( L"tavdages" == wsName ) return 0xFB4A;
                else if ( L"tavdagesh" == wsName ) return 0xFB4A;
                else if ( L"tavdageshhebrew" == wsName ) return 0xFB4A;
                else if ( L"tavhebrew" == wsName ) return 0x05EA;
                break;
            }
        case 'b':
            {
                if      ( L"tbar" == wsName ) return 0x0167;
                else if ( L"tbopomofo" == wsName ) return 0x310A;
                break;
            }
        case 'c':
            {
                if      ( L"tcaron" == wsName ) return 0x0165;
                else if ( L"tccurl" == wsName ) return 0x02A8;
                else if ( L"tcedilla" == wsName ) return 0x0163;
                else if ( L"tcheharabic" == wsName ) return 0x0686;
                else if ( L"tchehfinalarabic" == wsName ) return 0xFB7B;
                else if ( L"tchehinitialarabic" == wsName ) return 0xFB7C;
                else if ( L"tchehmedialarabic" == wsName ) return 0xFB7D;
                else if ( L"tchehmeeminitialarabic" == wsName ) return 0xFB7CFEE4;
                else if ( L"tcircle" == wsName ) return 0x24E3;
                else if ( L"tcircumflexbelow" == wsName ) return 0x1E71;
                else if ( L"tcommaaccent" == wsName ) return 0x0163;
                break;
            }
        case 'd':
            {
                if      ( L"tdieresis" == wsName ) return 0x1E97;
                else if ( L"tdotaccent" == wsName ) return 0x1E6B;
                else if ( L"tdotbelow" == wsName ) return 0x1E6D;
                break;
            }
        case 'e':
            {
                if      ( L"tecyrillic" == wsName ) return 0x0442;
                else if ( L"tedescendercyrillic" == wsName ) return 0x04AD;
                else if ( L"teharabic" == wsName ) return 0x062A;
                else if ( L"tehfinalarabic" == wsName ) return 0xFE96;
                else if ( L"tehhahinitialarabic" == wsName ) return 0xFCA2;
                else if ( L"tehhahisolatedarabic" == wsName ) return 0xFC0C;
                else if ( L"tehinitialarabic" == wsName ) return 0xFE97;
                else if ( L"tehiragana" == wsName ) return 0x3066;
                else if ( L"tehjeeminitialarabic" == wsName ) return 0xFCA1;
                else if ( L"tehjeemisolatedarabic" == wsName ) return 0xFC0B;
                else if ( L"tehmarbutaarabic" == wsName ) return 0x0629;
                else if ( L"tehmarbutafinalarabic" == wsName ) return 0xFE94;
                else if ( L"tehmedialarabic" == wsName ) return 0xFE98;
                else if ( L"tehmeeminitialarabic" == wsName ) return 0xFCA4;
                else if ( L"tehmeemisolatedarabic" == wsName ) return 0xFC0E;
                else if ( L"tehnoonfinalarabic" == wsName ) return 0xFC73;
                else if ( L"tekatakana" == wsName ) return 0x30C6;
                else if ( L"tekatakanahalfwidth" == wsName ) return 0xFF83;
                else if ( L"telephone" == wsName ) return 0x2121;
                else if ( L"telephoneblack" == wsName ) return 0x260E;
                else if ( L"telishagedolahebrew" == wsName ) return 0x05A0;
                else if ( L"telishaqetanahebrew" == wsName ) return 0x05A9;
                else if ( L"tencircle" == wsName ) return 0x2469;
                else if ( L"tenideographicparen" == wsName ) return 0x3229;
                else if ( L"tenparen" == wsName ) return 0x247D;
                else if ( L"tenperiod" == wsName ) return 0x2491;
                else if ( L"tenroman" == wsName ) return 0x2179;
                else if ( L"tesh" == wsName ) return 0x02A7;
                else if ( L"tet" == wsName ) return 0x05D8;
                else if ( L"tetdagesh" == wsName ) return 0xFB38;
                else if ( L"tetdageshhebrew" == wsName ) return 0xFB38;
                else if ( L"tethebrew" == wsName ) return 0x05D8;
                else if ( L"tetsecyrillic" == wsName ) return 0x04B5;
                else if ( L"tevirhebrew" == wsName ) return 0x059B;
                else if ( L"tevirlefthebrew" == wsName ) return 0x059B;
                break;
            }
        case 'h':
            {
                if      ( L"thabengali" == wsName ) return 0x09A5;
                else if ( L"thadeva" == wsName ) return 0x0925;
                else if ( L"thagujarati" == wsName ) return 0x0AA5;
                else if ( L"thagurmukhi" == wsName ) return 0x0A25;
                else if ( L"thalarabic" == wsName ) return 0x0630;
                else if ( L"thalfinalarabic" == wsName ) return 0xFEAC;
                else if ( L"thanthakhatlowleftthai" == wsName ) return 0xF898;
                else if ( L"thanthakhatlowrightthai" == wsName ) return 0xF897;
                else if ( L"thanthakhatthai" == wsName ) return 0x0E4C;
                else if ( L"thanthakhatupperleftthai" == wsName ) return 0xF896;
                else if ( L"theharabic" == wsName ) return 0x062B;
                else if ( L"thehfinalarabic" == wsName ) return 0xFE9A;
                else if ( L"thehinitialarabic" == wsName ) return 0xFE9B;
                else if ( L"thehmedialarabic" == wsName ) return 0xFE9C;
                else if ( L"thereexists" == wsName ) return 0x2203;
                else if ( L"therefore" == wsName ) return 0x2234;
                else if ( L"theta" == wsName ) return 0x03B8;
                else if ( L"theta1" == wsName ) return 0x03D1;
                else if ( L"thetasymbolgreek" == wsName ) return 0x03D1;
                else if ( L"thieuthacirclekorean" == wsName ) return 0x3279;
                else if ( L"thieuthaparenkorean" == wsName ) return 0x3219;
                else if ( L"thieuthcirclekorean" == wsName ) return 0x326B;
                else if ( L"thieuthkorean" == wsName ) return 0x314C;
                else if ( L"thieuthparenkorean" == wsName ) return 0x320B;
                else if ( L"thirteencircle" == wsName ) return 0x246C;
                else if ( L"thirteenparen" == wsName ) return 0x2480;
                else if ( L"thirteenperiod" == wsName ) return 0x2494;
                else if ( L"thonangmonthothai" == wsName ) return 0x0E11;
                else if ( L"thook" == wsName ) return 0x01AD;
                else if ( L"thophuthaothai" == wsName ) return 0x0E12;
                else if ( L"thorn" == wsName ) return 0x00FE;
                else if ( L"thothahanthai" == wsName ) return 0x0E17;
                else if ( L"thothanthai" == wsName ) return 0x0E10;
                else if ( L"thothongthai" == wsName ) return 0x0E18;
                else if ( L"thothungthai" == wsName ) return 0x0E16;
                else if ( L"thousandcyrillic" == wsName ) return 0x0482;
                else if ( L"thousandsseparatorarabic" == wsName ) return 0x066C;
                else if ( L"thousandsseparatorpersian" == wsName ) return 0x066C;
                else if ( L"three" == wsName ) return 0x0033;
                else if ( L"threearabic" == wsName ) return 0x0663;
                else if ( L"threebengali" == wsName ) return 0x09E9;
                else if ( L"threecircle" == wsName ) return 0x2462;
                else if ( L"threecircleinversesansserif" == wsName ) return 0x278C;
                else if ( L"threedeva" == wsName ) return 0x0969;
                else if ( L"threeeighths" == wsName ) return 0x215C;
                else if ( L"threegujarati" == wsName ) return 0x0AE9;
                else if ( L"threegurmukhi" == wsName ) return 0x0A69;
                else if ( L"threehackarabic" == wsName ) return 0x0663;
                else if ( L"threehangzhou" == wsName ) return 0x3023;
                else if ( L"threeideographicparen" == wsName ) return 0x3222;
                else if ( L"threeinferior" == wsName ) return 0x2083;
                else if ( L"threemonospace" == wsName ) return 0xFF13;
                else if ( L"threenumeratorbengali" == wsName ) return 0x09F6;
                else if ( L"threeoldstyle" == wsName ) return 0xF733;
                else if ( L"threeparen" == wsName ) return 0x2476;
                else if ( L"threeperiod" == wsName ) return 0x248A;
                else if ( L"threepersian" == wsName ) return 0x06F3;
                else if ( L"threequarters" == wsName ) return 0x00BE;
                else if ( L"threequartersemdash" == wsName ) return 0xF6DE;
                else if ( L"threeroman" == wsName ) return 0x2172;
                else if ( L"threesuperior" == wsName ) return 0x00B3;
                else if ( L"threethai" == wsName ) return 0x0E53;
                else if ( L"thzsquare" == wsName ) return 0x3394;
                break;
            }
        case 'i':
            {
                if      ( L"tihiragana" == wsName ) return 0x3061;
                else if ( L"tikatakana" == wsName ) return 0x30C1;
                else if ( L"tikatakanahalfwidth" == wsName ) return 0xFF81;
                else if ( L"tikeutacirclekorean" == wsName ) return 0x3270;
                else if ( L"tikeutaparenkorean" == wsName ) return 0x3210;
                else if ( L"tikeutcirclekorean" == wsName ) return 0x3262;
                else if ( L"tikeutkorean" == wsName ) return 0x3137;
                else if ( L"tikeutparenkorean" == wsName ) return 0x3202;
                else if ( L"tilde" == wsName ) return 0x02DC;
                else if ( L"tildebelowcmb" == wsName ) return 0x0330;
                else if ( L"tildecmb" == wsName ) return 0x0303;
                else if ( L"tildecomb" == wsName ) return 0x0303;
                else if ( L"tildedoublecmb" == wsName ) return 0x0360;
                else if ( L"tildeoperator" == wsName ) return 0x223C;
                else if ( L"tildeoverlaycmb" == wsName ) return 0x0334;
                else if ( L"tildeverticalcmb" == wsName ) return 0x033E;
                else if ( L"timescircle" == wsName ) return 0x2297;
                else if ( L"tipehahebrew" == wsName ) return 0x0596;
                else if ( L"tipehalefthebrew" == wsName ) return 0x0596;
                else if ( L"tippigurmukhi" == wsName ) return 0x0A70;
                else if ( L"titlocyrilliccmb" == wsName ) return 0x0483;
                else if ( L"tiwnarmenian" == wsName ) return 0x057F;
                break;
            }
        case 'l':
            {
                if      ( L"tlinebelow" == wsName ) return 0x1E6F;
                break;
            }
        case 'm':
            {
                if      ( L"tmonospace" == wsName ) return 0xFF54;
                break;
            }
        case 'o':
            {
                if      ( L"toarmenian" == wsName ) return 0x0569;
                else if ( L"tohiragana" == wsName ) return 0x3068;
                else if ( L"tokatakana" == wsName ) return 0x30C8;
                else if ( L"tokatakanahalfwidth" == wsName ) return 0xFF84;
                else if ( L"tonebarextrahighmod" == wsName ) return 0x02E5;
                else if ( L"tonebarextralowmod" == wsName ) return 0x02E9;
                else if ( L"tonebarhighmod" == wsName ) return 0x02E6;
                else if ( L"tonebarlowmod" == wsName ) return 0x02E8;
                else if ( L"tonebarmidmod" == wsName ) return 0x02E7;
                else if ( L"tonefive" == wsName ) return 0x01BD;
                else if ( L"tonesix" == wsName ) return 0x0185;
                else if ( L"tonetwo" == wsName ) return 0x01A8;
                else if ( L"tonos" == wsName ) return 0x0384;
                else if ( L"tonsquare" == wsName ) return 0x3327;
                else if ( L"topatakthai" == wsName ) return 0x0E0F;
                else if ( L"tortoiseshellbracketleft" == wsName ) return 0x3014;
                else if ( L"tortoiseshellbracketleftsmall" == wsName ) return 0xFE5D;
                else if ( L"tortoiseshellbracketleftvertical" == wsName ) return 0xFE39;
                else if ( L"tortoiseshellbracketright" == wsName ) return 0x3015;
                else if ( L"tortoiseshellbracketrightsmall" == wsName ) return 0xFE5E;
                else if ( L"tortoiseshellbracketrightvertical" == wsName ) return 0xFE3A;
                else if ( L"totaothai" == wsName ) return 0x0E15;
                break;
            }
        case 'p':
            {
                if      ( L"tpalatalhook" == wsName ) return 0x01AB;
                else if ( L"tparen" == wsName ) return 0x24AF;
                break;
            }
        case 'r':
            {
                if      ( L"trademark" == wsName ) return 0x2122;
                else if ( L"trademarksans" == wsName ) return 0xF8EA;
                else if ( L"trademarkserif" == wsName ) return 0xF6DB;
                else if ( L"tretroflexhook" == wsName ) return 0x0288;
                else if ( L"triagdn" == wsName ) return 0x25BC;
                else if ( L"triaglf" == wsName ) return 0x25C4;
                else if ( L"triagrt" == wsName ) return 0x25BA;
                else if ( L"triagup" == wsName ) return 0x25B2;
                break;
            }
        case 's':
            {
                if      ( L"ts" == wsName ) return 0x02A6;
                else if ( L"tsadi" == wsName ) return 0x05E6;
                else if ( L"tsadidagesh" == wsName ) return 0xFB46;
                else if ( L"tsadidageshhebrew" == wsName ) return 0xFB46;
                else if ( L"tsadihebrew" == wsName ) return 0x05E6;
                else if ( L"tsecyrillic" == wsName ) return 0x0446;
                else if ( L"tsere" == wsName ) return 0x05B5;
                else if ( L"tsere12" == wsName ) return 0x05B5;
                else if ( L"tsere1e" == wsName ) return 0x05B5;
                else if ( L"tsere2b" == wsName ) return 0x05B5;
                else if ( L"tserehebrew" == wsName ) return 0x05B5;
                else if ( L"tserenarrowhebrew" == wsName ) return 0x05B5;
                else if ( L"tserequarterhebrew" == wsName ) return 0x05B5;
                else if ( L"tserewidehebrew" == wsName ) return 0x05B5;
                else if ( L"tshecyrillic" == wsName ) return 0x045B;
                else if ( L"tsuperior" == wsName ) return 0xF6F3;
                break;
            }
        case 't':
            {
                if      ( L"ttabengali" == wsName ) return 0x099F;
                else if ( L"ttadeva" == wsName ) return 0x091F;
                else if ( L"ttagujarati" == wsName ) return 0x0A9F;
                else if ( L"ttagurmukhi" == wsName ) return 0x0A1F;
                else if ( L"tteharabic" == wsName ) return 0x0679;
                else if ( L"ttehfinalarabic" == wsName ) return 0xFB67;
                else if ( L"ttehinitialarabic" == wsName ) return 0xFB68;
                else if ( L"ttehmedialarabic" == wsName ) return 0xFB69;
                else if ( L"tthabengali" == wsName ) return 0x09A0;
                else if ( L"tthadeva" == wsName ) return 0x0920;
                else if ( L"tthagujarati" == wsName ) return 0x0AA0;
                else if ( L"tthagurmukhi" == wsName ) return 0x0A20;
                else if ( L"tturned" == wsName ) return 0x0287;
                break;
            }
        case 'u':
            {
                if      ( L"tuhiragana" == wsName ) return 0x3064;
                else if ( L"tukatakana" == wsName ) return 0x30C4;
                else if ( L"tukatakanahalfwidth" == wsName ) return 0xFF82;
                else if ( L"tusmallhiragana" == wsName ) return 0x3063;
                else if ( L"tusmallkatakana" == wsName ) return 0x30C3;
                else if ( L"tusmallkatakanahalfwidth" == wsName ) return 0xFF6F;
                break;
            }
        case 'w':
            {
                if      ( L"twelvecircle" == wsName ) return 0x246B;
                else if ( L"twelveparen" == wsName ) return 0x247F;
                else if ( L"twelveperiod" == wsName ) return 0x2493;
                else if ( L"twelveroman" == wsName ) return 0x217B;
                else if ( L"twentycircle" == wsName ) return 0x2473;
                else if ( L"twentyhangzhou" == wsName ) return 0x5344;
                else if ( L"twentyparen" == wsName ) return 0x2487;
                else if ( L"twentyperiod" == wsName ) return 0x249B;
                else if ( L"two" == wsName ) return 0x0032;
                else if ( L"twoarabic" == wsName ) return 0x0662;
                else if ( L"twobengali" == wsName ) return 0x09E8;
                else if ( L"twocircle" == wsName ) return 0x2461;
                else if ( L"twocircleinversesansserif" == wsName ) return 0x278B;
                else if ( L"twodeva" == wsName ) return 0x0968;
                else if ( L"twodotenleader" == wsName ) return 0x2025;
                else if ( L"twodotleader" == wsName ) return 0x2025;
                else if ( L"twodotleadervertical" == wsName ) return 0xFE30;
                else if ( L"twogujarati" == wsName ) return 0x0AE8;
                else if ( L"twogurmukhi" == wsName ) return 0x0A68;
                else if ( L"twohackarabic" == wsName ) return 0x0662;
                else if ( L"twohangzhou" == wsName ) return 0x3022;
                else if ( L"twoideographicparen" == wsName ) return 0x3221;
                else if ( L"twoinferior" == wsName ) return 0x2082;
                else if ( L"twomonospace" == wsName ) return 0xFF12;
                else if ( L"twonumeratorbengali" == wsName ) return 0x09F5;
                else if ( L"twooldstyle" == wsName ) return 0xF732;
                else if ( L"twoparen" == wsName ) return 0x2475;
                else if ( L"twoperiod" == wsName ) return 0x2489;
                else if ( L"twopersian" == wsName ) return 0x06F2;
                else if ( L"tworoman" == wsName ) return 0x2171;
                else if ( L"twostroke" == wsName ) return 0x01BB;
                else if ( L"twosuperior" == wsName ) return 0x00B2;
                else if ( L"twothai" == wsName ) return 0x0E52;
                else if ( L"twothirds" == wsName ) return 0x2154;
                break;
            }
        }

        return 0;
    }
    static int Type1NameToUnicode_u(CWCharWrapper &wsName)
    {
        if      ( L"u" == wsName ) return 0x0075;
        else if ( L"uacute" == wsName ) return 0x00FA;
        else if ( L"ubar" == wsName ) return 0x0289;
        else if ( L"ubengali" == wsName ) return 0x0989;
        else if ( L"ubopomofo" == wsName ) return 0x3128;
        else if ( L"ubreve" == wsName ) return 0x016D;
        else if ( L"ucaron" == wsName ) return 0x01D4;
        else if ( L"ucircle" == wsName ) return 0x24E4;
        else if ( L"ucircumflex" == wsName ) return 0x00FB;
        else if ( L"ucircumflexbelow" == wsName ) return 0x1E77;
        else if ( L"ucyrillic" == wsName ) return 0x0443;
        else if ( L"udattadeva" == wsName ) return 0x0951;
        else if ( L"udblacute" == wsName ) return 0x0171;
        else if ( L"udblgrave" == wsName ) return 0x0215;
        else if ( L"udeva" == wsName ) return 0x0909;
        else if ( L"udieresis" == wsName ) return 0x00FC;
        else if ( L"udieresisacute" == wsName ) return 0x01D8;
        else if ( L"udieresisbelow" == wsName ) return 0x1E73;
        else if ( L"udieresiscaron" == wsName ) return 0x01DA;
        else if ( L"udieresiscyrillic" == wsName ) return 0x04F1;
        else if ( L"udieresisgrave" == wsName ) return 0x01DC;
        else if ( L"udieresismacron" == wsName ) return 0x01D6;
        else if ( L"udotbelow" == wsName ) return 0x1EE5;
        else if ( L"ugrave" == wsName ) return 0x00F9;
        else if ( L"ugujarati" == wsName ) return 0x0A89;
        else if ( L"ugurmukhi" == wsName ) return 0x0A09;
        else if ( L"uhiragana" == wsName ) return 0x3046;
        else if ( L"uhookabove" == wsName ) return 0x1EE7;
        else if ( L"uhorn" == wsName ) return 0x01B0;
        else if ( L"uhornacute" == wsName ) return 0x1EE9;
        else if ( L"uhorndotbelow" == wsName ) return 0x1EF1;
        else if ( L"uhorngrave" == wsName ) return 0x1EEB;
        else if ( L"uhornhookabove" == wsName ) return 0x1EED;
        else if ( L"uhorntilde" == wsName ) return 0x1EEF;
        else if ( L"uhungarumlaut" == wsName ) return 0x0171;
        else if ( L"uhungarumlautcyrillic" == wsName ) return 0x04F3;
        else if ( L"uinvertedbreve" == wsName ) return 0x0217;
        else if ( L"ukatakana" == wsName ) return 0x30A6;
        else if ( L"ukatakanahalfwidth" == wsName ) return 0xFF73;
        else if ( L"ukcyrillic" == wsName ) return 0x0479;
        else if ( L"ukorean" == wsName ) return 0x315C;
        else if ( L"umacron" == wsName ) return 0x016B;
        else if ( L"umacroncyrillic" == wsName ) return 0x04EF;
        else if ( L"umacrondieresis" == wsName ) return 0x1E7B;
        else if ( L"umatragurmukhi" == wsName ) return 0x0A41;
        else if ( L"umonospace" == wsName ) return 0xFF55;
        else if ( L"underscore" == wsName ) return 0x005F;
        else if ( L"underscoredbl" == wsName ) return 0x2017;
        else if ( L"underscoremonospace" == wsName ) return 0xFF3F;
        else if ( L"underscorevertical" == wsName ) return 0xFE33;
        else if ( L"underscorewavy" == wsName ) return 0xFE4F;
        else if ( L"union" == wsName ) return 0x222A;
        else if ( L"universal" == wsName ) return 0x2200;
        else if ( L"uogonek" == wsName ) return 0x0173;
        else if ( L"uparen" == wsName ) return 0x24B0;
        else if ( L"upblock" == wsName ) return 0x2580;
        else if ( L"upperdothebrew" == wsName ) return 0x05C4;
        else if ( L"upsilon" == wsName ) return 0x03C5;
        else if ( L"upsilondieresis" == wsName ) return 0x03CB;
        else if ( L"upsilondieresistonos" == wsName ) return 0x03B0;
        else if ( L"upsilonlatin" == wsName ) return 0x028A;
        else if ( L"upsilontonos" == wsName ) return 0x03CD;
        else if ( L"uptackbelowcmb" == wsName ) return 0x031D;
        else if ( L"uptackmod" == wsName ) return 0x02D4;
        else if ( L"uragurmukhi" == wsName ) return 0x0A73;
        else if ( L"uring" == wsName ) return 0x016F;
        else if ( L"ushortcyrillic" == wsName ) return 0x045E;
        else if ( L"usmallhiragana" == wsName ) return 0x3045;
        else if ( L"usmallkatakana" == wsName ) return 0x30A5;
        else if ( L"usmallkatakanahalfwidth" == wsName ) return 0xFF69;
        else if ( L"ustraightcyrillic" == wsName ) return 0x04AF;
        else if ( L"ustraightstrokecyrillic" == wsName ) return 0x04B1;
        else if ( L"utilde" == wsName ) return 0x0169;
        else if ( L"utildeacute" == wsName ) return 0x1E79;
        else if ( L"utildebelow" == wsName ) return 0x1E75;
        else if ( L"uubengali" == wsName ) return 0x098A;
        else if ( L"uudeva" == wsName ) return 0x090A;
        else if ( L"uugujarati" == wsName ) return 0x0A8A;
        else if ( L"uugurmukhi" == wsName ) return 0x0A0A;
        else if ( L"uumatragurmukhi" == wsName ) return 0x0A42;
        else if ( L"uuvowelsignbengali" == wsName ) return 0x09C2;
        else if ( L"uuvowelsigndeva" == wsName ) return 0x0942;
        else if ( L"uuvowelsigngujarati" == wsName ) return 0x0AC2;
        else if ( L"uvowelsignbengali" == wsName ) return 0x09C1;
        else if ( L"uvowelsigndeva" == wsName ) return 0x0941;
        else if ( L"uvowelsigngujarati" == wsName ) return 0x0AC1;

        return 0;
    }
    static int Type1NameToUnicode_v(CWCharWrapper &wsName)
    {
        if      ( L"v" == wsName ) return 0x0076;
        else if ( L"vadeva" == wsName ) return 0x0935;
        else if ( L"vagujarati" == wsName ) return 0x0AB5;
        else if ( L"vagurmukhi" == wsName ) return 0x0A35;
        else if ( L"vakatakana" == wsName ) return 0x30F7;
        else if ( L"vav" == wsName ) return 0x05D5;
        else if ( L"vavdagesh" == wsName ) return 0xFB35;
        else if ( L"vavdagesh65" == wsName ) return 0xFB35;
        else if ( L"vavdageshhebrew" == wsName ) return 0xFB35;
        else if ( L"vavhebrew" == wsName ) return 0x05D5;
        else if ( L"vavholam" == wsName ) return 0xFB4B;
        else if ( L"vavholamhebrew" == wsName ) return 0xFB4B;
        else if ( L"vavvavhebrew" == wsName ) return 0x05F0;
        else if ( L"vavyodhebrew" == wsName ) return 0x05F1;
        else if ( L"vcircle" == wsName ) return 0x24E5;
        else if ( L"vdotbelow" == wsName ) return 0x1E7F;
        else if ( L"vecyrillic" == wsName ) return 0x0432;
        else if ( L"veharabic" == wsName ) return 0x06A4;
        else if ( L"vehfinalarabic" == wsName ) return 0xFB6B;
        else if ( L"vehinitialarabic" == wsName ) return 0xFB6C;
        else if ( L"vehmedialarabic" == wsName ) return 0xFB6D;
        else if ( L"vekatakana" == wsName ) return 0x30F9;
        else if ( L"venus" == wsName ) return 0x2640;
        else if ( L"verticalbar" == wsName ) return 0x007C;
        else if ( L"verticallineabovecmb" == wsName ) return 0x030D;
        else if ( L"verticallinebelowcmb" == wsName ) return 0x0329;
        else if ( L"verticallinelowmod" == wsName ) return 0x02CC;
        else if ( L"verticallinemod" == wsName ) return 0x02C8;
        else if ( L"vewarmenian" == wsName ) return 0x057E;
        else if ( L"vhook" == wsName ) return 0x028B;
        else if ( L"vikatakana" == wsName ) return 0x30F8;
        else if ( L"viramabengali" == wsName ) return 0x09CD;
        else if ( L"viramadeva" == wsName ) return 0x094D;
        else if ( L"viramagujarati" == wsName ) return 0x0ACD;
        else if ( L"visargabengali" == wsName ) return 0x0983;
        else if ( L"visargadeva" == wsName ) return 0x0903;
        else if ( L"visargagujarati" == wsName ) return 0x0A83;
        else if ( L"vmonospace" == wsName ) return 0xFF56;
        else if ( L"voarmenian" == wsName ) return 0x0578;
        else if ( L"voicediterationhiragana" == wsName ) return 0x309E;
        else if ( L"voicediterationkatakana" == wsName ) return 0x30FE;
        else if ( L"voicedmarkkana" == wsName ) return 0x309B;
        else if ( L"voicedmarkkanahalfwidth" == wsName ) return 0xFF9E;
        else if ( L"vokatakana" == wsName ) return 0x30FA;
        else if ( L"vparen" == wsName ) return 0x24B1;
        else if ( L"vtilde" == wsName ) return 0x1E7D;
        else if ( L"vturned" == wsName ) return 0x028C;
        else if ( L"vuhiragana" == wsName ) return 0x3094;
        else if ( L"vukatakana" == wsName ) return 0x30F4;

        return 0;
    }
    static int Type1NameToUnicode_w(CWCharWrapper &wsName)
    {
        if      ( L"w" == wsName ) return 0x0077;
        else if ( L"wacute" == wsName ) return 0x1E83;
        else if ( L"waekorean" == wsName ) return 0x3159;
        else if ( L"wahiragana" == wsName ) return 0x308F;
        else if ( L"wakatakana" == wsName ) return 0x30EF;
        else if ( L"wakatakanahalfwidth" == wsName ) return 0xFF9C;
        else if ( L"wakorean" == wsName ) return 0x3158;
        else if ( L"wasmallhiragana" == wsName ) return 0x308E;
        else if ( L"wasmallkatakana" == wsName ) return 0x30EE;
        else if ( L"wattosquare" == wsName ) return 0x3357;
        else if ( L"wavedash" == wsName ) return 0x301C;
        else if ( L"wavyunderscorevertical" == wsName ) return 0xFE34;
        else if ( L"wawarabic" == wsName ) return 0x0648;
        else if ( L"wawfinalarabic" == wsName ) return 0xFEEE;
        else if ( L"wawhamzaabovearabic" == wsName ) return 0x0624;
        else if ( L"wawhamzaabovefinalarabic" == wsName ) return 0xFE86;
        else if ( L"wbsquare" == wsName ) return 0x33DD;
        else if ( L"wcircle" == wsName ) return 0x24E6;
        else if ( L"wcircumflex" == wsName ) return 0x0175;
        else if ( L"wdieresis" == wsName ) return 0x1E85;
        else if ( L"wdotaccent" == wsName ) return 0x1E87;
        else if ( L"wdotbelow" == wsName ) return 0x1E89;
        else if ( L"wehiragana" == wsName ) return 0x3091;
        else if ( L"weierstrass" == wsName ) return 0x2118;
        else if ( L"wekatakana" == wsName ) return 0x30F1;
        else if ( L"wekorean" == wsName ) return 0x315E;
        else if ( L"weokorean" == wsName ) return 0x315D;
        else if ( L"wgrave" == wsName ) return 0x1E81;
        else if ( L"whitebullet" == wsName ) return 0x25E6;
        else if ( L"whitecircle" == wsName ) return 0x25CB;
        else if ( L"whitecircleinverse" == wsName ) return 0x25D9;
        else if ( L"whitecornerbracketleft" == wsName ) return 0x300E;
        else if ( L"whitecornerbracketleftvertical" == wsName ) return 0xFE43;
        else if ( L"whitecornerbracketright" == wsName ) return 0x300F;
        else if ( L"whitecornerbracketrightvertical" == wsName ) return 0xFE44;
        else if ( L"whitediamond" == wsName ) return 0x25C7;
        else if ( L"whitediamondcontainingblacksmalldiamond" == wsName ) return 0x25C8;
        else if ( L"whitedownpointingsmalltriangle" == wsName ) return 0x25BF;
        else if ( L"whitedownpointingtriangle" == wsName ) return 0x25BD;
        else if ( L"whiteleftpointingsmalltriangle" == wsName ) return 0x25C3;
        else if ( L"whiteleftpointingtriangle" == wsName ) return 0x25C1;
        else if ( L"whitelenticularbracketleft" == wsName ) return 0x3016;
        else if ( L"whitelenticularbracketright" == wsName ) return 0x3017;
        else if ( L"whiterightpointingsmalltriangle" == wsName ) return 0x25B9;
        else if ( L"whiterightpointingtriangle" == wsName ) return 0x25B7;
        else if ( L"whitesmallsquare" == wsName ) return 0x25AB;
        else if ( L"whitesmilingface" == wsName ) return 0x263A;
        else if ( L"whitesquare" == wsName ) return 0x25A1;
        else if ( L"whitestar" == wsName ) return 0x2606;
        else if ( L"whitetelephone" == wsName ) return 0x260F;
        else if ( L"whitetortoiseshellbracketleft" == wsName ) return 0x3018;
        else if ( L"whitetortoiseshellbracketright" == wsName ) return 0x3019;
        else if ( L"whiteuppointingsmalltriangle" == wsName ) return 0x25B5;
        else if ( L"whiteuppointingtriangle" == wsName ) return 0x25B3;
        else if ( L"wihiragana" == wsName ) return 0x3090;
        else if ( L"wikatakana" == wsName ) return 0x30F0;
        else if ( L"wikorean" == wsName ) return 0x315F;
        else if ( L"wmonospace" == wsName ) return 0xFF57;
        else if ( L"wohiragana" == wsName ) return 0x3092;
        else if ( L"wokatakana" == wsName ) return 0x30F2;
        else if ( L"wokatakanahalfwidth" == wsName ) return 0xFF66;
        else if ( L"won" == wsName ) return 0x20A9;
        else if ( L"wonmonospace" == wsName ) return 0xFFE6;
        else if ( L"wowaenthai" == wsName ) return 0x0E27;
        else if ( L"wparen" == wsName ) return 0x24B2;
        else if ( L"wring" == wsName ) return 0x1E98;
        else if ( L"wsuperior" == wsName ) return 0x02B7;
        else if ( L"wturned" == wsName ) return 0x028D;
        else if ( L"wynn" == wsName ) return 0x01BF;

        return 0;
    }
    static int Type1NameToUnicode_x(CWCharWrapper &wsName)
    {
        if      ( L"x" == wsName ) return 0x0078;
        else if ( L"xabovecmb" == wsName ) return 0x033D;
        else if ( L"xbopomofo" == wsName ) return 0x3112;
        else if ( L"xcircle" == wsName ) return 0x24E7;
        else if ( L"xdieresis" == wsName ) return 0x1E8D;
        else if ( L"xdotaccent" == wsName ) return 0x1E8B;
        else if ( L"xeharmenian" == wsName ) return 0x056D;
        else if ( L"xi" == wsName ) return 0x03BE;
        else if ( L"xmonospace" == wsName ) return 0xFF58;
        else if ( L"xparen" == wsName ) return 0x24B3;
        else if ( L"xsuperior" == wsName ) return 0x02E3;

        return 0;
    }
    static int Type1NameToUnicode_y(CWCharWrapper &wsName)
    {
        if      ( L"y" == wsName ) return 0x0079;
        else if ( L"yaadosquare" == wsName ) return 0x334E;
        else if ( L"yabengali" == wsName ) return 0x09AF;
        else if ( L"yacute" == wsName ) return 0x00FD;
        else if ( L"yadeva" == wsName ) return 0x092F;
        else if ( L"yaekorean" == wsName ) return 0x3152;
        else if ( L"yagujarati" == wsName ) return 0x0AAF;
        else if ( L"yagurmukhi" == wsName ) return 0x0A2F;
        else if ( L"yahiragana" == wsName ) return 0x3084;
        else if ( L"yakatakana" == wsName ) return 0x30E4;
        else if ( L"yakatakanahalfwidth" == wsName ) return 0xFF94;
        else if ( L"yakorean" == wsName ) return 0x3151;
        else if ( L"yamakkanthai" == wsName ) return 0x0E4E;
        else if ( L"yasmallhiragana" == wsName ) return 0x3083;
        else if ( L"yasmallkatakana" == wsName ) return 0x30E3;
        else if ( L"yasmallkatakanahalfwidth" == wsName ) return 0xFF6C;
        else if ( L"yatcyrillic" == wsName ) return 0x0463;
        else if ( L"ycircle" == wsName ) return 0x24E8;
        else if ( L"ycircumflex" == wsName ) return 0x0177;
        else if ( L"ydieresis" == wsName ) return 0x00FF;
        else if ( L"ydotaccent" == wsName ) return 0x1E8F;
        else if ( L"ydotbelow" == wsName ) return 0x1EF5;
        else if ( L"yeharabic" == wsName ) return 0x064A;
        else if ( L"yehbarreearabic" == wsName ) return 0x06D2;
        else if ( L"yehbarreefinalarabic" == wsName ) return 0xFBAF;
        else if ( L"yehfinalarabic" == wsName ) return 0xFEF2;
        else if ( L"yehhamzaabovearabic" == wsName ) return 0x0626;
        else if ( L"yehhamzaabovefinalarabic" == wsName ) return 0xFE8A;
        else if ( L"yehhamzaaboveinitialarabic" == wsName ) return 0xFE8B;
        else if ( L"yehhamzaabovemedialarabic" == wsName ) return 0xFE8C;
        else if ( L"yehinitialarabic" == wsName ) return 0xFEF3;
        else if ( L"yehmedialarabic" == wsName ) return 0xFEF4;
        else if ( L"yehmeeminitialarabic" == wsName ) return 0xFCDD;
        else if ( L"yehmeemisolatedarabic" == wsName ) return 0xFC58;
        else if ( L"yehnoonfinalarabic" == wsName ) return 0xFC94;
        else if ( L"yehthreedotsbelowarabic" == wsName ) return 0x06D1;
        else if ( L"yekorean" == wsName ) return 0x3156;
        else if ( L"yen" == wsName ) return 0x00A5;
        else if ( L"yenmonospace" == wsName ) return 0xFFE5;
        else if ( L"yeokorean" == wsName ) return 0x3155;
        else if ( L"yeorinhieuhkorean" == wsName ) return 0x3186;
        else if ( L"yerahbenyomohebrew" == wsName ) return 0x05AA;
        else if ( L"yerahbenyomolefthebrew" == wsName ) return 0x05AA;
        else if ( L"yericyrillic" == wsName ) return 0x044B;
        else if ( L"yerudieresiscyrillic" == wsName ) return 0x04F9;
        else if ( L"yesieungkorean" == wsName ) return 0x3181;
        else if ( L"yesieungpansioskorean" == wsName ) return 0x3183;
        else if ( L"yesieungsioskorean" == wsName ) return 0x3182;
        else if ( L"yetivhebrew" == wsName ) return 0x059A;
        else if ( L"ygrave" == wsName ) return 0x1EF3;
        else if ( L"yhook" == wsName ) return 0x01B4;
        else if ( L"yhookabove" == wsName ) return 0x1EF7;
        else if ( L"yiarmenian" == wsName ) return 0x0575;
        else if ( L"yicyrillic" == wsName ) return 0x0457;
        else if ( L"yikorean" == wsName ) return 0x3162;
        else if ( L"yinyang" == wsName ) return 0x262F;
        else if ( L"yiwnarmenian" == wsName ) return 0x0582;
        else if ( L"ymonospace" == wsName ) return 0xFF59;
        else if ( L"yod" == wsName ) return 0x05D9;
        else if ( L"yoddagesh" == wsName ) return 0xFB39;
        else if ( L"yoddageshhebrew" == wsName ) return 0xFB39;
        else if ( L"yodhebrew" == wsName ) return 0x05D9;
        else if ( L"yodyodhebrew" == wsName ) return 0x05F2;
        else if ( L"yodyodpatahhebrew" == wsName ) return 0xFB1F;
        else if ( L"yohiragana" == wsName ) return 0x3088;
        else if ( L"yoikorean" == wsName ) return 0x3189;
        else if ( L"yokatakana" == wsName ) return 0x30E8;
        else if ( L"yokatakanahalfwidth" == wsName ) return 0xFF96;
        else if ( L"yokorean" == wsName ) return 0x315B;
        else if ( L"yosmallhiragana" == wsName ) return 0x3087;
        else if ( L"yosmallkatakana" == wsName ) return 0x30E7;
        else if ( L"yosmallkatakanahalfwidth" == wsName ) return 0xFF6E;
        else if ( L"yotgreek" == wsName ) return 0x03F3;
        else if ( L"yoyaekorean" == wsName ) return 0x3188;
        else if ( L"yoyakorean" == wsName ) return 0x3187;
        else if ( L"yoyakthai" == wsName ) return 0x0E22;
        else if ( L"yoyingthai" == wsName ) return 0x0E0D;
        else if ( L"yparen" == wsName ) return 0x24B4;
        else if ( L"ypogegrammeni" == wsName ) return 0x037A;
        else if ( L"ypogegrammenigreekcmb" == wsName ) return 0x0345;
        else if ( L"yr" == wsName ) return 0x01A6;
        else if ( L"yring" == wsName ) return 0x1E99;
        else if ( L"ysuperior" == wsName ) return 0x02B8;
        else if ( L"ytilde" == wsName ) return 0x1EF9;
        else if ( L"yturned" == wsName ) return 0x028E;
        else if ( L"yuhiragana" == wsName ) return 0x3086;
        else if ( L"yuikorean" == wsName ) return 0x318C;
        else if ( L"yukatakana" == wsName ) return 0x30E6;
        else if ( L"yukatakanahalfwidth" == wsName ) return 0xFF95;
        else if ( L"yukorean" == wsName ) return 0x3160;
        else if ( L"yusbigcyrillic" == wsName ) return 0x046B;
        else if ( L"yusbigiotifiedcyrillic" == wsName ) return 0x046D;
        else if ( L"yuslittlecyrillic" == wsName ) return 0x0467;
        else if ( L"yuslittleiotifiedcyrillic" == wsName ) return 0x0469;
        else if ( L"yusmallhiragana" == wsName ) return 0x3085;
        else if ( L"yusmallkatakana" == wsName ) return 0x30E5;
        else if ( L"yusmallkatakanahalfwidth" == wsName ) return 0xFF6D;
        else if ( L"yuyekorean" == wsName ) return 0x318B;
        else if ( L"yuyeokorean" == wsName ) return 0x318A;
        else if ( L"yyabengali" == wsName ) return 0x09DF;
        else if ( L"yyadeva" == wsName ) return 0x095F;

        return 0;
    }
    static int Type1NameToUnicode_z(CWCharWrapper &wsName)
    {
        if      ( L"z" == wsName ) return 0x007A;
        else if ( L"zaarmenian" == wsName ) return 0x0566;
        else if ( L"zacute" == wsName ) return 0x017A;
        else if ( L"zadeva" == wsName ) return 0x095B;
        else if ( L"zagurmukhi" == wsName ) return 0x0A5B;
        else if ( L"zaharabic" == wsName ) return 0x0638;
        else if ( L"zahfinalarabic" == wsName ) return 0xFEC6;
        else if ( L"zahinitialarabic" == wsName ) return 0xFEC7;
        else if ( L"zahiragana" == wsName ) return 0x3056;
        else if ( L"zahmedialarabic" == wsName ) return 0xFEC8;
        else if ( L"zainarabic" == wsName ) return 0x0632;
        else if ( L"zainfinalarabic" == wsName ) return 0xFEB0;
        else if ( L"zakatakana" == wsName ) return 0x30B6;
        else if ( L"zaqefgadolhebrew" == wsName ) return 0x0595;
        else if ( L"zaqefqatanhebrew" == wsName ) return 0x0594;
        else if ( L"zarqahebrew" == wsName ) return 0x0598;
        else if ( L"zayin" == wsName ) return 0x05D6;
        else if ( L"zayindagesh" == wsName ) return 0xFB36;
        else if ( L"zayindageshhebrew" == wsName ) return 0xFB36;
        else if ( L"zayinhebrew" == wsName ) return 0x05D6;
        else if ( L"zbopomofo" == wsName ) return 0x3117;
        else if ( L"zcaron" == wsName ) return 0x017E;
        else if ( L"zcircle" == wsName ) return 0x24E9;
        else if ( L"zcircumflex" == wsName ) return 0x1E91;
        else if ( L"zcurl" == wsName ) return 0x0291;
        else if ( L"zdot" == wsName ) return 0x017C;
        else if ( L"zdotaccent" == wsName ) return 0x017C;
        else if ( L"zdotbelow" == wsName ) return 0x1E93;
        else if ( L"zecyrillic" == wsName ) return 0x0437;
        else if ( L"zedescendercyrillic" == wsName ) return 0x0499;
        else if ( L"zedieresiscyrillic" == wsName ) return 0x04DF;
        else if ( L"zehiragana" == wsName ) return 0x305C;
        else if ( L"zekatakana" == wsName ) return 0x30BC;
        else if ( L"zero" == wsName ) return 0x0030;
        else if ( L"zeroarabic" == wsName ) return 0x0660;
        else if ( L"zerobengali" == wsName ) return 0x09E6;
        else if ( L"zerodeva" == wsName ) return 0x0966;
        else if ( L"zerogujarati" == wsName ) return 0x0AE6;
        else if ( L"zerogurmukhi" == wsName ) return 0x0A66;
        else if ( L"zerohackarabic" == wsName ) return 0x0660;
        else if ( L"zeroinferior" == wsName ) return 0x2080;
        else if ( L"zeromonospace" == wsName ) return 0xFF10;
        else if ( L"zerooldstyle" == wsName ) return 0xF730;
        else if ( L"zeropersian" == wsName ) return 0x06F0;
        else if ( L"zerosuperior" == wsName ) return 0x2070;
        else if ( L"zerothai" == wsName ) return 0x0E50;
        else if ( L"zerowidthjoiner" == wsName ) return 0xFEFF;
        else if ( L"zerowidthnonjoiner" == wsName ) return 0x200C;
        else if ( L"zerowidthspace" == wsName ) return 0x200B;
        else if ( L"zeta" == wsName ) return 0x03B6;
        else if ( L"zhbopomofo" == wsName ) return 0x3113;
        else if ( L"zhearmenian" == wsName ) return 0x056A;
        else if ( L"zhebrevecyrillic" == wsName ) return 0x04C2;
        else if ( L"zhecyrillic" == wsName ) return 0x0436;
        else if ( L"zhedescendercyrillic" == wsName ) return 0x0497;
        else if ( L"zhedieresiscyrillic" == wsName ) return 0x04DD;
        else if ( L"zihiragana" == wsName ) return 0x3058;
        else if ( L"zikatakana" == wsName ) return 0x30B8;
        else if ( L"zinorhebrew" == wsName ) return 0x05AE;
        else if ( L"zlinebelow" == wsName ) return 0x1E95;
        else if ( L"zmonospace" == wsName ) return 0xFF5A;
        else if ( L"zohiragana" == wsName ) return 0x305E;
        else if ( L"zokatakana" == wsName ) return 0x30BE;
        else if ( L"zparen" == wsName ) return 0x24B5;
        else if ( L"zretroflexhook" == wsName ) return 0x0290;
        else if ( L"zstroke" == wsName ) return 0x01B6;
        else if ( L"zuhiragana" == wsName ) return 0x305A;
        else if ( L"zukatakana" == wsName ) return 0x30BA;

        return 0;
    }
    //-------------------------------------------------------------
    int Type1NameToUnicodeW(CWCharWrapper wsName)
    {
        wchar_t wChar0 = wsName[0];
        wchar_t wChar1 = wsName[1];

        switch(wChar0)
        {
        case 'A': return Type1NameToUnicode_A( wsName );
        case 'B': return Type1NameToUnicode_B( wsName );
        case 'C': return Type1NameToUnicode_C( wsName );
        case 'D': return Type1NameToUnicode_D( wsName );
        case 'E': return Type1NameToUnicode_E( wsName );
        case 'F': return Type1NameToUnicode_F( wsName );
        case 'G': return Type1NameToUnicode_G( wsName );
        case 'H': return Type1NameToUnicode_H( wsName );
        case 'I': return Type1NameToUnicode_I( wsName );
        case 'J': return Type1NameToUnicode_J( wsName );
        case 'K': return Type1NameToUnicode_K( wsName );
        case 'L': return Type1NameToUnicode_L( wsName );
        case 'M': return Type1NameToUnicode_M( wsName );
        case 'N': return Type1NameToUnicode_N( wsName );
        case 'O': return Type1NameToUnicode_O( wsName );
        case 'P': return Type1NameToUnicode_P( wsName );
        case 'Q': return Type1NameToUnicode_Q( wsName );
        case 'R': return Type1NameToUnicode_R( wsName );
        case 'S': return Type1NameToUnicode_S( wsName );
        case 'T': return Type1NameToUnicode_T( wsName );
        case 'U': return Type1NameToUnicode_U( wsName );
        case 'V': return Type1NameToUnicode_V( wsName );
        case 'W': return Type1NameToUnicode_W( wsName );
        case 'X': return Type1NameToUnicode_X( wsName );
        case 'Y': return Type1NameToUnicode_Y( wsName );
        case 'Z': return Type1NameToUnicode_Z( wsName );
        case 'a': return Type1NameToUnicode_a( wsName );
        case 'b': return Type1NameToUnicode_b( wsName );
        case 'c': return Type1NameToUnicode_c( wsName );
        case 'd': return Type1NameToUnicode_d( wsName );
        case 'e': return Type1NameToUnicode_e( wsName );
        case 'f': return Type1NameToUnicode_f( wsName );
        case 'g': return Type1NameToUnicode_g( wsName );
        case 'h': return Type1NameToUnicode_h( wsName );
        case 'i': return Type1NameToUnicode_i( wsName );
        case 'j': return Type1NameToUnicode_j( wsName );
        case 'k': return Type1NameToUnicode_k( wsName );
        case 'l': return Type1NameToUnicode_l( wsName );
        case 'm': return Type1NameToUnicode_m( wsName );
        case 'n': return Type1NameToUnicode_n( wsName );
        case 'o': return Type1NameToUnicode_o( wsName );
        case 'p': return Type1NameToUnicode_p( wsName );
        case 'q': return Type1NameToUnicode_q( wsName );
        case 'r': return Type1NameToUnicode_r( wsName );
        case 's': return Type1NameToUnicode_s( wsName );
        case 't': return Type1NameToUnicode_t( wsName );
        case 'u': return Type1NameToUnicode_u( wsName );
        case 'v': return Type1NameToUnicode_v( wsName );
        case 'w': return Type1NameToUnicode_w( wsName );
        case 'x': return Type1NameToUnicode_x( wsName );
        case 'y': return Type1NameToUnicode_y( wsName );
        case 'z': return Type1NameToUnicode_z( wsName );
        }

        return 0;
    };


    int Type1NameToUnicodeA(const char *sName)
    {
        std::wstring sUnicode = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sName, (LONG)strlen(sName));
        return Type1NameToUnicodeW( sUnicode.c_str() );
    }
}

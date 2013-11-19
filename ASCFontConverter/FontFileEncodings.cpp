#include "stdafx.h"

#include "FontFileEncodings.h"

static int Type1NameToUnicode_A(CWCharWrapper &wsName)
{
	if      ( _T("A") == wsName ) return 0x0041;
	else if ( _T("AE") == wsName ) return 0x00C6;
	else if ( _T("AEacute") == wsName ) return 0x01FC;
	else if ( _T("AEmacron") == wsName ) return 0x01E2;
	else if ( _T("AEsmall") == wsName ) return 0xF7E6;
	else if ( _T("Aacute") == wsName ) return 0x00C1;
	else if ( _T("Aacutesmall") == wsName ) return 0xF7E1;
	else if ( _T("Abreve") == wsName ) return 0x0102;
	else if ( _T("Abreveacute") == wsName ) return 0x1EAE;
	else if ( _T("Abrevecyrillic") == wsName ) return 0x04D0;
	else if ( _T("Abrevedotbelow") == wsName ) return 0x1EB6;
	else if ( _T("Abrevegrave") == wsName ) return 0x1EB0;
	else if ( _T("Abrevehookabove") == wsName ) return 0x1EB2;
	else if ( _T("Abrevetilde") == wsName ) return 0x1EB4;
	else if ( _T("Acaron") == wsName ) return 0x01CD;
	else if ( _T("Acircle") == wsName ) return 0x24B6;
	else if ( _T("Acircumflex") == wsName ) return 0x00C2;
	else if ( _T("Acircumflexacute") == wsName ) return 0x1EA4;
	else if ( _T("Acircumflexdotbelow") == wsName ) return 0x1EAC;
	else if ( _T("Acircumflexgrave") == wsName ) return 0x1EA6;
	else if ( _T("Acircumflexhookabove") == wsName ) return 0x1EA8;
	else if ( _T("Acircumflexsmall") == wsName ) return 0xF7E2;
	else if ( _T("Acircumflextilde") == wsName ) return 0x1EAA;
	else if ( _T("Acute") == wsName ) return 0xF6C9;
	else if ( _T("Acutesmall") == wsName ) return 0xF7B4;
	else if ( _T("Acyrillic") == wsName ) return 0x0410;
	else if ( _T("Adblgrave") == wsName ) return 0x0200;
	else if ( _T("Adieresis") == wsName ) return 0x00C4;
	else if ( _T("Adieresiscyrillic") == wsName ) return 0x04D2;
	else if ( _T("Adieresismacron") == wsName ) return 0x01DE;
	else if ( _T("Adieresissmall") == wsName ) return 0xF7E4;
	else if ( _T("Adotbelow") == wsName ) return 0x1EA0;
	else if ( _T("Adotmacron") == wsName ) return 0x01E0;
	else if ( _T("Agrave") == wsName ) return 0x00C0;
	else if ( _T("Agravesmall") == wsName ) return 0xF7E0;
	else if ( _T("Ahookabove") == wsName ) return 0x1EA2;
	else if ( _T("Aiecyrillic") == wsName ) return 0x04D4;
	else if ( _T("Ainvertedbreve") == wsName ) return 0x0202;
	else if ( _T("Alpha") == wsName ) return 0x0391;
	else if ( _T("Alphatonos") == wsName ) return 0x0386;
	else if ( _T("Amacron") == wsName ) return 0x0100;
	else if ( _T("Amonospace") == wsName ) return 0xFF21;
	else if ( _T("Aogonek") == wsName ) return 0x0104;
	else if ( _T("Aring") == wsName ) return 0x00C5;
	else if ( _T("Aringacute") == wsName ) return 0x01FA;
	else if ( _T("Aringbelow") == wsName ) return 0x1E00;
	else if ( _T("Aringsmall") == wsName ) return 0xF7E5;
	else if ( _T("Asmall") == wsName ) return 0xF761;
	else if ( _T("Atilde") == wsName ) return 0x00C3;
	else if ( _T("Atildesmall") == wsName ) return 0xF7E3;
	else if ( _T("Aybarmenian") == wsName ) return 0x0531;

	return 0;
}

static int Type1NameToUnicode_B(CWCharWrapper &wsName)
{
	if      ( _T("B") == wsName ) return 0x0042;
	else if ( _T("Bcircle") == wsName ) return 0x24B7;
	else if ( _T("Bdotaccent") == wsName ) return 0x1E02;
	else if ( _T("Bdotbelow") == wsName ) return 0x1E04;
	else if ( _T("Becyrillic") == wsName ) return 0x0411;
	else if ( _T("Benarmenian") == wsName ) return 0x0532;
	else if ( _T("Beta") == wsName ) return 0x0392;
	else if ( _T("Bhook") == wsName ) return 0x0181;
	else if ( _T("Blinebelow") == wsName ) return 0x1E06;
	else if ( _T("Bmonospace") == wsName ) return 0xFF22;
	else if ( _T("Brevesmall") == wsName ) return 0xF6F4;
	else if ( _T("Bsmall") == wsName ) return 0xF762;
	else if ( _T("Btopbar") == wsName ) return 0x0182;

	return 0;
}
static int Type1NameToUnicode_C(CWCharWrapper &wsName)
{
	if      ( _T("C") == wsName ) return 0x0043;
	else if ( _T("Caarmenian") == wsName ) return 0x053E;
	else if ( _T("Cacute") == wsName ) return 0x0106;
	else if ( _T("Caron") == wsName ) return 0xF6CA;
	else if ( _T("Caronsmall") == wsName ) return 0xF6F5;
	else if ( _T("Ccaron") == wsName ) return 0x010C;
	else if ( _T("Ccedilla") == wsName ) return 0x00C7;
	else if ( _T("Ccedillaacute") == wsName ) return 0x1E08;
	else if ( _T("Ccedillasmall") == wsName ) return 0xF7E7;
	else if ( _T("Ccircle") == wsName ) return 0x24B8;
	else if ( _T("Ccircumflex") == wsName ) return 0x0108;
	else if ( _T("Cdot") == wsName ) return 0x010A;
	else if ( _T("Cdotaccent") == wsName ) return 0x010A;
	else if ( _T("Cedillasmall") == wsName ) return 0xF7B8;
	else if ( _T("Chaarmenian") == wsName ) return 0x0549;
	else if ( _T("Cheabkhasiancyrillic") == wsName ) return 0x04BC;
	else if ( _T("Checyrillic") == wsName ) return 0x0427;
	else if ( _T("Chedescenderabkhasiancyrillic") == wsName ) return 0x04BE;
	else if ( _T("Chedescendercyrillic") == wsName ) return 0x04B6;
	else if ( _T("Chedieresiscyrillic") == wsName ) return 0x04F4;
	else if ( _T("Cheharmenian") == wsName ) return 0x0543;
	else if ( _T("Chekhakassiancyrillic") == wsName ) return 0x04CB;
	else if ( _T("Cheverticalstrokecyrillic") == wsName ) return 0x04B8;
	else if ( _T("Chi") == wsName ) return 0x03A7;
	else if ( _T("Chook") == wsName ) return 0x0187;
	else if ( _T("Circumflexsmall") == wsName ) return 0xF6F6;
	else if ( _T("Cmonospace") == wsName ) return 0xFF23;
	else if ( _T("Coarmenian") == wsName ) return 0x0551;
	else if ( _T("Csmall") == wsName ) return 0xF763;

	return 0;
}

static int Type1NameToUnicode_D(CWCharWrapper &wsName)
{
	if      ( _T("D") == wsName ) return 0x0044;
	else if ( _T("DZ") == wsName ) return 0x01F1;
	else if ( _T("DZcaron") == wsName ) return 0x01C4;
	else if ( _T("Daarmenian") == wsName ) return 0x0534;
	else if ( _T("Dafrican") == wsName ) return 0x0189;
	else if ( _T("Dcaron") == wsName ) return 0x010E;
	else if ( _T("Dcedilla") == wsName ) return 0x1E10;
	else if ( _T("Dcircle") == wsName ) return 0x24B9;
	else if ( _T("Dcircumflexbelow") == wsName ) return 0x1E12;
	else if ( _T("Dcroat") == wsName ) return 0x0110;
	else if ( _T("Ddotaccent") == wsName ) return 0x1E0A;
	else if ( _T("Ddotbelow") == wsName ) return 0x1E0C;
	else if ( _T("Decyrillic") == wsName ) return 0x0414;
	else if ( _T("Deicoptic") == wsName ) return 0x03EE;
	else if ( _T("Delta") == wsName ) return 0x2206;
	else if ( _T("Deltagreek") == wsName ) return 0x0394;
	else if ( _T("Dhook") == wsName ) return 0x018A;
	else if ( _T("Dieresis") == wsName ) return 0xF6CB;
	else if ( _T("DieresisAcute") == wsName ) return 0xF6CC;
	else if ( _T("DieresisGrave") == wsName ) return 0xF6CD;
	else if ( _T("Dieresissmall") == wsName ) return 0xF7A8;
	else if ( _T("Digammagreek") == wsName ) return 0x03DC;
	else if ( _T("Djecyrillic") == wsName ) return 0x0402;
	else if ( _T("Dlinebelow") == wsName ) return 0x1E0E;
	else if ( _T("Dmonospace") == wsName ) return 0xFF24;
	else if ( _T("Dotaccentsmall") == wsName ) return 0xF6F7;
	else if ( _T("Dslash") == wsName ) return 0x0110;
	else if ( _T("Dsmall") == wsName ) return 0xF764;
	else if ( _T("Dtopbar") == wsName ) return 0x018B;
	else if ( _T("Dz") == wsName ) return 0x01F2;
	else if ( _T("Dzcaron") == wsName ) return 0x01C5;
	else if ( _T("Dzeabkhasiancyrillic") == wsName ) return 0x04E0;
	else if ( _T("Dzecyrillic") == wsName ) return 0x0405;
	else if ( _T("Dzhecyrillic") == wsName ) return 0x040F;

	return 0;
}
static int Type1NameToUnicode_E(CWCharWrapper &wsName)
{
	if      ( _T("E") == wsName ) return 0x0045;
	else if ( _T("Eacute") == wsName ) return 0x00C9;
	else if ( _T("Eacutesmall") == wsName ) return 0xF7E9;
	else if ( _T("Ebreve") == wsName ) return 0x0114;
	else if ( _T("Ecaron") == wsName ) return 0x011A;
	else if ( _T("Ecedillabreve") == wsName ) return 0x1E1C;
	else if ( _T("Echarmenian") == wsName ) return 0x0535;
	else if ( _T("Ecircle") == wsName ) return 0x24BA;
	else if ( _T("Ecircumflex") == wsName ) return 0x00CA;
	else if ( _T("Ecircumflexacute") == wsName ) return 0x1EBE;
	else if ( _T("Ecircumflexbelow") == wsName ) return 0x1E18;
	else if ( _T("Ecircumflexdotbelow") == wsName ) return 0x1EC6;
	else if ( _T("Ecircumflexgrave") == wsName ) return 0x1EC0;
	else if ( _T("Ecircumflexhookabove") == wsName ) return 0x1EC2;
	else if ( _T("Ecircumflexsmall") == wsName ) return 0xF7EA;
	else if ( _T("Ecircumflextilde") == wsName ) return 0x1EC4;
	else if ( _T("Ecyrillic") == wsName ) return 0x0404;
	else if ( _T("Edblgrave") == wsName ) return 0x0204;
	else if ( _T("Edieresis") == wsName ) return 0x00CB;
	else if ( _T("Edieresissmall") == wsName ) return 0xF7EB;
	else if ( _T("Edot") == wsName ) return 0x0116;
	else if ( _T("Edotaccent") == wsName ) return 0x0116;
	else if ( _T("Edotbelow") == wsName ) return 0x1EB8;
	else if ( _T("Efcyrillic") == wsName ) return 0x0424;
	else if ( _T("Egrave") == wsName ) return 0x00C8;
	else if ( _T("Egravesmall") == wsName ) return 0xF7E8;
	else if ( _T("Eharmenian") == wsName ) return 0x0537;
	else if ( _T("Ehookabove") == wsName ) return 0x1EBA;
	else if ( _T("Eightroman") == wsName ) return 0x2167;
	else if ( _T("Einvertedbreve") == wsName ) return 0x0206;
	else if ( _T("Eiotifiedcyrillic") == wsName ) return 0x0464;
	else if ( _T("Elcyrillic") == wsName ) return 0x041B;
	else if ( _T("Elevenroman") == wsName ) return 0x216A;
	else if ( _T("Emacron") == wsName ) return 0x0112;
	else if ( _T("Emacronacute") == wsName ) return 0x1E16;
	else if ( _T("Emacrongrave") == wsName ) return 0x1E14;
	else if ( _T("Emcyrillic") == wsName ) return 0x041C;
	else if ( _T("Emonospace") == wsName ) return 0xFF25;
	else if ( _T("Encyrillic") == wsName ) return 0x041D;
	else if ( _T("Endescendercyrillic") == wsName ) return 0x04A2;
	else if ( _T("Eng") == wsName ) return 0x014A;
	else if ( _T("Enghecyrillic") == wsName ) return 0x04A4;
	else if ( _T("Enhookcyrillic") == wsName ) return 0x04C7;
	else if ( _T("Eogonek") == wsName ) return 0x0118;
	else if ( _T("Eopen") == wsName ) return 0x0190;
	else if ( _T("Epsilon") == wsName ) return 0x0395;
	else if ( _T("Epsilontonos") == wsName ) return 0x0388;
	else if ( _T("Ercyrillic") == wsName ) return 0x0420;
	else if ( _T("Ereversed") == wsName ) return 0x018E;
	else if ( _T("Ereversedcyrillic") == wsName ) return 0x042D;
	else if ( _T("Escyrillic") == wsName ) return 0x0421;
	else if ( _T("Esdescendercyrillic") == wsName ) return 0x04AA;
	else if ( _T("Esh") == wsName ) return 0x01A9;
	else if ( _T("Esmall") == wsName ) return 0xF765;
	else if ( _T("Eta") == wsName ) return 0x0397;
	else if ( _T("Etarmenian") == wsName ) return 0x0538;
	else if ( _T("Etatonos") == wsName ) return 0x0389;
	else if ( _T("Eth") == wsName ) return 0x00D0;
	else if ( _T("Ethsmall") == wsName ) return 0xF7F0;
	else if ( _T("Etilde") == wsName ) return 0x1EBC;
	else if ( _T("Etildebelow") == wsName ) return 0x1E1A;
	else if ( _T("Euro") == wsName ) return 0x20AC;
	else if ( _T("Ezh") == wsName ) return 0x01B7;
	else if ( _T("Ezhcaron") == wsName ) return 0x01EE;
	else if ( _T("Ezhreversed") == wsName ) return 0x01B8;

	return 0;
}
static int Type1NameToUnicode_F(CWCharWrapper &wsName)
{
	if      ( _T("F") == wsName ) return 0x0046;
	else if ( _T("Fcircle") == wsName ) return 0x24BB;
	else if ( _T("Fdotaccent") == wsName ) return 0x1E1E;
	else if ( _T("Feharmenian") == wsName ) return 0x0556;
	else if ( _T("Feicoptic") == wsName ) return 0x03E4;
	else if ( _T("Fhook") == wsName ) return 0x0191;
	else if ( _T("Fitacyrillic") == wsName ) return 0x0472;
	else if ( _T("Fiveroman") == wsName ) return 0x2164;
	else if ( _T("Fmonospace") == wsName ) return 0xFF26;
	else if ( _T("Fourroman") == wsName ) return 0x2163;
	else if ( _T("Fsmall") == wsName ) return 0xF766;

	return 0;
}
static int Type1NameToUnicode_G(CWCharWrapper &wsName)
{
	if      ( _T("G") == wsName ) return 0x0047;
	else if ( _T("GBsquare") == wsName ) return 0x3387;
	else if ( _T("Gacute") == wsName ) return 0x01F4;
	else if ( _T("Gamma") == wsName ) return 0x0393;
	else if ( _T("Gammaafrican") == wsName ) return 0x0194;
	else if ( _T("Gangiacoptic") == wsName ) return 0x03EA;
	else if ( _T("Gbreve") == wsName ) return 0x011E;
	else if ( _T("Gcaron") == wsName ) return 0x01E6;
	else if ( _T("Gcedilla") == wsName ) return 0x0122;
	else if ( _T("Gcircle") == wsName ) return 0x24BC;
	else if ( _T("Gcircumflex") == wsName ) return 0x011C;
	else if ( _T("Gcommaaccent") == wsName ) return 0x0122;
	else if ( _T("Gdot") == wsName ) return 0x0120;
	else if ( _T("Gdotaccent") == wsName ) return 0x0120;
	else if ( _T("Gecyrillic") == wsName ) return 0x0413;
	else if ( _T("Ghadarmenian") == wsName ) return 0x0542;
	else if ( _T("Ghemiddlehookcyrillic") == wsName ) return 0x0494;
	else if ( _T("Ghestrokecyrillic") == wsName ) return 0x0492;
	else if ( _T("Gheupturncyrillic") == wsName ) return 0x0490;
	else if ( _T("Ghook") == wsName ) return 0x0193;
	else if ( _T("Gimarmenian") == wsName ) return 0x0533;
	else if ( _T("Gjecyrillic") == wsName ) return 0x0403;
	else if ( _T("Gmacron") == wsName ) return 0x1E20;
	else if ( _T("Gmonospace") == wsName ) return 0xFF27;
	else if ( _T("Grave") == wsName ) return 0xF6CE;
	else if ( _T("Gravesmall") == wsName ) return 0xF760;
	else if ( _T("Gsmall") == wsName ) return 0xF767;
	else if ( _T("Gsmallhook") == wsName ) return 0x029B;
	else if ( _T("Gstroke") == wsName ) return 0x01E4;

	return 0;
}
static int Type1NameToUnicode_H(CWCharWrapper &wsName)
{
	if      ( _T("H") == wsName ) return 0x0048;
	else if ( _T("H18533") == wsName ) return 0x25CF;
	else if ( _T("H18543") == wsName ) return 0x25AA;
	else if ( _T("H18551") == wsName ) return 0x25AB;
	else if ( _T("H22073") == wsName ) return 0x25A1;
	else if ( _T("HPsquare") == wsName ) return 0x33CB;
	else if ( _T("Haabkhasiancyrillic") == wsName ) return 0x04A8;
	else if ( _T("Hadescendercyrillic") == wsName ) return 0x04B2;
	else if ( _T("Hardsigncyrillic") == wsName ) return 0x042A;
	else if ( _T("Hbar") == wsName ) return 0x0126;
	else if ( _T("Hbrevebelow") == wsName ) return 0x1E2A;
	else if ( _T("Hcedilla") == wsName ) return 0x1E28;
	else if ( _T("Hcircle") == wsName ) return 0x24BD;
	else if ( _T("Hcircumflex") == wsName ) return 0x0124;
	else if ( _T("Hdieresis") == wsName ) return 0x1E26;
	else if ( _T("Hdotaccent") == wsName ) return 0x1E22;
	else if ( _T("Hdotbelow") == wsName ) return 0x1E24;
	else if ( _T("Hmonospace") == wsName ) return 0xFF28;
	else if ( _T("Hoarmenian") == wsName ) return 0x0540;
	else if ( _T("Horicoptic") == wsName ) return 0x03E8;
	else if ( _T("Hsmall") == wsName ) return 0xF768;
	else if ( _T("Hungarumlaut") == wsName ) return 0xF6CF;
	else if ( _T("Hungarumlautsmall") == wsName ) return 0xF6F8;
	else if ( _T("Hzsquare") == wsName ) return 0x3390;

	return 0;
}
static int Type1NameToUnicode_I(CWCharWrapper &wsName)
{
	if      ( _T("I") == wsName ) return 0x0049;
	else if ( _T("IAcyrillic") == wsName ) return 0x042F;
	else if ( _T("IJ") == wsName ) return 0x0132;
	else if ( _T("IUcyrillic") == wsName ) return 0x042E;
	else if ( _T("Iacute") == wsName ) return 0x00CD;
	else if ( _T("Iacutesmall") == wsName ) return 0xF7ED;
	else if ( _T("Ibreve") == wsName ) return 0x012C;
	else if ( _T("Icaron") == wsName ) return 0x01CF;
	else if ( _T("Icircle") == wsName ) return 0x24BE;
	else if ( _T("Icircumflex") == wsName ) return 0x00CE;
	else if ( _T("Icircumflexsmall") == wsName ) return 0xF7EE;
	else if ( _T("Icyrillic") == wsName ) return 0x0406;
	else if ( _T("Idblgrave") == wsName ) return 0x0208;
	else if ( _T("Idieresis") == wsName ) return 0x00CF;
	else if ( _T("Idieresisacute") == wsName ) return 0x1E2E;
	else if ( _T("Idieresiscyrillic") == wsName ) return 0x04E4;
	else if ( _T("Idieresissmall") == wsName ) return 0xF7EF;
	else if ( _T("Idot") == wsName ) return 0x0130;
	else if ( _T("Idotaccent") == wsName ) return 0x0130;
	else if ( _T("Idotbelow") == wsName ) return 0x1ECA;
	else if ( _T("Iebrevecyrillic") == wsName ) return 0x04D6;
	else if ( _T("Iecyrillic") == wsName ) return 0x0415;
	else if ( _T("Ifraktur") == wsName ) return 0x2111;
	else if ( _T("Igrave") == wsName ) return 0x00CC;
	else if ( _T("Igravesmall") == wsName ) return 0xF7EC;
	else if ( _T("Ihookabove") == wsName ) return 0x1EC8;
	else if ( _T("Iicyrillic") == wsName ) return 0x0418;
	else if ( _T("Iinvertedbreve") == wsName ) return 0x020A;
	else if ( _T("Iishortcyrillic") == wsName ) return 0x0419;
	else if ( _T("Imacron") == wsName ) return 0x012A;
	else if ( _T("Imacroncyrillic") == wsName ) return 0x04E2;
	else if ( _T("Imonospace") == wsName ) return 0xFF29;
	else if ( _T("Iniarmenian") == wsName ) return 0x053B;
	else if ( _T("Iocyrillic") == wsName ) return 0x0401;
	else if ( _T("Iogonek") == wsName ) return 0x012E;
	else if ( _T("Iota") == wsName ) return 0x0399;
	else if ( _T("Iotaafrican") == wsName ) return 0x0196;
	else if ( _T("Iotadieresis") == wsName ) return 0x03AA;
	else if ( _T("Iotatonos") == wsName ) return 0x038A;
	else if ( _T("Ismall") == wsName ) return 0xF769;
	else if ( _T("Istroke") == wsName ) return 0x0197;
	else if ( _T("Itilde") == wsName ) return 0x0128;
	else if ( _T("Itildebelow") == wsName ) return 0x1E2C;
	else if ( _T("Izhitsacyrillic") == wsName ) return 0x0474;
	else if ( _T("Izhitsadblgravecyrillic") == wsName ) return 0x0476;

	return 0;
}
static int Type1NameToUnicode_J(CWCharWrapper &wsName)
{
	if      ( _T("J") == wsName ) return 0x004A;
	else if ( _T("Jaarmenian") == wsName ) return 0x0541;
	else if ( _T("Jcircle") == wsName ) return 0x24BF;
	else if ( _T("Jcircumflex") == wsName ) return 0x0134;
	else if ( _T("Jecyrillic") == wsName ) return 0x0408;
	else if ( _T("Jheharmenian") == wsName ) return 0x054B;
	else if ( _T("Jmonospace") == wsName ) return 0xFF2A;
	else if ( _T("Jsmall") == wsName ) return 0xF76A;

	return 0;
}
static int Type1NameToUnicode_K(CWCharWrapper &wsName)
{
	if      ( _T("K") == wsName ) return 0x004B;
	else if ( _T("KBsquare") == wsName ) return 0x3385;
	else if ( _T("KKsquare") == wsName ) return 0x33CD;
	else if ( _T("Kabashkircyrillic") == wsName ) return 0x04A0;
	else if ( _T("Kacute") == wsName ) return 0x1E30;
	else if ( _T("Kacyrillic") == wsName ) return 0x041A;
	else if ( _T("Kadescendercyrillic") == wsName ) return 0x049A;
	else if ( _T("Kahookcyrillic") == wsName ) return 0x04C3;
	else if ( _T("Kappa") == wsName ) return 0x039A;
	else if ( _T("Kastrokecyrillic") == wsName ) return 0x049E;
	else if ( _T("Kaverticalstrokecyrillic") == wsName ) return 0x049C;
	else if ( _T("Kcaron") == wsName ) return 0x01E8;
	else if ( _T("Kcedilla") == wsName ) return 0x0136;
	else if ( _T("Kcircle") == wsName ) return 0x24C0;
	else if ( _T("Kcommaaccent") == wsName ) return 0x0136;
	else if ( _T("Kdotbelow") == wsName ) return 0x1E32;
	else if ( _T("Keharmenian") == wsName ) return 0x0554;
	else if ( _T("Kenarmenian") == wsName ) return 0x053F;
	else if ( _T("Khacyrillic") == wsName ) return 0x0425;
	else if ( _T("Kheicoptic") == wsName ) return 0x03E6;
	else if ( _T("Khook") == wsName ) return 0x0198;
	else if ( _T("Kjecyrillic") == wsName ) return 0x040C;
	else if ( _T("Klinebelow") == wsName ) return 0x1E34;
	else if ( _T("Kmonospace") == wsName ) return 0xFF2B;
	else if ( _T("Koppacyrillic") == wsName ) return 0x0480;
	else if ( _T("Koppagreek") == wsName ) return 0x03DE;
	else if ( _T("Ksicyrillic") == wsName ) return 0x046E;
	else if ( _T("Ksmall") == wsName ) return 0xF76B;

	return 0;
}
static int Type1NameToUnicode_L(CWCharWrapper &wsName)
{
	if      ( _T("L") == wsName ) return 0x004C;
	else if ( _T("LJ") == wsName ) return 0x01C7;
	else if ( _T("LL") == wsName ) return 0xF6BF;
	else if ( _T("Lacute") == wsName ) return 0x0139;
	else if ( _T("Lambda") == wsName ) return 0x039B;
	else if ( _T("Lcaron") == wsName ) return 0x013D;
	else if ( _T("Lcedilla") == wsName ) return 0x013B;
	else if ( _T("Lcircle") == wsName ) return 0x24C1;
	else if ( _T("Lcircumflexbelow") == wsName ) return 0x1E3C;
	else if ( _T("Lcommaaccent") == wsName ) return 0x013B;
	else if ( _T("Ldot") == wsName ) return 0x013F;
	else if ( _T("Ldotaccent") == wsName ) return 0x013F;
	else if ( _T("Ldotbelow") == wsName ) return 0x1E36;
	else if ( _T("Ldotbelowmacron") == wsName ) return 0x1E38;
	else if ( _T("Liwnarmenian") == wsName ) return 0x053C;
	else if ( _T("Lj") == wsName ) return 0x01C8;
	else if ( _T("Ljecyrillic") == wsName ) return 0x0409;
	else if ( _T("Llinebelow") == wsName ) return 0x1E3A;
	else if ( _T("Lmonospace") == wsName ) return 0xFF2C;
	else if ( _T("Lslash") == wsName ) return 0x0141;
	else if ( _T("Lslashsmall") == wsName ) return 0xF6F9;
	else if ( _T("Lsmall") == wsName ) return 0xF76C;

	return 0;
}
static int Type1NameToUnicode_M(CWCharWrapper &wsName)
{
	if      ( _T("M") == wsName ) return 0x004D;
	else if ( _T("MBsquare") == wsName ) return 0x3386;
	else if ( _T("Macron") == wsName ) return 0xF6D0;
	else if ( _T("Macronsmall") == wsName ) return 0xF7AF;
	else if ( _T("Macute") == wsName ) return 0x1E3E;
	else if ( _T("Mcircle") == wsName ) return 0x24C2;
	else if ( _T("Mdotaccent") == wsName ) return 0x1E40;
	else if ( _T("Mdotbelow") == wsName ) return 0x1E42;
	else if ( _T("Menarmenian") == wsName ) return 0x0544;
	else if ( _T("Mmonospace") == wsName ) return 0xFF2D;
	else if ( _T("Msmall") == wsName ) return 0xF76D;
	else if ( _T("Mturned") == wsName ) return 0x019C;
	else if ( _T("Mu") == wsName ) return 0x039C;

	return 0;
}
static int Type1NameToUnicode_N(CWCharWrapper &wsName)
{
	if      ( _T("N") == wsName ) return 0x004E;
	else if ( _T("NJ") == wsName ) return 0x01CA;
	else if ( _T("Nacute") == wsName ) return 0x0143;
	else if ( _T("Ncaron") == wsName ) return 0x0147;
	else if ( _T("Ncedilla") == wsName ) return 0x0145;
	else if ( _T("Ncircle") == wsName ) return 0x24C3;
	else if ( _T("Ncircumflexbelow") == wsName ) return 0x1E4A;
	else if ( _T("Ncommaaccent") == wsName ) return 0x0145;
	else if ( _T("Ndotaccent") == wsName ) return 0x1E44;
	else if ( _T("Ndotbelow") == wsName ) return 0x1E46;
	else if ( _T("Nhookleft") == wsName ) return 0x019D;
	else if ( _T("Nineroman") == wsName ) return 0x2168;
	else if ( _T("Nj") == wsName ) return 0x01CB;
	else if ( _T("Njecyrillic") == wsName ) return 0x040A;
	else if ( _T("Nlinebelow") == wsName ) return 0x1E48;
	else if ( _T("Nmonospace") == wsName ) return 0xFF2E;
	else if ( _T("Nowarmenian") == wsName ) return 0x0546;
	else if ( _T("Nsmall") == wsName ) return 0xF76E;
	else if ( _T("Ntilde") == wsName ) return 0x00D1;
	else if ( _T("Ntildesmall") == wsName ) return 0xF7F1;
	else if ( _T("Nu") == wsName ) return 0x039D;

	return 0;
}
static int Type1NameToUnicode_O(CWCharWrapper &wsName)
{
	if      ( _T("O") == wsName ) return 0x004F;
	else if ( _T("OE") == wsName ) return 0x0152;
	else if ( _T("OEsmall") == wsName ) return 0xF6FA;
	else if ( _T("Oacute") == wsName ) return 0x00D3;
	else if ( _T("Oacutesmall") == wsName ) return 0xF7F3;
	else if ( _T("Obarredcyrillic") == wsName ) return 0x04E8;
	else if ( _T("Obarreddieresiscyrillic") == wsName ) return 0x04EA;
	else if ( _T("Obreve") == wsName ) return 0x014E;
	else if ( _T("Ocaron") == wsName ) return 0x01D1;
	else if ( _T("Ocenteredtilde") == wsName ) return 0x019F;
	else if ( _T("Ocircle") == wsName ) return 0x24C4;
	else if ( _T("Ocircumflex") == wsName ) return 0x00D4;
	else if ( _T("Ocircumflexacute") == wsName ) return 0x1ED0;
	else if ( _T("Ocircumflexdotbelow") == wsName ) return 0x1ED8;
	else if ( _T("Ocircumflexgrave") == wsName ) return 0x1ED2;
	else if ( _T("Ocircumflexhookabove") == wsName ) return 0x1ED4;
	else if ( _T("Ocircumflexsmall") == wsName ) return 0xF7F4;
	else if ( _T("Ocircumflextilde") == wsName ) return 0x1ED6;
	else if ( _T("Ocyrillic") == wsName ) return 0x041E;
	else if ( _T("Odblacute") == wsName ) return 0x0150;
	else if ( _T("Odblgrave") == wsName ) return 0x020C;
	else if ( _T("Odieresis") == wsName ) return 0x00D6;
	else if ( _T("Odieresiscyrillic") == wsName ) return 0x04E6;
	else if ( _T("Odieresissmall") == wsName ) return 0xF7F6;
	else if ( _T("Odotbelow") == wsName ) return 0x1ECC;
	else if ( _T("Ogoneksmall") == wsName ) return 0xF6FB;
	else if ( _T("Ograve") == wsName ) return 0x00D2;
	else if ( _T("Ogravesmall") == wsName ) return 0xF7F2;
	else if ( _T("Oharmenian") == wsName ) return 0x0555;
	else if ( _T("Ohm") == wsName ) return 0x2126;
	else if ( _T("Ohookabove") == wsName ) return 0x1ECE;
	else if ( _T("Ohorn") == wsName ) return 0x01A0;
	else if ( _T("Ohornacute") == wsName ) return 0x1EDA;
	else if ( _T("Ohorndotbelow") == wsName ) return 0x1EE2;
	else if ( _T("Ohorngrave") == wsName ) return 0x1EDC;
	else if ( _T("Ohornhookabove") == wsName ) return 0x1EDE;
	else if ( _T("Ohorntilde") == wsName ) return 0x1EE0;
	else if ( _T("Ohungarumlaut") == wsName ) return 0x0150;
	else if ( _T("Oi") == wsName ) return 0x01A2;
	else if ( _T("Oinvertedbreve") == wsName ) return 0x020E;
	else if ( _T("Omacron") == wsName ) return 0x014C;
	else if ( _T("Omacronacute") == wsName ) return 0x1E52;
	else if ( _T("Omacrongrave") == wsName ) return 0x1E50;
	else if ( _T("Omega") == wsName ) return 0x2126;
	else if ( _T("Omegacyrillic") == wsName ) return 0x0460;
	else if ( _T("Omegagreek") == wsName ) return 0x03A9;
	else if ( _T("Omegaroundcyrillic") == wsName ) return 0x047A;
	else if ( _T("Omegatitlocyrillic") == wsName ) return 0x047C;
	else if ( _T("Omegatonos") == wsName ) return 0x038F;
	else if ( _T("Omicron") == wsName ) return 0x039F;
	else if ( _T("Omicrontonos") == wsName ) return 0x038C;
	else if ( _T("Omonospace") == wsName ) return 0xFF2F;
	else if ( _T("Oneroman") == wsName ) return 0x2160;
	else if ( _T("Oogonek") == wsName ) return 0x01EA;
	else if ( _T("Oogonekmacron") == wsName ) return 0x01EC;
	else if ( _T("Oopen") == wsName ) return 0x0186;
	else if ( _T("Oslash") == wsName ) return 0x00D8;
	else if ( _T("Oslashacute") == wsName ) return 0x01FE;
	else if ( _T("Oslashsmall") == wsName ) return 0xF7F8;
	else if ( _T("Osmall") == wsName ) return 0xF76F;
	else if ( _T("Ostrokeacute") == wsName ) return 0x01FE;
	else if ( _T("Otcyrillic") == wsName ) return 0x047E;
	else if ( _T("Otilde") == wsName ) return 0x00D5;
	else if ( _T("Otildeacute") == wsName ) return 0x1E4C;
	else if ( _T("Otildedieresis") == wsName ) return 0x1E4E;
	else if ( _T("Otildesmall") == wsName ) return 0xF7F5;

	return 0;
}
static int Type1NameToUnicode_P(CWCharWrapper &wsName)
{
	if      ( _T("P") == wsName ) return 0x0050;
	else if ( _T("Pacute") == wsName ) return 0x1E54;
	else if ( _T("Pcircle") == wsName ) return 0x24C5;
	else if ( _T("Pdotaccent") == wsName ) return 0x1E56;
	else if ( _T("Pecyrillic") == wsName ) return 0x041F;
	else if ( _T("Peharmenian") == wsName ) return 0x054A;
	else if ( _T("Pemiddlehookcyrillic") == wsName ) return 0x04A6;
	else if ( _T("Phi") == wsName ) return 0x03A6;
	else if ( _T("Phook") == wsName ) return 0x01A4;
	else if ( _T("Pi") == wsName ) return 0x03A0;
	else if ( _T("Piwrarmenian") == wsName ) return 0x0553;
	else if ( _T("Pmonospace") == wsName ) return 0xFF30;
	else if ( _T("Psi") == wsName ) return 0x03A8;
	else if ( _T("Psicyrillic") == wsName ) return 0x0470;
	else if ( _T("Psmall") == wsName ) return 0xF770;

	return 0;
}
static int Type1NameToUnicode_Q(CWCharWrapper &wsName)
{
	if      ( _T("Q") == wsName ) return 0x0051;
	else if ( _T("Qcircle") == wsName ) return 0x24C6;
	else if ( _T("Qmonospace") == wsName ) return 0xFF31;
	else if ( _T("Qsmall") == wsName ) return 0xF771;

	return 0;
}
static int Type1NameToUnicode_R(CWCharWrapper &wsName)
{
	if      ( _T("R") == wsName ) return 0x0052;
	else if ( _T("Raarmenian") == wsName ) return 0x054C;
	else if ( _T("Racute") == wsName ) return 0x0154;
	else if ( _T("Rcaron") == wsName ) return 0x0158;
	else if ( _T("Rcedilla") == wsName ) return 0x0156;
	else if ( _T("Rcircle") == wsName ) return 0x24C7;
	else if ( _T("Rcommaaccent") == wsName ) return 0x0156;
	else if ( _T("Rdblgrave") == wsName ) return 0x0210;
	else if ( _T("Rdotaccent") == wsName ) return 0x1E58;
	else if ( _T("Rdotbelow") == wsName ) return 0x1E5A;
	else if ( _T("Rdotbelowmacron") == wsName ) return 0x1E5C;
	else if ( _T("Reharmenian") == wsName ) return 0x0550;
	else if ( _T("Rfraktur") == wsName ) return 0x211C;
	else if ( _T("Rho") == wsName ) return 0x03A1;
	else if ( _T("Ringsmall") == wsName ) return 0xF6FC;
	else if ( _T("Rinvertedbreve") == wsName ) return 0x0212;
	else if ( _T("Rlinebelow") == wsName ) return 0x1E5E;
	else if ( _T("Rmonospace") == wsName ) return 0xFF32;
	else if ( _T("Rsmall") == wsName ) return 0xF772;
	else if ( _T("Rsmallinverted") == wsName ) return 0x0281;
	else if ( _T("Rsmallinvertedsuperior") == wsName ) return 0x02B6;

	return 0;
}
static int Type1NameToUnicode_S(CWCharWrapper &wsName)
{
	if      ( _T("S") == wsName ) return 0x0053;
	else if ( _T("SF010000") == wsName ) return 0x250C;
	else if ( _T("SF020000") == wsName ) return 0x2514;
	else if ( _T("SF030000") == wsName ) return 0x2510;
	else if ( _T("SF040000") == wsName ) return 0x2518;
	else if ( _T("SF050000") == wsName ) return 0x253C;
	else if ( _T("SF060000") == wsName ) return 0x252C;
	else if ( _T("SF070000") == wsName ) return 0x2534;
	else if ( _T("SF080000") == wsName ) return 0x251C;
	else if ( _T("SF090000") == wsName ) return 0x2524;
	else if ( _T("SF100000") == wsName ) return 0x2500;
	else if ( _T("SF110000") == wsName ) return 0x2502;
	else if ( _T("SF190000") == wsName ) return 0x2561;
	else if ( _T("SF200000") == wsName ) return 0x2562;
	else if ( _T("SF210000") == wsName ) return 0x2556;
	else if ( _T("SF220000") == wsName ) return 0x2555;
	else if ( _T("SF230000") == wsName ) return 0x2563;
	else if ( _T("SF240000") == wsName ) return 0x2551;
	else if ( _T("SF250000") == wsName ) return 0x2557;
	else if ( _T("SF260000") == wsName ) return 0x255D;
	else if ( _T("SF270000") == wsName ) return 0x255C;
	else if ( _T("SF280000") == wsName ) return 0x255B;
	else if ( _T("SF360000") == wsName ) return 0x255E;
	else if ( _T("SF370000") == wsName ) return 0x255F;
	else if ( _T("SF380000") == wsName ) return 0x255A;
	else if ( _T("SF390000") == wsName ) return 0x2554;
	else if ( _T("SF400000") == wsName ) return 0x2569;
	else if ( _T("SF410000") == wsName ) return 0x2566;
	else if ( _T("SF420000") == wsName ) return 0x2560;
	else if ( _T("SF430000") == wsName ) return 0x2550;
	else if ( _T("SF440000") == wsName ) return 0x256C;
	else if ( _T("SF450000") == wsName ) return 0x2567;
	else if ( _T("SF460000") == wsName ) return 0x2568;
	else if ( _T("SF470000") == wsName ) return 0x2564;
	else if ( _T("SF480000") == wsName ) return 0x2565;
	else if ( _T("SF490000") == wsName ) return 0x2559;
	else if ( _T("SF500000") == wsName ) return 0x2558;
	else if ( _T("SF510000") == wsName ) return 0x2552;
	else if ( _T("SF520000") == wsName ) return 0x2553;
	else if ( _T("SF530000") == wsName ) return 0x256B;
	else if ( _T("SF540000") == wsName ) return 0x256A;
	else if ( _T("Sacute") == wsName ) return 0x015A;
	else if ( _T("Sacutedotaccent") == wsName ) return 0x1E64;
	else if ( _T("Sampigreek") == wsName ) return 0x03E0;
	else if ( _T("Scaron") == wsName ) return 0x0160;
	else if ( _T("Scarondotaccent") == wsName ) return 0x1E66;
	else if ( _T("Scaronsmall") == wsName ) return 0xF6FD;
	else if ( _T("Scedilla") == wsName ) return 0x015E;
	else if ( _T("Schwa") == wsName ) return 0x018F;
	else if ( _T("Schwacyrillic") == wsName ) return 0x04D8;
	else if ( _T("Schwadieresiscyrillic") == wsName ) return 0x04DA;
	else if ( _T("Scircle") == wsName ) return 0x24C8;
	else if ( _T("Scircumflex") == wsName ) return 0x015C;
	else if ( _T("Scommaaccent") == wsName ) return 0x0218;
	else if ( _T("Sdotaccent") == wsName ) return 0x1E60;
	else if ( _T("Sdotbelow") == wsName ) return 0x1E62;
	else if ( _T("Sdotbelowdotaccent") == wsName ) return 0x1E68;
	else if ( _T("Seharmenian") == wsName ) return 0x054D;
	else if ( _T("Sevenroman") == wsName ) return 0x2166;
	else if ( _T("Shaarmenian") == wsName ) return 0x0547;
	else if ( _T("Shacyrillic") == wsName ) return 0x0428;
	else if ( _T("Shchacyrillic") == wsName ) return 0x0429;
	else if ( _T("Sheicoptic") == wsName ) return 0x03E2;
	else if ( _T("Shhacyrillic") == wsName ) return 0x04BA;
	else if ( _T("Shimacoptic") == wsName ) return 0x03EC;
	else if ( _T("Sigma") == wsName ) return 0x03A3;
	else if ( _T("Sixroman") == wsName ) return 0x2165;
	else if ( _T("Smonospace") == wsName ) return 0xFF33;
	else if ( _T("Softsigncyrillic") == wsName ) return 0x042C;
	else if ( _T("Ssmall") == wsName ) return 0xF773;
	else if ( _T("Stigmagreek") == wsName ) return 0x03DA;

	return 0;
}
static int Type1NameToUnicode_T(CWCharWrapper &wsName)
{
	if      ( _T("T") == wsName ) return 0x0054;
	else if ( _T("Tau") == wsName ) return 0x03A4;
	else if ( _T("Tbar") == wsName ) return 0x0166;
	else if ( _T("Tcaron") == wsName ) return 0x0164;
	else if ( _T("Tcedilla") == wsName ) return 0x0162;
	else if ( _T("Tcircle") == wsName ) return 0x24C9;
	else if ( _T("Tcircumflexbelow") == wsName ) return 0x1E70;
	else if ( _T("Tcommaaccent") == wsName ) return 0x0162;
	else if ( _T("Tdotaccent") == wsName ) return 0x1E6A;
	else if ( _T("Tdotbelow") == wsName ) return 0x1E6C;
	else if ( _T("Tecyrillic") == wsName ) return 0x0422;
	else if ( _T("Tedescendercyrillic") == wsName ) return 0x04AC;
	else if ( _T("Tenroman") == wsName ) return 0x2169;
	else if ( _T("Tetsecyrillic") == wsName ) return 0x04B4;
	else if ( _T("Theta") == wsName ) return 0x0398;
	else if ( _T("Thook") == wsName ) return 0x01AC;
	else if ( _T("Thorn") == wsName ) return 0x00DE;
	else if ( _T("Thornsmall") == wsName ) return 0xF7FE;
	else if ( _T("Threeroman") == wsName ) return 0x2162;
	else if ( _T("Tildesmall") == wsName ) return 0xF6FE;
	else if ( _T("Tiwnarmenian") == wsName ) return 0x054F;
	else if ( _T("Tlinebelow") == wsName ) return 0x1E6E;
	else if ( _T("Tmonospace") == wsName ) return 0xFF34;
	else if ( _T("Toarmenian") == wsName ) return 0x0539;
	else if ( _T("Tonefive") == wsName ) return 0x01BC;
	else if ( _T("Tonesix") == wsName ) return 0x0184;
	else if ( _T("Tonetwo") == wsName ) return 0x01A7;
	else if ( _T("Tretroflexhook") == wsName ) return 0x01AE;
	else if ( _T("Tsecyrillic") == wsName ) return 0x0426;
	else if ( _T("Tshecyrillic") == wsName ) return 0x040B;
	else if ( _T("Tsmall") == wsName ) return 0xF774;
	else if ( _T("Twelveroman") == wsName ) return 0x216B;
	else if ( _T("Tworoman") == wsName ) return 0x2161;

	return 0;
}
static int Type1NameToUnicode_U(CWCharWrapper &wsName)
{
	if      ( _T("U") == wsName ) return 0x0055;
	else if ( _T("Uacute") == wsName ) return 0x00DA;
	else if ( _T("Uacutesmall") == wsName ) return 0xF7FA;
	else if ( _T("Ubreve") == wsName ) return 0x016C;
	else if ( _T("Ucaron") == wsName ) return 0x01D3;
	else if ( _T("Ucircle") == wsName ) return 0x24CA;
	else if ( _T("Ucircumflex") == wsName ) return 0x00DB;
	else if ( _T("Ucircumflexbelow") == wsName ) return 0x1E76;
	else if ( _T("Ucircumflexsmall") == wsName ) return 0xF7FB;
	else if ( _T("Ucyrillic") == wsName ) return 0x0423;
	else if ( _T("Udblacute") == wsName ) return 0x0170;
	else if ( _T("Udblgrave") == wsName ) return 0x0214;
	else if ( _T("Udieresis") == wsName ) return 0x00DC;
	else if ( _T("Udieresisacute") == wsName ) return 0x01D7;
	else if ( _T("Udieresisbelow") == wsName ) return 0x1E72;
	else if ( _T("Udieresiscaron") == wsName ) return 0x01D9;
	else if ( _T("Udieresiscyrillic") == wsName ) return 0x04F0;
	else if ( _T("Udieresisgrave") == wsName ) return 0x01DB;
	else if ( _T("Udieresismacron") == wsName ) return 0x01D5;
	else if ( _T("Udieresissmall") == wsName ) return 0xF7FC;
	else if ( _T("Udotbelow") == wsName ) return 0x1EE4;
	else if ( _T("Ugrave") == wsName ) return 0x00D9;
	else if ( _T("Ugravesmall") == wsName ) return 0xF7F9;
	else if ( _T("Uhookabove") == wsName ) return 0x1EE6;
	else if ( _T("Uhorn") == wsName ) return 0x01AF;
	else if ( _T("Uhornacute") == wsName ) return 0x1EE8;
	else if ( _T("Uhorndotbelow") == wsName ) return 0x1EF0;
	else if ( _T("Uhorngrave") == wsName ) return 0x1EEA;
	else if ( _T("Uhornhookabove") == wsName ) return 0x1EEC;
	else if ( _T("Uhorntilde") == wsName ) return 0x1EEE;
	else if ( _T("Uhungarumlaut") == wsName ) return 0x0170;
	else if ( _T("Uhungarumlautcyrillic") == wsName ) return 0x04F2;
	else if ( _T("Uinvertedbreve") == wsName ) return 0x0216;
	else if ( _T("Ukcyrillic") == wsName ) return 0x0478;
	else if ( _T("Umacron") == wsName ) return 0x016A;
	else if ( _T("Umacroncyrillic") == wsName ) return 0x04EE;
	else if ( _T("Umacrondieresis") == wsName ) return 0x1E7A;
	else if ( _T("Umonospace") == wsName ) return 0xFF35;
	else if ( _T("Uogonek") == wsName ) return 0x0172;
	else if ( _T("Upsilon") == wsName ) return 0x03A5;
	else if ( _T("Upsilon1") == wsName ) return 0x03D2;
	else if ( _T("Upsilonacutehooksymbolgreek") == wsName ) return 0x03D3;
	else if ( _T("Upsilonafrican") == wsName ) return 0x01B1;
	else if ( _T("Upsilondieresis") == wsName ) return 0x03AB;
	else if ( _T("Upsilondieresishooksymbolgreek") == wsName ) return 0x03D4;
	else if ( _T("Upsilonhooksymbol") == wsName ) return 0x03D2;
	else if ( _T("Upsilontonos") == wsName ) return 0x038E;
	else if ( _T("Uring") == wsName ) return 0x016E;
	else if ( _T("Ushortcyrillic") == wsName ) return 0x040E;
	else if ( _T("Usmall") == wsName ) return 0xF775;
	else if ( _T("Ustraightcyrillic") == wsName ) return 0x04AE;
	else if ( _T("Ustraightstrokecyrillic") == wsName ) return 0x04B0;
	else if ( _T("Utilde") == wsName ) return 0x0168;
	else if ( _T("Utildeacute") == wsName ) return 0x1E78;
	else if ( _T("Utildebelow") == wsName ) return 0x1E74;

	return 0;
}
static int Type1NameToUnicode_V(CWCharWrapper &wsName)
{
	if      ( _T("V") == wsName ) return 0x0056;
	else if ( _T("Vcircle") == wsName ) return 0x24CB;
	else if ( _T("Vdotbelow") == wsName ) return 0x1E7E;
	else if ( _T("Vecyrillic") == wsName ) return 0x0412;
	else if ( _T("Vewarmenian") == wsName ) return 0x054E;
	else if ( _T("Vhook") == wsName ) return 0x01B2;
	else if ( _T("Vmonospace") == wsName ) return 0xFF36;
	else if ( _T("Voarmenian") == wsName ) return 0x0548;
	else if ( _T("Vsmall") == wsName ) return 0xF776;
	else if ( _T("Vtilde") == wsName ) return 0x1E7C;

	return 0;
}
static int Type1NameToUnicode_W(CWCharWrapper &wsName)
{
	if      ( _T("W") == wsName ) return 0x0057;
	else if ( _T("Wacute") == wsName ) return 0x1E82;
	else if ( _T("Wcircle") == wsName ) return 0x24CC;
	else if ( _T("Wcircumflex") == wsName ) return 0x0174;
	else if ( _T("Wdieresis") == wsName ) return 0x1E84;
	else if ( _T("Wdotaccent") == wsName ) return 0x1E86;
	else if ( _T("Wdotbelow") == wsName ) return 0x1E88;
	else if ( _T("Wgrave") == wsName ) return 0x1E80;
	else if ( _T("Wmonospace") == wsName ) return 0xFF37;
	else if ( _T("Wsmall") == wsName ) return 0xF777;

	return 0;
}
static int Type1NameToUnicode_X(CWCharWrapper &wsName)
{
	if      ( _T("X") == wsName ) return 0x0058;
	else if ( _T("Xcircle") == wsName ) return 0x24CD;
	else if ( _T("Xdieresis") == wsName ) return 0x1E8C;
	else if ( _T("Xdotaccent") == wsName ) return 0x1E8A;
	else if ( _T("Xeharmenian") == wsName ) return 0x053D;
	else if ( _T("Xi") == wsName ) return 0x039E;
	else if ( _T("Xmonospace") == wsName ) return 0xFF38;
	else if ( _T("Xsmall") == wsName ) return 0xF778;

	return 0;
}
static int Type1NameToUnicode_Y(CWCharWrapper &wsName)
{
	if      ( _T("Y") == wsName ) return 0x0059;
	else if ( _T("Yacute") == wsName ) return 0x00DD;
	else if ( _T("Yacutesmall") == wsName ) return 0xF7FD;
	else if ( _T("Yatcyrillic") == wsName ) return 0x0462;
	else if ( _T("Ycircle") == wsName ) return 0x24CE;
	else if ( _T("Ycircumflex") == wsName ) return 0x0176;
	else if ( _T("Ydieresis") == wsName ) return 0x0178;
	else if ( _T("Ydieresissmall") == wsName ) return 0xF7FF;
	else if ( _T("Ydotaccent") == wsName ) return 0x1E8E;
	else if ( _T("Ydotbelow") == wsName ) return 0x1EF4;
	else if ( _T("Yericyrillic") == wsName ) return 0x042B;
	else if ( _T("Yerudieresiscyrillic") == wsName ) return 0x04F8;
	else if ( _T("Ygrave") == wsName ) return 0x1EF2;
	else if ( _T("Yhook") == wsName ) return 0x01B3;
	else if ( _T("Yhookabove") == wsName ) return 0x1EF6;
	else if ( _T("Yiarmenian") == wsName ) return 0x0545;
	else if ( _T("Yicyrillic") == wsName ) return 0x0407;
	else if ( _T("Yiwnarmenian") == wsName ) return 0x0552;
	else if ( _T("Ymonospace") == wsName ) return 0xFF39;
	else if ( _T("Ysmall") == wsName ) return 0xF779;
	else if ( _T("Ytilde") == wsName ) return 0x1EF8;
	else if ( _T("Yusbigcyrillic") == wsName ) return 0x046A;
	else if ( _T("Yusbigiotifiedcyrillic") == wsName ) return 0x046C;
	else if ( _T("Yuslittlecyrillic") == wsName ) return 0x0466;
	else if ( _T("Yuslittleiotifiedcyrillic") == wsName ) return 0x0468;

	return 0;
}
static int Type1NameToUnicode_Z(CWCharWrapper &wsName)
{
	if      ( _T("Z") == wsName ) return 0x005A;
	else if ( _T("Zaarmenian") == wsName ) return 0x0536;
	else if ( _T("Zacute") == wsName ) return 0x0179;
	else if ( _T("Zcaron") == wsName ) return 0x017D;
	else if ( _T("Zcaronsmall") == wsName ) return 0xF6FF;
	else if ( _T("Zcircle") == wsName ) return 0x24CF;
	else if ( _T("Zcircumflex") == wsName ) return 0x1E90;
	else if ( _T("Zdot") == wsName ) return 0x017B;
	else if ( _T("Zdotaccent") == wsName ) return 0x017B;
	else if ( _T("Zdotbelow") == wsName ) return 0x1E92;
	else if ( _T("Zecyrillic") == wsName ) return 0x0417;
	else if ( _T("Zedescendercyrillic") == wsName ) return 0x0498;
	else if ( _T("Zedieresiscyrillic") == wsName ) return 0x04DE;
	else if ( _T("Zeta") == wsName ) return 0x0396;
	else if ( _T("Zhearmenian") == wsName ) return 0x053A;
	else if ( _T("Zhebrevecyrillic") == wsName ) return 0x04C1;
	else if ( _T("Zhecyrillic") == wsName ) return 0x0416;
	else if ( _T("Zhedescendercyrillic") == wsName ) return 0x0496;
	else if ( _T("Zhedieresiscyrillic") == wsName ) return 0x04DC;
	else if ( _T("Zlinebelow") == wsName ) return 0x1E94;
	else if ( _T("Zmonospace") == wsName ) return 0xFF3A;
	else if ( _T("Zsmall") == wsName ) return 0xF77A;
	else if ( _T("Zstroke") == wsName ) return 0x01B5;

	return 0;
}
static int Type1NameToUnicode_a(CWCharWrapper &wsName)
{
	wchar_t wChar1 = wsName[1];
	switch (wChar1)
	{
	case 0:
		{
			if      ( _T("a") == wsName ) return 0x0061;
			break;
		}
	case 'a':
		{
			if      ( _T("aabengali") == wsName ) return 0x0986;
			else if ( _T("aacute") == wsName ) return 0x00E1;
			else if ( _T("aadeva") == wsName ) return 0x0906;
			else if ( _T("aagujarati") == wsName ) return 0x0A86;
			else if ( _T("aagurmukhi") == wsName ) return 0x0A06;
			else if ( _T("aamatragurmukhi") == wsName ) return 0x0A3E;
			else if ( _T("aarusquare") == wsName ) return 0x3303;
			else if ( _T("aavowelsignbengali") == wsName ) return 0x09BE;
			else if ( _T("aavowelsigndeva") == wsName ) return 0x093E;
			else if ( _T("aavowelsigngujarati") == wsName ) return 0x0ABE;
			break;
		}
	case 'b':
		{
			if      ( _T("abbreviationmarkarmenian") == wsName ) return 0x055F;
			else if ( _T("abbreviationsigndeva") == wsName ) return 0x0970;
			else if ( _T("abengali") == wsName ) return 0x0985;
			else if ( _T("abopomofo") == wsName ) return 0x311A;
			else if ( _T("abreve") == wsName ) return 0x0103;
			else if ( _T("abreveacute") == wsName ) return 0x1EAF;
			else if ( _T("abrevecyrillic") == wsName ) return 0x04D1;
			else if ( _T("abrevedotbelow") == wsName ) return 0x1EB7;
			else if ( _T("abrevegrave") == wsName ) return 0x1EB1;
			else if ( _T("abrevehookabove") == wsName ) return 0x1EB3;
			else if ( _T("abrevetilde") == wsName ) return 0x1EB5;
			break;
		}
	case 'c':
		{
			if      ( _T("acaron") == wsName ) return 0x01CE;
			else if ( _T("acircle") == wsName ) return 0x24D0;
			else if ( _T("acircumflex") == wsName ) return 0x00E2;
			else if ( _T("acircumflexacute") == wsName ) return 0x1EA5;
			else if ( _T("acircumflexdotbelow") == wsName ) return 0x1EAD;
			else if ( _T("acircumflexgrave") == wsName ) return 0x1EA7;
			else if ( _T("acircumflexhookabove") == wsName ) return 0x1EA9;
			else if ( _T("acircumflextilde") == wsName ) return 0x1EAB;
			else if ( _T("acute") == wsName ) return 0x00B4;
			else if ( _T("acutebelowcmb") == wsName ) return 0x0317;
			else if ( _T("acutecmb") == wsName ) return 0x0301;
			else if ( _T("acutecomb") == wsName ) return 0x0301;
			else if ( _T("acutedeva") == wsName ) return 0x0954;
			else if ( _T("acutelowmod") == wsName ) return 0x02CF;
			else if ( _T("acutetonecmb") == wsName ) return 0x0341;
			else if ( _T("acyrillic") == wsName ) return 0x0430;
			break;
		}
	case 'd':
		{
			if      ( _T("adblgrave") == wsName ) return 0x0201;
			else if ( _T("addakgurmukhi") == wsName ) return 0x0A71;
			else if ( _T("adeva") == wsName ) return 0x0905;
			else if ( _T("adieresis") == wsName ) return 0x00E4;
			else if ( _T("adieresiscyrillic") == wsName ) return 0x04D3;
			else if ( _T("adieresismacron") == wsName ) return 0x01DF;
			else if ( _T("adotbelow") == wsName ) return 0x1EA1;
			else if ( _T("adotmacron") == wsName ) return 0x01E1;
			break;
		}
	case 'e':
		{
			if      ( _T("ae") == wsName ) return 0x00E6;
			else if ( _T("aeacute") == wsName ) return 0x01FD;
			else if ( _T("aekorean") == wsName ) return 0x3150;
			else if ( _T("aemacron") == wsName ) return 0x01E3;
			break;
		}
	case 'f':
		{
			wchar_t wChar4 = wsName[4];
			switch( wChar4 )
			{
			case '0':
				{
					if      ( _T("afii00208") == wsName ) return 0x2015;
					else if ( _T("afii08941") == wsName ) return 0x20A4;
					break;
				}
			case '1':
				{
					if      ( _T("afii10017") == wsName ) return 0x0410;
					else if ( _T("afii10018") == wsName ) return 0x0411;
					else if ( _T("afii10019") == wsName ) return 0x0412;
					else if ( _T("afii10020") == wsName ) return 0x0413;
					else if ( _T("afii10021") == wsName ) return 0x0414;
					else if ( _T("afii10022") == wsName ) return 0x0415;
					else if ( _T("afii10023") == wsName ) return 0x0401;
					else if ( _T("afii10024") == wsName ) return 0x0416;
					else if ( _T("afii10025") == wsName ) return 0x0417;
					else if ( _T("afii10026") == wsName ) return 0x0418;
					else if ( _T("afii10027") == wsName ) return 0x0419;
					else if ( _T("afii10028") == wsName ) return 0x041A;
					else if ( _T("afii10029") == wsName ) return 0x041B;
					else if ( _T("afii10030") == wsName ) return 0x041C;
					else if ( _T("afii10031") == wsName ) return 0x041D;
					else if ( _T("afii10032") == wsName ) return 0x041E;
					else if ( _T("afii10033") == wsName ) return 0x041F;
					else if ( _T("afii10034") == wsName ) return 0x0420;
					else if ( _T("afii10035") == wsName ) return 0x0421;
					else if ( _T("afii10036") == wsName ) return 0x0422;
					else if ( _T("afii10037") == wsName ) return 0x0423;
					else if ( _T("afii10038") == wsName ) return 0x0424;
					else if ( _T("afii10039") == wsName ) return 0x0425;
					else if ( _T("afii10040") == wsName ) return 0x0426;
					else if ( _T("afii10041") == wsName ) return 0x0427;
					else if ( _T("afii10042") == wsName ) return 0x0428;
					else if ( _T("afii10043") == wsName ) return 0x0429;
					else if ( _T("afii10044") == wsName ) return 0x042A;
					else if ( _T("afii10045") == wsName ) return 0x042B;
					else if ( _T("afii10046") == wsName ) return 0x042C;
					else if ( _T("afii10047") == wsName ) return 0x042D;
					else if ( _T("afii10048") == wsName ) return 0x042E;
					else if ( _T("afii10049") == wsName ) return 0x042F;
					else if ( _T("afii10050") == wsName ) return 0x0490;
					else if ( _T("afii10051") == wsName ) return 0x0402;
					else if ( _T("afii10052") == wsName ) return 0x0403;
					else if ( _T("afii10053") == wsName ) return 0x0404;
					else if ( _T("afii10054") == wsName ) return 0x0405;
					else if ( _T("afii10055") == wsName ) return 0x0406;
					else if ( _T("afii10056") == wsName ) return 0x0407;
					else if ( _T("afii10057") == wsName ) return 0x0408;
					else if ( _T("afii10058") == wsName ) return 0x0409;
					else if ( _T("afii10059") == wsName ) return 0x040A;
					else if ( _T("afii10060") == wsName ) return 0x040B;
					else if ( _T("afii10061") == wsName ) return 0x040C;
					else if ( _T("afii10062") == wsName ) return 0x040E;
					else if ( _T("afii10063") == wsName ) return 0xF6C4;
					else if ( _T("afii10064") == wsName ) return 0xF6C5;
					else if ( _T("afii10065") == wsName ) return 0x0430;
					else if ( _T("afii10066") == wsName ) return 0x0431;
					else if ( _T("afii10067") == wsName ) return 0x0432;
					else if ( _T("afii10068") == wsName ) return 0x0433;
					else if ( _T("afii10069") == wsName ) return 0x0434;
					else if ( _T("afii10070") == wsName ) return 0x0435;
					else if ( _T("afii10071") == wsName ) return 0x0451;
					else if ( _T("afii10072") == wsName ) return 0x0436;
					else if ( _T("afii10073") == wsName ) return 0x0437;
					else if ( _T("afii10074") == wsName ) return 0x0438;
					else if ( _T("afii10075") == wsName ) return 0x0439;
					else if ( _T("afii10076") == wsName ) return 0x043A;
					else if ( _T("afii10077") == wsName ) return 0x043B;
					else if ( _T("afii10078") == wsName ) return 0x043C;
					else if ( _T("afii10079") == wsName ) return 0x043D;
					else if ( _T("afii10080") == wsName ) return 0x043E;
					else if ( _T("afii10081") == wsName ) return 0x043F;
					else if ( _T("afii10082") == wsName ) return 0x0440;
					else if ( _T("afii10083") == wsName ) return 0x0441;
					else if ( _T("afii10084") == wsName ) return 0x0442;
					else if ( _T("afii10085") == wsName ) return 0x0443;
					else if ( _T("afii10086") == wsName ) return 0x0444;
					else if ( _T("afii10087") == wsName ) return 0x0445;
					else if ( _T("afii10088") == wsName ) return 0x0446;
					else if ( _T("afii10089") == wsName ) return 0x0447;
					else if ( _T("afii10090") == wsName ) return 0x0448;
					else if ( _T("afii10091") == wsName ) return 0x0449;
					else if ( _T("afii10092") == wsName ) return 0x044A;
					else if ( _T("afii10093") == wsName ) return 0x044B;
					else if ( _T("afii10094") == wsName ) return 0x044C;
					else if ( _T("afii10095") == wsName ) return 0x044D;
					else if ( _T("afii10096") == wsName ) return 0x044E;
					else if ( _T("afii10097") == wsName ) return 0x044F;
					else if ( _T("afii10098") == wsName ) return 0x0491;
					else if ( _T("afii10099") == wsName ) return 0x0452;
					else if ( _T("afii10100") == wsName ) return 0x0453;
					else if ( _T("afii10101") == wsName ) return 0x0454;
					else if ( _T("afii10102") == wsName ) return 0x0455;
					else if ( _T("afii10103") == wsName ) return 0x0456;
					else if ( _T("afii10104") == wsName ) return 0x0457;
					else if ( _T("afii10105") == wsName ) return 0x0458;
					else if ( _T("afii10106") == wsName ) return 0x0459;
					else if ( _T("afii10107") == wsName ) return 0x045A;
					else if ( _T("afii10108") == wsName ) return 0x045B;
					else if ( _T("afii10109") == wsName ) return 0x045C;
					else if ( _T("afii10110") == wsName ) return 0x045E;
					else if ( _T("afii10145") == wsName ) return 0x040F;
					else if ( _T("afii10146") == wsName ) return 0x0462;
					else if ( _T("afii10147") == wsName ) return 0x0472;
					else if ( _T("afii10148") == wsName ) return 0x0474;
					else if ( _T("afii10192") == wsName ) return 0xF6C6;
					else if ( _T("afii10193") == wsName ) return 0x045F;
					else if ( _T("afii10194") == wsName ) return 0x0463;
					else if ( _T("afii10195") == wsName ) return 0x0473;
					else if ( _T("afii10196") == wsName ) return 0x0475;
					else if ( _T("afii10831") == wsName ) return 0xF6C7;
					else if ( _T("afii10832") == wsName ) return 0xF6C8;
					else if ( _T("afii10846") == wsName ) return 0x04D9;
					break;
				}
			case '2':
				{
					if      ( _T("afii299") == wsName ) return 0x200E;
					break;
				}
			case '3':
				{
					if      ( _T("afii300") == wsName ) return 0x200F;
					else if ( _T("afii301") == wsName ) return 0x200D;
					break;
				}
			case '5':
				{
					wchar_t wChar6 = wsName[6];
					switch ( wChar6 )
					{
					case '3':
						{
							if      ( _T("afii57381") == wsName ) return 0x066A;
							else if ( _T("afii57388") == wsName ) return 0x060C;
							else if ( _T("afii57392") == wsName ) return 0x0660;
							else if ( _T("afii57393") == wsName ) return 0x0661;
							else if ( _T("afii57394") == wsName ) return 0x0662;
							else if ( _T("afii57395") == wsName ) return 0x0663;
							else if ( _T("afii57396") == wsName ) return 0x0664;
							else if ( _T("afii57397") == wsName ) return 0x0665;
							else if ( _T("afii57398") == wsName ) return 0x0666;
							else if ( _T("afii57399") == wsName ) return 0x0667;
							break;
						}
					case '4':
						{
							if      ( _T("afii57400") == wsName ) return 0x0668;
							else if ( _T("afii57401") == wsName ) return 0x0669;
							else if ( _T("afii57403") == wsName ) return 0x061B;
							else if ( _T("afii57407") == wsName ) return 0x061F;
							else if ( _T("afii57409") == wsName ) return 0x0621;
							else if ( _T("afii57410") == wsName ) return 0x0622;
							else if ( _T("afii57411") == wsName ) return 0x0623;
							else if ( _T("afii57412") == wsName ) return 0x0624;
							else if ( _T("afii57413") == wsName ) return 0x0625;
							else if ( _T("afii57414") == wsName ) return 0x0626;
							else if ( _T("afii57415") == wsName ) return 0x0627;
							else if ( _T("afii57416") == wsName ) return 0x0628;
							else if ( _T("afii57417") == wsName ) return 0x0629;
							else if ( _T("afii57418") == wsName ) return 0x062A;
							else if ( _T("afii57419") == wsName ) return 0x062B;
							else if ( _T("afii57420") == wsName ) return 0x062C;
							else if ( _T("afii57421") == wsName ) return 0x062D;
							else if ( _T("afii57422") == wsName ) return 0x062E;
							else if ( _T("afii57423") == wsName ) return 0x062F;
							else if ( _T("afii57424") == wsName ) return 0x0630;
							else if ( _T("afii57425") == wsName ) return 0x0631;
							else if ( _T("afii57426") == wsName ) return 0x0632;
							else if ( _T("afii57427") == wsName ) return 0x0633;
							else if ( _T("afii57428") == wsName ) return 0x0634;
							else if ( _T("afii57429") == wsName ) return 0x0635;
							else if ( _T("afii57430") == wsName ) return 0x0636;
							else if ( _T("afii57431") == wsName ) return 0x0637;
							else if ( _T("afii57432") == wsName ) return 0x0638;
							else if ( _T("afii57433") == wsName ) return 0x0639;
							else if ( _T("afii57434") == wsName ) return 0x063A;
							else if ( _T("afii57440") == wsName ) return 0x0640;
							else if ( _T("afii57441") == wsName ) return 0x0641;
							else if ( _T("afii57442") == wsName ) return 0x0642;
							else if ( _T("afii57443") == wsName ) return 0x0643;
							else if ( _T("afii57444") == wsName ) return 0x0644;
							else if ( _T("afii57445") == wsName ) return 0x0645;
							else if ( _T("afii57446") == wsName ) return 0x0646;
							else if ( _T("afii57448") == wsName ) return 0x0648;
							else if ( _T("afii57449") == wsName ) return 0x0649;
							else if ( _T("afii57450") == wsName ) return 0x064A;
							else if ( _T("afii57451") == wsName ) return 0x064B;
							else if ( _T("afii57452") == wsName ) return 0x064C;
							else if ( _T("afii57453") == wsName ) return 0x064D;
							else if ( _T("afii57454") == wsName ) return 0x064E;
							else if ( _T("afii57455") == wsName ) return 0x064F;
							else if ( _T("afii57456") == wsName ) return 0x0650;
							else if ( _T("afii57457") == wsName ) return 0x0651;
							else if ( _T("afii57458") == wsName ) return 0x0652;
							else if ( _T("afii57470") == wsName ) return 0x0647;
							break;
						}
					case '5':
						{
							if      ( _T("afii57505") == wsName ) return 0x06A4;
							else if ( _T("afii57506") == wsName ) return 0x067E;
							else if ( _T("afii57507") == wsName ) return 0x0686;
							else if ( _T("afii57508") == wsName ) return 0x0698;
							else if ( _T("afii57509") == wsName ) return 0x06AF;
							else if ( _T("afii57511") == wsName ) return 0x0679;
							else if ( _T("afii57512") == wsName ) return 0x0688;
							else if ( _T("afii57513") == wsName ) return 0x0691;
							else if ( _T("afii57514") == wsName ) return 0x06BA;
							else if ( _T("afii57519") == wsName ) return 0x06D2;
							else if ( _T("afii57534") == wsName ) return 0x06D5;
							break;
						}
					case '6':
						{
							if      ( _T("afii57636") == wsName ) return 0x20AA;
							else if ( _T("afii57645") == wsName ) return 0x05BE;
							else if ( _T("afii57658") == wsName ) return 0x05C3;
							else if ( _T("afii57664") == wsName ) return 0x05D0;
							else if ( _T("afii57665") == wsName ) return 0x05D1;
							else if ( _T("afii57666") == wsName ) return 0x05D2;
							else if ( _T("afii57667") == wsName ) return 0x05D3;
							else if ( _T("afii57668") == wsName ) return 0x05D4;
							else if ( _T("afii57669") == wsName ) return 0x05D5;
							else if ( _T("afii57670") == wsName ) return 0x05D6;
							else if ( _T("afii57671") == wsName ) return 0x05D7;
							else if ( _T("afii57672") == wsName ) return 0x05D8;
							else if ( _T("afii57673") == wsName ) return 0x05D9;
							else if ( _T("afii57674") == wsName ) return 0x05DA;
							else if ( _T("afii57675") == wsName ) return 0x05DB;
							else if ( _T("afii57676") == wsName ) return 0x05DC;
							else if ( _T("afii57677") == wsName ) return 0x05DD;
							else if ( _T("afii57678") == wsName ) return 0x05DE;
							else if ( _T("afii57679") == wsName ) return 0x05DF;
							else if ( _T("afii57680") == wsName ) return 0x05E0;
							else if ( _T("afii57681") == wsName ) return 0x05E1;
							else if ( _T("afii57682") == wsName ) return 0x05E2;
							else if ( _T("afii57683") == wsName ) return 0x05E3;
							else if ( _T("afii57684") == wsName ) return 0x05E4;
							else if ( _T("afii57685") == wsName ) return 0x05E5;
							else if ( _T("afii57686") == wsName ) return 0x05E6;
							else if ( _T("afii57687") == wsName ) return 0x05E7;
							else if ( _T("afii57688") == wsName ) return 0x05E8;
							else if ( _T("afii57689") == wsName ) return 0x05E9;
							else if ( _T("afii57690") == wsName ) return 0x05EA;
							else if ( _T("afii57694") == wsName ) return 0xFB2A;
							else if ( _T("afii57695") == wsName ) return 0xFB2B;
							break;
						}
					case '7':
						{
							if      ( _T("afii57700") == wsName ) return 0xFB4B;
							else if ( _T("afii57705") == wsName ) return 0xFB1F;
							else if ( _T("afii57716") == wsName ) return 0x05F0;
							else if ( _T("afii57717") == wsName ) return 0x05F1;
							else if ( _T("afii57718") == wsName ) return 0x05F2;
							else if ( _T("afii57723") == wsName ) return 0xFB35;
							else if ( _T("afii57793") == wsName ) return 0x05B4;
							else if ( _T("afii57794") == wsName ) return 0x05B5;
							else if ( _T("afii57795") == wsName ) return 0x05B6;
							else if ( _T("afii57796") == wsName ) return 0x05BB;
							else if ( _T("afii57797") == wsName ) return 0x05B8;
							else if ( _T("afii57798") == wsName ) return 0x05B7;
							else if ( _T("afii57799") == wsName ) return 0x05B0;
							break;
						}
					case '8':
						{
							if      ( _T("afii57800") == wsName ) return 0x05B2;
							else if ( _T("afii57801") == wsName ) return 0x05B1;
							else if ( _T("afii57802") == wsName ) return 0x05B3;
							else if ( _T("afii57803") == wsName ) return 0x05C2;
							else if ( _T("afii57804") == wsName ) return 0x05C1;
							else if ( _T("afii57806") == wsName ) return 0x05B9;
							else if ( _T("afii57807") == wsName ) return 0x05BC;
							else if ( _T("afii57839") == wsName ) return 0x05BD;
							else if ( _T("afii57841") == wsName ) return 0x05BF;
							else if ( _T("afii57842") == wsName ) return 0x05C0;
							break;
						}
					case '9':
						{
							if      ( _T("afii57929") == wsName ) return 0x02BC;
							break;
						}
					}
					break;
				}
			case '6':
				{
					if      ( _T("afii61248") == wsName ) return 0x2105;
					else if ( _T("afii61289") == wsName ) return 0x2113;
					else if ( _T("afii61352") == wsName ) return 0x2116;
					else if ( _T("afii61573") == wsName ) return 0x202C;
					else if ( _T("afii61574") == wsName ) return 0x202D;
					else if ( _T("afii61575") == wsName ) return 0x202E;
					else if ( _T("afii61664") == wsName ) return 0x200C;
					else if ( _T("afii63167") == wsName ) return 0x066D;
					else if ( _T("afii64937") == wsName ) return 0x02BD;
					break;
				}
			}
			break;
		}
	case 'g':
		{
			if      ( _T("agrave") == wsName ) return 0x00E0;
			else if ( _T("agujarati") == wsName ) return 0x0A85;
			else if ( _T("agurmukhi") == wsName ) return 0x0A05;
			break;
		}
	case 'h':
		{
			if      ( _T("ahiragana") == wsName ) return 0x3042;
			else if ( _T("ahookabove") == wsName ) return 0x1EA3;
			break;
		}
	case 'i':
		{
			if      ( _T("aibengali") == wsName ) return 0x0990;
			else if ( _T("aibopomofo") == wsName ) return 0x311E;
			else if ( _T("aideva") == wsName ) return 0x0910;
			else if ( _T("aiecyrillic") == wsName ) return 0x04D5;
			else if ( _T("aigujarati") == wsName ) return 0x0A90;
			else if ( _T("aigurmukhi") == wsName ) return 0x0A10;
			else if ( _T("aimatragurmukhi") == wsName ) return 0x0A48;
			else if ( _T("ainarabic") == wsName ) return 0x0639;
			else if ( _T("ainfinalarabic") == wsName ) return 0xFECA;
			else if ( _T("aininitialarabic") == wsName ) return 0xFECB;
			else if ( _T("ainmedialarabic") == wsName ) return 0xFECC;
			else if ( _T("ainvertedbreve") == wsName ) return 0x0203;
			else if ( _T("aivowelsignbengali") == wsName ) return 0x09C8;
			else if ( _T("aivowelsigndeva") == wsName ) return 0x0948;
			else if ( _T("aivowelsigngujarati") == wsName ) return 0x0AC8;
			break;
		}
	case 'k':
		{
			if      ( _T("akatakana") == wsName ) return 0x30A2;
			else if ( _T("akatakanahalfwidth") == wsName ) return 0xFF71;
			else if ( _T("akorean") == wsName ) return 0x314F;
			break;
		}
	case 'l':
		{
			if      ( _T("alef") == wsName ) return 0x05D0;
			else if ( _T("alefarabic") == wsName ) return 0x0627;
			else if ( _T("alefdageshhebrew") == wsName ) return 0xFB30;
			else if ( _T("aleffinalarabic") == wsName ) return 0xFE8E;
			else if ( _T("alefhamzaabovearabic") == wsName ) return 0x0623;
			else if ( _T("alefhamzaabovefinalarabic") == wsName ) return 0xFE84;
			else if ( _T("alefhamzabelowarabic") == wsName ) return 0x0625;
			else if ( _T("alefhamzabelowfinalarabic") == wsName ) return 0xFE88;
			else if ( _T("alefhebrew") == wsName ) return 0x05D0;
			else if ( _T("aleflamedhebrew") == wsName ) return 0xFB4F;
			else if ( _T("alefmaddaabovearabic") == wsName ) return 0x0622;
			else if ( _T("alefmaddaabovefinalarabic") == wsName ) return 0xFE82;
			else if ( _T("alefmaksuraarabic") == wsName ) return 0x0649;
			else if ( _T("alefmaksurafinalarabic") == wsName ) return 0xFEF0;
			else if ( _T("alefmaksurainitialarabic") == wsName ) return 0xFEF3;
			else if ( _T("alefmaksuramedialarabic") == wsName ) return 0xFEF4;
			else if ( _T("alefpatahhebrew") == wsName ) return 0xFB2E;
			else if ( _T("alefqamatshebrew") == wsName ) return 0xFB2F;
			else if ( _T("aleph") == wsName ) return 0x2135;
			else if ( _T("allequal") == wsName ) return 0x224C;
			else if ( _T("alpha") == wsName ) return 0x03B1;
			else if ( _T("alphatonos") == wsName ) return 0x03AC;
			break;
		}
	case 'm':
		{
			if      ( _T("amacron") == wsName ) return 0x0101;
			else if ( _T("amonospace") == wsName ) return 0xFF41;
			else if ( _T("ampersand") == wsName ) return 0x0026;
			else if ( _T("ampersandmonospace") == wsName ) return 0xFF06;
			else if ( _T("ampersandsmall") == wsName ) return 0xF726;
			else if ( _T("amsquare") == wsName ) return 0x33C2;
			break;
		}
	case 'n':
		{
			if      ( _T("anbopomofo") == wsName ) return 0x3122;
			else if ( _T("angbopomofo") == wsName ) return 0x3124;
			else if ( _T("angbracketleft") == wsName ) return 0x3008;
			else if ( _T("angbracketright") == wsName ) return 0x3009;
			else if ( _T("angkhankhuthai") == wsName ) return 0x0E5A;
			else if ( _T("angle") == wsName ) return 0x2220;
			else if ( _T("anglebracketleft") == wsName ) return 0x3008;
			else if ( _T("anglebracketleftvertical") == wsName ) return 0xFE3F;
			else if ( _T("anglebracketright") == wsName ) return 0x3009;
			else if ( _T("anglebracketrightvertical") == wsName ) return 0xFE40;
			else if ( _T("angleleft") == wsName ) return 0x2329;
			else if ( _T("angleright") == wsName ) return 0x232A;
			else if ( _T("angstrom") == wsName ) return 0x212B;
			else if ( _T("anoteleia") == wsName ) return 0x0387;
			else if ( _T("anudattadeva") == wsName ) return 0x0952;
			else if ( _T("anusvarabengali") == wsName ) return 0x0982;
			else if ( _T("anusvaradeva") == wsName ) return 0x0902;
			else if ( _T("anusvaragujarati") == wsName ) return 0x0A82;
			break;
		}
	case 'o':
		{
			if      ( _T("aogonek") == wsName ) return 0x0105;
			break;
		}
	case 'p':
		{
			if      ( _T("apaatosquare") == wsName ) return 0x3300;
			else if ( _T("aparen") == wsName ) return 0x249C;
			else if ( _T("apostrophearmenian") == wsName ) return 0x055A;
			else if ( _T("apostrophemod") == wsName ) return 0x02BC;
			else if ( _T("apple") == wsName ) return 0xF8FF;
			else if ( _T("approaches") == wsName ) return 0x2250;
			else if ( _T("approxequal") == wsName ) return 0x2248;
			else if ( _T("approxequalorimage") == wsName ) return 0x2252;
			else if ( _T("approximatelyequal") == wsName ) return 0x2245;
			break;
		}
	case 'r':
		{
			if      ( _T("araeaekorean") == wsName ) return 0x318E;
			else if ( _T("araeakorean") == wsName ) return 0x318D;
			else if ( _T("arc") == wsName ) return 0x2312;
			else if ( _T("arighthalfring") == wsName ) return 0x1E9A;
			else if ( _T("aring") == wsName ) return 0x00E5;
			else if ( _T("aringacute") == wsName ) return 0x01FB;
			else if ( _T("aringbelow") == wsName ) return 0x1E01;
			else if ( _T("arrowboth") == wsName ) return 0x2194;
			else if ( _T("arrowdashdown") == wsName ) return 0x21E3;
			else if ( _T("arrowdashleft") == wsName ) return 0x21E0;
			else if ( _T("arrowdashright") == wsName ) return 0x21E2;
			else if ( _T("arrowdashup") == wsName ) return 0x21E1;
			else if ( _T("arrowdblboth") == wsName ) return 0x21D4;
			else if ( _T("arrowdbldown") == wsName ) return 0x21D3;
			else if ( _T("arrowdblleft") == wsName ) return 0x21D0;
			else if ( _T("arrowdblright") == wsName ) return 0x21D2;
			else if ( _T("arrowdblup") == wsName ) return 0x21D1;
			else if ( _T("arrowdown") == wsName ) return 0x2193;
			else if ( _T("arrowdownleft") == wsName ) return 0x2199;
			else if ( _T("arrowdownright") == wsName ) return 0x2198;
			else if ( _T("arrowdownwhite") == wsName ) return 0x21E9;
			else if ( _T("arrowheaddownmod") == wsName ) return 0x02C5;
			else if ( _T("arrowheadleftmod") == wsName ) return 0x02C2;
			else if ( _T("arrowheadrightmod") == wsName ) return 0x02C3;
			else if ( _T("arrowheadupmod") == wsName ) return 0x02C4;
			else if ( _T("arrowhorizex") == wsName ) return 0xF8E7;
			else if ( _T("arrowleft") == wsName ) return 0x2190;
			else if ( _T("arrowleftdbl") == wsName ) return 0x21D0;
			else if ( _T("arrowleftdblstroke") == wsName ) return 0x21CD;
			else if ( _T("arrowleftoverright") == wsName ) return 0x21C6;
			else if ( _T("arrowleftwhite") == wsName ) return 0x21E6;
			else if ( _T("arrowright") == wsName ) return 0x2192;
			else if ( _T("arrowrightdblstroke") == wsName ) return 0x21CF;
			else if ( _T("arrowrightheavy") == wsName ) return 0x279E;
			else if ( _T("arrowrightoverleft") == wsName ) return 0x21C4;
			else if ( _T("arrowrightwhite") == wsName ) return 0x21E8;
			else if ( _T("arrowtableft") == wsName ) return 0x21E4;
			else if ( _T("arrowtabright") == wsName ) return 0x21E5;
			else if ( _T("arrowup") == wsName ) return 0x2191;
			else if ( _T("arrowupdn") == wsName ) return 0x2195;
			else if ( _T("arrowupdnbse") == wsName ) return 0x21A8;
			else if ( _T("arrowupdownbase") == wsName ) return 0x21A8;
			else if ( _T("arrowupleft") == wsName ) return 0x2196;
			else if ( _T("arrowupleftofdown") == wsName ) return 0x21C5;
			else if ( _T("arrowupright") == wsName ) return 0x2197;
			else if ( _T("arrowupwhite") == wsName ) return 0x21E7;
			else if ( _T("arrowvertex") == wsName ) return 0xF8E6;
			break;
		}
	case 's':
		{
			if      ( _T("asciicircum") == wsName ) return 0x005E;
			else if ( _T("asciicircummonospace") == wsName ) return 0xFF3E;
			else if ( _T("asciitilde") == wsName ) return 0x007E;
			else if ( _T("asciitildemonospace") == wsName ) return 0xFF5E;
			else if ( _T("ascript") == wsName ) return 0x0251;
			else if ( _T("ascriptturned") == wsName ) return 0x0252;
			else if ( _T("asmallhiragana") == wsName ) return 0x3041;
			else if ( _T("asmallkatakana") == wsName ) return 0x30A1;
			else if ( _T("asmallkatakanahalfwidth") == wsName ) return 0xFF67;
			else if ( _T("asterisk") == wsName ) return 0x002A;
			else if ( _T("asteriskaltonearabic") == wsName ) return 0x066D;
			else if ( _T("asteriskarabic") == wsName ) return 0x066D;
			else if ( _T("asteriskmath") == wsName ) return 0x2217;
			else if ( _T("asteriskmonospace") == wsName ) return 0xFF0A;
			else if ( _T("asterisksmall") == wsName ) return 0xFE61;
			else if ( _T("asterism") == wsName ) return 0x2042;
			else if ( _T("asuperior") == wsName ) return 0xF6E9;
			else if ( _T("asymptoticallyequal") == wsName ) return 0x2243;
			break;
		}
	case 't':
		{
			if      ( _T("at") == wsName ) return 0x0040;
			else if ( _T("atilde") == wsName ) return 0x00E3;
			else if ( _T("atmonospace") == wsName ) return 0xFF20;
			else if ( _T("atsmall") == wsName ) return 0xFE6B;
			else if ( _T("aturned") == wsName ) return 0x0250;
			break;
		}
	case 'u':
		{
			if      ( _T("aubengali") == wsName ) return 0x0994;
			else if ( _T("aubopomofo") == wsName ) return 0x3120;
			else if ( _T("audeva") == wsName ) return 0x0914;
			else if ( _T("augujarati") == wsName ) return 0x0A94;
			else if ( _T("augurmukhi") == wsName ) return 0x0A14;
			else if ( _T("aulengthmarkbengali") == wsName ) return 0x09D7;
			else if ( _T("aumatragurmukhi") == wsName ) return 0x0A4C;
			else if ( _T("auvowelsignbengali") == wsName ) return 0x09CC;
			else if ( _T("auvowelsigndeva") == wsName ) return 0x094C;
			else if ( _T("auvowelsigngujarati") == wsName ) return 0x0ACC;
			break;
		}
	case 'v':
		{
			if      ( _T("avagrahadeva") == wsName ) return 0x093D;
			break;
		}
	case 'y':
		{
			if      ( _T("aybarmenian") == wsName ) return 0x0561;
			else if ( _T("ayin") == wsName ) return 0x05E2;
			else if ( _T("ayinaltonehebrew") == wsName ) return 0xFB20;
			else if ( _T("ayinhebrew") == wsName ) return 0x05E2;
			break;
		}
	}

	return 0;
}
static int Type1NameToUnicode_b(CWCharWrapper &wsName)
{
	if      ( _T("b") == wsName ) return 0x0062;
	else if ( _T("babengali") == wsName ) return 0x09AC;
	else if ( _T("backslash") == wsName ) return 0x005C;
	else if ( _T("backslashmonospace") == wsName ) return 0xFF3C;
	else if ( _T("badeva") == wsName ) return 0x092C;
	else if ( _T("bagujarati") == wsName ) return 0x0AAC;
	else if ( _T("bagurmukhi") == wsName ) return 0x0A2C;
	else if ( _T("bahiragana") == wsName ) return 0x3070;
	else if ( _T("bahtthai") == wsName ) return 0x0E3F;
	else if ( _T("bakatakana") == wsName ) return 0x30D0;
	else if ( _T("bar") == wsName ) return 0x007C;
	else if ( _T("barmonospace") == wsName ) return 0xFF5C;
	else if ( _T("bbopomofo") == wsName ) return 0x3105;
	else if ( _T("bcircle") == wsName ) return 0x24D1;
	else if ( _T("bdotaccent") == wsName ) return 0x1E03;
	else if ( _T("bdotbelow") == wsName ) return 0x1E05;
	else if ( _T("beamedsixteenthnotes") == wsName ) return 0x266C;
	else if ( _T("because") == wsName ) return 0x2235;
	else if ( _T("becyrillic") == wsName ) return 0x0431;
	else if ( _T("beharabic") == wsName ) return 0x0628;
	else if ( _T("behfinalarabic") == wsName ) return 0xFE90;
	else if ( _T("behinitialarabic") == wsName ) return 0xFE91;
	else if ( _T("behiragana") == wsName ) return 0x3079;
	else if ( _T("behmedialarabic") == wsName ) return 0xFE92;
	else if ( _T("behmeeminitialarabic") == wsName ) return 0xFC9F;
	else if ( _T("behmeemisolatedarabic") == wsName ) return 0xFC08;
	else if ( _T("behnoonfinalarabic") == wsName ) return 0xFC6D;
	else if ( _T("bekatakana") == wsName ) return 0x30D9;
	else if ( _T("benarmenian") == wsName ) return 0x0562;
	else if ( _T("bet") == wsName ) return 0x05D1;
	else if ( _T("beta") == wsName ) return 0x03B2;
	else if ( _T("betasymbolgreek") == wsName ) return 0x03D0;
	else if ( _T("betdagesh") == wsName ) return 0xFB31;
	else if ( _T("betdageshhebrew") == wsName ) return 0xFB31;
	else if ( _T("bethebrew") == wsName ) return 0x05D1;
	else if ( _T("betrafehebrew") == wsName ) return 0xFB4C;
	else if ( _T("bhabengali") == wsName ) return 0x09AD;
	else if ( _T("bhadeva") == wsName ) return 0x092D;
	else if ( _T("bhagujarati") == wsName ) return 0x0AAD;
	else if ( _T("bhagurmukhi") == wsName ) return 0x0A2D;
	else if ( _T("bhook") == wsName ) return 0x0253;
	else if ( _T("bihiragana") == wsName ) return 0x3073;
	else if ( _T("bikatakana") == wsName ) return 0x30D3;
	else if ( _T("bilabialclick") == wsName ) return 0x0298;
	else if ( _T("bindigurmukhi") == wsName ) return 0x0A02;
	else if ( _T("birusquare") == wsName ) return 0x3331;
	else if ( _T("blackcircle") == wsName ) return 0x25CF;
	else if ( _T("blackdiamond") == wsName ) return 0x25C6;
	else if ( _T("blackdownpointingtriangle") == wsName ) return 0x25BC;
	else if ( _T("blackleftpointingpointer") == wsName ) return 0x25C4;
	else if ( _T("blackleftpointingtriangle") == wsName ) return 0x25C0;
	else if ( _T("blacklenticularbracketleft") == wsName ) return 0x3010;
	else if ( _T("blacklenticularbracketleftvertical") == wsName ) return 0xFE3B;
	else if ( _T("blacklenticularbracketright") == wsName ) return 0x3011;
	else if ( _T("blacklenticularbracketrightvertical") == wsName ) return 0xFE3C;
	else if ( _T("blacklowerlefttriangle") == wsName ) return 0x25E3;
	else if ( _T("blacklowerrighttriangle") == wsName ) return 0x25E2;
	else if ( _T("blackrectangle") == wsName ) return 0x25AC;
	else if ( _T("blackrightpointingpointer") == wsName ) return 0x25BA;
	else if ( _T("blackrightpointingtriangle") == wsName ) return 0x25B6;
	else if ( _T("blacksmallsquare") == wsName ) return 0x25AA;
	else if ( _T("blacksmilingface") == wsName ) return 0x263B;
	else if ( _T("blacksquare") == wsName ) return 0x25A0;
	else if ( _T("blackstar") == wsName ) return 0x2605;
	else if ( _T("blackupperlefttriangle") == wsName ) return 0x25E4;
	else if ( _T("blackupperrighttriangle") == wsName ) return 0x25E5;
	else if ( _T("blackuppointingsmalltriangle") == wsName ) return 0x25B4;
	else if ( _T("blackuppointingtriangle") == wsName ) return 0x25B2;
	else if ( _T("blank") == wsName ) return 0x2423;
	else if ( _T("blinebelow") == wsName ) return 0x1E07;
	else if ( _T("block") == wsName ) return 0x2588;
	else if ( _T("bmonospace") == wsName ) return 0xFF42;
	else if ( _T("bobaimaithai") == wsName ) return 0x0E1A;
	else if ( _T("bohiragana") == wsName ) return 0x307C;
	else if ( _T("bokatakana") == wsName ) return 0x30DC;
	else if ( _T("bparen") == wsName ) return 0x249D;
	else if ( _T("bqsquare") == wsName ) return 0x33C3;
	else if ( _T("braceex") == wsName ) return 0xF8F4;
	else if ( _T("braceleft") == wsName ) return 0x007B;
	else if ( _T("braceleftbt") == wsName ) return 0xF8F3;
	else if ( _T("braceleftmid") == wsName ) return 0xF8F2;
	else if ( _T("braceleftmonospace") == wsName ) return 0xFF5B;
	else if ( _T("braceleftsmall") == wsName ) return 0xFE5B;
	else if ( _T("bracelefttp") == wsName ) return 0xF8F1;
	else if ( _T("braceleftvertical") == wsName ) return 0xFE37;
	else if ( _T("braceright") == wsName ) return 0x007D;
	else if ( _T("bracerightbt") == wsName ) return 0xF8FE;
	else if ( _T("bracerightmid") == wsName ) return 0xF8FD;
	else if ( _T("bracerightmonospace") == wsName ) return 0xFF5D;
	else if ( _T("bracerightsmall") == wsName ) return 0xFE5C;
	else if ( _T("bracerighttp") == wsName ) return 0xF8FC;
	else if ( _T("bracerightvertical") == wsName ) return 0xFE38;
	else if ( _T("bracketleft") == wsName ) return 0x005B;
	else if ( _T("bracketleftbt") == wsName ) return 0xF8F0;
	else if ( _T("bracketleftex") == wsName ) return 0xF8EF;
	else if ( _T("bracketleftmonospace") == wsName ) return 0xFF3B;
	else if ( _T("bracketlefttp") == wsName ) return 0xF8EE;
	else if ( _T("bracketright") == wsName ) return 0x005D;
	else if ( _T("bracketrightbt") == wsName ) return 0xF8FB;
	else if ( _T("bracketrightex") == wsName ) return 0xF8FA;
	else if ( _T("bracketrightmonospace") == wsName ) return 0xFF3D;
	else if ( _T("bracketrighttp") == wsName ) return 0xF8F9;
	else if ( _T("breve") == wsName ) return 0x02D8;
	else if ( _T("brevebelowcmb") == wsName ) return 0x032E;
	else if ( _T("brevecmb") == wsName ) return 0x0306;
	else if ( _T("breveinvertedbelowcmb") == wsName ) return 0x032F;
	else if ( _T("breveinvertedcmb") == wsName ) return 0x0311;
	else if ( _T("breveinverteddoublecmb") == wsName ) return 0x0361;
	else if ( _T("bridgebelowcmb") == wsName ) return 0x032A;
	else if ( _T("bridgeinvertedbelowcmb") == wsName ) return 0x033A;
	else if ( _T("brokenbar") == wsName ) return 0x00A6;
	else if ( _T("bstroke") == wsName ) return 0x0180;
	else if ( _T("bsuperior") == wsName ) return 0xF6EA;
	else if ( _T("btopbar") == wsName ) return 0x0183;
	else if ( _T("buhiragana") == wsName ) return 0x3076;
	else if ( _T("bukatakana") == wsName ) return 0x30D6;
	else if ( _T("bullet") == wsName ) return 0x2022;
	else if ( _T("bulletinverse") == wsName ) return 0x25D8;
	else if ( _T("bulletoperator") == wsName ) return 0x2219;
	else if ( _T("bullseye") == wsName ) return 0x25CE;

	return 0;
}
static int Type1NameToUnicode_c(CWCharWrapper &wsName)
{
	wchar_t wChar1 = wsName[1];
	switch (wChar1)
	{
	case 0:
		{
			if      ( _T("c") == wsName ) return 0x0063;
			break;
		}
	case 'a':
		{
			if      ( _T("caarmenian") == wsName ) return 0x056E;
			else if ( _T("cabengali") == wsName ) return 0x099A;
			else if ( _T("cacute") == wsName ) return 0x0107;
			else if ( _T("cadeva") == wsName ) return 0x091A;
			else if ( _T("cagujarati") == wsName ) return 0x0A9A;
			else if ( _T("cagurmukhi") == wsName ) return 0x0A1A;
			else if ( _T("calsquare") == wsName ) return 0x3388;
			else if ( _T("candrabindubengali") == wsName ) return 0x0981;
			else if ( _T("candrabinducmb") == wsName ) return 0x0310;
			else if ( _T("candrabindudeva") == wsName ) return 0x0901;
			else if ( _T("candrabindugujarati") == wsName ) return 0x0A81;
			else if ( _T("capslock") == wsName ) return 0x21EA;
			else if ( _T("careof") == wsName ) return 0x2105;
			else if ( _T("caron") == wsName ) return 0x02C7;
			else if ( _T("caronbelowcmb") == wsName ) return 0x032C;
			else if ( _T("caroncmb") == wsName ) return 0x030C;
			else if ( _T("carriagereturn") == wsName ) return 0x21B5;
			break;
		}
	case 'b':
		{
			if      ( _T("cbopomofo") == wsName ) return 0x3118;
			break;
		}
	case 'c':
		{
			if      ( _T("ccaron") == wsName ) return 0x010D;
			else if ( _T("ccedilla") == wsName ) return 0x00E7;
			else if ( _T("ccedillaacute") == wsName ) return 0x1E09;
			else if ( _T("ccircle") == wsName ) return 0x24D2;
			else if ( _T("ccircumflex") == wsName ) return 0x0109;
			else if ( _T("ccurl") == wsName ) return 0x0255;
			break;
		}
	case 'd':
		{
			if      ( _T("cdot") == wsName ) return 0x010B;
			else if ( _T("cdotaccent") == wsName ) return 0x010B;
			else if ( _T("cdsquare") == wsName ) return 0x33C5;
			break;
		}
	case 'e':
		{
			if      ( _T("cedilla") == wsName ) return 0x00B8;
			else if ( _T("cedillacmb") == wsName ) return 0x0327;
			else if ( _T("cent") == wsName ) return 0x00A2;
			else if ( _T("centigrade") == wsName ) return 0x2103;
			else if ( _T("centinferior") == wsName ) return 0xF6DF;
			else if ( _T("centmonospace") == wsName ) return 0xFFE0;
			else if ( _T("centoldstyle") == wsName ) return 0xF7A2;
			else if ( _T("centsuperior") == wsName ) return 0xF6E0;
			break;
		}
	case 'h':
		{
			if      ( _T("chaarmenian") == wsName ) return 0x0579;
			else if ( _T("chabengali") == wsName ) return 0x099B;
			else if ( _T("chadeva") == wsName ) return 0x091B;
			else if ( _T("chagujarati") == wsName ) return 0x0A9B;
			else if ( _T("chagurmukhi") == wsName ) return 0x0A1B;
			else if ( _T("chbopomofo") == wsName ) return 0x3114;
			else if ( _T("cheabkhasiancyrillic") == wsName ) return 0x04BD;
			else if ( _T("checkmark") == wsName ) return 0x2713;
			else if ( _T("checyrillic") == wsName ) return 0x0447;
			else if ( _T("chedescenderabkhasiancyrillic") == wsName ) return 0x04BF;
			else if ( _T("chedescendercyrillic") == wsName ) return 0x04B7;
			else if ( _T("chedieresiscyrillic") == wsName ) return 0x04F5;
			else if ( _T("cheharmenian") == wsName ) return 0x0573;
			else if ( _T("chekhakassiancyrillic") == wsName ) return 0x04CC;
			else if ( _T("cheverticalstrokecyrillic") == wsName ) return 0x04B9;
			else if ( _T("chi") == wsName ) return 0x03C7;
			else if ( _T("chieuchacirclekorean") == wsName ) return 0x3277;
			else if ( _T("chieuchaparenkorean") == wsName ) return 0x3217;
			else if ( _T("chieuchcirclekorean") == wsName ) return 0x3269;
			else if ( _T("chieuchkorean") == wsName ) return 0x314A;
			else if ( _T("chieuchparenkorean") == wsName ) return 0x3209;
			else if ( _T("chochangthai") == wsName ) return 0x0E0A;
			else if ( _T("chochanthai") == wsName ) return 0x0E08;
			else if ( _T("chochingthai") == wsName ) return 0x0E09;
			else if ( _T("chochoethai") == wsName ) return 0x0E0C;
			else if ( _T("chook") == wsName ) return 0x0188;
			break;
		}
	case 'i':
		{
			if      ( _T("cieucacirclekorean") == wsName ) return 0x3276;
			else if ( _T("cieucaparenkorean") == wsName ) return 0x3216;
			else if ( _T("cieuccirclekorean") == wsName ) return 0x3268;
			else if ( _T("cieuckorean") == wsName ) return 0x3148;
			else if ( _T("cieucparenkorean") == wsName ) return 0x3208;
			else if ( _T("cieucuparenkorean") == wsName ) return 0x321C;
			else if ( _T("circle") == wsName ) return 0x25CB;
			else if ( _T("circlecopyrt") == wsName ) return 0x00A9;
			else if ( _T("circlemultiply") == wsName ) return 0x2297;
			else if ( _T("circleot") == wsName ) return 0x2299;
			else if ( _T("circleplus") == wsName ) return 0x2295;
			else if ( _T("circlepostalmark") == wsName ) return 0x3036;
			else if ( _T("circlewithlefthalfblack") == wsName ) return 0x25D0;
			else if ( _T("circlewithrighthalfblack") == wsName ) return 0x25D1;
			else if ( _T("circumflex") == wsName ) return 0x02C6;
			else if ( _T("circumflexbelowcmb") == wsName ) return 0x032D;
			else if ( _T("circumflexcmb") == wsName ) return 0x0302;
			break;
		}
	case 'l':
		{
			if      ( _T("clear") == wsName ) return 0x2327;
			else if ( _T("clickalveolar") == wsName ) return 0x01C2;
			else if ( _T("clickdental") == wsName ) return 0x01C0;
			else if ( _T("clicklateral") == wsName ) return 0x01C1;
			else if ( _T("clickretroflex") == wsName ) return 0x01C3;
			else if ( _T("club") == wsName ) return 0x2663;
			else if ( _T("clubsuitblack") == wsName ) return 0x2663;
			else if ( _T("clubsuitwhite") == wsName ) return 0x2667;
			break;
		}
	case 'm':
		{
			if      ( _T("cmcubedsquare") == wsName ) return 0x33A4;
			else if ( _T("cmonospace") == wsName ) return 0xFF43;
			else if ( _T("cmsquaredsquare") == wsName ) return 0x33A0;
			break;
		}
	case 'o':
		{
			if      ( _T("coarmenian") == wsName ) return 0x0581;
			else if ( _T("colon") == wsName ) return 0x003A;
			else if ( _T("colonmonetary") == wsName ) return 0x20A1;
			else if ( _T("colonmonospace") == wsName ) return 0xFF1A;
			else if ( _T("colonsign") == wsName ) return 0x20A1;
			else if ( _T("colonsmall") == wsName ) return 0xFE55;
			else if ( _T("colontriangularhalfmod") == wsName ) return 0x02D1;
			else if ( _T("colontriangularmod") == wsName ) return 0x02D0;
			else if ( _T("comma") == wsName ) return 0x002C;
			else if ( _T("commaabovecmb") == wsName ) return 0x0313;
			else if ( _T("commaaboverightcmb") == wsName ) return 0x0315;
			else if ( _T("commaaccent") == wsName ) return 0xF6C3;
			else if ( _T("commaarabic") == wsName ) return 0x060C;
			else if ( _T("commaarmenian") == wsName ) return 0x055D;
			else if ( _T("commainferior") == wsName ) return 0xF6E1;
			else if ( _T("commamonospace") == wsName ) return 0xFF0C;
			else if ( _T("commareversedabovecmb") == wsName ) return 0x0314;
			else if ( _T("commareversedmod") == wsName ) return 0x02BD;
			else if ( _T("commasmall") == wsName ) return 0xFE50;
			else if ( _T("commasuperior") == wsName ) return 0xF6E2;
			else if ( _T("commaturnedabovecmb") == wsName ) return 0x0312;
			else if ( _T("commaturnedmod") == wsName ) return 0x02BB;
			else if ( _T("compass") == wsName ) return 0x263C;
			else if ( _T("congruent") == wsName ) return 0x2245;
			else if ( _T("contourintegral") == wsName ) return 0x222E;
			else if ( _T("control") == wsName ) return 0x2303;
			else if ( _T("controlACK") == wsName ) return 0x0006;
			else if ( _T("controlBEL") == wsName ) return 0x0007;
			else if ( _T("controlBS") == wsName ) return 0x0008;
			else if ( _T("controlCAN") == wsName ) return 0x0018;
			else if ( _T("controlCR") == wsName ) return 0x000D;
			else if ( _T("controlDC1") == wsName ) return 0x0011;
			else if ( _T("controlDC2") == wsName ) return 0x0012;
			else if ( _T("controlDC3") == wsName ) return 0x0013;
			else if ( _T("controlDC4") == wsName ) return 0x0014;
			else if ( _T("controlDEL") == wsName ) return 0x007F;
			else if ( _T("controlDLE") == wsName ) return 0x0010;
			else if ( _T("controlEM") == wsName ) return 0x0019;
			else if ( _T("controlENQ") == wsName ) return 0x0005;
			else if ( _T("controlEOT") == wsName ) return 0x0004;
			else if ( _T("controlESC") == wsName ) return 0x001B;
			else if ( _T("controlETB") == wsName ) return 0x0017;
			else if ( _T("controlETX") == wsName ) return 0x0003;
			else if ( _T("controlFF") == wsName ) return 0x000C;
			else if ( _T("controlFS") == wsName ) return 0x001C;
			else if ( _T("controlGS") == wsName ) return 0x001D;
			else if ( _T("controlHT") == wsName ) return 0x0009;
			else if ( _T("controlLF") == wsName ) return 0x000A;
			else if ( _T("controlNAK") == wsName ) return 0x0015;
			else if ( _T("controlRS") == wsName ) return 0x001E;
			else if ( _T("controlSI") == wsName ) return 0x000F;
			else if ( _T("controlSO") == wsName ) return 0x000E;
			else if ( _T("controlSOT") == wsName ) return 0x0002;
			else if ( _T("controlSTX") == wsName ) return 0x0001;
			else if ( _T("controlSUB") == wsName ) return 0x001A;
			else if ( _T("controlSYN") == wsName ) return 0x0016;
			else if ( _T("controlUS") == wsName ) return 0x001F;
			else if ( _T("controlVT") == wsName ) return 0x000B;
			else if ( _T("copyright") == wsName ) return 0x00A9;
			else if ( _T("copyrightsans") == wsName ) return 0xF8E9;
			else if ( _T("copyrightserif") == wsName ) return 0xF6D9;
			else if ( _T("cornerbracketleft") == wsName ) return 0x300C;
			else if ( _T("cornerbracketlefthalfwidth") == wsName ) return 0xFF62;
			else if ( _T("cornerbracketleftvertical") == wsName ) return 0xFE41;
			else if ( _T("cornerbracketright") == wsName ) return 0x300D;
			else if ( _T("cornerbracketrighthalfwidth") == wsName ) return 0xFF63;
			else if ( _T("cornerbracketrightvertical") == wsName ) return 0xFE42;
			else if ( _T("corporationsquare") == wsName ) return 0x337F;
			else if ( _T("cosquare") == wsName ) return 0x33C7;
			else if ( _T("coverkgsquare") == wsName ) return 0x33C6;
			break;
		}
	case 'p':
		{
			if      ( _T("cparen") == wsName ) return 0x249E;
			break;
		}
	case 'r':
		{
			if      ( _T("cruzeiro") == wsName ) return 0x20A2;
			break;
		}
	case 's':
		{
			if      ( _T("cstretched") == wsName ) return 0x0297;
			break;
		}
	case 'u':
		{
			if      ( _T("curlyand") == wsName ) return 0x22CF;
			else if ( _T("curlyor") == wsName ) return 0x22CE;
			else if ( _T("currency") == wsName ) return 0x00A4;
			break;
		}
	case 'y':
		{
			if      ( _T("cyrBreve") == wsName ) return 0xF6D1;
			else if ( _T("cyrFlex") == wsName ) return 0xF6D2;
			else if ( _T("cyrbreve") == wsName ) return 0xF6D4;
			else if ( _T("cyrflex") == wsName ) return 0xF6D5;
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
			if      ( _T("d") == wsName ) return 0x0064;
			break;
		}
	case 'a':
		{
			if      ( _T("daarmenian") == wsName ) return 0x0564;
			else if ( _T("dabengali") == wsName ) return 0x09A6;
			else if ( _T("dadarabic") == wsName ) return 0x0636;
			else if ( _T("dadeva") == wsName ) return 0x0926;
			else if ( _T("dadfinalarabic") == wsName ) return 0xFEBE;
			else if ( _T("dadinitialarabic") == wsName ) return 0xFEBF;
			else if ( _T("dadmedialarabic") == wsName ) return 0xFEC0;
			else if ( _T("dagesh") == wsName ) return 0x05BC;
			else if ( _T("dageshhebrew") == wsName ) return 0x05BC;
			else if ( _T("dagger") == wsName ) return 0x2020;
			else if ( _T("daggerdbl") == wsName ) return 0x2021;
			else if ( _T("dagujarati") == wsName ) return 0x0AA6;
			else if ( _T("dagurmukhi") == wsName ) return 0x0A26;
			else if ( _T("dahiragana") == wsName ) return 0x3060;
			else if ( _T("dakatakana") == wsName ) return 0x30C0;
			else if ( _T("dalarabic") == wsName ) return 0x062F;
			else if ( _T("dalet") == wsName ) return 0x05D3;
			else if ( _T("daletdagesh") == wsName ) return 0xFB33;
			else if ( _T("daletdageshhebrew") == wsName ) return 0xFB33;
			else if ( _T("dalethatafpatah") == wsName ) return 0x05D305B2;
			else if ( _T("dalethatafpatahhebrew") == wsName ) return 0x05D305B2;
			else if ( _T("dalethatafsegol") == wsName ) return 0x05D305B1;
			else if ( _T("dalethatafsegolhebrew") == wsName ) return 0x05D305B1;
			else if ( _T("dalethebrew") == wsName ) return 0x05D3;
			else if ( _T("dalethiriq") == wsName ) return 0x05D305B4;
			else if ( _T("dalethiriqhebrew") == wsName ) return 0x05D305B4;
			else if ( _T("daletholam") == wsName ) return 0x05D305B9;
			else if ( _T("daletholamhebrew") == wsName ) return 0x05D305B9;
			else if ( _T("daletpatah") == wsName ) return 0x05D305B7;
			else if ( _T("daletpatahhebrew") == wsName ) return 0x05D305B7;
			else if ( _T("daletqamats") == wsName ) return 0x05D305B8;
			else if ( _T("daletqamatshebrew") == wsName ) return 0x05D305B8;
			else if ( _T("daletqubuts") == wsName ) return 0x05D305BB;
			else if ( _T("daletqubutshebrew") == wsName ) return 0x05D305BB;
			else if ( _T("daletsegol") == wsName ) return 0x05D305B6;
			else if ( _T("daletsegolhebrew") == wsName ) return 0x05D305B6;
			else if ( _T("daletsheva") == wsName ) return 0x05D305B0;
			else if ( _T("daletshevahebrew") == wsName ) return 0x05D305B0;
			else if ( _T("dalettsere") == wsName ) return 0x05D305B5;
			else if ( _T("dalettserehebrew") == wsName ) return 0x05D305B5;
			else if ( _T("dalfinalarabic") == wsName ) return 0xFEAA;
			else if ( _T("dammaarabic") == wsName ) return 0x064F;
			else if ( _T("dammalowarabic") == wsName ) return 0x064F;
			else if ( _T("dammatanaltonearabic") == wsName ) return 0x064C;
			else if ( _T("dammatanarabic") == wsName ) return 0x064C;
			else if ( _T("danda") == wsName ) return 0x0964;
			else if ( _T("dargahebrew") == wsName ) return 0x05A7;
			else if ( _T("dargalefthebrew") == wsName ) return 0x05A7;
			else if ( _T("dasiapneumatacyrilliccmb") == wsName ) return 0x0485;
			break;
		}
	case 'b':
		{
			if      ( _T("dblGrave") == wsName ) return 0xF6D3;
			else if ( _T("dblanglebracketleft") == wsName ) return 0x300A;
			else if ( _T("dblanglebracketleftvertical") == wsName ) return 0xFE3D;
			else if ( _T("dblanglebracketright") == wsName ) return 0x300B;
			else if ( _T("dblanglebracketrightvertical") == wsName ) return 0xFE3E;
			else if ( _T("dblarchinvertedbelowcmb") == wsName ) return 0x032B;
			else if ( _T("dblarrowleft") == wsName ) return 0x21D4;
			else if ( _T("dblarrowright") == wsName ) return 0x21D2;
			else if ( _T("dbldanda") == wsName ) return 0x0965;
			else if ( _T("dblgrave") == wsName ) return 0xF6D6;
			else if ( _T("dblgravecmb") == wsName ) return 0x030F;
			else if ( _T("dblintegral") == wsName ) return 0x222C;
			else if ( _T("dbllowline") == wsName ) return 0x2017;
			else if ( _T("dbllowlinecmb") == wsName ) return 0x0333;
			else if ( _T("dbloverlinecmb") == wsName ) return 0x033F;
			else if ( _T("dblprimemod") == wsName ) return 0x02BA;
			else if ( _T("dblverticalbar") == wsName ) return 0x2016;
			else if ( _T("dblverticallineabovecmb") == wsName ) return 0x030E;
			else if ( _T("dbopomofo") == wsName ) return 0x3109;
			else if ( _T("dbsquare") == wsName ) return 0x33C8;
			break;
		}
	case 'c':
		{
			if      ( _T("dcaron") == wsName ) return 0x010F;
			else if ( _T("dcedilla") == wsName ) return 0x1E11;
			else if ( _T("dcircle") == wsName ) return 0x24D3;
			else if ( _T("dcircumflexbelow") == wsName ) return 0x1E13;
			else if ( _T("dcroat") == wsName ) return 0x0111;
			break;
		}
	case 'd':
		{
			if      ( _T("ddabengali") == wsName ) return 0x09A1;
			else if ( _T("ddadeva") == wsName ) return 0x0921;
			else if ( _T("ddagujarati") == wsName ) return 0x0AA1;
			else if ( _T("ddagurmukhi") == wsName ) return 0x0A21;
			else if ( _T("ddalarabic") == wsName ) return 0x0688;
			else if ( _T("ddalfinalarabic") == wsName ) return 0xFB89;
			else if ( _T("dddhadeva") == wsName ) return 0x095C;
			else if ( _T("ddhabengali") == wsName ) return 0x09A2;
			else if ( _T("ddhadeva") == wsName ) return 0x0922;
			else if ( _T("ddhagujarati") == wsName ) return 0x0AA2;
			else if ( _T("ddhagurmukhi") == wsName ) return 0x0A22;
			else if ( _T("ddotaccent") == wsName ) return 0x1E0B;
			else if ( _T("ddotbelow") == wsName ) return 0x1E0D;
			break;
		}
	case 'e':
		{
			if      ( _T("decimalseparatorarabic") == wsName ) return 0x066B;
			else if ( _T("decimalseparatorpersian") == wsName ) return 0x066B;
			else if ( _T("decyrillic") == wsName ) return 0x0434;
			else if ( _T("degree") == wsName ) return 0x00B0;
			else if ( _T("dehihebrew") == wsName ) return 0x05AD;
			else if ( _T("dehiragana") == wsName ) return 0x3067;
			else if ( _T("deicoptic") == wsName ) return 0x03EF;
			else if ( _T("dekatakana") == wsName ) return 0x30C7;
			else if ( _T("deleteleft") == wsName ) return 0x232B;
			else if ( _T("deleteright") == wsName ) return 0x2326;
			else if ( _T("delta") == wsName ) return 0x03B4;
			else if ( _T("deltaturned") == wsName ) return 0x018D;
			else if ( _T("denominatorminusonenumeratorbengali") == wsName ) return 0x09F8;
			else if ( _T("dezh") == wsName ) return 0x02A4;
			break;
		}
	case 'h':
		{
			if      ( _T("dhabengali") == wsName ) return 0x09A7;
			else if ( _T("dhadeva") == wsName ) return 0x0927;
			else if ( _T("dhagujarati") == wsName ) return 0x0AA7;
			else if ( _T("dhagurmukhi") == wsName ) return 0x0A27;
			else if ( _T("dhook") == wsName ) return 0x0257;
			break;
		}
	case 'i':
		{
			if      ( _T("dialytikatonos") == wsName ) return 0x0385;
			else if ( _T("dialytikatonoscmb") == wsName ) return 0x0344;
			else if ( _T("diamond") == wsName ) return 0x2666;
			else if ( _T("diamondsuitwhite") == wsName ) return 0x2662;
			else if ( _T("dieresis") == wsName ) return 0x00A8;
			else if ( _T("dieresisacute") == wsName ) return 0xF6D7;
			else if ( _T("dieresisbelowcmb") == wsName ) return 0x0324;
			else if ( _T("dieresiscmb") == wsName ) return 0x0308;
			else if ( _T("dieresisgrave") == wsName ) return 0xF6D8;
			else if ( _T("dieresistonos") == wsName ) return 0x0385;
			else if ( _T("dihiragana") == wsName ) return 0x3062;
			else if ( _T("dikatakana") == wsName ) return 0x30C2;
			else if ( _T("dittomark") == wsName ) return 0x3003;
			else if ( _T("divide") == wsName ) return 0x00F7;
			else if ( _T("divides") == wsName ) return 0x2223;
			else if ( _T("divisionslash") == wsName ) return 0x2215;
			break;
		}
	case 'j':
		{
			if      ( _T("djecyrillic") == wsName ) return 0x0452;
			break;
		}
	case 'k':
		{
			if      ( _T("dkshade") == wsName ) return 0x2593;
			break;
		}
	case 'l':
		{
			if      ( _T("dlinebelow") == wsName ) return 0x1E0F;
			else if ( _T("dlsquare") == wsName ) return 0x3397;
			break;
		}
	case 'm':
		{
			if      ( _T("dmacron") == wsName ) return 0x0111;
			else if ( _T("dmonospace") == wsName ) return 0xFF44;
			break;
		}
	case 'n':
		{
			if      ( _T("dnblock") == wsName ) return 0x2584;
			break;
		}
	case 'o':
		{
			if      ( _T("dochadathai") == wsName ) return 0x0E0E;
			else if ( _T("dodekthai") == wsName ) return 0x0E14;
			else if ( _T("dohiragana") == wsName ) return 0x3069;
			else if ( _T("dokatakana") == wsName ) return 0x30C9;
			else if ( _T("dollar") == wsName ) return 0x0024;
			else if ( _T("dollarinferior") == wsName ) return 0xF6E3;
			else if ( _T("dollarmonospace") == wsName ) return 0xFF04;
			else if ( _T("dollaroldstyle") == wsName ) return 0xF724;
			else if ( _T("dollarsmall") == wsName ) return 0xFE69;
			else if ( _T("dollarsuperior") == wsName ) return 0xF6E4;
			else if ( _T("dong") == wsName ) return 0x20AB;
			else if ( _T("dorusquare") == wsName ) return 0x3326;
			else if ( _T("dotaccent") == wsName ) return 0x02D9;
			else if ( _T("dotaccentcmb") == wsName ) return 0x0307;
			else if ( _T("dotbelowcmb") == wsName ) return 0x0323;
			else if ( _T("dotbelowcomb") == wsName ) return 0x0323;
			else if ( _T("dotkatakana") == wsName ) return 0x30FB;
			else if ( _T("dotlessi") == wsName ) return 0x0131;
			else if ( _T("dotlessj") == wsName ) return 0xF6BE;
			else if ( _T("dotlessjstrokehook") == wsName ) return 0x0284;
			else if ( _T("dotmath") == wsName ) return 0x22C5;
			else if ( _T("dottedcircle") == wsName ) return 0x25CC;
			else if ( _T("doubleyodpatah") == wsName ) return 0xFB1F;
			else if ( _T("doubleyodpatahhebrew") == wsName ) return 0xFB1F;
			else if ( _T("downtackbelowcmb") == wsName ) return 0x031E;
			else if ( _T("downtackmod") == wsName ) return 0x02D5;
			break;
		}
	case 'p':
		{
			if      ( _T("dparen") == wsName ) return 0x249F;
			break;
		}
	case 's':
		{
			if      ( _T("dsuperior") == wsName ) return 0xF6EB;
			break;
		}
	case 't':
		{
			if      ( _T("dtail") == wsName ) return 0x0256;
			else if ( _T("dtopbar") == wsName ) return 0x018C;
			break;
		}
	case 'u':
		{
			if      ( _T("duhiragana") == wsName ) return 0x3065;
			else if ( _T("dukatakana") == wsName ) return 0x30C5;
			break;
		}
	case 'z':
		{
			if      ( _T("dz") == wsName ) return 0x01F3;
			else if ( _T("dzaltone") == wsName ) return 0x02A3;
			else if ( _T("dzcaron") == wsName ) return 0x01C6;
			else if ( _T("dzcurl") == wsName ) return 0x02A5;
			else if ( _T("dzeabkhasiancyrillic") == wsName ) return 0x04E1;
			else if ( _T("dzecyrillic") == wsName ) return 0x0455;
			else if ( _T("dzhecyrillic") == wsName ) return 0x045F;
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
			if      ( _T("e") == wsName ) return 0x0065;
			break;
		}
	case 'a':
		{
			if      ( _T("eacute") == wsName ) return 0x00E9;
			else if ( _T("earth") == wsName ) return 0x2641;
			break;
		}
	case 'b':
		{
			if      ( _T("ebengali") == wsName ) return 0x098F;
			else if ( _T("ebopomofo") == wsName ) return 0x311C;
			else if ( _T("ebreve") == wsName ) return 0x0115;
			break;
		}
	case 'c':
		{
			if      ( _T("ecandradeva") == wsName ) return 0x090D;
			else if ( _T("ecandragujarati") == wsName ) return 0x0A8D;
			else if ( _T("ecandravowelsigndeva") == wsName ) return 0x0945;
			else if ( _T("ecandravowelsigngujarati") == wsName ) return 0x0AC5;
			else if ( _T("ecaron") == wsName ) return 0x011B;
			else if ( _T("ecedillabreve") == wsName ) return 0x1E1D;
			else if ( _T("echarmenian") == wsName ) return 0x0565;
			else if ( _T("echyiwnarmenian") == wsName ) return 0x0587;
			else if ( _T("ecircle") == wsName ) return 0x24D4;
			else if ( _T("ecircumflex") == wsName ) return 0x00EA;
			else if ( _T("ecircumflexacute") == wsName ) return 0x1EBF;
			else if ( _T("ecircumflexbelow") == wsName ) return 0x1E19;
			else if ( _T("ecircumflexdotbelow") == wsName ) return 0x1EC7;
			else if ( _T("ecircumflexgrave") == wsName ) return 0x1EC1;
			else if ( _T("ecircumflexhookabove") == wsName ) return 0x1EC3;
			else if ( _T("ecircumflextilde") == wsName ) return 0x1EC5;
			else if ( _T("ecyrillic") == wsName ) return 0x0454;
			break;
		}
	case 'd':
		{
			if      ( _T("edblgrave") == wsName ) return 0x0205;
			else if ( _T("edeva") == wsName ) return 0x090F;
			else if ( _T("edieresis") == wsName ) return 0x00EB;
			else if ( _T("edot") == wsName ) return 0x0117;
			else if ( _T("edotaccent") == wsName ) return 0x0117;
			else if ( _T("edotbelow") == wsName ) return 0x1EB9;
			break;
		}
	case 'e':
		{
			if      ( _T("eegurmukhi") == wsName ) return 0x0A0F;
			else if ( _T("eematragurmukhi") == wsName ) return 0x0A47;
			break;
		}
	case 'f':
		{
			if      ( _T("efcyrillic") == wsName ) return 0x0444;
			break;
		}
	case 'g':
		{
			if      ( _T("egrave") == wsName ) return 0x00E8;
			else if ( _T("egujarati") == wsName ) return 0x0A8F;
			break;
		}
	case 'h':
		{
			if      ( _T("eharmenian") == wsName ) return 0x0567;
			else if ( _T("ehbopomofo") == wsName ) return 0x311D;
			else if ( _T("ehiragana") == wsName ) return 0x3048;
			else if ( _T("ehookabove") == wsName ) return 0x1EBB;
			break;
		}
	case 'i':
		{
			if      ( _T("eibopomofo") == wsName ) return 0x311F;
			else if ( _T("eight") == wsName ) return 0x0038;
			else if ( _T("eightarabic") == wsName ) return 0x0668;
			else if ( _T("eightbengali") == wsName ) return 0x09EE;
			else if ( _T("eightcircle") == wsName ) return 0x2467;
			else if ( _T("eightcircleinversesansserif") == wsName ) return 0x2791;
			else if ( _T("eightdeva") == wsName ) return 0x096E;
			else if ( _T("eighteencircle") == wsName ) return 0x2471;
			else if ( _T("eighteenparen") == wsName ) return 0x2485;
			else if ( _T("eighteenperiod") == wsName ) return 0x2499;
			else if ( _T("eightgujarati") == wsName ) return 0x0AEE;
			else if ( _T("eightgurmukhi") == wsName ) return 0x0A6E;
			else if ( _T("eighthackarabic") == wsName ) return 0x0668;
			else if ( _T("eighthangzhou") == wsName ) return 0x3028;
			else if ( _T("eighthnotebeamed") == wsName ) return 0x266B;
			else if ( _T("eightideographicparen") == wsName ) return 0x3227;
			else if ( _T("eightinferior") == wsName ) return 0x2088;
			else if ( _T("eightmonospace") == wsName ) return 0xFF18;
			else if ( _T("eightoldstyle") == wsName ) return 0xF738;
			else if ( _T("eightparen") == wsName ) return 0x247B;
			else if ( _T("eightperiod") == wsName ) return 0x248F;
			else if ( _T("eightpersian") == wsName ) return 0x06F8;
			else if ( _T("eightroman") == wsName ) return 0x2177;
			else if ( _T("eightsuperior") == wsName ) return 0x2078;
			else if ( _T("eightthai") == wsName ) return 0x0E58;
			else if ( _T("einvertedbreve") == wsName ) return 0x0207;
			else if ( _T("eiotifiedcyrillic") == wsName ) return 0x0465;
			break;
		}
	case 'k':
		{
			if      ( _T("ekatakana") == wsName ) return 0x30A8;
			else if ( _T("ekatakanahalfwidth") == wsName ) return 0xFF74;
			else if ( _T("ekonkargurmukhi") == wsName ) return 0x0A74;
			else if ( _T("ekorean") == wsName ) return 0x3154;
			break;
		}
	case 'l':
		{
			if      ( _T("elcyrillic") == wsName ) return 0x043B;
			else if ( _T("element") == wsName ) return 0x2208;
			else if ( _T("elevencircle") == wsName ) return 0x246A;
			else if ( _T("elevenparen") == wsName ) return 0x247E;
			else if ( _T("elevenperiod") == wsName ) return 0x2492;
			else if ( _T("elevenroman") == wsName ) return 0x217A;
			else if ( _T("ellipsis") == wsName ) return 0x2026;
			else if ( _T("ellipsisvertical") == wsName ) return 0x22EE;
			break;
		}
	case 'm':
		{
			if      ( _T("emacron") == wsName ) return 0x0113;
			else if ( _T("emacronacute") == wsName ) return 0x1E17;
			else if ( _T("emacrongrave") == wsName ) return 0x1E15;
			else if ( _T("emcyrillic") == wsName ) return 0x043C;
			else if ( _T("emdash") == wsName ) return 0x2014;
			else if ( _T("emdashvertical") == wsName ) return 0xFE31;
			else if ( _T("emonospace") == wsName ) return 0xFF45;
			else if ( _T("emphasismarkarmenian") == wsName ) return 0x055B;
			else if ( _T("emptyset") == wsName ) return 0x2205;
			break;
		}
	case 'n':
		{
			if      ( _T("enbopomofo") == wsName ) return 0x3123;
			else if ( _T("encyrillic") == wsName ) return 0x043D;
			else if ( _T("endash") == wsName ) return 0x2013;
			else if ( _T("endashvertical") == wsName ) return 0xFE32;
			else if ( _T("endescendercyrillic") == wsName ) return 0x04A3;
			else if ( _T("eng") == wsName ) return 0x014B;
			else if ( _T("engbopomofo") == wsName ) return 0x3125;
			else if ( _T("enghecyrillic") == wsName ) return 0x04A5;
			else if ( _T("enhookcyrillic") == wsName ) return 0x04C8;
			else if ( _T("enspace") == wsName ) return 0x2002;
			break;
		}
	case 'o':
		{
			if      ( _T("eogonek") == wsName ) return 0x0119;
			else if ( _T("eokorean") == wsName ) return 0x3153;
			else if ( _T("eopen") == wsName ) return 0x025B;
			else if ( _T("eopenclosed") == wsName ) return 0x029A;
			else if ( _T("eopenreversed") == wsName ) return 0x025C;
			else if ( _T("eopenreversedclosed") == wsName ) return 0x025E;
			else if ( _T("eopenreversedhook") == wsName ) return 0x025D;
			break;
		}
	case 'p':
		{
			if      ( _T("eparen") == wsName ) return 0x24A0;
			else if ( _T("epsilon") == wsName ) return 0x03B5;
			else if ( _T("epsilontonos") == wsName ) return 0x03AD;
			break;
		}
	case 'q':
		{
			if      ( _T("equal") == wsName ) return 0x003D;
			else if ( _T("equalmonospace") == wsName ) return 0xFF1D;
			else if ( _T("equalsmall") == wsName ) return 0xFE66;
			else if ( _T("equalsuperior") == wsName ) return 0x207C;
			else if ( _T("equivalence") == wsName ) return 0x2261;
			break;
		}
	case 'r':
		{
			if      ( _T("erbopomofo") == wsName ) return 0x3126;
			else if ( _T("ercyrillic") == wsName ) return 0x0440;
			else if ( _T("ereversed") == wsName ) return 0x0258;
			else if ( _T("ereversedcyrillic") == wsName ) return 0x044D;
			break;
		}
	case 's':
		{
			if      ( _T("escyrillic") == wsName ) return 0x0441;
			else if ( _T("esdescendercyrillic") == wsName ) return 0x04AB;
			else if ( _T("esh") == wsName ) return 0x0283;
			else if ( _T("eshcurl") == wsName ) return 0x0286;
			else if ( _T("eshortdeva") == wsName ) return 0x090E;
			else if ( _T("eshortvowelsigndeva") == wsName ) return 0x0946;
			else if ( _T("eshreversedloop") == wsName ) return 0x01AA;
			else if ( _T("eshsquatreversed") == wsName ) return 0x0285;
			else if ( _T("esmallhiragana") == wsName ) return 0x3047;
			else if ( _T("esmallkatakana") == wsName ) return 0x30A7;
			else if ( _T("esmallkatakanahalfwidth") == wsName ) return 0xFF6A;
			else if ( _T("estimated") == wsName ) return 0x212E;
			else if ( _T("esuperior") == wsName ) return 0xF6EC;
			break;
		}
	case 't':
		{
			if      ( _T("eta") == wsName ) return 0x03B7;
			else if ( _T("etarmenian") == wsName ) return 0x0568;
			else if ( _T("etatonos") == wsName ) return 0x03AE;
			else if ( _T("eth") == wsName ) return 0x00F0;
			else if ( _T("etilde") == wsName ) return 0x1EBD;
			else if ( _T("etildebelow") == wsName ) return 0x1E1B;
			else if ( _T("etnahtafoukhhebrew") == wsName ) return 0x0591;
			else if ( _T("etnahtafoukhlefthebrew") == wsName ) return 0x0591;
			else if ( _T("etnahtahebrew") == wsName ) return 0x0591;
			else if ( _T("etnahtalefthebrew") == wsName ) return 0x0591;
			else if ( _T("eturned") == wsName ) return 0x01DD;
			break;
		}
	case 'u':
		{
			if      ( _T("eukorean") == wsName ) return 0x3161;
			else if ( _T("euro") == wsName ) return 0x20AC;
			break;
		}
	case 'v':
		{
			if      ( _T("evowelsignbengali") == wsName ) return 0x09C7;
			else if ( _T("evowelsigndeva") == wsName ) return 0x0947;
			else if ( _T("evowelsigngujarati") == wsName ) return 0x0AC7;
			break;
		}
	case 'x':
		{
			if      ( _T("exclam") == wsName ) return 0x0021;
			else if ( _T("exclamarmenian") == wsName ) return 0x055C;
			else if ( _T("exclamdbl") == wsName ) return 0x203C;
			else if ( _T("exclamdown") == wsName ) return 0x00A1;
			else if ( _T("exclamdownsmall") == wsName ) return 0xF7A1;
			else if ( _T("exclammonospace") == wsName ) return 0xFF01;
			else if ( _T("exclamsmall") == wsName ) return 0xF721;
			else if ( _T("existential") == wsName ) return 0x2203;
			break;
		}
	case 'z':
		{
			if      ( _T("ezh") == wsName ) return 0x0292;
			else if ( _T("ezhcaron") == wsName ) return 0x01EF;
			else if ( _T("ezhcurl") == wsName ) return 0x0293;
			else if ( _T("ezhreversed") == wsName ) return 0x01B9;
			else if ( _T("ezhtail") == wsName ) return 0x01BA;
			break;
		}
	}

	return 0;
}
static int Type1NameToUnicode_f(CWCharWrapper &wsName)
{
	if      ( _T("f") == wsName ) return 0x0066;
	else if ( _T("fadeva") == wsName ) return 0x095E;
	else if ( _T("fagurmukhi") == wsName ) return 0x0A5E;
	else if ( _T("fahrenheit") == wsName ) return 0x2109;
	else if ( _T("fathaarabic") == wsName ) return 0x064E;
	else if ( _T("fathalowarabic") == wsName ) return 0x064E;
	else if ( _T("fathatanarabic") == wsName ) return 0x064B;
	else if ( _T("fbopomofo") == wsName ) return 0x3108;
	else if ( _T("fcircle") == wsName ) return 0x24D5;
	else if ( _T("fdotaccent") == wsName ) return 0x1E1F;
	else if ( _T("feharabic") == wsName ) return 0x0641;
	else if ( _T("feharmenian") == wsName ) return 0x0586;
	else if ( _T("fehfinalarabic") == wsName ) return 0xFED2;
	else if ( _T("fehinitialarabic") == wsName ) return 0xFED3;
	else if ( _T("fehmedialarabic") == wsName ) return 0xFED4;
	else if ( _T("feicoptic") == wsName ) return 0x03E5;
	else if ( _T("female") == wsName ) return 0x2640;
	else if ( _T("ff") == wsName ) return 0xFB00;
	else if ( _T("ffi") == wsName ) return 0xFB03;
	else if ( _T("ffl") == wsName ) return 0xFB04;
	else if ( _T("fi") == wsName ) return 0xFB01;
	else if ( _T("fifteencircle") == wsName ) return 0x246E;
	else if ( _T("fifteenparen") == wsName ) return 0x2482;
	else if ( _T("fifteenperiod") == wsName ) return 0x2496;
	else if ( _T("figuredash") == wsName ) return 0x2012;
	else if ( _T("filledbox") == wsName ) return 0x25A0;
	else if ( _T("filledrect") == wsName ) return 0x25AC;
	else if ( _T("finalkaf") == wsName ) return 0x05DA;
	else if ( _T("finalkafdagesh") == wsName ) return 0xFB3A;
	else if ( _T("finalkafdageshhebrew") == wsName ) return 0xFB3A;
	else if ( _T("finalkafhebrew") == wsName ) return 0x05DA;
	else if ( _T("finalkafqamats") == wsName ) return 0x05DA05B8;
	else if ( _T("finalkafqamatshebrew") == wsName ) return 0x05DA05B8;
	else if ( _T("finalkafsheva") == wsName ) return 0x05DA05B0;
	else if ( _T("finalkafshevahebrew") == wsName ) return 0x05DA05B0;
	else if ( _T("finalmem") == wsName ) return 0x05DD;
	else if ( _T("finalmemhebrew") == wsName ) return 0x05DD;
	else if ( _T("finalnun") == wsName ) return 0x05DF;
	else if ( _T("finalnunhebrew") == wsName ) return 0x05DF;
	else if ( _T("finalpe") == wsName ) return 0x05E3;
	else if ( _T("finalpehebrew") == wsName ) return 0x05E3;
	else if ( _T("finaltsadi") == wsName ) return 0x05E5;
	else if ( _T("finaltsadihebrew") == wsName ) return 0x05E5;
	else if ( _T("firsttonechinese") == wsName ) return 0x02C9;
	else if ( _T("fisheye") == wsName ) return 0x25C9;
	else if ( _T("fitacyrillic") == wsName ) return 0x0473;
	else if ( _T("five") == wsName ) return 0x0035;
	else if ( _T("fivearabic") == wsName ) return 0x0665;
	else if ( _T("fivebengali") == wsName ) return 0x09EB;
	else if ( _T("fivecircle") == wsName ) return 0x2464;
	else if ( _T("fivecircleinversesansserif") == wsName ) return 0x278E;
	else if ( _T("fivedeva") == wsName ) return 0x096B;
	else if ( _T("fiveeighths") == wsName ) return 0x215D;
	else if ( _T("fivegujarati") == wsName ) return 0x0AEB;
	else if ( _T("fivegurmukhi") == wsName ) return 0x0A6B;
	else if ( _T("fivehackarabic") == wsName ) return 0x0665;
	else if ( _T("fivehangzhou") == wsName ) return 0x3025;
	else if ( _T("fiveideographicparen") == wsName ) return 0x3224;
	else if ( _T("fiveinferior") == wsName ) return 0x2085;
	else if ( _T("fivemonospace") == wsName ) return 0xFF15;
	else if ( _T("fiveoldstyle") == wsName ) return 0xF735;
	else if ( _T("fiveparen") == wsName ) return 0x2478;
	else if ( _T("fiveperiod") == wsName ) return 0x248C;
	else if ( _T("fivepersian") == wsName ) return 0x06F5;
	else if ( _T("fiveroman") == wsName ) return 0x2174;
	else if ( _T("fivesuperior") == wsName ) return 0x2075;
	else if ( _T("fivethai") == wsName ) return 0x0E55;
	else if ( _T("fl") == wsName ) return 0xFB02;
	else if ( _T("florin") == wsName ) return 0x0192;
	else if ( _T("fmonospace") == wsName ) return 0xFF46;
	else if ( _T("fmsquare") == wsName ) return 0x3399;
	else if ( _T("fofanthai") == wsName ) return 0x0E1F;
	else if ( _T("fofathai") == wsName ) return 0x0E1D;
	else if ( _T("fongmanthai") == wsName ) return 0x0E4F;
	else if ( _T("forall") == wsName ) return 0x2200;
	else if ( _T("four") == wsName ) return 0x0034;
	else if ( _T("fourarabic") == wsName ) return 0x0664;
	else if ( _T("fourbengali") == wsName ) return 0x09EA;
	else if ( _T("fourcircle") == wsName ) return 0x2463;
	else if ( _T("fourcircleinversesansserif") == wsName ) return 0x278D;
	else if ( _T("fourdeva") == wsName ) return 0x096A;
	else if ( _T("fourgujarati") == wsName ) return 0x0AEA;
	else if ( _T("fourgurmukhi") == wsName ) return 0x0A6A;
	else if ( _T("fourhackarabic") == wsName ) return 0x0664;
	else if ( _T("fourhangzhou") == wsName ) return 0x3024;
	else if ( _T("fourideographicparen") == wsName ) return 0x3223;
	else if ( _T("fourinferior") == wsName ) return 0x2084;
	else if ( _T("fourmonospace") == wsName ) return 0xFF14;
	else if ( _T("fournumeratorbengali") == wsName ) return 0x09F7;
	else if ( _T("fouroldstyle") == wsName ) return 0xF734;
	else if ( _T("fourparen") == wsName ) return 0x2477;
	else if ( _T("fourperiod") == wsName ) return 0x248B;
	else if ( _T("fourpersian") == wsName ) return 0x06F4;
	else if ( _T("fourroman") == wsName ) return 0x2173;
	else if ( _T("foursuperior") == wsName ) return 0x2074;
	else if ( _T("fourteencircle") == wsName ) return 0x246D;
	else if ( _T("fourteenparen") == wsName ) return 0x2481;
	else if ( _T("fourteenperiod") == wsName ) return 0x2495;
	else if ( _T("fourthai") == wsName ) return 0x0E54;
	else if ( _T("fourthtonechinese") == wsName ) return 0x02CB;
	else if ( _T("fparen") == wsName ) return 0x24A1;
	else if ( _T("fraction") == wsName ) return 0x2044;
	else if ( _T("franc") == wsName ) return 0x20A3;

	return 0;
}
static int Type1NameToUnicode_g(CWCharWrapper &wsName)
{
	if      ( _T("g") == wsName ) return 0x0067;
	else if ( _T("gabengali") == wsName ) return 0x0997;
	else if ( _T("gacute") == wsName ) return 0x01F5;
	else if ( _T("gadeva") == wsName ) return 0x0917;
	else if ( _T("gafarabic") == wsName ) return 0x06AF;
	else if ( _T("gaffinalarabic") == wsName ) return 0xFB93;
	else if ( _T("gafinitialarabic") == wsName ) return 0xFB94;
	else if ( _T("gafmedialarabic") == wsName ) return 0xFB95;
	else if ( _T("gagujarati") == wsName ) return 0x0A97;
	else if ( _T("gagurmukhi") == wsName ) return 0x0A17;
	else if ( _T("gahiragana") == wsName ) return 0x304C;
	else if ( _T("gakatakana") == wsName ) return 0x30AC;
	else if ( _T("gamma") == wsName ) return 0x03B3;
	else if ( _T("gammalatinsmall") == wsName ) return 0x0263;
	else if ( _T("gammasuperior") == wsName ) return 0x02E0;
	else if ( _T("gangiacoptic") == wsName ) return 0x03EB;
	else if ( _T("gbopomofo") == wsName ) return 0x310D;
	else if ( _T("gbreve") == wsName ) return 0x011F;
	else if ( _T("gcaron") == wsName ) return 0x01E7;
	else if ( _T("gcedilla") == wsName ) return 0x0123;
	else if ( _T("gcircle") == wsName ) return 0x24D6;
	else if ( _T("gcircumflex") == wsName ) return 0x011D;
	else if ( _T("gcommaaccent") == wsName ) return 0x0123;
	else if ( _T("gdot") == wsName ) return 0x0121;
	else if ( _T("gdotaccent") == wsName ) return 0x0121;
	else if ( _T("gecyrillic") == wsName ) return 0x0433;
	else if ( _T("gehiragana") == wsName ) return 0x3052;
	else if ( _T("gekatakana") == wsName ) return 0x30B2;
	else if ( _T("geometricallyequal") == wsName ) return 0x2251;
	else if ( _T("gereshaccenthebrew") == wsName ) return 0x059C;
	else if ( _T("gereshhebrew") == wsName ) return 0x05F3;
	else if ( _T("gereshmuqdamhebrew") == wsName ) return 0x059D;
	else if ( _T("germandbls") == wsName ) return 0x00DF;
	else if ( _T("gershayimaccenthebrew") == wsName ) return 0x059E;
	else if ( _T("gershayimhebrew") == wsName ) return 0x05F4;
	else if ( _T("getamark") == wsName ) return 0x3013;
	else if ( _T("ghabengali") == wsName ) return 0x0998;
	else if ( _T("ghadarmenian") == wsName ) return 0x0572;
	else if ( _T("ghadeva") == wsName ) return 0x0918;
	else if ( _T("ghagujarati") == wsName ) return 0x0A98;
	else if ( _T("ghagurmukhi") == wsName ) return 0x0A18;
	else if ( _T("ghainarabic") == wsName ) return 0x063A;
	else if ( _T("ghainfinalarabic") == wsName ) return 0xFECE;
	else if ( _T("ghaininitialarabic") == wsName ) return 0xFECF;
	else if ( _T("ghainmedialarabic") == wsName ) return 0xFED0;
	else if ( _T("ghemiddlehookcyrillic") == wsName ) return 0x0495;
	else if ( _T("ghestrokecyrillic") == wsName ) return 0x0493;
	else if ( _T("gheupturncyrillic") == wsName ) return 0x0491;
	else if ( _T("ghhadeva") == wsName ) return 0x095A;
	else if ( _T("ghhagurmukhi") == wsName ) return 0x0A5A;
	else if ( _T("ghook") == wsName ) return 0x0260;
	else if ( _T("ghzsquare") == wsName ) return 0x3393;
	else if ( _T("gihiragana") == wsName ) return 0x304E;
	else if ( _T("gikatakana") == wsName ) return 0x30AE;
	else if ( _T("gimarmenian") == wsName ) return 0x0563;
	else if ( _T("gimel") == wsName ) return 0x05D2;
	else if ( _T("gimeldagesh") == wsName ) return 0xFB32;
	else if ( _T("gimeldageshhebrew") == wsName ) return 0xFB32;
	else if ( _T("gimelhebrew") == wsName ) return 0x05D2;
	else if ( _T("gjecyrillic") == wsName ) return 0x0453;
	else if ( _T("glottalinvertedstroke") == wsName ) return 0x01BE;
	else if ( _T("glottalstop") == wsName ) return 0x0294;
	else if ( _T("glottalstopinverted") == wsName ) return 0x0296;
	else if ( _T("glottalstopmod") == wsName ) return 0x02C0;
	else if ( _T("glottalstopreversed") == wsName ) return 0x0295;
	else if ( _T("glottalstopreversedmod") == wsName ) return 0x02C1;
	else if ( _T("glottalstopreversedsuperior") == wsName ) return 0x02E4;
	else if ( _T("glottalstopstroke") == wsName ) return 0x02A1;
	else if ( _T("glottalstopstrokereversed") == wsName ) return 0x02A2;
	else if ( _T("gmacron") == wsName ) return 0x1E21;
	else if ( _T("gmonospace") == wsName ) return 0xFF47;
	else if ( _T("gohiragana") == wsName ) return 0x3054;
	else if ( _T("gokatakana") == wsName ) return 0x30B4;
	else if ( _T("gparen") == wsName ) return 0x24A2;
	else if ( _T("gpasquare") == wsName ) return 0x33AC;
	else if ( _T("gradient") == wsName ) return 0x2207;
	else if ( _T("grave") == wsName ) return 0x0060;
	else if ( _T("gravebelowcmb") == wsName ) return 0x0316;
	else if ( _T("gravecmb") == wsName ) return 0x0300;
	else if ( _T("gravecomb") == wsName ) return 0x0300;
	else if ( _T("gravedeva") == wsName ) return 0x0953;
	else if ( _T("gravelowmod") == wsName ) return 0x02CE;
	else if ( _T("gravemonospace") == wsName ) return 0xFF40;
	else if ( _T("gravetonecmb") == wsName ) return 0x0340;
	else if ( _T("greater") == wsName ) return 0x003E;
	else if ( _T("greaterequal") == wsName ) return 0x2265;
	else if ( _T("greaterequalorless") == wsName ) return 0x22DB;
	else if ( _T("greatermonospace") == wsName ) return 0xFF1E;
	else if ( _T("greaterorequivalent") == wsName ) return 0x2273;
	else if ( _T("greaterorless") == wsName ) return 0x2277;
	else if ( _T("greateroverequal") == wsName ) return 0x2267;
	else if ( _T("greatersmall") == wsName ) return 0xFE65;
	else if ( _T("gscript") == wsName ) return 0x0261;
	else if ( _T("gstroke") == wsName ) return 0x01E5;
	else if ( _T("guhiragana") == wsName ) return 0x3050;
	else if ( _T("guillemotleft") == wsName ) return 0x00AB;
	else if ( _T("guillemotright") == wsName ) return 0x00BB;
	else if ( _T("guilsinglleft") == wsName ) return 0x2039;
	else if ( _T("guilsinglright") == wsName ) return 0x203A;
	else if ( _T("gukatakana") == wsName ) return 0x30B0;
	else if ( _T("guramusquare") == wsName ) return 0x3318;
	else if ( _T("gysquare") == wsName ) return 0x33C9;

	return 0;
}
static int Type1NameToUnicode_h(CWCharWrapper &wsName)
{
	wchar_t wChar1 = wsName[1];
	switch( wChar1 )
	{
	case 0:
		{
			if      ( _T("h") == wsName ) return 0x0068;
			break;
		}
	case 'a':
		{
			if      ( _T("haabkhasiancyrillic") == wsName ) return 0x04A9;
			else if ( _T("haaltonearabic") == wsName ) return 0x06C1;
			else if ( _T("habengali") == wsName ) return 0x09B9;
			else if ( _T("hadescendercyrillic") == wsName ) return 0x04B3;
			else if ( _T("hadeva") == wsName ) return 0x0939;
			else if ( _T("hagujarati") == wsName ) return 0x0AB9;
			else if ( _T("hagurmukhi") == wsName ) return 0x0A39;
			else if ( _T("haharabic") == wsName ) return 0x062D;
			else if ( _T("hahfinalarabic") == wsName ) return 0xFEA2;
			else if ( _T("hahinitialarabic") == wsName ) return 0xFEA3;
			else if ( _T("hahiragana") == wsName ) return 0x306F;
			else if ( _T("hahmedialarabic") == wsName ) return 0xFEA4;
			else if ( _T("haitusquare") == wsName ) return 0x332A;
			else if ( _T("hakatakana") == wsName ) return 0x30CF;
			else if ( _T("hakatakanahalfwidth") == wsName ) return 0xFF8A;
			else if ( _T("halantgurmukhi") == wsName ) return 0x0A4D;
			else if ( _T("hamzaarabic") == wsName ) return 0x0621;
			else if ( _T("hamzadammaarabic") == wsName ) return 0x0621064F;
			else if ( _T("hamzadammatanarabic") == wsName ) return 0x0621064C;
			else if ( _T("hamzafathaarabic") == wsName ) return 0x0621064E;
			else if ( _T("hamzafathatanarabic") == wsName ) return 0x0621064B;
			else if ( _T("hamzalowarabic") == wsName ) return 0x0621;
			else if ( _T("hamzalowkasraarabic") == wsName ) return 0x06210650;
			else if ( _T("hamzalowkasratanarabic") == wsName ) return 0x0621064D;
			else if ( _T("hamzasukunarabic") == wsName ) return 0x06210652;
			else if ( _T("hangulfiller") == wsName ) return 0x3164;
			else if ( _T("hardsigncyrillic") == wsName ) return 0x044A;
			else if ( _T("harpoonleftbarbup") == wsName ) return 0x21BC;
			else if ( _T("harpoonrightbarbup") == wsName ) return 0x21C0;
			else if ( _T("hasquare") == wsName ) return 0x33CA;
			else if ( _T("hatafpatah") == wsName ) return 0x05B2;
			else if ( _T("hatafpatah16") == wsName ) return 0x05B2;
			else if ( _T("hatafpatah23") == wsName ) return 0x05B2;
			else if ( _T("hatafpatah2f") == wsName ) return 0x05B2;
			else if ( _T("hatafpatahhebrew") == wsName ) return 0x05B2;
			else if ( _T("hatafpatahnarrowhebrew") == wsName ) return 0x05B2;
			else if ( _T("hatafpatahquarterhebrew") == wsName ) return 0x05B2;
			else if ( _T("hatafpatahwidehebrew") == wsName ) return 0x05B2;
			else if ( _T("hatafqamats") == wsName ) return 0x05B3;
			else if ( _T("hatafqamats1b") == wsName ) return 0x05B3;
			else if ( _T("hatafqamats28") == wsName ) return 0x05B3;
			else if ( _T("hatafqamats34") == wsName ) return 0x05B3;
			else if ( _T("hatafqamatshebrew") == wsName ) return 0x05B3;
			else if ( _T("hatafqamatsnarrowhebrew") == wsName ) return 0x05B3;
			else if ( _T("hatafqamatsquarterhebrew") == wsName ) return 0x05B3;
			else if ( _T("hatafqamatswidehebrew") == wsName ) return 0x05B3;
			else if ( _T("hatafsegol") == wsName ) return 0x05B1;
			else if ( _T("hatafsegol17") == wsName ) return 0x05B1;
			else if ( _T("hatafsegol24") == wsName ) return 0x05B1;
			else if ( _T("hatafsegol30") == wsName ) return 0x05B1;
			else if ( _T("hatafsegolhebrew") == wsName ) return 0x05B1;
			else if ( _T("hatafsegolnarrowhebrew") == wsName ) return 0x05B1;
			else if ( _T("hatafsegolquarterhebrew") == wsName ) return 0x05B1;
			else if ( _T("hatafsegolwidehebrew") == wsName ) return 0x05B1;
			break;
		}
	case 'b':
		{
			if      ( _T("hbar") == wsName ) return 0x0127;
			else if ( _T("hbopomofo") == wsName ) return 0x310F;
			else if ( _T("hbrevebelow") == wsName ) return 0x1E2B;
			break;
		}
	case 'c':
		{
			if      ( _T("hcedilla") == wsName ) return 0x1E29;
			else if ( _T("hcircle") == wsName ) return 0x24D7;
			else if ( _T("hcircumflex") == wsName ) return 0x0125;
			break;
		}
	case 'd':
		{
			if      ( _T("hdieresis") == wsName ) return 0x1E27;
			else if ( _T("hdotaccent") == wsName ) return 0x1E23;
			else if ( _T("hdotbelow") == wsName ) return 0x1E25;
			break;
		}
	case 'e':
		{
			if      ( _T("he") == wsName ) return 0x05D4;
			else if ( _T("heart") == wsName ) return 0x2665;
			else if ( _T("heartsuitblack") == wsName ) return 0x2665;
			else if ( _T("heartsuitwhite") == wsName ) return 0x2661;
			else if ( _T("hedagesh") == wsName ) return 0xFB34;
			else if ( _T("hedageshhebrew") == wsName ) return 0xFB34;
			else if ( _T("hehaltonearabic") == wsName ) return 0x06C1;
			else if ( _T("heharabic") == wsName ) return 0x0647;
			else if ( _T("hehebrew") == wsName ) return 0x05D4;
			else if ( _T("hehfinalaltonearabic") == wsName ) return 0xFBA7;
			else if ( _T("hehfinalalttwoarabic") == wsName ) return 0xFEEA;
			else if ( _T("hehfinalarabic") == wsName ) return 0xFEEA;
			else if ( _T("hehhamzaabovefinalarabic") == wsName ) return 0xFBA5;
			else if ( _T("hehhamzaaboveisolatedarabic") == wsName ) return 0xFBA4;
			else if ( _T("hehinitialaltonearabic") == wsName ) return 0xFBA8;
			else if ( _T("hehinitialarabic") == wsName ) return 0xFEEB;
			else if ( _T("hehiragana") == wsName ) return 0x3078;
			else if ( _T("hehmedialaltonearabic") == wsName ) return 0xFBA9;
			else if ( _T("hehmedialarabic") == wsName ) return 0xFEEC;
			else if ( _T("heiseierasquare") == wsName ) return 0x337B;
			else if ( _T("hekatakana") == wsName ) return 0x30D8;
			else if ( _T("hekatakanahalfwidth") == wsName ) return 0xFF8D;
			else if ( _T("hekutaarusquare") == wsName ) return 0x3336;
			else if ( _T("henghook") == wsName ) return 0x0267;
			else if ( _T("herutusquare") == wsName ) return 0x3339;
			else if ( _T("het") == wsName ) return 0x05D7;
			else if ( _T("hethebrew") == wsName ) return 0x05D7;
			break;
		}
	case 'h':
		{
			if      ( _T("hhook") == wsName ) return 0x0266;
			else if ( _T("hhooksuperior") == wsName ) return 0x02B1;
			break;
		}
	case 'i':
		{
			if      ( _T("hieuhacirclekorean") == wsName ) return 0x327B;
			else if ( _T("hieuhaparenkorean") == wsName ) return 0x321B;
			else if ( _T("hieuhcirclekorean") == wsName ) return 0x326D;
			else if ( _T("hieuhkorean") == wsName ) return 0x314E;
			else if ( _T("hieuhparenkorean") == wsName ) return 0x320D;
			else if ( _T("hihiragana") == wsName ) return 0x3072;
			else if ( _T("hikatakana") == wsName ) return 0x30D2;
			else if ( _T("hikatakanahalfwidth") == wsName ) return 0xFF8B;
			else if ( _T("hiriq") == wsName ) return 0x05B4;
			else if ( _T("hiriq14") == wsName ) return 0x05B4;
			else if ( _T("hiriq21") == wsName ) return 0x05B4;
			else if ( _T("hiriq2d") == wsName ) return 0x05B4;
			else if ( _T("hiriqhebrew") == wsName ) return 0x05B4;
			else if ( _T("hiriqnarrowhebrew") == wsName ) return 0x05B4;
			else if ( _T("hiriqquarterhebrew") == wsName ) return 0x05B4;
			else if ( _T("hiriqwidehebrew") == wsName ) return 0x05B4;
			break;
		}
	case 'l':
		{
			if      ( _T("hlinebelow") == wsName ) return 0x1E96;
			break;
		}
	case 'm':
		{
			if      ( _T("hmonospace") == wsName ) return 0xFF48;
			break;
		}
	case 'o':
		{
			if      ( _T("hoarmenian") == wsName ) return 0x0570;
			else if ( _T("hohipthai") == wsName ) return 0x0E2B;
			else if ( _T("hohiragana") == wsName ) return 0x307B;
			else if ( _T("hokatakana") == wsName ) return 0x30DB;
			else if ( _T("hokatakanahalfwidth") == wsName ) return 0xFF8E;
			else if ( _T("holam") == wsName ) return 0x05B9;
			else if ( _T("holam19") == wsName ) return 0x05B9;
			else if ( _T("holam26") == wsName ) return 0x05B9;
			else if ( _T("holam32") == wsName ) return 0x05B9;
			else if ( _T("holamhebrew") == wsName ) return 0x05B9;
			else if ( _T("holamnarrowhebrew") == wsName ) return 0x05B9;
			else if ( _T("holamquarterhebrew") == wsName ) return 0x05B9;
			else if ( _T("holamwidehebrew") == wsName ) return 0x05B9;
			else if ( _T("honokhukthai") == wsName ) return 0x0E2E;
			else if ( _T("hookabovecomb") == wsName ) return 0x0309;
			else if ( _T("hookcmb") == wsName ) return 0x0309;
			else if ( _T("hookpalatalizedbelowcmb") == wsName ) return 0x0321;
			else if ( _T("hookretroflexbelowcmb") == wsName ) return 0x0322;
			else if ( _T("hoonsquare") == wsName ) return 0x3342;
			else if ( _T("horicoptic") == wsName ) return 0x03E9;
			else if ( _T("horizontalbar") == wsName ) return 0x2015;
			else if ( _T("horncmb") == wsName ) return 0x031B;
			else if ( _T("hotsprings") == wsName ) return 0x2668;
			else if ( _T("house") == wsName ) return 0x2302;
			break;
		}
	case 'p':
		{
			if      ( _T("hparen") == wsName ) return 0x24A3;
			break;
		}
	case 's':
		{
			if      ( _T("hsuperior") == wsName ) return 0x02B0;
			break;
		}
	case 't':
		{
			if      ( _T("hturned") == wsName ) return 0x0265;
			break;
		}
	case 'u':
		{
			if      ( _T("huhiragana") == wsName ) return 0x3075;
			else if ( _T("huiitosquare") == wsName ) return 0x3333;
			else if ( _T("hukatakana") == wsName ) return 0x30D5;
			else if ( _T("hukatakanahalfwidth") == wsName ) return 0xFF8C;
			else if ( _T("hungarumlaut") == wsName ) return 0x02DD;
			else if ( _T("hungarumlautcmb") == wsName ) return 0x030B;
			break;
		}
	case 'v':
		{
			if      ( _T("hv") == wsName ) return 0x0195;
			break;
		}
	case 'y':
		{
			if      ( _T("hyphen") == wsName ) return 0x002D;
			else if ( _T("hypheninferior") == wsName ) return 0xF6E5;
			else if ( _T("hyphenmonospace") == wsName ) return 0xFF0D;
			else if ( _T("hyphensmall") == wsName ) return 0xFE63;
			else if ( _T("hyphensuperior") == wsName ) return 0xF6E6;
			else if ( _T("hyphentwo") == wsName ) return 0x2010;
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
			if      ( _T("i") == wsName ) return 0x0069;
			break;
		}
	case 'a':
		{
			if      ( _T("iacute") == wsName ) return 0x00ED;
			else if ( _T("iacyrillic") == wsName ) return 0x044F;
			break;
		}
	case 'b':
		{
			if      ( _T("ibengali") == wsName ) return 0x0987;
			else if ( _T("ibopomofo") == wsName ) return 0x3127;
			else if ( _T("ibreve") == wsName ) return 0x012D;
			break;
		}
	case 'c':
		{
			if      ( _T("icaron") == wsName ) return 0x01D0;
			else if ( _T("icircle") == wsName ) return 0x24D8;
			else if ( _T("icircumflex") == wsName ) return 0x00EE;
			else if ( _T("icyrillic") == wsName ) return 0x0456;
			break;
		}
	case 'd':
		{
			if      ( _T("idblgrave") == wsName ) return 0x0209;
			else if ( _T("ideographearthcircle") == wsName ) return 0x328F;
			else if ( _T("ideographfirecircle") == wsName ) return 0x328B;
			else if ( _T("ideographicallianceparen") == wsName ) return 0x323F;
			else if ( _T("ideographiccallparen") == wsName ) return 0x323A;
			else if ( _T("ideographiccentrecircle") == wsName ) return 0x32A5;
			else if ( _T("ideographicclose") == wsName ) return 0x3006;
			else if ( _T("ideographiccomma") == wsName ) return 0x3001;
			else if ( _T("ideographiccommaleft") == wsName ) return 0xFF64;
			else if ( _T("ideographiccongratulationparen") == wsName ) return 0x3237;
			else if ( _T("ideographiccorrectcircle") == wsName ) return 0x32A3;
			else if ( _T("ideographicearthparen") == wsName ) return 0x322F;
			else if ( _T("ideographicenterpriseparen") == wsName ) return 0x323D;
			else if ( _T("ideographicexcellentcircle") == wsName ) return 0x329D;
			else if ( _T("ideographicfestivalparen") == wsName ) return 0x3240;
			else if ( _T("ideographicfinancialcircle") == wsName ) return 0x3296;
			else if ( _T("ideographicfinancialparen") == wsName ) return 0x3236;
			else if ( _T("ideographicfireparen") == wsName ) return 0x322B;
			else if ( _T("ideographichaveparen") == wsName ) return 0x3232;
			else if ( _T("ideographichighcircle") == wsName ) return 0x32A4;
			else if ( _T("ideographiciterationmark") == wsName ) return 0x3005;
			else if ( _T("ideographiclaborcircle") == wsName ) return 0x3298;
			else if ( _T("ideographiclaborparen") == wsName ) return 0x3238;
			else if ( _T("ideographicleftcircle") == wsName ) return 0x32A7;
			else if ( _T("ideographiclowcircle") == wsName ) return 0x32A6;
			else if ( _T("ideographicmedicinecircle") == wsName ) return 0x32A9;
			else if ( _T("ideographicmetalparen") == wsName ) return 0x322E;
			else if ( _T("ideographicmoonparen") == wsName ) return 0x322A;
			else if ( _T("ideographicnameparen") == wsName ) return 0x3234;
			else if ( _T("ideographicperiod") == wsName ) return 0x3002;
			else if ( _T("ideographicprintcircle") == wsName ) return 0x329E;
			else if ( _T("ideographicreachparen") == wsName ) return 0x3243;
			else if ( _T("ideographicrepresentparen") == wsName ) return 0x3239;
			else if ( _T("ideographicresourceparen") == wsName ) return 0x323E;
			else if ( _T("ideographicrightcircle") == wsName ) return 0x32A8;
			else if ( _T("ideographicsecretcircle") == wsName ) return 0x3299;
			else if ( _T("ideographicselfparen") == wsName ) return 0x3242;
			else if ( _T("ideographicsocietyparen") == wsName ) return 0x3233;
			else if ( _T("ideographicspace") == wsName ) return 0x3000;
			else if ( _T("ideographicspecialparen") == wsName ) return 0x3235;
			else if ( _T("ideographicstockparen") == wsName ) return 0x3231;
			else if ( _T("ideographicstudyparen") == wsName ) return 0x323B;
			else if ( _T("ideographicsunparen") == wsName ) return 0x3230;
			else if ( _T("ideographicsuperviseparen") == wsName ) return 0x323C;
			else if ( _T("ideographicwaterparen") == wsName ) return 0x322C;
			else if ( _T("ideographicwoodparen") == wsName ) return 0x322D;
			else if ( _T("ideographiczero") == wsName ) return 0x3007;
			else if ( _T("ideographmetalcircle") == wsName ) return 0x328E;
			else if ( _T("ideographmooncircle") == wsName ) return 0x328A;
			else if ( _T("ideographnamecircle") == wsName ) return 0x3294;
			else if ( _T("ideographsuncircle") == wsName ) return 0x3290;
			else if ( _T("ideographwatercircle") == wsName ) return 0x328C;
			else if ( _T("ideographwoodcircle") == wsName ) return 0x328D;
			else if ( _T("ideva") == wsName ) return 0x0907;
			else if ( _T("idieresis") == wsName ) return 0x00EF;
			else if ( _T("idieresisacute") == wsName ) return 0x1E2F;
			else if ( _T("idieresiscyrillic") == wsName ) return 0x04E5;
			else if ( _T("idotbelow") == wsName ) return 0x1ECB;
			break;
		}
	case 'e':
		{
			if      ( _T("iebrevecyrillic") == wsName ) return 0x04D7;
			else if ( _T("iecyrillic") == wsName ) return 0x0435;
			else if ( _T("ieungacirclekorean") == wsName ) return 0x3275;
			else if ( _T("ieungaparenkorean") == wsName ) return 0x3215;
			else if ( _T("ieungcirclekorean") == wsName ) return 0x3267;
			else if ( _T("ieungkorean") == wsName ) return 0x3147;
			else if ( _T("ieungparenkorean") == wsName ) return 0x3207;
			break;
		}
	case 'g':
		{
			if      ( _T("igrave") == wsName ) return 0x00EC;
			else if ( _T("igujarati") == wsName ) return 0x0A87;
			else if ( _T("igurmukhi") == wsName ) return 0x0A07;
			break;
		}
	case 'h':
		{
			if      ( _T("ihiragana") == wsName ) return 0x3044;
			else if ( _T("ihookabove") == wsName ) return 0x1EC9;
			break;
		}
	case 'i':
		{
			if      ( _T("iibengali") == wsName ) return 0x0988;
			else if ( _T("iicyrillic") == wsName ) return 0x0438;
			else if ( _T("iideva") == wsName ) return 0x0908;
			else if ( _T("iigujarati") == wsName ) return 0x0A88;
			else if ( _T("iigurmukhi") == wsName ) return 0x0A08;
			else if ( _T("iimatragurmukhi") == wsName ) return 0x0A40;
			else if ( _T("iinvertedbreve") == wsName ) return 0x020B;
			else if ( _T("iishortcyrillic") == wsName ) return 0x0439;
			else if ( _T("iivowelsignbengali") == wsName ) return 0x09C0;
			else if ( _T("iivowelsigndeva") == wsName ) return 0x0940;
			else if ( _T("iivowelsigngujarati") == wsName ) return 0x0AC0;
			break;
		}
	case 'j':
		{
			if      ( _T("ij") == wsName ) return 0x0133;
			break;
		}
	case 'k':
		{
			if      ( _T("ikatakana") == wsName ) return 0x30A4;
			else if ( _T("ikatakanahalfwidth") == wsName ) return 0xFF72;
			else if ( _T("ikorean") == wsName ) return 0x3163;
			break;
		}
	case 'l':
		{
			if      ( _T("ilde") == wsName ) return 0x02DC;
			else if ( _T("iluyhebrew") == wsName ) return 0x05AC;
			break;
		}
	case 'm':
		{
			if      ( _T("imacron") == wsName ) return 0x012B;
			else if ( _T("imacroncyrillic") == wsName ) return 0x04E3;
			else if ( _T("imageorapproximatelyequal") == wsName ) return 0x2253;
			else if ( _T("imatragurmukhi") == wsName ) return 0x0A3F;
			else if ( _T("imonospace") == wsName ) return 0xFF49;
			break;
		}
	case 'n':
		{
			if      ( _T("increment") == wsName ) return 0x2206;
			else if ( _T("infinity") == wsName ) return 0x221E;
			else if ( _T("iniarmenian") == wsName ) return 0x056B;
			else if ( _T("integral") == wsName ) return 0x222B;
			else if ( _T("integralbottom") == wsName ) return 0x2321;
			else if ( _T("integralbt") == wsName ) return 0x2321;
			else if ( _T("integralex") == wsName ) return 0xF8F5;
			else if ( _T("integraltop") == wsName ) return 0x2320;
			else if ( _T("integraltp") == wsName ) return 0x2320;
			else if ( _T("intersection") == wsName ) return 0x2229;
			else if ( _T("intisquare") == wsName ) return 0x3305;
			else if ( _T("invbullet") == wsName ) return 0x25D8;
			else if ( _T("invcircle") == wsName ) return 0x25D9;
			else if ( _T("invsmileface") == wsName ) return 0x263B;
			break;
		}
	case 'o':
		{
			if      ( _T("iocyrillic") == wsName ) return 0x0451;
			else if ( _T("iogonek") == wsName ) return 0x012F;
			else if ( _T("iota") == wsName ) return 0x03B9;
			else if ( _T("iotadieresis") == wsName ) return 0x03CA;
			else if ( _T("iotadieresistonos") == wsName ) return 0x0390;
			else if ( _T("iotalatin") == wsName ) return 0x0269;
			else if ( _T("iotatonos") == wsName ) return 0x03AF;
			break;
		}
	case 'p':
		{
			if      ( _T("iparen") == wsName ) return 0x24A4;
			break;
		}
	case 'r':
		{
			if      ( _T("irigurmukhi") == wsName ) return 0x0A72;
			break;
		}
	case 's':
		{
			if      ( _T("ismallhiragana") == wsName ) return 0x3043;
			else if ( _T("ismallkatakana") == wsName ) return 0x30A3;
			else if ( _T("ismallkatakanahalfwidth") == wsName ) return 0xFF68;
			else if ( _T("issharbengali") == wsName ) return 0x09FA;
			else if ( _T("istroke") == wsName ) return 0x0268;
			else if ( _T("isuperior") == wsName ) return 0xF6ED;
			break;
		}
	case 't':
		{
			if      ( _T("iterationhiragana") == wsName ) return 0x309D;
			else if ( _T("iterationkatakana") == wsName ) return 0x30FD;
			else if ( _T("itilde") == wsName ) return 0x0129;
			else if ( _T("itildebelow") == wsName ) return 0x1E2D;
			break;
		}
	case 'u':
		{
			if      ( _T("iubopomofo") == wsName ) return 0x3129;
			else if ( _T("iucyrillic") == wsName ) return 0x044E;
			break;
		}
	case 'v':
		{
			if      ( _T("ivowelsignbengali") == wsName ) return 0x09BF;
			else if ( _T("ivowelsigndeva") == wsName ) return 0x093F;
			else if ( _T("ivowelsigngujarati") == wsName ) return 0x0ABF;
			break;
		}
	case 'z':
		{
			if      ( _T("izhitsacyrillic") == wsName ) return 0x0475;
			else if ( _T("izhitsadblgravecyrillic") == wsName ) return 0x0477;
			break;
		}
	}

	return 0;
}
static int Type1NameToUnicode_j(CWCharWrapper &wsName)
{
	if      ( _T("j") == wsName ) return 0x006A;
	else if ( _T("jaarmenian") == wsName ) return 0x0571;
	else if ( _T("jabengali") == wsName ) return 0x099C;
	else if ( _T("jadeva") == wsName ) return 0x091C;
	else if ( _T("jagujarati") == wsName ) return 0x0A9C;
	else if ( _T("jagurmukhi") == wsName ) return 0x0A1C;
	else if ( _T("jbopomofo") == wsName ) return 0x3110;
	else if ( _T("jcaron") == wsName ) return 0x01F0;
	else if ( _T("jcircle") == wsName ) return 0x24D9;
	else if ( _T("jcircumflex") == wsName ) return 0x0135;
	else if ( _T("jcrossedtail") == wsName ) return 0x029D;
	else if ( _T("jdotlessstroke") == wsName ) return 0x025F;
	else if ( _T("jecyrillic") == wsName ) return 0x0458;
	else if ( _T("jeemarabic") == wsName ) return 0x062C;
	else if ( _T("jeemfinalarabic") == wsName ) return 0xFE9E;
	else if ( _T("jeeminitialarabic") == wsName ) return 0xFE9F;
	else if ( _T("jeemmedialarabic") == wsName ) return 0xFEA0;
	else if ( _T("jeharabic") == wsName ) return 0x0698;
	else if ( _T("jehfinalarabic") == wsName ) return 0xFB8B;
	else if ( _T("jhabengali") == wsName ) return 0x099D;
	else if ( _T("jhadeva") == wsName ) return 0x091D;
	else if ( _T("jhagujarati") == wsName ) return 0x0A9D;
	else if ( _T("jhagurmukhi") == wsName ) return 0x0A1D;
	else if ( _T("jheharmenian") == wsName ) return 0x057B;
	else if ( _T("jis") == wsName ) return 0x3004;
	else if ( _T("jmonospace") == wsName ) return 0xFF4A;
	else if ( _T("jparen") == wsName ) return 0x24A5;
	else if ( _T("jsuperior") == wsName ) return 0x02B2;

	return 0;
}
static int Type1NameToUnicode_k(CWCharWrapper &wsName)
{
	if      ( _T("k") == wsName ) return 0x006B;
	else if ( _T("kabashkircyrillic") == wsName ) return 0x04A1;
	else if ( _T("kabengali") == wsName ) return 0x0995;
	else if ( _T("kacute") == wsName ) return 0x1E31;
	else if ( _T("kacyrillic") == wsName ) return 0x043A;
	else if ( _T("kadescendercyrillic") == wsName ) return 0x049B;
	else if ( _T("kadeva") == wsName ) return 0x0915;
	else if ( _T("kaf") == wsName ) return 0x05DB;
	else if ( _T("kafarabic") == wsName ) return 0x0643;
	else if ( _T("kafdagesh") == wsName ) return 0xFB3B;
	else if ( _T("kafdageshhebrew") == wsName ) return 0xFB3B;
	else if ( _T("kaffinalarabic") == wsName ) return 0xFEDA;
	else if ( _T("kafhebrew") == wsName ) return 0x05DB;
	else if ( _T("kafinitialarabic") == wsName ) return 0xFEDB;
	else if ( _T("kafmedialarabic") == wsName ) return 0xFEDC;
	else if ( _T("kafrafehebrew") == wsName ) return 0xFB4D;
	else if ( _T("kagujarati") == wsName ) return 0x0A95;
	else if ( _T("kagurmukhi") == wsName ) return 0x0A15;
	else if ( _T("kahiragana") == wsName ) return 0x304B;
	else if ( _T("kahookcyrillic") == wsName ) return 0x04C4;
	else if ( _T("kakatakana") == wsName ) return 0x30AB;
	else if ( _T("kakatakanahalfwidth") == wsName ) return 0xFF76;
	else if ( _T("kappa") == wsName ) return 0x03BA;
	else if ( _T("kappasymbolgreek") == wsName ) return 0x03F0;
	else if ( _T("kapyeounmieumkorean") == wsName ) return 0x3171;
	else if ( _T("kapyeounphieuphkorean") == wsName ) return 0x3184;
	else if ( _T("kapyeounpieupkorean") == wsName ) return 0x3178;
	else if ( _T("kapyeounssangpieupkorean") == wsName ) return 0x3179;
	else if ( _T("karoriisquare") == wsName ) return 0x330D;
	else if ( _T("kashidaautoarabic") == wsName ) return 0x0640;
	else if ( _T("kashidaautonosidebearingarabic") == wsName ) return 0x0640;
	else if ( _T("kasmallkatakana") == wsName ) return 0x30F5;
	else if ( _T("kasquare") == wsName ) return 0x3384;
	else if ( _T("kasraarabic") == wsName ) return 0x0650;
	else if ( _T("kasratanarabic") == wsName ) return 0x064D;
	else if ( _T("kastrokecyrillic") == wsName ) return 0x049F;
	else if ( _T("katahiraprolongmarkhalfwidth") == wsName ) return 0xFF70;
	else if ( _T("kaverticalstrokecyrillic") == wsName ) return 0x049D;
	else if ( _T("kbopomofo") == wsName ) return 0x310E;
	else if ( _T("kcalsquare") == wsName ) return 0x3389;
	else if ( _T("kcaron") == wsName ) return 0x01E9;
	else if ( _T("kcedilla") == wsName ) return 0x0137;
	else if ( _T("kcircle") == wsName ) return 0x24DA;
	else if ( _T("kcommaaccent") == wsName ) return 0x0137;
	else if ( _T("kdotbelow") == wsName ) return 0x1E33;
	else if ( _T("keharmenian") == wsName ) return 0x0584;
	else if ( _T("kehiragana") == wsName ) return 0x3051;
	else if ( _T("kekatakana") == wsName ) return 0x30B1;
	else if ( _T("kekatakanahalfwidth") == wsName ) return 0xFF79;
	else if ( _T("kenarmenian") == wsName ) return 0x056F;
	else if ( _T("kesmallkatakana") == wsName ) return 0x30F6;
	else if ( _T("kgreenlandic") == wsName ) return 0x0138;
	else if ( _T("khabengali") == wsName ) return 0x0996;
	else if ( _T("khacyrillic") == wsName ) return 0x0445;
	else if ( _T("khadeva") == wsName ) return 0x0916;
	else if ( _T("khagujarati") == wsName ) return 0x0A96;
	else if ( _T("khagurmukhi") == wsName ) return 0x0A16;
	else if ( _T("khaharabic") == wsName ) return 0x062E;
	else if ( _T("khahfinalarabic") == wsName ) return 0xFEA6;
	else if ( _T("khahinitialarabic") == wsName ) return 0xFEA7;
	else if ( _T("khahmedialarabic") == wsName ) return 0xFEA8;
	else if ( _T("kheicoptic") == wsName ) return 0x03E7;
	else if ( _T("khhadeva") == wsName ) return 0x0959;
	else if ( _T("khhagurmukhi") == wsName ) return 0x0A59;
	else if ( _T("khieukhacirclekorean") == wsName ) return 0x3278;
	else if ( _T("khieukhaparenkorean") == wsName ) return 0x3218;
	else if ( _T("khieukhcirclekorean") == wsName ) return 0x326A;
	else if ( _T("khieukhkorean") == wsName ) return 0x314B;
	else if ( _T("khieukhparenkorean") == wsName ) return 0x320A;
	else if ( _T("khokhaithai") == wsName ) return 0x0E02;
	else if ( _T("khokhonthai") == wsName ) return 0x0E05;
	else if ( _T("khokhuatthai") == wsName ) return 0x0E03;
	else if ( _T("khokhwaithai") == wsName ) return 0x0E04;
	else if ( _T("khomutthai") == wsName ) return 0x0E5B;
	else if ( _T("khook") == wsName ) return 0x0199;
	else if ( _T("khorakhangthai") == wsName ) return 0x0E06;
	else if ( _T("khzsquare") == wsName ) return 0x3391;
	else if ( _T("kihiragana") == wsName ) return 0x304D;
	else if ( _T("kikatakana") == wsName ) return 0x30AD;
	else if ( _T("kikatakanahalfwidth") == wsName ) return 0xFF77;
	else if ( _T("kiroguramusquare") == wsName ) return 0x3315;
	else if ( _T("kiromeetorusquare") == wsName ) return 0x3316;
	else if ( _T("kirosquare") == wsName ) return 0x3314;
	else if ( _T("kiyeokacirclekorean") == wsName ) return 0x326E;
	else if ( _T("kiyeokaparenkorean") == wsName ) return 0x320E;
	else if ( _T("kiyeokcirclekorean") == wsName ) return 0x3260;
	else if ( _T("kiyeokkorean") == wsName ) return 0x3131;
	else if ( _T("kiyeokparenkorean") == wsName ) return 0x3200;
	else if ( _T("kiyeoksioskorean") == wsName ) return 0x3133;
	else if ( _T("kjecyrillic") == wsName ) return 0x045C;
	else if ( _T("klinebelow") == wsName ) return 0x1E35;
	else if ( _T("klsquare") == wsName ) return 0x3398;
	else if ( _T("kmcubedsquare") == wsName ) return 0x33A6;
	else if ( _T("kmonospace") == wsName ) return 0xFF4B;
	else if ( _T("kmsquaredsquare") == wsName ) return 0x33A2;
	else if ( _T("kohiragana") == wsName ) return 0x3053;
	else if ( _T("kohmsquare") == wsName ) return 0x33C0;
	else if ( _T("kokaithai") == wsName ) return 0x0E01;
	else if ( _T("kokatakana") == wsName ) return 0x30B3;
	else if ( _T("kokatakanahalfwidth") == wsName ) return 0xFF7A;
	else if ( _T("kooposquare") == wsName ) return 0x331E;
	else if ( _T("koppacyrillic") == wsName ) return 0x0481;
	else if ( _T("koreanstandardsymbol") == wsName ) return 0x327F;
	else if ( _T("koroniscmb") == wsName ) return 0x0343;
	else if ( _T("kparen") == wsName ) return 0x24A6;
	else if ( _T("kpasquare") == wsName ) return 0x33AA;
	else if ( _T("ksicyrillic") == wsName ) return 0x046F;
	else if ( _T("ktsquare") == wsName ) return 0x33CF;
	else if ( _T("kturned") == wsName ) return 0x029E;
	else if ( _T("kuhiragana") == wsName ) return 0x304F;
	else if ( _T("kukatakana") == wsName ) return 0x30AF;
	else if ( _T("kukatakanahalfwidth") == wsName ) return 0xFF78;
	else if ( _T("kvsquare") == wsName ) return 0x33B8;
	else if ( _T("kwsquare") == wsName ) return 0x33BE;

	return 0;
}
static int Type1NameToUnicode_l(CWCharWrapper &wsName)
{
	if      ( _T("l") == wsName ) return 0x006C;
	else if ( _T("labengali") == wsName ) return 0x09B2;
	else if ( _T("lacute") == wsName ) return 0x013A;
	else if ( _T("ladeva") == wsName ) return 0x0932;
	else if ( _T("lagujarati") == wsName ) return 0x0AB2;
	else if ( _T("lagurmukhi") == wsName ) return 0x0A32;
	else if ( _T("lakkhangyaothai") == wsName ) return 0x0E45;
	else if ( _T("lamaleffinalarabic") == wsName ) return 0xFEFC;
	else if ( _T("lamalefhamzaabovefinalarabic") == wsName ) return 0xFEF8;
	else if ( _T("lamalefhamzaaboveisolatedarabic") == wsName ) return 0xFEF7;
	else if ( _T("lamalefhamzabelowfinalarabic") == wsName ) return 0xFEFA;
	else if ( _T("lamalefhamzabelowisolatedarabic") == wsName ) return 0xFEF9;
	else if ( _T("lamalefisolatedarabic") == wsName ) return 0xFEFB;
	else if ( _T("lamalefmaddaabovefinalarabic") == wsName ) return 0xFEF6;
	else if ( _T("lamalefmaddaaboveisolatedarabic") == wsName ) return 0xFEF5;
	else if ( _T("lamarabic") == wsName ) return 0x0644;
	else if ( _T("lambda") == wsName ) return 0x03BB;
	else if ( _T("lambdastroke") == wsName ) return 0x019B;
	else if ( _T("lamed") == wsName ) return 0x05DC;
	else if ( _T("lameddagesh") == wsName ) return 0xFB3C;
	else if ( _T("lameddageshhebrew") == wsName ) return 0xFB3C;
	else if ( _T("lamedhebrew") == wsName ) return 0x05DC;
	else if ( _T("lamedholam") == wsName ) return 0x05DC05B9;
	// "lamedholamdagesh"  "05DC 05B9 05BC";
	// "lamedholamdageshhebrew"  "05DC 05B9 05BC";
	else if ( _T("lamedholamhebrew") == wsName ) return 0x05DC05B9;
	else if ( _T("lamfinalarabic") == wsName ) return 0xFEDE;
	else if ( _T("lamhahinitialarabic") == wsName ) return 0xFCCA;
	else if ( _T("laminitialarabic") == wsName ) return 0xFEDF;
	else if ( _T("lamjeeminitialarabic") == wsName ) return 0xFCC9;
	else if ( _T("lamkhahinitialarabic") == wsName ) return 0xFCCB;
	else if ( _T("lamlamhehisolatedarabic") == wsName ) return 0xFDF2;
	else if ( _T("lammedialarabic") == wsName ) return 0xFEE0;
	else if ( _T("lammeemhahinitialarabic") == wsName ) return 0xFD88;
	else if ( _T("lammeeminitialarabic") == wsName ) return 0xFCCC;
	// "lammeemjeeminitialarabic" "FEDF FEE4 FEA0";
	// "lammeemkhahinitialarabic" "FEDF FEE4 FEA8";
	else if ( _T("largecircle") == wsName ) return 0x25EF;
	else if ( _T("lbar") == wsName ) return 0x019A;
	else if ( _T("lbelt") == wsName ) return 0x026C;
	else if ( _T("lbopomofo") == wsName ) return 0x310C;
	else if ( _T("lcaron") == wsName ) return 0x013E;
	else if ( _T("lcedilla") == wsName ) return 0x013C;
	else if ( _T("lcircle") == wsName ) return 0x24DB;
	else if ( _T("lcircumflexbelow") == wsName ) return 0x1E3D;
	else if ( _T("lcommaaccent") == wsName ) return 0x013C;
	else if ( _T("ldot") == wsName ) return 0x0140;
	else if ( _T("ldotaccent") == wsName ) return 0x0140;
	else if ( _T("ldotbelow") == wsName ) return 0x1E37;
	else if ( _T("ldotbelowmacron") == wsName ) return 0x1E39;
	else if ( _T("leftangleabovecmb") == wsName ) return 0x031A;
	else if ( _T("lefttackbelowcmb") == wsName ) return 0x0318;
	else if ( _T("less") == wsName ) return 0x003C;
	else if ( _T("lessequal") == wsName ) return 0x2264;
	else if ( _T("lessequalorgreater") == wsName ) return 0x22DA;
	else if ( _T("lessmonospace") == wsName ) return 0xFF1C;
	else if ( _T("lessorequivalent") == wsName ) return 0x2272;
	else if ( _T("lessorgreater") == wsName ) return 0x2276;
	else if ( _T("lessoverequal") == wsName ) return 0x2266;
	else if ( _T("lesssmall") == wsName ) return 0xFE64;
	else if ( _T("lezh") == wsName ) return 0x026E;
	else if ( _T("lfblock") == wsName ) return 0x258C;
	else if ( _T("lhookretroflex") == wsName ) return 0x026D;
	else if ( _T("lira") == wsName ) return 0x20A4;
	else if ( _T("liwnarmenian") == wsName ) return 0x056C;
	else if ( _T("lj") == wsName ) return 0x01C9;
	else if ( _T("ljecyrillic") == wsName ) return 0x0459;
	else if ( _T("ll") == wsName ) return 0xF6C0;
	else if ( _T("lladeva") == wsName ) return 0x0933;
	else if ( _T("llagujarati") == wsName ) return 0x0AB3;
	else if ( _T("llinebelow") == wsName ) return 0x1E3B;
	else if ( _T("llladeva") == wsName ) return 0x0934;
	else if ( _T("llvocalicbengali") == wsName ) return 0x09E1;
	else if ( _T("llvocalicdeva") == wsName ) return 0x0961;
	else if ( _T("llvocalicvowelsignbengali") == wsName ) return 0x09E3;
	else if ( _T("llvocalicvowelsigndeva") == wsName ) return 0x0963;
	else if ( _T("lmiddletilde") == wsName ) return 0x026B;
	else if ( _T("lmonospace") == wsName ) return 0xFF4C;
	else if ( _T("lmsquare") == wsName ) return 0x33D0;
	else if ( _T("lochulathai") == wsName ) return 0x0E2C;
	else if ( _T("logicaland") == wsName ) return 0x2227;
	else if ( _T("logicalnot") == wsName ) return 0x00AC;
	else if ( _T("logicalnotreversed") == wsName ) return 0x2310;
	else if ( _T("logicalor") == wsName ) return 0x2228;
	else if ( _T("lolingthai") == wsName ) return 0x0E25;
	else if ( _T("longs") == wsName ) return 0x017F;
	else if ( _T("lowlinecenterline") == wsName ) return 0xFE4E;
	else if ( _T("lowlinecmb") == wsName ) return 0x0332;
	else if ( _T("lowlinedashed") == wsName ) return 0xFE4D;
	else if ( _T("lozenge") == wsName ) return 0x25CA;
	else if ( _T("lparen") == wsName ) return 0x24A7;
	else if ( _T("lslash") == wsName ) return 0x0142;
	else if ( _T("lsquare") == wsName ) return 0x2113;
	else if ( _T("lsuperior") == wsName ) return 0xF6EE;
	else if ( _T("ltshade") == wsName ) return 0x2591;
	else if ( _T("luthai") == wsName ) return 0x0E26;
	else if ( _T("lvocalicbengali") == wsName ) return 0x098C;
	else if ( _T("lvocalicdeva") == wsName ) return 0x090C;
	else if ( _T("lvocalicvowelsignbengali") == wsName ) return 0x09E2;
	else if ( _T("lvocalicvowelsigndeva") == wsName ) return 0x0962;
	else if ( _T("lxsquare") == wsName ) return 0x33D3;

	return 0;
}
static int Type1NameToUnicode_m(CWCharWrapper &wsName)
{
	wchar_t wChar1 = wsName[1];
	switch( wChar1 )
	{
	case 0:
		{
			if      ( _T("m") == wsName ) return 0x006D;
			break;
		}
	case 'a':
		{
			if      ( _T("mabengali") == wsName ) return 0x09AE;
			else if ( _T("macron") == wsName ) return 0x00AF;
			else if ( _T("macronbelowcmb") == wsName ) return 0x0331;
			else if ( _T("macroncmb") == wsName ) return 0x0304;
			else if ( _T("macronlowmod") == wsName ) return 0x02CD;
			else if ( _T("macronmonospace") == wsName ) return 0xFFE3;
			else if ( _T("macute") == wsName ) return 0x1E3F;
			else if ( _T("madeva") == wsName ) return 0x092E;
			else if ( _T("magujarati") == wsName ) return 0x0AAE;
			else if ( _T("magurmukhi") == wsName ) return 0x0A2E;
			else if ( _T("mahapakhhebrew") == wsName ) return 0x05A4;
			else if ( _T("mahapakhlefthebrew") == wsName ) return 0x05A4;
			else if ( _T("mahiragana") == wsName ) return 0x307E;
			else if ( _T("maichattawalowleftthai") == wsName ) return 0xF895;
			else if ( _T("maichattawalowrightthai") == wsName ) return 0xF894;
			else if ( _T("maichattawathai") == wsName ) return 0x0E4B;
			else if ( _T("maichattawaupperleftthai") == wsName ) return 0xF893;
			else if ( _T("maieklowleftthai") == wsName ) return 0xF88C;
			else if ( _T("maieklowrightthai") == wsName ) return 0xF88B;
			else if ( _T("maiekthai") == wsName ) return 0x0E48;
			else if ( _T("maiekupperleftthai") == wsName ) return 0xF88A;
			else if ( _T("maihanakatleftthai") == wsName ) return 0xF884;
			else if ( _T("maihanakatthai") == wsName ) return 0x0E31;
			else if ( _T("maitaikhuleftthai") == wsName ) return 0xF889;
			else if ( _T("maitaikhuthai") == wsName ) return 0x0E47;
			else if ( _T("maitholowleftthai") == wsName ) return 0xF88F;
			else if ( _T("maitholowrightthai") == wsName ) return 0xF88E;
			else if ( _T("maithothai") == wsName ) return 0x0E49;
			else if ( _T("maithoupperleftthai") == wsName ) return 0xF88D;
			else if ( _T("maitrilowleftthai") == wsName ) return 0xF892;
			else if ( _T("maitrilowrightthai") == wsName ) return 0xF891;
			else if ( _T("maitrithai") == wsName ) return 0x0E4A;
			else if ( _T("maitriupperleftthai") == wsName ) return 0xF890;
			else if ( _T("maiyamokthai") == wsName ) return 0x0E46;
			else if ( _T("makatakana") == wsName ) return 0x30DE;
			else if ( _T("makatakanahalfwidth") == wsName ) return 0xFF8F;
			else if ( _T("male") == wsName ) return 0x2642;
			else if ( _T("mansyonsquare") == wsName ) return 0x3347;
			else if ( _T("maqafhebrew") == wsName ) return 0x05BE;
			else if ( _T("mars") == wsName ) return 0x2642;
			else if ( _T("masoracirclehebrew") == wsName ) return 0x05AF;
			else if ( _T("masquare") == wsName ) return 0x3383;
			break;
		}
	case 'b':
		{
			if      ( _T("mbopomofo") == wsName ) return 0x3107;
			else if ( _T("mbsquare") == wsName ) return 0x33D4;
			break;
		}
	case 'c':
		{
			if      ( _T("mcircle") == wsName ) return 0x24DC;
			else if ( _T("mcubedsquare") == wsName ) return 0x33A5;
			break;
		}
	case 'd':
		{
			if      ( _T("mdotaccent") == wsName ) return 0x1E41;
			else if ( _T("mdotbelow") == wsName ) return 0x1E43;
			break;
		}
	case 'e':
		{
			if      ( _T("meemarabic") == wsName ) return 0x0645;
			else if ( _T("meemfinalarabic") == wsName ) return 0xFEE2;
			else if ( _T("meeminitialarabic") == wsName ) return 0xFEE3;
			else if ( _T("meemmedialarabic") == wsName ) return 0xFEE4;
			else if ( _T("meemmeeminitialarabic") == wsName ) return 0xFCD1;
			else if ( _T("meemmeemisolatedarabic") == wsName ) return 0xFC48;
			else if ( _T("meetorusquare") == wsName ) return 0x334D;
			else if ( _T("mehiragana") == wsName ) return 0x3081;
			else if ( _T("meizierasquare") == wsName ) return 0x337E;
			else if ( _T("mekatakana") == wsName ) return 0x30E1;
			else if ( _T("mekatakanahalfwidth") == wsName ) return 0xFF92;
			else if ( _T("mem") == wsName ) return 0x05DE;
			else if ( _T("memdagesh") == wsName ) return 0xFB3E;
			else if ( _T("memdageshhebrew") == wsName ) return 0xFB3E;
			else if ( _T("memhebrew") == wsName ) return 0x05DE;
			else if ( _T("menarmenian") == wsName ) return 0x0574;
			else if ( _T("merkhahebrew") == wsName ) return 0x05A5;
			else if ( _T("merkhakefulahebrew") == wsName ) return 0x05A6;
			else if ( _T("merkhakefulalefthebrew") == wsName ) return 0x05A6;
			else if ( _T("merkhalefthebrew") == wsName ) return 0x05A5;
			break;
		}
	case 'h':
		{
			if      ( _T("mhook") == wsName ) return 0x0271;
			else if ( _T("mhzsquare") == wsName ) return 0x3392;
			break;
		}
	case 'i':
		{
			if      ( _T("middledotkatakanahalfwidth") == wsName ) return 0xFF65;
			else if ( _T("middot") == wsName ) return 0x00B7;
			else if ( _T("mieumacirclekorean") == wsName ) return 0x3272;
			else if ( _T("mieumaparenkorean") == wsName ) return 0x3212;
			else if ( _T("mieumcirclekorean") == wsName ) return 0x3264;
			else if ( _T("mieumkorean") == wsName ) return 0x3141;
			else if ( _T("mieumpansioskorean") == wsName ) return 0x3170;
			else if ( _T("mieumparenkorean") == wsName ) return 0x3204;
			else if ( _T("mieumpieupkorean") == wsName ) return 0x316E;
			else if ( _T("mieumsioskorean") == wsName ) return 0x316F;
			else if ( _T("mihiragana") == wsName ) return 0x307F;
			else if ( _T("mikatakana") == wsName ) return 0x30DF;
			else if ( _T("mikatakanahalfwidth") == wsName ) return 0xFF90;
			else if ( _T("minus") == wsName ) return 0x2212;
			else if ( _T("minusbelowcmb") == wsName ) return 0x0320;
			else if ( _T("minuscircle") == wsName ) return 0x2296;
			else if ( _T("minusmod") == wsName ) return 0x02D7;
			else if ( _T("minusplus") == wsName ) return 0x2213;
			else if ( _T("minute") == wsName ) return 0x2032;
			else if ( _T("miribaarusquare") == wsName ) return 0x334A;
			else if ( _T("mirisquare") == wsName ) return 0x3349;
			break;
		}
	case 'l':
		{
			if      ( _T("mlonglegturned") == wsName ) return 0x0270;
			else if ( _T("mlsquare") == wsName ) return 0x3396;
			break;
		}
	case 'm':
		{
			if      ( _T("mmcubedsquare") == wsName ) return 0x33A3;
			else if ( _T("mmonospace") == wsName ) return 0xFF4D;
			else if ( _T("mmsquaredsquare") == wsName ) return 0x339F;
			break;
		}
	case 'o':
		{
			if      ( _T("mohiragana") == wsName ) return 0x3082;
			else if ( _T("mohmsquare") == wsName ) return 0x33C1;
			else if ( _T("mokatakana") == wsName ) return 0x30E2;
			else if ( _T("mokatakanahalfwidth") == wsName ) return 0xFF93;
			else if ( _T("molsquare") == wsName ) return 0x33D6;
			else if ( _T("momathai") == wsName ) return 0x0E21;
			else if ( _T("moverssquare") == wsName ) return 0x33A7;
			else if ( _T("moverssquaredsquare") == wsName ) return 0x33A8;
			break;
		}
	case 'p':
		{
			if      ( _T("mparen") == wsName ) return 0x24A8;
			else if ( _T("mpasquare") == wsName ) return 0x33AB;
			break;
		}
	case 's':
		{
			if      ( _T("mssquare") == wsName ) return 0x33B3;
			else if ( _T("msuperior") == wsName ) return 0xF6EF;
			break;
		}
	case 't':
		{
			if      ( _T("mturned") == wsName ) return 0x026F;
			break;
		}
	case 'u':
		{
			if      ( _T("mu") == wsName ) return 0x00B5;
			else if ( _T("mu1") == wsName ) return 0x00B5;
			else if ( _T("muasquare") == wsName ) return 0x3382;
			else if ( _T("muchgreater") == wsName ) return 0x226B;
			else if ( _T("muchless") == wsName ) return 0x226A;
			else if ( _T("mufsquare") == wsName ) return 0x338C;
			else if ( _T("mugreek") == wsName ) return 0x03BC;
			else if ( _T("mugsquare") == wsName ) return 0x338D;
			else if ( _T("muhiragana") == wsName ) return 0x3080;
			else if ( _T("mukatakana") == wsName ) return 0x30E0;
			else if ( _T("mukatakanahalfwidth") == wsName ) return 0xFF91;
			else if ( _T("mulsquare") == wsName ) return 0x3395;
			else if ( _T("multiply") == wsName ) return 0x00D7;
			else if ( _T("mumsquare") == wsName ) return 0x339B;
			else if ( _T("munahhebrew") == wsName ) return 0x05A3;
			else if ( _T("munahlefthebrew") == wsName ) return 0x05A3;
			else if ( _T("musicalnote") == wsName ) return 0x266A;
			else if ( _T("musicalnotedbl") == wsName ) return 0x266B;
			else if ( _T("musicflatsign") == wsName ) return 0x266D;
			else if ( _T("musicsharpsign") == wsName ) return 0x266F;
			else if ( _T("mussquare") == wsName ) return 0x33B2;
			else if ( _T("muvsquare") == wsName ) return 0x33B6;
			else if ( _T("muwsquare") == wsName ) return 0x33BC;
			break;
		}
	case 'v':
		{
			if      ( _T("mvmegasquare") == wsName ) return 0x33B9;
			else if ( _T("mvsquare") == wsName ) return 0x33B7;
			break;
		}
	case 'w':
		{
			if      ( _T("mwmegasquare") == wsName ) return 0x33BF;
			else if ( _T("mwsquare") == wsName ) return 0x33BD;
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
			if      ( _T("n") == wsName ) return 0x006E;
			break;
		}
	case 'a':
		{
			if      ( _T("nabengali") == wsName ) return 0x09A8;
			else if ( _T("nabla") == wsName ) return 0x2207;
			else if ( _T("nacute") == wsName ) return 0x0144;
			else if ( _T("nadeva") == wsName ) return 0x0928;
			else if ( _T("nagujarati") == wsName ) return 0x0AA8;
			else if ( _T("nagurmukhi") == wsName ) return 0x0A28;
			else if ( _T("nahiragana") == wsName ) return 0x306A;
			else if ( _T("nakatakana") == wsName ) return 0x30CA;
			else if ( _T("nakatakanahalfwidth") == wsName ) return 0xFF85;
			else if ( _T("napostrophe") == wsName ) return 0x0149;
			else if ( _T("nasquare") == wsName ) return 0x3381;
			break;
		}
	case 'b':
		{
			if      ( _T("nbopomofo") == wsName ) return 0x310B;
			else if ( _T("nbspace") == wsName ) return 0x00A0;
			break;
		}
	case 'c':
		{
			if      ( _T("ncaron") == wsName ) return 0x0148;
			else if ( _T("ncedilla") == wsName ) return 0x0146;
			else if ( _T("ncircle") == wsName ) return 0x24DD;
			else if ( _T("ncircumflexbelow") == wsName ) return 0x1E4B;
			else if ( _T("ncommaaccent") == wsName ) return 0x0146;
			break;
		}
	case 'd':
		{
			if      ( _T("ndotaccent") == wsName ) return 0x1E45;
			else if ( _T("ndotbelow") == wsName ) return 0x1E47;
			break;
		}
	case 'e':
		{
			if      ( _T("nehiragana") == wsName ) return 0x306D;
			else if ( _T("nekatakana") == wsName ) return 0x30CD;
			else if ( _T("nekatakanahalfwidth") == wsName ) return 0xFF88;
			else if ( _T("newsheqelsign") == wsName ) return 0x20AA;
			break;
		}
	case 'f':
		{
			if      ( _T("nfsquare") == wsName ) return 0x338B;
			break;
		}
	case 'g':
		{
			if      ( _T("ngabengali") == wsName ) return 0x0999;
			else if ( _T("ngadeva") == wsName ) return 0x0919;
			else if ( _T("ngagujarati") == wsName ) return 0x0A99;
			else if ( _T("ngagurmukhi") == wsName ) return 0x0A19;
			else if ( _T("ngonguthai") == wsName ) return 0x0E07;
			break;
		}
	case 'h':
		{
			if      ( _T("nhiragana") == wsName ) return 0x3093;
			else if ( _T("nhookleft") == wsName ) return 0x0272;
			else if ( _T("nhookretroflex") == wsName ) return 0x0273;
			break;
		}
	case 'i':
		{
			if      ( _T("nieunacirclekorean") == wsName ) return 0x326F;
			else if ( _T("nieunaparenkorean") == wsName ) return 0x320F;
			else if ( _T("nieuncieuckorean") == wsName ) return 0x3135;
			else if ( _T("nieuncirclekorean") == wsName ) return 0x3261;
			else if ( _T("nieunhieuhkorean") == wsName ) return 0x3136;
			else if ( _T("nieunkorean") == wsName ) return 0x3134;
			else if ( _T("nieunpansioskorean") == wsName ) return 0x3168;
			else if ( _T("nieunparenkorean") == wsName ) return 0x3201;
			else if ( _T("nieunsioskorean") == wsName ) return 0x3167;
			else if ( _T("nieuntikeutkorean") == wsName ) return 0x3166;
			else if ( _T("nihiragana") == wsName ) return 0x306B;
			else if ( _T("nikatakana") == wsName ) return 0x30CB;
			else if ( _T("nikatakanahalfwidth") == wsName ) return 0xFF86;
			else if ( _T("nikhahitleftthai") == wsName ) return 0xF899;
			else if ( _T("nikhahitthai") == wsName ) return 0x0E4D;
			else if ( _T("nine") == wsName ) return 0x0039;
			else if ( _T("ninearabic") == wsName ) return 0x0669;
			else if ( _T("ninebengali") == wsName ) return 0x09EF;
			else if ( _T("ninecircle") == wsName ) return 0x2468;
			else if ( _T("ninecircleinversesansserif") == wsName ) return 0x2792;
			else if ( _T("ninedeva") == wsName ) return 0x096F;
			else if ( _T("ninegujarati") == wsName ) return 0x0AEF;
			else if ( _T("ninegurmukhi") == wsName ) return 0x0A6F;
			else if ( _T("ninehackarabic") == wsName ) return 0x0669;
			else if ( _T("ninehangzhou") == wsName ) return 0x3029;
			else if ( _T("nineideographicparen") == wsName ) return 0x3228;
			else if ( _T("nineinferior") == wsName ) return 0x2089;
			else if ( _T("ninemonospace") == wsName ) return 0xFF19;
			else if ( _T("nineoldstyle") == wsName ) return 0xF739;
			else if ( _T("nineparen") == wsName ) return 0x247C;
			else if ( _T("nineperiod") == wsName ) return 0x2490;
			else if ( _T("ninepersian") == wsName ) return 0x06F9;
			else if ( _T("nineroman") == wsName ) return 0x2178;
			else if ( _T("ninesuperior") == wsName ) return 0x2079;
			else if ( _T("nineteencircle") == wsName ) return 0x2472;
			else if ( _T("nineteenparen") == wsName ) return 0x2486;
			else if ( _T("nineteenperiod") == wsName ) return 0x249A;
			else if ( _T("ninethai") == wsName ) return 0x0E59;
			break;
		}
	case 'j':
		{
			if      ( _T("nj") == wsName ) return 0x01CC;
			else if ( _T("njecyrillic") == wsName ) return 0x045A;
			break;
		}
	case 'k':
		{
			if      ( _T("nkatakana") == wsName ) return 0x30F3;
			else if ( _T("nkatakanahalfwidth") == wsName ) return 0xFF9D;
			break;
		}
	case 'l':
		{
			if      ( _T("nlegrightlong") == wsName ) return 0x019E;
			else if ( _T("nlinebelow") == wsName ) return 0x1E49;
			break;
		}
	case 'm':
		{
			if      ( _T("nmonospace") == wsName ) return 0xFF4E;
			else if ( _T("nmsquare") == wsName ) return 0x339A;
			break;
		}
	case 'n':
		{
			if      ( _T("nnabengali") == wsName ) return 0x09A3;
			else if ( _T("nnadeva") == wsName ) return 0x0923;
			else if ( _T("nnagujarati") == wsName ) return 0x0AA3;
			else if ( _T("nnagurmukhi") == wsName ) return 0x0A23;
			else if ( _T("nnnadeva") == wsName ) return 0x0929;
			break;
		}
	case 'o':
		{
			if      ( _T("nohiragana") == wsName ) return 0x306E;
			else if ( _T("nokatakana") == wsName ) return 0x30CE;
			else if ( _T("nokatakanahalfwidth") == wsName ) return 0xFF89;
			else if ( _T("nonbreakingspace") == wsName ) return 0x00A0;
			else if ( _T("nonenthai") == wsName ) return 0x0E13;
			else if ( _T("nonuthai") == wsName ) return 0x0E19;
			else if ( _T("noonarabic") == wsName ) return 0x0646;
			else if ( _T("noonfinalarabic") == wsName ) return 0xFEE6;
			else if ( _T("noonghunnaarabic") == wsName ) return 0x06BA;
			else if ( _T("noonghunnafinalarabic") == wsName ) return 0xFB9F;
			else if ( _T("noonhehinitialarabic") == wsName ) return 0xFEE7FEEC;
			else if ( _T("nooninitialarabic") == wsName ) return 0xFEE7;
			else if ( _T("noonjeeminitialarabic") == wsName ) return 0xFCD2;
			else if ( _T("noonjeemisolatedarabic") == wsName ) return 0xFC4B;
			else if ( _T("noonmedialarabic") == wsName ) return 0xFEE8;
			else if ( _T("noonmeeminitialarabic") == wsName ) return 0xFCD5;
			else if ( _T("noonmeemisolatedarabic") == wsName ) return 0xFC4E;
			else if ( _T("noonnoonfinalarabic") == wsName ) return 0xFC8D;
			else if ( _T("notcontains") == wsName ) return 0x220C;
			else if ( _T("notelement") == wsName ) return 0x2209;
			else if ( _T("notelementof") == wsName ) return 0x2209;
			else if ( _T("notequal") == wsName ) return 0x2260;
			else if ( _T("notgreater") == wsName ) return 0x226F;
			else if ( _T("notgreaternorequal") == wsName ) return 0x2271;
			else if ( _T("notgreaternorless") == wsName ) return 0x2279;
			else if ( _T("notidentical") == wsName ) return 0x2262;
			else if ( _T("notless") == wsName ) return 0x226E;
			else if ( _T("notlessnorequal") == wsName ) return 0x2270;
			else if ( _T("notparallel") == wsName ) return 0x2226;
			else if ( _T("notprecedes") == wsName ) return 0x2280;
			else if ( _T("notsubset") == wsName ) return 0x2284;
			else if ( _T("notsucceeds") == wsName ) return 0x2281;
			else if ( _T("notsuperset") == wsName ) return 0x2285;
			else if ( _T("nowarmenian") == wsName ) return 0x0576;
			break;
		}
	case 'p':
		{
			if      ( _T("nparen") == wsName ) return 0x24A9;
			break;
		}
	case 's':
		{
			if      ( _T("nssquare") == wsName ) return 0x33B1;
			else if ( _T("nsuperior") == wsName ) return 0x207F;
			break;
		}
	case 't':
		{
			if      ( _T("ntilde") == wsName ) return 0x00F1;
			break;
		}
	case 'u':
		{
			if      ( _T("nu") == wsName ) return 0x03BD;
			else if ( _T("nuhiragana") == wsName ) return 0x306C;
			else if ( _T("nukatakana") == wsName ) return 0x30CC;
			else if ( _T("nukatakanahalfwidth") == wsName ) return 0xFF87;
			else if ( _T("nuktabengali") == wsName ) return 0x09BC;
			else if ( _T("nuktadeva") == wsName ) return 0x093C;
			else if ( _T("nuktagujarati") == wsName ) return 0x0ABC;
			else if ( _T("nuktagurmukhi") == wsName ) return 0x0A3C;
			else if ( _T("numbersign") == wsName ) return 0x0023;
			else if ( _T("numbersignmonospace") == wsName ) return 0xFF03;
			else if ( _T("numbersignsmall") == wsName ) return 0xFE5F;
			else if ( _T("numeralsigngreek") == wsName ) return 0x0374;
			else if ( _T("numeralsignlowergreek") == wsName ) return 0x0375;
			else if ( _T("numero") == wsName ) return 0x2116;
			else if ( _T("nun") == wsName ) return 0x05E0;
			else if ( _T("nundagesh") == wsName ) return 0xFB40;
			else if ( _T("nundageshhebrew") == wsName ) return 0xFB40;
			else if ( _T("nunhebrew") == wsName ) return 0x05E0;
			break;
		}
	case 'v':
		{
			if      ( _T("nvsquare") == wsName ) return 0x33B5;
			break;
		}
	case 'w':
		{
			if      ( _T("nwsquare") == wsName ) return 0x33BB;
			break;
		}
	case 'y':
		{
			if      ( _T("nyabengali") == wsName ) return 0x099E;
			else if ( _T("nyadeva") == wsName ) return 0x091E;
			else if ( _T("nyagujarati") == wsName ) return 0x0A9E;
			else if ( _T("nyagurmukhi") == wsName ) return 0x0A1E;
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
			if      ( _T("o") == wsName ) return 0x006F;
			break;
		}
	case 'a':
		{
			if      ( _T("oacute") == wsName ) return 0x00F3;
			else if ( _T("oangthai") == wsName ) return 0x0E2D;
			break;
		}
	case 'b':
		{
			if      ( _T("obarred") == wsName ) return 0x0275;
			else if ( _T("obarredcyrillic") == wsName ) return 0x04E9;
			else if ( _T("obarreddieresiscyrillic") == wsName ) return 0x04EB;
			else if ( _T("obengali") == wsName ) return 0x0993;
			else if ( _T("obopomofo") == wsName ) return 0x311B;
			else if ( _T("obreve") == wsName ) return 0x014F;
			break;
		}
	case 'c':
		{
			if      ( _T("ocandradeva") == wsName ) return 0x0911;
			else if ( _T("ocandragujarati") == wsName ) return 0x0A91;
			else if ( _T("ocandravowelsigndeva") == wsName ) return 0x0949;
			else if ( _T("ocandravowelsigngujarati") == wsName ) return 0x0AC9;
			else if ( _T("ocaron") == wsName ) return 0x01D2;
			else if ( _T("ocircle") == wsName ) return 0x24DE;
			else if ( _T("ocircumflex") == wsName ) return 0x00F4;
			else if ( _T("ocircumflexacute") == wsName ) return 0x1ED1;
			else if ( _T("ocircumflexdotbelow") == wsName ) return 0x1ED9;
			else if ( _T("ocircumflexgrave") == wsName ) return 0x1ED3;
			else if ( _T("ocircumflexhookabove") == wsName ) return 0x1ED5;
			else if ( _T("ocircumflextilde") == wsName ) return 0x1ED7;
			else if ( _T("ocyrillic") == wsName ) return 0x043E;
			break;
		}
	case 'd':
		{
			if      ( _T("odblacute") == wsName ) return 0x0151;
			else if ( _T("odblgrave") == wsName ) return 0x020D;
			else if ( _T("odeva") == wsName ) return 0x0913;
			else if ( _T("odieresis") == wsName ) return 0x00F6;
			else if ( _T("odieresiscyrillic") == wsName ) return 0x04E7;
			else if ( _T("odotbelow") == wsName ) return 0x1ECD;
			break;
		}
	case 'e':
		{
			if      ( _T("oe") == wsName ) return 0x0153;
			else if ( _T("oekorean") == wsName ) return 0x315A;
			break;
		}
	case 'g':
		{
			if      ( _T("ogonek") == wsName ) return 0x02DB;
			else if ( _T("ogonekcmb") == wsName ) return 0x0328;
			else if ( _T("ograve") == wsName ) return 0x00F2;
			else if ( _T("ogujarati") == wsName ) return 0x0A93;
			break;
		}
	case 'h':
		{
			if      ( _T("oharmenian") == wsName ) return 0x0585;
			else if ( _T("ohiragana") == wsName ) return 0x304A;
			else if ( _T("ohookabove") == wsName ) return 0x1ECF;
			else if ( _T("ohorn") == wsName ) return 0x01A1;
			else if ( _T("ohornacute") == wsName ) return 0x1EDB;
			else if ( _T("ohorndotbelow") == wsName ) return 0x1EE3;
			else if ( _T("ohorngrave") == wsName ) return 0x1EDD;
			else if ( _T("ohornhookabove") == wsName ) return 0x1EDF;
			else if ( _T("ohorntilde") == wsName ) return 0x1EE1;
			else if ( _T("ohungarumlaut") == wsName ) return 0x0151;
			break;
		}
	case 'i':
		{
			if      ( _T("oi") == wsName ) return 0x01A3;
			else if ( _T("oinvertedbreve") == wsName ) return 0x020F;
			break;
		}
	case 'k':
		{
			if      ( _T("okatakana") == wsName ) return 0x30AA;
			else if ( _T("okatakanahalfwidth") == wsName ) return 0xFF75;
			else if ( _T("okorean") == wsName ) return 0x3157;
			break;
		}
	case 'l':
		{
			if      ( _T("olehebrew") == wsName ) return 0x05AB;
			break;
		}
	case 'm':
		{
			if      ( _T("omacron") == wsName ) return 0x014D;
			else if ( _T("omacronacute") == wsName ) return 0x1E53;
			else if ( _T("omacrongrave") == wsName ) return 0x1E51;
			else if ( _T("omdeva") == wsName ) return 0x0950;
			else if ( _T("omega") == wsName ) return 0x03C9;
			else if ( _T("omega1") == wsName ) return 0x03D6;
			else if ( _T("omegacyrillic") == wsName ) return 0x0461;
			else if ( _T("omegalatinclosed") == wsName ) return 0x0277;
			else if ( _T("omegaroundcyrillic") == wsName ) return 0x047B;
			else if ( _T("omegatitlocyrillic") == wsName ) return 0x047D;
			else if ( _T("omegatonos") == wsName ) return 0x03CE;
			else if ( _T("omgujarati") == wsName ) return 0x0AD0;
			else if ( _T("omicron") == wsName ) return 0x03BF;
			else if ( _T("omicrontonos") == wsName ) return 0x03CC;
			else if ( _T("omonospace") == wsName ) return 0xFF4F;
			break;
		}
	case 'n':
		{
			if      ( _T("one") == wsName ) return 0x0031;
			else if ( _T("onearabic") == wsName ) return 0x0661;
			else if ( _T("onebengali") == wsName ) return 0x09E7;
			else if ( _T("onecircle") == wsName ) return 0x2460;
			else if ( _T("onecircleinversesansserif") == wsName ) return 0x278A;
			else if ( _T("onedeva") == wsName ) return 0x0967;
			else if ( _T("onedotenleader") == wsName ) return 0x2024;
			else if ( _T("oneeighth") == wsName ) return 0x215B;
			else if ( _T("onefitted") == wsName ) return 0xF6DC;
			else if ( _T("onegujarati") == wsName ) return 0x0AE7;
			else if ( _T("onegurmukhi") == wsName ) return 0x0A67;
			else if ( _T("onehackarabic") == wsName ) return 0x0661;
			else if ( _T("onehalf") == wsName ) return 0x00BD;
			else if ( _T("onehangzhou") == wsName ) return 0x3021;
			else if ( _T("oneideographicparen") == wsName ) return 0x3220;
			else if ( _T("oneinferior") == wsName ) return 0x2081;
			else if ( _T("onemonospace") == wsName ) return 0xFF11;
			else if ( _T("onenumeratorbengali") == wsName ) return 0x09F4;
			else if ( _T("oneoldstyle") == wsName ) return 0xF731;
			else if ( _T("oneparen") == wsName ) return 0x2474;
			else if ( _T("oneperiod") == wsName ) return 0x2488;
			else if ( _T("onepersian") == wsName ) return 0x06F1;
			else if ( _T("onequarter") == wsName ) return 0x00BC;
			else if ( _T("oneroman") == wsName ) return 0x2170;
			else if ( _T("onesuperior") == wsName ) return 0x00B9;
			else if ( _T("onethai") == wsName ) return 0x0E51;
			else if ( _T("onethird") == wsName ) return 0x2153;
			break;
		}
	case 'o':
		{
			if      ( _T("oogonek") == wsName ) return 0x01EB;
			else if ( _T("oogonekmacron") == wsName ) return 0x01ED;
			else if ( _T("oogurmukhi") == wsName ) return 0x0A13;
			else if ( _T("oomatragurmukhi") == wsName ) return 0x0A4B;
			else if ( _T("oopen") == wsName ) return 0x0254;
			break;
		}
	case 'p':
		{
			if      ( _T("oparen") == wsName ) return 0x24AA;
			else if ( _T("openbullet") == wsName ) return 0x25E6;
			else if ( _T("option") == wsName ) return 0x2325;
			break;
		}
	case 'r':
		{
			if      ( _T("ordfeminine") == wsName ) return 0x00AA;
			else if ( _T("ordmasculine") == wsName ) return 0x00BA;
			else if ( _T("orthogonal") == wsName ) return 0x221F;
			break;
		}
	case 's':
		{
			if      ( _T("oshortdeva") == wsName ) return 0x0912;
			else if ( _T("oshortvowelsigndeva") == wsName ) return 0x094A;
			else if ( _T("oslash") == wsName ) return 0x00F8;
			else if ( _T("oslashacute") == wsName ) return 0x01FF;
			else if ( _T("osmallhiragana") == wsName ) return 0x3049;
			else if ( _T("osmallkatakana") == wsName ) return 0x30A9;
			else if ( _T("osmallkatakanahalfwidth") == wsName ) return 0xFF6B;
			else if ( _T("ostrokeacute") == wsName ) return 0x01FF;
			else if ( _T("osuperior") == wsName ) return 0xF6F0;
			break;
		}
	case 't':
		{
			if      ( _T("otcyrillic") == wsName ) return 0x047F;
			else if ( _T("otilde") == wsName ) return 0x00F5;
			else if ( _T("otildeacute") == wsName ) return 0x1E4D;
			else if ( _T("otildedieresis") == wsName ) return 0x1E4F;
			break;
		}
	case 'u':
		{
			if      ( _T("oubopomofo") == wsName ) return 0x3121;
			break;
		}
	case 'v':
		{
			if      ( _T("overline") == wsName ) return 0x203E;
			else if ( _T("overlinecenterline") == wsName ) return 0xFE4A;
			else if ( _T("overlinecmb") == wsName ) return 0x0305;
			else if ( _T("overlinedashed") == wsName ) return 0xFE49;
			else if ( _T("overlinedblwavy") == wsName ) return 0xFE4C;
			else if ( _T("overlinewavy") == wsName ) return 0xFE4B;
			else if ( _T("overscore") == wsName ) return 0x00AF;
			else if ( _T("ovowelsignbengali") == wsName ) return 0x09CB;
			else if ( _T("ovowelsigndeva") == wsName ) return 0x094B;
			else if ( _T("ovowelsigngujarati") == wsName ) return 0x0ACB;
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
			if      ( _T("p") == wsName ) return 0x0070;
			break;
		}
	case 'a':
		{
			if      ( _T("paampssquare") == wsName ) return 0x3380;
			else if ( _T("paasentosquare") == wsName ) return 0x332B;
			else if ( _T("pabengali") == wsName ) return 0x09AA;
			else if ( _T("pacute") == wsName ) return 0x1E55;
			else if ( _T("padeva") == wsName ) return 0x092A;
			else if ( _T("pagedown") == wsName ) return 0x21DF;
			else if ( _T("pageup") == wsName ) return 0x21DE;
			else if ( _T("pagujarati") == wsName ) return 0x0AAA;
			else if ( _T("pagurmukhi") == wsName ) return 0x0A2A;
			else if ( _T("pahiragana") == wsName ) return 0x3071;
			else if ( _T("paiyannoithai") == wsName ) return 0x0E2F;
			else if ( _T("pakatakana") == wsName ) return 0x30D1;
			else if ( _T("palatalizationcyrilliccmb") == wsName ) return 0x0484;
			else if ( _T("palochkacyrillic") == wsName ) return 0x04C0;
			else if ( _T("pansioskorean") == wsName ) return 0x317F;
			else if ( _T("paragraph") == wsName ) return 0x00B6;
			else if ( _T("parallel") == wsName ) return 0x2225;
			else if ( _T("parenleft") == wsName ) return 0x0028;
			else if ( _T("parenleftaltonearabic") == wsName ) return 0xFD3E;
			else if ( _T("parenleftbt") == wsName ) return 0xF8ED;
			else if ( _T("parenleftex") == wsName ) return 0xF8EC;
			else if ( _T("parenleftinferior") == wsName ) return 0x208D;
			else if ( _T("parenleftmonospace") == wsName ) return 0xFF08;
			else if ( _T("parenleftsmall") == wsName ) return 0xFE59;
			else if ( _T("parenleftsuperior") == wsName ) return 0x207D;
			else if ( _T("parenlefttp") == wsName ) return 0xF8EB;
			else if ( _T("parenleftvertical") == wsName ) return 0xFE35;
			else if ( _T("parenright") == wsName ) return 0x0029;
			else if ( _T("parenrightaltonearabic") == wsName ) return 0xFD3F;
			else if ( _T("parenrightbt") == wsName ) return 0xF8F8;
			else if ( _T("parenrightex") == wsName ) return 0xF8F7;
			else if ( _T("parenrightinferior") == wsName ) return 0x208E;
			else if ( _T("parenrightmonospace") == wsName ) return 0xFF09;
			else if ( _T("parenrightsmall") == wsName ) return 0xFE5A;
			else if ( _T("parenrightsuperior") == wsName ) return 0x207E;
			else if ( _T("parenrighttp") == wsName ) return 0xF8F6;
			else if ( _T("parenrightvertical") == wsName ) return 0xFE36;
			else if ( _T("partialdiff") == wsName ) return 0x2202;
			else if ( _T("paseqhebrew") == wsName ) return 0x05C0;
			else if ( _T("pashtahebrew") == wsName ) return 0x0599;
			else if ( _T("pasquare") == wsName ) return 0x33A9;
			else if ( _T("patah") == wsName ) return 0x05B7;
			else if ( _T("patah11") == wsName ) return 0x05B7;
			else if ( _T("patah1d") == wsName ) return 0x05B7;
			else if ( _T("patah2a") == wsName ) return 0x05B7;
			else if ( _T("patahhebrew") == wsName ) return 0x05B7;
			else if ( _T("patahnarrowhebrew") == wsName ) return 0x05B7;
			else if ( _T("patahquarterhebrew") == wsName ) return 0x05B7;
			else if ( _T("patahwidehebrew") == wsName ) return 0x05B7;
			else if ( _T("pazerhebrew") == wsName ) return 0x05A1;
			break;
		}
	case 'b':
		{
			if      ( _T("pbopomofo") == wsName ) return 0x3106;
			break;
		}
	case 'c':
		{
			if      ( _T("pcircle") == wsName ) return 0x24DF;
			break;
		}
	case 'd':
		{
			if      ( _T("pdotaccent") == wsName ) return 0x1E57;
			break;
		}
	case 'e':
		{
			if      ( _T("pe") == wsName ) return 0x05E4;
			else if ( _T("pecyrillic") == wsName ) return 0x043F;
			else if ( _T("pedagesh") == wsName ) return 0xFB44;
			else if ( _T("pedageshhebrew") == wsName ) return 0xFB44;
			else if ( _T("peezisquare") == wsName ) return 0x333B;
			else if ( _T("pefinaldageshhebrew") == wsName ) return 0xFB43;
			else if ( _T("peharabic") == wsName ) return 0x067E;
			else if ( _T("peharmenian") == wsName ) return 0x057A;
			else if ( _T("pehebrew") == wsName ) return 0x05E4;
			else if ( _T("pehfinalarabic") == wsName ) return 0xFB57;
			else if ( _T("pehinitialarabic") == wsName ) return 0xFB58;
			else if ( _T("pehiragana") == wsName ) return 0x307A;
			else if ( _T("pehmedialarabic") == wsName ) return 0xFB59;
			else if ( _T("pekatakana") == wsName ) return 0x30DA;
			else if ( _T("pemiddlehookcyrillic") == wsName ) return 0x04A7;
			else if ( _T("perafehebrew") == wsName ) return 0xFB4E;
			else if ( _T("percent") == wsName ) return 0x0025;
			else if ( _T("percentarabic") == wsName ) return 0x066A;
			else if ( _T("percentmonospace") == wsName ) return 0xFF05;
			else if ( _T("percentsmall") == wsName ) return 0xFE6A;
			else if ( _T("period") == wsName ) return 0x002E;
			else if ( _T("periodarmenian") == wsName ) return 0x0589;
			else if ( _T("periodcentered") == wsName ) return 0x00B7;
			else if ( _T("periodhalfwidth") == wsName ) return 0xFF61;
			else if ( _T("periodinferior") == wsName ) return 0xF6E7;
			else if ( _T("periodmonospace") == wsName ) return 0xFF0E;
			else if ( _T("periodsmall") == wsName ) return 0xFE52;
			else if ( _T("periodsuperior") == wsName ) return 0xF6E8;
			else if ( _T("perispomenigreekcmb") == wsName ) return 0x0342;
			else if ( _T("perpendicular") == wsName ) return 0x22A5;
			else if ( _T("perthousand") == wsName ) return 0x2030;
			else if ( _T("peseta") == wsName ) return 0x20A7;
			break;
		}
	case 'f':
		{
			if      ( _T("pfsquare") == wsName ) return 0x338A;
			break;
		}
	case 'h':
		{
			if      ( _T("phabengali") == wsName ) return 0x09AB;
			else if ( _T("phadeva") == wsName ) return 0x092B;
			else if ( _T("phagujarati") == wsName ) return 0x0AAB;
			else if ( _T("phagurmukhi") == wsName ) return 0x0A2B;
			else if ( _T("phi") == wsName ) return 0x03C6;
			else if ( _T("phi1") == wsName ) return 0x03D5;
			else if ( _T("phieuphacirclekorean") == wsName ) return 0x327A;
			else if ( _T("phieuphaparenkorean") == wsName ) return 0x321A;
			else if ( _T("phieuphcirclekorean") == wsName ) return 0x326C;
			else if ( _T("phieuphkorean") == wsName ) return 0x314D;
			else if ( _T("phieuphparenkorean") == wsName ) return 0x320C;
			else if ( _T("philatin") == wsName ) return 0x0278;
			else if ( _T("phinthuthai") == wsName ) return 0x0E3A;
			else if ( _T("phisymbolgreek") == wsName ) return 0x03D5;
			else if ( _T("phook") == wsName ) return 0x01A5;
			else if ( _T("phophanthai") == wsName ) return 0x0E1E;
			else if ( _T("phophungthai") == wsName ) return 0x0E1C;
			else if ( _T("phosamphaothai") == wsName ) return 0x0E20;
			break;
		}
	case 'i':
		{
			if      ( _T("pi") == wsName ) return 0x03C0;
			else if ( _T("pieupacirclekorean") == wsName ) return 0x3273;
			else if ( _T("pieupaparenkorean") == wsName ) return 0x3213;
			else if ( _T("pieupcieuckorean") == wsName ) return 0x3176;
			else if ( _T("pieupcirclekorean") == wsName ) return 0x3265;
			else if ( _T("pieupkiyeokkorean") == wsName ) return 0x3172;
			else if ( _T("pieupkorean") == wsName ) return 0x3142;
			else if ( _T("pieupparenkorean") == wsName ) return 0x3205;
			else if ( _T("pieupsioskiyeokkorean") == wsName ) return 0x3174;
			else if ( _T("pieupsioskorean") == wsName ) return 0x3144;
			else if ( _T("pieupsiostikeutkorean") == wsName ) return 0x3175;
			else if ( _T("pieupthieuthkorean") == wsName ) return 0x3177;
			else if ( _T("pieuptikeutkorean") == wsName ) return 0x3173;
			else if ( _T("pihiragana") == wsName ) return 0x3074;
			else if ( _T("pikatakana") == wsName ) return 0x30D4;
			else if ( _T("pisymbolgreek") == wsName ) return 0x03D6;
			else if ( _T("piwrarmenian") == wsName ) return 0x0583;
			break;
		}
	case 'l':
		{
			if      ( _T("plus") == wsName ) return 0x002B;
			else if ( _T("plusbelowcmb") == wsName ) return 0x031F;
			else if ( _T("pluscircle") == wsName ) return 0x2295;
			else if ( _T("plusminus") == wsName ) return 0x00B1;
			else if ( _T("plusmod") == wsName ) return 0x02D6;
			else if ( _T("plusmonospace") == wsName ) return 0xFF0B;
			else if ( _T("plussmall") == wsName ) return 0xFE62;
			else if ( _T("plussuperior") == wsName ) return 0x207A;
			break;
		}
	case 'm':
		{
			if      ( _T("pmonospace") == wsName ) return 0xFF50;
			else if ( _T("pmsquare") == wsName ) return 0x33D8;
			break;
		}
	case 'o':
		{
			if      ( _T("pohiragana") == wsName ) return 0x307D;
			else if ( _T("pointingindexdownwhite") == wsName ) return 0x261F;
			else if ( _T("pointingindexleftwhite") == wsName ) return 0x261C;
			else if ( _T("pointingindexrightwhite") == wsName ) return 0x261E;
			else if ( _T("pointingindexupwhite") == wsName ) return 0x261D;
			else if ( _T("pokatakana") == wsName ) return 0x30DD;
			else if ( _T("poplathai") == wsName ) return 0x0E1B;
			else if ( _T("postalmark") == wsName ) return 0x3012;
			else if ( _T("postalmarkface") == wsName ) return 0x3020;
			break;
		}
	case 'p':
		{
			if      ( _T("pparen") == wsName ) return 0x24AB;
			break;
		}
	case 'r':
		{
			if      ( _T("precedes") == wsName ) return 0x227A;
			else if ( _T("prescription") == wsName ) return 0x211E;
			else if ( _T("primemod") == wsName ) return 0x02B9;
			else if ( _T("primereversed") == wsName ) return 0x2035;
			else if ( _T("product") == wsName ) return 0x220F;
			else if ( _T("projective") == wsName ) return 0x2305;
			else if ( _T("prolongedkana") == wsName ) return 0x30FC;
			else if ( _T("propellor") == wsName ) return 0x2318;
			else if ( _T("propersubset") == wsName ) return 0x2282;
			else if ( _T("propersuperset") == wsName ) return 0x2283;
			else if ( _T("proportion") == wsName ) return 0x2237;
			else if ( _T("proportional") == wsName ) return 0x221D;
			break;
		}
	case 's':
		{
			if      ( _T("psi") == wsName ) return 0x03C8;
			else if ( _T("psicyrillic") == wsName ) return 0x0471;
			else if ( _T("psilipneumatacyrilliccmb") == wsName ) return 0x0486;
			else if ( _T("pssquare") == wsName ) return 0x33B0;
			break;
		}
	case 'u':
		{
			if      ( _T("puhiragana") == wsName ) return 0x3077;
			else if ( _T("pukatakana") == wsName ) return 0x30D7;
			break;
		}
	case 'v':
		{
			if      ( _T("pvsquare") == wsName ) return 0x33B4;
			break;
		}
	case 'w':
		{
			if      ( _T("pwsquare") == wsName ) return 0x33BA;
			break;
		}
	}

	return 0;
}
static int Type1NameToUnicode_q(CWCharWrapper &wsName)
{
	if      ( _T("q") == wsName ) return 0x0071;
	else if ( _T("qadeva") == wsName ) return 0x0958;
	else if ( _T("qadmahebrew") == wsName ) return 0x05A8;
	else if ( _T("qafarabic") == wsName ) return 0x0642;
	else if ( _T("qaffinalarabic") == wsName ) return 0xFED6;
	else if ( _T("qafinitialarabic") == wsName ) return 0xFED7;
	else if ( _T("qafmedialarabic") == wsName ) return 0xFED8;
	else if ( _T("qamats") == wsName ) return 0x05B8;
	else if ( _T("qamats10") == wsName ) return 0x05B8;
	else if ( _T("qamats1a") == wsName ) return 0x05B8;
	else if ( _T("qamats1c") == wsName ) return 0x05B8;
	else if ( _T("qamats27") == wsName ) return 0x05B8;
	else if ( _T("qamats29") == wsName ) return 0x05B8;
	else if ( _T("qamats33") == wsName ) return 0x05B8;
	else if ( _T("qamatsde") == wsName ) return 0x05B8;
	else if ( _T("qamatshebrew") == wsName ) return 0x05B8;
	else if ( _T("qamatsnarrowhebrew") == wsName ) return 0x05B8;
	else if ( _T("qamatsqatanhebrew") == wsName ) return 0x05B8;
	else if ( _T("qamatsqatannarrowhebrew") == wsName ) return 0x05B8;
	else if ( _T("qamatsqatanquarterhebrew") == wsName ) return 0x05B8;
	else if ( _T("qamatsqatanwidehebrew") == wsName ) return 0x05B8;
	else if ( _T("qamatsquarterhebrew") == wsName ) return 0x05B8;
	else if ( _T("qamatswidehebrew") == wsName ) return 0x05B8;
	else if ( _T("qarneyparahebrew") == wsName ) return 0x059F;
	else if ( _T("qbopomofo") == wsName ) return 0x3111;
	else if ( _T("qcircle") == wsName ) return 0x24E0;
	else if ( _T("qhook") == wsName ) return 0x02A0;
	else if ( _T("qmonospace") == wsName ) return 0xFF51;
	else if ( _T("qof") == wsName ) return 0x05E7;
	else if ( _T("qofdagesh") == wsName ) return 0xFB47;
	else if ( _T("qofdageshhebrew") == wsName ) return 0xFB47;
	else if ( _T("qofhatafpatah") == wsName ) return 0x05E705B2;
	else if ( _T("qofhatafpatahhebrew") == wsName ) return 0x05E705B2;
	else if ( _T("qofhatafsegol") == wsName ) return 0x05E705B1;
	else if ( _T("qofhatafsegolhebrew") == wsName ) return 0x05E705B1;
	else if ( _T("qofhebrew") == wsName ) return 0x05E7;
	else if ( _T("qofhiriq") == wsName ) return 0x05E705B4;
	else if ( _T("qofhiriqhebrew") == wsName ) return 0x05E705B4;
	else if ( _T("qofholam") == wsName ) return 0x05E705B9;
	else if ( _T("qofholamhebrew") == wsName ) return 0x05E705B9;
	else if ( _T("qofpatah") == wsName ) return 0x05E705B7;
	else if ( _T("qofpatahhebrew") == wsName ) return 0x05E705B7;
	else if ( _T("qofqamats") == wsName ) return 0x05E705B8;
	else if ( _T("qofqamatshebrew") == wsName ) return 0x05E705B8;
	else if ( _T("qofqubuts") == wsName ) return 0x05E705BB;
	else if ( _T("qofqubutshebrew") == wsName ) return 0x05E705BB;
	else if ( _T("qofsegol") == wsName ) return 0x05E705B6;
	else if ( _T("qofsegolhebrew") == wsName ) return 0x05E705B6;
	else if ( _T("qofsheva") == wsName ) return 0x05E705B0;
	else if ( _T("qofshevahebrew") == wsName ) return 0x05E705B0;
	else if ( _T("qoftsere") == wsName ) return 0x05E705B5;
	else if ( _T("qoftserehebrew") == wsName ) return 0x05E705B5;
	else if ( _T("qparen") == wsName ) return 0x24AC;
	else if ( _T("quarternote") == wsName ) return 0x2669;
	else if ( _T("qubuts") == wsName ) return 0x05BB;
	else if ( _T("qubuts18") == wsName ) return 0x05BB;
	else if ( _T("qubuts25") == wsName ) return 0x05BB;
	else if ( _T("qubuts31") == wsName ) return 0x05BB;
	else if ( _T("qubutshebrew") == wsName ) return 0x05BB;
	else if ( _T("qubutsnarrowhebrew") == wsName ) return 0x05BB;
	else if ( _T("qubutsquarterhebrew") == wsName ) return 0x05BB;
	else if ( _T("qubutswidehebrew") == wsName ) return 0x05BB;
	else if ( _T("question") == wsName ) return 0x003F;
	else if ( _T("questionarabic") == wsName ) return 0x061F;
	else if ( _T("questionarmenian") == wsName ) return 0x055E;
	else if ( _T("questiondown") == wsName ) return 0x00BF;
	else if ( _T("questiondownsmall") == wsName ) return 0xF7BF;
	else if ( _T("questiongreek") == wsName ) return 0x037E;
	else if ( _T("questionmonospace") == wsName ) return 0xFF1F;
	else if ( _T("questionsmall") == wsName ) return 0xF73F;
	else if ( _T("quotedbl") == wsName ) return 0x0022;
	else if ( _T("quotedblbase") == wsName ) return 0x201E;
	else if ( _T("quotedblleft") == wsName ) return 0x201C;
	else if ( _T("quotedblmonospace") == wsName ) return 0xFF02;
	else if ( _T("quotedblprime") == wsName ) return 0x301E;
	else if ( _T("quotedblprimereversed") == wsName ) return 0x301D;
	else if ( _T("quotedblright") == wsName ) return 0x201D;
	else if ( _T("quoteleft") == wsName ) return 0x2018;
	else if ( _T("quoteleftreversed") == wsName ) return 0x201B;
	else if ( _T("quotereversed") == wsName ) return 0x201B;
	else if ( _T("quoteright") == wsName ) return 0x2019;
	else if ( _T("quoterightn") == wsName ) return 0x0149;
	else if ( _T("quotesinglbase") == wsName ) return 0x201A;
	else if ( _T("quotesingle") == wsName ) return 0x0027;
	else if ( _T("quotesinglemonospace") == wsName ) return 0xFF07;

	return 0;
}
static int Type1NameToUnicode_r(CWCharWrapper &wsName)
{
	wchar_t wChar1 = wsName[1];
	switch ( wChar1 )
	{
	case 0:
		{
			if      ( _T("r") == wsName ) return 0x0072;
			break;
		}
	case 'a':
		{
			if      ( _T("raarmenian") == wsName ) return 0x057C;
			else if ( _T("rabengali") == wsName ) return 0x09B0;
			else if ( _T("racute") == wsName ) return 0x0155;
			else if ( _T("radeva") == wsName ) return 0x0930;
			else if ( _T("radical") == wsName ) return 0x221A;
			else if ( _T("radicalex") == wsName ) return 0xF8E5;
			else if ( _T("radoverssquare") == wsName ) return 0x33AE;
			else if ( _T("radoverssquaredsquare") == wsName ) return 0x33AF;
			else if ( _T("radsquare") == wsName ) return 0x33AD;
			else if ( _T("rafe") == wsName ) return 0x05BF;
			else if ( _T("rafehebrew") == wsName ) return 0x05BF;
			else if ( _T("ragujarati") == wsName ) return 0x0AB0;
			else if ( _T("ragurmukhi") == wsName ) return 0x0A30;
			else if ( _T("rahiragana") == wsName ) return 0x3089;
			else if ( _T("rakatakana") == wsName ) return 0x30E9;
			else if ( _T("rakatakanahalfwidth") == wsName ) return 0xFF97;
			else if ( _T("ralowerdiagonalbengali") == wsName ) return 0x09F1;
			else if ( _T("ramiddlediagonalbengali") == wsName ) return 0x09F0;
			else if ( _T("ramshorn") == wsName ) return 0x0264;
			else if ( _T("ratio") == wsName ) return 0x2236;
			break;
		}
	case 'b':
		{
			if      ( _T("rbopomofo") == wsName ) return 0x3116;
			break;
		}
	case 'c':
		{
			if      ( _T("rcaron") == wsName ) return 0x0159;
			else if ( _T("rcedilla") == wsName ) return 0x0157;
			else if ( _T("rcircle") == wsName ) return 0x24E1;
			else if ( _T("rcommaaccent") == wsName ) return 0x0157;
			break;
		}
	case 'd':
		{
			if      ( _T("rdblgrave") == wsName ) return 0x0211;
			else if ( _T("rdotaccent") == wsName ) return 0x1E59;
			else if ( _T("rdotbelow") == wsName ) return 0x1E5B;
			else if ( _T("rdotbelowmacron") == wsName ) return 0x1E5D;
			break;
		}
	case 'e':
		{
			if      ( _T("referencemark") == wsName ) return 0x203B;
			else if ( _T("reflexsubset") == wsName ) return 0x2286;
			else if ( _T("reflexsuperset") == wsName ) return 0x2287;
			else if ( _T("registered") == wsName ) return 0x00AE;
			else if ( _T("registersans") == wsName ) return 0xF8E8;
			else if ( _T("registerserif") == wsName ) return 0xF6DA;
			else if ( _T("reharabic") == wsName ) return 0x0631;
			else if ( _T("reharmenian") == wsName ) return 0x0580;
			else if ( _T("rehfinalarabic") == wsName ) return 0xFEAE;
			else if ( _T("rehiragana") == wsName ) return 0x308C;
			//{ "rehyehaleflamarabic") == wsName ) return "0631 FEF3 FE8E 0644";
			else if ( _T("rekatakana") == wsName ) return 0x30EC;
			else if ( _T("rekatakanahalfwidth") == wsName ) return 0xFF9A;
			else if ( _T("resh") == wsName ) return 0x05E8;
			else if ( _T("reshdageshhebrew") == wsName ) return 0xFB48;
			else if ( _T("reshhatafpatah") == wsName ) return 0x05E805B2;
			else if ( _T("reshhatafpatahhebrew") == wsName ) return 0x05E805B2;
			else if ( _T("reshhatafsegol") == wsName ) return 0x05E805B1;
			else if ( _T("reshhatafsegolhebrew") == wsName ) return 0x05E805B1;
			else if ( _T("reshhebrew") == wsName ) return 0x05E8;
			else if ( _T("reshhiriq") == wsName ) return 0x05E805B4;
			else if ( _T("reshhiriqhebrew") == wsName ) return 0x05E805B4;
			else if ( _T("reshholam") == wsName ) return 0x05E805B9;
			else if ( _T("reshholamhebrew") == wsName ) return 0x05E805B9;
			else if ( _T("reshpatah") == wsName ) return 0x05E805B7;
			else if ( _T("reshpatahhebrew") == wsName ) return 0x05E805B7;
			else if ( _T("reshqamats") == wsName ) return 0x05E805B8;
			else if ( _T("reshqamatshebrew") == wsName ) return 0x05E805B8;
			else if ( _T("reshqubuts") == wsName ) return 0x05E805BB;
			else if ( _T("reshqubutshebrew") == wsName ) return 0x05E805BB;
			else if ( _T("reshsegol") == wsName ) return 0x05E805B6;
			else if ( _T("reshsegolhebrew") == wsName ) return 0x05E805B6;
			else if ( _T("reshsheva") == wsName ) return 0x05E805B0;
			else if ( _T("reshshevahebrew") == wsName ) return 0x05E805B0;
			else if ( _T("reshtsere") == wsName ) return 0x05E805B5;
			else if ( _T("reshtserehebrew") == wsName ) return 0x05E805B5;
			else if ( _T("reversedtilde") == wsName ) return 0x223D;
			else if ( _T("reviahebrew") == wsName ) return 0x0597;
			else if ( _T("reviamugrashhebrew") == wsName ) return 0x0597;
			else if ( _T("revlogicalnot") == wsName ) return 0x2310;
			break;
		}
	case 'f':
		{
			if      ( _T("rfishhook") == wsName ) return 0x027E;
			else if ( _T("rfishhookreversed") == wsName ) return 0x027F;
			break;
		}
	case 'h':
		{
			if      ( _T("rhabengali") == wsName ) return 0x09DD;
			else if ( _T("rhadeva") == wsName ) return 0x095D;
			else if ( _T("rho") == wsName ) return 0x03C1;
			else if ( _T("rhook") == wsName ) return 0x027D;
			else if ( _T("rhookturned") == wsName ) return 0x027B;
			else if ( _T("rhookturnedsuperior") == wsName ) return 0x02B5;
			else if ( _T("rhosymbolgreek") == wsName ) return 0x03F1;
			else if ( _T("rhotichookmod") == wsName ) return 0x02DE;
			break;
		}
	case 'i':
		{
			if      ( _T("rieulacirclekorean") == wsName ) return 0x3271;
			else if ( _T("rieulaparenkorean") == wsName ) return 0x3211;
			else if ( _T("rieulcirclekorean") == wsName ) return 0x3263;
			else if ( _T("rieulhieuhkorean") == wsName ) return 0x3140;
			else if ( _T("rieulkiyeokkorean") == wsName ) return 0x313A;
			else if ( _T("rieulkiyeoksioskorean") == wsName ) return 0x3169;
			else if ( _T("rieulkorean") == wsName ) return 0x3139;
			else if ( _T("rieulmieumkorean") == wsName ) return 0x313B;
			else if ( _T("rieulpansioskorean") == wsName ) return 0x316C;
			else if ( _T("rieulparenkorean") == wsName ) return 0x3203;
			else if ( _T("rieulphieuphkorean") == wsName ) return 0x313F;
			else if ( _T("rieulpieupkorean") == wsName ) return 0x313C;
			else if ( _T("rieulpieupsioskorean") == wsName ) return 0x316B;
			else if ( _T("rieulsioskorean") == wsName ) return 0x313D;
			else if ( _T("rieulthieuthkorean") == wsName ) return 0x313E;
			else if ( _T("rieultikeutkorean") == wsName ) return 0x316A;
			else if ( _T("rieulyeorinhieuhkorean") == wsName ) return 0x316D;
			else if ( _T("rightangle") == wsName ) return 0x221F;
			else if ( _T("righttackbelowcmb") == wsName ) return 0x0319;
			else if ( _T("righttriangle") == wsName ) return 0x22BF;
			else if ( _T("rihiragana") == wsName ) return 0x308A;
			else if ( _T("rikatakana") == wsName ) return 0x30EA;
			else if ( _T("rikatakanahalfwidth") == wsName ) return 0xFF98;
			else if ( _T("ring") == wsName ) return 0x02DA;
			else if ( _T("ringbelowcmb") == wsName ) return 0x0325;
			else if ( _T("ringcmb") == wsName ) return 0x030A;
			else if ( _T("ringhalfleft") == wsName ) return 0x02BF;
			else if ( _T("ringhalfleftarmenian") == wsName ) return 0x0559;
			else if ( _T("ringhalfleftbelowcmb") == wsName ) return 0x031C;
			else if ( _T("ringhalfleftcentered") == wsName ) return 0x02D3;
			else if ( _T("ringhalfright") == wsName ) return 0x02BE;
			else if ( _T("ringhalfrightbelowcmb") == wsName ) return 0x0339;
			else if ( _T("ringhalfrightcentered") == wsName ) return 0x02D2;
			else if ( _T("rinvertedbreve") == wsName ) return 0x0213;
			else if ( _T("rittorusquare") == wsName ) return 0x3351;
			break;
		}
	case 'l':
		{
			if      ( _T("rlinebelow") == wsName ) return 0x1E5F;
			else if ( _T("rlongleg") == wsName ) return 0x027C;
			else if ( _T("rlonglegturned") == wsName ) return 0x027A;
			break;
		}
	case 'm':
		{
			if      ( _T("rmonospace") == wsName ) return 0xFF52;
			break;
		}
	case 'o':
		{
			if      ( _T("rohiragana") == wsName ) return 0x308D;
			else if ( _T("rokatakana") == wsName ) return 0x30ED;
			else if ( _T("rokatakanahalfwidth") == wsName ) return 0xFF9B;
			else if ( _T("roruathai") == wsName ) return 0x0E23;
			break;
		}
	case 'p':
		{
			if      ( _T("rparen") == wsName ) return 0x24AD;
			break;
		}
	case 'r':
		{
			if      ( _T("rrabengali") == wsName ) return 0x09DC;
			else if ( _T("rradeva") == wsName ) return 0x0931;
			else if ( _T("rragurmukhi") == wsName ) return 0x0A5C;
			else if ( _T("rreharabic") == wsName ) return 0x0691;
			else if ( _T("rrehfinalarabic") == wsName ) return 0xFB8D;
			else if ( _T("rrvocalicbengali") == wsName ) return 0x09E0;
			else if ( _T("rrvocalicdeva") == wsName ) return 0x0960;
			else if ( _T("rrvocalicgujarati") == wsName ) return 0x0AE0;
			else if ( _T("rrvocalicvowelsignbengali") == wsName ) return 0x09C4;
			else if ( _T("rrvocalicvowelsigndeva") == wsName ) return 0x0944;
			else if ( _T("rrvocalicvowelsigngujarati") == wsName ) return 0x0AC4;
			break;
		}
	case 's':
		{
			if      ( _T("rsuperior") == wsName ) return 0xF6F1;
			break;
		}
	case 't':
		{
			if      ( _T("rtblock") == wsName ) return 0x2590;
			else if ( _T("rturned") == wsName ) return 0x0279;
			else if ( _T("rturnedsuperior") == wsName ) return 0x02B4;
			break;
		}
	case 'u':
		{
			if      ( _T("ruhiragana") == wsName ) return 0x308B;
			else if ( _T("rukatakana") == wsName ) return 0x30EB;
			else if ( _T("rukatakanahalfwidth") == wsName ) return 0xFF99;
			else if ( _T("rupeemarkbengali") == wsName ) return 0x09F2;
			else if ( _T("rupeesignbengali") == wsName ) return 0x09F3;
			else if ( _T("rupiah") == wsName ) return 0xF6DD;
			else if ( _T("ruthai") == wsName ) return 0x0E24;
			break;
		}
	case 'v':
		{
			if      ( _T("rvocalicbengali") == wsName ) return 0x098B;
			else if ( _T("rvocalicdeva") == wsName ) return 0x090B;
			else if ( _T("rvocalicgujarati") == wsName ) return 0x0A8B;
			else if ( _T("rvocalicvowelsignbengali") == wsName ) return 0x09C3;
			else if ( _T("rvocalicvowelsigndeva") == wsName ) return 0x0943;
			else if ( _T("rvocalicvowelsigngujarati") == wsName ) return 0x0AC3;
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
			if      ( _T("s") == wsName ) return 0x0073;
			break;
		}
	case 'a':
		{
			if      ( _T("sabengali") == wsName ) return 0x09B8;
			else if ( _T("sacute") == wsName ) return 0x015B;
			else if ( _T("sacutedotaccent") == wsName ) return 0x1E65;
			else if ( _T("sadarabic") == wsName ) return 0x0635;
			else if ( _T("sadeva") == wsName ) return 0x0938;
			else if ( _T("sadfinalarabic") == wsName ) return 0xFEBA;
			else if ( _T("sadinitialarabic") == wsName ) return 0xFEBB;
			else if ( _T("sadmedialarabic") == wsName ) return 0xFEBC;
			else if ( _T("sagujarati") == wsName ) return 0x0AB8;
			else if ( _T("sagurmukhi") == wsName ) return 0x0A38;
			else if ( _T("sahiragana") == wsName ) return 0x3055;
			else if ( _T("sakatakana") == wsName ) return 0x30B5;
			else if ( _T("sakatakanahalfwidth") == wsName ) return 0xFF7B;
			else if ( _T("sallallahoualayhewasallamarabic") == wsName ) return 0xFDFA;
			else if ( _T("samekh") == wsName ) return 0x05E1;
			else if ( _T("samekhdagesh") == wsName ) return 0xFB41;
			else if ( _T("samekhdageshhebrew") == wsName ) return 0xFB41;
			else if ( _T("samekhhebrew") == wsName ) return 0x05E1;
			else if ( _T("saraaathai") == wsName ) return 0x0E32;
			else if ( _T("saraaethai") == wsName ) return 0x0E41;
			else if ( _T("saraaimaimalaithai") == wsName ) return 0x0E44;
			else if ( _T("saraaimaimuanthai") == wsName ) return 0x0E43;
			else if ( _T("saraamthai") == wsName ) return 0x0E33;
			else if ( _T("saraathai") == wsName ) return 0x0E30;
			else if ( _T("saraethai") == wsName ) return 0x0E40;
			else if ( _T("saraiileftthai") == wsName ) return 0xF886;
			else if ( _T("saraiithai") == wsName ) return 0x0E35;
			else if ( _T("saraileftthai") == wsName ) return 0xF885;
			else if ( _T("saraithai") == wsName ) return 0x0E34;
			else if ( _T("saraothai") == wsName ) return 0x0E42;
			else if ( _T("saraueeleftthai") == wsName ) return 0xF888;
			else if ( _T("saraueethai") == wsName ) return 0x0E37;
			else if ( _T("saraueleftthai") == wsName ) return 0xF887;
			else if ( _T("sarauethai") == wsName ) return 0x0E36;
			else if ( _T("sarauthai") == wsName ) return 0x0E38;
			else if ( _T("sarauuthai") == wsName ) return 0x0E39;
			break;
		}
	case 'b':
		{
			if      ( _T("sbopomofo") == wsName ) return 0x3119;
			break;
		}
	case 'c':
		{
			if      ( _T("scaron") == wsName ) return 0x0161;
			else if ( _T("scarondotaccent") == wsName ) return 0x1E67;
			else if ( _T("scedilla") == wsName ) return 0x015F;
			else if ( _T("schwa") == wsName ) return 0x0259;
			else if ( _T("schwacyrillic") == wsName ) return 0x04D9;
			else if ( _T("schwadieresiscyrillic") == wsName ) return 0x04DB;
			else if ( _T("schwahook") == wsName ) return 0x025A;
			else if ( _T("scircle") == wsName ) return 0x24E2;
			else if ( _T("scircumflex") == wsName ) return 0x015D;
			else if ( _T("scommaaccent") == wsName ) return 0x0219;
			break;
		}
	case 'd':
		{
			if      ( _T("sdotaccent") == wsName ) return 0x1E61;
			else if ( _T("sdotbelow") == wsName ) return 0x1E63;
			else if ( _T("sdotbelowdotaccent") == wsName ) return 0x1E69;
			break;
		}
	case 'e':
		{
			if      ( _T("seagullbelowcmb") == wsName ) return 0x033C;
			else if ( _T("second") == wsName ) return 0x2033;
			else if ( _T("secondtonechinese") == wsName ) return 0x02CA;
			else if ( _T("section") == wsName ) return 0x00A7;
			else if ( _T("seenarabic") == wsName ) return 0x0633;
			else if ( _T("seenfinalarabic") == wsName ) return 0xFEB2;
			else if ( _T("seeninitialarabic") == wsName ) return 0xFEB3;
			else if ( _T("seenmedialarabic") == wsName ) return 0xFEB4;
			else if ( _T("segol") == wsName ) return 0x05B6;
			else if ( _T("segol13") == wsName ) return 0x05B6;
			else if ( _T("segol1f") == wsName ) return 0x05B6;
			else if ( _T("segol2c") == wsName ) return 0x05B6;
			else if ( _T("segolhebrew") == wsName ) return 0x05B6;
			else if ( _T("segolnarrowhebrew") == wsName ) return 0x05B6;
			else if ( _T("segolquarterhebrew") == wsName ) return 0x05B6;
			else if ( _T("segoltahebrew") == wsName ) return 0x0592;
			else if ( _T("segolwidehebrew") == wsName ) return 0x05B6;
			else if ( _T("seharmenian") == wsName ) return 0x057D;
			else if ( _T("sehiragana") == wsName ) return 0x305B;
			else if ( _T("sekatakana") == wsName ) return 0x30BB;
			else if ( _T("sekatakanahalfwidth") == wsName ) return 0xFF7E;
			else if ( _T("semicolon") == wsName ) return 0x003B;
			else if ( _T("semicolonarabic") == wsName ) return 0x061B;
			else if ( _T("semicolonmonospace") == wsName ) return 0xFF1B;
			else if ( _T("semicolonsmall") == wsName ) return 0xFE54;
			else if ( _T("semivoicedmarkkana") == wsName ) return 0x309C;
			else if ( _T("semivoicedmarkkanahalfwidth") == wsName ) return 0xFF9F;
			else if ( _T("sentisquare") == wsName ) return 0x3322;
			else if ( _T("sentosquare") == wsName ) return 0x3323;
			else if ( _T("seven") == wsName ) return 0x0037;
			else if ( _T("sevenarabic") == wsName ) return 0x0667;
			else if ( _T("sevenbengali") == wsName ) return 0x09ED;
			else if ( _T("sevencircle") == wsName ) return 0x2466;
			else if ( _T("sevencircleinversesansserif") == wsName ) return 0x2790;
			else if ( _T("sevendeva") == wsName ) return 0x096D;
			else if ( _T("seveneighths") == wsName ) return 0x215E;
			else if ( _T("sevengujarati") == wsName ) return 0x0AED;
			else if ( _T("sevengurmukhi") == wsName ) return 0x0A6D;
			else if ( _T("sevenhackarabic") == wsName ) return 0x0667;
			else if ( _T("sevenhangzhou") == wsName ) return 0x3027;
			else if ( _T("sevenideographicparen") == wsName ) return 0x3226;
			else if ( _T("seveninferior") == wsName ) return 0x2087;
			else if ( _T("sevenmonospace") == wsName ) return 0xFF17;
			else if ( _T("sevenoldstyle") == wsName ) return 0xF737;
			else if ( _T("sevenparen") == wsName ) return 0x247A;
			else if ( _T("sevenperiod") == wsName ) return 0x248E;
			else if ( _T("sevenpersian") == wsName ) return 0x06F7;
			else if ( _T("sevenroman") == wsName ) return 0x2176;
			else if ( _T("sevensuperior") == wsName ) return 0x2077;
			else if ( _T("seventeencircle") == wsName ) return 0x2470;
			else if ( _T("seventeenparen") == wsName ) return 0x2484;
			else if ( _T("seventeenperiod") == wsName ) return 0x2498;
			else if ( _T("seventhai") == wsName ) return 0x0E57;
			break;
		}
	case 'f':
		{
			if      ( _T("sfthyphen") == wsName ) return 0x00AD;
			break;
		}
	case 'h':
		{
			if      ( _T("shaarmenian") == wsName ) return 0x0577;
			else if ( _T("shabengali") == wsName ) return 0x09B6;
			else if ( _T("shacyrillic") == wsName ) return 0x0448;
			else if ( _T("shaddaarabic") == wsName ) return 0x0651;
			else if ( _T("shaddadammaarabic") == wsName ) return 0xFC61;
			else if ( _T("shaddadammatanarabic") == wsName ) return 0xFC5E;
			else if ( _T("shaddafathaarabic") == wsName ) return 0xFC60;
			else if ( _T("shaddafathatanarabic") == wsName ) return 0x0651064B;
			else if ( _T("shaddakasraarabic") == wsName ) return 0xFC62;
			else if ( _T("shaddakasratanarabic") == wsName ) return 0xFC5F;
			else if ( _T("shade") == wsName ) return 0x2592;
			else if ( _T("shadedark") == wsName ) return 0x2593;
			else if ( _T("shadelight") == wsName ) return 0x2591;
			else if ( _T("shademedium") == wsName ) return 0x2592;
			else if ( _T("shadeva") == wsName ) return 0x0936;
			else if ( _T("shagujarati") == wsName ) return 0x0AB6;
			else if ( _T("shagurmukhi") == wsName ) return 0x0A36;
			else if ( _T("shalshelethebrew") == wsName ) return 0x0593;
			else if ( _T("shbopomofo") == wsName ) return 0x3115;
			else if ( _T("shchacyrillic") == wsName ) return 0x0449;
			else if ( _T("sheenarabic") == wsName ) return 0x0634;
			else if ( _T("sheenfinalarabic") == wsName ) return 0xFEB6;
			else if ( _T("sheeninitialarabic") == wsName ) return 0xFEB7;
			else if ( _T("sheenmedialarabic") == wsName ) return 0xFEB8;
			else if ( _T("sheicoptic") == wsName ) return 0x03E3;
			else if ( _T("sheqel") == wsName ) return 0x20AA;
			else if ( _T("sheqelhebrew") == wsName ) return 0x20AA;
			else if ( _T("sheva") == wsName ) return 0x05B0;
			else if ( _T("sheva115") == wsName ) return 0x05B0;
			else if ( _T("sheva15") == wsName ) return 0x05B0;
			else if ( _T("sheva22") == wsName ) return 0x05B0;
			else if ( _T("sheva2e") == wsName ) return 0x05B0;
			else if ( _T("shevahebrew") == wsName ) return 0x05B0;
			else if ( _T("shevanarrowhebrew") == wsName ) return 0x05B0;
			else if ( _T("shevaquarterhebrew") == wsName ) return 0x05B0;
			else if ( _T("shevawidehebrew") == wsName ) return 0x05B0;
			else if ( _T("shhacyrillic") == wsName ) return 0x04BB;
			else if ( _T("shimacoptic") == wsName ) return 0x03ED;
			else if ( _T("shin") == wsName ) return 0x05E9;
			else if ( _T("shindagesh") == wsName ) return 0xFB49;
			else if ( _T("shindageshhebrew") == wsName ) return 0xFB49;
			else if ( _T("shindageshshindot") == wsName ) return 0xFB2C;
			else if ( _T("shindageshshindothebrew") == wsName ) return 0xFB2C;
			else if ( _T("shindageshsindot") == wsName ) return 0xFB2D;
			else if ( _T("shindageshsindothebrew") == wsName ) return 0xFB2D;
			else if ( _T("shindothebrew") == wsName ) return 0x05C1;
			else if ( _T("shinhebrew") == wsName ) return 0x05E9;
			else if ( _T("shinshindot") == wsName ) return 0xFB2A;
			else if ( _T("shinshindothebrew") == wsName ) return 0xFB2A;
			else if ( _T("shinsindot") == wsName ) return 0xFB2B;
			else if ( _T("shinsindothebrew") == wsName ) return 0xFB2B;
			else if ( _T("shook") == wsName ) return 0x0282;
			break;
		}
	case 'i':
		{
			if      ( _T("sigma") == wsName ) return 0x03C3;
			else if ( _T("sigma1") == wsName ) return 0x03C2;
			else if ( _T("sigmafinal") == wsName ) return 0x03C2;
			else if ( _T("sigmalunatesymbolgreek") == wsName ) return 0x03F2;
			else if ( _T("sihiragana") == wsName ) return 0x3057;
			else if ( _T("sikatakana") == wsName ) return 0x30B7;
			else if ( _T("sikatakanahalfwidth") == wsName ) return 0xFF7C;
			else if ( _T("siluqhebrew") == wsName ) return 0x05BD;
			else if ( _T("siluqlefthebrew") == wsName ) return 0x05BD;
			else if ( _T("similar") == wsName ) return 0x223C;
			else if ( _T("sindothebrew") == wsName ) return 0x05C2;
			else if ( _T("siosacirclekorean") == wsName ) return 0x3274;
			else if ( _T("siosaparenkorean") == wsName ) return 0x3214;
			else if ( _T("sioscieuckorean") == wsName ) return 0x317E;
			else if ( _T("sioscirclekorean") == wsName ) return 0x3266;
			else if ( _T("sioskiyeokkorean") == wsName ) return 0x317A;
			else if ( _T("sioskorean") == wsName ) return 0x3145;
			else if ( _T("siosnieunkorean") == wsName ) return 0x317B;
			else if ( _T("siosparenkorean") == wsName ) return 0x3206;
			else if ( _T("siospieupkorean") == wsName ) return 0x317D;
			else if ( _T("siostikeutkorean") == wsName ) return 0x317C;
			else if ( _T("six") == wsName ) return 0x0036;
			else if ( _T("sixarabic") == wsName ) return 0x0666;
			else if ( _T("sixbengali") == wsName ) return 0x09EC;
			else if ( _T("sixcircle") == wsName ) return 0x2465;
			else if ( _T("sixcircleinversesansserif") == wsName ) return 0x278F;
			else if ( _T("sixdeva") == wsName ) return 0x096C;
			else if ( _T("sixgujarati") == wsName ) return 0x0AEC;
			else if ( _T("sixgurmukhi") == wsName ) return 0x0A6C;
			else if ( _T("sixhackarabic") == wsName ) return 0x0666;
			else if ( _T("sixhangzhou") == wsName ) return 0x3026;
			else if ( _T("sixideographicparen") == wsName ) return 0x3225;
			else if ( _T("sixinferior") == wsName ) return 0x2086;
			else if ( _T("sixmonospace") == wsName ) return 0xFF16;
			else if ( _T("sixoldstyle") == wsName ) return 0xF736;
			else if ( _T("sixparen") == wsName ) return 0x2479;
			else if ( _T("sixperiod") == wsName ) return 0x248D;
			else if ( _T("sixpersian") == wsName ) return 0x06F6;
			else if ( _T("sixroman") == wsName ) return 0x2175;
			else if ( _T("sixsuperior") == wsName ) return 0x2076;
			else if ( _T("sixteencircle") == wsName ) return 0x246F;
			else if ( _T("sixteencurrencydenominatorbengali") == wsName ) return 0x09F9;
			else if ( _T("sixteenparen") == wsName ) return 0x2483;
			else if ( _T("sixteenperiod") == wsName ) return 0x2497;
			else if ( _T("sixthai") == wsName ) return 0x0E56;
			break;
		}
	case 'l':
		{
			if      ( _T("slash") == wsName ) return 0x002F;
			else if ( _T("slashmonospace") == wsName ) return 0xFF0F;
			else if ( _T("slong") == wsName ) return 0x017F;
			else if ( _T("slongdotaccent") == wsName ) return 0x1E9B;
			break;
		}
	case 'm':
		{
			if      ( _T("smileface") == wsName ) return 0x263A;
			else if ( _T("smonospace") == wsName ) return 0xFF53;
			break;
		}
	case 'o':
		{
			if      ( _T("sofpasuqhebrew") == wsName ) return 0x05C3;
			else if ( _T("softhyphen") == wsName ) return 0x00AD;
			else if ( _T("softsigncyrillic") == wsName ) return 0x044C;
			else if ( _T("sohiragana") == wsName ) return 0x305D;
			else if ( _T("sokatakana") == wsName ) return 0x30BD;
			else if ( _T("sokatakanahalfwidth") == wsName ) return 0xFF7F;
			else if ( _T("soliduslongoverlaycmb") == wsName ) return 0x0338;
			else if ( _T("solidusshortoverlaycmb") == wsName ) return 0x0337;
			else if ( _T("sorusithai") == wsName ) return 0x0E29;
			else if ( _T("sosalathai") == wsName ) return 0x0E28;
			else if ( _T("sosothai") == wsName ) return 0x0E0B;
			else if ( _T("sosuathai") == wsName ) return 0x0E2A;
			break;
		}
	case 'p':
		{
			if      ( _T("space") == wsName ) return 0x0020;
			else if ( _T("spacehackarabic") == wsName ) return 0x0020;
			else if ( _T("spade") == wsName ) return 0x2660;
			else if ( _T("spadesuitblack") == wsName ) return 0x2660;
			else if ( _T("spadesuitwhite") == wsName ) return 0x2664;
			else if ( _T("sparen") == wsName ) return 0x24AE;
			break;
		}
	case 'q':
		{
			if      ( _T("squarebelowcmb") == wsName ) return 0x033B;
			else if ( _T("squarecc") == wsName ) return 0x33C4;
			else if ( _T("squarecm") == wsName ) return 0x339D;
			else if ( _T("squarediagonalcrosshatchfill") == wsName ) return 0x25A9;
			else if ( _T("squarehorizontalfill") == wsName ) return 0x25A4;
			else if ( _T("squarekg") == wsName ) return 0x338F;
			else if ( _T("squarekm") == wsName ) return 0x339E;
			else if ( _T("squarekmcapital") == wsName ) return 0x33CE;
			else if ( _T("squareln") == wsName ) return 0x33D1;
			else if ( _T("squarelog") == wsName ) return 0x33D2;
			else if ( _T("squaremg") == wsName ) return 0x338E;
			else if ( _T("squaremil") == wsName ) return 0x33D5;
			else if ( _T("squaremm") == wsName ) return 0x339C;
			else if ( _T("squaremsquared") == wsName ) return 0x33A1;
			else if ( _T("squareorthogonalcrosshatchfill") == wsName ) return 0x25A6;
			else if ( _T("squareupperlefttolowerrightfill") == wsName ) return 0x25A7;
			else if ( _T("squareupperrighttolowerleftfill") == wsName ) return 0x25A8;
			else if ( _T("squareverticalfill") == wsName ) return 0x25A5;
			else if ( _T("squarewhitewithsmallblack") == wsName ) return 0x25A3;
			break;
		}
	case 'r':
		{
			if      ( _T("srsquare") == wsName ) return 0x33DB;
			break;
		}
	case 's':
		{
			if      ( _T("ssabengali") == wsName ) return 0x09B7;
			else if ( _T("ssadeva") == wsName ) return 0x0937;
			else if ( _T("ssagujarati") == wsName ) return 0x0AB7;
			else if ( _T("ssangcieuckorean") == wsName ) return 0x3149;
			else if ( _T("ssanghieuhkorean") == wsName ) return 0x3185;
			else if ( _T("ssangieungkorean") == wsName ) return 0x3180;
			else if ( _T("ssangkiyeokkorean") == wsName ) return 0x3132;
			else if ( _T("ssangnieunkorean") == wsName ) return 0x3165;
			else if ( _T("ssangpieupkorean") == wsName ) return 0x3143;
			else if ( _T("ssangsioskorean") == wsName ) return 0x3146;
			else if ( _T("ssangtikeutkorean") == wsName ) return 0x3138;
			else if ( _T("ssuperior") == wsName ) return 0xF6F2;
			break;
		}
	case 't':
		{
			if      ( _T("sterling") == wsName ) return 0x00A3;
			else if ( _T("sterlingmonospace") == wsName ) return 0xFFE1;
			else if ( _T("strokelongoverlaycmb") == wsName ) return 0x0336;
			else if ( _T("strokeshortoverlaycmb") == wsName ) return 0x0335;
			break;
		}
	case 'u':
		{
			if      ( _T("subset") == wsName ) return 0x2282;
			else if ( _T("subsetnotequal") == wsName ) return 0x228A;
			else if ( _T("subsetorequal") == wsName ) return 0x2286;
			else if ( _T("succeeds") == wsName ) return 0x227B;
			else if ( _T("suchthat") == wsName ) return 0x220B;
			else if ( _T("suhiragana") == wsName ) return 0x3059;
			else if ( _T("sukatakana") == wsName ) return 0x30B9;
			else if ( _T("sukatakanahalfwidth") == wsName ) return 0xFF7D;
			else if ( _T("sukunarabic") == wsName ) return 0x0652;
			else if ( _T("summation") == wsName ) return 0x2211;
			else if ( _T("sun") == wsName ) return 0x263C;
			else if ( _T("superset") == wsName ) return 0x2283;
			else if ( _T("supersetnotequal") == wsName ) return 0x228B;
			else if ( _T("supersetorequal") == wsName ) return 0x2287;
			break;
		}
	case 'v':
		{
			if      ( _T("svsquare") == wsName ) return 0x33DC;
			break;
		}
	case 'y':
		{
			if      ( _T("syouwaerasquare") == wsName ) return 0x337C;
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
			if      ( _T("t") == wsName ) return 0x0074;
			break;
		}
	case 'a':
		{
			if      ( _T("tabengali") == wsName ) return 0x09A4;
			else if ( _T("tackdown") == wsName ) return 0x22A4;
			else if ( _T("tackleft") == wsName ) return 0x22A3;
			else if ( _T("tadeva") == wsName ) return 0x0924;
			else if ( _T("tagujarati") == wsName ) return 0x0AA4;
			else if ( _T("tagurmukhi") == wsName ) return 0x0A24;
			else if ( _T("taharabic") == wsName ) return 0x0637;
			else if ( _T("tahfinalarabic") == wsName ) return 0xFEC2;
			else if ( _T("tahinitialarabic") == wsName ) return 0xFEC3;
			else if ( _T("tahiragana") == wsName ) return 0x305F;
			else if ( _T("tahmedialarabic") == wsName ) return 0xFEC4;
			else if ( _T("taisyouerasquare") == wsName ) return 0x337D;
			else if ( _T("takatakana") == wsName ) return 0x30BF;
			else if ( _T("takatakanahalfwidth") == wsName ) return 0xFF80;
			else if ( _T("tatweelarabic") == wsName ) return 0x0640;
			else if ( _T("tau") == wsName ) return 0x03C4;
			else if ( _T("tav") == wsName ) return 0x05EA;
			else if ( _T("tavdages") == wsName ) return 0xFB4A;
			else if ( _T("tavdagesh") == wsName ) return 0xFB4A;
			else if ( _T("tavdageshhebrew") == wsName ) return 0xFB4A;
			else if ( _T("tavhebrew") == wsName ) return 0x05EA;
			break;
		}
	case 'b':
		{
			if      ( _T("tbar") == wsName ) return 0x0167;
			else if ( _T("tbopomofo") == wsName ) return 0x310A;
			break;
		}
	case 'c':
		{
			if      ( _T("tcaron") == wsName ) return 0x0165;
			else if ( _T("tccurl") == wsName ) return 0x02A8;
			else if ( _T("tcedilla") == wsName ) return 0x0163;
			else if ( _T("tcheharabic") == wsName ) return 0x0686;
			else if ( _T("tchehfinalarabic") == wsName ) return 0xFB7B;
			else if ( _T("tchehinitialarabic") == wsName ) return 0xFB7C;
			else if ( _T("tchehmedialarabic") == wsName ) return 0xFB7D;
			else if ( _T("tchehmeeminitialarabic") == wsName ) return 0xFB7CFEE4;
			else if ( _T("tcircle") == wsName ) return 0x24E3;
			else if ( _T("tcircumflexbelow") == wsName ) return 0x1E71;
			else if ( _T("tcommaaccent") == wsName ) return 0x0163;
			break;
		}
	case 'd':
		{
			if      ( _T("tdieresis") == wsName ) return 0x1E97;
			else if ( _T("tdotaccent") == wsName ) return 0x1E6B;
			else if ( _T("tdotbelow") == wsName ) return 0x1E6D;
			break;
		}
	case 'e':
		{
			if      ( _T("tecyrillic") == wsName ) return 0x0442;
			else if ( _T("tedescendercyrillic") == wsName ) return 0x04AD;
			else if ( _T("teharabic") == wsName ) return 0x062A;
			else if ( _T("tehfinalarabic") == wsName ) return 0xFE96;
			else if ( _T("tehhahinitialarabic") == wsName ) return 0xFCA2;
			else if ( _T("tehhahisolatedarabic") == wsName ) return 0xFC0C;
			else if ( _T("tehinitialarabic") == wsName ) return 0xFE97;
			else if ( _T("tehiragana") == wsName ) return 0x3066;
			else if ( _T("tehjeeminitialarabic") == wsName ) return 0xFCA1;
			else if ( _T("tehjeemisolatedarabic") == wsName ) return 0xFC0B;
			else if ( _T("tehmarbutaarabic") == wsName ) return 0x0629;
			else if ( _T("tehmarbutafinalarabic") == wsName ) return 0xFE94;
			else if ( _T("tehmedialarabic") == wsName ) return 0xFE98;
			else if ( _T("tehmeeminitialarabic") == wsName ) return 0xFCA4;
			else if ( _T("tehmeemisolatedarabic") == wsName ) return 0xFC0E;
			else if ( _T("tehnoonfinalarabic") == wsName ) return 0xFC73;
			else if ( _T("tekatakana") == wsName ) return 0x30C6;
			else if ( _T("tekatakanahalfwidth") == wsName ) return 0xFF83;
			else if ( _T("telephone") == wsName ) return 0x2121;
			else if ( _T("telephoneblack") == wsName ) return 0x260E;
			else if ( _T("telishagedolahebrew") == wsName ) return 0x05A0;
			else if ( _T("telishaqetanahebrew") == wsName ) return 0x05A9;
			else if ( _T("tencircle") == wsName ) return 0x2469;
			else if ( _T("tenideographicparen") == wsName ) return 0x3229;
			else if ( _T("tenparen") == wsName ) return 0x247D;
			else if ( _T("tenperiod") == wsName ) return 0x2491;
			else if ( _T("tenroman") == wsName ) return 0x2179;
			else if ( _T("tesh") == wsName ) return 0x02A7;
			else if ( _T("tet") == wsName ) return 0x05D8;
			else if ( _T("tetdagesh") == wsName ) return 0xFB38;
			else if ( _T("tetdageshhebrew") == wsName ) return 0xFB38;
			else if ( _T("tethebrew") == wsName ) return 0x05D8;
			else if ( _T("tetsecyrillic") == wsName ) return 0x04B5;
			else if ( _T("tevirhebrew") == wsName ) return 0x059B;
			else if ( _T("tevirlefthebrew") == wsName ) return 0x059B;
			break;
		}
	case 'h':
		{
			if      ( _T("thabengali") == wsName ) return 0x09A5;
			else if ( _T("thadeva") == wsName ) return 0x0925;
			else if ( _T("thagujarati") == wsName ) return 0x0AA5;
			else if ( _T("thagurmukhi") == wsName ) return 0x0A25;
			else if ( _T("thalarabic") == wsName ) return 0x0630;
			else if ( _T("thalfinalarabic") == wsName ) return 0xFEAC;
			else if ( _T("thanthakhatlowleftthai") == wsName ) return 0xF898;
			else if ( _T("thanthakhatlowrightthai") == wsName ) return 0xF897;
			else if ( _T("thanthakhatthai") == wsName ) return 0x0E4C;
			else if ( _T("thanthakhatupperleftthai") == wsName ) return 0xF896;
			else if ( _T("theharabic") == wsName ) return 0x062B;
			else if ( _T("thehfinalarabic") == wsName ) return 0xFE9A;
			else if ( _T("thehinitialarabic") == wsName ) return 0xFE9B;
			else if ( _T("thehmedialarabic") == wsName ) return 0xFE9C;
			else if ( _T("thereexists") == wsName ) return 0x2203;
			else if ( _T("therefore") == wsName ) return 0x2234;
			else if ( _T("theta") == wsName ) return 0x03B8;
			else if ( _T("theta1") == wsName ) return 0x03D1;
			else if ( _T("thetasymbolgreek") == wsName ) return 0x03D1;
			else if ( _T("thieuthacirclekorean") == wsName ) return 0x3279;
			else if ( _T("thieuthaparenkorean") == wsName ) return 0x3219;
			else if ( _T("thieuthcirclekorean") == wsName ) return 0x326B;
			else if ( _T("thieuthkorean") == wsName ) return 0x314C;
			else if ( _T("thieuthparenkorean") == wsName ) return 0x320B;
			else if ( _T("thirteencircle") == wsName ) return 0x246C;
			else if ( _T("thirteenparen") == wsName ) return 0x2480;
			else if ( _T("thirteenperiod") == wsName ) return 0x2494;
			else if ( _T("thonangmonthothai") == wsName ) return 0x0E11;
			else if ( _T("thook") == wsName ) return 0x01AD;
			else if ( _T("thophuthaothai") == wsName ) return 0x0E12;
			else if ( _T("thorn") == wsName ) return 0x00FE;
			else if ( _T("thothahanthai") == wsName ) return 0x0E17;
			else if ( _T("thothanthai") == wsName ) return 0x0E10;
			else if ( _T("thothongthai") == wsName ) return 0x0E18;
			else if ( _T("thothungthai") == wsName ) return 0x0E16;
			else if ( _T("thousandcyrillic") == wsName ) return 0x0482;
			else if ( _T("thousandsseparatorarabic") == wsName ) return 0x066C;
			else if ( _T("thousandsseparatorpersian") == wsName ) return 0x066C;
			else if ( _T("three") == wsName ) return 0x0033;
			else if ( _T("threearabic") == wsName ) return 0x0663;
			else if ( _T("threebengali") == wsName ) return 0x09E9;
			else if ( _T("threecircle") == wsName ) return 0x2462;
			else if ( _T("threecircleinversesansserif") == wsName ) return 0x278C;
			else if ( _T("threedeva") == wsName ) return 0x0969;
			else if ( _T("threeeighths") == wsName ) return 0x215C;
			else if ( _T("threegujarati") == wsName ) return 0x0AE9;
			else if ( _T("threegurmukhi") == wsName ) return 0x0A69;
			else if ( _T("threehackarabic") == wsName ) return 0x0663;
			else if ( _T("threehangzhou") == wsName ) return 0x3023;
			else if ( _T("threeideographicparen") == wsName ) return 0x3222;
			else if ( _T("threeinferior") == wsName ) return 0x2083;
			else if ( _T("threemonospace") == wsName ) return 0xFF13;
			else if ( _T("threenumeratorbengali") == wsName ) return 0x09F6;
			else if ( _T("threeoldstyle") == wsName ) return 0xF733;
			else if ( _T("threeparen") == wsName ) return 0x2476;
			else if ( _T("threeperiod") == wsName ) return 0x248A;
			else if ( _T("threepersian") == wsName ) return 0x06F3;
			else if ( _T("threequarters") == wsName ) return 0x00BE;
			else if ( _T("threequartersemdash") == wsName ) return 0xF6DE;
			else if ( _T("threeroman") == wsName ) return 0x2172;
			else if ( _T("threesuperior") == wsName ) return 0x00B3;
			else if ( _T("threethai") == wsName ) return 0x0E53;
			else if ( _T("thzsquare") == wsName ) return 0x3394;
			break;
		}
	case 'i':
		{
			if      ( _T("tihiragana") == wsName ) return 0x3061;
			else if ( _T("tikatakana") == wsName ) return 0x30C1;
			else if ( _T("tikatakanahalfwidth") == wsName ) return 0xFF81;
			else if ( _T("tikeutacirclekorean") == wsName ) return 0x3270;
			else if ( _T("tikeutaparenkorean") == wsName ) return 0x3210;
			else if ( _T("tikeutcirclekorean") == wsName ) return 0x3262;
			else if ( _T("tikeutkorean") == wsName ) return 0x3137;
			else if ( _T("tikeutparenkorean") == wsName ) return 0x3202;
			else if ( _T("tilde") == wsName ) return 0x02DC;
			else if ( _T("tildebelowcmb") == wsName ) return 0x0330;
			else if ( _T("tildecmb") == wsName ) return 0x0303;
			else if ( _T("tildecomb") == wsName ) return 0x0303;
			else if ( _T("tildedoublecmb") == wsName ) return 0x0360;
			else if ( _T("tildeoperator") == wsName ) return 0x223C;
			else if ( _T("tildeoverlaycmb") == wsName ) return 0x0334;
			else if ( _T("tildeverticalcmb") == wsName ) return 0x033E;
			else if ( _T("timescircle") == wsName ) return 0x2297;
			else if ( _T("tipehahebrew") == wsName ) return 0x0596;
			else if ( _T("tipehalefthebrew") == wsName ) return 0x0596;
			else if ( _T("tippigurmukhi") == wsName ) return 0x0A70;
			else if ( _T("titlocyrilliccmb") == wsName ) return 0x0483;
			else if ( _T("tiwnarmenian") == wsName ) return 0x057F;
			break;
		}
	case 'l':
		{
			if      ( _T("tlinebelow") == wsName ) return 0x1E6F;
			break;
		}
	case 'm':
		{
			if      ( _T("tmonospace") == wsName ) return 0xFF54;
			break;
		}
	case 'o':
		{
			if      ( _T("toarmenian") == wsName ) return 0x0569;
			else if ( _T("tohiragana") == wsName ) return 0x3068;
			else if ( _T("tokatakana") == wsName ) return 0x30C8;
			else if ( _T("tokatakanahalfwidth") == wsName ) return 0xFF84;
			else if ( _T("tonebarextrahighmod") == wsName ) return 0x02E5;
			else if ( _T("tonebarextralowmod") == wsName ) return 0x02E9;
			else if ( _T("tonebarhighmod") == wsName ) return 0x02E6;
			else if ( _T("tonebarlowmod") == wsName ) return 0x02E8;
			else if ( _T("tonebarmidmod") == wsName ) return 0x02E7;
			else if ( _T("tonefive") == wsName ) return 0x01BD;
			else if ( _T("tonesix") == wsName ) return 0x0185;
			else if ( _T("tonetwo") == wsName ) return 0x01A8;
			else if ( _T("tonos") == wsName ) return 0x0384;
			else if ( _T("tonsquare") == wsName ) return 0x3327;
			else if ( _T("topatakthai") == wsName ) return 0x0E0F;
			else if ( _T("tortoiseshellbracketleft") == wsName ) return 0x3014;
			else if ( _T("tortoiseshellbracketleftsmall") == wsName ) return 0xFE5D;
			else if ( _T("tortoiseshellbracketleftvertical") == wsName ) return 0xFE39;
			else if ( _T("tortoiseshellbracketright") == wsName ) return 0x3015;
			else if ( _T("tortoiseshellbracketrightsmall") == wsName ) return 0xFE5E;
			else if ( _T("tortoiseshellbracketrightvertical") == wsName ) return 0xFE3A;
			else if ( _T("totaothai") == wsName ) return 0x0E15;
			break;
		}
	case 'p':
		{
			if      ( _T("tpalatalhook") == wsName ) return 0x01AB;
			else if ( _T("tparen") == wsName ) return 0x24AF;
			break;
		}
	case 'r':
		{
			if      ( _T("trademark") == wsName ) return 0x2122;
			else if ( _T("trademarksans") == wsName ) return 0xF8EA;
			else if ( _T("trademarkserif") == wsName ) return 0xF6DB;
			else if ( _T("tretroflexhook") == wsName ) return 0x0288;
			else if ( _T("triagdn") == wsName ) return 0x25BC;
			else if ( _T("triaglf") == wsName ) return 0x25C4;
			else if ( _T("triagrt") == wsName ) return 0x25BA;
			else if ( _T("triagup") == wsName ) return 0x25B2;
			break;
		}
	case 's':
		{
			if      ( _T("ts") == wsName ) return 0x02A6;
			else if ( _T("tsadi") == wsName ) return 0x05E6;
			else if ( _T("tsadidagesh") == wsName ) return 0xFB46;
			else if ( _T("tsadidageshhebrew") == wsName ) return 0xFB46;
			else if ( _T("tsadihebrew") == wsName ) return 0x05E6;
			else if ( _T("tsecyrillic") == wsName ) return 0x0446;
			else if ( _T("tsere") == wsName ) return 0x05B5;
			else if ( _T("tsere12") == wsName ) return 0x05B5;
			else if ( _T("tsere1e") == wsName ) return 0x05B5;
			else if ( _T("tsere2b") == wsName ) return 0x05B5;
			else if ( _T("tserehebrew") == wsName ) return 0x05B5;
			else if ( _T("tserenarrowhebrew") == wsName ) return 0x05B5;
			else if ( _T("tserequarterhebrew") == wsName ) return 0x05B5;
			else if ( _T("tserewidehebrew") == wsName ) return 0x05B5;
			else if ( _T("tshecyrillic") == wsName ) return 0x045B;
			else if ( _T("tsuperior") == wsName ) return 0xF6F3;
			break;
		}
	case 't':
		{
			if      ( _T("ttabengali") == wsName ) return 0x099F;
			else if ( _T("ttadeva") == wsName ) return 0x091F;
			else if ( _T("ttagujarati") == wsName ) return 0x0A9F;
			else if ( _T("ttagurmukhi") == wsName ) return 0x0A1F;
			else if ( _T("tteharabic") == wsName ) return 0x0679;
			else if ( _T("ttehfinalarabic") == wsName ) return 0xFB67;
			else if ( _T("ttehinitialarabic") == wsName ) return 0xFB68;
			else if ( _T("ttehmedialarabic") == wsName ) return 0xFB69;
			else if ( _T("tthabengali") == wsName ) return 0x09A0;
			else if ( _T("tthadeva") == wsName ) return 0x0920;
			else if ( _T("tthagujarati") == wsName ) return 0x0AA0;
			else if ( _T("tthagurmukhi") == wsName ) return 0x0A20;
			else if ( _T("tturned") == wsName ) return 0x0287;
			break;
		}
	case 'u':
		{
			if      ( _T("tuhiragana") == wsName ) return 0x3064;
			else if ( _T("tukatakana") == wsName ) return 0x30C4;
			else if ( _T("tukatakanahalfwidth") == wsName ) return 0xFF82;
			else if ( _T("tusmallhiragana") == wsName ) return 0x3063;
			else if ( _T("tusmallkatakana") == wsName ) return 0x30C3;
			else if ( _T("tusmallkatakanahalfwidth") == wsName ) return 0xFF6F;
			break;
		}
	case 'w':
		{
			if      ( _T("twelvecircle") == wsName ) return 0x246B;
			else if ( _T("twelveparen") == wsName ) return 0x247F;
			else if ( _T("twelveperiod") == wsName ) return 0x2493;
			else if ( _T("twelveroman") == wsName ) return 0x217B;
			else if ( _T("twentycircle") == wsName ) return 0x2473;
			else if ( _T("twentyhangzhou") == wsName ) return 0x5344;
			else if ( _T("twentyparen") == wsName ) return 0x2487;
			else if ( _T("twentyperiod") == wsName ) return 0x249B;
			else if ( _T("two") == wsName ) return 0x0032;
			else if ( _T("twoarabic") == wsName ) return 0x0662;
			else if ( _T("twobengali") == wsName ) return 0x09E8;
			else if ( _T("twocircle") == wsName ) return 0x2461;
			else if ( _T("twocircleinversesansserif") == wsName ) return 0x278B;
			else if ( _T("twodeva") == wsName ) return 0x0968;
			else if ( _T("twodotenleader") == wsName ) return 0x2025;
			else if ( _T("twodotleader") == wsName ) return 0x2025;
			else if ( _T("twodotleadervertical") == wsName ) return 0xFE30;
			else if ( _T("twogujarati") == wsName ) return 0x0AE8;
			else if ( _T("twogurmukhi") == wsName ) return 0x0A68;
			else if ( _T("twohackarabic") == wsName ) return 0x0662;
			else if ( _T("twohangzhou") == wsName ) return 0x3022;
			else if ( _T("twoideographicparen") == wsName ) return 0x3221;
			else if ( _T("twoinferior") == wsName ) return 0x2082;
			else if ( _T("twomonospace") == wsName ) return 0xFF12;
			else if ( _T("twonumeratorbengali") == wsName ) return 0x09F5;
			else if ( _T("twooldstyle") == wsName ) return 0xF732;
			else if ( _T("twoparen") == wsName ) return 0x2475;
			else if ( _T("twoperiod") == wsName ) return 0x2489;
			else if ( _T("twopersian") == wsName ) return 0x06F2;
			else if ( _T("tworoman") == wsName ) return 0x2171;
			else if ( _T("twostroke") == wsName ) return 0x01BB;
			else if ( _T("twosuperior") == wsName ) return 0x00B2;
			else if ( _T("twothai") == wsName ) return 0x0E52;
			else if ( _T("twothirds") == wsName ) return 0x2154;
			break;
		}
	}

	return 0;
}
static int Type1NameToUnicode_u(CWCharWrapper &wsName)
{
	if      ( _T("u") == wsName ) return 0x0075;
	else if ( _T("uacute") == wsName ) return 0x00FA;
	else if ( _T("ubar") == wsName ) return 0x0289;
	else if ( _T("ubengali") == wsName ) return 0x0989;
	else if ( _T("ubopomofo") == wsName ) return 0x3128;
	else if ( _T("ubreve") == wsName ) return 0x016D;
	else if ( _T("ucaron") == wsName ) return 0x01D4;
	else if ( _T("ucircle") == wsName ) return 0x24E4;
	else if ( _T("ucircumflex") == wsName ) return 0x00FB;
	else if ( _T("ucircumflexbelow") == wsName ) return 0x1E77;
	else if ( _T("ucyrillic") == wsName ) return 0x0443;
	else if ( _T("udattadeva") == wsName ) return 0x0951;
	else if ( _T("udblacute") == wsName ) return 0x0171;
	else if ( _T("udblgrave") == wsName ) return 0x0215;
	else if ( _T("udeva") == wsName ) return 0x0909;
	else if ( _T("udieresis") == wsName ) return 0x00FC;
	else if ( _T("udieresisacute") == wsName ) return 0x01D8;
	else if ( _T("udieresisbelow") == wsName ) return 0x1E73;
	else if ( _T("udieresiscaron") == wsName ) return 0x01DA;
	else if ( _T("udieresiscyrillic") == wsName ) return 0x04F1;
	else if ( _T("udieresisgrave") == wsName ) return 0x01DC;
	else if ( _T("udieresismacron") == wsName ) return 0x01D6;
	else if ( _T("udotbelow") == wsName ) return 0x1EE5;
	else if ( _T("ugrave") == wsName ) return 0x00F9;
	else if ( _T("ugujarati") == wsName ) return 0x0A89;
	else if ( _T("ugurmukhi") == wsName ) return 0x0A09;
	else if ( _T("uhiragana") == wsName ) return 0x3046;
	else if ( _T("uhookabove") == wsName ) return 0x1EE7;
	else if ( _T("uhorn") == wsName ) return 0x01B0;
	else if ( _T("uhornacute") == wsName ) return 0x1EE9;
	else if ( _T("uhorndotbelow") == wsName ) return 0x1EF1;
	else if ( _T("uhorngrave") == wsName ) return 0x1EEB;
	else if ( _T("uhornhookabove") == wsName ) return 0x1EED;
	else if ( _T("uhorntilde") == wsName ) return 0x1EEF;
	else if ( _T("uhungarumlaut") == wsName ) return 0x0171;
	else if ( _T("uhungarumlautcyrillic") == wsName ) return 0x04F3;
	else if ( _T("uinvertedbreve") == wsName ) return 0x0217;
	else if ( _T("ukatakana") == wsName ) return 0x30A6;
	else if ( _T("ukatakanahalfwidth") == wsName ) return 0xFF73;
	else if ( _T("ukcyrillic") == wsName ) return 0x0479;
	else if ( _T("ukorean") == wsName ) return 0x315C;
	else if ( _T("umacron") == wsName ) return 0x016B;
	else if ( _T("umacroncyrillic") == wsName ) return 0x04EF;
	else if ( _T("umacrondieresis") == wsName ) return 0x1E7B;
	else if ( _T("umatragurmukhi") == wsName ) return 0x0A41;
	else if ( _T("umonospace") == wsName ) return 0xFF55;
	else if ( _T("underscore") == wsName ) return 0x005F;
	else if ( _T("underscoredbl") == wsName ) return 0x2017;
	else if ( _T("underscoremonospace") == wsName ) return 0xFF3F;
	else if ( _T("underscorevertical") == wsName ) return 0xFE33;
	else if ( _T("underscorewavy") == wsName ) return 0xFE4F;
	else if ( _T("union") == wsName ) return 0x222A;
	else if ( _T("universal") == wsName ) return 0x2200;
	else if ( _T("uogonek") == wsName ) return 0x0173;
	else if ( _T("uparen") == wsName ) return 0x24B0;
	else if ( _T("upblock") == wsName ) return 0x2580;
	else if ( _T("upperdothebrew") == wsName ) return 0x05C4;
	else if ( _T("upsilon") == wsName ) return 0x03C5;
	else if ( _T("upsilondieresis") == wsName ) return 0x03CB;
	else if ( _T("upsilondieresistonos") == wsName ) return 0x03B0;
	else if ( _T("upsilonlatin") == wsName ) return 0x028A;
	else if ( _T("upsilontonos") == wsName ) return 0x03CD;
	else if ( _T("uptackbelowcmb") == wsName ) return 0x031D;
	else if ( _T("uptackmod") == wsName ) return 0x02D4;
	else if ( _T("uragurmukhi") == wsName ) return 0x0A73;
	else if ( _T("uring") == wsName ) return 0x016F;
	else if ( _T("ushortcyrillic") == wsName ) return 0x045E;
	else if ( _T("usmallhiragana") == wsName ) return 0x3045;
	else if ( _T("usmallkatakana") == wsName ) return 0x30A5;
	else if ( _T("usmallkatakanahalfwidth") == wsName ) return 0xFF69;
	else if ( _T("ustraightcyrillic") == wsName ) return 0x04AF;
	else if ( _T("ustraightstrokecyrillic") == wsName ) return 0x04B1;
	else if ( _T("utilde") == wsName ) return 0x0169;
	else if ( _T("utildeacute") == wsName ) return 0x1E79;
	else if ( _T("utildebelow") == wsName ) return 0x1E75;
	else if ( _T("uubengali") == wsName ) return 0x098A;
	else if ( _T("uudeva") == wsName ) return 0x090A;
	else if ( _T("uugujarati") == wsName ) return 0x0A8A;
	else if ( _T("uugurmukhi") == wsName ) return 0x0A0A;
	else if ( _T("uumatragurmukhi") == wsName ) return 0x0A42;
	else if ( _T("uuvowelsignbengali") == wsName ) return 0x09C2;
	else if ( _T("uuvowelsigndeva") == wsName ) return 0x0942;
	else if ( _T("uuvowelsigngujarati") == wsName ) return 0x0AC2;
	else if ( _T("uvowelsignbengali") == wsName ) return 0x09C1;
	else if ( _T("uvowelsigndeva") == wsName ) return 0x0941;
	else if ( _T("uvowelsigngujarati") == wsName ) return 0x0AC1;

	return 0;
}
static int Type1NameToUnicode_v(CWCharWrapper &wsName)
{
	if      ( _T("v") == wsName ) return 0x0076;
	else if ( _T("vadeva") == wsName ) return 0x0935;
	else if ( _T("vagujarati") == wsName ) return 0x0AB5;
	else if ( _T("vagurmukhi") == wsName ) return 0x0A35;
	else if ( _T("vakatakana") == wsName ) return 0x30F7;
	else if ( _T("vav") == wsName ) return 0x05D5;
	else if ( _T("vavdagesh") == wsName ) return 0xFB35;
	else if ( _T("vavdagesh65") == wsName ) return 0xFB35;
	else if ( _T("vavdageshhebrew") == wsName ) return 0xFB35;
	else if ( _T("vavhebrew") == wsName ) return 0x05D5;
	else if ( _T("vavholam") == wsName ) return 0xFB4B;
	else if ( _T("vavholamhebrew") == wsName ) return 0xFB4B;
	else if ( _T("vavvavhebrew") == wsName ) return 0x05F0;
	else if ( _T("vavyodhebrew") == wsName ) return 0x05F1;
	else if ( _T("vcircle") == wsName ) return 0x24E5;
	else if ( _T("vdotbelow") == wsName ) return 0x1E7F;
	else if ( _T("vecyrillic") == wsName ) return 0x0432;
	else if ( _T("veharabic") == wsName ) return 0x06A4;
	else if ( _T("vehfinalarabic") == wsName ) return 0xFB6B;
	else if ( _T("vehinitialarabic") == wsName ) return 0xFB6C;
	else if ( _T("vehmedialarabic") == wsName ) return 0xFB6D;
	else if ( _T("vekatakana") == wsName ) return 0x30F9;
	else if ( _T("venus") == wsName ) return 0x2640;
	else if ( _T("verticalbar") == wsName ) return 0x007C;
	else if ( _T("verticallineabovecmb") == wsName ) return 0x030D;
	else if ( _T("verticallinebelowcmb") == wsName ) return 0x0329;
	else if ( _T("verticallinelowmod") == wsName ) return 0x02CC;
	else if ( _T("verticallinemod") == wsName ) return 0x02C8;
	else if ( _T("vewarmenian") == wsName ) return 0x057E;
	else if ( _T("vhook") == wsName ) return 0x028B;
	else if ( _T("vikatakana") == wsName ) return 0x30F8;
	else if ( _T("viramabengali") == wsName ) return 0x09CD;
	else if ( _T("viramadeva") == wsName ) return 0x094D;
	else if ( _T("viramagujarati") == wsName ) return 0x0ACD;
	else if ( _T("visargabengali") == wsName ) return 0x0983;
	else if ( _T("visargadeva") == wsName ) return 0x0903;
	else if ( _T("visargagujarati") == wsName ) return 0x0A83;
	else if ( _T("vmonospace") == wsName ) return 0xFF56;
	else if ( _T("voarmenian") == wsName ) return 0x0578;
	else if ( _T("voicediterationhiragana") == wsName ) return 0x309E;
	else if ( _T("voicediterationkatakana") == wsName ) return 0x30FE;
	else if ( _T("voicedmarkkana") == wsName ) return 0x309B;
	else if ( _T("voicedmarkkanahalfwidth") == wsName ) return 0xFF9E;
	else if ( _T("vokatakana") == wsName ) return 0x30FA;
	else if ( _T("vparen") == wsName ) return 0x24B1;
	else if ( _T("vtilde") == wsName ) return 0x1E7D;
	else if ( _T("vturned") == wsName ) return 0x028C;
	else if ( _T("vuhiragana") == wsName ) return 0x3094;
	else if ( _T("vukatakana") == wsName ) return 0x30F4;

	return 0;
}
static int Type1NameToUnicode_w(CWCharWrapper &wsName)
{
	if      ( _T("w") == wsName ) return 0x0077;
	else if ( _T("wacute") == wsName ) return 0x1E83;
	else if ( _T("waekorean") == wsName ) return 0x3159;
	else if ( _T("wahiragana") == wsName ) return 0x308F;
	else if ( _T("wakatakana") == wsName ) return 0x30EF;
	else if ( _T("wakatakanahalfwidth") == wsName ) return 0xFF9C;
	else if ( _T("wakorean") == wsName ) return 0x3158;
	else if ( _T("wasmallhiragana") == wsName ) return 0x308E;
	else if ( _T("wasmallkatakana") == wsName ) return 0x30EE;
	else if ( _T("wattosquare") == wsName ) return 0x3357;
	else if ( _T("wavedash") == wsName ) return 0x301C;
	else if ( _T("wavyunderscorevertical") == wsName ) return 0xFE34;
	else if ( _T("wawarabic") == wsName ) return 0x0648;
	else if ( _T("wawfinalarabic") == wsName ) return 0xFEEE;
	else if ( _T("wawhamzaabovearabic") == wsName ) return 0x0624;
	else if ( _T("wawhamzaabovefinalarabic") == wsName ) return 0xFE86;
	else if ( _T("wbsquare") == wsName ) return 0x33DD;
	else if ( _T("wcircle") == wsName ) return 0x24E6;
	else if ( _T("wcircumflex") == wsName ) return 0x0175;
	else if ( _T("wdieresis") == wsName ) return 0x1E85;
	else if ( _T("wdotaccent") == wsName ) return 0x1E87;
	else if ( _T("wdotbelow") == wsName ) return 0x1E89;
	else if ( _T("wehiragana") == wsName ) return 0x3091;
	else if ( _T("weierstrass") == wsName ) return 0x2118;
	else if ( _T("wekatakana") == wsName ) return 0x30F1;
	else if ( _T("wekorean") == wsName ) return 0x315E;
	else if ( _T("weokorean") == wsName ) return 0x315D;
	else if ( _T("wgrave") == wsName ) return 0x1E81;
	else if ( _T("whitebullet") == wsName ) return 0x25E6;
	else if ( _T("whitecircle") == wsName ) return 0x25CB;
	else if ( _T("whitecircleinverse") == wsName ) return 0x25D9;
	else if ( _T("whitecornerbracketleft") == wsName ) return 0x300E;
	else if ( _T("whitecornerbracketleftvertical") == wsName ) return 0xFE43;
	else if ( _T("whitecornerbracketright") == wsName ) return 0x300F;
	else if ( _T("whitecornerbracketrightvertical") == wsName ) return 0xFE44;
	else if ( _T("whitediamond") == wsName ) return 0x25C7;
	else if ( _T("whitediamondcontainingblacksmalldiamond") == wsName ) return 0x25C8;
	else if ( _T("whitedownpointingsmalltriangle") == wsName ) return 0x25BF;
	else if ( _T("whitedownpointingtriangle") == wsName ) return 0x25BD;
	else if ( _T("whiteleftpointingsmalltriangle") == wsName ) return 0x25C3;
	else if ( _T("whiteleftpointingtriangle") == wsName ) return 0x25C1;
	else if ( _T("whitelenticularbracketleft") == wsName ) return 0x3016;
	else if ( _T("whitelenticularbracketright") == wsName ) return 0x3017;
	else if ( _T("whiterightpointingsmalltriangle") == wsName ) return 0x25B9;
	else if ( _T("whiterightpointingtriangle") == wsName ) return 0x25B7;
	else if ( _T("whitesmallsquare") == wsName ) return 0x25AB;
	else if ( _T("whitesmilingface") == wsName ) return 0x263A;
	else if ( _T("whitesquare") == wsName ) return 0x25A1;
	else if ( _T("whitestar") == wsName ) return 0x2606;
	else if ( _T("whitetelephone") == wsName ) return 0x260F;
	else if ( _T("whitetortoiseshellbracketleft") == wsName ) return 0x3018;
	else if ( _T("whitetortoiseshellbracketright") == wsName ) return 0x3019;
	else if ( _T("whiteuppointingsmalltriangle") == wsName ) return 0x25B5;
	else if ( _T("whiteuppointingtriangle") == wsName ) return 0x25B3;
	else if ( _T("wihiragana") == wsName ) return 0x3090;
	else if ( _T("wikatakana") == wsName ) return 0x30F0;
	else if ( _T("wikorean") == wsName ) return 0x315F;
	else if ( _T("wmonospace") == wsName ) return 0xFF57;
	else if ( _T("wohiragana") == wsName ) return 0x3092;
	else if ( _T("wokatakana") == wsName ) return 0x30F2;
	else if ( _T("wokatakanahalfwidth") == wsName ) return 0xFF66;
	else if ( _T("won") == wsName ) return 0x20A9;
	else if ( _T("wonmonospace") == wsName ) return 0xFFE6;
	else if ( _T("wowaenthai") == wsName ) return 0x0E27;
	else if ( _T("wparen") == wsName ) return 0x24B2;
	else if ( _T("wring") == wsName ) return 0x1E98;
	else if ( _T("wsuperior") == wsName ) return 0x02B7;
	else if ( _T("wturned") == wsName ) return 0x028D;
	else if ( _T("wynn") == wsName ) return 0x01BF;

	return 0;
}
static int Type1NameToUnicode_x(CWCharWrapper &wsName)
{
	if      ( _T("x") == wsName ) return 0x0078;
	else if ( _T("xabovecmb") == wsName ) return 0x033D;
	else if ( _T("xbopomofo") == wsName ) return 0x3112;
	else if ( _T("xcircle") == wsName ) return 0x24E7;
	else if ( _T("xdieresis") == wsName ) return 0x1E8D;
	else if ( _T("xdotaccent") == wsName ) return 0x1E8B;
	else if ( _T("xeharmenian") == wsName ) return 0x056D;
	else if ( _T("xi") == wsName ) return 0x03BE;
	else if ( _T("xmonospace") == wsName ) return 0xFF58;
	else if ( _T("xparen") == wsName ) return 0x24B3;
	else if ( _T("xsuperior") == wsName ) return 0x02E3;

	return 0;
}
static int Type1NameToUnicode_y(CWCharWrapper &wsName)
{
	if      ( _T("y") == wsName ) return 0x0079;
	else if ( _T("yaadosquare") == wsName ) return 0x334E;
	else if ( _T("yabengali") == wsName ) return 0x09AF;
	else if ( _T("yacute") == wsName ) return 0x00FD;
	else if ( _T("yadeva") == wsName ) return 0x092F;
	else if ( _T("yaekorean") == wsName ) return 0x3152;
	else if ( _T("yagujarati") == wsName ) return 0x0AAF;
	else if ( _T("yagurmukhi") == wsName ) return 0x0A2F;
	else if ( _T("yahiragana") == wsName ) return 0x3084;
	else if ( _T("yakatakana") == wsName ) return 0x30E4;
	else if ( _T("yakatakanahalfwidth") == wsName ) return 0xFF94;
	else if ( _T("yakorean") == wsName ) return 0x3151;
	else if ( _T("yamakkanthai") == wsName ) return 0x0E4E;
	else if ( _T("yasmallhiragana") == wsName ) return 0x3083;
	else if ( _T("yasmallkatakana") == wsName ) return 0x30E3;
	else if ( _T("yasmallkatakanahalfwidth") == wsName ) return 0xFF6C;
	else if ( _T("yatcyrillic") == wsName ) return 0x0463;
	else if ( _T("ycircle") == wsName ) return 0x24E8;
	else if ( _T("ycircumflex") == wsName ) return 0x0177;
	else if ( _T("ydieresis") == wsName ) return 0x00FF;
	else if ( _T("ydotaccent") == wsName ) return 0x1E8F;
	else if ( _T("ydotbelow") == wsName ) return 0x1EF5;
	else if ( _T("yeharabic") == wsName ) return 0x064A;
	else if ( _T("yehbarreearabic") == wsName ) return 0x06D2;
	else if ( _T("yehbarreefinalarabic") == wsName ) return 0xFBAF;
	else if ( _T("yehfinalarabic") == wsName ) return 0xFEF2;
	else if ( _T("yehhamzaabovearabic") == wsName ) return 0x0626;
	else if ( _T("yehhamzaabovefinalarabic") == wsName ) return 0xFE8A;
	else if ( _T("yehhamzaaboveinitialarabic") == wsName ) return 0xFE8B;
	else if ( _T("yehhamzaabovemedialarabic") == wsName ) return 0xFE8C;
	else if ( _T("yehinitialarabic") == wsName ) return 0xFEF3;
	else if ( _T("yehmedialarabic") == wsName ) return 0xFEF4;
	else if ( _T("yehmeeminitialarabic") == wsName ) return 0xFCDD;
	else if ( _T("yehmeemisolatedarabic") == wsName ) return 0xFC58;
	else if ( _T("yehnoonfinalarabic") == wsName ) return 0xFC94;
	else if ( _T("yehthreedotsbelowarabic") == wsName ) return 0x06D1;
	else if ( _T("yekorean") == wsName ) return 0x3156;
	else if ( _T("yen") == wsName ) return 0x00A5;
	else if ( _T("yenmonospace") == wsName ) return 0xFFE5;
	else if ( _T("yeokorean") == wsName ) return 0x3155;
	else if ( _T("yeorinhieuhkorean") == wsName ) return 0x3186;
	else if ( _T("yerahbenyomohebrew") == wsName ) return 0x05AA;
	else if ( _T("yerahbenyomolefthebrew") == wsName ) return 0x05AA;
	else if ( _T("yericyrillic") == wsName ) return 0x044B;
	else if ( _T("yerudieresiscyrillic") == wsName ) return 0x04F9;
	else if ( _T("yesieungkorean") == wsName ) return 0x3181;
	else if ( _T("yesieungpansioskorean") == wsName ) return 0x3183;
	else if ( _T("yesieungsioskorean") == wsName ) return 0x3182;
	else if ( _T("yetivhebrew") == wsName ) return 0x059A;
	else if ( _T("ygrave") == wsName ) return 0x1EF3;
	else if ( _T("yhook") == wsName ) return 0x01B4;
	else if ( _T("yhookabove") == wsName ) return 0x1EF7;
	else if ( _T("yiarmenian") == wsName ) return 0x0575;
	else if ( _T("yicyrillic") == wsName ) return 0x0457;
	else if ( _T("yikorean") == wsName ) return 0x3162;
	else if ( _T("yinyang") == wsName ) return 0x262F;
	else if ( _T("yiwnarmenian") == wsName ) return 0x0582;
	else if ( _T("ymonospace") == wsName ) return 0xFF59;
	else if ( _T("yod") == wsName ) return 0x05D9;
	else if ( _T("yoddagesh") == wsName ) return 0xFB39;
	else if ( _T("yoddageshhebrew") == wsName ) return 0xFB39;
	else if ( _T("yodhebrew") == wsName ) return 0x05D9;
	else if ( _T("yodyodhebrew") == wsName ) return 0x05F2;
	else if ( _T("yodyodpatahhebrew") == wsName ) return 0xFB1F;
	else if ( _T("yohiragana") == wsName ) return 0x3088;
	else if ( _T("yoikorean") == wsName ) return 0x3189;
	else if ( _T("yokatakana") == wsName ) return 0x30E8;
	else if ( _T("yokatakanahalfwidth") == wsName ) return 0xFF96;
	else if ( _T("yokorean") == wsName ) return 0x315B;
	else if ( _T("yosmallhiragana") == wsName ) return 0x3087;
	else if ( _T("yosmallkatakana") == wsName ) return 0x30E7;
	else if ( _T("yosmallkatakanahalfwidth") == wsName ) return 0xFF6E;
	else if ( _T("yotgreek") == wsName ) return 0x03F3;
	else if ( _T("yoyaekorean") == wsName ) return 0x3188;
	else if ( _T("yoyakorean") == wsName ) return 0x3187;
	else if ( _T("yoyakthai") == wsName ) return 0x0E22;
	else if ( _T("yoyingthai") == wsName ) return 0x0E0D;
	else if ( _T("yparen") == wsName ) return 0x24B4;
	else if ( _T("ypogegrammeni") == wsName ) return 0x037A;
	else if ( _T("ypogegrammenigreekcmb") == wsName ) return 0x0345;
	else if ( _T("yr") == wsName ) return 0x01A6;
	else if ( _T("yring") == wsName ) return 0x1E99;
	else if ( _T("ysuperior") == wsName ) return 0x02B8;
	else if ( _T("ytilde") == wsName ) return 0x1EF9;
	else if ( _T("yturned") == wsName ) return 0x028E;
	else if ( _T("yuhiragana") == wsName ) return 0x3086;
	else if ( _T("yuikorean") == wsName ) return 0x318C;
	else if ( _T("yukatakana") == wsName ) return 0x30E6;
	else if ( _T("yukatakanahalfwidth") == wsName ) return 0xFF95;
	else if ( _T("yukorean") == wsName ) return 0x3160;
	else if ( _T("yusbigcyrillic") == wsName ) return 0x046B;
	else if ( _T("yusbigiotifiedcyrillic") == wsName ) return 0x046D;
	else if ( _T("yuslittlecyrillic") == wsName ) return 0x0467;
	else if ( _T("yuslittleiotifiedcyrillic") == wsName ) return 0x0469;
	else if ( _T("yusmallhiragana") == wsName ) return 0x3085;
	else if ( _T("yusmallkatakana") == wsName ) return 0x30E5;
	else if ( _T("yusmallkatakanahalfwidth") == wsName ) return 0xFF6D;
	else if ( _T("yuyekorean") == wsName ) return 0x318B;
	else if ( _T("yuyeokorean") == wsName ) return 0x318A;
	else if ( _T("yyabengali") == wsName ) return 0x09DF;
	else if ( _T("yyadeva") == wsName ) return 0x095F;

	return 0;
}
static int Type1NameToUnicode_z(CWCharWrapper &wsName)
{
	if      ( _T("z") == wsName ) return 0x007A;
	else if ( _T("zaarmenian") == wsName ) return 0x0566;
	else if ( _T("zacute") == wsName ) return 0x017A;
	else if ( _T("zadeva") == wsName ) return 0x095B;
	else if ( _T("zagurmukhi") == wsName ) return 0x0A5B;
	else if ( _T("zaharabic") == wsName ) return 0x0638;
	else if ( _T("zahfinalarabic") == wsName ) return 0xFEC6;
	else if ( _T("zahinitialarabic") == wsName ) return 0xFEC7;
	else if ( _T("zahiragana") == wsName ) return 0x3056;
	else if ( _T("zahmedialarabic") == wsName ) return 0xFEC8;
	else if ( _T("zainarabic") == wsName ) return 0x0632;
	else if ( _T("zainfinalarabic") == wsName ) return 0xFEB0;
	else if ( _T("zakatakana") == wsName ) return 0x30B6;
	else if ( _T("zaqefgadolhebrew") == wsName ) return 0x0595;
	else if ( _T("zaqefqatanhebrew") == wsName ) return 0x0594;
	else if ( _T("zarqahebrew") == wsName ) return 0x0598;
	else if ( _T("zayin") == wsName ) return 0x05D6;
	else if ( _T("zayindagesh") == wsName ) return 0xFB36;
	else if ( _T("zayindageshhebrew") == wsName ) return 0xFB36;
	else if ( _T("zayinhebrew") == wsName ) return 0x05D6;
	else if ( _T("zbopomofo") == wsName ) return 0x3117;
	else if ( _T("zcaron") == wsName ) return 0x017E;
	else if ( _T("zcircle") == wsName ) return 0x24E9;
	else if ( _T("zcircumflex") == wsName ) return 0x1E91;
	else if ( _T("zcurl") == wsName ) return 0x0291;
	else if ( _T("zdot") == wsName ) return 0x017C;
	else if ( _T("zdotaccent") == wsName ) return 0x017C;
	else if ( _T("zdotbelow") == wsName ) return 0x1E93;
	else if ( _T("zecyrillic") == wsName ) return 0x0437;
	else if ( _T("zedescendercyrillic") == wsName ) return 0x0499;
	else if ( _T("zedieresiscyrillic") == wsName ) return 0x04DF;
	else if ( _T("zehiragana") == wsName ) return 0x305C;
	else if ( _T("zekatakana") == wsName ) return 0x30BC;
	else if ( _T("zero") == wsName ) return 0x0030;
	else if ( _T("zeroarabic") == wsName ) return 0x0660;
	else if ( _T("zerobengali") == wsName ) return 0x09E6;
	else if ( _T("zerodeva") == wsName ) return 0x0966;
	else if ( _T("zerogujarati") == wsName ) return 0x0AE6;
	else if ( _T("zerogurmukhi") == wsName ) return 0x0A66;
	else if ( _T("zerohackarabic") == wsName ) return 0x0660;
	else if ( _T("zeroinferior") == wsName ) return 0x2080;
	else if ( _T("zeromonospace") == wsName ) return 0xFF10;
	else if ( _T("zerooldstyle") == wsName ) return 0xF730;
	else if ( _T("zeropersian") == wsName ) return 0x06F0;
	else if ( _T("zerosuperior") == wsName ) return 0x2070;
	else if ( _T("zerothai") == wsName ) return 0x0E50;
	else if ( _T("zerowidthjoiner") == wsName ) return 0xFEFF;
	else if ( _T("zerowidthnonjoiner") == wsName ) return 0x200C;
	else if ( _T("zerowidthspace") == wsName ) return 0x200B;
	else if ( _T("zeta") == wsName ) return 0x03B6;
	else if ( _T("zhbopomofo") == wsName ) return 0x3113;
	else if ( _T("zhearmenian") == wsName ) return 0x056A;
	else if ( _T("zhebrevecyrillic") == wsName ) return 0x04C2;
	else if ( _T("zhecyrillic") == wsName ) return 0x0436;
	else if ( _T("zhedescendercyrillic") == wsName ) return 0x0497;
	else if ( _T("zhedieresiscyrillic") == wsName ) return 0x04DD;
	else if ( _T("zihiragana") == wsName ) return 0x3058;
	else if ( _T("zikatakana") == wsName ) return 0x30B8;
	else if ( _T("zinorhebrew") == wsName ) return 0x05AE;
	else if ( _T("zlinebelow") == wsName ) return 0x1E95;
	else if ( _T("zmonospace") == wsName ) return 0xFF5A;
	else if ( _T("zohiragana") == wsName ) return 0x305E;
	else if ( _T("zokatakana") == wsName ) return 0x30BE;
	else if ( _T("zparen") == wsName ) return 0x24B5;
	else if ( _T("zretroflexhook") == wsName ) return 0x0290;
	else if ( _T("zstroke") == wsName ) return 0x01B6;
	else if ( _T("zuhiragana") == wsName ) return 0x305A;
	else if ( _T("zukatakana") == wsName ) return 0x30BA;

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
	USES_CONVERSION;
	return Type1NameToUnicodeW( A2W( sName ) );
}

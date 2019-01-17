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


#include "ParagraphItem.h"
#include "DocxToDocUtils.h"
#include "ShapeBuilder.h"
#include "PropertyReader.h"

#include "FileConverter.h"

using namespace ASCDocFileFormat;

// NOTE: delete after remove bost
const std::wstring string2wstring__(const std::string& sline, const unsigned int codePage = CP_ACP)
{
	const int nSize = MultiByteToWideChar(codePage, 0, sline.c_str(), sline.size(), NULL, 0);

	wchar_t *sTemp = new wchar_t[nSize];
	if (!sTemp)
		return std::wstring();

	int size = MultiByteToWideChar(codePage, 0, sline.c_str(), sline.size(), sTemp, nSize);

	std::wstring sResult(sTemp, size);
	delete []sTemp;

	return sResult;
}

// NOTE: delete after remove bost
const std::string wstring2string__(const std::wstring& sLine, const unsigned int codePage = CP_ACP)
{
	const int nSize = WideCharToMultiByte(codePage, 0, sLine.c_str(), sLine.length(), NULL, 0, NULL, NULL);
	char *sTemp = new char[nSize];
	if (!sTemp)
		return std::string();

	int size = WideCharToMultiByte(codePage, 0, sLine.c_str(), sLine.length(), sTemp, nSize, NULL, NULL);

	std::string sResult(sTemp, size);
	delete []sTemp;

	return sResult;
}

namespace DOCXTODOC
{
	CFileTransformer::CFileTransformer()
	{
		m_pDocFile				=	NULL;

		m_bHaveSeparateFldChar	=	false;
		m_bIsInlineShape		=	FALSE;

		m_bIsHaveRunPr			=	FALSE;

		m_pDocFile				=	new ASCDocFileFormat::CDocFile();

		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "af-ZA" ), ASCDocFileFormat::LID( DocFileFormat::Afrikaans ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sq-AL" ), ASCDocFileFormat::LID( DocFileFormat::Albanian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "am-ET" ), ASCDocFileFormat::LID( DocFileFormat::Amharic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-DZ" ), ASCDocFileFormat::LID( DocFileFormat::ArabicAlgeria ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-BH" ), ASCDocFileFormat::LID( DocFileFormat::ArabicBahrain ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-EG" ), ASCDocFileFormat::LID( DocFileFormat::ArabicEgypt ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-IQ" ), ASCDocFileFormat::LID( DocFileFormat::ArabicIraq ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-JO" ), ASCDocFileFormat::LID( DocFileFormat::ArabicJordan ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-KW" ), ASCDocFileFormat::LID( DocFileFormat::ArabicKuwait ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-LB" ), ASCDocFileFormat::LID( DocFileFormat::ArabicLebanon ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-LY" ), ASCDocFileFormat::LID( DocFileFormat::ArabicLibya ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-MA" ), ASCDocFileFormat::LID( DocFileFormat::ArabicMorocco ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-OM" ), ASCDocFileFormat::LID( DocFileFormat::ArabicOman ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-QA" ), ASCDocFileFormat::LID( DocFileFormat::ArabicQatar ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-SA" ), ASCDocFileFormat::LID( DocFileFormat::ArabicSaudiArabia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-SY" ), ASCDocFileFormat::LID( DocFileFormat::ArabicSyria ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-TN" ), ASCDocFileFormat::LID( DocFileFormat::ArabicTunisia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-AE" ), ASCDocFileFormat::LID( DocFileFormat::ArabicUAE ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ar-YE" ), ASCDocFileFormat::LID( DocFileFormat::ArabicYemen ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "hy-AM" ), ASCDocFileFormat::LID( DocFileFormat::Armenian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "as-IN" ), ASCDocFileFormat::LID( DocFileFormat::Assamese ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "az-AZ-cyrl" ), ASCDocFileFormat::LID( DocFileFormat::AzeriCyrillic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "az-AZ-latn" ), ASCDocFileFormat::LID( DocFileFormat::AzeriLatin ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "eu-ES" ), ASCDocFileFormat::LID( DocFileFormat::Basque ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "be-BY" ), ASCDocFileFormat::LID( DocFileFormat::Belarusian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "bn-IN" ), ASCDocFileFormat::LID( DocFileFormat::Bengali ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "bn-BD" ), ASCDocFileFormat::LID( DocFileFormat::BengaliBangladesh ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "bg-BG" ), ASCDocFileFormat::LID( DocFileFormat::Bulgarian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "my-MM" ), ASCDocFileFormat::LID( DocFileFormat::Burmese ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ca-ES" ), ASCDocFileFormat::LID( DocFileFormat::Catalan ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "zh-HK" ), ASCDocFileFormat::LID( DocFileFormat::ChineseHongKong ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "zh-MO" ), ASCDocFileFormat::LID( DocFileFormat::ChineseMacao ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "zh-CN" ), ASCDocFileFormat::LID( DocFileFormat::ChinesePRC ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "zh-SG" ), ASCDocFileFormat::LID( DocFileFormat::ChineseSingapore ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "zh-TW" ), ASCDocFileFormat::LID( DocFileFormat::ChineseTaiwan ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "hr-HR" ), ASCDocFileFormat::LID( DocFileFormat::Croatian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "cs-CZ" ), ASCDocFileFormat::LID( DocFileFormat::Czech ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "da-DK" ), ASCDocFileFormat::LID( DocFileFormat::Danish ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "dv-MV" ), ASCDocFileFormat::LID( DocFileFormat::Divehi ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "nl-BE" ), ASCDocFileFormat::LID( DocFileFormat::DutchBelgium ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "nl-NL" ), ASCDocFileFormat::LID( DocFileFormat::DutchNetherlands ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-AU" ), ASCDocFileFormat::LID( DocFileFormat::EnglishAustralia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-BZ" ), ASCDocFileFormat::LID( DocFileFormat::EnglishBelize ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-CA" ), ASCDocFileFormat::LID( DocFileFormat::EnglishCanada ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-DO" ), ASCDocFileFormat::LID( DocFileFormat::EnglishCaribbean ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-HK" ), ASCDocFileFormat::LID( DocFileFormat::EnglishHongKong ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-IN" ), ASCDocFileFormat::LID( DocFileFormat::EnglishIndia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-ID" ), ASCDocFileFormat::LID( DocFileFormat::EnglishIndonesia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-IE" ), ASCDocFileFormat::LID( DocFileFormat::EnglishIreland ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-JM" ), ASCDocFileFormat::LID( DocFileFormat::EnglishJamaica ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-MY" ), ASCDocFileFormat::LID( DocFileFormat::EnglishMalaysia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-NZ" ), ASCDocFileFormat::LID( DocFileFormat::EnglishNewZealand ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-PH" ), ASCDocFileFormat::LID( DocFileFormat::EnglishPhilippines ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-SG" ), ASCDocFileFormat::LID( DocFileFormat::EnglishSingapore ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-ZA" ), ASCDocFileFormat::LID( DocFileFormat::EnglishSouthAfrica ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-TT" ), ASCDocFileFormat::LID( DocFileFormat::EnglishTrinidadAndTobago ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-UK" ), ASCDocFileFormat::LID( DocFileFormat::EnglishUK ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-US" ), ASCDocFileFormat::LID( DocFileFormat::EnglishUS ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "en-ZW" ), ASCDocFileFormat::LID( DocFileFormat::EnglishZimbabwe ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "et-EE" ), ASCDocFileFormat::LID( DocFileFormat::Estonian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fo-FO" ), ASCDocFileFormat::LID( DocFileFormat::Faeroese ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fi-FI" ), ASCDocFileFormat::LID( DocFileFormat::Finnish ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-BE" ), ASCDocFileFormat::LID( DocFileFormat::FrenchBelgium ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-CM" ), ASCDocFileFormat::LID( DocFileFormat::FrenchCameroon ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-CA" ), ASCDocFileFormat::LID( DocFileFormat::FrenchCanada ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-CD" ), ASCDocFileFormat::LID( DocFileFormat::FrenchCongoDRC ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-CI" ), ASCDocFileFormat::LID( DocFileFormat::FrenchCotedIvoire ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-FR" ), ASCDocFileFormat::LID( DocFileFormat::FrenchFrance ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-HT" ), ASCDocFileFormat::LID( DocFileFormat::FrenchHaiti ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-LU" ), ASCDocFileFormat::LID( DocFileFormat::FrenchLuxembourg ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-ML" ), ASCDocFileFormat::LID( DocFileFormat::FrenchMali ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-MC" ), ASCDocFileFormat::LID( DocFileFormat::FrenchMonaco ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-MA" ), ASCDocFileFormat::LID( DocFileFormat::FrenchMorocco ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-RE" ), ASCDocFileFormat::LID( DocFileFormat::FrenchReunion ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-SN" ), ASCDocFileFormat::LID( DocFileFormat::FrenchSenegal ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-CH" ), ASCDocFileFormat::LID( DocFileFormat::FrenchSwitzerland ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fr-DO" ), ASCDocFileFormat::LID( DocFileFormat::FrenchWestIndies ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "fy-NL" ), ASCDocFileFormat::LID( DocFileFormat::FrisianNetherlands ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "mk-MK" ), ASCDocFileFormat::LID( DocFileFormat::FYROMacedonian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ga-IE" ), ASCDocFileFormat::LID( DocFileFormat::GaelicIreland ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "gd-UK" ), ASCDocFileFormat::LID( DocFileFormat::GaelicScotland ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "gl-ES" ), ASCDocFileFormat::LID( DocFileFormat::Galician ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ka-GE" ), ASCDocFileFormat::LID( DocFileFormat::Georgian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "de-AT" ), ASCDocFileFormat::LID( DocFileFormat::GermanAustria ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "de-DE" ), ASCDocFileFormat::LID( DocFileFormat::GermanGermany ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "de-LI" ), ASCDocFileFormat::LID( DocFileFormat::GermanLiechtenstein ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "de-LU" ), ASCDocFileFormat::LID( DocFileFormat::GermanLuxembourg ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "de-CH" ), ASCDocFileFormat::LID( DocFileFormat::GermanSwitzerland ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "el-GR" ), ASCDocFileFormat::LID( DocFileFormat::Greek ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "gn-BR" ), ASCDocFileFormat::LID( DocFileFormat::Guarani ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "gu-IN" ), ASCDocFileFormat::LID( DocFileFormat::Gujarati ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ha-NG" ), ASCDocFileFormat::LID( DocFileFormat::Hausa ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "he-IL" ), ASCDocFileFormat::LID( DocFileFormat::Hebrew ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "hi-IN" ), ASCDocFileFormat::LID( DocFileFormat::Hindi ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "hu-HU" ), ASCDocFileFormat::LID( DocFileFormat::Hungarian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "is-IS" ), ASCDocFileFormat::LID( DocFileFormat::Icelandic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "id-ID" ), ASCDocFileFormat::LID( DocFileFormat::Indonesian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "iu-CA" ), ASCDocFileFormat::LID( DocFileFormat::Inuktitut ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "it-IT" ), ASCDocFileFormat::LID( DocFileFormat::ItalianItaly ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "it-CH" ), ASCDocFileFormat::LID( DocFileFormat::ItalianSwitzerland ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ja-JP" ), ASCDocFileFormat::LID( DocFileFormat::Japanese ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "kn-ID" ), ASCDocFileFormat::LID( DocFileFormat::Kannada ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ks-ID" ), ASCDocFileFormat::LID( DocFileFormat::Kashmiri ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ks-PK" ), ASCDocFileFormat::LID( DocFileFormat::KashmiriArabic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "kk-KZ" ), ASCDocFileFormat::LID( DocFileFormat::Kazakh ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ko-KR" ), ASCDocFileFormat::LID( DocFileFormat::Korean ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ky-KG" ), ASCDocFileFormat::LID( DocFileFormat::Kyrgyz ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "lo-LA" ), ASCDocFileFormat::LID( DocFileFormat::Lao ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "la" ), ASCDocFileFormat::LID( DocFileFormat::Latin ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "lv-LV" ), ASCDocFileFormat::LID( DocFileFormat::Latvian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "lt-LT" ), ASCDocFileFormat::LID( DocFileFormat::Lithuanian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ms-MY" ), ASCDocFileFormat::LID( DocFileFormat::Malay ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ms-BN" ), ASCDocFileFormat::LID( DocFileFormat::MalayBruneiDarussalam ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ml-ID" ), ASCDocFileFormat::LID( DocFileFormat::Malayalam ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "mt-MT" ), ASCDocFileFormat::LID( DocFileFormat::Maltese ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "mi-NZ" ), ASCDocFileFormat::LID( DocFileFormat::Maori ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "mr-ID" ), ASCDocFileFormat::LID( DocFileFormat::Marathi ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "mn-MN" ), ASCDocFileFormat::LID( DocFileFormat::Mongolian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ne-NP" ), ASCDocFileFormat::LID( DocFileFormat::Nepali ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ne-ID" ), ASCDocFileFormat::LID( DocFileFormat::NepaliIndia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "nb-NO" ), ASCDocFileFormat::LID( DocFileFormat::NorwegianBokmal ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "nn-NO" ), ASCDocFileFormat::LID( DocFileFormat::NorwegianNynorsk ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "or-ID" ), ASCDocFileFormat::LID( DocFileFormat::Oriya ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ps-PK" ), ASCDocFileFormat::LID( DocFileFormat::Pashto ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "pl-PL" ), ASCDocFileFormat::LID( DocFileFormat::Polish ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "pt-BR" ), ASCDocFileFormat::LID( DocFileFormat::PortugueseBrazil ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "pt-PT" ), ASCDocFileFormat::LID( DocFileFormat::PortuguesePortugal ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "pa-ID" ), ASCDocFileFormat::LID( DocFileFormat::Punjabi ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "pa-PK" ), ASCDocFileFormat::LID( DocFileFormat::PunjabiPakistan ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "qu-BO" ), ASCDocFileFormat::LID( DocFileFormat::QuechuaBolivia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "qu-EC" ), ASCDocFileFormat::LID( DocFileFormat::QuechuaEcuador ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "qu-PE" ), ASCDocFileFormat::LID( DocFileFormat::QuechuaPeru ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "rm-CH" ), ASCDocFileFormat::LID( DocFileFormat::RhaetoRomanic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ro-MD" ), ASCDocFileFormat::LID( DocFileFormat::RomanianMoldova ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ro-RO" ), ASCDocFileFormat::LID( DocFileFormat::RomanianRomania ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ru-MD" ), ASCDocFileFormat::LID( DocFileFormat::RussianMoldova ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ru-RU" ), ASCDocFileFormat::LID( DocFileFormat::RussianRussia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "se-FI" ), ASCDocFileFormat::LID( DocFileFormat::SamiLappish ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sa-ID" ), ASCDocFileFormat::LID( DocFileFormat::Sanskrit ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sr-YU-cyrl" ), ASCDocFileFormat::LID( DocFileFormat::SerbianCyrillic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sr-YU-latn" ), ASCDocFileFormat::LID( DocFileFormat::SerbianLatin ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sd-PK" ), ASCDocFileFormat::LID( DocFileFormat::SindhiArabic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sd-ID" ), ASCDocFileFormat::LID( DocFileFormat::SindhiDevanagari ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "si-ID" ), ASCDocFileFormat::LID( DocFileFormat::Sinhalese ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sk-SK" ), ASCDocFileFormat::LID( DocFileFormat::Slovak ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sl-SI" ), ASCDocFileFormat::LID( DocFileFormat::Slovenian ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "so-SO" ), ASCDocFileFormat::LID( DocFileFormat::Somali ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-AR" ), ASCDocFileFormat::LID( DocFileFormat::SpanishArgentina ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-BO" ), ASCDocFileFormat::LID( DocFileFormat::SpanishBolivia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-CL" ), ASCDocFileFormat::LID( DocFileFormat::SpanishChile ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-CO" ), ASCDocFileFormat::LID( DocFileFormat::SpanishColombia ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-CR" ), ASCDocFileFormat::LID( DocFileFormat::SpanishCostaRica ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-DO" ), ASCDocFileFormat::LID( DocFileFormat::SpanishDominicanRepublic ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-EC" ), ASCDocFileFormat::LID( DocFileFormat::SpanishEcuador ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-SV" ), ASCDocFileFormat::LID( DocFileFormat::SpanishElSalvador ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-GT" ), ASCDocFileFormat::LID( DocFileFormat::SpanishGuatemala ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-HN" ), ASCDocFileFormat::LID( DocFileFormat::SpanishHonduras ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-MX" ), ASCDocFileFormat::LID( DocFileFormat::SpanishMexico ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-NI" ), ASCDocFileFormat::LID( DocFileFormat::SpanishNicaragua ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-PA" ), ASCDocFileFormat::LID( DocFileFormat::SpanishPanama ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-PY" ), ASCDocFileFormat::LID( DocFileFormat::SpanishParaguay ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-PE" ), ASCDocFileFormat::LID( DocFileFormat::SpanishPeru ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-PR" ), ASCDocFileFormat::LID( DocFileFormat::SpanishPuertoRico ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-ES" ), ASCDocFileFormat::LID( DocFileFormat::SpanishSpainTraditionalSort ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-UY" ), ASCDocFileFormat::LID( DocFileFormat::SpanishUruguay ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "es-VE" ), ASCDocFileFormat::LID( DocFileFormat::SpanishVenezuela ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sw-TZ" ), ASCDocFileFormat::LID( DocFileFormat::Swahili ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sv-FI" ), ASCDocFileFormat::LID( DocFileFormat::SwedishFinland ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "sv-SE" ), ASCDocFileFormat::LID( DocFileFormat::SwedishSweden ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "tg-TJ" ), ASCDocFileFormat::LID( DocFileFormat::Tajik ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "ta-ID" ), ASCDocFileFormat::LID( DocFileFormat::Tamil ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "tt-RU" ), ASCDocFileFormat::LID( DocFileFormat::Tatar ) ) );
		lidMap.insert( pair<string, ASCDocFileFormat::LID>( string( "te-ID" ), ASCDocFileFormat::LID( DocFileFormat::Telugu ) ) );

		for (unsigned short i = 0; i < ( sizeof(ASCDocFileFormat::StyleIDs) / sizeof(ASCDocFileFormat::StyleIDs[0]) ); i++ )
		{
			predefinedStyleIDMap.insert( pair<string, unsigned short>( ASCDocFileFormat::StyleIDs[i], i ) );
		}

		styleTypeMap.insert( pair<string, ASCDocFileFormat::Constants::StyleType>( string( "paragraph" ), ASCDocFileFormat::Constants::styleTypeParagraph ) );
		styleTypeMap.insert( pair<string, ASCDocFileFormat::Constants::StyleType>( string( "character" ), ASCDocFileFormat::Constants::styleTypeCharacter ) );
		styleTypeMap.insert( pair<string, ASCDocFileFormat::Constants::StyleType>( string( "table" ), ASCDocFileFormat::Constants::styleTypeTable ) );
		styleTypeMap.insert( pair<string, ASCDocFileFormat::Constants::StyleType>( string( "numbering" ), ASCDocFileFormat::Constants::styleTypeNumbering ) );

		for ( unsigned short i = 0; i < ( sizeof(ASCDocFileFormat::NumberFormatCodes) / sizeof(ASCDocFileFormat::NumberFormatCodes[0]) ); i++ )
		{
			numFmtMap.insert( pair<string, ASCDocFileFormat::Constants::MSONFC>( ASCDocFileFormat::NumberFormatCodes[i], (ASCDocFileFormat::Constants::MSONFC)i ) );
		}

		kulMap.insert( pair<string, unsigned char>( string( "none" ), 0x00 ) );
		kulMap.insert( pair<string, unsigned char>( string( "single" ), 0x01 ) );
		kulMap.insert( pair<string, unsigned char>( string( "words" ), 0x02 ) );
		kulMap.insert( pair<string, unsigned char>( string( "double" ), 0x03 ) );
		kulMap.insert( pair<string, unsigned char>( string( "dotted" ), 0x04 ) );
		kulMap.insert( pair<string, unsigned char>( string( "thick" ), 0x06 ) );
		kulMap.insert( pair<string, unsigned char>( string( "dash" ), 0x07 ) );
		kulMap.insert( pair<string, unsigned char>( string( "dotDash" ), 0x09 ) );
		kulMap.insert( pair<string, unsigned char>( string( "dotDotDash" ), 0x0A ) );
		kulMap.insert( pair<string, unsigned char>( string( "wave" ), 0x0B ) );
		kulMap.insert( pair<string, unsigned char>( string( "dottedHeavy" ), 0x14 ) );
		kulMap.insert( pair<string, unsigned char>( string( "dashedHeavy" ), 0x17 ) );
		kulMap.insert( pair<string, unsigned char>( string( "dashDotHeavy" ), 0x19 ) );
		kulMap.insert( pair<string, unsigned char>( string( "dashDotDotHeavy" ), 0x1A ) );
		kulMap.insert( pair<string, unsigned char>( string( "wavyHeavy" ), 0x1B ) );
		kulMap.insert( pair<string, unsigned char>( string( "dashLong" ), 0x27 ) );
		kulMap.insert( pair<string, unsigned char>( string( "wavyDouble" ), 0x2B ) );
		kulMap.insert( pair<string, unsigned char>( string( "dashLongHeavy" ), 0x37 ) );

		sectionBreakTypeMap.insert( pair<string, ASCDocFileFormat::Constants::SBkcOperand>( string( "continuous" ), ASCDocFileFormat::Constants::bkcContinuous ) );
		sectionBreakTypeMap.insert( pair<string, ASCDocFileFormat::Constants::SBkcOperand>( string( "nextColumn" ), ASCDocFileFormat::Constants::bkcNewColumn ) );
		sectionBreakTypeMap.insert( pair<string, ASCDocFileFormat::Constants::SBkcOperand>( string( "nextPage" ), ASCDocFileFormat::Constants::bkcNewPage ) );
		sectionBreakTypeMap.insert( pair<string, ASCDocFileFormat::Constants::SBkcOperand>( string( "evenPage" ), ASCDocFileFormat::Constants::bkcEvenPage ) );
		sectionBreakTypeMap.insert( pair<string, ASCDocFileFormat::Constants::SBkcOperand>( string( "oddPage" ), ASCDocFileFormat::Constants::bkcOddPage ) );

		verticalPositionCodeMap.insert( make_pair( "margin", 0x00 ) );
		verticalPositionCodeMap.insert( make_pair( "page", 0x01 ) );
		verticalPositionCodeMap.insert( make_pair( "text", 0x02 ) );
		verticalPositionCodeMap.insert( make_pair( "none", 0x03 ) );

		horizontalPositionCodeMap.insert( make_pair( "text", 0x00 ) );
		horizontalPositionCodeMap.insert( make_pair( "margin", 0x01 ) );
		horizontalPositionCodeMap.insert( make_pair( "page", 0x02 ) );
		horizontalPositionCodeMap.insert( make_pair( "none", 0x03 ) );

		textFrameWrappingMap.insert( make_pair( "auto", 0x00 ) );
		textFrameWrappingMap.insert( make_pair( "notBeside", 0x01 ) );
		textFrameWrappingMap.insert( make_pair( "around", 0x02 ) );
		textFrameWrappingMap.insert( make_pair( "none", 0x03 ) );
		textFrameWrappingMap.insert( make_pair( "tight", 0x04 ) );
		textFrameWrappingMap.insert( make_pair( "through", 0x05 ) );

		tableCellWidthMap.insert( make_pair( "nil", ASCDocFileFormat::Constants::ftsNil ) );
		tableCellWidthMap.insert( make_pair( "auto", ASCDocFileFormat::Constants::ftsAuto ) );
		tableCellWidthMap.insert( make_pair( "pct", ASCDocFileFormat::Constants::ftsPercent ) );
		tableCellWidthMap.insert( make_pair( "dxa", ASCDocFileFormat::Constants::ftsDxa ) );

		customTabStopAlignment.insert( make_pair( "left", ASCDocFileFormat::Constants::jcLeft ) );
		customTabStopAlignment.insert( make_pair( "center", ASCDocFileFormat::Constants::jcCenter ) );
		customTabStopAlignment.insert( make_pair( "right", ASCDocFileFormat::Constants::jcRight ) );
		customTabStopAlignment.insert( make_pair( "decimal", ASCDocFileFormat::Constants::jcDecimal ) );
		customTabStopAlignment.insert( make_pair( "bar", ASCDocFileFormat::Constants::jcBar ) );
		customTabStopAlignment.insert( make_pair( "clear", (ASCDocFileFormat::Constants::TabJC)0x05 ) );
		customTabStopAlignment.insert( make_pair( "num", ASCDocFileFormat::Constants::jcList ) );

		customTabStopLeader.insert( make_pair( "none", ASCDocFileFormat::Constants::tlcNone ) );
		customTabStopLeader.insert( make_pair( "dot", ASCDocFileFormat::Constants::tlcDot ) );
		customTabStopLeader.insert( make_pair( "hyphen", ASCDocFileFormat::Constants::tlcHyphen ) );
		customTabStopLeader.insert( make_pair( "underscore", ASCDocFileFormat::Constants::tlcUnderscore ) );
		customTabStopLeader.insert( make_pair( "heavy", ASCDocFileFormat::Constants::tlcHeavy ) );
		customTabStopLeader.insert( make_pair( "middleDot", ASCDocFileFormat::Constants::tlcMiddleDot ) );
	}

	CFileTransformer::~CFileTransformer()
	{
		ASCDocFileFormat::BinaryStorageSingleton* pBin	=	ASCDocFileFormat::BinaryStorageSingleton::Instance();
		if (pBin)
			pBin->FreeInstance();

		ASCDocFileFormat::COArtStorage* pStorage = ASCDocFileFormat::COArtStorage::Instance();
		if (pStorage)
			pStorage->FreeInstance();

		RELEASEOBJECT (m_pDocFile);
	}
}

namespace DOCXTODOC
{
	long CFileTransformer::Convert(const wchar_t* ooxFolder, const wchar_t* docFile, const ProgressCallback* ffCallBack)
	{
		LONG hrStatus = S_FALSE;

		if ((NULL != ooxFolder) && (NULL != docFile) && (NULL != m_pDocFile))
		{		
#ifndef _DEBUG
			try
			{
#endif
				m_docxInputFile.read(ooxFolder);

				if (UpdateProgress (ffCallBack,500000))
					return S_FALSE;

				SHORT index = 0;

				size_t count = (m_docxInputFile.find<OOX::Document>().find<OOX::FontTable>().m_fonts).size();
				std::vector<OOX::FontTable::Font>& items = (m_docxInputFile.find<OOX::Document>().find<OOX::FontTable>().m_fonts);

				for (size_t j = 0; j < count; ++j)	
				{
					m_mapFontTableMap.insert(pair<wstring, short>(items[j].m_name.get(), index++));
				}

				if (UpdateProgress (ffCallBack,625000))
					return S_FALSE;

				if (m_docxInputFile.find<OOX::Document>().exist<OOX::Numbering>())
					ConvertNumbering( m_docxInputFile.find<OOX::Document>().find<OOX::Numbering>() );

				ConvertStyleSheet (m_docxInputFile.find<OOX::Document>().find<OOX::Styles>());

				if (UpdateProgress (ffCallBack,750000))
					return S_FALSE;

				ConvertDocument (m_docxInputFile.find<OOX::Document>());
				ConvertFontTable(m_docxInputFile.find<OOX::Document>().find<OOX::FontTable>());

				if (UpdateProgress (ffCallBack,875000))
					return S_FALSE;

				hrStatus = m_pDocFile->SaveToFile (docFile);

				if (UpdateProgress (ffCallBack,1000000))
					return S_FALSE;
#ifndef _DEBUG
			}
			catch (...)
			{
				hrStatus = S_FALSE;
			}
#endif
		}

		return hrStatus;
	}

	bool CFileTransformer::UpdateProgress(const ProgressCallback* ffCallBack, long nComplete)
	{
		if (ffCallBack)
		{
			ffCallBack->OnProgress (ffCallBack->caller, DOC_ONPROGRESSEVENT_ID, nComplete);

			SHORT pbStop = 0;
			ffCallBack->OnProgressEx (ffCallBack->caller, DOC_ONPROGRESSEVENT_ID, nComplete, &pbStop);

			if (0 != pbStop)
				return TRUE;
		}

		return FALSE;
	}
}

namespace DOCXTODOC
{
	void CFileTransformer::ConvertDocument(const OOX::Document& oXmlDoc)
	{
		ConvertContent (*oXmlDoc.Items);

		// TODO : если список в конце документа, то добавляется брэйк (бага)

		ASCDocFileFormat::Paragraph paragraph;
		paragraph.AddParagraphItem (ASCDocFileFormat::Run());
		m_pDocFile->AddTextItem (paragraph);

		m_pDocFile->AddSectionProperties (ConvertSectionProperties(oXmlDoc.SectorProperty));
	}

	void CFileTransformer::ConvertContent(const vector<OOX::Logic::TextItem>& oXmlItems)
	{
		m_oOArtBuilder.SetLocation (MAIN_DOCUMENT);

		for (size_t i = 0; i < oXmlItems.size(); ++i)
		{
			const OOX::Logic::TextItem& oXmlItem = oXmlItems[i];

			if (oXmlItem.is<OOX::Logic::Paragraph>())
			{
				const OOX::Logic::Paragraph& docxParagraph	=	oXmlItem.as<OOX::Logic::Paragraph>();
				if (ValidParagraph(docxParagraph))
				{
					ASCDocFileFormat::Paragraph docParagraph	=	ConvertParagraph<OOX::Document>(docxParagraph);

					m_pDocFile->AddTextItem (docParagraph);

					if (docxParagraph.Property.is_init() && docxParagraph.Property->SectorProperty.is_init())
					{
						m_pDocFile->AddTextItem (ASCDocFileFormat::SectionBreak());
						m_pDocFile->AddSectionProperties(ConvertSectionProperties( docxParagraph.Property->SectorProperty));
					}
				}
			}

			if (oXmlItem.is<OOX::Logic::Table>())
			{
				ASCDocFileFormat::Table oDocTable = CreateTable<OOX::Document>(oXmlItem.as<OOX::Logic::Table>());
				m_pDocFile->AddTextItem (oDocTable);
			}

			if (oXmlItem.is<OOX::Logic::Sdt>())
			{
				ConvertContent((*oXmlItem.as<OOX::Logic::Sdt>().Content).m_items);  
			}
		}  
	}

	template<class T> ASCDocFileFormat::Paragraph CFileTransformer::ConvertParagraph (const OOX::Logic::Paragraph& oXmlParagraph)
	{
		PrlList styleRunPr;
		PrlList styleParPr;

		std::string style	=	GetStyleID (oXmlParagraph);

		ASCDocFileFormat::Paragraph oParagraph;
		oParagraph.SetStyle (m_mapStyleSheetMap [style]);

		styleRunPr			=	GetRunPropertiesFromStyleHierarchy (style);
		styleParPr			=	GetParagraphPropertiesFromStyleHierarchy (style, &styleRunPr);

		// retrieve run properties
		m_bIsHaveRunPr			=	FALSE;

		PrlList defaultRunPr;
		if (oXmlParagraph.Property.is_init() && oXmlParagraph.Property->RunProperty.is_init())
		{
			defaultRunPr		=	ConvertRunProperties(*oXmlParagraph.Property->RunProperty);

			CPrCopier copier(&styleRunPr, &defaultRunPr);
		}

		m_bHaveSeparateFldChar	=	false;

		bool haveGoBack			=	FALSE;	//TODO : some content

		for (size_t i = 0; i < oXmlParagraph.Items->size(); ++i)
		{
			const OOX::Logic::ParagraphItem& oParagraphItem	= oXmlParagraph.Items->operator[](i);

			std::wstring strRunType;

			if (oParagraphItem.is<OOX::Logic::Run>())
			{
				ASCDocFileFormat::Run oAddRun = ConvertRun<T>(oParagraphItem.as<OOX::Logic::Run>(), styleRunPr, strRunType);

				if (m_bIsInlineShape)
				{
					//ASCDocFileFormat::Picture oPicture (std::wstring(L"c:\\man.png"), 0, 0, 1000, 1000);
					//oParagraph.AddParagraphItem(ASCDocFileFormat::Run(oPicture));

					oParagraph.AddParagraphItem(m_oInlineShape);
				}
				else
				{
					oParagraph.AddParagraphItem(oAddRun);
				}

				//ASCDocFileFormat::Picture oPicture (std::wstring(L"c:\\man.png"), 0, 0, 5000, 5000);
				//oParagraph.AddParagraphItem(ASCDocFileFormat::Run(oPicture));

				m_bIsInlineShape	=	FALSE;
			}
			else if (oParagraphItem.is<OOX::Logic::Hyperlink>())
			{
				oParagraph.AddParagraphItem (ConvertHyperlink<T>(oParagraphItem.as<OOX::Logic::Hyperlink>(), styleRunPr));
			}
			else if (oParagraphItem.is<OOX::Logic::BookmarkStart>())
			{
				const OOX::Logic::BookmarkStart& oBookMark = oParagraphItem.as<OOX::Logic::BookmarkStart>();
				if (std::wstring(_T("_GoBack")) == FormatUtils::UTF8Decode(oBookMark.Name))
				{
					haveGoBack	=	TRUE;
					continue;
				}

				oParagraph.AddParagraphItem (ASCDocFileFormat::BookmarkStart(FormatUtils::UTF8Decode(oBookMark.Id), FormatUtils::UTF8Decode(oBookMark.Name)));
			}
			else if (oParagraphItem.is<OOX::Logic::BookmarkEnd>())
			{
				if (haveGoBack)
				{
					haveGoBack	=	FALSE;
					continue;
				}

				const OOX::Logic::BookmarkEnd& oBookMark = oParagraphItem.as<OOX::Logic::BookmarkEnd>();
				oParagraph.AddParagraphItem (ASCDocFileFormat::BookmarkEnd(FormatUtils::UTF8Decode(oBookMark.Id)));
			}
			else if (oParagraphItem.is<OOX::Logic::FldSimple>())
			{
				// TODO: 

				ConvertFldSimple<T>(oParagraphItem.as<OOX::Logic::FldSimple>(), styleRunPr, oParagraph, strRunType);
			}
			else if (oParagraphItem.is<OOX::Logic::Insert>())
			{
				const OOX::Logic::Insert& insertElement = oParagraphItem.as<OOX::Logic::Insert>();

				if (!insertElement.Runs->empty())
				{
					size_t count = (*insertElement.Runs).size();
					const std::vector<OOX::Logic::Run>& items = (*insertElement.Runs);

					for (size_t j = 0; j < count; ++j)	
					{
						oParagraph.AddParagraphItem(ConvertRun<T>(items[j], styleRunPr, strRunType));
					}
				}
			}
			else if (oParagraphItem.is<OOX::Logic::Delete>())
			{
				const OOX::Logic::Delete& deleteElement = oParagraphItem.as<OOX::Logic::Delete>();

				if (!deleteElement.Runs->empty())
				{
					size_t count = (*deleteElement.Runs).size();
					const std::vector<OOX::Logic::Run>& items = (*deleteElement.Runs);

					for (size_t j = 0; j < count; ++j)	
					{			
						oParagraph.AddParagraphItem(ConvertRun<T>(items[j], styleRunPr, strRunType));
					}
				}
			}

			UpdateItemByCondition<ASCDocFileFormat::Paragraph> (&oParagraph, strRunType);
		}

		if (oXmlParagraph.Items->empty())
		{
			// Have some other properties

			ASCDocFileFormat::Run run;
			run.AddProperties(styleRunPr);

			if (oXmlParagraph.Property.is_init() && oXmlParagraph.Property->RunProperty.is_init())
			{
				run.AddOrReplaceProperties(styleRunPr);
			}

			oParagraph.AddParagraphItem(run);
		}

		oParagraph.AddProperties (ConvertParagraphProperties (m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().Default->ParagraphProperty));
		oParagraph.AddOrReplaceProperties (styleParPr);

		if (oXmlParagraph.Property.is_init())
		{
			oParagraph.AddOrReplaceProperties(ConvertParagraphProperties (*oXmlParagraph.Property));
		}

		return oParagraph;
	}
}

namespace DOCXTODOC
{
	bool CFileTransformer::ValidParagraph (const OOX::Logic::Paragraph& oXmlParagraph)
	{
		// NOT IMPEMENT

		return TRUE;
	}

	PrlList CFileTransformer::GetParagraphPropertiesFromStyleHierarchy(const string& styleID, PrlList* styleDocRunLinkProperties ) 
	{
		PrlList allParagraphProperties;

		const OOX::Styles::Style styleById = this->m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetStyleById(styleID);

		if (styleById.BasedOn.is_init())
		{
			allParagraphProperties = this->GetParagraphPropertiesFromStyleHierarchy(*styleById.BasedOn, styleDocRunLinkProperties);
		}

		if ( styleById.ParagraphProperty.is_init() )
		{
			PrlList styleParagraphProperties = ConvertParagraphProperties(*styleById.ParagraphProperty);

			for (size_t i = 0; i < styleParagraphProperties.size(); ++i)
				allParagraphProperties.push_back(styleParagraphProperties[i]);
		}

		if ((styleById.Link.is_init()) && (styleDocRunLinkProperties != NULL))
		{
			PrlList styleDocRunLinkPropertiesHierarchy = GetRunPropertiesFromStyleHierarchy( *styleById.Link );

			for (size_t i = 0; i < styleDocRunLinkPropertiesHierarchy.size(); ++i)
				styleDocRunLinkProperties->push_back(styleDocRunLinkPropertiesHierarchy[i]);
		}

		return allParagraphProperties;
	}

	PrlList CFileTransformer::GetRunPropertiesFromStyleHierarchy(const string& styleID) 
	{
		PrlList allRunProperties;

		const OOX::Styles::Style styleById = m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetStyleById( styleID );

		if ( styleById.BasedOn.is_init() )
		{
			allRunProperties = GetRunPropertiesFromStyleHierarchy(*styleById.BasedOn);
		}

		if ( styleById.RunProperty.is_init() )
		{
			PrlList styleRunProperties = this->ConvertRunProperties( *styleById.RunProperty );

			for (size_t i = 0; i < styleRunProperties.size(); ++i)
				allRunProperties.push_back(styleRunProperties[i]);
		}

		return allRunProperties;
	}

	PrlList CFileTransformer::GetTablePropertiesFromStyleHierarchy(const string& styleID) 
	{
		PrlList allTableProperties;

		const OOX::Styles::Style styleById = m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetStyleById( styleID );

		if ( styleById.BasedOn.is_init() )
		{
			allTableProperties = GetTablePropertiesFromStyleHierarchy(*styleById.BasedOn);
		}

		if ( styleById.tblPr.is_init() )
		{
			PrlList styleTableProperties = ConvertTableProperties(*styleById.tblPr);

			for (size_t i = 0; i < styleTableProperties.size(); ++i)
				allTableProperties.push_back(styleTableProperties[i]);
		}

		return allTableProperties;
	}

	std::wstring CFileTransformer::GetFontNameByThemeName(const std::wstring& themeName) 
	{
		string fontName;

		const OOX::Theme::File& themeFile = this->m_docxInputFile.find<OOX::Document>().find<OOX::Theme::File>();

		if (themeFile.themeElements.is_init() && themeFile.themeElements->fontScheme.is_init())
		{
			const OOX::Theme::FontScheme& fontScheme = *themeFile.themeElements->fontScheme;

			std::wstring major		=	std::wstring(L"major");
			std::wstring minor		=	std::wstring(L"minor");

			std::wstring hAnsi		=	std::wstring(L"HAnsi");
			std::wstring eastAsia	=	std::wstring(L"EastAsia");
			std::wstring bidi		=	std::wstring(L"Bidi");

			if ( fontScheme.majorFont.is_init() && ( search( themeName.begin(), themeName.end(), major.begin(), major.end() ) != themeName.end() ) )
			{
				const OOX::Theme::MajorFont& majorFont = *fontScheme.majorFont;

				if ( ( majorFont.latin.is_init() ) && ( search( themeName.begin(), themeName.end(), hAnsi.begin(), hAnsi.end() ) != themeName.end() ) )
				{
					fontName = *majorFont.latin;    
				}
				else if ( majorFont.ea.is_init() && ( search( themeName.begin(), themeName.end(), eastAsia.begin(), eastAsia.end() ) != themeName.end() ) )
				{
					fontName = *majorFont.ea;    
				}
				else if ( majorFont.cs.is_init() && ( search( themeName.begin(), themeName.end(), bidi.begin(), bidi.end() ) != themeName.end() ) )
				{
					fontName = *majorFont.cs;    
				}
			}
			else if ( search( themeName.begin(), themeName.end(), minor.begin(), minor.end() ) != themeName.end() )
			{
				const OOX::Theme::MinorFont& minorFont = *fontScheme.minorFont;

				if ( minorFont.latin.is_init() && ( search( themeName.begin(), themeName.end(), hAnsi.begin(), hAnsi.end() ) != themeName.end() ) )
				{
					fontName = *minorFont.latin;    
				}
				else if ( minorFont.ea.is_init() && ( search( themeName.begin(), themeName.end(), eastAsia.begin(), eastAsia.end() ) != themeName.end() ) )
				{
					fontName = *minorFont.ea;    
				}
				else if ( minorFont.cs.is_init() && ( search( themeName.begin(), themeName.end(), bidi.begin(), bidi.end() ) != themeName.end() ) )
				{
					fontName = *minorFont.cs;    
				}
			}
		}

		return  string2wstring__(fontName);
	}

	template<class T> void CFileTransformer::ConvertFldSimple(const OOX::Logic::FldSimple& fldSimpleDocx, const PrlList& styleDocRunProperties, ASCDocFileFormat::Paragraph& docParagraph, std::wstring& strRunType) 
	{
		string::size_type findIndex = string::npos;

		findIndex = fldSimpleDocx.Instr->find( "PAGE" );

		if ( findIndex != string::npos )
		{
			docParagraph.AddParagraphItem( ASCDocFileFormat::Run( ASCDocFileFormat::FldChar( ASCDocFileFormat::FldCharTypeBegin, ASCDocFileFormat::Constants::fltPAGE ) ) );
			docParagraph.AddParagraphItem( ASCDocFileFormat::Run( ASCDocFileFormat::FldChar( ASCDocFileFormat::FldCharTypeSeparate ) ) );
			docParagraph.AddParagraphItem( ConvertRun<T>( *fldSimpleDocx.Run, styleDocRunProperties, strRunType ) );
			docParagraph.AddParagraphItem( ASCDocFileFormat::Run( ASCDocFileFormat::FldChar( ASCDocFileFormat::FldCharTypeEnd ) ) );
		}

		findIndex = fldSimpleDocx.Instr->find( "SYMBOL" );

		if ( findIndex != string::npos )
		{
			ASCDocFileFormat::Run fldSymbolRun;

			fldSymbolRun.AddRunItem( ASCDocFileFormat::Text( FormatUtils::UTF8Decode( *fldSimpleDocx.Instr ) ) );
			fldSymbolRun.AddProperties( styleDocRunProperties );

			strRunType = _T( "SYMBOL" );

			docParagraph.AddParagraphItem( ASCDocFileFormat::Run( ASCDocFileFormat::FldChar( ASCDocFileFormat::FldCharTypeBegin, ASCDocFileFormat::Constants::fltSYMBOL ) ) );
			docParagraph.AddParagraphItem( fldSymbolRun );
			docParagraph.AddParagraphItem( ASCDocFileFormat::Run( ASCDocFileFormat::FldChar( ASCDocFileFormat::FldCharTypeSeparate ) ) );
			docParagraph.AddParagraphItem( ASCDocFileFormat::Run( ASCDocFileFormat::FldChar( ASCDocFileFormat::FldCharTypeEnd ) ) );
		}  
	}

	template<class T> ASCDocFileFormat::Hyperlink CFileTransformer::ConvertHyperlink(const OOX::Logic::Hyperlink& docxHyperlink, const PrlList& styleDocRunProperties ) 
	{
		ASCDocFileFormat::Hyperlink docHyperlink;

		if (docxHyperlink.rId.is_init())
		{
			OOX::HyperLink* hyperlink = NULL;
			const OOX::RId& rid = *docxHyperlink.rId;

			if (typeid(T) != typeid(OOX::Document))
			{
				if (m_docxInputFile.find<OOX::Document>().find<T>().exist(rid))
				{
					hyperlink = dynamic_cast<OOX::HyperLink*>(m_docxInputFile.find<OOX::Document>().find<T>()[rid].operator->());
				}
			}
			else
			{
				if (m_docxInputFile.find<OOX::Document>().exist(rid))
				{
					hyperlink = dynamic_cast<OOX::HyperLink*>(m_docxInputFile.find<OOX::Document>()[rid].operator->());
				}
			}

			if (hyperlink)
			{
				docHyperlink.SetURL(hyperlink->GetPath().c_str());
			}
		}

		if (docxHyperlink.Anchor.is_init())
		{
			docHyperlink.SetLocationInTheFile(FormatUtils::UTF8Decode( *docxHyperlink.Anchor ).c_str());
		}

		size_t count = (*docxHyperlink.Runs).size();
		const std::vector<OOX::Logic::Run>& items = (*docxHyperlink.Runs);

		for (size_t i = 0; i < count; ++i)	
		{			
			std::wstring strRunType;
			docHyperlink.AddRun(ConvertRun<T>(items[i], styleDocRunProperties, strRunType));
			UpdateItemByCondition<ASCDocFileFormat::Hyperlink>(&docHyperlink, strRunType);
		}

		return docHyperlink;
	}

	template<class T> void CFileTransformer::UpdateItemByCondition(T* docItem, const wstring& condition) 
	{
		if ( ( docItem != NULL ) && ( !condition.empty() ) )
		{
			static bool haveSeparator = false;

			ASCDocFileFormat::FldChar* fldChar = NULL;

			bool findComplete	=	FALSE;

			for (T::reverse_iterator riter = docItem->rbegin(); riter != docItem->rend(); ++riter)
			{
				if (riter->is<ASCDocFileFormat::Run>())
				{
					ASCDocFileFormat::Run& run = riter->as<ASCDocFileFormat::Run>();

					for (ASCDocFileFormat::Run::reverse_iterator runRIter = run.rbegin(); runRIter != run.rend(); ++runRIter)
					{
						if ( runRIter->is<ASCDocFileFormat::FldChar>() )
						{
							fldChar = &runRIter->as<ASCDocFileFormat::FldChar>();

							findComplete	=	TRUE;
							break;
						}
					}

					if (findComplete)
						break;
				}
			}

			if ( NULL == fldChar )
				return;

			unsigned char charType	=	fldChar->CharType();

			if (condition == std::wstring(_T("HYPERLINK")))
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties(ASCDocFileFormat::Constants::fltHYPERLINK);
				}
			}
			else if ( condition == std::wstring( _T( "PAGEREF" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( ASCDocFileFormat::Constants::fltPAGEREF );
				}
			}
			else if ( condition == std::wstring( _T( "TOC" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( ASCDocFileFormat::Constants::fltTOC );
				}
			}
			else if ( condition == std::wstring( _T( "PAGE" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( ASCDocFileFormat::Constants::fltPAGE );
				}  
			}
			else if ( condition == std::wstring( _T( "SYMBOL" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( ASCDocFileFormat::Constants::fltSYMBOL );
				}  
			}
			else if ( condition == std::wstring( _T( "ADDRESSBLOCK" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( ASCDocFileFormat::Constants::fltADDRESSBLOCK );
				}  
			}
			else if ( condition == std::wstring( _T( "GREETINGLINE" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( ASCDocFileFormat::Constants::fltGREETINGLINE );
				}  
			}
			else if ( condition == std::wstring( _T( "MERGEFIELD" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( ASCDocFileFormat::Constants::fltMERGEFIELD );
				}  
			}
			else if ( condition == std::wstring( _T( "separate" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharSeparate )
				{
					//fldChar->SetFieldCharacterProperties(0);
					haveSeparator = true;
				}
			}
			else if ( condition == std::wstring( _T( "end" ) ) )
			{
				if (charType == ASCDocFileFormat::FldChar::FldCharEnd )
				{
					fldChar->SetFieldCharacterProperties( (unsigned char)ASCDocFileFormat::grffldEnd( false, false, false, false, false, false, false, haveSeparator ) );

					haveSeparator = false;
				}
			}
		}  
	}

	//
	ASCDocFileFormat::Constants::VerticalMergeFlag CFileTransformer::ConvertTableVerticalMergeFlag(const nullable
		ASCDocFileFormat::Constants::VerticalMergeFlag vmf = ASCDocFileFormat::Constants::fvmClear;

		if ( vMerge.is_init() )
		{
			if ( vMerge->Value.is_init() )
			{
				if ( *vMerge->Value == string( "restart" ) )
				{
					vmf = ASCDocFileFormat::Constants::fvmRestart;
				}
				else if ( *vMerge->Value == string( "continue" ) )
				{
					vmf = ASCDocFileFormat::Constants::fvmMerge;
				}
			}
			else
			{
				vmf = ASCDocFileFormat::Constants::fvmMerge;
			}
		}

		return vmf;
	}

	const PrlList CFileTransformer::ConvertRunProperties(const OOX::Logic::RunProperty& docxRunProperties) 
	{
		m_bIsHaveRunPr	=	TRUE;

		PrlList docRunProperties;

		if ( docxRunProperties.Bold.is_init() )
		{
			unsigned char bold = ( ( *docxRunProperties.Bold ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFBold, &bold ) );
		}

		if ( docxRunProperties.Italic.is_init() )
		{
			unsigned char italic = ( ( *docxRunProperties.Italic ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFItalic, &italic ) );
		}

		if ( docxRunProperties.Under.is_init() && *docxRunProperties.Under && docxRunProperties.UnderType.is_init() )
		{
			unsigned char under = this->kulMap[*docxRunProperties.UnderType];

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCKul, &under ) );
		}

		if ( docxRunProperties.Strike.is_init() )
		{
			unsigned char strike = ( ( *docxRunProperties.Strike ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFStrike, &strike ) );
		}

		if ( docxRunProperties.DStrike.is_init() )
		{
			unsigned char dStrike = ( ( *docxRunProperties.DStrike ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFDStrike, &dStrike ) );
		}

		if ( docxRunProperties.SmallCaps.is_init() )
		{
			unsigned char smallCaps = ( ( *docxRunProperties.SmallCaps ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFSmallCaps, &smallCaps ) );
		}

		if ( docxRunProperties.Caps.is_init() )
		{
			unsigned char caps = ( ( *docxRunProperties.Caps ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFCaps, &caps ) );
		}

		if ( docxRunProperties.Emboss.is_init() )
		{
			unsigned char emboss = ( ( *docxRunProperties.Emboss ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFEmboss, &emboss ) );
		}

		if ( docxRunProperties.Imprint.is_init() )
		{
			unsigned char imprint = ( ( *docxRunProperties.Imprint ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFImprint, &imprint ) );
		}

		if ( docxRunProperties.Outline.is_init() )
		{
			unsigned char outline = ( ( *docxRunProperties.Outline ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFOutline, &outline ) );
		}

		if ( docxRunProperties.Shadow.is_init() )
		{
			unsigned char shadow = ( ( *docxRunProperties.Shadow ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFShadow, &shadow ) );
		}

		if ( docxRunProperties.Vanish.is_init() )
		{
			unsigned char vanish = ( ( *docxRunProperties.Vanish ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFVanish, &vanish ) );
		}

		if ( docxRunProperties.FontSize.is_init() )
		{
			unsigned short fontSize = (unsigned short)docxRunProperties.FontSize;
			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHps, (unsigned char*)&fontSize ) );
			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsBi, (unsigned char*)&fontSize ) );
		}

		if (docxRunProperties.FontColor.is_init())
		{
			int colorIntValue	=	ASCDocFileFormat::COLORREF::cvAuto;
			if (false == docxRunProperties.FontColor->isAuto())
				colorIntValue	=	HexString2Int( docxRunProperties.FontColor->ToString() );

			ASCDocFileFormat::COLORREF color (colorIntValue);

			docRunProperties.push_back (ASCDocFileFormat::Prl((short)DocFileFormat::sprmCCv, color));
		}

		if ( docxRunProperties.Highlight.is_init() )
		{
			unsigned char ico = DOCXDOCUTILS::ColorToIco (*docxRunProperties.Highlight);
			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHighlight, &ico ) );
		}

		if ( docxRunProperties.Shading.is_init() && docxRunProperties.Shading->fill.is_init() )
		{
			int colorIntValue	=	0;
			int fillAuto		=	0x00;

			if (docxRunProperties.Shading->fill == "auto")
			{
				fillAuto = ASCDocFileFormat::COLORREF::cvAuto;
			}
			else
			{
				colorIntValue = HexString2Int( *docxRunProperties.Shading->fill );
			}

			ASCDocFileFormat::SHDOperand shdOperand ( 
				ASCDocFileFormat::Shd( ASCDocFileFormat::COLORREF( (int)( 0 | fillAuto ) ), 
				ASCDocFileFormat::COLORREF( colorIntValue | fillAuto ),
				0 ) );

			docRunProperties.push_back (ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCShd, shdOperand ));
		}

		if ( docxRunProperties.rFonts.is_init() )
		{
			short fontIndex = 0;

			if ( docxRunProperties.rFonts->ascii.is_init() && !docxRunProperties.rFonts->AsciiTheme.is_init() )
			{
				std::wstring strFontName = string2wstring__(docxRunProperties.rFonts->ascii);
				if (strFontName.length())
				{			
					AddInternalFont (strFontName);

					fontIndex	=	m_mapFontTableMap[strFontName];

					docRunProperties.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmCRgFtc0, (unsigned char*)&fontIndex));
				}
			}
			else if ( docxRunProperties.rFonts->AsciiTheme.is_init())
			{
				std::wstring fontNameByThemeName = GetFontNameByThemeName(string2wstring__(docxRunProperties.rFonts->AsciiTheme) );

				if (!fontNameByThemeName.empty())
				{
					std::map<std::wstring, short>::const_iterator findResult = m_mapFontTableMap.find(fontNameByThemeName);  

					if (findResult != m_mapFontTableMap.end())
					{
						fontIndex = findResult->second;
					}

					docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc0, (unsigned char*)&fontIndex ) );
				}
			}

			if (docxRunProperties.rFonts->Cs.is_init() && !docxRunProperties.rFonts->Cstheme.is_init())
			{
				std::wstring strFontName	= string2wstring__(docxRunProperties.rFonts->Cs);
				if (strFontName.length())
				{			
					AddInternalFont (strFontName);

					fontIndex	=	m_mapFontTableMap[strFontName];

					docRunProperties.push_back (ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (unsigned char*)&fontIndex));
				}
			}
			else if ( docxRunProperties.rFonts->Cstheme.is_init() )
			{
				std::wstring fontNameByThemeName = GetFontNameByThemeName(string2wstring__(docxRunProperties.rFonts->Cstheme));

				if ( !fontNameByThemeName.empty() )
				{
					std::map<std::wstring, short>::const_iterator findResult = m_mapFontTableMap.find(fontNameByThemeName);  

					if ( findResult != m_mapFontTableMap.end() )
					{
						fontIndex = findResult->second;
					}

					docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (unsigned char*)&fontIndex ) );
				}
			}

			if ( docxRunProperties.rFonts->hAnsi.is_init() && !docxRunProperties.rFonts->HAnsiTheme.is_init() )
			{
				std::wstring strFontName = string2wstring__(docxRunProperties.rFonts->hAnsi);
				if (strFontName.length())
				{			
					AddInternalFont (strFontName);

					fontIndex	=	m_mapFontTableMap[string2wstring__(docxRunProperties.rFonts->hAnsi)];

					docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (unsigned char*)&fontIndex ) );
				}
			}
			else if ( docxRunProperties.rFonts->HAnsiTheme.is_init() )
			{
				std::wstring fontNameByThemeName = GetFontNameByThemeName(string2wstring__(docxRunProperties.rFonts->HAnsiTheme));

				if ( !fontNameByThemeName.empty() )
				{
					map<std::wstring, short>::const_iterator findResult = m_mapFontTableMap.find(fontNameByThemeName);  

					if (findResult != m_mapFontTableMap.end())
					{
						fontIndex = findResult->second;
					}

					docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (unsigned char*)&fontIndex ) );
				}
			}
		}

		if ( docxRunProperties.Lang.is_init() )
		{
			if ( docxRunProperties.Lang->Value.is_init() )
			{
				ASCDocFileFormat::LID lid = lidMap[docxRunProperties.Lang->Value];
				docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0_80, (unsigned char*)lid ) );
				docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0, (unsigned char*)lid ) );
			}

			if ( docxRunProperties.Lang->EastAsia.is_init() )
			{
				ASCDocFileFormat::LID lid = lidMap[docxRunProperties.Lang->EastAsia];
				docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1_80, (unsigned char*)lid ) );
				docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1, (unsigned char*)lid ) );
			}

			if ( docxRunProperties.Lang->Bidi.is_init() )
			{
				ASCDocFileFormat::LID lid = lidMap[docxRunProperties.Lang->Bidi];
				docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCLidBi, (unsigned char*)lid ) );
			}
		}

		if ( docxRunProperties.Spacing.is_init() )
		{
			short CDxaSpace = ASCDocFileFormat::XAS( *docxRunProperties.Spacing );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCDxaSpace, (unsigned char*)&CDxaSpace ) );
		}

		if ( docxRunProperties.Index.is_init() )
		{
			string index = docxRunProperties.Index->ToString();
			unsigned char CIss = (unsigned char)ASCDocFileFormat::Constants::superSubScriptNormalText;

			if ( index == string( "superscript" ) )
			{
				CIss = (unsigned char)ASCDocFileFormat::Constants::superSubScriptSuperscript;  
			}
			else if ( index == string( "subscript" ) )
			{
				CIss = (unsigned char)ASCDocFileFormat::Constants::superSubScriptSubscript;  
			}

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCIss, (unsigned char*)&CIss ) );
		}

		if ( docxRunProperties.Kern.is_init() )
		{
			int kern = *docxRunProperties.Kern;

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsKern, (unsigned char*)&kern ) );
		}

		if ( docxRunProperties.Position.is_init() )
		{
			int position = *docxRunProperties.Position;

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsPos, (unsigned char*)&position ) );
		}

		if ( docxRunProperties.Scale.is_init() )
		{
			unsigned short scale = *docxRunProperties.Scale;

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCCharScale, (unsigned char*)&scale ) );
		}

		if ( docxRunProperties.Border.is_init() )
		{
			DOCXDOCUTILS::CTblBorders oBorder;
			map<std::string, unsigned char>& oBrcMap	=	oBorder.GetBrcMap ();

			unsigned int brc80 =  (unsigned int)ASCDocFileFormat::Brc80(
				docxRunProperties.Border->Bdr->Sz.get_value_or_default(), 
				oBrcMap[*docxRunProperties.Border->Bdr->Value], 
				DOCXDOCUTILS::ColorToIco (docxRunProperties.Border->Bdr->Color.get_value_or_default()), 
				docxRunProperties.Border->Bdr->Space.get_value_or_default(), 
				false, 
				false );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc80, (unsigned char*)(&brc80) ) );

			ASCDocFileFormat::BrcOperand brcOperand( ASCDocFileFormat::Brc(
				ASCDocFileFormat::COLORREF( HexString2Int( docxRunProperties.Border->Bdr->Color.get_value_or_default().ToString() ) ),
				docxRunProperties.Border->Bdr->Sz.get_value_or_default(), 
				oBrcMap[*docxRunProperties.Border->Bdr->Value],
				docxRunProperties.Border->Bdr->Space.get_value_or_default(), 
				false, 
				false ) );

			docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc, (unsigned char*)brcOperand ) );
		}

		return docRunProperties;
	}

	const PrlList CFileTransformer::ConvertParagraphProperties(const OOX::Logic::ParagraphProperty& docxParagraphProperties) 
	{
		PrlList docParagraphProperties;

		if ( docxParagraphProperties.Align.is_init() )
		{
			unsigned char justification	=	DOCXDOCUTILS::AlignFromString (docxParagraphProperties.Align->ToStringW());

			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPJc, &justification ) );
			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPJc80, &justification ) );
		}

		if ( docxParagraphProperties.Shading.is_init() && docxParagraphProperties.Shading->fill.is_init() )
		{
			int colorIntValue = 0;
			int fillAuto = 0x00;

			if ( *docxParagraphProperties.Shading->fill == "auto" )
			{
				fillAuto = ASCDocFileFormat::COLORREF::cvAuto;
			}
			else
			{
				colorIntValue = HexString2Int( *docxParagraphProperties.Shading->fill );
			}  

			ASCDocFileFormat::SHDOperand shdOperand( ASCDocFileFormat::Shd( ASCDocFileFormat::COLORREF( (int)( 0 | fillAuto ) ), 
				ASCDocFileFormat::COLORREF( colorIntValue | fillAuto ),
				0 ) );

			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPShd, shdOperand ) );
		}

		if ( docxParagraphProperties.Spacing.is_init() )
		{
			if ( docxParagraphProperties.Spacing->After.is_init() )
			{
				unsigned short pDyaAfter = (unsigned short)(*docxParagraphProperties.Spacing->After);

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaAfter, (unsigned char*)(&pDyaAfter) ) );
			}

			if ( docxParagraphProperties.Spacing->Before.is_init() )
			{
				unsigned short pDyaBefore = (unsigned short)(*docxParagraphProperties.Spacing->Before);

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaBefore, (unsigned char*)(&pDyaBefore) ) );
			}

			if ( ( docxParagraphProperties.Spacing->Line.is_init() ) && ( docxParagraphProperties.Spacing->LineRule.is_init() ) )
			{
				bool lineRule = false;
				short line = 0;

				line = (short)(*docxParagraphProperties.Spacing->Line);

				if ( *docxParagraphProperties.Spacing->LineRule == string( "exact" ) )
				{
					line = -line;
				}
				else if ( *docxParagraphProperties.Spacing->LineRule == string( "auto" ) )
				{
					lineRule = true;
				}

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaLine, (unsigned char*)ASCDocFileFormat::LSPD( line, lineRule ) ) );
			}

			if ( docxParagraphProperties.Spacing->BeforeAutospacing.is_init() )
			{
				Bool8 beforeAutospacing = 0x00;

				if ( *docxParagraphProperties.Spacing->BeforeAutospacing )
				{
					beforeAutospacing = 0x01;
				}

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPFDyaBeforeAuto, &beforeAutospacing ) );
			}

			if ( docxParagraphProperties.Spacing->AfterAutospacing.is_init() )
			{
				Bool8 afterAutospacing = 0x00;

				if ( *docxParagraphProperties.Spacing->AfterAutospacing )
				{
					afterAutospacing = 0x01;
				}

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPFDyaAfterAuto, &afterAutospacing ) );
			}
		}

		if ( docxParagraphProperties.Ind.is_init() )
		{
			if ( docxParagraphProperties.Ind->Left.is_init() )
			{
				short pDxaLeft = ASCDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->Left) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft, (unsigned char*)(&pDxaLeft) ) );
				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft80, (unsigned char*)(&pDxaLeft) ) );
			}

			if ( docxParagraphProperties.Ind->Right.is_init() )
			{
				short pDxaRight = ASCDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->Right) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaRight, (unsigned char*)(&pDxaRight) ) );
				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaRight80, (unsigned char*)(&pDxaRight) ) );
			}

			if ( docxParagraphProperties.Ind->Hanging.is_init() )
			{
				short pDxaLeft1 = ( ( ASCDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->Hanging) ) ) * ( -1 ) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft1, (unsigned char*)(&pDxaLeft1) ) );
				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft180, (unsigned char*)(&pDxaLeft1) ) );
			}

			if ( docxParagraphProperties.Ind->FirstLine.is_init() )
			{
				short pDxaLeft1 = ASCDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->FirstLine) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft1, (unsigned char*)(&pDxaLeft1) ) );
				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft180, (unsigned char*)(&pDxaLeft1) ) );
			}
		}

		if (docxParagraphProperties.NumPr.is_init())
		{
			if (docxParagraphProperties.NumPr->Ilvl.is_init())
			{
				unsigned char pIlvl	=	(unsigned char)(*docxParagraphProperties.NumPr->Ilvl);
				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPIlvl, (unsigned char*)(&pIlvl) ) );
			}

			if ( docxParagraphProperties.NumPr->NumId.is_init() )
			{
				int nNumID	=	(*docxParagraphProperties.NumPr->NumId);
				SHORT pIlfo	=	idIndexMap [nNumID];

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPIlfo, (unsigned char*)(&pIlfo) ) );
			}
		}

		if ( docxParagraphProperties.OutlineLvl.is_init() )
		{
			unsigned char POutLvl = *docxParagraphProperties.OutlineLvl;

			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPOutLvl, &POutLvl ) );
		}

		if ( *docxParagraphProperties.KeepLines )
		{
			Bool8 PFKeep = 0x01;

			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPFKeep, &PFKeep ) );
		}

		if ( *docxParagraphProperties.KeepNext )
		{
			Bool8 PFKeepFollow = 0x01;

			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPFKeepFollow, &PFKeepFollow ) );
		}

		if (*docxParagraphProperties.pageBreakBefore)
		{
			Bool8 PFPageBreakBefore = 0x01;
			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPFPageBreakBefore, &PFPageBreakBefore ) );
		}

		if ( *docxParagraphProperties.ContextualSpacing )
		{
			Bool8 PFContextualSpacing = 0x01;
			docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPFContextualSpacing, &PFContextualSpacing ) );
		}

		if ( docxParagraphProperties.ParagraphBorder.is_init() )
		{
			DOCXDOCUTILS::CTblBorders oBorder;
			map<std::string, unsigned char>& oBrcMap	=	oBorder.GetBrcMap ();

			if ( docxParagraphProperties.ParagraphBorder->Top.is_init() )
			{
				unsigned int brc80Top =  (unsigned int)ASCDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Top->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Top->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Top->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Top->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcTop80, (unsigned char*)(&brc80Top) ) );

				ASCDocFileFormat::BrcOperand brcOperandTop( ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Top->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Top->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Top->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Top->Bdr->Space.get_value_or_default(), 
					false, 
					false ) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcTop, (unsigned char*)brcOperandTop ) );
			}

			if ( docxParagraphProperties.ParagraphBorder->Bottom.is_init() )
			{
				unsigned int brc80Bottom =  (unsigned int)ASCDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcBottom80, (unsigned char*)(&brc80Bottom) ) );

				ASCDocFileFormat::BrcOperand brcOperandBottom( ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Space.get_value_or_default(),
					false, 
					false ) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcBottom, (unsigned char*)brcOperandBottom ) );
			}

			if ( docxParagraphProperties.ParagraphBorder->Left.is_init() )
			{
				unsigned int brc80Left =  (unsigned int)ASCDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Left->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Left->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Left->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Left->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcLeft80, (unsigned char*)(&brc80Left) ) );

				ASCDocFileFormat::BrcOperand brcOperandLeft( ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Left->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Left->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Left->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Left->Bdr->Space.get_value_or_default(),
					false, 
					false ) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcLeft, (unsigned char*)brcOperandLeft ) );
			}

			if ( docxParagraphProperties.ParagraphBorder->Right.is_init() )
			{
				unsigned int brc80Right =  (unsigned int)ASCDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Right->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Right->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Right->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Right->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcRight80, (unsigned char*)(&brc80Right) ) );

				ASCDocFileFormat::BrcOperand brcOperandRight( ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Right->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Right->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Right->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Right->Bdr->Space.get_value_or_default(),
					false, 
					false ) );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcRight, (unsigned char*)brcOperandRight ) );
			}
		}

		if ( docxParagraphProperties.TextFrameProperties.is_init() )
		{
			if ( docxParagraphProperties.TextFrameProperties->HAnchor.is_init() && docxParagraphProperties.TextFrameProperties->VAnchor.is_init() )
			{
				unsigned char positionCodeOperand = ASCDocFileFormat::PositionCodeOperand( this->verticalPositionCodeMap[*docxParagraphProperties.TextFrameProperties->VAnchor], this->horizontalPositionCodeMap[*docxParagraphProperties.TextFrameProperties->HAnchor] );
				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPPc, &positionCodeOperand ) );
			}

			if ( docxParagraphProperties.TextFrameProperties->Wrap.is_init() )
			{
				unsigned char PWr = this->textFrameWrappingMap[*docxParagraphProperties.TextFrameProperties->Wrap];
				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPWr, &PWr ) );
			}

			if ( docxParagraphProperties.TextFrameProperties->X.is_init() )
			{
				short PDxaAbs = ASCDocFileFormat::XAS_plusOne ((short)(*docxParagraphProperties.TextFrameProperties->X));
				docParagraphProperties.push_back (ASCDocFileFormat::Prl((short)DocFileFormat::sprmPDxaAbs, (unsigned char*)(&PDxaAbs)));
			}

			if ( docxParagraphProperties.TextFrameProperties->Y.is_init() )
			{
				short PDyaAbs = ASCDocFileFormat::YAS_plusOne ((short)*docxParagraphProperties.TextFrameProperties->Y);
				docParagraphProperties.push_back (ASCDocFileFormat::Prl((short)DocFileFormat::sprmPDyaAbs, (unsigned char*)(&PDyaAbs)));
			}

			if ( docxParagraphProperties.TextFrameProperties->H.is_init() )
			{
				unsigned short PWHeightAbs = ASCDocFileFormat::YAS_nonNeg ((unsigned short)*docxParagraphProperties.TextFrameProperties->H);
				docParagraphProperties.push_back (ASCDocFileFormat::Prl ((short)DocFileFormat::sprmPWHeightAbs, (unsigned char*)(&PWHeightAbs)));
			}

			if ( docxParagraphProperties.TextFrameProperties->W.is_init() )
			{
				unsigned short PDxaWidth = ASCDocFileFormat::XAS_nonNeg ((unsigned short)*docxParagraphProperties.TextFrameProperties->W);
				docParagraphProperties.push_back (ASCDocFileFormat::Prl((short)DocFileFormat::sprmPDxaWidth, (unsigned char*)(&PDxaWidth)));
			}

			if ( docxParagraphProperties.TextFrameProperties->HSpace.is_init() )
			{
				unsigned short PDxaFromText = ASCDocFileFormat::XAS_nonNeg((unsigned short)*docxParagraphProperties.TextFrameProperties->HSpace);
				docParagraphProperties.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmPDxaFromText, (unsigned char*)(&PDxaFromText)));
			}

			if ( docxParagraphProperties.TextFrameProperties->VSpace.is_init() )
			{
				unsigned short PDyaFromText = ASCDocFileFormat::YAS_nonNeg((unsigned short)*docxParagraphProperties.TextFrameProperties->VSpace);
				docParagraphProperties.push_back (ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaFromText, (unsigned char*)(&PDyaFromText)));
			}
		}

		if ( docxParagraphProperties.Tabs.is_init() )
		{
			vector<ASCDocFileFormat::TBD> tbds;
			vector<ASCDocFileFormat::XAS> xass;

			size_t count = (*docxParagraphProperties.Tabs->Tabs).size();
			const std::vector<OOX::Logic::TabProperty>& items = (*docxParagraphProperties.Tabs->Tabs);

			for (size_t i = 0; i < count; ++i)	
			{			
				ASCDocFileFormat::Constants::TabJC TabStopAlign = customTabStopAlignment[*(items[i]).Val];								
				if (0x05 == (int)TabStopAlign)	// 0x05 означает clear, т.е. No Tab Stop
					continue;

				ASCDocFileFormat::TBD tbd(TabStopAlign, customTabStopLeader[items[i].Leader.get_value_or_default()]);
				tbds.push_back(tbd);

				ASCDocFileFormat::XAS xas((short)(*(items[i]).Pos));
				xass.push_back(xas);
			}

			if (xass.size() > 0 && tbds.size() > 0)
			{
				ASCDocFileFormat::PChgTabsAdd pChgTabsAdd( xass, tbds );
				ASCDocFileFormat::PChgTabsPapxOperand pChgTabsPapxOperand( ASCDocFileFormat::PChgTabsDel(), pChgTabsAdd );

				docParagraphProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmPChgTabsPapx, (unsigned char*)pChgTabsPapxOperand ) );
			}
		}

		return docParagraphProperties;
	}
}

namespace DOCXTODOC	//	LEVELS
{
	void CFileTransformer::ConvertNumbering(const OOX::Numbering& numbering)
	{
		if ((0 == numbering.AbstractNums.size()) || (0 == numbering.Nums.size()))
			return;

		ASCDocFileFormat::PlfLst plfLst(ConvertAbstractNums(numbering.AbstractNums));
		vector<ASCDocFileFormat::LVL> lvls = ConvertLVLs(numbering, numbering.AbstractNums);
		ASCDocFileFormat::ListFormattingInformation lfi(plfLst, lvls); 

		m_pDocFile->SetListFormattingInformation(lfi);
		m_pDocFile->SetListFormattingOverrideInformation(ConvertNums(numbering.Nums));
	}

	const vector<ASCDocFileFormat::LSTF> CFileTransformer::ConvertAbstractNums (const vector<OOX::Numbering::AbstractNum>& arrNums) 
	{
		// Fill LSTF ( The LSTF structure contains formatting properties that apply to an entire list. )

		vector<ASCDocFileFormat::LSTF> arrLSTF;

		int nId	=	1;	//	уникальный индентификатор для списка, любое число от 1 ~ 0xFFFFFFFF

		size_t count = arrNums.size();
		for (size_t j = 0; j < count; ++j)
		{
			const OOX::Numbering::AbstractNum& number = arrNums[j];

			if (number.Nsid.is_init())
			{
				nId			=	HexString2IntW(number.Nsid.get());
			}
			else
			{
				for (map<int, int>::const_iterator it = idLsidMap.begin(); it != idLsidMap.end(); ++it)
				{
					nId		=	__max(it->second, nId);
				}

				++nId;
			}

			idLsidMap.insert(make_pair(number.Id.get(), nId));

			ASCDocFileFormat::Tplc* tplc = ASCDocFileFormat::TplcFactory::CreateTplc(HexString2IntW(number.Tmpl.get()));

			bool fSimpleList	=	false;
			bool fAutoNum		=	false;
			bool fHybrid		=	false;

			if (number.MultiLevelType.IsInit())
			{
				if ((number.MultiLevelType.get()) == std::wstring(L"hybridMultilevel"))
					fHybrid			=	true; 
			}

			if (number.MultiLevelType.IsInit())
			{
				if ((number.MultiLevelType.get()) ==  std::wstring(L"singleLevel"))
					fSimpleList		=	true;  
			}

			arrLSTF.push_back (ASCDocFileFormat::LSTF (nId, tplc, fSimpleList, fAutoNum, fHybrid, ASCDocFileFormat::grfhic(), NULL /*!!!TODO!!!*/));

			RELEASEOBJECT(tplc);
		}

		return arrLSTF;
	}

	const vector<ASCDocFileFormat::LVL> CFileTransformer::ConvertLVLs(const OOX::Numbering& numbering, const vector<OOX::Numbering::AbstractNum>& arAbstractNums) 
	{
		vector<ASCDocFileFormat::LVL> oLevels;

		size_t numsCount = arAbstractNums.size();
		for (size_t j = 0; j < numsCount; ++j)
		{
			const OOX::Numbering::AbstractNum& oAbstractNum = arAbstractNums[j];
			size_t levCount = oAbstractNum.Levels.size();
			for (size_t i = 0; i < levCount; ++i)
			{
				const OOX::Numbering::Level& oLevel = oAbstractNum.Levels[i];
				oLevels.push_back (ConvertLVL(oLevel));
			}

			// 
			if (0 == levCount)
			{
				if (oAbstractNum.numStyleLink.is_init())
				{
					int nInd = FindAbstractNumIdWithStyleRef(numbering, oAbstractNum.numStyleLink.get());
					if ((nInd >= 0) && (nInd < (int)numsCount))
					{
						for (size_t i = 0; i < numsCount; ++i)
						{
							const OOX::Numbering::AbstractNum& oFindNum = arAbstractNums[i];
							if (oFindNum.Id.is_init())
							{
								if (nInd == oFindNum.Id.get())
								{
									size_t levCount = oFindNum.Levels.size();
									for (size_t m = 0; m < levCount; ++m)
									{
										const OOX::Numbering::Level& oLevel = oFindNum.Levels[m];
										oLevels.push_back (ConvertLVL(oLevel));
									}

									break;
								}
							}
						}
					}
				}
			}
		}

		return oLevels;
	}

	const ASCDocFileFormat::LVL CFileTransformer::ConvertLVL(const OOX::Numbering::Level& _level) 
	{
		ASCDocFileFormat::Constants::MSONFC nfc = this->numFmtMap[_level.NumFmt->ToString()];
		ASCDocFileFormat::Constants::LevelJustification jc = 
			(ASCDocFileFormat::Constants::LevelJustification)
			DOCXDOCUTILS::AlignFromString (_level.Align.get().ToStringW());
		//DOCXDOCUTILS::AlignFromString (_level.Align.get_value_or(OOX::Logic::Align(std::wstring(L"left"))).ToStringW());

		//	bool fTentative = ( ( _level.Tentative.get_value_or( 0 ) == 1 ) ? ( true ) : ( false ) );
		bool fTentative = ( ( _level.Tentative.get() == 1 ) ? ( true ) : ( false ) );
		ASCDocFileFormat::Constants::CharacterFollows ixchFollow = ASCDocFileFormat::Constants::characterFollowsTab;

		if (_level.Suffix.is_init())
		{
			if (_level.Suffix.get() == std::wstring(L"tab"))
			{
				ixchFollow = ASCDocFileFormat::Constants::characterFollowsTab;
			}
			else if (_level.Suffix == std::wstring(L"space"))
			{
				ixchFollow = ASCDocFileFormat::Constants::characterFollowsSpace;  
			}
			else
			{
				ixchFollow = ASCDocFileFormat::Constants::characterFollowsNothing;
			}
		}

		ASCDocFileFormat::LVLF lvlf(_level.Start.get(), nfc, jc, false, false, false, false, fTentative, ixchFollow, 0, 0, ASCDocFileFormat::grfhic() /*!!!TODO!!!*/ );

		PrlList grpprlPapx;
		PrlList grpprlChpx;

		if ( _level.ParagraphProperty.is_init() )
		{
			grpprlPapx = ConvertParagraphProperties( *_level.ParagraphProperty );  
		}

		if ( _level.RunProperty.is_init() )
		{
			grpprlChpx = ConvertRunProperties( *_level.RunProperty );  
		}

		return ASCDocFileFormat::LVL( lvlf, grpprlPapx, grpprlChpx, ConvertLvlText(_level.Text.get(), nfc  ) );
	}

	const ASCDocFileFormat::PlfLfo CFileTransformer::ConvertNums(const vector<OOX::Numbering::Num>& oXmlNums) 
	{
		vector<ASCDocFileFormat::LFO> lfos;
		vector<ASCDocFileFormat::LFOData> lfoDatas;

		short listIndex = 1;

		for (size_t i = 0; i < oXmlNums.size(); ++i)
		{
			const OOX::Numbering::Num& oXmlNum = oXmlNums[i];
			idIndexMap.insert( make_pair( *oXmlNum.NumId, listIndex++ ) );

			vector<ASCDocFileFormat::LFOLVL> rgLfoLvl;

			for (size_t j = 0; j < oXmlNum.LevelOverrides.size(); ++j)
			{
				const OOX::Numbering::LevelOverride& levelOverride = oXmlNum.LevelOverrides[j];
				ASCDocFileFormat::LVL lvl;

				bool bHaveLVL = false;
				if (levelOverride.Level.is_init())
				{
					lvl			=	ConvertLVL(*levelOverride.Level);
					bHaveLVL	=	true;
				}

				int iStartAt	=	0;
				bool fStartAt	=	false;

				if (levelOverride.StartOverride.is_init())
				{
					iStartAt	=	levelOverride.StartOverride.get();
					fStartAt	=	true;
				}

				rgLfoLvl.push_back(ASCDocFileFormat::LFOLVL(iStartAt, *levelOverride.Ilvl, fStartAt, ASCDocFileFormat::grfhic(), bHaveLVL ? &lvl : NULL));
			}

			int NumId = idLsidMap[oXmlNum.AbstractNumId.get()];

			lfos.push_back(ASCDocFileFormat::LFO(NumId, rgLfoLvl.size(), ASCDocFileFormat::Constants::lfoFieldNotUsed00, ASCDocFileFormat::grfhic()));
			lfoDatas.push_back(ASCDocFileFormat::LFOData(0xFFFFFFFF, rgLfoLvl));
		}

		return ASCDocFileFormat::PlfLfo(lfos, lfoDatas);
	}

	const ASCDocFileFormat::Xst CFileTransformer::ConvertLvlText(const std::wstring& strLvlText, ASCDocFileFormat::Constants::MSONFC oMsoNfc) 
	{
		std::wstring wstr(strLvlText.size(), 0);
		utf8_decode(strLvlText.begin(), strLvlText.end(), wstr.begin());

		if (wstr == std::wstring(L"%1"))
			return ASCDocFileFormat::Xst(true);

		std::wstring::iterator result = wstr.begin();
		std::wstring::iterator newResult = wstr.begin();

		newResult = find( wstr.begin(), wstr.end(), _T( '%' ) );
		wstring docPlaceHolderValue = wstring( wstr.begin(), newResult );
		result = newResult;

		while (result != wstr.end())
		{
			newResult	=	find( ( result + 1 ), wstr.end(), _T( '%' ) );
			if(result + 1 != wstr.end() && result + 2 != wstr.end())
			{
				wstring placeHolderString = wstring( ( result + 1 ), ( result + 2 ) );
				int placeHolderValue = _wtoi( placeHolderString.c_str() );
				docPlaceHolderValue += ( placeHolderValue - 1 );
				docPlaceHolderValue += wstring( ( result + 2 ), newResult );
			}

			result		=	newResult;
		}

		unsigned short cch = docPlaceHolderValue.size();
		if (ASCDocFileFormat::Constants::msonfcBullet == oMsoNfc)
			cch = 0x0001;

		return ASCDocFileFormat::Xst(docPlaceHolderValue.c_str(), cch);
	}

	int CFileTransformer::FindAbstractNumIdWithStyleRef(const OOX::Numbering& numbering, const std::wstring& refLink)	
	{
		const OOX::Styles::Style& oStyle = m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetStyleById(wstring2string__(refLink));
		if (oStyle.ParagraphProperty.is_init())
		{
			if (oStyle.ParagraphProperty->NumPr.is_init())
			{
				if (oStyle.ParagraphProperty->NumPr->NumId.is_init())
				{
					int nInd = oStyle.ParagraphProperty->NumPr->NumId;
					size_t length = numbering.Nums.size();
					for (size_t i = 0; i < length; ++i)
					{
						if (nInd == numbering.Nums[i].NumId.get())
						{
							return numbering.Nums[i].AbstractNumId.get();
						}
					}
				}
			}
		}

		return -1;
	}
}

namespace DOCXTODOC	//	STYLES
{
	void CFileTransformer::ConvertStyleSheet(const OOX::Styles& oStyleSheet)
	{
		vector<ASCDocFileFormat::LSD> mpstiilsd;

		//if ( _styleSheet.LattentStyles.is_init() )
		mpstiilsd = ConvertLatentStyles( /**_styleSheet.LattentStyles*/ );

		vector<ASCDocFileFormat::LPStd> rglpstd = ConvertStyleDefinitions (*oStyleSheet.Named);

		short ftcAsci	= 0;
		short ftcFE		= 0;
		short ftcOther	= 0;

		if (oStyleSheet.Default->RunProperty->rFonts.is_init() )
		{
			if (oStyleSheet.Default->RunProperty->rFonts->ascii.is_init() )
				ftcAsci		=	m_mapFontTableMap [string2wstring__(oStyleSheet.Default->RunProperty->rFonts->ascii)];

			if (oStyleSheet.Default->RunProperty->rFonts->Cs.is_init() )
				ftcFE		=	m_mapFontTableMap [string2wstring__(oStyleSheet.Default->RunProperty->rFonts->Cs)];

			if (oStyleSheet.Default->RunProperty->rFonts->hAnsi.is_init() )
				ftcOther	=	m_mapFontTableMap [string2wstring__(oStyleSheet.Default->RunProperty->rFonts->hAnsi)];
		}

		ASCDocFileFormat::Stshif stshif (rglpstd.size(), true, mpstiilsd.size(), ftcAsci, ftcFE, ftcOther);
		ASCDocFileFormat::StshiLsd stshiLsd (mpstiilsd);

		// MUST be ignored.  
		ASCDocFileFormat::LPStshiGrpPrl grpprlChpStandard (ConvertRunProperties(*oStyleSheet.Default->RunProperty));
		ASCDocFileFormat::LPStshiGrpPrl grpprlPapStandard (ConvertParagraphProperties(*oStyleSheet.Default->ParagraphProperty));
		ASCDocFileFormat::STSHIB stshib (grpprlChpStandard, grpprlPapStandard);

		ASCDocFileFormat::LPStshi lpStshi (ASCDocFileFormat::STSHI(stshif, 0, stshiLsd, stshib));

		m_pDocFile->SetStyleSheet (ASCDocFileFormat::STSH(lpStshi, rglpstd));
	}

	std::vector<ASCDocFileFormat::LPStd> CFileTransformer::ConvertStyleDefinitions (const vector<OOX::Styles::Style>& arrStyles)
	{
		vector<ASCDocFileFormat::LPStd> styleDefinitions(15);
		short styleIndex = 15;

		for (size_t i = 0; i < arrStyles.size(); ++i)
		{
			const OOX::Styles::Style& oXmlStyle	=	arrStyles[i];

			unsigned short sti	=	predefinedStyleIDMap[*oXmlStyle.StyleId];
			short istd			=	DOCXDOCUTILS::StiToIstd ((short)sti);

			if ( istd == -1 )
				istd = styleIndex++;

			m_mapStyleSheetMap.insert(pair<string, short>(oXmlStyle.StyleId,istd));
		}

		for (size_t i = 0; i < arrStyles.size(); ++i)
		{
			const OOX::Styles::Style& oXmlStyle	=	arrStyles[i];
			unsigned short sti	=	predefinedStyleIDMap[*oXmlStyle.StyleId];
			short istd			=	DOCXDOCUTILS::StiToIstd ((short)sti);

			ASCDocFileFormat::Constants::StyleType styleType;
			map<string, ASCDocFileFormat::Constants::StyleType>::const_iterator findResult = styleTypeMap.find( *oXmlStyle.Type );  
			if ( findResult != styleTypeMap.end() )
				styleType = findResult->second;
			else
				styleType = ASCDocFileFormat::Constants::styleTypeCharacter;

			unsigned short istdBase = 0x0FFF;
			unsigned short istdNext = 0x0000;
			ASCDocFileFormat::StdfPost2000 StdfPost2000OrNone;

			if ( oXmlStyle.BasedOn.is_init() )
			{
				istdBase = m_mapStyleSheetMap[*oXmlStyle.BasedOn];
			}

			if ( oXmlStyle.Next.is_init() )
			{
				istdNext = m_mapStyleSheetMap[*oXmlStyle.Next];  
			}

			if ( ( oXmlStyle.Link.is_init() ) && ( oXmlStyle.UiPriority.is_init() ) )
			{
				//!!!TODO: Revision!!!
				StdfPost2000OrNone = ASCDocFileFormat::StdfPost2000( (unsigned short)m_mapStyleSheetMap[*oXmlStyle.Link], false, 0, (unsigned short)(*oXmlStyle.UiPriority) );
			}

			ASCDocFileFormat::GRFSTD grfstd( false, false, false, false, false, *oXmlStyle.SemiHidden, false, *oXmlStyle.UnhideWhenUsed, *oXmlStyle.QFormat ); 
			ASCDocFileFormat::StdfBase stdfBase( sti, styleType, istdBase, istdNext, grfstd );

			std::wstring styleName( ( oXmlStyle.name->size() ), 0 );
			utf8_decode( oXmlStyle.name->begin(), oXmlStyle.name->end(), styleName.begin() );

			ASCDocFileFormat::LPUpxPapx lPUpxPapx;
			ASCDocFileFormat::LPUpxChpx lPUpxChpx;
			ASCDocFileFormat::LPUpxTapx lPUpxTapx;

			CXmlPropertyReader oXmlReader;

			if (oXmlStyle.ParagraphProperty.is_init())
				lPUpxPapx	=	ASCDocFileFormat::LPUpxPapx (ASCDocFileFormat::UpxPapx(istd, oXmlReader.GetParagraphStyleProperties (ConvertParagraphProperties(*oXmlStyle.ParagraphProperty)) ));

			if (oXmlStyle.RunProperty.is_init())
				lPUpxChpx	=	ASCDocFileFormat::LPUpxChpx( ASCDocFileFormat::UpxChpx (oXmlReader.GetRunStyleProperties( ConvertRunProperties(*oXmlStyle.RunProperty)) ));    

			if (oXmlStyle.tblPr.is_init())
				lPUpxTapx	=	ASCDocFileFormat::LPUpxTapx(ASCDocFileFormat::UpxTapx(oXmlReader.GetTableStyleProperties( ConvertTableProperties(*oXmlStyle.tblPr)) ));

			ASCDocFileFormat::GrLPUpxSw grLPUpxSw (styleType, lPUpxPapx, lPUpxChpx, lPUpxTapx);
			ASCDocFileFormat::LPStd lPStd (ASCDocFileFormat::STD(ASCDocFileFormat::Stdf(stdfBase, &StdfPost2000OrNone), 
				ASCDocFileFormat::Xstz(ASCDocFileFormat::Xst(styleName.c_str())), grLPUpxSw));
			if (-1 != istd)
			{
				styleDefinitions[istd]	=	lPStd; 
			}
			else
			{
				styleDefinitions.push_back( lPStd );
			}
		}

		return styleDefinitions;
	}

	std::vector<ASCDocFileFormat::LSD> CFileTransformer::ConvertLatentStyles()
	{
		vector<ASCDocFileFormat::LSD> latentStylesDatas;

		for ( unsigned int i = 0; i < ( sizeof(ASCDocFileFormat::LatentStylesTemplate) / sizeof(ASCDocFileFormat::LatentStylesTemplate[0]) ); i++ )
			latentStylesDatas.push_back( ASCDocFileFormat::LSD( ASCDocFileFormat::LatentStylesTemplate[i] ) );

		return latentStylesDatas;
	}

	std::string CFileTransformer::GetStyleID(const OOX::Logic::Paragraph& oXmlParagraph) 
	{
		std::string strStyleID;

		if (oXmlParagraph.Property.is_init() && oXmlParagraph.Property->PStyle.is_init())
		{
			strStyleID								=	(*oXmlParagraph.Property->PStyle);
		}
		else
		{
			const OOX::Styles::Style defaultStyle	=	m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetDefaultStyle ("paragraph");
			strStyleID								=	(*defaultStyle.StyleId);

			if (0 == strStyleID.length())
			{
				const OOX::Styles::Style oStyle		=	m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetStyleWithTypeAndName ("paragraph", "Normal");
				strStyleID							=	(*oStyle.StyleId);
			}
		}

		return strStyleID;
	}
}

namespace DOCXTODOC
{
	// настройки страниц документа
	ASCDocFileFormat::SectionProperties CFileTransformer::ConvertSectionProperties(const OOX::Logic::SectorProperty& docxSectionProperties)
	{
		PrlList docSectionProperties;

		unsigned short SXaPage = *docxSectionProperties.PageSize->Width;
		unsigned short SYaPage = *docxSectionProperties.PageSize->Height;

		docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSXaPage, (unsigned char*)&SXaPage ) );
		docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSYaPage, (unsigned char*)&SYaPage ) );

		if ( docxSectionProperties.Type.is_init() )
		{
			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSBkc, (unsigned char*)&(sectionBreakTypeMap[*docxSectionProperties.Type]) ) );
		}

		if ( docxSectionProperties.PageSize->Orient.is_init() )
		{
			unsigned char SBOrientation = (unsigned char)ASCDocFileFormat::Constants::dmOrientPortrait;

			if ( *docxSectionProperties.PageSize->Orient == string( "portrait" ) )
			{
				SBOrientation = (unsigned char)ASCDocFileFormat::Constants::dmOrientPortrait;  
			}
			else if ( *docxSectionProperties.PageSize->Orient == string( "landscape" ) )
			{
				SBOrientation = (unsigned char)ASCDocFileFormat::Constants::dmOrientLandscape;
			}

			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSBOrientation, (unsigned char*)&SBOrientation ) );
		}

		unsigned short SDxaLeft = ASCDocFileFormat::XAS_nonNeg( *docxSectionProperties.PageMargin->Left );
		unsigned short SDxaRight = ASCDocFileFormat::XAS_nonNeg( *docxSectionProperties.PageMargin->Right );
		short SDyaTop = ASCDocFileFormat::YAS( *docxSectionProperties.PageMargin->Top );
		short SDyaBottom = ASCDocFileFormat::YAS( *docxSectionProperties.PageMargin->Bottom );

		docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaLeft, (unsigned char*)&SDxaLeft ) );
		docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaRight, (unsigned char*)&SDxaRight ) );
		docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaTop, (unsigned char*)&SDyaTop ) );
		docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaBottom, (unsigned char*)&SDyaBottom ) );

		if ( docxSectionProperties.PageMargin->Gutter.is_init() )
		{
			unsigned short SDzaGutter = (unsigned short)(*docxSectionProperties.PageMargin->Gutter);

			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDzaGutter, (unsigned char*)&SDzaGutter ) );
		}

		if ( docxSectionProperties.PageMargin->Header.is_init() )
		{ 
			unsigned short SDyaHdrTop = ASCDocFileFormat::YAS_nonNeg(*docxSectionProperties.PageMargin->Header);

			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaHdrTop, (unsigned char*)&SDyaHdrTop ) );
		}

		if ( docxSectionProperties.PageMargin->Footer.is_init() )
		{ 
			unsigned short SDyaHdrBottom = ASCDocFileFormat::YAS_nonNeg(*docxSectionProperties.PageMargin->Footer);

			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaHdrBottom, (unsigned char*)&SDyaHdrBottom ) );
		}

		if ( docxSectionProperties.Columns.is_init() )
		{
			if ( docxSectionProperties.Columns->Num.is_init() )
			{
				unsigned short SCcolumns = ( *docxSectionProperties.Columns->Num - 1 );

				docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSCcolumns, (unsigned char*)&SCcolumns ) );
			}

			if ( docxSectionProperties.Columns->Space.is_init() )
			{
				unsigned short SDxaColumns = (unsigned short)ASCDocFileFormat::XAS_nonNeg( (unsigned short)(*docxSectionProperties.Columns->Space) );

				docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaColumns, (unsigned char*)&SDxaColumns ) );
			}

			unsigned char index = 0;

			size_t count = (*docxSectionProperties.Columns->Items).size();
			const std::vector<OOX::Logic::Column>& items = (*docxSectionProperties.Columns->Items);

			for (size_t i = 0; i < count; ++i)	
			{			
				ASCDocFileFormat::SDxaColWidthOperand SDxaColWidth(index, ASCDocFileFormat::XAS_nonNeg( (unsigned short)(*(items[i]).Width)));

				docSectionProperties.push_back( ASCDocFileFormat::Prl((short)DocFileFormat::sprmSDxaColWidth, (unsigned char*)SDxaColWidth));

				if (items[i].Space.is_init())
				{
					//TODO: 

					ASCDocFileFormat::SDxaColSpacingOperand SDxaColSpacing( index, ASCDocFileFormat::XAS_nonNeg( (unsigned short)(*items[i].Space) ) );

					docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaColSpacing, (unsigned char*)SDxaColSpacing ) );
				}

				++index;
			}
		}

		if ( docxSectionProperties.DocumentGrid.is_init() )
		{
			if ( docxSectionProperties.DocumentGrid->LinePitch.is_init() )
			{
				short SDyaLinePitch = ASCDocFileFormat::YAS( *docxSectionProperties.DocumentGrid->LinePitch );

				docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaLinePitch, (unsigned char*)(&SDyaLinePitch) ) );
			}

			if ( docxSectionProperties.DocumentGrid->CharSpace.is_init() )
			{
				int SDxtCharSpace = *docxSectionProperties.DocumentGrid->CharSpace;

				docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSDxtCharSpace, (unsigned char*)(&SDxtCharSpace) ) );
			}

			if ( docxSectionProperties.DocumentGrid->Type.is_init() )
			{
				unsigned short SClm = (unsigned short)ASCDocFileFormat::Constants::clmUseDefault;

				if ( *docxSectionProperties.DocumentGrid->Type == string( "Default" ) )
				{
					SClm = (unsigned short)ASCDocFileFormat::Constants::clmUseDefault;
				}
				else if ( *docxSectionProperties.DocumentGrid->Type == string( "linesAndChars" ) )
				{
					SClm = (unsigned short)ASCDocFileFormat::Constants::clmCharsAndLines;
				}
				else if ( *docxSectionProperties.DocumentGrid->Type == string( "lines" ) )
				{
					SClm = (unsigned short)ASCDocFileFormat::Constants::clmLinesOnly;
				}
				else if ( *docxSectionProperties.DocumentGrid->Type == string( "snapToChars" ) )
				{
					SClm = (unsigned short)ASCDocFileFormat::Constants::clmEnforceGrid;
				}

				docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSClm, (unsigned char*)(&SClm) ) );
			}
		}

		if ( docxSectionProperties.FootNoteProperty.is_init() )
		{
			unsigned short sNfcFtnRef = (unsigned short)(this->numFmtMap[docxSectionProperties.FootNoteProperty->NumFormat->ToString()]);
			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSNfcFtnRef, (unsigned char*)(&sNfcFtnRef) ) );

			unsigned short sNFtn = (unsigned short)(docxSectionProperties.FootNoteProperty->NumStart);
			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSNFtn, (unsigned char*)(&sNFtn) ) );
		}

		if ( docxSectionProperties.EndNoteProperty.is_init() )
		{
			unsigned short sNfcEdnRef = (unsigned short)(this->numFmtMap[docxSectionProperties.EndNoteProperty->NumFormat->ToString()]);
			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSNfcEdnRef, (unsigned char*)(&sNfcEdnRef) ) );

			unsigned short sNEdn = (unsigned short)(docxSectionProperties.EndNoteProperty->NumStart);
			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSNEdn, (unsigned char*)(&sNEdn) ) );
		}

		AddLineNumberingSettings (docxSectionProperties, docSectionProperties);		//	<w:lnNumType>

		if (*docxSectionProperties.TitlePage)
		{
			Bool8 SFTitlePage = ( ( *docxSectionProperties.TitlePage ) ? ( 1 ) : ( 0 ) );
			docSectionProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmSFTitlePage, &SFTitlePage ) );
		}

		ASCDocFileFormat::Sepx sepx (docSectionProperties);
		ASCDocFileFormat::SectionProperties oSectionProperties (sepx);

		for ( vector<OOX::Logic::HeaderReference>::const_iterator headerReferencesIter = docxSectionProperties.Headers->begin(); 
			headerReferencesIter != docxSectionProperties.Headers->end();
			++headerReferencesIter )
		{
			OOX::Header* pHeader = dynamic_cast<OOX::Header*>(m_docxInputFile.find<OOX::Document>()[headerReferencesIter->rId].operator->()); 

			if (pHeader)
			{
				if ( headerReferencesIter->Type == "even" )
				{
					oSectionProperties.SetEvenPageHeader (ConvertHeader(*pHeader));  
				}
				else if ( headerReferencesIter->Type == "default" )
				{
					oSectionProperties.SetOddPageHeader (ConvertHeader(*pHeader));  
				}
				else if ( headerReferencesIter->Type == "first" )
				{
					oSectionProperties.SetFirstPageHeader (ConvertHeader(*pHeader));  
				}
			}
		}

		for ( vector<OOX::Logic::FooterReference>::const_iterator footerReferencesIter = docxSectionProperties.Footers->begin(); 
			footerReferencesIter != docxSectionProperties.Footers->end(); 
			++footerReferencesIter )
		{
			OOX::Footer* pFooter = dynamic_cast<OOX::Footer*>(m_docxInputFile.find<OOX::Document>()[footerReferencesIter->rId].operator->());  
			if (pFooter)
			{
				if ( footerReferencesIter->Type == "even" )
				{
					oSectionProperties.SetEvenPageFooter (ConvertFooter(*pFooter));  
				}
				else if ( footerReferencesIter->Type == "default" )
				{
					oSectionProperties.SetOddPageFooter (ConvertFooter(*pFooter));  
				}
				else if ( footerReferencesIter->Type == "first" )
				{
					oSectionProperties.SetFirstPageFooter (ConvertFooter(*pFooter));  
				}
			}
		}

		return oSectionProperties;
	}

	bool CFileTransformer::AddLineNumberingSettings(const OOX::Logic::SectorProperty& oSection, PrlList& arrSettings)
	{
		//	Line Numbering Restart Setting

		if (oSection.LnNumType->countBy.is_init())
		{
			//	Line Number Increments to Display

			short countBy			=	(unsigned short)(oSection.LnNumType->countBy);
			arrSettings.push_back (ASCDocFileFormat::Prl ((short)DocFileFormat::sprmSNLnnMod, (unsigned char*)(&countBy) ) );		

			unsigned short mode		=	1;	//
			if (oSection.LnNumType->restart.is_init())
			{
				if ("newPage" == oSection.LnNumType->restart)
					mode			=	0x00;	//	DocFileFormat::lncPerPage;

				if ("newSection" == oSection.LnNumType->restart)
					mode			=	0x01;	//	DocFileFormat::lncRestart;

				if ("continuous" == oSection.LnNumType->restart)
					mode			=	0x02;	//	DocFileFormat::lncContinue;
			}

			arrSettings.push_back (ASCDocFileFormat::Prl ((short)DocFileFormat::sprmSLnc, (unsigned char*)(&mode) ) );

			//	Line Numbering Starting Value

			unsigned short start	=	0;
			if (oSection.LnNumType->start.is_init())
				start				=	(unsigned short)(oSection.LnNumType->start);

			arrSettings.push_back (ASCDocFileFormat::Prl ((short)DocFileFormat::sprmSLnnMin, (unsigned char*)(&start) ) );

			return true;
		}

		return false;
	}

	// верхний колонтитул
	ASCDocFileFormat::Header CFileTransformer::ConvertHeader(const OOX::Header& oHeader) 
	{
		m_oOArtBuilder.SetLocation(HEADER_DOCUMENT);

		ASCDocFileFormat::Header docHeader;

		size_t count = oHeader.m_items.size();
		for (size_t i = 0; i < count; ++i)
		{
			const OOX::Logic::TextItem& oItem = oHeader.m_items[i];
			if (oItem.is<OOX::Logic::Paragraph>())
			{
				const OOX::Logic::Paragraph& oMSDocXParagraph = oItem.as<OOX::Logic::Paragraph>();
				if (ValidParagraph(oMSDocXParagraph))
				{
					ASCDocFileFormat::Paragraph oMSDocParagraph = ConvertParagraph<OOX::Header>(oMSDocXParagraph);
					docHeader.AddTextItem (oMSDocParagraph);
				}
			}
			else if (oItem.is<OOX::Logic::Table>())
			{
				ASCDocFileFormat::Table oMSDocTable = CreateTable<OOX::Header>(oItem.as<OOX::Logic::Table>());
				docHeader.AddTextItem (oMSDocTable);
			}
			else if (oItem.is<OOX::Logic::Sdt>())
			{
				const OOX::Logic::SdtContent& oStdC = oItem.as<OOX::Logic::Sdt>().Content;

				size_t stdCount = oStdC.m_items.size();
				for (size_t j = 0; j < stdCount; ++j)
				{
					const OOX::Logic::TextItem& oStdItem = oStdC.m_items[j];
					if (oStdItem.is<OOX::Logic::Paragraph>())
					{
						const OOX::Logic::Paragraph& oMSDocXParagraph = oStdItem.as<OOX::Logic::Paragraph>();
						if (ValidParagraph(oMSDocXParagraph))
						{
							ASCDocFileFormat::Paragraph oMSDocParagraph = ConvertParagraph<OOX::Header>(oMSDocXParagraph);
							docHeader.AddTextItem (oMSDocParagraph);
						}
					}
					else if (oStdItem.is<OOX::Logic::Table>())
					{
						ASCDocFileFormat::Table oMSDocTable = CreateTable<OOX::Header>(oStdItem.as<OOX::Logic::Table>());
						docHeader.AddTextItem (oMSDocTable);
					}
				}
			}
		}

		m_oOArtBuilder.SetLocation(MAIN_DOCUMENT);

#ifdef _DEBUG		
		// DOCXDOCUTILS::DebugStrPrint (_T("Header : "), docHeader.GetAllText());
#endif
		return docHeader;
	}

	// нижний колонтитул
	ASCDocFileFormat::Footer CFileTransformer::ConvertFooter(const OOX::Footer& oFooter) 
	{
		m_oOArtBuilder.SetLocation (HEADER_DOCUMENT);

		ASCDocFileFormat::Footer docFooter;

		size_t count = oFooter.m_items.size();
		for (size_t i = 0; i < count; ++i)
		{
			const OOX::Logic::TextItem& oItem = oFooter.m_items[i];
			if (oItem.is<OOX::Logic::Paragraph>())
			{
				const OOX::Logic::Paragraph& oMSDocXParagraph = oItem.as<OOX::Logic::Paragraph>();
				if (ValidParagraph(oMSDocXParagraph))
				{
					ASCDocFileFormat::Paragraph oMSDocParagraph	= ConvertParagraph<OOX::Footer>(oMSDocXParagraph);
					docFooter.AddTextItem (oMSDocParagraph);
				}
			}
			else if (oItem.is<OOX::Logic::Table>())
			{
				ASCDocFileFormat::Table oMSDocTable = CreateTable<OOX::Footer> (oItem.as<OOX::Logic::Table>());
				docFooter.AddTextItem (oMSDocTable);
			}
			else if (oItem.is<OOX::Logic::Sdt>())
			{
				const OOX::Logic::SdtContent& oStdC = oItem.as<OOX::Logic::Sdt>().Content;

				size_t stdCount = oStdC.m_items.size();
				for (size_t j = 0; j < stdCount; ++j)
				{
					const OOX::Logic::TextItem& oStdItem = oStdC.m_items[j];

					if (oStdItem.is<OOX::Logic::Paragraph>())
					{
						const OOX::Logic::Paragraph& oMSDocXParagraph = oStdItem.as<OOX::Logic::Paragraph>();
						if (ValidParagraph(oMSDocXParagraph))
						{
							ASCDocFileFormat::Paragraph oMSDocParagraph = ConvertParagraph<OOX::Footer>(oMSDocXParagraph);
							docFooter.AddTextItem (oMSDocParagraph);
						}
					}
					else if (oStdItem.is<OOX::Logic::Table>())
					{
						ASCDocFileFormat::Table oMSDocTable = CreateTable<OOX::Footer> (oStdItem.as<OOX::Logic::Table>());
						docFooter.AddTextItem (oMSDocTable);
					}
				}
			}
		}

		m_oOArtBuilder.SetLocation (MAIN_DOCUMENT);

#ifdef _DEBUG		
		// DOCXDOCUTILS::DebugStrPrint (_T("Footer : "), docFooter.GetAllText());
#endif
		return docFooter;
	}
}

namespace DOCXTODOC	//	TABLE
{
	template<class T> ASCDocFileFormat::Table CFileTransformer::CreateTable(const OOX::Logic::Table& oXmlTable) 
	{
		static unsigned int tableDepth = 1;

		ASCDocFileFormat::Table table;

		OOX::Styles::Style oTableStyle;
		if (oXmlTable.tblPr->Style.is_init())
		{			
			oTableStyle	= m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetStyleById(*oXmlTable.tblPr->Style);
		}

		DOCXDOCUTILS::CTblBorders oTblBorders (oXmlTable, oTableStyle);

		int nYC		=	(int)oXmlTable.Rows->size();
		for (int nY = 0; nY < nYC; ++nY)
		{
			const OOX::Logic::TableRow& oXmlTableRow = oXmlTable.Rows->operator [](nY);

			ASCDocFileFormat::TableRow tableRow (tableDepth);

			vector<ASCDocFileFormat::XAS> xass;
			vector<ASCDocFileFormat::TC80> tc80s;

			unsigned int cellIndex = 0;

			int nXC		=	(int)oXmlTableRow.Cells->size();
			for (int nX = 0; nX < nXC; ++nX)
			{
				const OOX::Logic::TableCell& oXmlCell	=	oXmlTableRow.Cells->operator[](nX);

				ASCDocFileFormat::TableCell tableCell(tableDepth);

				ASCDocFileFormat::Constants::VerticalMergeFlag vmf = ConvertTableVerticalMergeFlag( oXmlCell.Properties->VMerge );

				oTblBorders.Append (nX, nY, nXC, nYC, oXmlTable.tblPr->tblBorders, oXmlCell.Properties->tblBorders);

				int nCellWidth	= (int)(*oXmlCell.Properties->Width->Width);
				if (0.0 == nCellWidth)
				{
					std::vector<OOX::Logic::GridColumn> columnes = *oXmlTable.tblGrid->Columns;
					if ((int)columnes.size () > nX)
					{
						nCellWidth = static_cast<int>(*(columnes[nX].Width));
					}
				}

				ASCDocFileFormat::TC80 tc80( ASCDocFileFormat::TCGRF( 
					ASCDocFileFormat::Constants::horzMergeNotMerged, 
					ASCDocFileFormat::Constants::grpfTFlrtb, vmf, 
					ASCDocFileFormat::Constants::vaTop, 
					ASCDocFileFormat::Constants::ftsDxa, false, false, false ),
					nCellWidth, 
					oTblBorders.GetTopNillBorder(), oTblBorders.GetLeftNillBorder(), oTblBorders.GetBottomNillBorder(),
					oTblBorders.GetRightNillBorder()); 				

				size_t count = (*oXmlCell.Items).size();
				const std::vector<OOX::Logic::TextItem>& items = (*oXmlCell.Items);

				for (size_t i = 0; i < count; ++i)	
				{		
					if (items[i].is<OOX::Logic::Paragraph>())
					{
						const OOX::Logic::Paragraph& oDocxParagraph		=	items[i].as<OOX::Logic::Paragraph>();
						ASCDocFileFormat::Paragraph oDocParagraph		=	ConvertParagraph<T>(oDocxParagraph);
						tableCell.AddTextItem (oDocParagraph);

						//ASCDocFileFormat::Paragraph oDocParagraph;	//	 ONLY FOR TEST
						//tableCell.AddTextItem (oDocParagraph);		//	 ONLY FOR TEST
					}
					else if (items[i].is<OOX::Logic::Table>())
					{
						tableDepth++;
						ASCDocFileFormat::Table docTable = CreateTable<T>(items[i].as<OOX::Logic::Table>());
						tableDepth--;
						tableCell.AddTextItem(docTable);
					}
				}				

				tc80s.push_back(tc80);

				tableRow.AddCell(tableCell);
				++cellIndex;
			}

			xass = BuildXASs( oXmlTable, *oXmlTableRow.Cells );

			ASCDocFileFormat::TDefTableOperand tdto( oXmlTableRow.Cells->size(), xass, tc80s );

			tableRow.AddProperty( (short)DocFileFormat::sprmTDefTable, tdto );

			tableRow.AddProperties(ConvertTableProperties(*oXmlTable.tblPr, oXmlTableRow.Cells->size()));

			if (oXmlTableRow.Properties.is_init())
			{
				tableRow.AddProperties(ConvertTableRowProperties(nY,*oXmlTableRow.Properties ) );
			}

			const vector<ASCDocFileFormat::TableBrcOperand>& oBrcs		=	oTblBorders.GetSpecificationBorders ();
			const vector<ASCDocFileFormat::TableBrc80Operand>& oBrc80s	=	oTblBorders.GetDescriptBorders ();
			for (size_t i = 0; i < oBrc80s.size(); ++i)
			{
				tableRow.AddProperty( (short)DocFileFormat::sprmTSetBrc80, oBrc80s[i] );
				tableRow.AddProperty( (short)DocFileFormat::sprmTSetBrc, oBrcs[i] );
			}

			tableRow.AddProperties(ConvertTableRowCellsProperties(oXmlTable, nY, nYC, *oXmlTableRow.Cells,oTableStyle));

			table.AddRow(tableRow);

			oTblBorders.Clean ();
		}

		return table;
	}

	PrlList CFileTransformer::ConvertTableProperties(const OOX::Logic::TableProperty& docxTableProperties, unsigned int cellsCount) 
	{
		PrlList docTableProperties;

		DOCXDOCUTILS::CTblBorders oBorder;

		ASCDocFileFormat::TableBordersOperand80 tableBordersOperand80;
		ASCDocFileFormat::TableBordersOperand tableBordersOperand;

		if ( docxTableProperties.Style.is_init() )
		{
			docTableProperties = GetTablePropertiesFromStyleHierarchy( docxTableProperties.Style );

			short istd = m_mapStyleSheetMap[*docxTableProperties.Style];
			docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTIstd, (unsigned char*)(&istd) ) );

			if ( istd < (short)m_pDocFile->GetStyleSheet().Count() )
			{
				PrlList styleTableProperties = m_pDocFile->GetStyleSheet()[istd].GetProperties();

				for (PrlList::const_iterator iter = styleTableProperties.begin(); iter != styleTableProperties.end(); ++iter)
				{
					docTableProperties.push_back( *iter );

					switch ( iter->GetSprmCode() )
					{
					case ( (unsigned short)DocFileFormat::sprmTTableBorders80 ):
						{
							tableBordersOperand80 = ASCDocFileFormat::TableBordersOperand80( *iter );
						}
						break;

					case ( (unsigned short)DocFileFormat::sprmTTableBorders ):
						{
							tableBordersOperand = ASCDocFileFormat::TableBordersOperand( *iter );
						}
						break;
					}
				}
			}
		}

		if ( docxTableProperties.Look.is_init() )
		{
			ASCDocFileFormat::TLP tlp( ASCDocFileFormat::Constants::itlNone, ASCDocFileFormat::Fatl( HexString2Int( *docxTableProperties.Look ) ) );

			docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTTlp, tlp ) );
		}

		if ( docxTableProperties.Width.is_init() )
		{
			ASCDocFileFormat::FtsWWidth_Table ftsWWidth_Table (DOCXDOCUTILS::TableWidthUnitsFromString (*docxTableProperties.Width->Type),*docxTableProperties.Width->Width );
			docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTTableWidth, ftsWWidth_Table ) );
		}

		unsigned char bAutoFit = 0x01;
		if ( docxTableProperties.Layout.is_init() && docxTableProperties.Layout == "fixed")
			bAutoFit = 0x00;
		docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTFAutofit, (unsigned char*)&bAutoFit ) );

		if ( docxTableProperties.Ind.is_init() )
		{
			ASCDocFileFormat::FtsWWidth_Indent tWidthIndent (DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.Ind->Type), (short)docxTableProperties.Ind->Width);
			docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTWidthIndent, (unsigned char*)tWidthIndent ) );
		}

		map<std::string, unsigned char>& oBrcMap	=	oBorder.GetBrcMap ();

		if (docxTableProperties.tblBorders.is_init())
		{
			if (docxTableProperties.tblBorders->top.is_init())
			{
				ASCDocFileFormat::Brc80MayBeNil Brc80MayBeNilTop =  ASCDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Value],
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetTopBorder(Brc80MayBeNilTop);

				std::string strColor	=	docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				ASCDocFileFormat::Brc brcTop = ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetTopBorder(brcTop);
			}

			if ( docxTableProperties.tblBorders->bottom.is_init() )
			{
				ASCDocFileFormat::Brc80MayBeNil Brc80MayBeNilBottom = ASCDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetBottomBorder(Brc80MayBeNilBottom);

				std::string strColor	=	docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				ASCDocFileFormat::Brc brcBottom = ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetBottomBorder( brcBottom );
			}

			if ( docxTableProperties.tblBorders->left.is_init() )
			{
				ASCDocFileFormat::Brc80MayBeNil Brc80MayBeNilLeft = ASCDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetLeftBorder( Brc80MayBeNilLeft );

				std::string strColor	=	docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				ASCDocFileFormat::Brc brcLeft = ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetLeftBorder( brcLeft );
			}

			if ( docxTableProperties.tblBorders->right.is_init() )
			{
				ASCDocFileFormat::Brc80MayBeNil Brc80MayBeNilRight = ASCDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetRightBorder( Brc80MayBeNilRight );

				std::string strColor	=	docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				ASCDocFileFormat::Brc brcRight = ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetRightBorder( brcRight );
			}

			if ( docxTableProperties.tblBorders->insideH.is_init() )
			{
				ASCDocFileFormat::Brc80MayBeNil Brc80MayBeNilInsideH = ASCDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetHorizontalInsideBorder( Brc80MayBeNilInsideH );

				std::string strColor	=	docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				ASCDocFileFormat::Brc brcInsideH = ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetHorizontalInsideBorder( brcInsideH );
			}

			if ( docxTableProperties.tblBorders->insideV.is_init() )
			{
				ASCDocFileFormat::Brc80MayBeNil Brc80MayBeNilInsideV = ASCDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetVerticalInsideBorder( Brc80MayBeNilInsideV );

				std::string strColor	=	docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				ASCDocFileFormat::Brc brcInsideV = ASCDocFileFormat::Brc( ASCDocFileFormat::COLORREF( HexString2Int(strColor) ),
					docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetVerticalInsideBorder( brcInsideV );
			}

			docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTTableBorders80, (unsigned char*)tableBordersOperand80 ) );
			docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTTableBorders, (unsigned char*)tableBordersOperand ) );
		}

		if ( docxTableProperties.CellMar.is_init() )
		{
			if ( docxTableProperties.CellMar->Top.is_init() )
			{
				ASCDocFileFormat::CSSAOperand cssaOperandTop( ASCDocFileFormat::CSSA( ASCDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					ASCDocFileFormat::Constants::fbrcTop,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Top->Margin->Type),
					docxTableProperties.CellMar->Top->Margin->W ) );

				docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (unsigned char*)cssaOperandTop ) );
			}

			if ( docxTableProperties.CellMar->Left.is_init() )
			{
				ASCDocFileFormat::CSSAOperand cssaOperandLeft( ASCDocFileFormat::CSSA( ASCDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					ASCDocFileFormat::Constants::fbrcLeft,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Left->Margin->Type),
					docxTableProperties.CellMar->Left->Margin->W ) );

				docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (unsigned char*)cssaOperandLeft ) );
			}

			if ( docxTableProperties.CellMar->Bottom.is_init() )
			{
				ASCDocFileFormat::CSSAOperand cssaOperandBottom( ASCDocFileFormat::CSSA( ASCDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					ASCDocFileFormat::Constants::fbrcBottom,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Bottom->Margin->Type),
					docxTableProperties.CellMar->Bottom->Margin->W ) );

				docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (unsigned char*)cssaOperandBottom ) );
			}

			if ( docxTableProperties.CellMar->Right.is_init() )
			{
				ASCDocFileFormat::CSSAOperand cssaOperandRight( ASCDocFileFormat::CSSA( ASCDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					ASCDocFileFormat::Constants::fbrcRight,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Right->Margin->Type),
					docxTableProperties.CellMar->Right->Margin->W ) );

				docTableProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (unsigned char*)cssaOperandRight ) );
			}
		}

		return docTableProperties;
	}

	vector<ASCDocFileFormat::XAS> CFileTransformer::BuildXASs(const OOX::Logic::Table& oXmlTable, const vector<OOX::Logic::TableCell>& tableCells) 
	{
		std::vector<ASCDocFileFormat::XAS> resultXASs;

		resultXASs.push_back( ASCDocFileFormat::XAS( 0 ) ); //!!!TODO!!!

		int nSpanLineOff	=	0;

		for (size_t i = 0; i < tableCells.size(); ++i)
		{
			const OOX::Logic::TableCell& oXmlTblCell	=	tableCells[i];

			//Если ширина 0, то выставляем ширину gc_nZeroWidth, как это делаем word когда к пустой таблице применяется автоподбор по содержимому
			int nCellWidth = (int)(*oXmlTblCell.Properties->Width->Width);
			if (nCellWidth <= 0)
			{
				int nColInd	=	nSpanLineOff + i;
				std::vector<OOX::Logic::GridColumn> oXmlColumnes = (*oXmlTable.tblGrid->Columns);
				if ((int)oXmlColumnes.size() > nColInd)
				{
					nCellWidth	=	static_cast<int>(*(oXmlColumnes[nColInd].Width));
				}
				else
				{
					nCellWidth	=	ASCDocFormatUtils::gc_nZeroWidth;
				}
			}

			if (oXmlTblCell.Properties->GridSpan.is_init())
			{
				std::vector<OOX::Logic::GridColumn> oXmlColumnes = (*oXmlTable.tblGrid->Columns);

				int nColsSize	=	(int)oXmlColumnes.size();
				int nXmlSpan	=	oXmlTblCell.Properties->GridSpan;
				int nToMerge	=	min(nXmlSpan + (int)i + nSpanLineOff,nColsSize);

				nCellWidth		=	0;

				for (int j = i + nSpanLineOff; j < nToMerge; ++j)
				{
					nCellWidth += static_cast<int>(*(oXmlColumnes[j].Width));
				}

				if (nCellWidth <= 0)
					nCellWidth	=	ASCDocFormatUtils::gc_nZeroWidth;

				//if (0 == nSpanLineOff)
				--nXmlSpan;

				nSpanLineOff	+=	nXmlSpan;
			}

			resultXASs.push_back(ASCDocFileFormat::XAS(nCellWidth + resultXASs.back()));
		}

		return resultXASs;
	}

	//
	const PrlList CFileTransformer::ConvertTableRowProperties(int nY, const OOX::Logic::TableRowProperties& oXmlRow) 
	{
		PrlList oPrls;

		short tableRowHeight = 0;

		if ( oXmlRow.Height->Type.is_init() && ( *oXmlRow.Height->Type ==  "exact" ) )
		{
			tableRowHeight = ASCDocFileFormat::YAS( *oXmlRow.Height->Height * ( -1 ) );    
		}
		else
		{
			tableRowHeight = ASCDocFileFormat::YAS( *oXmlRow.Height->Height );
		}

		oPrls.push_back(ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTDyaRowHeight, (unsigned char*)(&tableRowHeight)));

		return oPrls;
	}

	const PrlList CFileTransformer::ConvertTableRowCellsProperties(const OOX::Logic::Table& oXmlTable, int nY, int nCY, const vector<OOX::Logic::TableCell>& arrXmlCells, const OOX::Styles::Style& oStyle) 
	{
		PrlList arCellsPrls;

		DOCXDOCUTILS::CTblFill oTblFill (oXmlTable, oStyle);

		int nCX = arrXmlCells.size();

		for (int nX = 0; nX < nCX; ++nX)
		{
			const OOX::Logic::TableCell& oXmlCell	=	arrXmlCells[nX];

			oTblFill.AddCell (nX, nY, nCX-1, nCY-1, oXmlCell.Properties->Shading);

			ASCDocFileFormat::TableCellWidthOperand tableCellWidthOperand( ASCDocFileFormat::ItcFirstLim( nX, ( nX + 1 ) ), ASCDocFileFormat::FtsWWidth_TablePart( tableCellWidthMap[*oXmlCell.Properties->Width->Type], (unsigned short)(*oXmlCell.Properties->Width->Width) ) );
			arCellsPrls.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellWidth, tableCellWidthOperand ) );

			if(oXmlCell.Properties->CellMar.is_init())
			{
				const OOX::Logic::TCMar& oTCMar	= oXmlCell.Properties->CellMar;
				if (oTCMar.Top.is_init())
					arCellsPrls.push_back (DOCXDOCUTILS::GetOpTop(oTCMar, nCX));

				if (oTCMar.Left.is_init())
					arCellsPrls.push_back (DOCXDOCUTILS::GetOpLeft(oTCMar, nCX));

				if (oTCMar.Right.is_init())
					arCellsPrls.push_back (DOCXDOCUTILS::GetOpRight(oTCMar, nCX));

				if (oTCMar.Bottom.is_init())
					arCellsPrls.push_back (DOCXDOCUTILS::GetOpBottom(oTCMar, nCX));
			}

			if ( oXmlCell.Properties->VAlign.is_init() )
			{
				ASCDocFileFormat::Constants::VerticalAlign verticalAlign = ASCDocFileFormat::Constants::vaTop;

				if ( *oXmlCell.Properties->VAlign == "top" )
				{
					verticalAlign = ASCDocFileFormat::Constants::vaTop;
				}
				else if ( *oXmlCell.Properties->VAlign == "center" )
				{
					verticalAlign = ASCDocFileFormat::Constants::vaCenter;
				}
				else if ( *oXmlCell.Properties->VAlign == "bottom" )
				{
					verticalAlign = ASCDocFileFormat::Constants::vaBottom;
				}

				ASCDocFileFormat::CellRangeVertAlign cellRangeVertAlign( ASCDocFileFormat::ItcFirstLim( nX, ( nX + 1 ) ), verticalAlign );

				arCellsPrls.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTVertAlign, cellRangeVertAlign ) );
			}
		}

		if (oTblFill.GetShd22().size())
		{
			ASCDocFileFormat::DefTableShdOperand defOperand(oTblFill.GetShd22());

			arCellsPrls.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShd,	defOperand));
			arCellsPrls.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShdRaw, defOperand));
		}

		if (oTblFill.GetShd44().size())
		{
			ASCDocFileFormat::DefTableShdOperand defOperand(oTblFill.GetShd44());

			arCellsPrls.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShd,	defOperand));
			arCellsPrls.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShdRaw, defOperand));
		}

		if (oTblFill.GetShd63().size())
		{
			ASCDocFileFormat::DefTableShdOperand defOperand(oTblFill.GetShd63());

			arCellsPrls.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShd,	defOperand));
			arCellsPrls.push_back(ASCDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShdRaw, defOperand));
		}

		return arCellsPrls;
	}
}

namespace DOCXTODOC	//	FONTS
{
	void CFileTransformer::AddInternalFont(const std::wstring& strFontName)
	{	
		// шрифт может не содержаться в таблице шрифтов, а определен прямо по ходу текста

		std::map<std::wstring, short>::const_iterator findResult = m_mapFontTableMap.find(strFontName);  
		if (findResult == m_mapFontTableMap.end())
		{
			m_mapFontTableMap.insert(pair<wstring, short>(strFontName, m_mapFontTableMap.size()));

			m_arrInternalFonts.push_back(strFontName);
		}
	}

	void CFileTransformer::ConvertFontTable(const OOX::FontTable& oFontTable)
	{
		vector<ASCDocFileFormat::FFN> ffns;

		for (size_t i = 0; i < oFontTable.m_fonts.size(); ++i)
		{
			const OOX::FontTable::Font& oFont = oFontTable.m_fonts[i];

			ASCDocFileFormat::FFID ffid(DOCXDOCUTILS::FontPitchFromString(oFont.m_pitch.get()), true, DOCXDOCUTILS::FontFamilyFromString(oFont.m_family.get()));

			//!!!TODO!!!
			short wWeight = 400;

			FONTSIGNATURE fs;

			fs.fsCsb[0] = HexString2IntW(oFont.m_csb0.get());
			fs.fsCsb[1] = HexString2IntW(oFont.m_csb1.get());
			fs.fsUsb[0] = HexString2IntW(oFont.m_usb0.get());
			fs.fsUsb[1] = HexString2IntW(oFont.m_usb1.get());
			fs.fsUsb[2] = HexString2IntW(oFont.m_usb2.get());
			fs.fsUsb[3] = HexString2IntW(oFont.m_usb3.get());

			ASCDocFileFormat::FFN ffn (ffid, wWeight, 
				DOCXDOCUTILS::FontCharsetFromString(oFont.m_charset.get()),
				ConvertPanose(oFont.m_panose1.get()), fs, oFont.m_name.get());

			ffns.push_back(ffn);
		}

		for (size_t i = 0; i < m_arrInternalFonts.size(); ++i)
		{
			ASCDocFileFormat::FFID ffid (DOCXDOCUTILS::FontPitchFromString(std::wstring(L"")), true,	DOCXDOCUTILS::FontFamilyFromString(std::wstring(L"auto")));

			//!!!TODO!!!
			short wWeight = 400;

			FONTSIGNATURE fs;

			fs.fsCsb[0] = 0;
			fs.fsCsb[1] = 0;
			fs.fsUsb[0] = 0;
			fs.fsUsb[1] = 0;
			fs.fsUsb[2] = 0;
			fs.fsUsb[3] = 0;

			ASCDocFileFormat::FFN ffn (ffid, wWeight, 
				DOCXDOCUTILS::FontCharsetFromString (std::wstring(L"")),
				ConvertPanose(std::wstring(L"")), fs, m_arrInternalFonts[i]);

			ffns.push_back(ffn);
		}

		m_pDocFile->SetFontTable(ASCDocFileFormat::STTB<ASCDocFileFormat::FFN>(false, &ffns));
	}

	ASCDocFileFormat::PANOSE CFileTransformer::ConvertPanose(const std::wstring& strPanose)
	{
		if (20 == strPanose.size())
		{
			unsigned char bFamilyType		=	HexString2IntW(std::wstring((strPanose.begin() ),		( strPanose.begin() + 2 ) ) );
			unsigned char bSerifStyle		=	HexString2IntW(std::wstring((strPanose.begin() + 2 ),	( strPanose.begin() + 4 ) ) );
			unsigned char bWeight			=	HexString2IntW(std::wstring((strPanose.begin() + 4 ),	( strPanose.begin() + 6 ) ) );
			unsigned char bProportion		=	HexString2IntW(std::wstring((strPanose.begin() + 6 ),	( strPanose.begin() + 8 ) ) );
			unsigned char bContrast			=	HexString2IntW(std::wstring((strPanose.begin() + 8 ),	( strPanose.begin() + 10 ) ) );
			unsigned char bStrokeVariation	=	HexString2IntW(std::wstring((strPanose.begin() + 10 ),	( strPanose.begin() + 12 ) ) );
			unsigned char bArmStyle			=	HexString2IntW(std::wstring((strPanose.begin() + 12 ),	( strPanose.begin() + 14 ) ) );
			unsigned char bLetterform		=	HexString2IntW(std::wstring((strPanose.begin() + 14 ),	( strPanose.begin() + 16 ) ) );
			unsigned char bMidline			=	HexString2IntW(std::wstring((strPanose.begin() + 16 ),	( strPanose.begin() + 18 ) ) );
			unsigned char bHeight			=	HexString2IntW(std::wstring((strPanose.begin() + 18 ),	( strPanose.end() ) ) );

			return ASCDocFileFormat::PANOSE (bFamilyType, bSerifStyle, bWeight, bProportion, bContrast,	bStrokeVariation, bArmStyle, bLetterform, bMidline, bHeight);
		}

		return ASCDocFileFormat::PANOSE();
	}
}

namespace DOCXTODOC // run rule transform
{
	template<class T> ASCDocFileFormat::Run CFileTransformer::ConvertRun(const OOX::Logic::Run& oXml, const PrlList& styleDocRunProperties, std::wstring& strRunType) 
	{
		ASCDocFileFormat::Run oBinRun;
		for (vector<OOX::Logic::RunItem>::const_iterator iter = oXml.Items->begin(); iter != oXml.Items->end(); ++iter)
		{
			if (iter->is<OOX::Logic::Break>())
				BreakXmlRunTransform<T>(iter->as<OOX::Logic::Break>(),oBinRun);
			else if (iter->is<OOX::Logic::Tab>())
				TabXmlRunTransform<T>(iter->as<OOX::Logic::Tab>(),oBinRun);
			else if (iter->is<OOX::Logic::FootnoteReference>())
				FootnoteReferenceXmlRunTransform<T>(iter->as<OOX::Logic::FootnoteReference>(),oBinRun);
			else if (iter->is<OOX::Logic::FootnoteRef>())
				FootnoteRefXmlRunTransform<T>(iter->as<OOX::Logic::FootnoteRef>(),oBinRun);
			else if (iter->is<OOX::Logic::EndnoteReference>())
				EndnoteReferenceXmlRunTransform<T>(iter->as<OOX::Logic::EndnoteReference>(),oBinRun);
			else if (iter->is<OOX::Logic::EndnoteRef>())
				EndnoteRefXmlRunTransform<T>(iter->as<OOX::Logic::EndnoteRef>(),oBinRun);
			else if (iter->is<OOX::Logic::Text>())
				TextXmlRunTransform<T>(iter->as<OOX::Logic::Text>(),oBinRun);
			else if (iter->is<OOX::Logic::FldChar>())
				FldCharXmlRunTransform <T>(iter->as<OOX::Logic::FldChar>(),oBinRun,strRunType);
			else if (iter->is<OOX::Logic::InstrText>())
				InstrTextXmlRunTransform <T>(iter->as<OOX::Logic::InstrText>(),oBinRun,strRunType);
			else if (iter->is<OOX::Logic::Drawing>())
				DrawingXmlRunTransform<T>(iter->as<OOX::Logic::Drawing>(),oBinRun);
			else if (iter->is<OOX::Logic::Pict>())
				PictXmlRunTransform<T>(iter->as<OOX::Logic::Pict>(),oBinRun);
			else if (iter->is<OOX::Logic::Symbol>())
				SymbolXmlRunTransform<T>(iter->as<OOX::Logic::Symbol>(),oBinRun);
			else if (iter->is<OOX::Logic::DelText>())
				DelTextXmlRunTransform<T>(iter->as<OOX::Logic::DelText>(),oBinRun);
			else if (iter->is<OOX::Logic::NullRun>())
			{
				/// TODO 
			}
		}

		oBinRun.AddProperties(ConvertRunProperties(m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().Default->RunProperty));

		if (oXml.Property.is_init())
		{
			if (oXml.Property->RStyle.is_init())
			{
				//!!!TODO!!!  
				//short istd = m_mapStyleSheetMap[*oXml.Property->RStyle];
				//oBinRun.AddProperty( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCIstd, (unsigned char*)&istd ) );

				PrlList styleDocRunProperties = GetRunPropertiesFromStyleHierarchy(*oXml.Property->RStyle);
				oBinRun.AddOrReplaceProperties( styleDocRunProperties );
			}
			else
			{
				const OOX::Styles::Style defaultStyle = m_docxInputFile.find<OOX::Document>().find<OOX::Styles>().GetDefaultStyle("character");
				oBinRun.AddOrReplaceProperties (GetRunPropertiesFromStyleHierarchy(*defaultStyle.StyleId));
			}
		}

		oBinRun.AddOrReplaceProperties(styleDocRunProperties);

		if (oXml.Property.is_init())
		{
			oBinRun.AddOrReplaceProperties(ConvertRunProperties(*oXml.Property));
		}

		return oBinRun;
	}

	//
	template<class T> bool CFileTransformer::FldCharXmlRunTransform(const OOX::Logic::FldChar& oXml, ASCDocFileFormat::Run& oBinRun, std::wstring& strRunType)
	{
		std::wstring fldCharType	=	FormatUtils::UTF8Decode (*oXml.FldCharType);		
		if (fldCharType.empty())
			return FALSE;

		ASCDocFileFormat::FldChar oFldChar(fldCharType);

		strRunType					=	fldCharType;

		if (oFldChar.CharType() == ASCDocFileFormat::FldChar::FldCharSeparate)
		{
			oFldChar.SetFieldCharacterProperties((unsigned char)ASCDocFileFormat::grffldEnd(true, true, true, true, true, true, true, true));
			m_bHaveSeparateFldChar	=	true;
		}

		if (oFldChar.CharType() == ASCDocFileFormat::FldChar::FldCharEnd)
		{
			oFldChar.SetFieldCharacterProperties((unsigned char)ASCDocFileFormat::grffldEnd(false, false, true, true, false, false, false, m_bHaveSeparateFldChar));
			m_bHaveSeparateFldChar	=	false;
		}

		oBinRun.AddRunItem (oFldChar);

		return TRUE;
	}

	template<class T> bool CFileTransformer::TextXmlRunTransform(const OOX::Logic::Text& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		ASCDocFileFormat::Text oText(FormatUtils::UTF8Decode(oXml.toTxt()).c_str());
		oBinRun.AddRunItem (oText);

		return TRUE;
	}

	template<class T> bool CFileTransformer::InstrTextXmlRunTransform(const OOX::Logic::InstrText& oXml, ASCDocFileFormat::Run& oBinRun, std::wstring& strRunType)
	{
		std::wstring strText	=	FormatUtils::UTF8Decode(oXml.Text.get_value_or_default());

		std::wstring Source;
		std::wstring FieldCode	=	DOCXDOCUTILS::GetInstrText_FieldCode(strText, Source);
		if (FieldCode.empty())
			return FALSE;

		strRunType				=	FieldCode;

		oBinRun.AddRunItem(ASCDocFileFormat::Text(strText));

		return TRUE;
	}

	template<class T> bool CFileTransformer::DrawingXmlRunTransform(const OOX::Logic::Drawing& oXml, ASCDocFileFormat::Run& oBinRun) 
	{
		OOX::Image* pImage				=	GetImageDOCX <T, OOX::Logic::Drawing>(&oXml);
		if (pImage)
		{
			if (oXml.haveAnchor)
			{
				return	m_oOArtBuilder.BuildImageRun (*pImage, oXml, oBinRun);
			}

			std::wstring sFileName		=	pImage->GetPath();
			if (sFileName.length())
			{
				Unit<int, Dx> twWidth	=	*oXml.Inline->Extent->Size->Width;
				Unit<int, Dx> twHeight	=	*oXml.Inline->Extent->Size->Height;

				ASCDocFileFormat::Picture oPicture (sFileName, twWidth, twHeight);
				if (oPicture.IsValid())
				{
					oBinRun.AddRunItem (oPicture);
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	template<class T> bool CFileTransformer::PictXmlRunTransform(const OOX::Logic::Pict& oXml, ASCDocFileFormat::Run& oBinRun) 
	{		
		if (oXml.group.is_init())
			return PictGroupXmlTransform<T>(oXml, oBinRun);

		// фигура имеет заливку текстурой

		if (CreateTextureFillShape<T>(oXml, oBinRun))
			return TRUE;

		CShapeRun oShapeRun;
		if (m_oOArtBuilder.BuildShapeRun(oXml, oShapeRun))
		{	
			BuildContentTbRef (oXml);

			if (oShapeRun.IsInline())
			{
				m_oInlineShape.RemoveAllRuns();

				// TODO : сделать нормальное сорхранение Inline фигур (разобраться с недокументированым форматом сохранения объекта)

				//m_oInlineShape.SetShape (oShapeRun);
				//ASCDocFileFormat::Picture oPicture (std::wstring(L""), 0, 0, 0, 0);		//	 fake image
				//m_oInlineShape.SetPicture (oPicture);
				//m_oInlineShape.AddRun (ASCDocFileFormat::Run(ASCDocFileFormat::Picture (std::wstring(L""), 0, 0, 0, 0)));//	 fake image
				//m_oInlineShape.AddRun (ASCDocFileFormat::Run(oShapeRun));//	 fake image

				//m_oInlineShape.AddRun (ASCDocFileFormat::Run(ASCDocFileFormat::CInlineShape (oShapeRun) ));	//	 fake image
				//m_oInlineShape.AddRun (ASCDocFileFormat::Run(oShapeRun));
				//m_oInlineShape.AddRun (ASCDocFileFormat::Run(ASCDocFileFormat::CShapeRun()));

				//m_bIsInlineShape	=	TRUE;

				// TODO : временное решение для Inline Shape - объектов (бинарный парсер доделать)

				oBinRun.AddRunItem (ASCDocFileFormat::CInlineShape (oShapeRun) );						
				m_bIsInlineShape	=	FALSE;

				return TRUE;
			}
			else
			{
				oBinRun.AddRunItem(oShapeRun);

				m_bIsInlineShape	=	FALSE;

				return TRUE;
			}
		}
		else
		{
			m_bIsInlineShape	=	FALSE;

			if (CreateImage<T>(oXml, oBinRun))
			{
				return TRUE;
			}

			return FALSE;
		}

		return TRUE;
	}
	template<class T> bool CFileTransformer::DelTextXmlRunTransform(const OOX::Logic::DelText& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		ASCDocFileFormat::Text oText( FormatUtils::UTF8Decode( oXml.toTxt() ).c_str() );
		oBinRun.AddRunItem(oText);

		return TRUE;
	}
	template<class T> bool CFileTransformer::SymbolXmlRunTransform(const OOX::Logic::Symbol& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		if (oXml.Font.is_init() && oXml.Char.is_init())
		{
			ASCDocFileFormat::Text oSymbol ((wchar_t)TextMark::Symbol);
			oBinRun.AddRunItem(oSymbol);

			ASCDocFileFormat::CSymbolOperand oSymbolOp (m_mapFontTableMap[string2wstring__(*oXml.Font)], HexString2Int(*oXml.Char));
			oBinRun.AddProperty(ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCSymbol, oSymbolOp));

			return TRUE;
		}

		return FALSE;
	}

	template<class T> bool CFileTransformer::BreakXmlRunTransform(const OOX::Logic::Break& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		/// TODO

		ASCDocFileFormat::Text oText;

		if ( oXml.Type.is_init() )
		{
			if ( *oXml.Type == std::string( "page" ) )
			{
				oText.SetText( &TextMark::PageBreakOrSectionMark );
			}
			else if ( *oXml.Type == std::string( "column" ) )
			{
				oText.SetText( &TextMark::ColumnBreak );
			}
			else if ( *oXml.Type == std::string( "textWrapping" ) )
			{
				oText.SetText( &TextMark::HardLineBreak );
			}
		}
		else
		{
			oText.SetText( &TextMark::HardLineBreak );
		}

		oBinRun.AddRunItem(oText);

		return TRUE;
	}

	template<class T> bool CFileTransformer::TabXmlRunTransform(const OOX::Logic::Tab& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		/// TODO

		ASCDocFileFormat::Text oText;
		oText.SetText(&TextMark::Tab);

		oBinRun.AddRunItem(oText);

		return TRUE;		
	}

	template<class T> bool CFileTransformer::FootnoteReferenceXmlRunTransform(const OOX::Logic::FootnoteReference& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		if (m_docxInputFile.find<OOX::Document>().exist<OOX::FootNote>())
		{
			static short footnoteIndex = 1;

			const OOX::FootNote::Note& footNote = m_docxInputFile.find<OOX::Document>().find<OOX::FootNote>().find(oXml);

			ASCDocFileFormat::FootnoteReference docFootnoteReference( footnoteIndex );
			ASCDocFileFormat::Footnote docFootnote( footnoteIndex++ );

			size_t count = (*footNote.Items).size();
			const std::vector<OOX::Logic::TextItem>& items = (*footNote.Items);

			for (size_t i = 0; i < count; ++i)	
			{			
				if (items[i].is<OOX::Logic::Paragraph>())
				{
					const OOX::Logic::Paragraph& docxParagraph	=	items[i].as<OOX::Logic::Paragraph>();

					ASCDocFileFormat::Paragraph oParagraph		=	ConvertParagraph<OOX::FootNote>(docxParagraph);
					docFootnote.AddTextItem (oParagraph);
				}
				else if (items[i].is<OOX::Logic::Table>())
				{
					ASCDocFileFormat::Table oTable				=	CreateTable<OOX::FootNote>(items[i].as<OOX::Logic::Table>());
					docFootnote.AddTextItem (oTable);
				}
			}

			oBinRun.AddRunItem (docFootnoteReference);
			m_pDocFile->AddFootnote(docFootnote);

			return TRUE;		
		}

		return FALSE;
	}

	template<class T> bool CFileTransformer::FootnoteRefXmlRunTransform(const OOX::Logic::FootnoteRef& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		if (m_docxInputFile.find<OOX::Document>().exist<OOX::FootNote>())
		{
			static short footnoteIndex = 1;

			oBinRun.AddRunItem(ASCDocFileFormat::FootnoteRef(footnoteIndex++));

			return TRUE;		
		}

		return FALSE;
	}

	template<class T> bool CFileTransformer::EndnoteReferenceXmlRunTransform(const OOX::Logic::EndnoteReference& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		if (m_docxInputFile.find<OOX::Document>().exist<OOX::EndNote>())
		{
			static short endnoteIndex = 1;

			const OOX::EndNote::Note& endNote = m_docxInputFile.find<OOX::Document>().find<OOX::EndNote>().find(oXml);

			ASCDocFileFormat::EndnoteReference docEndnoteReference(endnoteIndex);
			ASCDocFileFormat::Endnote docEndnote(endnoteIndex++);

			size_t count = (*endNote.Items).size();
			const std::vector<OOX::Logic::TextItem>& items = (*endNote.Items);

			for (size_t j = 0; j < count; ++j)	
			{
				if (items[j].is<OOX::Logic::Paragraph>())
				{							
					const OOX::Logic::Paragraph& docxParagraph	=	items[j].as<OOX::Logic::Paragraph>();

					ASCDocFileFormat::Paragraph oParagraph		=	ConvertParagraph<OOX::EndNote>(docxParagraph);
					docEndnote.AddTextItem(oParagraph);
				}
				else if (items[j].is<OOX::Logic::Table>())
				{
					ASCDocFileFormat::Table oTable				=	CreateTable<OOX::EndNote>(items[j].as<OOX::Logic::Table>());
					docEndnote.AddTextItem(oTable);
				}
			}

			oBinRun.AddRunItem( docEndnoteReference );
			m_pDocFile->AddEndnote( docEndnote );

			return TRUE;		
		}

		return FALSE;
	}

	template<class T> bool CFileTransformer::EndnoteRefXmlRunTransform(const OOX::Logic::EndnoteRef& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		if (m_docxInputFile.find<OOX::Document>().exist<OOX::EndNote>())
		{
			static short endnoteIndex = 1;

			oBinRun.AddRunItem(ASCDocFileFormat::EndnoteRef(endnoteIndex++));

			return TRUE;		
		}

		return FALSE;
	}
	template<class T> bool CFileTransformer::PictGroupXmlTransform(const OOX::Logic::Pict& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		ASCDocFileFormat::COArtStorage* pStorage	=	ASCDocFileFormat::COArtStorage::Instance();
		if (NULL == pStorage)
			return FALSE;

		ASCDocFileFormat::COArtGroup* pShapes	=	CreateGroup<T>(oXml.group);
		if (NULL == pShapes)
			return FALSE;

		oBinRun.AddRunItem(m_oOArtBuilder.BuildGroupRun(oXml.group,pShapes));
		return TRUE;
	}
}

namespace DOCXTODOC
{
	template<class T, class TXmlImage> OOX::Image* CFileTransformer::GetImageDOCX(const TXmlImage* pXmlImage)
	{	
		if (pXmlImage)
		{
			bool bFindOK	=	FALSE;
			OOX::RId nID;

			if (typeid(TXmlImage) == typeid(OOX::Logic::Drawing))
			{
				OOX::Logic::Drawing* pPicture		=	(OOX::Logic::Drawing*)(pXmlImage);
				if (pPicture)
				{
					if (pPicture->Inline->Graphic->Pic->rId.is_init())
					{
						nID			=	pPicture->Inline->Graphic->Pic->rId;
						bFindOK		=	TRUE;
					}
				}
			}

			if (typeid(TXmlImage) == typeid(OOX::Logic::Pict))
			{
				OOX::Logic::Pict* pPicture	=	(OOX::Logic::Pict*)(pXmlImage);
				if (pPicture)
				{
					if(pPicture->shape.is_init())
					{
						if (pPicture->shape->imageData.is_init())
						{
							nID				=	pPicture->shape->imageData->rId;
							bFindOK			=	TRUE;							
						}
					}
				}
			}

			if (typeid(TXmlImage) == typeid(OOX::Logic::Shape))
			{
				OOX::Logic::Shape* pShape	=	(OOX::Logic::Shape*)(pXmlImage);
				if (pShape)
				{
					if (pShape->imageData.is_init())
					{
						nID				=	pShape->imageData->rId;
						bFindOK			=	TRUE;							
					}
				}
			}

			if (bFindOK)
			{
				if (typeid(T) != typeid(OOX::Document))
				{
					if (m_docxInputFile.find<OOX::Document>().find<T>().exist(nID))
						return dynamic_cast<OOX::Image*>(m_docxInputFile.find<OOX::Document>().find<T>()[nID].operator->());
				}
				else
				{
					if (m_docxInputFile.find<OOX::Document>().exist(nID))
						return dynamic_cast<OOX::Image*>(m_docxInputFile.find<OOX::Document>()[nID].operator->());
				}
			}
		}

		return NULL;
	}

	template<class T> OOX::Image* CFileTransformer::GetImageWithId(const OOX::RId& nID)
	{
		if (typeid(T) != typeid(OOX::Document))
		{
			if (m_docxInputFile.find<OOX::Document>().find<T>().exist(nID))
				return dynamic_cast<OOX::Image*>(m_docxInputFile.find<OOX::Document>().find<T>()[nID].operator->());
		}

		if (m_docxInputFile.find<OOX::Document>().exist(nID))
			return dynamic_cast<OOX::Image*>(m_docxInputFile.find<OOX::Document>()[nID].operator->());

		return NULL;
	}

	template<class T> bool CFileTransformer::CreateImage(const OOX::Logic::Pict& oXml, ASCDocFileFormat::Run& oBinRun) 
	{
		OOX::Image* pImage				=	GetImageDOCX <T, OOX::Logic::Pict>(&oXml);

		if (pImage)
		{
			std::wstring sFileName		=	pImage->GetPath();
			if (sFileName.length())
			{
				if (oXml.shape.is_init())
				{
					if (oXml.shape->imageData.is_init())
					{
						return m_oOArtBuilder.BuildImageRun(oXml.shape, std::wstring(pImage->GetPath()), oBinRun);
					}
				}

				Unit<int, Dx> twWidth	=	*oXml.shape->style->Size->Width;
				Unit<int, Dx> twHeight	=	*oXml.shape->style->Size->Height;

				ASCDocFileFormat::Picture oPicture (sFileName, twWidth, twHeight);
				if (oPicture.IsValid())
				{
					oBinRun.AddRunItem (ASCDocFileFormat::Picture());

					return TRUE;
				}
			}
		}

		return FALSE;
	}

	template<class T> ASCDocFileFormat::COArtGroup* CFileTransformer::CreateGroup(const OOX::Logic::Group& oXmlGroup)
	{
		ASCDocFileFormat::COArtStorage* pStorage	=	ASCDocFileFormat::COArtStorage::Instance();
		if (NULL == pStorage)
			return FALSE;

		ASCDocFileFormat::COArtGroup* pBinGroup		=	new ASCDocFileFormat::COArtGroup(pStorage->GenID(m_oOArtBuilder.Location()));
		if (NULL == pBinGroup)
			return FALSE;

		pBinGroup->SetCoord(DOCX::CFPoint(oXmlGroup.coordorigin, 0.0, 0.0), DOCX::CFPoint(oXmlGroup.coordsize, 0.0, 0.0));
		pBinGroup->SetBounds(oXmlGroup.style);
		pBinGroup->SetupFromStyleXml(oXmlGroup.style);

		for (size_t i = 0; i < oXmlGroup.items->size(); ++i)
		{
			const OOX::Logic::GroupItem& oXmlItem	=	oXmlGroup.items->operator[](i);

			if (oXmlItem.is<OOX::Logic::Oval>())
			{
				const OOX::Logic::Oval& shape = oXmlItem.as<OOX::Logic::Oval>();

				if (shape.fillstyle->Id.is_init())
				{
					OOX::Image* image = GetImageWithId <T>(OOX::RId(string2wstring__(shape.fillstyle->Id)));
					if (image)
					{
						m_oOArtBuilder.SetTextureFill(image->GetPath());
					}
				}

				pBinGroup->Append (m_oOArtBuilder.BuildOArtGroupShape<OOX::Logic::Oval>(shape, OfficeArt::Enumerations::msosptEllipse));
			}

			if (oXmlItem.is<OOX::Logic::Roundrect>())
			{
				const OOX::Logic::Roundrect& shape = oXmlItem.as<OOX::Logic::Roundrect>();

				if (shape.fillstyle->Id.is_init())
				{
					OOX::Image* image = GetImageWithId <T>(OOX::RId(string2wstring__(shape.fillstyle->Id)));
					if (image)
					{
						m_oOArtBuilder.SetTextureFill(image->GetPath());
					}
				}

				pBinGroup->Append (m_oOArtBuilder.BuildOArtGroupShape<OOX::Logic::Roundrect>(oXmlItem.as<OOX::Logic::Roundrect>(), OfficeArt::Enumerations::msosptRoundRectangle));
			}

			if (oXmlItem.is<OOX::Logic::Rect>())
			{
				const OOX::Logic::Rect& shape = oXmlItem.as<OOX::Logic::Rect>();

				if (shape.fillstyle->Id.is_init())
				{
					OOX::Image* image = GetImageWithId <T>(OOX::RId(string2wstring__(shape.fillstyle->Id)));
					if (image)
					{
						m_oOArtBuilder.SetTextureFill(image->GetPath());
					}
				}

				pBinGroup->Append (m_oOArtBuilder.BuildOArtGroupShape<OOX::Logic::Rect>(oXmlItem.as<OOX::Logic::Rect>(), OfficeArt::Enumerations::msosptRectangle));
			}

			if (oXmlItem.is<OOX::Logic::Line>())
			{
				pBinGroup->Append (m_oOArtBuilder.BuildOArtGroupShape<OOX::Logic::Line>(oXmlItem.as<OOX::Logic::Line>(), OfficeArt::Enumerations::msosptLine));
			}

			if (oXmlItem.is<OOX::Logic::Shape>())
			{
				const OOX::Logic::Shape& shape = oXmlItem.as<OOX::Logic::Shape>();				
				if (shape.imageData.is_init())
				{
					OOX::Image* image = GetImageDOCX <T, OOX::Logic::Shape>(&shape);
					if (image)
					{
						//pBinGroup->Append (m_oOArtBuilder.BuildOArtImage(shape, std::wstring(image->filename().GetPath())));
						pBinGroup->Append (m_oOArtBuilder.BuildOArtImage(shape, std::wstring(image->GetPath())));
					}
				}
				else
				{
					if (shape.fillstyle->Id.is_init())
					{
						OOX::Image* image = GetImageWithId <T>(OOX::RId(string2wstring__(shape.fillstyle->Id)));
						if (image)
						{
							m_oOArtBuilder.SetTextureFill(std::wstring(image->GetPath()));
						}
					}

					pBinGroup->Append (m_oOArtBuilder.BuildOArtShape(shape, oXmlGroup));
				}
			}

			if (oXmlItem.is<OOX::Logic::Group>())
			{
				const OOX::Logic::Group& oXmlInGroup = oXmlItem.as<OOX::Logic::Group>();
				pBinGroup->Append (CreateGroup<T>(oXmlInGroup));
			}
		}

		if (pBinGroup->IsValid())
			return pBinGroup;

		RELEASEOBJECT(pBinGroup);
		return NULL;
	}

	template<class T> bool CFileTransformer::CreateTextureFillShape(const OOX::Logic::Pict& oXml, ASCDocFileFormat::Run& oBinRun)
	{
		// фигура имеет заливку текстурой

		// RECT

		if (oXml.rect.is_init())
		{
			if (oXml.rect->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(string2wstring__(oXml.rect->fillstyle->Id)));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Rect>(oXml.rect, std::wstring(pImage->GetPath()), OfficeArt::Enumerations::msosptRectangle, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		// OVAL

		if (oXml.oval.is_init())
		{
			if (oXml.oval->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(string2wstring__(oXml.oval->fillstyle->Id)));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Oval>(oXml.oval, std::wstring(pImage->GetPath()), OfficeArt::Enumerations::msosptEllipse, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		// ROUNDRECT

		if (oXml.roundrect.is_init())
		{
			if (oXml.roundrect->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(string2wstring__(oXml.roundrect->fillstyle->Id)));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Roundrect>(oXml.roundrect, std::wstring(pImage->GetPath()), OfficeArt::Enumerations::msosptRoundRectangle, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		// SHAPE CUSTOM

		if (oXml.shape.is_init())
		{
			if (oXml.shape->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(string2wstring__(oXml.shape->fillstyle->Id)));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Shape>(oXml.shape, std::wstring(pImage->GetPath()), 0, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		return FALSE;
	}

}

namespace DOCXTODOC
{
	bool CFileTransformer::BuildContentTbRef(const OOX::Logic::Pict& oXml)
	{
		CTextBoxRef* pTbRef = m_oOArtBuilder.LastTbRef ();
		if (pTbRef)
		{
			std::vector<ASCDocFileFormat::TextItem> oTextItems;

			if (oXml.rect.is_init())
			{
				if (oXml.rect->textbox.is_init())
					TransformTb(oXml.rect->textbox->content->items, oTextItems);
			}

			if (oXml.oval.is_init())
			{
				if (oXml.oval->textbox.is_init())
					TransformTb(oXml.oval->textbox->content->items, oTextItems);
			}

			if (oXml.roundrect.is_init())
			{
				if (oXml.roundrect->textbox.is_init())
					TransformTb(oXml.roundrect->textbox->content->items, oTextItems);
			}

			if (oXml.line.is_init())
			{
				if (oXml.line->textbox.is_init())
					TransformTb(oXml.line->textbox->content->items, oTextItems);
			}

			if (oXml.shape.is_init())
			{
				if (oXml.shape->textbox.is_init())
					TransformTb(oXml.shape->textbox->content->items, oTextItems);
			}

			if (oTextItems.size())
			{
				pTbRef->AppendTbItems(oTextItems);
				return TRUE;
			}
		}

		return FALSE;
	}

	bool CFileTransformer::TransformTb(const std::vector<OOX::Logic::TextItem>& oXmlItems, std::vector<ASCDocFileFormat::TextItem>& oTextItems)
	{
		for (size_t i = 0; i < oXmlItems.size(); ++i)
		{
			const OOX::Logic::TextItem& oXmlItem	=	oXmlItems[i];

			if (oXmlItem.is<OOX::Logic::Paragraph>())
			{
				ASCDocFileFormat::Paragraph oBinPr	=	ConvertParagraph<OOX::Document>(oXmlItem.as<OOX::Logic::Paragraph>());
				oTextItems.push_back(TextItem(oBinPr));
			}

			if (oXmlItem.is<OOX::Logic::Table>())
			{
				ASCDocFileFormat::Table oBinTable	=	CreateTable<OOX::Document>(oXmlItem.as<OOX::Logic::Table>());
				oTextItems.push_back(TextItem(oBinTable));
			}

			if (oXmlItem.is<OOX::Logic::Sdt>())
			{
				//const OOX::Logic::SdtContent& content = oXmlItem.as<OOX::Logic::Sdt>().Content;
				//TransformTb(*content.Items, oTextItems);  
			}
		}

		return TRUE;
	}
}
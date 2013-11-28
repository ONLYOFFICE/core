#include "stdafx.h"

#include <AtlConv.h>

#include "ParagraphItem.h"
#include "DocxToDocUtils.h"
#include "ShapeBuilder.h"
#include "PropertyReader.h"

#include "FileConverter.h"

using namespace AVSDocFileFormat;

namespace DOCXTODOC
{
	CFileTransformer::CFileTransformer ()
	{
		m_pDOCFile				=	NULL;

		m_bHaveSeparateFldChar	=	false;
		m_bIsInlineShape		=	FALSE;

		m_bIsHaveRunPr			=	FALSE;

		m_pDOCFile				=	new AVSDocFileFormat::CDocFile();

		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "af-ZA" ), AVSDocFileFormat::LID( DocFileFormat::Afrikaans ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sq-AL" ), AVSDocFileFormat::LID( DocFileFormat::Albanian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "am-ET" ), AVSDocFileFormat::LID( DocFileFormat::Amharic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-DZ" ), AVSDocFileFormat::LID( DocFileFormat::ArabicAlgeria ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-BH" ), AVSDocFileFormat::LID( DocFileFormat::ArabicBahrain ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-EG" ), AVSDocFileFormat::LID( DocFileFormat::ArabicEgypt ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-IQ" ), AVSDocFileFormat::LID( DocFileFormat::ArabicIraq ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-JO" ), AVSDocFileFormat::LID( DocFileFormat::ArabicJordan ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-KW" ), AVSDocFileFormat::LID( DocFileFormat::ArabicKuwait ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-LB" ), AVSDocFileFormat::LID( DocFileFormat::ArabicLebanon ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-LY" ), AVSDocFileFormat::LID( DocFileFormat::ArabicLibya ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-MA" ), AVSDocFileFormat::LID( DocFileFormat::ArabicMorocco ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-OM" ), AVSDocFileFormat::LID( DocFileFormat::ArabicOman ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-QA" ), AVSDocFileFormat::LID( DocFileFormat::ArabicQatar ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-SA" ), AVSDocFileFormat::LID( DocFileFormat::ArabicSaudiArabia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-SY" ), AVSDocFileFormat::LID( DocFileFormat::ArabicSyria ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-TN" ), AVSDocFileFormat::LID( DocFileFormat::ArabicTunisia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-AE" ), AVSDocFileFormat::LID( DocFileFormat::ArabicUAE ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ar-YE" ), AVSDocFileFormat::LID( DocFileFormat::ArabicYemen ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "hy-AM" ), AVSDocFileFormat::LID( DocFileFormat::Armenian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "as-IN" ), AVSDocFileFormat::LID( DocFileFormat::Assamese ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "az-AZ-cyrl" ), AVSDocFileFormat::LID( DocFileFormat::AzeriCyrillic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "az-AZ-latn" ), AVSDocFileFormat::LID( DocFileFormat::AzeriLatin ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "eu-ES" ), AVSDocFileFormat::LID( DocFileFormat::Basque ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "be-BY" ), AVSDocFileFormat::LID( DocFileFormat::Belarusian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "bn-IN" ), AVSDocFileFormat::LID( DocFileFormat::Bengali ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "bn-BD" ), AVSDocFileFormat::LID( DocFileFormat::BengaliBangladesh ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "bg-BG" ), AVSDocFileFormat::LID( DocFileFormat::Bulgarian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "my-MM" ), AVSDocFileFormat::LID( DocFileFormat::Burmese ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ca-ES" ), AVSDocFileFormat::LID( DocFileFormat::Catalan ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "zh-HK" ), AVSDocFileFormat::LID( DocFileFormat::ChineseHongKong ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "zh-MO" ), AVSDocFileFormat::LID( DocFileFormat::ChineseMacao ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "zh-CN" ), AVSDocFileFormat::LID( DocFileFormat::ChinesePRC ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "zh-SG" ), AVSDocFileFormat::LID( DocFileFormat::ChineseSingapore ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "zh-TW" ), AVSDocFileFormat::LID( DocFileFormat::ChineseTaiwan ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "hr-HR" ), AVSDocFileFormat::LID( DocFileFormat::Croatian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "cs-CZ" ), AVSDocFileFormat::LID( DocFileFormat::Czech ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "da-DK" ), AVSDocFileFormat::LID( DocFileFormat::Danish ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "dv-MV" ), AVSDocFileFormat::LID( DocFileFormat::Divehi ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "nl-BE" ), AVSDocFileFormat::LID( DocFileFormat::DutchBelgium ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "nl-NL" ), AVSDocFileFormat::LID( DocFileFormat::DutchNetherlands ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-AU" ), AVSDocFileFormat::LID( DocFileFormat::EnglishAustralia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-BZ" ), AVSDocFileFormat::LID( DocFileFormat::EnglishBelize ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-CA" ), AVSDocFileFormat::LID( DocFileFormat::EnglishCanada ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-DO" ), AVSDocFileFormat::LID( DocFileFormat::EnglishCaribbean ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-HK" ), AVSDocFileFormat::LID( DocFileFormat::EnglishHongKong ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-IN" ), AVSDocFileFormat::LID( DocFileFormat::EnglishIndia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-ID" ), AVSDocFileFormat::LID( DocFileFormat::EnglishIndonesia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-IE" ), AVSDocFileFormat::LID( DocFileFormat::EnglishIreland ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-JM" ), AVSDocFileFormat::LID( DocFileFormat::EnglishJamaica ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-MY" ), AVSDocFileFormat::LID( DocFileFormat::EnglishMalaysia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-NZ" ), AVSDocFileFormat::LID( DocFileFormat::EnglishNewZealand ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-PH" ), AVSDocFileFormat::LID( DocFileFormat::EnglishPhilippines ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-SG" ), AVSDocFileFormat::LID( DocFileFormat::EnglishSingapore ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-ZA" ), AVSDocFileFormat::LID( DocFileFormat::EnglishSouthAfrica ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-TT" ), AVSDocFileFormat::LID( DocFileFormat::EnglishTrinidadAndTobago ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-UK" ), AVSDocFileFormat::LID( DocFileFormat::EnglishUK ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-US" ), AVSDocFileFormat::LID( DocFileFormat::EnglishUS ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "en-ZW" ), AVSDocFileFormat::LID( DocFileFormat::EnglishZimbabwe ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "et-EE" ), AVSDocFileFormat::LID( DocFileFormat::Estonian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fo-FO" ), AVSDocFileFormat::LID( DocFileFormat::Faeroese ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fi-FI" ), AVSDocFileFormat::LID( DocFileFormat::Finnish ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-BE" ), AVSDocFileFormat::LID( DocFileFormat::FrenchBelgium ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-CM" ), AVSDocFileFormat::LID( DocFileFormat::FrenchCameroon ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-CA" ), AVSDocFileFormat::LID( DocFileFormat::FrenchCanada ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-CD" ), AVSDocFileFormat::LID( DocFileFormat::FrenchCongoDRC ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-CI" ), AVSDocFileFormat::LID( DocFileFormat::FrenchCotedIvoire ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-FR" ), AVSDocFileFormat::LID( DocFileFormat::FrenchFrance ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-HT" ), AVSDocFileFormat::LID( DocFileFormat::FrenchHaiti ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-LU" ), AVSDocFileFormat::LID( DocFileFormat::FrenchLuxembourg ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-ML" ), AVSDocFileFormat::LID( DocFileFormat::FrenchMali ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-MC" ), AVSDocFileFormat::LID( DocFileFormat::FrenchMonaco ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-MA" ), AVSDocFileFormat::LID( DocFileFormat::FrenchMorocco ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-RE" ), AVSDocFileFormat::LID( DocFileFormat::FrenchReunion ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-SN" ), AVSDocFileFormat::LID( DocFileFormat::FrenchSenegal ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-CH" ), AVSDocFileFormat::LID( DocFileFormat::FrenchSwitzerland ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fr-DO" ), AVSDocFileFormat::LID( DocFileFormat::FrenchWestIndies ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "fy-NL" ), AVSDocFileFormat::LID( DocFileFormat::FrisianNetherlands ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "mk-MK" ), AVSDocFileFormat::LID( DocFileFormat::FYROMacedonian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ga-IE" ), AVSDocFileFormat::LID( DocFileFormat::GaelicIreland ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "gd-UK" ), AVSDocFileFormat::LID( DocFileFormat::GaelicScotland ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "gl-ES" ), AVSDocFileFormat::LID( DocFileFormat::Galician ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ka-GE" ), AVSDocFileFormat::LID( DocFileFormat::Georgian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "de-AT" ), AVSDocFileFormat::LID( DocFileFormat::GermanAustria ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "de-DE" ), AVSDocFileFormat::LID( DocFileFormat::GermanGermany ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "de-LI" ), AVSDocFileFormat::LID( DocFileFormat::GermanLiechtenstein ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "de-LU" ), AVSDocFileFormat::LID( DocFileFormat::GermanLuxembourg ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "de-CH" ), AVSDocFileFormat::LID( DocFileFormat::GermanSwitzerland ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "el-GR" ), AVSDocFileFormat::LID( DocFileFormat::Greek ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "gn-BR" ), AVSDocFileFormat::LID( DocFileFormat::Guarani ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "gu-IN" ), AVSDocFileFormat::LID( DocFileFormat::Gujarati ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ha-NG" ), AVSDocFileFormat::LID( DocFileFormat::Hausa ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "he-IL" ), AVSDocFileFormat::LID( DocFileFormat::Hebrew ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "hi-IN" ), AVSDocFileFormat::LID( DocFileFormat::Hindi ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "hu-HU" ), AVSDocFileFormat::LID( DocFileFormat::Hungarian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "is-IS" ), AVSDocFileFormat::LID( DocFileFormat::Icelandic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "id-ID" ), AVSDocFileFormat::LID( DocFileFormat::Indonesian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "iu-CA" ), AVSDocFileFormat::LID( DocFileFormat::Inuktitut ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "it-IT" ), AVSDocFileFormat::LID( DocFileFormat::ItalianItaly ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "it-CH" ), AVSDocFileFormat::LID( DocFileFormat::ItalianSwitzerland ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ja-JP" ), AVSDocFileFormat::LID( DocFileFormat::Japanese ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "kn-ID" ), AVSDocFileFormat::LID( DocFileFormat::Kannada ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ks-ID" ), AVSDocFileFormat::LID( DocFileFormat::Kashmiri ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ks-PK" ), AVSDocFileFormat::LID( DocFileFormat::KashmiriArabic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "kk-KZ" ), AVSDocFileFormat::LID( DocFileFormat::Kazakh ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ko-KR" ), AVSDocFileFormat::LID( DocFileFormat::Korean ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ky-KG" ), AVSDocFileFormat::LID( DocFileFormat::Kyrgyz ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "lo-LA" ), AVSDocFileFormat::LID( DocFileFormat::Lao ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "la" ), AVSDocFileFormat::LID( DocFileFormat::Latin ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "lv-LV" ), AVSDocFileFormat::LID( DocFileFormat::Latvian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "lt-LT" ), AVSDocFileFormat::LID( DocFileFormat::Lithuanian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ms-MY" ), AVSDocFileFormat::LID( DocFileFormat::Malay ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ms-BN" ), AVSDocFileFormat::LID( DocFileFormat::MalayBruneiDarussalam ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ml-ID" ), AVSDocFileFormat::LID( DocFileFormat::Malayalam ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "mt-MT" ), AVSDocFileFormat::LID( DocFileFormat::Maltese ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "mi-NZ" ), AVSDocFileFormat::LID( DocFileFormat::Maori ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "mr-ID" ), AVSDocFileFormat::LID( DocFileFormat::Marathi ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "mn-MN" ), AVSDocFileFormat::LID( DocFileFormat::Mongolian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ne-NP" ), AVSDocFileFormat::LID( DocFileFormat::Nepali ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ne-ID" ), AVSDocFileFormat::LID( DocFileFormat::NepaliIndia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "nb-NO" ), AVSDocFileFormat::LID( DocFileFormat::NorwegianBokmal ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "nn-NO" ), AVSDocFileFormat::LID( DocFileFormat::NorwegianNynorsk ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "or-ID" ), AVSDocFileFormat::LID( DocFileFormat::Oriya ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ps-PK" ), AVSDocFileFormat::LID( DocFileFormat::Pashto ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "pl-PL" ), AVSDocFileFormat::LID( DocFileFormat::Polish ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "pt-BR" ), AVSDocFileFormat::LID( DocFileFormat::PortugueseBrazil ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "pt-PT" ), AVSDocFileFormat::LID( DocFileFormat::PortuguesePortugal ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "pa-ID" ), AVSDocFileFormat::LID( DocFileFormat::Punjabi ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "pa-PK" ), AVSDocFileFormat::LID( DocFileFormat::PunjabiPakistan ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "qu-BO" ), AVSDocFileFormat::LID( DocFileFormat::QuechuaBolivia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "qu-EC" ), AVSDocFileFormat::LID( DocFileFormat::QuechuaEcuador ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "qu-PE" ), AVSDocFileFormat::LID( DocFileFormat::QuechuaPeru ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "rm-CH" ), AVSDocFileFormat::LID( DocFileFormat::RhaetoRomanic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ro-MD" ), AVSDocFileFormat::LID( DocFileFormat::RomanianMoldova ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ro-RO" ), AVSDocFileFormat::LID( DocFileFormat::RomanianRomania ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ru-MD" ), AVSDocFileFormat::LID( DocFileFormat::RussianMoldova ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ru-RU" ), AVSDocFileFormat::LID( DocFileFormat::RussianRussia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "se-FI" ), AVSDocFileFormat::LID( DocFileFormat::SamiLappish ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sa-ID" ), AVSDocFileFormat::LID( DocFileFormat::Sanskrit ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sr-YU-cyrl" ), AVSDocFileFormat::LID( DocFileFormat::SerbianCyrillic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sr-YU-latn" ), AVSDocFileFormat::LID( DocFileFormat::SerbianLatin ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sd-PK" ), AVSDocFileFormat::LID( DocFileFormat::SindhiArabic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sd-ID" ), AVSDocFileFormat::LID( DocFileFormat::SindhiDevanagari ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "si-ID" ), AVSDocFileFormat::LID( DocFileFormat::Sinhalese ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sk-SK" ), AVSDocFileFormat::LID( DocFileFormat::Slovak ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sl-SI" ), AVSDocFileFormat::LID( DocFileFormat::Slovenian ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "so-SO" ), AVSDocFileFormat::LID( DocFileFormat::Somali ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-AR" ), AVSDocFileFormat::LID( DocFileFormat::SpanishArgentina ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-BO" ), AVSDocFileFormat::LID( DocFileFormat::SpanishBolivia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-CL" ), AVSDocFileFormat::LID( DocFileFormat::SpanishChile ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-CO" ), AVSDocFileFormat::LID( DocFileFormat::SpanishColombia ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-CR" ), AVSDocFileFormat::LID( DocFileFormat::SpanishCostaRica ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-DO" ), AVSDocFileFormat::LID( DocFileFormat::SpanishDominicanRepublic ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-EC" ), AVSDocFileFormat::LID( DocFileFormat::SpanishEcuador ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-SV" ), AVSDocFileFormat::LID( DocFileFormat::SpanishElSalvador ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-GT" ), AVSDocFileFormat::LID( DocFileFormat::SpanishGuatemala ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-HN" ), AVSDocFileFormat::LID( DocFileFormat::SpanishHonduras ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-MX" ), AVSDocFileFormat::LID( DocFileFormat::SpanishMexico ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-NI" ), AVSDocFileFormat::LID( DocFileFormat::SpanishNicaragua ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-PA" ), AVSDocFileFormat::LID( DocFileFormat::SpanishPanama ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-PY" ), AVSDocFileFormat::LID( DocFileFormat::SpanishParaguay ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-PE" ), AVSDocFileFormat::LID( DocFileFormat::SpanishPeru ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-PR" ), AVSDocFileFormat::LID( DocFileFormat::SpanishPuertoRico ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-ES" ), AVSDocFileFormat::LID( DocFileFormat::SpanishSpainTraditionalSort ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-UY" ), AVSDocFileFormat::LID( DocFileFormat::SpanishUruguay ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "es-VE" ), AVSDocFileFormat::LID( DocFileFormat::SpanishVenezuela ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sw-TZ" ), AVSDocFileFormat::LID( DocFileFormat::Swahili ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sv-FI" ), AVSDocFileFormat::LID( DocFileFormat::SwedishFinland ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "sv-SE" ), AVSDocFileFormat::LID( DocFileFormat::SwedishSweden ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "tg-TJ" ), AVSDocFileFormat::LID( DocFileFormat::Tajik ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "ta-ID" ), AVSDocFileFormat::LID( DocFileFormat::Tamil ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "tt-RU" ), AVSDocFileFormat::LID( DocFileFormat::Tatar ) ) );
		lidMap.insert( pair<string, AVSDocFileFormat::LID>( string( "te-ID" ), AVSDocFileFormat::LID( DocFileFormat::Telugu ) ) );

		for (unsigned short i = 0; i < ( sizeof(AVSDocFileFormat::StyleIDs) / sizeof(AVSDocFileFormat::StyleIDs[0]) ); i++ )
		{
			predefinedStyleIDMap.insert( pair<string, unsigned short>( AVSDocFileFormat::StyleIDs[i], i ) );
		}

		styleTypeMap.insert( pair<string, AVSDocFileFormat::Constants::StyleType>( string( "paragraph" ), AVSDocFileFormat::Constants::styleTypeParagraph ) );
		styleTypeMap.insert( pair<string, AVSDocFileFormat::Constants::StyleType>( string( "character" ), AVSDocFileFormat::Constants::styleTypeCharacter ) );
		styleTypeMap.insert( pair<string, AVSDocFileFormat::Constants::StyleType>( string( "table" ), AVSDocFileFormat::Constants::styleTypeTable ) );
		styleTypeMap.insert( pair<string, AVSDocFileFormat::Constants::StyleType>( string( "numbering" ), AVSDocFileFormat::Constants::styleTypeNumbering ) );

		for ( unsigned short i = 0; i < ( sizeof(AVSDocFileFormat::NumberFormatCodes) / sizeof(AVSDocFileFormat::NumberFormatCodes[0]) ); i++ )
		{
			numFmtMap.insert( pair<string, AVSDocFileFormat::Constants::MSONFC>( AVSDocFileFormat::NumberFormatCodes[i], (AVSDocFileFormat::Constants::MSONFC)i ) );
		}

		kulMap.insert( pair<string, byte>( string( "none" ), 0x00 ) );
		kulMap.insert( pair<string, byte>( string( "single" ), 0x01 ) );
		kulMap.insert( pair<string, byte>( string( "words" ), 0x02 ) );
		kulMap.insert( pair<string, byte>( string( "double" ), 0x03 ) );
		kulMap.insert( pair<string, byte>( string( "dotted" ), 0x04 ) );
		kulMap.insert( pair<string, byte>( string( "thick" ), 0x06 ) );
		kulMap.insert( pair<string, byte>( string( "dash" ), 0x07 ) );
		kulMap.insert( pair<string, byte>( string( "dotDash" ), 0x09 ) );
		kulMap.insert( pair<string, byte>( string( "dotDotDash" ), 0x0A ) );
		kulMap.insert( pair<string, byte>( string( "wave" ), 0x0B ) );
		kulMap.insert( pair<string, byte>( string( "dottedHeavy" ), 0x14 ) );
		kulMap.insert( pair<string, byte>( string( "dashedHeavy" ), 0x17 ) );
		kulMap.insert( pair<string, byte>( string( "dashDotHeavy" ), 0x19 ) );
		kulMap.insert( pair<string, byte>( string( "dashDotDotHeavy" ), 0x1A ) );
		kulMap.insert( pair<string, byte>( string( "wavyHeavy" ), 0x1B ) );
		kulMap.insert( pair<string, byte>( string( "dashLong" ), 0x27 ) );
		kulMap.insert( pair<string, byte>( string( "wavyDouble" ), 0x2B ) );
		kulMap.insert( pair<string, byte>( string( "dashLongHeavy" ), 0x37 ) );

		sectionBreakTypeMap.insert( pair<string, AVSDocFileFormat::Constants::SBkcOperand>( string( "continuous" ), AVSDocFileFormat::Constants::bkcContinuous ) );
		sectionBreakTypeMap.insert( pair<string, AVSDocFileFormat::Constants::SBkcOperand>( string( "nextColumn" ), AVSDocFileFormat::Constants::bkcNewColumn ) );
		sectionBreakTypeMap.insert( pair<string, AVSDocFileFormat::Constants::SBkcOperand>( string( "nextPage" ), AVSDocFileFormat::Constants::bkcNewPage ) );
		sectionBreakTypeMap.insert( pair<string, AVSDocFileFormat::Constants::SBkcOperand>( string( "evenPage" ), AVSDocFileFormat::Constants::bkcEvenPage ) );
		sectionBreakTypeMap.insert( pair<string, AVSDocFileFormat::Constants::SBkcOperand>( string( "oddPage" ), AVSDocFileFormat::Constants::bkcOddPage ) );

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

		tableCellWidthMap.insert( make_pair( "nil", AVSDocFileFormat::Constants::ftsNil ) );
		tableCellWidthMap.insert( make_pair( "auto", AVSDocFileFormat::Constants::ftsAuto ) );
		tableCellWidthMap.insert( make_pair( "pct", AVSDocFileFormat::Constants::ftsPercent ) );
		tableCellWidthMap.insert( make_pair( "dxa", AVSDocFileFormat::Constants::ftsDxa ) );

		customTabStopAlignment.insert( make_pair( "left", AVSDocFileFormat::Constants::jcLeft ) );
		customTabStopAlignment.insert( make_pair( "center", AVSDocFileFormat::Constants::jcCenter ) );
		customTabStopAlignment.insert( make_pair( "right", AVSDocFileFormat::Constants::jcRight ) );
		customTabStopAlignment.insert( make_pair( "decimal", AVSDocFileFormat::Constants::jcDecimal ) );
		customTabStopAlignment.insert( make_pair( "bar", AVSDocFileFormat::Constants::jcBar ) );
		customTabStopAlignment.insert( make_pair( "clear", (AVSDocFileFormat::Constants::TabJC)0x05 ) );
		customTabStopAlignment.insert( make_pair( "num", AVSDocFileFormat::Constants::jcList ) );

		customTabStopLeader.insert( make_pair( "none", AVSDocFileFormat::Constants::tlcNone ) );
		customTabStopLeader.insert( make_pair( "dot", AVSDocFileFormat::Constants::tlcDot ) );
		customTabStopLeader.insert( make_pair( "hyphen", AVSDocFileFormat::Constants::tlcHyphen ) );
		customTabStopLeader.insert( make_pair( "underscore", AVSDocFileFormat::Constants::tlcUnderscore ) );
		customTabStopLeader.insert( make_pair( "heavy", AVSDocFileFormat::Constants::tlcHeavy ) );
		customTabStopLeader.insert( make_pair( "middleDot", AVSDocFileFormat::Constants::tlcMiddleDot ) );
	}

	CFileTransformer::~CFileTransformer ()
	{
		AVSDocFileFormat::BinaryStorageSingleton* pBin	=	AVSDocFileFormat::BinaryStorageSingleton::Instance();
		if (pBin)
			pBin->FreeInstance();

		AVSDocFileFormat::COArtStorage* pStorage = AVSDocFileFormat::COArtStorage::Instance();
		if (pStorage)
			pStorage->FreeInstance();

		RELEASEOBJECT (m_pDOCFile);
	}
}

namespace DOCXTODOC
{
	long CFileTransformer::Convert (const WCHAR* ooxFolder, const WCHAR* docFile, const ProgressCallback* ffCallBack)
	{
		LONG hrStatus = AVS_ERROR_UNEXPECTED;

		if ( (NULL != ooxFolder) && (NULL != docFile) && m_pDOCFile )
		{
			try
			{
				inputFolder.read (ooxFolder);

				if (UpdateProgress (ffCallBack,500000))
					return S_FALSE;

				SHORT index = 0;

				size_t count = (*inputFolder.find<OOX::Document>().find<OOX::FontTable>().Fonts).size();
				std::vector<OOX::FontTable::Font>& items = (*inputFolder.find<OOX::Document>().find<OOX::FontTable>().Fonts);

				for (size_t j = 0; j < count; ++j)	
				{
					m_mapFontTableMap.insert(pair<string, short>(items[j].Name, index++));
				}

				if (UpdateProgress (ffCallBack,625000))
					return S_FALSE;

				if (inputFolder.find<OOX::Document>().exist<OOX::Numbering>())
					ConvertNumbering( inputFolder.find<OOX::Document>().find<OOX::Numbering>() );

				ConvertStyleSheet (inputFolder.find<OOX::Document>().find<OOX::Styles>());

				if (UpdateProgress (ffCallBack,750000))
					return S_FALSE;

				ConvertDocument (inputFolder.find<OOX::Document>());
				ConvertFontTable(inputFolder.find<OOX::Document>().find<OOX::FontTable>());

				if (UpdateProgress (ffCallBack,875000))
					return S_FALSE;

				hrStatus = m_pDOCFile->SaveToFile (docFile);

				if (UpdateProgress (ffCallBack,1000000))
					return S_FALSE;
			}
			catch (...)
			{
				hrStatus = AVS_ERROR_UNEXPECTED;
			}
		}

		return hrStatus;
	}

	BOOL CFileTransformer::UpdateProgress (const ProgressCallback* ffCallBack, long nComplete)
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
	void CFileTransformer::ConvertDocument (const OOX::Document& oXmlDoc)
	{
		ConvertContent (*oXmlDoc.Items);

		// TODO : если список в конце документа, то добавляется брэйк (бага)

		AVSDocFileFormat::Paragraph paragraph;
		paragraph.AddParagraphItem (AVSDocFileFormat::Run());
		m_pDOCFile->AddTextItem (paragraph);

		m_pDOCFile->AddSectionProperties (ConvertSectionProperties(oXmlDoc.SectorProperty));
	}

	void CFileTransformer::ConvertContent (const vector<OOX::Logic::TextItem>& oXmlItems)
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
					AVSDocFileFormat::Paragraph docParagraph	=	ConvertParagraph<OOX::Document>(docxParagraph);

					m_pDOCFile->AddTextItem (docParagraph);

					if (docxParagraph.Property.is_init() && docxParagraph.Property->SectorProperty.is_init())
					{
						m_pDOCFile->AddTextItem (AVSDocFileFormat::SectionBreak());
						m_pDOCFile->AddSectionProperties(ConvertSectionProperties( docxParagraph.Property->SectorProperty));
					}
				}
			}

			if (oXmlItem.is<OOX::Logic::Table>())
			{
				AVSDocFileFormat::Table oDocTable = CreateTable<OOX::Document>(oXmlItem.as<OOX::Logic::Table>());
				m_pDOCFile->AddTextItem (oDocTable);
			}

			if (oXmlItem.is<OOX::Logic::Sdt>())
			{
				ConvertContent(*oXmlItem.as<OOX::Logic::Sdt>().Content->Items);  
			}
		}  
	}

	template<class T> AVSDocFileFormat::Paragraph CFileTransformer::ConvertParagraph (const OOX::Logic::Paragraph& oXmlParagraph)
	{
		PrlList styleRunPr;
		PrlList styleParPr;

		std::string style	=	GetStyleID (oXmlParagraph);

		AVSDocFileFormat::Paragraph oParagraph;
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

		BOOL haveGoBack			=	FALSE;	//TODO : some content

		for (size_t i = 0; i < oXmlParagraph.Items->size(); ++i)
		{
			const OOX::Logic::ParagraphItem& oParagraphItem	= oXmlParagraph.Items->operator[](i);

			std::wstring strRunType;

			if (oParagraphItem.is<OOX::Logic::Run>())
			{
				AVSDocFileFormat::Run oAddRun = ConvertRun<T>(oParagraphItem.as<OOX::Logic::Run>(), styleRunPr, strRunType);

				if (m_bIsInlineShape)
				{
					//AVSDocFileFormat::Picture oPicture (std::wstring(L"c:\\man.png"), 0, 0, 1000, 1000);
					//oParagraph.AddParagraphItem(AVSDocFileFormat::Run(oPicture));

					oParagraph.AddParagraphItem(m_oInlineShape);
				}
				else
				{
					oParagraph.AddParagraphItem(oAddRun);
				}

				//AVSDocFileFormat::Picture oPicture (std::wstring(L"c:\\man.png"), 0, 0, 5000, 5000);
				//oParagraph.AddParagraphItem(AVSDocFileFormat::Run(oPicture));

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

				oParagraph.AddParagraphItem (AVSDocFileFormat::BookmarkStart(FormatUtils::UTF8Decode(oBookMark.Id), FormatUtils::UTF8Decode(oBookMark.Name)));
			}
			else if (oParagraphItem.is<OOX::Logic::BookmarkEnd>())
			{
				if (haveGoBack)
				{
					haveGoBack	=	FALSE;
					continue;
				}

				const OOX::Logic::BookmarkEnd& oBookMark = oParagraphItem.as<OOX::Logic::BookmarkEnd>();
				oParagraph.AddParagraphItem (AVSDocFileFormat::BookmarkEnd(FormatUtils::UTF8Decode(oBookMark.Id)));
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

			UpdateItemByCondition<AVSDocFileFormat::Paragraph> (&oParagraph, strRunType);
		}

		if (oXmlParagraph.Items->empty())
		{
			// Have some other properties

			AVSDocFileFormat::Run run;
			run.AddProperties(styleRunPr);

			if (oXmlParagraph.Property.is_init() && oXmlParagraph.Property->RunProperty.is_init())
			{
				run.AddOrReplaceProperties(styleRunPr);
			}

			oParagraph.AddParagraphItem(run);
		}

		oParagraph.AddProperties (ConvertParagraphProperties (inputFolder.find<OOX::Document>().find<OOX::Styles>().Default->ParagraphProperty));
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
	BOOL CFileTransformer::ValidParagraph (const OOX::Logic::Paragraph& oXmlParagraph)
	{
		// NOT IMPEMENT

		return TRUE;
	}

	PrlList CFileTransformer::GetParagraphPropertiesFromStyleHierarchy(const string& styleID, PrlList* styleDocRunLinkProperties ) 
	{
		PrlList allParagraphProperties;

		const OOX::Styles::Style styleById = this->inputFolder.find<OOX::Document>().find<OOX::Styles>().GetStyleById(styleID);

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

		const OOX::Styles::Style styleById = inputFolder.find<OOX::Document>().find<OOX::Styles>().GetStyleById( styleID );

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

	PrlList CFileTransformer::GetTablePropertiesFromStyleHierarchy (const string& styleID) 
	{
		PrlList allTableProperties;

		const OOX::Styles::Style styleById = inputFolder.find<OOX::Document>().find<OOX::Styles>().GetStyleById( styleID );

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

	string CFileTransformer::GetFontNameByThemeName (const string& themeName) 
	{
		string fontName;

		const OOX::Theme::File& themeFile = this->inputFolder.find<OOX::Document>().find<OOX::Theme::File>();

		if ( themeFile.themeElements.is_init() && themeFile.themeElements->fontScheme.is_init() )
		{
			const OOX::Theme::FontScheme& fontScheme = *themeFile.themeElements->fontScheme;

			string major = "major";
			string minor = "minor";

			string hAnsi = "HAnsi";
			string eastAsia = "EastAsia";
			string bidi = "Bidi";

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

		return fontName;
	}

	template<class T> void CFileTransformer::ConvertFldSimple(const OOX::Logic::FldSimple& fldSimpleDocx, const PrlList& styleDocRunProperties, AVSDocFileFormat::Paragraph& docParagraph, std::wstring& strRunType) 
	{
		string::size_type findIndex = string::npos;

		findIndex = fldSimpleDocx.Instr->find( "PAGE" );

		if ( findIndex != string::npos )
		{
			docParagraph.AddParagraphItem( AVSDocFileFormat::Run( AVSDocFileFormat::FldChar( AVSDocFileFormat::FldCharTypeBegin, AVSDocFileFormat::Constants::fltPAGE ) ) );
			docParagraph.AddParagraphItem( AVSDocFileFormat::Run( AVSDocFileFormat::FldChar( AVSDocFileFormat::FldCharTypeSeparate ) ) );
			docParagraph.AddParagraphItem( ConvertRun<T>( *fldSimpleDocx.Run, styleDocRunProperties, strRunType ) );
			docParagraph.AddParagraphItem( AVSDocFileFormat::Run( AVSDocFileFormat::FldChar( AVSDocFileFormat::FldCharTypeEnd ) ) );
		}

		findIndex = fldSimpleDocx.Instr->find( "SYMBOL" );

		if ( findIndex != string::npos )
		{
			AVSDocFileFormat::Run fldSymbolRun;

			fldSymbolRun.AddRunItem( AVSDocFileFormat::Text( FormatUtils::UTF8Decode( *fldSimpleDocx.Instr ) ) );
			fldSymbolRun.AddProperties( styleDocRunProperties );

			strRunType = _T( "SYMBOL" );

			docParagraph.AddParagraphItem( AVSDocFileFormat::Run( AVSDocFileFormat::FldChar( AVSDocFileFormat::FldCharTypeBegin, AVSDocFileFormat::Constants::fltSYMBOL ) ) );
			docParagraph.AddParagraphItem( fldSymbolRun );
			docParagraph.AddParagraphItem( AVSDocFileFormat::Run( AVSDocFileFormat::FldChar( AVSDocFileFormat::FldCharTypeSeparate ) ) );
			docParagraph.AddParagraphItem( AVSDocFileFormat::Run( AVSDocFileFormat::FldChar( AVSDocFileFormat::FldCharTypeEnd ) ) );
		}  
	}

	template<class T> AVSDocFileFormat::Hyperlink CFileTransformer::ConvertHyperlink (const OOX::Logic::Hyperlink& docxHyperlink, const PrlList& styleDocRunProperties ) 
	{
		AVSDocFileFormat::Hyperlink docHyperlink;

		if (docxHyperlink.rId.is_init())
		{
			OOX::HyperLink* hyperlink = NULL;
			const OOX::RId& rid = *docxHyperlink.rId;

			if (typeid(T) != typeid(OOX::Document))
			{
				if (inputFolder.find<OOX::Document>().find<T>().exist(rid))
				{
					hyperlink = dynamic_cast<OOX::HyperLink*>(inputFolder.find<OOX::Document>().find<T>()[rid].get());
				}
			}
			else
			{
				if (inputFolder.find<OOX::Document>().exist(rid))
				{
					hyperlink = dynamic_cast<OOX::HyperLink*>(inputFolder.find<OOX::Document>()[rid].get());
				}
			}

			if ( hyperlink != NULL )
			{
				wstring hyperlinkURL = hyperlink->Uri().string();

				docHyperlink.SetURL( hyperlinkURL.c_str() );
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
			UpdateItemByCondition<AVSDocFileFormat::Hyperlink>(&docHyperlink, strRunType);
		}

		return docHyperlink;
	}

	template<class T> void CFileTransformer::UpdateItemByCondition (T* docItem, const wstring& condition) 
	{
		if ( ( docItem != NULL ) && ( !condition.empty() ) )
		{
			static bool haveSeparator = false;

			AVSDocFileFormat::FldChar* fldChar = NULL;

			BOOL findComplete	=	FALSE;

			for (T::reverse_iterator riter = docItem->rbegin(); riter != docItem->rend(); ++riter)
			{
				if (riter->is<AVSDocFileFormat::Run>())
				{
					AVSDocFileFormat::Run& run = riter->as<AVSDocFileFormat::Run>();

					for (AVSDocFileFormat::Run::reverse_iterator runRIter = run.rbegin(); runRIter != run.rend(); ++runRIter)
					{
						if ( runRIter->is<AVSDocFileFormat::FldChar>() )
						{
							fldChar = &runRIter->as<AVSDocFileFormat::FldChar>();

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

			byte charType	=	fldChar->CharType();

			if (condition == std::wstring(_T("HYPERLINK")))
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties(AVSDocFileFormat::Constants::fltHYPERLINK);
				}
			}
			else if ( condition == std::wstring( _T( "PAGEREF" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( AVSDocFileFormat::Constants::fltPAGEREF );
				}
			}
			else if ( condition == std::wstring( _T( "TOC" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( AVSDocFileFormat::Constants::fltTOC );
				}
			}
			else if ( condition == std::wstring( _T( "PAGE" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( AVSDocFileFormat::Constants::fltPAGE );
				}  
			}
			else if ( condition == std::wstring( _T( "SYMBOL" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( AVSDocFileFormat::Constants::fltSYMBOL );
				}  
			}
			else if ( condition == std::wstring( _T( "ADDRESSBLOCK" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( AVSDocFileFormat::Constants::fltADDRESSBLOCK );
				}  
			}
			else if ( condition == std::wstring( _T( "GREETINGLINE" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( AVSDocFileFormat::Constants::fltGREETINGLINE );
				}  
			}
			else if ( condition == std::wstring( _T( "MERGEFIELD" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharBegin )
				{
					fldChar->SetFieldCharacterProperties( AVSDocFileFormat::Constants::fltMERGEFIELD );
				}  
			}
			else if ( condition == std::wstring( _T( "separate" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharSeparate )
				{
					//fldChar->SetFieldCharacterProperties(0);
					haveSeparator = true;
				}
			}
			else if ( condition == std::wstring( _T( "end" ) ) )
			{
				if (charType == AVSDocFileFormat::FldChar::FldCharEnd )
				{
					fldChar->SetFieldCharacterProperties( (byte)AVSDocFileFormat::grffldEnd( false, false, false, false, false, false, false, haveSeparator ) );

					haveSeparator = false;
				}
			}
		}  
	}

	//
	AVSDocFileFormat::Constants::VerticalMergeFlag CFileTransformer::ConvertTableVerticalMergeFlag (const nullable_property<OOX::Logic::VMerge>& vMerge) 
	{
		AVSDocFileFormat::Constants::VerticalMergeFlag vmf = AVSDocFileFormat::Constants::fvmClear;

		if ( vMerge.is_init() )
		{
			if ( vMerge->Value.is_init() )
			{
				if ( *vMerge->Value == string( "restart" ) )
				{
					vmf = AVSDocFileFormat::Constants::fvmRestart;
				}
				else if ( *vMerge->Value == string( "continue" ) )
				{
					vmf = AVSDocFileFormat::Constants::fvmMerge;
				}
			}
			else
			{
				vmf = AVSDocFileFormat::Constants::fvmMerge;
			}
		}

		return vmf;
	}

	const PrlList CFileTransformer::ConvertRunProperties (const OOX::Logic::RunProperty& docxRunProperties) 
	{
		m_bIsHaveRunPr	=	TRUE;

		PrlList docRunProperties;

		if ( docxRunProperties.Bold.is_init() )
		{
			byte bold = ( ( *docxRunProperties.Bold ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFBold, &bold ) );
		}

		if ( docxRunProperties.Italic.is_init() )
		{
			byte italic = ( ( *docxRunProperties.Italic ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFItalic, &italic ) );
		}

		if ( docxRunProperties.Under.is_init() && *docxRunProperties.Under && docxRunProperties.UnderType.is_init() )
		{
			byte under = this->kulMap[*docxRunProperties.UnderType];

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCKul, &under ) );
		}

		if ( docxRunProperties.Strike.is_init() )
		{
			byte strike = ( ( *docxRunProperties.Strike ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFStrike, &strike ) );
		}

		if ( docxRunProperties.DStrike.is_init() )
		{
			byte dStrike = ( ( *docxRunProperties.DStrike ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFDStrike, &dStrike ) );
		}

		if ( docxRunProperties.SmallCaps.is_init() )
		{
			byte smallCaps = ( ( *docxRunProperties.SmallCaps ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFSmallCaps, &smallCaps ) );
		}

		if ( docxRunProperties.Caps.is_init() )
		{
			byte caps = ( ( *docxRunProperties.Caps ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFCaps, &caps ) );
		}

		if ( docxRunProperties.Emboss.is_init() )
		{
			byte emboss = ( ( *docxRunProperties.Emboss ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFEmboss, &emboss ) );
		}

		if ( docxRunProperties.Imprint.is_init() )
		{
			byte imprint = ( ( *docxRunProperties.Imprint ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFImprint, &imprint ) );
		}

		if ( docxRunProperties.Outline.is_init() )
		{
			byte outline = ( ( *docxRunProperties.Outline ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFOutline, &outline ) );
		}

		if ( docxRunProperties.Shadow.is_init() )
		{
			byte shadow = ( ( *docxRunProperties.Shadow ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFShadow, &shadow ) );
		}

		if ( docxRunProperties.Vanish.is_init() )
		{
			byte vanish = ( ( *docxRunProperties.Vanish ) ? ( 1 ) : ( 0 ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFVanish, &vanish ) );
		}

		if ( docxRunProperties.FontSize.is_init() )
		{
			unsigned short fontSize = (unsigned short)docxRunProperties.FontSize;
			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHps, (byte*)&fontSize ) );
			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsBi, (byte*)&fontSize ) );
		}

		if (docxRunProperties.FontColor.is_init())
		{
			int colorIntValue	=	AVSDocFileFormat::COLORREF::cvAuto;
			if (false == docxRunProperties.FontColor->isAuto())
				colorIntValue	=	HexString2Int( docxRunProperties.FontColor->ToString() );

			AVSDocFileFormat::COLORREF color (colorIntValue);

			docRunProperties.push_back (AVSDocFileFormat::Prl((short)DocFileFormat::sprmCCv, color));
		}

		if ( docxRunProperties.Highlight.is_init() )
		{
			byte ico = DOCXDOCUTILS::ColorToIco (*docxRunProperties.Highlight);
			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHighlight, &ico ) );
		}

		if ( docxRunProperties.Shading.is_init() && docxRunProperties.Shading->fill.is_init() )
		{
			int colorIntValue	=	0;
			int fillAuto		=	0x00;

			if (docxRunProperties.Shading->fill == "auto")
			{
				fillAuto = AVSDocFileFormat::COLORREF::cvAuto;
			}
			else
			{
				colorIntValue = HexString2Int( *docxRunProperties.Shading->fill );
			}

			AVSDocFileFormat::SHDOperand shdOperand ( 
				AVSDocFileFormat::Shd( AVSDocFileFormat::COLORREF( (int)( 0 | fillAuto ) ), 
				AVSDocFileFormat::COLORREF( colorIntValue | fillAuto ),
				0 ) );

			docRunProperties.push_back (AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCShd, shdOperand ));
		}

		if ( docxRunProperties.rFonts.is_init() )
		{
			short fontIndex = 0;

			if ( docxRunProperties.rFonts->ascii.is_init() && !docxRunProperties.rFonts->AsciiTheme.is_init() )
			{
				std::string strFontName	= docxRunProperties.rFonts->ascii;
				if (strFontName.length())
				{			
					AddInternalFont (strFontName);

					fontIndex	=	m_mapFontTableMap[strFontName];

					docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc0, (byte*)&fontIndex ) );
				}
			}
			else if ( docxRunProperties.rFonts->AsciiTheme.is_init() )
			{
				std::string fontNameByThemeName = GetFontNameByThemeName(docxRunProperties.rFonts->AsciiTheme);

				if ( !fontNameByThemeName.empty() )
				{
					map<string, short>::const_iterator findResult = m_mapFontTableMap.find( fontNameByThemeName );  

					if ( findResult != m_mapFontTableMap.end() )
					{
						fontIndex = findResult->second;
					}

					docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc0, (byte*)&fontIndex ) );
				}
			}

			if (docxRunProperties.rFonts->Cs.is_init() && !docxRunProperties.rFonts->Cstheme.is_init())
			{
				std::string strFontName	= docxRunProperties.rFonts->Cs;
				if (strFontName.length())
				{			
					AddInternalFont (strFontName);

					fontIndex	=	m_mapFontTableMap[strFontName];

					docRunProperties.push_back (AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (byte*)&fontIndex));
				}
			}
			else if ( docxRunProperties.rFonts->Cstheme.is_init() )
			{
				string fontNameByThemeName = this->GetFontNameByThemeName( docxRunProperties.rFonts->Cstheme );

				if ( !fontNameByThemeName.empty() )
				{
					std::map<string, short>::const_iterator findResult = m_mapFontTableMap.find( fontNameByThemeName );  

					if ( findResult != m_mapFontTableMap.end() )
					{
						fontIndex = findResult->second;
					}

					docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (byte*)&fontIndex ) );
				}
			}

			if ( docxRunProperties.rFonts->hAnsi.is_init() && !docxRunProperties.rFonts->HAnsiTheme.is_init() )
			{
				std::string strFontName	= docxRunProperties.rFonts->hAnsi;
				if (strFontName.length())
				{			
					AddInternalFont (strFontName);

					fontIndex	=	m_mapFontTableMap[docxRunProperties.rFonts->hAnsi];

					docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (byte*)&fontIndex ) );
				}
			}
			else if ( docxRunProperties.rFonts->HAnsiTheme.is_init() )
			{
				string fontNameByThemeName = GetFontNameByThemeName( docxRunProperties.rFonts->HAnsiTheme );

				if ( !fontNameByThemeName.empty() )
				{
					map<string, short>::const_iterator findResult = m_mapFontTableMap.find( fontNameByThemeName );  

					if (findResult != m_mapFontTableMap.end())
					{
						fontIndex = findResult->second;
					}

					docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (byte*)&fontIndex ) );
				}
			}
		}

		if ( docxRunProperties.Lang.is_init() )
		{
			if ( docxRunProperties.Lang->Value.is_init() )
			{
				AVSDocFileFormat::LID lid = lidMap[docxRunProperties.Lang->Value];
				docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0_80, (byte*)lid ) );
				docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0, (byte*)lid ) );
			}

			if ( docxRunProperties.Lang->EastAsia.is_init() )
			{
				AVSDocFileFormat::LID lid = lidMap[docxRunProperties.Lang->EastAsia];
				docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1_80, (byte*)lid ) );
				docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1, (byte*)lid ) );
			}

			if ( docxRunProperties.Lang->Bidi.is_init() )
			{
				AVSDocFileFormat::LID lid = lidMap[docxRunProperties.Lang->Bidi];
				docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCLidBi, (byte*)lid ) );
			}
		}

		if ( docxRunProperties.Spacing.is_init() )
		{
			short CDxaSpace = AVSDocFileFormat::XAS( *docxRunProperties.Spacing );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCDxaSpace, (byte*)&CDxaSpace ) );
		}

		if ( docxRunProperties.Index.is_init() )
		{
			string index = docxRunProperties.Index->ToString();
			byte CIss = (byte)AVSDocFileFormat::Constants::superSubScriptNormalText;

			if ( index == string( "superscript" ) )
			{
				CIss = (byte)AVSDocFileFormat::Constants::superSubScriptSuperscript;  
			}
			else if ( index == string( "subscript" ) )
			{
				CIss = (byte)AVSDocFileFormat::Constants::superSubScriptSubscript;  
			}

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCIss, (byte*)&CIss ) );
		}

		if ( docxRunProperties.Kern.is_init() )
		{
			int kern = *docxRunProperties.Kern;

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsKern, (byte*)&kern ) );
		}

		if ( docxRunProperties.Position.is_init() )
		{
			int position = *docxRunProperties.Position;

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsPos, (byte*)&position ) );
		}

		if ( docxRunProperties.Scale.is_init() )
		{
			unsigned short scale = *docxRunProperties.Scale;

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCCharScale, (byte*)&scale ) );
		}

		if ( docxRunProperties.Border.is_init() )
		{
			DOCXDOCUTILS::CTblBorders oBorder;
			map<std::string, byte>& oBrcMap	=	oBorder.GetBrcMap ();

			unsigned int brc80 =  (unsigned int)AVSDocFileFormat::Brc80(
				docxRunProperties.Border->Bdr->Sz.get_value_or_default(), 
				oBrcMap[*docxRunProperties.Border->Bdr->Value], 
				DOCXDOCUTILS::ColorToIco (docxRunProperties.Border->Bdr->Color.get_value_or_default()), 
				docxRunProperties.Border->Bdr->Space.get_value_or_default(), 
				false, 
				false );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc80, (byte*)(&brc80) ) );

			AVSDocFileFormat::BrcOperand brcOperand( AVSDocFileFormat::Brc(
				AVSDocFileFormat::COLORREF( HexString2Int( docxRunProperties.Border->Bdr->Color.get_value_or_default().ToString() ) ),
				docxRunProperties.Border->Bdr->Sz.get_value_or_default(), 
				oBrcMap[*docxRunProperties.Border->Bdr->Value],
				docxRunProperties.Border->Bdr->Space.get_value_or_default(), 
				false, 
				false ) );

			docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc, (byte*)brcOperand ) );
		}

		return docRunProperties;
	}

	const PrlList CFileTransformer::ConvertParagraphProperties (const OOX::Logic::ParagraphProperty& docxParagraphProperties) 
	{
		PrlList docParagraphProperties;

		if ( docxParagraphProperties.Align.is_init() )
		{
			byte justification	=	DOCXDOCUTILS::AlignFromString (docxParagraphProperties.Align->ToString());

			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPJc, &justification ) );
			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPJc80, &justification ) );
		}

		if ( docxParagraphProperties.Shading.is_init() && docxParagraphProperties.Shading->fill.is_init() )
		{
			int colorIntValue = 0;
			int fillAuto = 0x00;

			if ( *docxParagraphProperties.Shading->fill == "auto" )
			{
				fillAuto = AVSDocFileFormat::COLORREF::cvAuto;
			}
			else
			{
				colorIntValue = HexString2Int( *docxParagraphProperties.Shading->fill );
			}  

			AVSDocFileFormat::SHDOperand shdOperand( AVSDocFileFormat::Shd( AVSDocFileFormat::COLORREF( (int)( 0 | fillAuto ) ), 
				AVSDocFileFormat::COLORREF( colorIntValue | fillAuto ),
				0 ) );

			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPShd, shdOperand ) );
		}

		if ( docxParagraphProperties.Spacing.is_init() )
		{
			if ( docxParagraphProperties.Spacing->After.is_init() )
			{
				unsigned short pDyaAfter = (unsigned short)(*docxParagraphProperties.Spacing->After);

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaAfter, (byte*)(&pDyaAfter) ) );
			}

			if ( docxParagraphProperties.Spacing->Before.is_init() )
			{
				unsigned short pDyaBefore = (unsigned short)(*docxParagraphProperties.Spacing->Before);

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaBefore, (byte*)(&pDyaBefore) ) );
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

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaLine, (byte*)AVSDocFileFormat::LSPD( line, lineRule ) ) );
			}

			if ( docxParagraphProperties.Spacing->BeforeAutospacing.is_init() )
			{
				Bool8 beforeAutospacing = 0x00;

				if ( *docxParagraphProperties.Spacing->BeforeAutospacing )
				{
					beforeAutospacing = 0x01;
				}

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPFDyaBeforeAuto, &beforeAutospacing ) );
			}

			if ( docxParagraphProperties.Spacing->AfterAutospacing.is_init() )
			{
				Bool8 afterAutospacing = 0x00;

				if ( *docxParagraphProperties.Spacing->AfterAutospacing )
				{
					afterAutospacing = 0x01;
				}

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPFDyaAfterAuto, &afterAutospacing ) );
			}
		}

		if ( docxParagraphProperties.Ind.is_init() )
		{
			if ( docxParagraphProperties.Ind->Left.is_init() )
			{
				short pDxaLeft = AVSDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->Left) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft, (byte*)(&pDxaLeft) ) );
				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft80, (byte*)(&pDxaLeft) ) );
			}

			if ( docxParagraphProperties.Ind->Right.is_init() )
			{
				short pDxaRight = AVSDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->Right) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaRight, (byte*)(&pDxaRight) ) );
				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaRight80, (byte*)(&pDxaRight) ) );
			}

			if ( docxParagraphProperties.Ind->Hanging.is_init() )
			{
				short pDxaLeft1 = ( ( AVSDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->Hanging) ) ) * ( -1 ) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft1, (byte*)(&pDxaLeft1) ) );
				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft180, (byte*)(&pDxaLeft1) ) );
			}

			if ( docxParagraphProperties.Ind->FirstLine.is_init() )
			{
				short pDxaLeft1 = AVSDocFileFormat::XAS( (short)(*docxParagraphProperties.Ind->FirstLine) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft1, (byte*)(&pDxaLeft1) ) );
				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDxaLeft180, (byte*)(&pDxaLeft1) ) );
			}
		}

		if (docxParagraphProperties.NumPr.is_init())
		{
			if (docxParagraphProperties.NumPr->Ilvl.is_init())
			{
				BYTE pIlvl	=	(BYTE)(*docxParagraphProperties.NumPr->Ilvl);
				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPIlvl, (byte*)(&pIlvl) ) );
			}

			if ( docxParagraphProperties.NumPr->NumId.is_init() )
			{
				int nNumID	=	(*docxParagraphProperties.NumPr->NumId);
				SHORT pIlfo	=	idIndexMap [nNumID];

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPIlfo, (byte*)(&pIlfo) ) );
			}
		}

		if ( docxParagraphProperties.OutlineLvl.is_init() )
		{
			byte POutLvl = *docxParagraphProperties.OutlineLvl;

			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPOutLvl, &POutLvl ) );
		}

		if ( *docxParagraphProperties.KeepLines )
		{
			Bool8 PFKeep = 0x01;

			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPFKeep, &PFKeep ) );
		}

		if ( *docxParagraphProperties.KeepNext )
		{
			Bool8 PFKeepFollow = 0x01;

			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPFKeepFollow, &PFKeepFollow ) );
		}

		if (*docxParagraphProperties.pageBreakBefore)
		{
			Bool8 PFPageBreakBefore = 0x01;
			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPFPageBreakBefore, &PFPageBreakBefore ) );
		}

		if ( *docxParagraphProperties.ContextualSpacing )
		{
			Bool8 PFContextualSpacing = 0x01;
			docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPFContextualSpacing, &PFContextualSpacing ) );
		}

		if ( docxParagraphProperties.ParagraphBorder.is_init() )
		{
			DOCXDOCUTILS::CTblBorders oBorder;
			map<std::string, byte>& oBrcMap	=	oBorder.GetBrcMap ();

			if ( docxParagraphProperties.ParagraphBorder->Top.is_init() )
			{
				unsigned int brc80Top =  (unsigned int)AVSDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Top->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Top->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Top->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Top->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcTop80, (byte*)(&brc80Top) ) );

				AVSDocFileFormat::BrcOperand brcOperandTop( AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Top->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Top->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Top->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Top->Bdr->Space.get_value_or_default(), 
					false, 
					false ) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcTop, (byte*)brcOperandTop ) );
			}

			if ( docxParagraphProperties.ParagraphBorder->Bottom.is_init() )
			{
				unsigned int brc80Bottom =  (unsigned int)AVSDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcBottom80, (byte*)(&brc80Bottom) ) );

				AVSDocFileFormat::BrcOperand brcOperandBottom( AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Bottom->Bdr->Space.get_value_or_default(),
					false, 
					false ) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcBottom, (byte*)brcOperandBottom ) );
			}

			if ( docxParagraphProperties.ParagraphBorder->Left.is_init() )
			{
				unsigned int brc80Left =  (unsigned int)AVSDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Left->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Left->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Left->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Left->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcLeft80, (byte*)(&brc80Left) ) );

				AVSDocFileFormat::BrcOperand brcOperandLeft( AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Left->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Left->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Left->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Left->Bdr->Space.get_value_or_default(),
					false, 
					false ) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcLeft, (byte*)brcOperandLeft ) );
			}

			if ( docxParagraphProperties.ParagraphBorder->Right.is_init() )
			{
				unsigned int brc80Right =  (unsigned int)AVSDocFileFormat::Brc80( docxParagraphProperties.ParagraphBorder->Right->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Right->Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxParagraphProperties.ParagraphBorder->Right->Bdr->Color.get_value_or_default()), 
					docxParagraphProperties.ParagraphBorder->Right->Bdr->Space.get_value_or_default(), 
					false, 
					false );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcRight80, (byte*)(&brc80Right) ) );

				AVSDocFileFormat::BrcOperand brcOperandRight( AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF( HexString2Int( docxParagraphProperties.ParagraphBorder->Right->Bdr->Color.get_value_or_default().ToString() ) ),
					docxParagraphProperties.ParagraphBorder->Right->Bdr->Sz.get_value_or_default(), 
					oBrcMap[*docxParagraphProperties.ParagraphBorder->Right->Bdr->Value],
					docxParagraphProperties.ParagraphBorder->Right->Bdr->Space.get_value_or_default(),
					false, 
					false ) );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPBrcRight, (byte*)brcOperandRight ) );
			}
		}

		if ( docxParagraphProperties.TextFrameProperties.is_init() )
		{
			if ( docxParagraphProperties.TextFrameProperties->HAnchor.is_init() && docxParagraphProperties.TextFrameProperties->VAnchor.is_init() )
			{
				byte positionCodeOperand = AVSDocFileFormat::PositionCodeOperand( this->verticalPositionCodeMap[*docxParagraphProperties.TextFrameProperties->VAnchor], this->horizontalPositionCodeMap[*docxParagraphProperties.TextFrameProperties->HAnchor] );
				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPPc, &positionCodeOperand ) );
			}

			if ( docxParagraphProperties.TextFrameProperties->Wrap.is_init() )
			{
				byte PWr = this->textFrameWrappingMap[*docxParagraphProperties.TextFrameProperties->Wrap];
				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPWr, &PWr ) );
			}

			if ( docxParagraphProperties.TextFrameProperties->X.is_init() )
			{
				short PDxaAbs = AVSDocFileFormat::XAS_plusOne ((short)(*docxParagraphProperties.TextFrameProperties->X));
				docParagraphProperties.push_back (AVSDocFileFormat::Prl((short)DocFileFormat::sprmPDxaAbs, (byte*)(&PDxaAbs)));
			}

			if ( docxParagraphProperties.TextFrameProperties->Y.is_init() )
			{
				short PDyaAbs = AVSDocFileFormat::YAS_plusOne ((short)*docxParagraphProperties.TextFrameProperties->Y);
				docParagraphProperties.push_back (AVSDocFileFormat::Prl((short)DocFileFormat::sprmPDyaAbs, (byte*)(&PDyaAbs)));
			}

			if ( docxParagraphProperties.TextFrameProperties->H.is_init() )
			{
				unsigned short PWHeightAbs = AVSDocFileFormat::YAS_nonNeg ((unsigned short)*docxParagraphProperties.TextFrameProperties->H);
				docParagraphProperties.push_back (AVSDocFileFormat::Prl ((short)DocFileFormat::sprmPWHeightAbs, (byte*)(&PWHeightAbs)));
			}

			if ( docxParagraphProperties.TextFrameProperties->W.is_init() )
			{
				unsigned short PDxaWidth = AVSDocFileFormat::XAS_nonNeg ((unsigned short)*docxParagraphProperties.TextFrameProperties->W);
				docParagraphProperties.push_back (AVSDocFileFormat::Prl((short)DocFileFormat::sprmPDxaWidth, (byte*)(&PDxaWidth)));
			}

			if ( docxParagraphProperties.TextFrameProperties->HSpace.is_init() )
			{
				unsigned short PDxaFromText = AVSDocFileFormat::XAS_nonNeg((unsigned short)*docxParagraphProperties.TextFrameProperties->HSpace);
				docParagraphProperties.push_back(AVSDocFileFormat::Prl((short)DocFileFormat::sprmPDxaFromText, (byte*)(&PDxaFromText)));
			}

			if ( docxParagraphProperties.TextFrameProperties->VSpace.is_init() )
			{
				unsigned short PDyaFromText = AVSDocFileFormat::YAS_nonNeg((unsigned short)*docxParagraphProperties.TextFrameProperties->VSpace);
				docParagraphProperties.push_back (AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPDyaFromText, (byte*)(&PDyaFromText)));
			}
		}

		if ( docxParagraphProperties.Tabs.is_init() )
		{
			vector<AVSDocFileFormat::TBD> tbds;
			vector<AVSDocFileFormat::XAS> xass;

			size_t count = (*docxParagraphProperties.Tabs->Tabs).size();
			const std::vector<OOX::Logic::TabProperty>& items = (*docxParagraphProperties.Tabs->Tabs);

			for (size_t i = 0; i < count; ++i)	
			{			
				AVSDocFileFormat::Constants::TabJC TabStopAlign = customTabStopAlignment[*(items[i]).Val];								
				if (0x05 == (int)TabStopAlign)	// 0x05 означает clear, т.е. No Tab Stop
					continue;

				AVSDocFileFormat::TBD tbd(TabStopAlign, customTabStopLeader[items[i].Leader.get_value_or_default()]);
				tbds.push_back(tbd);

				AVSDocFileFormat::XAS xas((short)(*(items[i]).Pos));
				xass.push_back(xas);
			}

			if (xass.size() > 0 && tbds.size() > 0)
			{
				AVSDocFileFormat::PChgTabsAdd pChgTabsAdd( xass, tbds );
				AVSDocFileFormat::PChgTabsPapxOperand pChgTabsPapxOperand( AVSDocFileFormat::PChgTabsDel(), pChgTabsAdd );

				docParagraphProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmPChgTabsPapx, (byte*)pChgTabsPapxOperand ) );
			}
		}

		return docParagraphProperties;
	}
}

namespace DOCXTODOC	//	LEVELS
{
	void CFileTransformer::ConvertNumbering (const OOX::Numbering& oXmlNumbering)
	{
		if (oXmlNumbering.AbstractNums->size() == 0 || oXmlNumbering.Nums->size() == 0)
			return;

		AVSDocFileFormat::PlfLst plfLst	(ConvertAbstractNums(*oXmlNumbering.AbstractNums));
		vector<AVSDocFileFormat::LVL> lvls = ConvertLVLs (oXmlNumbering, *oXmlNumbering.AbstractNums);
		AVSDocFileFormat::ListFormattingInformation lfi (plfLst, lvls); 

		m_pDOCFile->SetListFormattingInformation (lfi);
		m_pDOCFile->SetListFormattingOverrideInformation (ConvertNums(*oXmlNumbering.Nums));
	}

	const vector<AVSDocFileFormat::LSTF> CFileTransformer::ConvertAbstractNums (const vector<OOX::Numbering::AbstractNum>& arrNums) 
	{
		// Fill LSTF ( The LSTF structure contains formatting properties that apply to an entire list. )

		vector<AVSDocFileFormat::LSTF> arrLSTF;

		int nId	=	1;	//	уникальный индентификатор для списка, любое число от 1 ~ 0xFFFFFFFF

		size_t count = arrNums.size();
		for (size_t j = 0; j < count; ++j)
		{
			const OOX::Numbering::AbstractNum& number = arrNums[j];

			if (number.Nsid.is_init())
			{
				nId			=	HexString2Int (number.Nsid.get_value_or_default());
			}
			else
			{
				for (map<int, int>::const_iterator it = idLsidMap.begin(); it != idLsidMap.end(); ++it)
				{
					nId		=	__max(it->second, nId);
				}

				++nId;
			}

			idLsidMap.insert(make_pair(number.Id, nId));

			AVSDocFileFormat::Tplc* tplc = AVSDocFileFormat::TplcFactory::CreateTplc (HexString2Int(number.Tmpl.get_value_or_default()));

			bool fSimpleList	=	false;
			bool fAutoNum		=	false;
			bool fHybrid		=	false;

			if ((*number.MultiLevelType) == string("hybridMultilevel"))
				fHybrid			=	true; 
			if ((*number.MultiLevelType) == string("singleLevel"))
				fSimpleList		=	true;  

			arrLSTF.push_back (AVSDocFileFormat::LSTF (nId, tplc, fSimpleList, fAutoNum, fHybrid, AVSDocFileFormat::grfhic(), NULL /*!!!TODO!!!*/));

			RELEASEOBJECT(tplc);
		}

		return arrLSTF;
	}

	const vector<AVSDocFileFormat::LVL> CFileTransformer::ConvertLVLs(const OOX::Numbering& oXmlNumbering, const vector<OOX::Numbering::AbstractNum>& arAbstractNums) 
	{
		vector<AVSDocFileFormat::LVL> oLevels;

		size_t numsCount = arAbstractNums.size();
		for (size_t j = 0; j < numsCount; ++j)
		{
			const OOX::Numbering::AbstractNum& oAbstractNum = arAbstractNums[j];
			size_t levCount = oAbstractNum.Levels->size();
			for (size_t i = 0; i < levCount; ++i)
			{
				const OOX::Numbering::Level& oLevel = oAbstractNum.Levels->operator [](i);
				oLevels.push_back (ConvertLVL(oLevel));
			}

			// 
			if (0 == levCount)
			{
				if (oAbstractNum.numStyleLink.is_init())
				{
					int nInd = FindAbstractNumIdWithStyleRef (oXmlNumbering, oAbstractNum.numStyleLink);
					if ((nInd >= 0) && (nInd < (int)numsCount))
					{
						for (size_t i = 0; i < numsCount; ++i)
						{
							const OOX::Numbering::AbstractNum& oFindNum = arAbstractNums[i];
							if (nInd == oFindNum.Id)
							{
								size_t levCount = oFindNum.Levels->size();
								for (size_t m = 0; m < levCount; ++m)
								{
									const OOX::Numbering::Level& oLevel = oFindNum.Levels->operator [](m);
									oLevels.push_back (ConvertLVL(oLevel));
								}

								break;
							}
						}
					}
				}
			}
		}

		return oLevels;
	}

	const AVSDocFileFormat::LVL CFileTransformer::ConvertLVL (const OOX::Numbering::Level& _level) 
	{
		AVSDocFileFormat::Constants::MSONFC nfc = this->numFmtMap[_level.NumFmt->ToString()];
		AVSDocFileFormat::Constants::LevelJustification jc = 
			(AVSDocFileFormat::Constants::LevelJustification)DOCXDOCUTILS::AlignFromString (_level.Align.get_value_or(OOX::Logic::Align( string( "left" ) ) ).ToString());
		bool fTentative = ( ( _level.Tentative.get_value_or( 0 ) == 1 ) ? ( true ) : ( false ) );
		AVSDocFileFormat::Constants::CharacterFollows ixchFollow = AVSDocFileFormat::Constants::characterFollowsTab;

		if ( _level.Suffix.is_init() )
		{
			if ( *_level.Suffix == string( "tab" ) )
			{
				ixchFollow = AVSDocFileFormat::Constants::characterFollowsTab;
			}
			else if ( *_level.Suffix == string( "space" ) )
			{
				ixchFollow = AVSDocFileFormat::Constants::characterFollowsSpace;  
			}
			else
			{
				ixchFollow = AVSDocFileFormat::Constants::characterFollowsNothing;
			}
		}

		AVSDocFileFormat::LVLF lvlf( *_level.Start, nfc, jc, false, false, false, false, fTentative, ixchFollow, 0, 0, AVSDocFileFormat::grfhic() /*!!!TODO!!!*/ );

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

		return AVSDocFileFormat::LVL( lvlf, grpprlPapx, grpprlChpx, this->ConvertLvlText( *_level.Text, nfc  ) );
	}

	const AVSDocFileFormat::PlfLfo CFileTransformer::ConvertNums (const vector<OOX::Numbering::Num>& oXmlNums) 
	{
		vector<AVSDocFileFormat::LFO> lfos;
		vector<AVSDocFileFormat::LFOData> lfoDatas;

		short listIndex = 1;

		for (size_t i = 0; i < oXmlNums.size(); ++i)
		{
			const OOX::Numbering::Num& oXmlNum = oXmlNums[i];
			idIndexMap.insert( make_pair( *oXmlNum.NumId, listIndex++ ) );

			vector<AVSDocFileFormat::LFOLVL> rgLfoLvl;

			for (size_t j = 0; j < oXmlNum.LevelOverrides->size(); ++j)
			{
				const OOX::Numbering::LevelOverride& levelOverride = oXmlNum.LevelOverrides->operator[](j);
				AVSDocFileFormat::LVL lvl;

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
					iStartAt	=	*levelOverride.StartOverride;
					fStartAt	=	true;
				}

				rgLfoLvl.push_back( AVSDocFileFormat::LFOLVL(iStartAt, *levelOverride.Ilvl, fStartAt, AVSDocFileFormat::grfhic(), bHaveLVL ? &lvl : NULL));
			}

			int NumId = idLsidMap[oXmlNum.AbstractNumId];

			lfos.push_back(AVSDocFileFormat::LFO(NumId, rgLfoLvl.size(), AVSDocFileFormat::Constants::lfoFieldNotUsed00, AVSDocFileFormat::grfhic()));
			lfoDatas.push_back(AVSDocFileFormat::LFOData(0xFFFFFFFF, rgLfoLvl));
		}

		return AVSDocFileFormat::PlfLfo(lfos, lfoDatas);
	}

	const AVSDocFileFormat::Xst CFileTransformer::ConvertLvlText (const string& strLvlText, AVSDocFileFormat::Constants::MSONFC oMsoNfc) 
	{
		std::wstring wstr(strLvlText.size(), 0);
		utf8_decode(strLvlText.begin(), strLvlText.end(), wstr.begin());

		if (wstr == std::wstring(L"%1"))
			return AVSDocFileFormat::Xst(true);

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
		if (AVSDocFileFormat::Constants::msonfcBullet == oMsoNfc)
			cch = 0x0001;

		return AVSDocFileFormat::Xst(docPlaceHolderValue.c_str(), cch);
	}

	int CFileTransformer::FindAbstractNumIdWithStyleRef (const OOX::Numbering& oXmlNumbering, const std::string& refLink)	
	{
		const OOX::Styles::Style& oStyle = inputFolder.find<OOX::Document>().find<OOX::Styles>().GetStyleById(refLink);
		if (oStyle.ParagraphProperty.is_init())
		{
			if (oStyle.ParagraphProperty->NumPr.is_init())
			{
				if (oStyle.ParagraphProperty->NumPr->NumId.is_init())
				{
					int nInd = oStyle.ParagraphProperty->NumPr->NumId;
					size_t length = oXmlNumbering.Nums->size();
					for (size_t i = 0; i < length; ++i)
					{
						if (nInd == oXmlNumbering.Nums->operator [](i).NumId)
						{
							return oXmlNumbering.Nums->operator [](i).AbstractNumId;
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
	void CFileTransformer::ConvertStyleSheet (const OOX::Styles& oStyleSheet)
	{
		vector<AVSDocFileFormat::LSD> mpstiilsd;

		//if ( _styleSheet.LattentStyles.is_init() )
		mpstiilsd = ConvertLatentStyles( /**_styleSheet.LattentStyles*/ );

		vector<AVSDocFileFormat::LPStd> rglpstd = ConvertStyleDefinitions (*oStyleSheet.Named);

		short ftcAsci	= 0;
		short ftcFE		= 0;
		short ftcOther	= 0;

		if (oStyleSheet.Default->RunProperty->rFonts.is_init() )
		{
			if (oStyleSheet.Default->RunProperty->rFonts->ascii.is_init() )
				ftcAsci		=	m_mapFontTableMap [oStyleSheet.Default->RunProperty->rFonts->ascii];

			if (oStyleSheet.Default->RunProperty->rFonts->Cs.is_init() )
				ftcFE		=	m_mapFontTableMap [oStyleSheet.Default->RunProperty->rFonts->Cs];

			if (oStyleSheet.Default->RunProperty->rFonts->hAnsi.is_init() )
				ftcOther	=	m_mapFontTableMap [oStyleSheet.Default->RunProperty->rFonts->hAnsi];
		}

		AVSDocFileFormat::Stshif stshif (rglpstd.size(), true, mpstiilsd.size(), ftcAsci, ftcFE, ftcOther);
		AVSDocFileFormat::StshiLsd stshiLsd (mpstiilsd);

		// MUST be ignored.  
		AVSDocFileFormat::LPStshiGrpPrl grpprlChpStandard (ConvertRunProperties(*oStyleSheet.Default->RunProperty));
		AVSDocFileFormat::LPStshiGrpPrl grpprlPapStandard (ConvertParagraphProperties(*oStyleSheet.Default->ParagraphProperty));
		AVSDocFileFormat::STSHIB stshib (grpprlChpStandard, grpprlPapStandard);

		AVSDocFileFormat::LPStshi lpStshi (AVSDocFileFormat::STSHI(stshif, 0, stshiLsd, stshib));

		m_pDOCFile->SetStyleSheet (AVSDocFileFormat::STSH(lpStshi, rglpstd));
	}

	vector<AVSDocFileFormat::LPStd> CFileTransformer::ConvertStyleDefinitions (const vector<OOX::Styles::Style>& arrStyles)
	{
		vector<AVSDocFileFormat::LPStd> styleDefinitions(15);
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

			AVSDocFileFormat::Constants::StyleType styleType;
			map<string, AVSDocFileFormat::Constants::StyleType>::const_iterator findResult = styleTypeMap.find( *oXmlStyle.Type );  
			if ( findResult != styleTypeMap.end() )
				styleType = findResult->second;
			else
				styleType = AVSDocFileFormat::Constants::styleTypeCharacter;

			unsigned short istdBase = 0x0FFF;
			unsigned short istdNext = 0x0000;
			AVSDocFileFormat::StdfPost2000 StdfPost2000OrNone;

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
				StdfPost2000OrNone = AVSDocFileFormat::StdfPost2000( (unsigned short)m_mapStyleSheetMap[*oXmlStyle.Link], false, 0, (unsigned short)(*oXmlStyle.UiPriority) );
			}

			AVSDocFileFormat::GRFSTD grfstd( false, false, false, false, false, *oXmlStyle.SemiHidden, false, *oXmlStyle.UnhideWhenUsed, *oXmlStyle.QFormat ); 
			AVSDocFileFormat::StdfBase stdfBase( sti, styleType, istdBase, istdNext, grfstd );

			std::wstring styleName( ( oXmlStyle.name->size() ), 0 );
			utf8_decode( oXmlStyle.name->begin(), oXmlStyle.name->end(), styleName.begin() );

			AVSDocFileFormat::LPUpxPapx lPUpxPapx;
			AVSDocFileFormat::LPUpxChpx lPUpxChpx;
			AVSDocFileFormat::LPUpxTapx lPUpxTapx;

			CXmlPropertyReader oXmlReader;

			if (oXmlStyle.ParagraphProperty.is_init())
				lPUpxPapx	=	AVSDocFileFormat::LPUpxPapx (AVSDocFileFormat::UpxPapx(istd, oXmlReader.GetParagraphStyleProperties (ConvertParagraphProperties(*oXmlStyle.ParagraphProperty)) ));

			if (oXmlStyle.RunProperty.is_init())
				lPUpxChpx	=	AVSDocFileFormat::LPUpxChpx( AVSDocFileFormat::UpxChpx (oXmlReader.GetRunStyleProperties( ConvertRunProperties(*oXmlStyle.RunProperty)) ));    

			if (oXmlStyle.tblPr.is_init())
				lPUpxTapx	=	AVSDocFileFormat::LPUpxTapx(AVSDocFileFormat::UpxTapx(oXmlReader.GetTableStyleProperties( ConvertTableProperties(*oXmlStyle.tblPr)) ));

			AVSDocFileFormat::GrLPUpxSw grLPUpxSw (styleType, lPUpxPapx, lPUpxChpx, lPUpxTapx);
			AVSDocFileFormat::LPStd lPStd (AVSDocFileFormat::STD(AVSDocFileFormat::Stdf(stdfBase, &StdfPost2000OrNone), 
				AVSDocFileFormat::Xstz(AVSDocFileFormat::Xst(styleName.c_str())), grLPUpxSw));
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

	vector<AVSDocFileFormat::LSD> CFileTransformer::ConvertLatentStyles(/*const OOX::Styles::LattentStyles &latentStyles*/)
	{
		vector<AVSDocFileFormat::LSD> latentStylesDatas;

		/*bool fLocked = ( latentStyles.DefLockedState == 0 ) ? ( false ) : ( true );

		std::vector<OOX::Styles::LsdException>::const_iterator iter = (*latentStyles.LsdExceptions).begin();
		std::vector<OOX::Styles::LsdException>::const_iterator end = (*latentStyles.LsdExceptions).end();

		for (;iter != end; ++iter)	//	(*iter)
		{			

		bool fSemiHidden = false;
		bool fUnhideWhenUsed = false;
		bool fQFormat = false;
		unsigned short iPriority = 0;

		if ( (*iter).SemiHidden.is_init() )
		{
		fSemiHidden = ( (*iter).SemiHidden == 0 ) ? ( false ) : ( true );
		}

		if ( (*iter).UnhideWhenUsed.is_init() )
		{
		fUnhideWhenUsed = ( (*iter).UnhideWhenUsed == 0 ) ? ( false ) : ( true );
		}

		if ( (*iter).QFormat.is_init() )
		{
		fQFormat = ( (*iter).QFormat == 0 ) ? ( false ) : ( true );
		}

		if ( (*iter).UiPriority.is_init() )
		{
		iPriority = (unsigned short)(*iter).UiPriority;
		}

		latentStylesDatas.push_back( AVSDocFileFormat::LSD( fLocked, fSemiHidden, fUnhideWhenUsed, fQFormat, iPriority ) );
		}*/

		for ( unsigned int i = 0; i < ( sizeof(AVSDocFileFormat::LatentStylesTemplate) / sizeof(AVSDocFileFormat::LatentStylesTemplate[0]) ); i++ )
		{
			latentStylesDatas.push_back( AVSDocFileFormat::LSD( AVSDocFileFormat::LatentStylesTemplate[i] ) );
		}

		return latentStylesDatas;
	}


	std::string CFileTransformer::GetStyleID (const OOX::Logic::Paragraph& oXmlParagraph) 
	{
		std::string strStyleID;

		if (oXmlParagraph.Property.is_init() && oXmlParagraph.Property->PStyle.is_init())
		{
			strStyleID								=	(*oXmlParagraph.Property->PStyle);
		}
		else
		{
			const OOX::Styles::Style defaultStyle	=	inputFolder.find<OOX::Document>().find<OOX::Styles>().GetDefaultStyle ("paragraph");
			strStyleID								=	(*defaultStyle.StyleId);

			if (0 == strStyleID.length())
			{
				const OOX::Styles::Style oStyle		=	inputFolder.find<OOX::Document>().find<OOX::Styles>().GetStyleWithTypeAndName ("paragraph", "Normal");
				strStyleID							=	(*oStyle.StyleId);
			}
		}

		return strStyleID;
	}
}

namespace DOCXTODOC
{
	// настройки страниц документа
	AVSDocFileFormat::SectionProperties CFileTransformer::ConvertSectionProperties (const OOX::Logic::SectorProperty& docxSectionProperties)
	{
		PrlList docSectionProperties;

		unsigned short SXaPage = *docxSectionProperties.PageSize->Width;
		unsigned short SYaPage = *docxSectionProperties.PageSize->Height;

		docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSXaPage, (byte*)&SXaPage ) );
		docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSYaPage, (byte*)&SYaPage ) );

		if ( docxSectionProperties.Type.is_init() )
		{
			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSBkc, (byte*)&(sectionBreakTypeMap[*docxSectionProperties.Type]) ) );
		}

		if ( docxSectionProperties.PageSize->Orient.is_init() )
		{
			byte SBOrientation = (byte)AVSDocFileFormat::Constants::dmOrientPortrait;

			if ( *docxSectionProperties.PageSize->Orient == string( "portrait" ) )
			{
				SBOrientation = (byte)AVSDocFileFormat::Constants::dmOrientPortrait;  
			}
			else if ( *docxSectionProperties.PageSize->Orient == string( "landscape" ) )
			{
				SBOrientation = (byte)AVSDocFileFormat::Constants::dmOrientLandscape;
			}

			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSBOrientation, (byte*)&SBOrientation ) );
		}

		unsigned short SDxaLeft = AVSDocFileFormat::XAS_nonNeg( *docxSectionProperties.PageMargin->Left );
		unsigned short SDxaRight = AVSDocFileFormat::XAS_nonNeg( *docxSectionProperties.PageMargin->Right );
		short SDyaTop = AVSDocFileFormat::YAS( *docxSectionProperties.PageMargin->Top );
		short SDyaBottom = AVSDocFileFormat::YAS( *docxSectionProperties.PageMargin->Bottom );

		docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaLeft, (byte*)&SDxaLeft ) );
		docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaRight, (byte*)&SDxaRight ) );
		docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaTop, (byte*)&SDyaTop ) );
		docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaBottom, (byte*)&SDyaBottom ) );

		if ( docxSectionProperties.PageMargin->Gutter.is_init() )
		{
			unsigned short SDzaGutter = (unsigned short)(*docxSectionProperties.PageMargin->Gutter);

			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDzaGutter, (byte*)&SDzaGutter ) );
		}

		if ( docxSectionProperties.PageMargin->Header.is_init() )
		{ 
			unsigned short SDyaHdrTop = AVSDocFileFormat::YAS_nonNeg(*docxSectionProperties.PageMargin->Header);

			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaHdrTop, (byte*)&SDyaHdrTop ) );
		}

		if ( docxSectionProperties.PageMargin->Footer.is_init() )
		{ 
			unsigned short SDyaHdrBottom = AVSDocFileFormat::YAS_nonNeg(*docxSectionProperties.PageMargin->Footer);

			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaHdrBottom, (byte*)&SDyaHdrBottom ) );
		}

		if ( docxSectionProperties.Columns.is_init() )
		{
			if ( docxSectionProperties.Columns->Num.is_init() )
			{
				unsigned short SCcolumns = ( *docxSectionProperties.Columns->Num - 1 );

				docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSCcolumns, (byte*)&SCcolumns ) );
			}

			if ( docxSectionProperties.Columns->Space.is_init() )
			{
				unsigned short SDxaColumns = (unsigned short)AVSDocFileFormat::XAS_nonNeg( (unsigned short)(*docxSectionProperties.Columns->Space) );

				docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaColumns, (byte*)&SDxaColumns ) );
			}

			byte index = 0;

			size_t count = (*docxSectionProperties.Columns->Items).size();
			const std::vector<OOX::Logic::Column>& items = (*docxSectionProperties.Columns->Items);

			for (size_t i = 0; i < count; ++i)	
			{			
				AVSDocFileFormat::SDxaColWidthOperand SDxaColWidth(index, AVSDocFileFormat::XAS_nonNeg( (unsigned short)(*(items[i]).Width)));

				docSectionProperties.push_back( AVSDocFileFormat::Prl((short)DocFileFormat::sprmSDxaColWidth, (byte*)SDxaColWidth));

				if (items[i].Space.is_init())
				{
					//TODO: 

					AVSDocFileFormat::SDxaColSpacingOperand SDxaColSpacing( index, AVSDocFileFormat::XAS_nonNeg( (unsigned short)(*items[i].Space) ) );

					docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDxaColSpacing, (byte*)SDxaColSpacing ) );
				}

				++index;
			}
		}

		if ( docxSectionProperties.DocumentGrid.is_init() )
		{
			if ( docxSectionProperties.DocumentGrid->LinePitch.is_init() )
			{
				short SDyaLinePitch = AVSDocFileFormat::YAS( *docxSectionProperties.DocumentGrid->LinePitch );

				docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDyaLinePitch, (byte*)(&SDyaLinePitch) ) );
			}

			if ( docxSectionProperties.DocumentGrid->CharSpace.is_init() )
			{
				int SDxtCharSpace = *docxSectionProperties.DocumentGrid->CharSpace;

				docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSDxtCharSpace, (byte*)(&SDxtCharSpace) ) );
			}

			if ( docxSectionProperties.DocumentGrid->Type.is_init() )
			{
				unsigned short SClm = (unsigned short)AVSDocFileFormat::Constants::clmUseDefault;

				if ( *docxSectionProperties.DocumentGrid->Type == string( "Default" ) )
				{
					SClm = (unsigned short)AVSDocFileFormat::Constants::clmUseDefault;
				}
				else if ( *docxSectionProperties.DocumentGrid->Type == string( "linesAndChars" ) )
				{
					SClm = (unsigned short)AVSDocFileFormat::Constants::clmCharsAndLines;
				}
				else if ( *docxSectionProperties.DocumentGrid->Type == string( "lines" ) )
				{
					SClm = (unsigned short)AVSDocFileFormat::Constants::clmLinesOnly;
				}
				else if ( *docxSectionProperties.DocumentGrid->Type == string( "snapToChars" ) )
				{
					SClm = (unsigned short)AVSDocFileFormat::Constants::clmEnforceGrid;
				}

				docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSClm, (byte*)(&SClm) ) );
			}
		}

		if ( docxSectionProperties.FootNoteProperty.is_init() )
		{
			unsigned short sNfcFtnRef = (unsigned short)(this->numFmtMap[docxSectionProperties.FootNoteProperty->NumFormat->ToString()]);
			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSNfcFtnRef, (byte*)(&sNfcFtnRef) ) );

			unsigned short sNFtn = (unsigned short)(docxSectionProperties.FootNoteProperty->NumStart);
			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSNFtn, (byte*)(&sNFtn) ) );
		}

		if ( docxSectionProperties.EndNoteProperty.is_init() )
		{
			unsigned short sNfcEdnRef = (unsigned short)(this->numFmtMap[docxSectionProperties.EndNoteProperty->NumFormat->ToString()]);
			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSNfcEdnRef, (byte*)(&sNfcEdnRef) ) );

			unsigned short sNEdn = (unsigned short)(docxSectionProperties.EndNoteProperty->NumStart);
			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSNEdn, (byte*)(&sNEdn) ) );
		}

		AddLineNumberingSettings (docxSectionProperties, docSectionProperties);		//	<w:lnNumType>

		if (*docxSectionProperties.TitlePage)
		{
			Bool8 SFTitlePage = ( ( *docxSectionProperties.TitlePage ) ? ( 1 ) : ( 0 ) );
			docSectionProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmSFTitlePage, &SFTitlePage ) );
		}

		AVSDocFileFormat::Sepx sepx (docSectionProperties);
		AVSDocFileFormat::SectionProperties oSectionProperties (sepx);

		for ( vector<OOX::Logic::HeaderReference>::const_iterator headerReferencesIter = docxSectionProperties.Headers->begin(); 
			headerReferencesIter != docxSectionProperties.Headers->end();
			++headerReferencesIter )
		{
			OOX::Header* pHeader = dynamic_cast<OOX::Header*>(inputFolder.find<OOX::Document>()[headerReferencesIter->rId].get()); 

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
			OOX::Footer* pFooter = dynamic_cast<OOX::Footer*>(inputFolder.find<OOX::Document>()[footerReferencesIter->rId].get());  
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

	bool CFileTransformer::AddLineNumberingSettings (const OOX::Logic::SectorProperty& oSection, PrlList& arrSettings)
	{
		//	Line Numbering Restart Setting

		if (oSection.LnNumType->countBy.is_init())
		{
			//	Line Number Increments to Display

			short countBy			=	(unsigned short)(oSection.LnNumType->countBy);
			arrSettings.push_back (AVSDocFileFormat::Prl ((short)DocFileFormat::sprmSNLnnMod, (byte*)(&countBy) ) );		

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

			arrSettings.push_back (AVSDocFileFormat::Prl ((short)DocFileFormat::sprmSLnc, (byte*)(&mode) ) );

			//	Line Numbering Starting Value

			unsigned short start	=	0;
			if (oSection.LnNumType->start.is_init())
				start				=	(unsigned short)(oSection.LnNumType->start);

			arrSettings.push_back (AVSDocFileFormat::Prl ((short)DocFileFormat::sprmSLnnMin, (byte*)(&start) ) );

			return true;
		}

		return false;
	}

	// верхний колонтитул
	AVSDocFileFormat::Header CFileTransformer::ConvertHeader (const OOX::Header& oXmlDOCXHeader) 
	{
		m_oOArtBuilder.SetLocation (HEADER_DOCUMENT);

		AVSDocFileFormat::Header oMSDocHeader;

		for (size_t i = 0; i < oXmlDOCXHeader.Items->size(); ++i)
		{
			const OOX::Logic::TextItem& oItem					=	oXmlDOCXHeader.Items->operator [](i);

			if (oItem.is<OOX::Logic::Paragraph>())
			{
				const OOX::Logic::Paragraph& oMSDocXParagraph	=	oItem.as<OOX::Logic::Paragraph>();
				if (ValidParagraph(oMSDocXParagraph))
				{
					AVSDocFileFormat::Paragraph oMSDocParagraph	=	ConvertParagraph<OOX::Header>(oMSDocXParagraph);
					oMSDocHeader.AddTextItem (oMSDocParagraph);
				}
			}
			else if (oItem.is<OOX::Logic::Table>())
			{
				AVSDocFileFormat::Table oMSDocTable				=	CreateTable<OOX::Header> (oItem.as<OOX::Logic::Table>());
				oMSDocHeader.AddTextItem (oMSDocTable);
			}
			else if (oItem.is<OOX::Logic::Sdt>())
			{
				const OOX::Logic::SdtContent& oStdC				=	oItem.as<OOX::Logic::Sdt>().Content;

				for (size_t j = 0; j < oStdC.Items->size(); ++j)
				{
					const OOX::Logic::TextItem& oStdItem		=	oStdC.Items->operator [](j);

					if (oStdItem.is<OOX::Logic::Paragraph>())
					{
						const OOX::Logic::Paragraph& oMSDocXParagraph	=	oStdItem.as<OOX::Logic::Paragraph>();
						if (ValidParagraph(oMSDocXParagraph))
						{
							AVSDocFileFormat::Paragraph oMSDocParagraph	=	ConvertParagraph<OOX::Header>(oMSDocXParagraph);
							oMSDocHeader.AddTextItem (oMSDocParagraph);
						}
					}
					else if (oStdItem.is<OOX::Logic::Table>())
					{
						AVSDocFileFormat::Table oMSDocTable		=	CreateTable<OOX::Header> (oStdItem.as<OOX::Logic::Table>());
						oMSDocHeader.AddTextItem (oMSDocTable);
					}
				}
			}
		}

		m_oOArtBuilder.SetLocation (MAIN_DOCUMENT);

#ifdef _DEBUG		
		// DOCXDOCUTILS::DebugStrPrint (_T("Header : "), oMSDocHeader.GetAllText());
#endif
		return oMSDocHeader;
	}

	// нижний колонтитул
	AVSDocFileFormat::Footer CFileTransformer::ConvertFooter (const OOX::Footer& oXmlDOCXFooter) 
	{
		m_oOArtBuilder.SetLocation (HEADER_DOCUMENT);

		AVSDocFileFormat::Footer oMSDocFooter;

		for (size_t i = 0; i < oXmlDOCXFooter.Items->size(); ++i)
		{
			const OOX::Logic::TextItem& oItem					=	oXmlDOCXFooter.Items->operator [](i);

			if (oItem.is<OOX::Logic::Paragraph>())
			{
				const OOX::Logic::Paragraph& oMSDocXParagraph	=	oItem.as<OOX::Logic::Paragraph>();
				if (ValidParagraph(oMSDocXParagraph))
				{
					AVSDocFileFormat::Paragraph oMSDocParagraph	=	ConvertParagraph<OOX::Footer>(oMSDocXParagraph);
					oMSDocFooter.AddTextItem (oMSDocParagraph);
				}
			}
			else if (oItem.is<OOX::Logic::Table>())
			{
				AVSDocFileFormat::Table oMSDocTable				=	CreateTable<OOX::Footer> (oItem.as<OOX::Logic::Table>());
				oMSDocFooter.AddTextItem (oMSDocTable);
			}
			else if (oItem.is<OOX::Logic::Sdt>())
			{
				const OOX::Logic::SdtContent& oStdC				=	oItem.as<OOX::Logic::Sdt>().Content;

				for (size_t j = 0; j < oStdC.Items->size(); ++j)
				{
					const OOX::Logic::TextItem& oStdItem		=	oStdC.Items->operator [](j);

					if (oStdItem.is<OOX::Logic::Paragraph>())
					{
						const OOX::Logic::Paragraph& oMSDocXParagraph	=	oStdItem.as<OOX::Logic::Paragraph>();
						if (ValidParagraph(oMSDocXParagraph))
						{
							AVSDocFileFormat::Paragraph oMSDocParagraph	=	ConvertParagraph<OOX::Footer>(oMSDocXParagraph);
							oMSDocFooter.AddTextItem (oMSDocParagraph);
						}
					}
					else if (oStdItem.is<OOX::Logic::Table>())
					{
						AVSDocFileFormat::Table oMSDocTable		=	CreateTable<OOX::Footer> (oStdItem.as<OOX::Logic::Table>());
						oMSDocFooter.AddTextItem (oMSDocTable);
					}
				}
			}
		}

		m_oOArtBuilder.SetLocation (MAIN_DOCUMENT);

#ifdef _DEBUG		
		// DOCXDOCUTILS::DebugStrPrint (_T("Footer : "), oMSDocFooter.GetAllText());
#endif

		return oMSDocFooter;
	}
}

namespace DOCXTODOC	//	TABLE
{
	template<class T> AVSDocFileFormat::Table CFileTransformer::CreateTable (const OOX::Logic::Table& oXmlTable) 
	{
		static unsigned int tableDepth = 1;

		AVSDocFileFormat::Table table;

		OOX::Styles::Style oTableStyle;
		if (oXmlTable.tblPr->Style.is_init())
		{			
			oTableStyle	= inputFolder.find<OOX::Document>().find<OOX::Styles>().GetStyleById(*oXmlTable.tblPr->Style);
		}

		DOCXDOCUTILS::CTblBorders oTblBorders (oXmlTable, oTableStyle);

		int nYC		=	(int)oXmlTable.Rows->size();
		for (int nY = 0; nY < nYC; ++nY)
		{
			const OOX::Logic::TableRow& oXmlTableRow = oXmlTable.Rows->operator [](nY);

			AVSDocFileFormat::TableRow tableRow (tableDepth);

			vector<AVSDocFileFormat::XAS> xass;
			vector<AVSDocFileFormat::TC80> tc80s;

			unsigned int cellIndex = 0;

			int nXC		=	(int)oXmlTableRow.Cells->size();
			for (int nX = 0; nX < nXC; ++nX)
			{
				const OOX::Logic::TableCell& oXmlCell	=	oXmlTableRow.Cells->operator[](nX);

				AVSDocFileFormat::TableCell tableCell(tableDepth);

				AVSDocFileFormat::Constants::VerticalMergeFlag vmf = ConvertTableVerticalMergeFlag( oXmlCell.Properties->VMerge );

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

				AVSDocFileFormat::TC80 tc80( AVSDocFileFormat::TCGRF( 
					AVSDocFileFormat::Constants::horzMergeNotMerged, 
					AVSDocFileFormat::Constants::grpfTFlrtb, vmf, 
					AVSDocFileFormat::Constants::vaTop, 
					AVSDocFileFormat::Constants::ftsDxa, false, false, false ),
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
						AVSDocFileFormat::Paragraph oDocParagraph		=	ConvertParagraph<T>(oDocxParagraph);
						tableCell.AddTextItem (oDocParagraph);

						//AVSDocFileFormat::Paragraph oDocParagraph;	//	 ONLY FOR TEST
						//tableCell.AddTextItem (oDocParagraph);		//	 ONLY FOR TEST
					}
					else if (items[i].is<OOX::Logic::Table>())
					{
						tableDepth++;
						AVSDocFileFormat::Table docTable = CreateTable<T>(items[i].as<OOX::Logic::Table>());
						tableDepth--;
						tableCell.AddTextItem(docTable);
					}
				}				

				tc80s.push_back(tc80);

				tableRow.AddCell(tableCell);
				++cellIndex;
			}

			xass = BuildXASs( oXmlTable, *oXmlTableRow.Cells );

			AVSDocFileFormat::TDefTableOperand tdto( oXmlTableRow.Cells->size(), xass, tc80s );

			tableRow.AddProperty( (short)DocFileFormat::sprmTDefTable, tdto );

			tableRow.AddProperties(ConvertTableProperties(*oXmlTable.tblPr, oXmlTableRow.Cells->size()));

			if (oXmlTableRow.Properties.is_init())
			{
				tableRow.AddProperties(ConvertTableRowProperties(nY,*oXmlTableRow.Properties ) );
			}

			const vector<AVSDocFileFormat::TableBrcOperand>& oBrcs		=	oTblBorders.GetSpecificationBorders ();
			const vector<AVSDocFileFormat::TableBrc80Operand>& oBrc80s	=	oTblBorders.GetDescriptBorders ();
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

	PrlList CFileTransformer::ConvertTableProperties (const OOX::Logic::TableProperty& docxTableProperties, unsigned int cellsCount) 
	{
		PrlList docTableProperties;

		DOCXDOCUTILS::CTblBorders oBorder;

		AVSDocFileFormat::TableBordersOperand80 tableBordersOperand80;
		AVSDocFileFormat::TableBordersOperand tableBordersOperand;

		if ( docxTableProperties.Style.is_init() )
		{
			docTableProperties = GetTablePropertiesFromStyleHierarchy( docxTableProperties.Style );

			short istd = m_mapStyleSheetMap[*docxTableProperties.Style];
			docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTIstd, (byte*)(&istd) ) );

			if ( istd < (short)m_pDOCFile->GetStyleSheet().Count() )
			{
				PrlList styleTableProperties = m_pDOCFile->GetStyleSheet()[istd].GetProperties();

				for (PrlList::const_iterator iter = styleTableProperties.begin(); iter != styleTableProperties.end(); ++iter)
				{
					docTableProperties.push_back( *iter );

					switch ( iter->GetSprmCode() )
					{
					case ( (unsigned short)DocFileFormat::sprmTTableBorders80 ):
						{
							tableBordersOperand80 = AVSDocFileFormat::TableBordersOperand80( *iter );
						}
						break;

					case ( (unsigned short)DocFileFormat::sprmTTableBorders ):
						{
							tableBordersOperand = AVSDocFileFormat::TableBordersOperand( *iter );
						}
						break;
					}
				}
			}
		}

		if ( docxTableProperties.Look.is_init() )
		{
			AVSDocFileFormat::TLP tlp( AVSDocFileFormat::Constants::itlNone, AVSDocFileFormat::Fatl( HexString2Int( *docxTableProperties.Look ) ) );

			docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTTlp, tlp ) );
		}

		if ( docxTableProperties.Width.is_init() )
		{
			AVSDocFileFormat::FtsWWidth_Table ftsWWidth_Table (DOCXDOCUTILS::TableWidthUnitsFromString (*docxTableProperties.Width->Type),*docxTableProperties.Width->Width );
			docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTTableWidth, ftsWWidth_Table ) );
		}

		byte bAutoFit = 0x01;
		if ( docxTableProperties.Layout.is_init() && docxTableProperties.Layout == "fixed")
			bAutoFit = 0x00;
		docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTFAutofit, (byte*)&bAutoFit ) );

		if ( docxTableProperties.Ind.is_init() )
		{
			AVSDocFileFormat::FtsWWidth_Indent tWidthIndent (DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.Ind->Type), (short)docxTableProperties.Ind->Width);
			docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTWidthIndent, (byte*)tWidthIndent ) );
		}

		map<std::string, byte>& oBrcMap	=	oBorder.GetBrcMap ();

		if (docxTableProperties.tblBorders.is_init())
		{
			if (docxTableProperties.tblBorders->top.is_init())
			{
				AVSDocFileFormat::Brc80MayBeNil Brc80MayBeNilTop =  AVSDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Value],
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetTopBorder(Brc80MayBeNilTop);

				std::string strColor	=	docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				AVSDocFileFormat::Brc brcTop = AVSDocFileFormat::Brc(AVSDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->top.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetTopBorder(brcTop);
			}

			if ( docxTableProperties.tblBorders->bottom.is_init() )
			{
				AVSDocFileFormat::Brc80MayBeNil Brc80MayBeNilBottom = AVSDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetBottomBorder(Brc80MayBeNilBottom);

				std::string strColor	=	docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				AVSDocFileFormat::Brc brcBottom = AVSDocFileFormat::Brc(AVSDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->bottom.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetBottomBorder( brcBottom );
			}

			if ( docxTableProperties.tblBorders->left.is_init() )
			{
				AVSDocFileFormat::Brc80MayBeNil Brc80MayBeNilLeft = AVSDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetLeftBorder( Brc80MayBeNilLeft );

				std::string strColor	=	docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				AVSDocFileFormat::Brc brcLeft = AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->left.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetLeftBorder( brcLeft );
			}

			if ( docxTableProperties.tblBorders->right.is_init() )
			{
				AVSDocFileFormat::Brc80MayBeNil Brc80MayBeNilRight = AVSDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetRightBorder( Brc80MayBeNilRight );

				std::string strColor	=	docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				AVSDocFileFormat::Brc brcRight = AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->right.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetRightBorder( brcRight );
			}

			if ( docxTableProperties.tblBorders->insideH.is_init() )
			{
				AVSDocFileFormat::Brc80MayBeNil Brc80MayBeNilInsideH = AVSDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetHorizontalInsideBorder( Brc80MayBeNilInsideH );

				std::string strColor	=	docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				AVSDocFileFormat::Brc brcInsideH = AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF(HexString2Int(strColor)),
					docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->insideH.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetHorizontalInsideBorder( brcInsideH );
			}

			if ( docxTableProperties.tblBorders->insideV.is_init() )
			{
				AVSDocFileFormat::Brc80MayBeNil Brc80MayBeNilInsideV = AVSDocFileFormat::Brc80MayBeNil( docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Value], 
					DOCXDOCUTILS::ColorToIco (docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Color.get_value_or_default()), 
					docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand80.SetVerticalInsideBorder( Brc80MayBeNilInsideV );

				std::string strColor	=	docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Color.get_value_or_default().ToString();
				if (std::string("auto") == strColor)
					strColor			=	std::string ("000000");

				AVSDocFileFormat::Brc brcInsideV = AVSDocFileFormat::Brc( AVSDocFileFormat::COLORREF( HexString2Int(strColor) ),
					docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Sz.get_value_or_default(), 
					oBrcMap[docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Value], 
					docxTableProperties.tblBorders->insideV.get_value_or_default().Bdr->Space.get_value_or_default(), 
					false, 
					false );

				tableBordersOperand.SetVerticalInsideBorder( brcInsideV );
			}

			docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTTableBorders80, (byte*)tableBordersOperand80 ) );
			docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTTableBorders, (byte*)tableBordersOperand ) );
		}

		if ( docxTableProperties.CellMar.is_init() )
		{
			if ( docxTableProperties.CellMar->Top.is_init() )
			{
				AVSDocFileFormat::CSSAOperand cssaOperandTop( AVSDocFileFormat::CSSA( AVSDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					AVSDocFileFormat::Constants::fbrcTop,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Top->Margin->Type),
					docxTableProperties.CellMar->Top->Margin->W ) );

				docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)cssaOperandTop ) );
			}

			if ( docxTableProperties.CellMar->Left.is_init() )
			{
				AVSDocFileFormat::CSSAOperand cssaOperandLeft( AVSDocFileFormat::CSSA( AVSDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					AVSDocFileFormat::Constants::fbrcLeft,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Left->Margin->Type),
					docxTableProperties.CellMar->Left->Margin->W ) );

				docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)cssaOperandLeft ) );
			}

			if ( docxTableProperties.CellMar->Bottom.is_init() )
			{
				AVSDocFileFormat::CSSAOperand cssaOperandBottom( AVSDocFileFormat::CSSA( AVSDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					AVSDocFileFormat::Constants::fbrcBottom,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Bottom->Margin->Type),
					docxTableProperties.CellMar->Bottom->Margin->W ) );

				docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)cssaOperandBottom ) );
			}

			if ( docxTableProperties.CellMar->Right.is_init() )
			{
				AVSDocFileFormat::CSSAOperand cssaOperandRight( AVSDocFileFormat::CSSA( AVSDocFileFormat::ItcFirstLim( 0, cellsCount ), 
					AVSDocFileFormat::Constants::fbrcRight,
					DOCXDOCUTILS::TableWidthUnitsFromString (docxTableProperties.CellMar->Right->Margin->Type),
					docxTableProperties.CellMar->Right->Margin->W ) );

				docTableProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)cssaOperandRight ) );
			}
		}

		return docTableProperties;
	}

	vector<AVSDocFileFormat::XAS> CFileTransformer::BuildXASs(const OOX::Logic::Table& oXmlTable, const vector<OOX::Logic::TableCell>& tableCells) 
	{
		std::vector<AVSDocFileFormat::XAS> resultXASs;

		resultXASs.push_back( AVSDocFileFormat::XAS( 0 ) ); //!!!TODO!!!

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
					nCellWidth	=	AVSDocFormatUtils::gc_nZeroWidth;
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
					nCellWidth	=	AVSDocFormatUtils::gc_nZeroWidth;

				//if (0 == nSpanLineOff)
				--nXmlSpan;

				nSpanLineOff	+=	nXmlSpan;
			}

			resultXASs.push_back(AVSDocFileFormat::XAS(nCellWidth + resultXASs.back()));
		}

		return resultXASs;
	}

	//
	const PrlList CFileTransformer::ConvertTableRowProperties (int nY, const OOX::Logic::TableRowProperties& oXmlRow) 
	{
		PrlList oPrls;

		short tableRowHeight = 0;

		if ( oXmlRow.Height->Type.is_init() && ( *oXmlRow.Height->Type ==  "exact" ) )
		{
			tableRowHeight = AVSDocFileFormat::YAS( *oXmlRow.Height->Height * ( -1 ) );    
		}
		else
		{
			tableRowHeight = AVSDocFileFormat::YAS( *oXmlRow.Height->Height );
		}

		oPrls.push_back(AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTDyaRowHeight, (byte*)(&tableRowHeight)));

		return oPrls;
	}

	const PrlList CFileTransformer::ConvertTableRowCellsProperties (const OOX::Logic::Table& oXmlTable, int nY, int nCY, const vector<OOX::Logic::TableCell>& arrXmlCells, const OOX::Styles::Style& oStyle) 
	{
		PrlList arCellsPrls;

		DOCXDOCUTILS::CTblFill oTblFill (oXmlTable, oStyle);

		int nCX = arrXmlCells.size();

		for (int nX = 0; nX < nCX; ++nX)
		{
			const OOX::Logic::TableCell& oXmlCell	=	arrXmlCells[nX];

			oTblFill.AddCell (nX, nY, nCX-1, nCY-1, oXmlCell.Properties->Shading);

			AVSDocFileFormat::TableCellWidthOperand tableCellWidthOperand( AVSDocFileFormat::ItcFirstLim( nX, ( nX + 1 ) ), AVSDocFileFormat::FtsWWidth_TablePart( tableCellWidthMap[*oXmlCell.Properties->Width->Type], (unsigned short)(*oXmlCell.Properties->Width->Width) ) );
			arCellsPrls.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTCellWidth, tableCellWidthOperand ) );

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
				AVSDocFileFormat::Constants::VerticalAlign verticalAlign = AVSDocFileFormat::Constants::vaTop;

				if ( *oXmlCell.Properties->VAlign == "top" )
				{
					verticalAlign = AVSDocFileFormat::Constants::vaTop;
				}
				else if ( *oXmlCell.Properties->VAlign == "center" )
				{
					verticalAlign = AVSDocFileFormat::Constants::vaCenter;
				}
				else if ( *oXmlCell.Properties->VAlign == "bottom" )
				{
					verticalAlign = AVSDocFileFormat::Constants::vaBottom;
				}

				AVSDocFileFormat::CellRangeVertAlign cellRangeVertAlign( AVSDocFileFormat::ItcFirstLim( nX, ( nX + 1 ) ), verticalAlign );

				arCellsPrls.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmTVertAlign, cellRangeVertAlign ) );
			}
		}

		if (oTblFill.GetShd22().size())
		{
			AVSDocFileFormat::DefTableShdOperand defOperand(oTblFill.GetShd22());

			arCellsPrls.push_back(AVSDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShd,	defOperand));
			arCellsPrls.push_back(AVSDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShdRaw, defOperand));
		}

		if (oTblFill.GetShd44().size())
		{
			AVSDocFileFormat::DefTableShdOperand defOperand(oTblFill.GetShd44());

			arCellsPrls.push_back(AVSDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShd,	defOperand));
			arCellsPrls.push_back(AVSDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShdRaw, defOperand));
		}

		if (oTblFill.GetShd63().size())
		{
			AVSDocFileFormat::DefTableShdOperand defOperand(oTblFill.GetShd63());

			arCellsPrls.push_back(AVSDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShd,	defOperand));
			arCellsPrls.push_back(AVSDocFileFormat::Prl((short)DocFileFormat::sprmTDefTableShdRaw, defOperand));
		}

		return arCellsPrls;
	}
}

namespace DOCXTODOC	//	FONTS
{
	void CFileTransformer::AddInternalFont (const std::string& strFontName)
	{	
		// шрифт может не содержаться в таблице шрифтов, а определен прямо по ходу текста

		map<string, short>::const_iterator findResult = m_mapFontTableMap.find(strFontName);  
		if (findResult == m_mapFontTableMap.end())
		{
			m_mapFontTableMap.insert(pair<string, short>(strFontName, m_mapFontTableMap.size()));

			m_arrInternalFonts.push_back (strFontName);
		}
	}

	void CFileTransformer::ConvertFontTable (const OOX::FontTable& oXmlFontTable)
	{
		vector<AVSDocFileFormat::FFN> ffns;

		for (size_t i = 0; i < oXmlFontTable.Fonts->size(); ++i)
		{
			const OOX::FontTable::Font& oXmlFont = oXmlFontTable.Fonts->operator [](i);

			AVSDocFileFormat::FFID ffid (DOCXDOCUTILS::FontPitchFromString (*oXmlFont.Pitch), true,	DOCXDOCUTILS::FontFamilyFromString (*oXmlFont.Family));

			//!!!TODO!!!
			short wWeight = 400;

			FONTSIGNATURE fs;

			fs.fsCsb[0] = HexString2Int( oXmlFont.Csb0.get_value_or_default() );
			fs.fsCsb[1] = HexString2Int( oXmlFont.Csb1.get_value_or_default() );
			fs.fsUsb[0] = HexString2Int( oXmlFont.Usb0.get_value_or_default() );
			fs.fsUsb[1] = HexString2Int( oXmlFont.Usb1.get_value_or_default() );
			fs.fsUsb[2] = HexString2Int( oXmlFont.Usb2.get_value_or_default() );
			fs.fsUsb[3] = HexString2Int( oXmlFont.Usb3.get_value_or_default() );

			std::wstring strFontName;

			FormatUtils::GetSTLCollectionFromBytes<wstring>(&strFontName, (byte*)((*oXmlFont.Name).c_str()), (*oXmlFont.Name).size(), ENCODING_WINDOWS_1251); 

			AVSDocFileFormat::FFN ffn (ffid, wWeight, 
				DOCXDOCUTILS::FontCharsetFromString (oXmlFont.Charset.get_value_or_default()),
				ConvertPanose(oXmlFont.Panose1.get_value_or_default()), fs, strFontName);

			ffns.push_back(ffn);
		}

		for (size_t i = 0; i < m_arrInternalFonts.size(); ++i)
		{
			AVSDocFileFormat::FFID ffid (DOCXDOCUTILS::FontPitchFromString (std::string("")), true,	DOCXDOCUTILS::FontFamilyFromString (std::string("auto")));

			//!!!TODO!!!
			short wWeight = 400;

			FONTSIGNATURE fs;

			fs.fsCsb[0] = 0;
			fs.fsCsb[1] = 0;
			fs.fsUsb[0] = 0;
			fs.fsUsb[1] = 0;
			fs.fsUsb[2] = 0;
			fs.fsUsb[3] = 0;

			std::wstring strFontName;

			FormatUtils::GetSTLCollectionFromBytes<wstring>(&strFontName, (byte*)(m_arrInternalFonts[i].c_str()), m_arrInternalFonts[i].size(), ENCODING_WINDOWS_1251); 

			AVSDocFileFormat::FFN ffn (ffid, wWeight, 
				DOCXDOCUTILS::FontCharsetFromString (std::string("")),
				ConvertPanose(std::string("")), fs, strFontName);

			ffns.push_back(ffn);
		}

		m_pDOCFile->SetFontTable(AVSDocFileFormat::STTB<AVSDocFileFormat::FFN>(false, &ffns));
	}

	AVSDocFileFormat::PANOSE CFileTransformer::ConvertPanose (const std::string& strPanose)
	{
		if (20 == strPanose.size())
		{
			byte bFamilyType		=	HexString2Int ( std::string( ( strPanose.begin() ),			( strPanose.begin() + 2 ) ) );
			byte bSerifStyle		=	HexString2Int ( std::string( ( strPanose.begin() + 2 ),		( strPanose.begin() + 4 ) ) );
			byte bWeight			=	HexString2Int ( std::string( ( strPanose.begin() + 4 ),		( strPanose.begin() + 6 ) ) );
			byte bProportion		=	HexString2Int ( std::string( ( strPanose.begin() + 6 ),		( strPanose.begin() + 8 ) ) );
			byte bContrast			=	HexString2Int ( std::string( ( strPanose.begin() + 8 ),		( strPanose.begin() + 10 ) ) );
			byte bStrokeVariation	=	HexString2Int ( std::string( ( strPanose.begin() + 10 ),	( strPanose.begin() + 12 ) ) );
			byte bArmStyle			=	HexString2Int ( std::string( ( strPanose.begin() + 12 ),	( strPanose.begin() + 14 ) ) );
			byte bLetterform		=	HexString2Int ( std::string( ( strPanose.begin() + 14 ),	( strPanose.begin() + 16 ) ) );
			byte bMidline			=	HexString2Int ( std::string( ( strPanose.begin() + 16 ),	( strPanose.begin() + 18 ) ) );
			byte bHeight			=	HexString2Int ( std::string( ( strPanose.begin() + 18 ),	( strPanose.end() ) ) );

			return AVSDocFileFormat::PANOSE (bFamilyType, bSerifStyle, bWeight, bProportion, bContrast,	bStrokeVariation, bArmStyle, bLetterform, bMidline, bHeight);
		}

		return AVSDocFileFormat::PANOSE();
	}
}

namespace DOCXTODOC // run rule transform
{
	template<class T> AVSDocFileFormat::Run CFileTransformer::ConvertRun (const OOX::Logic::Run& oXml, const PrlList& styleDocRunProperties, std::wstring& strRunType) 
	{
		AVSDocFileFormat::Run oBinRun;
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

		oBinRun.AddProperties(ConvertRunProperties(inputFolder.find<OOX::Document>().find<OOX::Styles>().Default->RunProperty));

		if (oXml.Property.is_init())
		{
			if (oXml.Property->RStyle.is_init())
			{
				//!!!TODO!!!  
				//short istd = m_mapStyleSheetMap[*oXml.Property->RStyle];
				//oBinRun.AddProperty( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCIstd, (byte*)&istd ) );

				PrlList styleDocRunProperties = GetRunPropertiesFromStyleHierarchy(*oXml.Property->RStyle);
				oBinRun.AddOrReplaceProperties( styleDocRunProperties );
			}
			else
			{
				const OOX::Styles::Style defaultStyle = inputFolder.find<OOX::Document>().find<OOX::Styles>().GetDefaultStyle("character");
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
	template<class T> BOOL CFileTransformer::FldCharXmlRunTransform (const OOX::Logic::FldChar& oXml, AVSDocFileFormat::Run& oBinRun, std::wstring& strRunType)
	{
		std::wstring fldCharType	=	FormatUtils::UTF8Decode (*oXml.FldCharType);		
		if (fldCharType.empty())
			return FALSE;

		AVSDocFileFormat::FldChar oFldChar(fldCharType);

		strRunType					=	fldCharType;

		if (oFldChar.CharType() == AVSDocFileFormat::FldChar::FldCharSeparate)
		{
			oFldChar.SetFieldCharacterProperties((byte)AVSDocFileFormat::grffldEnd(true, true, true, true, true, true, true, true));
			m_bHaveSeparateFldChar	=	true;
		}

		if (oFldChar.CharType() == AVSDocFileFormat::FldChar::FldCharEnd)
		{
			oFldChar.SetFieldCharacterProperties((byte)AVSDocFileFormat::grffldEnd(false, false, true, true, false, false, false, m_bHaveSeparateFldChar));
			m_bHaveSeparateFldChar	=	false;
		}

		oBinRun.AddRunItem (oFldChar);

		return TRUE;
	}

	template<class T> BOOL CFileTransformer::TextXmlRunTransform (const OOX::Logic::Text& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		AVSDocFileFormat::Text oText(FormatUtils::UTF8Decode(oXml.toTxt()).c_str());
		oBinRun.AddRunItem (oText);

		return TRUE;
	}

	template<class T> BOOL CFileTransformer::InstrTextXmlRunTransform (const OOX::Logic::InstrText& oXml, AVSDocFileFormat::Run& oBinRun, std::wstring& strRunType)
	{
		std::wstring strText	=	FormatUtils::UTF8Decode(oXml.Text.get_value_or_default());

		std::wstring Source;
		std::wstring FieldCode	=	DOCXDOCUTILS::GetInstrText_FieldCode(strText, Source);
		if (FieldCode.empty())
			return FALSE;

		strRunType				=	FieldCode;

		oBinRun.AddRunItem(AVSDocFileFormat::Text(strText));

		return TRUE;
	}

	template<class T> BOOL CFileTransformer::DrawingXmlRunTransform (const OOX::Logic::Drawing& oXml, AVSDocFileFormat::Run& oBinRun) 
	{
		OOX::Image* pImage				=	GetImageDOCX <T, OOX::Logic::Drawing>(&oXml);
		if (pImage)
		{
			if (oXml.haveAnchor)
			{
				return	m_oOArtBuilder.BuildImageRun (*pImage, oXml, oBinRun);
			}

			std::wstring sFileName		=	pImage->filename().string();
			if (sFileName.length())
			{
				Unit<int, Dx> twWidth	=	*oXml.Inline->Extent->Size->Width;
				Unit<int, Dx> twHeight	=	*oXml.Inline->Extent->Size->Height;

				AVSDocFileFormat::Picture oPicture (sFileName, twWidth, twHeight);
				if (oPicture.IsValid())
				{
					oBinRun.AddRunItem (oPicture);
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	template<class T> BOOL CFileTransformer::PictXmlRunTransform (const OOX::Logic::Pict& oXml, AVSDocFileFormat::Run& oBinRun) 
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
				//AVSDocFileFormat::Picture oPicture (std::wstring(L""), 0, 0, 0, 0);		//	 fake image
				//m_oInlineShape.SetPicture (oPicture);
				//m_oInlineShape.AddRun (AVSDocFileFormat::Run(AVSDocFileFormat::Picture (std::wstring(L""), 0, 0, 0, 0)));//	 fake image
				//m_oInlineShape.AddRun (AVSDocFileFormat::Run(oShapeRun));//	 fake image

				//m_oInlineShape.AddRun (AVSDocFileFormat::Run(AVSDocFileFormat::CInlineShape (oShapeRun) ));	//	 fake image
				//m_oInlineShape.AddRun (AVSDocFileFormat::Run(oShapeRun));
				//m_oInlineShape.AddRun (AVSDocFileFormat::Run(AVSDocFileFormat::CShapeRun()));

				//m_bIsInlineShape	=	TRUE;

				// TODO : временное решение для Inline Shape - объектов (бинарный парсер доделать)

				oBinRun.AddRunItem (AVSDocFileFormat::CInlineShape (oShapeRun) );						
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
	template<class T> BOOL CFileTransformer::DelTextXmlRunTransform (const OOX::Logic::DelText& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		AVSDocFileFormat::Text oText( FormatUtils::UTF8Decode( oXml.toTxt() ).c_str() );
		oBinRun.AddRunItem(oText);

		return TRUE;
	}
	template<class T> BOOL CFileTransformer::SymbolXmlRunTransform (const OOX::Logic::Symbol& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		if (oXml.Font.is_init() && oXml.Char.is_init())
		{
			AVSDocFileFormat::Text oSymbol ((WCHAR)TextMark::Symbol);
			oBinRun.AddRunItem(oSymbol);

			AVSDocFileFormat::CSymbolOperand oSymbolOp (m_mapFontTableMap[*oXml.Font], HexString2Int(*oXml.Char));
			oBinRun.AddProperty(AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCSymbol, oSymbolOp));

			return TRUE;
		}

		return FALSE;
	}

	template<class T> BOOL CFileTransformer::BreakXmlRunTransform (const OOX::Logic::Break& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		/// TODO

		AVSDocFileFormat::Text oText;

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

	template<class T> BOOL CFileTransformer::TabXmlRunTransform (const OOX::Logic::Tab& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		/// TODO

		AVSDocFileFormat::Text oText;
		oText.SetText(&TextMark::Tab);

		oBinRun.AddRunItem(oText);

		return TRUE;		
	}

	template<class T> BOOL CFileTransformer::FootnoteReferenceXmlRunTransform (const OOX::Logic::FootnoteReference& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		if (inputFolder.find<OOX::Document>().exist<OOX::FootNote>())
		{
			static short footnoteIndex = 1;

			const OOX::FootNote::Note& footNote = inputFolder.find<OOX::Document>().find<OOX::FootNote>().find(oXml);

			AVSDocFileFormat::FootnoteReference docFootnoteReference( footnoteIndex );
			AVSDocFileFormat::Footnote docFootnote( footnoteIndex++ );

			size_t count = (*footNote.Items).size();
			const std::vector<OOX::Logic::TextItem>& items = (*footNote.Items);

			for (size_t i = 0; i < count; ++i)	
			{			
				if (items[i].is<OOX::Logic::Paragraph>())
				{
					const OOX::Logic::Paragraph& docxParagraph	=	items[i].as<OOX::Logic::Paragraph>();

					AVSDocFileFormat::Paragraph oParagraph		=	ConvertParagraph<OOX::FootNote>(docxParagraph);
					docFootnote.AddTextItem (oParagraph);
				}
				else if (items[i].is<OOX::Logic::Table>())
				{
					AVSDocFileFormat::Table oTable				=	CreateTable<OOX::FootNote>(items[i].as<OOX::Logic::Table>());
					docFootnote.AddTextItem (oTable);
				}
			}

			oBinRun.AddRunItem (docFootnoteReference);
			m_pDOCFile->AddFootnote(docFootnote);

			return TRUE;		
		}

		return FALSE;
	}

	template<class T> BOOL CFileTransformer::FootnoteRefXmlRunTransform (const OOX::Logic::FootnoteRef& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		if (inputFolder.find<OOX::Document>().exist<OOX::FootNote>())
		{
			static short footnoteIndex = 1;

			oBinRun.AddRunItem(AVSDocFileFormat::FootnoteRef(footnoteIndex++));

			return TRUE;		
		}

		return FALSE;
	}

	template<class T> BOOL CFileTransformer::EndnoteReferenceXmlRunTransform (const OOX::Logic::EndnoteReference& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		if (inputFolder.find<OOX::Document>().exist<OOX::EndNote>())
		{
			static short endnoteIndex = 1;

			const OOX::EndNote::Note& endNote = inputFolder.find<OOX::Document>().find<OOX::EndNote>().find(oXml);

			AVSDocFileFormat::EndnoteReference docEndnoteReference(endnoteIndex);
			AVSDocFileFormat::Endnote docEndnote(endnoteIndex++);

			size_t count = (*endNote.Items).size();
			const std::vector<OOX::Logic::TextItem>& items = (*endNote.Items);

			for (size_t j = 0; j < count; ++j)	
			{
				if (items[j].is<OOX::Logic::Paragraph>())
				{							
					const OOX::Logic::Paragraph& docxParagraph	=	items[j].as<OOX::Logic::Paragraph>();

					AVSDocFileFormat::Paragraph oParagraph		=	ConvertParagraph<OOX::EndNote>(docxParagraph);
					docEndnote.AddTextItem(oParagraph);
				}
				else if (items[j].is<OOX::Logic::Table>())
				{
					AVSDocFileFormat::Table oTable				=	CreateTable<OOX::EndNote>(items[j].as<OOX::Logic::Table>());
					docEndnote.AddTextItem(oTable);
				}
			}

			oBinRun.AddRunItem( docEndnoteReference );
			m_pDOCFile->AddEndnote( docEndnote );

			return TRUE;		
		}

		return FALSE;
	}

	template<class T> BOOL CFileTransformer::EndnoteRefXmlRunTransform (const OOX::Logic::EndnoteRef& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		if (inputFolder.find<OOX::Document>().exist<OOX::EndNote>())
		{
			static short endnoteIndex = 1;

			oBinRun.AddRunItem(AVSDocFileFormat::EndnoteRef(endnoteIndex++));

			return TRUE;		
		}

		return FALSE;
	}
	template<class T> BOOL CFileTransformer::PictGroupXmlTransform (const OOX::Logic::Pict& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		AVSDocFileFormat::COArtStorage* pStorage	=	AVSDocFileFormat::COArtStorage::Instance();
		if (NULL == pStorage)
			return FALSE;

		AVSDocFileFormat::COArtGroup* pShapes	=	CreateGroup<T>(oXml.group);
		if (NULL == pShapes)
			return FALSE;

		oBinRun.AddRunItem(m_oOArtBuilder.BuildGroupRun(oXml.group,pShapes));
		return TRUE;
	}
}

namespace DOCXTODOC
{
	template <class T, class TXmlImage> OOX::Image* CFileTransformer::GetImageDOCX (const TXmlImage* pXmlImage)
	{	
		if (pXmlImage)
		{
			BOOL bFindOK	=	FALSE;
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
					if (inputFolder.find<OOX::Document>().find<T>().exist(nID))
						return dynamic_cast<OOX::Image*>(inputFolder.find<OOX::Document>().find<T>()[nID].get());
				}
				else
				{
					if (inputFolder.find<OOX::Document>().exist(nID))
						return dynamic_cast<OOX::Image*>(inputFolder.find<OOX::Document>()[nID].get());
				}
			}
		}

		return NULL;
	}

	template <class T> OOX::Image* CFileTransformer::GetImageWithId(const OOX::RId& nID)
	{
		if (typeid(T) != typeid(OOX::Document))
		{
			if (inputFolder.find<OOX::Document>().find<T>().exist(nID))
				return dynamic_cast<OOX::Image*>(inputFolder.find<OOX::Document>().find<T>()[nID].get());
		}

		if (inputFolder.find<OOX::Document>().exist(nID))
			return dynamic_cast<OOX::Image*>(inputFolder.find<OOX::Document>()[nID].get());

		return NULL;
	}

	template<class T> BOOL CFileTransformer::CreateImage (const OOX::Logic::Pict& oXml, AVSDocFileFormat::Run& oBinRun) 
	{
		OOX::Image* pImage				=	GetImageDOCX <T, OOX::Logic::Pict>(&oXml);

		if (pImage)
		{
			std::wstring sFileName		=	pImage->filename().string();
			if (sFileName.length())
			{
				if (oXml.shape.is_init())
				{
					if (oXml.shape->imageData.is_init())
					{
						return m_oOArtBuilder.BuildImageRun(oXml.shape, pImage->filename().string(), oBinRun);
					}
				}

				Unit<int, Dx> twWidth	=	*oXml.shape->style->Size->Width;
				Unit<int, Dx> twHeight	=	*oXml.shape->style->Size->Height;

				AVSDocFileFormat::Picture oPicture (sFileName, twWidth, twHeight);
				if (oPicture.IsValid())
				{
					oBinRun.AddRunItem (AVSDocFileFormat::Picture());

					return TRUE;
				}
			}
		}

		return FALSE;
	}

	template<class T> AVSDocFileFormat::COArtGroup* CFileTransformer::CreateGroup(const OOX::Logic::Group& oXmlGroup)
	{
		AVSDocFileFormat::COArtStorage* pStorage	=	AVSDocFileFormat::COArtStorage::Instance();
		if (NULL == pStorage)
			return FALSE;

		AVSDocFileFormat::COArtGroup* pBinGroup		=	new AVSDocFileFormat::COArtGroup(pStorage->GenID(m_oOArtBuilder.Location()));
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
					OOX::Image* image = GetImageWithId <T>(OOX::RId(shape.fillstyle->Id));
					if (image)
					{
						m_oOArtBuilder.SetTextureFill(image->filename().string());
					}
				}

				pBinGroup->Append (m_oOArtBuilder.BuildOArtGroupShape<OOX::Logic::Oval>(shape, OfficeArt::Enumerations::msosptEllipse));
			}

			if (oXmlItem.is<OOX::Logic::Roundrect>())
			{
				const OOX::Logic::Roundrect& shape = oXmlItem.as<OOX::Logic::Roundrect>();

				if (shape.fillstyle->Id.is_init())
				{
					OOX::Image* image = GetImageWithId <T>(OOX::RId(shape.fillstyle->Id));
					if (image)
					{
						m_oOArtBuilder.SetTextureFill(image->filename().string());
					}
				}

				pBinGroup->Append (m_oOArtBuilder.BuildOArtGroupShape<OOX::Logic::Roundrect>(oXmlItem.as<OOX::Logic::Roundrect>(), OfficeArt::Enumerations::msosptRoundRectangle));
			}

			if (oXmlItem.is<OOX::Logic::Rect>())
			{
				const OOX::Logic::Rect& shape = oXmlItem.as<OOX::Logic::Rect>();

				if (shape.fillstyle->Id.is_init())
				{
					OOX::Image* image = GetImageWithId <T>(OOX::RId(shape.fillstyle->Id));
					if (image)
					{
						m_oOArtBuilder.SetTextureFill(image->filename().string());
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
						pBinGroup->Append (m_oOArtBuilder.BuildOArtImage(shape, image->filename().string()));
					}
				}
				else
				{
					if (shape.fillstyle->Id.is_init())
					{
						OOX::Image* image = GetImageWithId <T>(OOX::RId(shape.fillstyle->Id));
						if (image)
						{
							m_oOArtBuilder.SetTextureFill(image->filename().string());
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

	template<class T> BOOL CFileTransformer::CreateTextureFillShape (const OOX::Logic::Pict& oXml, AVSDocFileFormat::Run& oBinRun)
	{
		// фигура имеет заливку текстурой

		// RECT

		if (oXml.rect.is_init())
		{
			if (oXml.rect->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(oXml.rect->fillstyle->Id));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Rect>(oXml.rect, pImage->filename().string(), OfficeArt::Enumerations::msosptRectangle, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		// OVAL

		if (oXml.oval.is_init())
		{
			if (oXml.oval->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(oXml.oval->fillstyle->Id));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Oval>(oXml.oval, pImage->filename().string(), OfficeArt::Enumerations::msosptEllipse, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		// ROUNDRECT

		if (oXml.roundrect.is_init())
		{
			if (oXml.roundrect->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(oXml.roundrect->fillstyle->Id));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Roundrect>(oXml.roundrect, pImage->filename().string(), OfficeArt::Enumerations::msosptRoundRectangle, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		// SHAPE CUSTOM

		if (oXml.shape.is_init())
		{
			if (oXml.shape->fillstyle->Id.is_init())
			{
				OOX::Image* pImage = GetImageWithId <T>(OOX::RId(oXml.shape->fillstyle->Id));
				if (pImage)
				{
					return m_oOArtBuilder.BuildShapeWithTextureFill<OOX::Logic::Shape>(oXml.shape, pImage->filename().string(), 0, oXml, oBinRun);
				}

				return FALSE;
			}
		}

		return FALSE;
	}

}

namespace DOCXTODOC
{
	BOOL CFileTransformer::BuildContentTbRef (const OOX::Logic::Pict& oXml)
	{
		CTextBoxRef* pTbRef = m_oOArtBuilder.LastTbRef ();
		if (pTbRef)
		{
			std::vector<AVSDocFileFormat::TextItem> oTextItems;

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

	BOOL CFileTransformer::TransformTb (const std::vector<OOX::Logic::TextItem>& oXmlItems, std::vector<AVSDocFileFormat::TextItem>& oTextItems)
	{
		for (size_t i = 0; i < oXmlItems.size(); ++i)
		{
			const OOX::Logic::TextItem& oXmlItem	=	oXmlItems[i];

			if (oXmlItem.is<OOX::Logic::Paragraph>())
			{
				AVSDocFileFormat::Paragraph oBinPr	=	ConvertParagraph<OOX::Document>(oXmlItem.as<OOX::Logic::Paragraph>());
				oTextItems.push_back(TextItem(oBinPr));
			}

			if (oXmlItem.is<OOX::Logic::Table>())
			{
				AVSDocFileFormat::Table oBinTable	=	CreateTable<OOX::Document>(oXmlItem.as<OOX::Logic::Table>());
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
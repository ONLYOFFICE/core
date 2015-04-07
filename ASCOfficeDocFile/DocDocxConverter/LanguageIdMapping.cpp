

#include "LanguageIdMapping.h"

namespace DocFileFormat
{
	LanguageIdMapping::LanguageIdMapping (XmlUtils::CXmlWriter* pWriter, LanguageType type) : PropertiesMapping (pWriter)
	{
		_type = type;
	}

	LanguageIdMapping::LanguageIdMapping (XMLTools::XMLElement<wchar_t>* parentElement, LanguageType type) : PropertiesMapping(NULL)
	{
		_parent = parentElement;
		_type = type;
	}

	LanguageIdMapping::~LanguageIdMapping()
	{
	}
}

namespace DocFileFormat
{
	void LanguageIdMapping::Apply( IVisitable* lid )
	{
		if ( dynamic_cast<LanguageId*>( lid )->Code != Nothing )
		{
			wstring langcode = _T( "en-US" );

			langcode = getLanguageCode( dynamic_cast<LanguageId*>( lid ) );

			XMLTools::XMLAttribute<wchar_t>* att = NULL;

			switch ( _type )
			{
			case Default:
				{
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), langcode.c_str() );
				}
				break;

			case EastAsian:
				{                    
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:eastAsia" ), langcode.c_str() );
				}
				break;

			case Complex:
				{
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:bidi" ), langcode.c_str() );
				}
				break;

			default:
				{    
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), langcode.c_str() );
				}
				break;
			}

			if (m_pXmlWriter)
			{
				// !!!TODO!!!
				m_pXmlWriter->WriteString( att->GetXMLString().c_str() );
			}
			else if ( _parent != NULL )
			{
				_parent->AppendAttribute( *att );
			}

			RELEASEOBJECT( att );
		}
	}

	wstring LanguageIdMapping::getLanguageCode( LanguageId* lid )
	{
		switch ( lid->Code )
		{
		case Afrikaans:
			return wstring( _T( "af-ZA" ) );
		case Albanian:
			return wstring( _T( "sq-AL" ) );
		case Amharic:
			return wstring( _T( "am-ET" ) );
		case ArabicAlgeria:
			return wstring( _T( "ar-DZ" ) );
		case ArabicBahrain:
			return wstring( _T( "ar-BH" ) );
		case ArabicEgypt:
			return wstring( _T( "ar-EG" ) );
		case ArabicIraq:
			return wstring( _T( "ar-IQ" ) );
		case ArabicJordan:
			return wstring( _T( "ar-JO" ) );
		case ArabicKuwait:
			return wstring( _T( "ar-KW" ) );
		case ArabicLebanon:
			return wstring( _T( "ar-LB" ) );
		case ArabicLibya:
			return wstring( _T( "ar-LY" ) );
		case ArabicMorocco:
			return wstring( _T( "ar-MA" ) );
		case ArabicOman:
			return wstring( _T( "ar-OM" ) );
		case ArabicQatar:
			return wstring( _T( "ar-QA" ) );
		case ArabicSaudiArabia:
			return wstring( _T( "ar-SA" ) );
		case ArabicSyria:
			return wstring( _T( "ar-SY" ) );
		case ArabicTunisia:
			return wstring( _T( "ar-TN" ) );
		case ArabicUAE:
			return wstring( _T( "ar-AE" ) );
		case ArabicYemen:
			return wstring( _T( "ar-YE" ) );
		case Armenian:
			return wstring( _T( "hy-AM" ) );
		case Assamese:
			return wstring( _T( "as-IN" ) );
		case AzeriCyrillic:
			return wstring( _T( "az-AZ-cyrl" ) );
		case AzeriLatin:
			return wstring( _T( "az-AZ-latn" ) );
		case Basque:
			return wstring( _T( "eu-ES" ) );
		case Belarusian:
			return wstring( _T( "be-BY" ) );
		case Bengali:
			return wstring( _T( "bn-IN" ) );
		case BengaliBangladesh:
			return wstring( _T( "bn-BD" ) );
		case Bulgarian:
			return wstring( _T( "bg-BG" ) );
		case Burmese:
			return wstring( _T( "my-MM" ) );
		case Catalan:
			return wstring( _T( "ca-ES" ) );
		case Cherokee:
			//there is no iso code fpr cherokee
		case ChineseHongKong:
			return wstring( _T( "zh-HK" ) );
		case ChineseMacao:
			return wstring( _T( "zh-MO" ) );
		case ChinesePRC:
			return wstring( _T( "zh-CN" ) );
		case ChineseSingapore:
			return wstring( _T( "zh-SG" ) );
		case ChineseTaiwan:
			return wstring( _T( "zh-TW" ) );
		case Croatian:
			return wstring( _T( "hr-HR" ) );
		case Czech:
			return wstring( _T( "cs-CZ" ) );
		case Danish:
			return wstring( _T( "da-DK" ) );
		case Divehi:
			return wstring( _T( "dv-MV" ) );
		case DutchBelgium:
			return wstring( _T( "nl-BE" ) );
		case DutchNetherlands:
			return wstring( _T( "nl-NL" ) );
		case Edo:
			//there is no iso 639-1 code for edo
		case EnglishAustralia:
			return wstring( _T( "en-AU" ) );
		case EnglishBelize:
			return wstring( _T( "en-BZ" ) );
		case EnglishCanada:
			return wstring( _T( "en-CA" ) );
		case EnglishCaribbean:
			//the caribbean sea has many english speaking countires.
			//we use the Dominican Republic
			return wstring( _T( "en-DO" ) );
		case EnglishHongKong:
			return wstring( _T( "en-HK" ) );
		case EnglishIndia:
			return wstring( _T( "en-IN" ) );
		case EnglishIndonesia:
			return wstring( _T( "en-ID" ) );
		case EnglishIreland:
			return wstring( _T( "en-IE" ) );
		case EnglishJamaica:
			return wstring( _T( "en-JM" ) );
		case EnglishMalaysia:
			return wstring( _T( "en-MY" ) );
		case EnglishNewZealand:
			return wstring( _T( "en-NZ" ) );
		case EnglishPhilippines:
			return wstring( _T( "en-PH" ) );
		case EnglishSingapore:
			return wstring( _T( "en-SG" ) );
		case EnglishSouthAfrica:
			return wstring( _T( "en-ZA" ) );
		case EnglishTrinidadAndTobago:
			return wstring( _T( "en-TT" ) );
		case EnglishUK:
			return wstring( _T( "en-UK" ) );
		case EnglishUS:
			return wstring( _T( "en-US" ) );
		case EnglishZimbabwe:
			return wstring( _T( "en-ZW" ) );
		case Estonian:
			return wstring( _T( "et-EE" ) );
		case Faeroese:
			return wstring( _T( "fo-FO" ) );
		case Farsi:
			//there is no iso 639-1 code for farsi
		case Filipino:
			//there is no iso 639-1 code for filipino
		case Finnish:
			return wstring( _T( "fi-FI" ) );
		case FrenchBelgium:
			return wstring( _T( "fr-BE" ) );
		case FrenchCameroon:
			return wstring( _T( "fr-CM" ) );
		case FrenchCanada:
			return wstring( _T( "fr-CA" ) );
		case FrenchCongoDRC:
			return wstring( _T( "fr-CD" ) );
		case FrenchCotedIvoire:
			return wstring( _T( "fr-CI" ) );
		case FrenchFrance:
			return wstring( _T( "fr-FR" ) );
		case FrenchHaiti:
			return wstring( _T( "fr-HT" ) );
		case FrenchLuxembourg:
			return wstring( _T( "fr-LU" ) );
		case FrenchMali:
			return wstring( _T( "fr-ML" ) );
		case FrenchMonaco:
			return wstring( _T( "fr-MC" ) );
		case FrenchMorocco:
			return wstring( _T( "fr-MA" ) );
		case FrenchReunion:
			return wstring( _T( "fr-RE" ) );
		case FrenchSenegal:
			return wstring( _T( "fr-SN" ) );
		case FrenchSwitzerland:
			return wstring( _T( "fr-CH" ) );
		case FrenchWestIndies:
			//the western caribbean sea has many french speaking countires.
			//we use the Dominican Republic
			return wstring( _T( "fr-DO" ) );
		case FrisianNetherlands:
			return wstring( _T( "fy-NL" ) );
		case Fulfulde:
			//there is no iso 639-1 code for fulfulde
		case FYROMacedonian:
			return wstring( _T( "mk-MK" ) );
		case GaelicIreland:
			return wstring( _T( "ga-IE" ) );
		case GaelicScotland:
			return wstring( _T( "gd-UK" ) );
		case Galician:
			return wstring( _T( "gl-ES" ) );
		case Georgian:
			return wstring( _T( "ka-GE" ) );
		case GermanAustria:
			return wstring( _T( "de-AT" ) );
		case GermanGermany:
			return wstring( _T( "de-DE" ) );
		case GermanLiechtenstein:
			return wstring( _T( "de-LI" ) );
		case GermanLuxembourg:
			return wstring( _T( "de-LU" ) );
		case GermanSwitzerland:
			return wstring( _T( "de-CH" ) );
		case Greek:
			return wstring( _T( "el-GR" ) );
		case Guarani:
			return wstring( _T( "gn-BR" ) );
		case Gujarati:
			return wstring( _T( "gu-IN" ) );
		case Hausa:
			return wstring( _T( "ha-NG" ) );
		case Hawaiian:
			//there is no iso 639-1 language code for hawaiian
		case Hebrew:
			return wstring( _T( "he-IL" ) );
		case Hindi:
			return wstring( _T( "hi-IN" ) );
		case Hungarian:
			return wstring( _T( "hu-HU" ) );
		case Ibibio:
			//there is no iso 639-1 language code for ibibio
		case Icelandic:
			return wstring( _T( "is-IS" ) );
		case Igbo:
			//there is no iso 639-1 language code for ibibio
		case Indonesian:
			return wstring( _T( "id-ID" ) );
		case Inuktitut:
			return wstring( _T( "iu-CA" ) );
		case ItalianItaly:
			return wstring( _T( "it-IT" ) );
		case ItalianSwitzerland:
			return wstring( _T( "it-CH" ) );
		case Japanese:
			return wstring( _T( "ja-JP" ) );
		case Kannada:
			return wstring( _T( "kn-ID" ) );
		case Kanuri:
			//there is no iso 639-1 language code for kanuri
		case Kashmiri:
			return wstring( _T( "ks-ID" ) );
		case KashmiriArabic:
			return wstring( _T( "ks-PK" ) );
		case Kazakh:
			return wstring( _T( "kk-KZ" ) );
		case Khmer:
			//there is no iso 639-1 language code for khmer
		case Konkani:
			//there is no iso 639-1 language code for konkani
		case Korean:
			return wstring( _T( "ko-KR" ) );
		case Kyrgyz:
			return wstring( _T( "ky-KG" ) );
		case Lao:
			return wstring( _T( "lo-LA" ) );
		case Latin:
			return wstring( _T( "la" ) );
		case Latvian:
			return wstring( _T( "lv-LV" ) );
		case Lithuanian:
			return wstring( _T( "lt-LT" ) );
		case Malay:
			return wstring( _T( "ms-MY" ) );
		case MalayBruneiDarussalam:
			return wstring( _T( "ms-BN" ) );
		case Malayalam:
			return wstring( _T( "ml-ID" ) );
		case Maltese:
			return wstring( _T( "mt-MT" ) );
		case Manipuri:
			//there is no iso 639-1 language code for manipuri
		case Maori:
			return wstring( _T( "mi-NZ" ) );
		case Marathi:
			return wstring( _T( "mr-ID" ) );
		case Mongolian:
			return wstring( _T( "mn-MN" ) );
		case MongolianMongolian:
			return wstring( _T( "mn-MN" ) );
		case Nepali:
			return wstring( _T( "ne-NP" ) );
		case NepaliIndia:
			return wstring( _T( "ne-ID" ) );
		case NorwegianBokmal:
			return wstring( _T( "nb-NO" ) );
			//also possible: no-NO
		case NorwegianNynorsk:
			return wstring( _T( "nn-NO" ) );
			//also possible: no-NO
		case Oriya:
			return wstring( _T( "or-ID" ) );
		case Oromo:
			//there is no iso 639-1 language code for oromo
		case Papiamentu:
			//there is no iso 639-1 language code for papiamentu
		case Pashto:
			return wstring( _T( "ps-PK" ) );
		case Polish:
			return wstring( _T( "pl-PL" ) );
		case PortugueseBrazil:
			return wstring( _T( "pt-BR" ) );
		case PortuguesePortugal:
			return wstring( _T( "pt-PT" ) );
		case Punjabi:
			return wstring( _T( "pa-ID" ) );
		case PunjabiPakistan:
			return wstring( _T( "pa-PK" ) );
		case QuechuaBolivia:
			return wstring( _T( "qu-BO" ) );
		case QuechuaEcuador:
			return wstring( _T( "qu-EC" ) );
		case QuechuaPeru:
			return wstring( _T( "qu-PE" ) );
		case RhaetoRomanic:
			return wstring( _T( "rm-CH" ) );
		case RomanianMoldova:
			return wstring( _T( "ro-MD" ) );
		case RomanianRomania:
			return wstring( _T( "ro-RO" ) );
		case RussianMoldova:
			return wstring( _T( "ru-MD" ) );
		case RussianRussia:
			return wstring( _T( "ru-RU" ) );
		case SamiLappish:
			return wstring( _T( "se-FI" ) );
		case Sanskrit:
			return wstring( _T( "sa-ID" ) );
		case Sepedi:
			//there is no iso 639-1 language code for sepedi
		case SerbianCyrillic:
			return wstring( _T( "sr-YU-cyrl" ) );
		case SerbianLatin:
			return wstring( _T( "sr-YU-latn" ) );
		case SindhiArabic:
			return wstring( _T( "sd-PK" ) );
		case SindhiDevanagari:
			return wstring( _T( "sd-ID" ) );
		case Sinhalese:
			return wstring( _T( "si-ID" ) );
		case Slovak:
			return wstring( _T( "sk-SK" ) );
		case Slovenian:
			return wstring( _T( "sl-SI" ) );
		case Somali:
			return wstring( _T( "so-SO" ) );
		case Sorbian:
			//there is no iso 639-1 language code for sorbian
		case SpanishArgentina:
			return wstring( _T( "es-AR" ) );
		case SpanishBolivia:
			return wstring( _T( "es-BO" ) );
		case SpanishChile:
			return wstring( _T( "es-CL" ) );
		case SpanishColombia:
			return wstring( _T( "es-CO" ) );
		case SpanishCostaRica:
			return wstring( _T( "es-CR" ) );
		case SpanishDominicanRepublic:
			return wstring( _T( "es-DO" ) );
		case SpanishEcuador:
			return wstring( _T( "es-EC" ) );
		case SpanishElSalvador:
			return wstring( _T( "es-SV" ) );
		case SpanishGuatemala:
			return wstring( _T( "es-GT" ) );
		case SpanishHonduras:
			return wstring( _T( "es-HN" ) );
		case SpanishMexico:
			return wstring( _T( "es-MX" ) );
		case SpanishNicaragua:
			return wstring( _T( "es-NI" ) );
		case SpanishPanama:
			return wstring( _T( "es-PA" ) );
		case SpanishParaguay:
			return wstring( _T( "es-PY" ) );
		case SpanishPeru:
			return wstring( _T( "es-PE" ) );
		case SpanishPuertoRico:
			return wstring( _T( "es-PR" ) );
		case SpanishSpainModernSort:
			return wstring( _T( "es-ES" ) );
		case SpanishSpainTraditionalSort:
			return wstring( _T( "es-ES" ) );
		case SpanishUruguay:
			return wstring( _T( "es-UY" ) );
		case SpanishVenezuela:
			return wstring( _T( "es-VE" ) );
		case Sutu:
			//there is no iso 639-1 language code for sutu
		case Swahili:
			//Swahili is spoken in many east african countries, so we use tansania
			return wstring( _T( "sw-TZ" ) );
		case SwedishFinland:
			return wstring( _T( "sv-FI" ) );
		case SwedishSweden:
			return wstring( _T( "sv-SE" ) );
		case Syriac:
			//there is no iso 639-1 language code for syriac
		case Tajik:
			return wstring( _T( "tg-TJ" ) );
		case Tamazight:
			//there is no iso 639-1 language code for tamazight
		case TamazightLatin:
			//there is no iso 639-1 language code for tamazight
		case Tamil:
			return wstring( _T( "ta-ID" ) );
		case Tatar:
			return wstring( _T( "tt-RU" ) );
		case Telugu:
			return wstring( _T( "te-ID" ) );
			//case Thai:

			//case TibetanBhutan:

			//case TibetanPRC:

			//case TigrignaEritrea:

			//case TigrignaEthiopia:

			//case Tsonga:

			//case Tswana:

			//case Turkish:

			//case Turkmen:

			//case Ukrainian:

			//case Urdu:

			//case UzbekCyrillic:

			//case UzbekLatin:

			//case Venda:

			//case Vietnamese:

			//case Welsh:

			//case Xhosa:

			//case Yi:

			//case Yiddish:

			//case Yoruba:

			//case Zulu:

		default:
			return wstring( _T( "en-US" ) );
		}
	}
}
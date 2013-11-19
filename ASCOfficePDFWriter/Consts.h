#ifndef _CONSTS_H
#define _CONSTS_H

#define  OK                    0
#define  NOERROR               0

// Стандартный размер буфера в memory-stream-object
#define STREAM_BUF_SIZ         4096

// стандартный размер массива list-object
#define DEF_ITEMS_PER_BLOCK    20

// стандартный размер массива cross-reference-table
#define DEFALUT_XREF_ENTRY_NUM 1024

// стандартный размер таблицы ширин (widths-table) в cid-fontdef 
#define DEF_CHAR_WIDTHS_NUM    128

// стандартный рамер массива page-list-tablef 
#define DEF_PAGE_LIST_NUM      256

// стандартный размер таблицы range-table в cid-fontdef 
#define DEF_RANGE_TBL_NUM      128

// Необходимый размер буффера для конвертирования с символьную строку.
#define TEMP_BUFFER_SIZE       512
#define SHORT_BUFFER_SIZE      32
#define REAL_LEN               31//11
#define INT_LEN                11
#define TEXT_DEFAULT_LEN       256
#define UNICODE_HEADER_LEN     2
#define DATE_TIME_STR_LEN      23

// Соответсвующие длины в таблицу cross-reference-table
#define BYTE_OFFSET_LEN        10
#define OBJ_ID_LEN             7
#define GEN_NO_LEN             5

// Стандартные значения для Graphic State
#define DEF_FONT               "Helvetica"
#define DEF_PAGE_LAYOUT        PLSingle
#define DEF_PAGE_MODE          PageModeUseNone
#define DEF_WORDSPACE          0
#define DEF_CHARSPACE          0
#define DEF_FONTSIZE           10
#define DEF_HSCALING           100
#define DEF_LEADING            0
#define DEF_RENDERING_MODE     Fill
#define DEF_RISE               0
#define DEF_LINEWIDTH          1
#define DEF_LINECAP            ButtEnd
#define DEF_LINEJOIN           MiterJoin
#define DEF_MITERLIMIT         10
#define DEF_FLATNESS           1
#define DEF_PAGE_NUM           1

#define BS_DEF_WIDTH           1

// Стандартный размер страницы (А4)
#define DEF_PAGE_WIDTH         595.276F
#define DEF_PAGE_HEIGHT        841.89F

// Типы компрессии

#define  COMP_NONE            0x00
#define  COMP_TEXT            0x01
#define  COMP_IMAGE           0x02
#define  COMP_METADATA        0x04
#define  COMP_ALL             0x0F
// #define  COMP_BEST_COMPRESS   0x10
// #define  COMP_BEST_SPEED      0x20
#define  COMP_MASK            0xFF


// Permission flags (only Revision 2 is supported)

#define ENABLE_READ         0
#define ENABLE_PRINT        4
#define ENABLE_EDIT_ALL     8
#define ENABLE_COPY         16
#define ENABLE_EDIT         32

// Варианты просмотра документа

#define HIDE_TOOLBAR    1
#define HIDE_MENUBAR    2
#define HIDE_WINDOW_UI  4
#define FIT_WINDOW      8
#define CENTER_WINDOW   16


// Ограничения, в соответствии со спецификацией (PDF1.4)

#define LIMIT_MAX_INT             2147483647
#define LIMIT_MIN_INT             -2147483647


#define LIMIT_MAX_REAL            3.402823466e+38F
#define LIMIT_MIN_REAL            -3.402823466e+38F

#define LIMIT_MAX_STRING_LEN      65535
#define LIMIT_MAX_NAME_LEN        127

#define LIMIT_MAX_ARRAY           8191
#define LIMIT_MAX_DICT_ELEMENT    4095
#define LIMIT_MAX_XREF_ELEMENT    8388607
#define LIMIT_MAX_GSTATE          28
#define LIMIT_MAX_DEVICE_N        8
#define LIMIT_MAX_DEVICE_N_V15    32
#define LIMIT_MAX_CID             65535
#define MAX_GENERATION_NUM        65535

#define MIN_PAGE_HEIGHT           3
#define MIN_PAGE_WIDTH            3
#define MAX_PAGE_HEIGHT           14400
#define MAX_PAGE_WIDTH            14400
#define MIN_MAGNIFICATION_FACTOR  8
#define MAX_MAGNIFICATION_FACTOR  3200

// Дополонительные ограничения

#define MIN_PAGE_SIZE          3
#define MAX_PAGE_SIZE          14400
#define MIN_HORIZONTALSCALING  0.01
#define MAX_HORIZONTALSCALING  1000
#define MIN_WORDSPACE          -30
#define MAX_WORDSPACE          300
#define MIN_CHARSPACE          -30
#define MAX_CHARSPACE          300
#define MAX_FONTSIZE           1000
#define MAX_ZOOMSIZE           10
#define MAX_LEADING            300
#define MAX_LINEWIDTH          100
#define MAX_DASH_PATTERN       100

#define MAX_JWW_NUM            128

// Коды стран

#define COUNTRY_AF  "AF"    /* AFGHANISTAN */
#define COUNTRY_AL  "AL"    /* ALBANIA */
#define COUNTRY_DZ  "DZ"    /* ALGERIA */
#define COUNTRY_AS  "AS"    /* AMERICAN SAMOA */
#define COUNTRY_AD  "AD"    /* ANDORRA */
#define COUNTRY_AO  "AO"    /* ANGOLA */
#define COUNTRY_AI  "AI"    /* ANGUILLA */
#define COUNTRY_AQ  "AQ"    /* ANTARCTICA */
#define COUNTRY_AG  "AG"    /* ANTIGUA AND BARBUDA */
#define COUNTRY_AR  "AR"    /* ARGENTINA */
#define COUNTRY_AM  "AM"    /* ARMENIA */
#define COUNTRY_AW  "AW"    /* ARUBA */
#define COUNTRY_AU  "AU"    /* AUSTRALIA */
#define COUNTRY_AT  "AT"    /* AUSTRIA */
#define COUNTRY_AZ  "AZ"    /* AZERBAIJAN */
#define COUNTRY_BS  "BS"    /* BAHAMAS */
#define COUNTRY_BH  "BH"    /* BAHRAIN */
#define COUNTRY_BD  "BD"    /* BANGLADESH */
#define COUNTRY_BB  "BB"    /* BARBADOS */
#define COUNTRY_BY  "BY"    /* BELARUS */
#define COUNTRY_BE  "BE"    /* BELGIUM */
#define COUNTRY_BZ  "BZ"    /* BELIZE */
#define COUNTRY_BJ  "BJ"    /* BENIN */
#define COUNTRY_BM  "BM"    /* BERMUDA */
#define COUNTRY_BT  "BT"    /* BHUTAN */
#define COUNTRY_BO  "BO"    /* BOLIVIA */
#define COUNTRY_BA  "BA"    /* BOSNIA AND HERZEGOWINA */
#define COUNTRY_BW  "BW"    /* BOTSWANA */
#define COUNTRY_BV  "BV"    /* BOUVET ISLAND */
#define COUNTRY_BR  "BR"    /* BRAZIL */
#define COUNTRY_IO  "IO"    /* BRITISH INDIAN OCEAN TERRITORY */
#define COUNTRY_BN  "BN"    /* BRUNEI DARUSSALAM */
#define COUNTRY_BG  "BG"    /* BULGARIA */
#define COUNTRY_BF  "BF"    /* BURKINA FASO */
#define COUNTRY_BI  "BI"    /* BURUNDI */
#define COUNTRY_KH  "KH"    /* CAMBODIA */
#define COUNTRY_CM  "CM"    /* CAMEROON */
#define COUNTRY_CA  "CA"    /* CANADA */
#define COUNTRY_CV  "CV"    /* CAPE VERDE */
#define COUNTRY_KY  "KY"    /* CAYMAN ISLANDS */
#define COUNTRY_CF  "CF"    /* CENTRAL AFRICAN REPUBLIC */
#define COUNTRY_TD  "TD"    /* CHAD */
#define COUNTRY_CL  "CL"    /* CHILE */
#define COUNTRY_CN  "CN"    /* CHINA */
#define COUNTRY_CX  "CX"    /* CHRISTMAS ISLAND */
#define COUNTRY_CC  "CC"    /* COCOS (KEELING) ISLANDS */
#define COUNTRY_CO  "CO"    /* COLOMBIA */
#define COUNTRY_KM  "KM"    /* COMOROS */
#define COUNTRY_CG  "CG"    /* CONGO */
#define COUNTRY_CK  "CK"    /* COOK ISLANDS */
#define COUNTRY_CR  "CR"    /* COSTA RICA */
#define COUNTRY_CI  "CI"    /* COTE D'IVOIRE */
#define COUNTRY_HR  "HR"    /* CROATIA (local name: Hrvatska) */
#define COUNTRY_CU  "CU"    /* CUBA */
#define COUNTRY_CY  "CY"    /* CYPRUS */
#define COUNTRY_CZ  "CZ"    /* CZECH REPUBLIC */
#define COUNTRY_DK  "DK"    /* DENMARK */
#define COUNTRY_DJ  "DJ"    /* DJIBOUTI */
#define COUNTRY_DM  "DM"    /* DOMINICA */
#define COUNTRY_DO  "DO"    /* DOMINICAN REPUBLIC */
#define COUNTRY_TP  "TP"    /* EAST TIMOR */
#define COUNTRY_EC  "EC"    /* ECUADOR */
#define COUNTRY_EG  "EG"    /* EGYPT */
#define COUNTRY_SV  "SV"    /* EL SALVADOR */
#define COUNTRY_GQ  "GQ"    /* EQUATORIAL GUINEA */
#define COUNTRY_ER  "ER"    /* ERITREA */
#define COUNTRY_EE  "EE"    /* ESTONIA */
#define COUNTRY_ET  "ET"    /* ETHIOPIA */
#define COUNTRY_FK  "FK"    /* FALKLAND ISLANDS (MALVINAS) */
#define COUNTRY_FO  "FO"    /* FAROE ISLANDS */
#define COUNTRY_FJ  "FJ"    /* FIJI */
#define COUNTRY_FI  "FI"    /* FINLAND */
#define COUNTRY_FR  "FR"    /* FRANCE */
#define COUNTRY_FX  "FX"    /* FRANCE, METROPOLITAN */
#define COUNTRY_GF  "GF"    /* FRENCH GUIANA */
#define COUNTRY_PF  "PF"    /* FRENCH POLYNESIA */
#define COUNTRY_TF  "TF"    /* FRENCH SOUTHERN TERRITORIES */
#define COUNTRY_GA  "GA"    /* GABON */
#define COUNTRY_GM  "GM"    /* GAMBIA */
#define COUNTRY_GE  "GE"    /* GEORGIA */
#define COUNTRY_DE  "DE"    /* GERMANY */
#define COUNTRY_GH  "GH"    /* GHANA */
#define COUNTRY_GI  "GI"    /* GIBRALTAR */
#define COUNTRY_GR  "GR"    /* GREECE */
#define COUNTRY_GL  "GL"    /* GREENLAND */
#define COUNTRY_GD  "GD"    /* GRENADA */
#define COUNTRY_GP  "GP"    /* GUADELOUPE */
#define COUNTRY_GU  "GU"    /* GUAM */
#define COUNTRY_GT  "GT"    /* GUATEMALA */
#define COUNTRY_GN  "GN"    /* GUINEA */
#define COUNTRY_GW  "GW"    /* GUINEA-BISSAU */
#define COUNTRY_GY  "GY"    /* GUYANA */
#define COUNTRY_HT  "HT"    /* HAITI */
#define COUNTRY_HM  "HM"    /* HEARD AND MC DONALD ISLANDS */
#define COUNTRY_HN  "HN"    /* HONDURAS */
#define COUNTRY_HK  "HK"    /* HONG KONG */
#define COUNTRY_HU  "HU"    /* HUNGARY */
#define COUNTRY_IS  "IS"    /* ICELAND */
#define COUNTRY_IN  "IN"    /* INDIA */
#define COUNTRY_ID  "ID"    /* INDONESIA */
#define COUNTRY_IR  "IR"    /* IRAN (ISLAMIC REPUBLIC OF) */
#define COUNTRY_IQ  "IQ"    /* IRAQ */
#define COUNTRY_IE  "IE"    /* IRELAND */
#define COUNTRY_IL  "IL"    /* ISRAEL */
#define COUNTRY_IT  "IT"    /* ITALY */
#define COUNTRY_JM  "JM"    /* JAMAICA */
#define COUNTRY_JP  "JP"    /* JAPAN */
#define COUNTRY_JO  "JO"    /* JORDAN */
#define COUNTRY_KZ  "KZ"    /* KAZAKHSTAN */
#define COUNTRY_KE  "KE"    /* KENYA */
#define COUNTRY_KI  "KI"    /* KIRIBATI */
#define COUNTRY_KP  "KP"    /* KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF */
#define COUNTRY_KR  "KR"    /* KOREA, REPUBLIC OF */
#define COUNTRY_KW  "KW"    /* KUWAIT */
#define COUNTRY_KG  "KG"    /* KYRGYZSTAN */
#define COUNTRY_LA  "LA"    /* LAO PEOPLE'S DEMOCRATIC REPUBLIC */
#define COUNTRY_LV  "LV"    /* LATVIA */
#define COUNTRY_LB  "LB"    /* LEBANON */
#define COUNTRY_LS  "LS"    /* LESOTHO */
#define COUNTRY_LR  "LR"    /* LIBERIA */
#define COUNTRY_LY  "LY"    /* LIBYAN ARAB JAMAHIRIYA */
#define COUNTRY_LI  "LI"    /* LIECHTENSTEIN */
#define COUNTRY_LT  "LT"    /* LITHUANIA */
#define COUNTRY_LU  "LU"    /* LUXEMBOURG */
#define COUNTRY_MO  "MO"    /* MACAU */
#define COUNTRY_MK  "MK"    /* MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF */
#define COUNTRY_MG  "MG"    /* MADAGASCAR */
#define COUNTRY_MW  "MW"    /* MALAWI */
#define COUNTRY_MY  "MY"    /* MALAYSIA */
#define COUNTRY_MV  "MV"    /* MALDIVES */
#define COUNTRY_ML  "ML"    /* MALI */
#define COUNTRY_MT  "MT"    /* MALTA */
#define COUNTRY_MH  "MH"    /* MARSHALL ISLANDS */
#define COUNTRY_MQ  "MQ"    /* MARTINIQUE */
#define COUNTRY_MR  "MR"    /* MAURITANIA */
#define COUNTRY_MU  "MU"    /* MAURITIUS */
#define COUNTRY_YT  "YT"    /* MAYOTTE */
#define COUNTRY_MX  "MX"    /* MEXICO */
#define COUNTRY_FM  "FM"    /* MICRONESIA, FEDERATED STATES OF */
#define COUNTRY_MD  "MD"    /* MOLDOVA, REPUBLIC OF */
#define COUNTRY_MC  "MC"    /* MONACO */
#define COUNTRY_MN  "MN"    /* MONGOLIA */
#define COUNTRY_MS  "MS"    /* MONTSERRAT */
#define COUNTRY_MA  "MA"    /* MOROCCO */
#define COUNTRY_MZ  "MZ"    /* MOZAMBIQUE */
#define COUNTRY_MM  "MM"    /* MYANMAR */
#define COUNTRY_NA  "NA"    /* NAMIBIA */
#define COUNTRY_NR  "NR"    /* NAURU */
#define COUNTRY_NP  "NP"    /* NEPAL */
#define COUNTRY_NL  "NL"    /* NETHERLANDS */
#define COUNTRY_AN  "AN"    /* NETHERLANDS ANTILLES */
#define COUNTRY_NC  "NC"    /* NEW CALEDONIA */
#define COUNTRY_NZ  "NZ"    /* NEW ZEALAND */
#define COUNTRY_NI  "NI"    /* NICARAGUA */
#define COUNTRY_NE  "NE"    /* NIGER */
#define COUNTRY_NG  "NG"    /* NIGERIA */
#define COUNTRY_NU  "NU"    /* NIUE */
#define COUNTRY_NF  "NF"    /* NORFOLK ISLAND */
#define COUNTRY_MP  "MP"    /* NORTHERN MARIANA ISLANDS */
#define COUNTRY_NO  "NO"    /* NORWAY */
#define COUNTRY_OM  "OM"    /* OMAN */
#define COUNTRY_PK  "PK"    /* PAKISTAN */
#define COUNTRY_PW  "PW"    /* PALAU */
#define COUNTRY_PA  "PA"    /* PANAMA */
#define COUNTRY_PG  "PG"    /* PAPUA NEW GUINEA */
#define COUNTRY_PY  "PY"    /* PARAGUAY */
#define COUNTRY_PE  "PE"    /* PERU */
#define COUNTRY_PH  "PH"    /* PHILIPPINES */
#define COUNTRY_PN  "PN"    /* PITCAIRN */
#define COUNTRY_PL  "PL"    /* POLAND */
#define COUNTRY_PT  "PT"    /* PORTUGAL */
#define COUNTRY_PR  "PR"    /* PUERTO RICO */
#define COUNTRY_QA  "QA"    /* QATAR */
#define COUNTRY_RE  "RE"    /* REUNION */
#define COUNTRY_RO  "RO"    /* ROMANIA */
#define COUNTRY_RU  "RU"    /* RUSSIAN FEDERATION */
#define COUNTRY_RW  "RW"    /* RWANDA */
#define COUNTRY_KN  "KN"    /* SAINT KITTS AND NEVIS */
#define COUNTRY_LC  "LC"    /* SAINT LUCIA */
#define COUNTRY_VC  "VC"    /* SAINT VINCENT AND THE GRENADINES */
#define COUNTRY_WS  "WS"    /* SAMOA */
#define COUNTRY_SM  "SM"    /* SAN MARINO */
#define COUNTRY_ST  "ST"    /* SAO TOME AND PRINCIPE */
#define COUNTRY_SA  "SA"    /* SAUDI ARABIA */
#define COUNTRY_SN  "SN"    /* SENEGAL */
#define COUNTRY_SC  "SC"    /* SEYCHELLES */
#define COUNTRY_SL  "SL"    /* SIERRA LEONE */
#define COUNTRY_SG  "SG"    /* SINGAPORE */
#define COUNTRY_SK  "SK"    /* SLOVAKIA (Slovak Republic) */
#define COUNTRY_SI  "SI"    /* SLOVENIA */
#define COUNTRY_SB  "SB"    /* SOLOMON ISLANDS */
#define COUNTRY_SO  "SO"    /* SOMALIA */
#define COUNTRY_ZA  "ZA"    /* SOUTH AFRICA */
#define COUNTRY_ES  "ES"    /* SPAIN */
#define COUNTRY_LK  "LK"    /* SRI LANKA */
#define COUNTRY_SH  "SH"    /* ST. HELENA */
#define COUNTRY_PM  "PM"    /* ST. PIERRE AND MIQUELON */
#define COUNTRY_SD  "SD"    /* SUDAN */
#define COUNTRY_SR  "SR"    /* SURINAME */
#define COUNTRY_SJ  "SJ"    /* SVALBARD AND JAN MAYEN ISLANDS */
#define COUNTRY_SZ  "SZ"    /* SWAZILAND */
#define COUNTRY_SE  "SE"    /* SWEDEN */
#define COUNTRY_CH  "CH"    /* SWITZERLAND */
#define COUNTRY_SY  "SY"    /* SYRIAN ARAB REPUBLIC */
#define COUNTRY_TW  "TW"    /* TAIWAN, PROVINCE OF CHINA */
#define COUNTRY_TJ  "TJ"    /* TAJIKISTAN */
#define COUNTRY_TZ  "TZ"    /* TANZANIA, UNITED REPUBLIC OF */
#define COUNTRY_TH  "TH"    /* THAILAND */
#define COUNTRY_TG  "TG"    /* TOGO */
#define COUNTRY_TK  "TK"    /* TOKELAU */
#define COUNTRY_TO  "TO"    /* TONGA */
#define COUNTRY_TT  "TT"    /* TRINIDAD AND TOBAGO */
#define COUNTRY_TN  "TN"    /* TUNISIA */
#define COUNTRY_TR  "TR"    /* TURKEY */
#define COUNTRY_TM  "TM"    /* TURKMENISTAN */
#define COUNTRY_TC  "TC"    /* TURKS AND CAICOS ISLANDS */
#define COUNTRY_TV  "TV"    /* TUVALU */
#define COUNTRY_UG  "UG"    /* UGANDA */
#define COUNTRY_UA  "UA"    /* UKRAINE */
#define COUNTRY_AE  "AE"    /* UNITED ARAB EMIRATES */
#define COUNTRY_GB  "GB"    /* UNITED KINGDOM */
#define COUNTRY_US  "US"    /* UNITED STATES */
#define COUNTRY_UM  "UM"    /* UNITED STATES MINOR OUTLYING ISLANDS */
#define COUNTRY_UY  "UY"    /* URUGUAY */
#define COUNTRY_UZ  "UZ"    /* UZBEKISTAN */
#define COUNTRY_VU  "VU"    /* VANUATU */
#define COUNTRY_VA  "VA"    /* VATICAN CITY STATE (HOLY SEE) */
#define COUNTRY_VE  "VE"    /* VENEZUELA */
#define COUNTRY_VN  "VN"    /* VIET NAM */
#define COUNTRY_VG  "VG"    /* VIRGIN ISLANDS (BRITISH) */
#define COUNTRY_VI  "VI"    /* VIRGIN ISLANDS (U.S.) */
#define COUNTRY_WF  "WF"    /* WALLIS AND FUTUNA ISLANDS */
#define COUNTRY_EH  "EH"    /* WESTERN SAHARA */
#define COUNTRY_YE  "YE"    /* YEMEN */
#define COUNTRY_YU  "YU"    /* YUGOSLAVIA */
#define COUNTRY_ZR  "ZR"    /* ZAIRE */
#define COUNTRY_ZM  "ZM"    /* ZAMBIA */
#define COUNTRY_ZW  "ZW"    /* ZIMBABWE */

// Коды языков

#define LANG_AA    "aa"     /* Afar */
#define LANG_AB    "ab"     /* Abkhazian */
#define LANG_AF    "af"     /* Afrikaans */
#define LANG_AM    "am"     /* Amharic */
#define LANG_AR    "ar"     /* Arabic */
#define LANG_AS    "as"     /* Assamese */
#define LANG_AY    "ay"     /* Aymara */
#define LANG_AZ    "az"     /* Azerbaijani */
#define LANG_BA    "ba"     /* Bashkir */
#define LANG_BE    "be"     /* Byelorussian */
#define LANG_BG    "bg"     /* Bulgarian */
#define LANG_BH    "bh"     /* Bihari */
#define LANG_BI    "bi"     /* Bislama */
#define LANG_BN    "bn"     /* Bengali Bangla */
#define LANG_BO    "bo"     /* Tibetan */
#define LANG_BR    "br"     /* Breton */
#define LANG_CA    "ca"     /* Catalan */
#define LANG_CO    "co"     /* Corsican */
#define LANG_CS    "cs"     /* Czech */
#define LANG_CY    "cy"     /* Welsh */
#define LANG_DA    "da"     /* Danish */
#define LANG_DE    "de"     /* German */
#define LANG_DZ    "dz"     /* Bhutani */
#define LANG_EL    "el"     /* Greek */
#define LANG_EN    "en"     /* English */
#define LANG_EO    "eo"     /* Esperanto */
#define LANG_ES    "es"     /* Spanish */
#define LANG_ET    "et"     /* Estonian */
#define LANG_EU    "eu"     /* Basque */
#define LANG_FA    "fa"     /* Persian */
#define LANG_FI    "fi"     /* Finnish */
#define LANG_FJ    "fj"     /* Fiji */
#define LANG_FO    "fo"     /* Faeroese */
#define LANG_FR    "fr"     /* French */
#define LANG_FY    "fy"     /* Frisian */
#define LANG_GA    "ga"     /* Irish */
#define LANG_GD    "gd"     /* Scots Gaelic */
#define LANG_GL    "gl"     /* Galician */
#define LANG_GN    "gn"     /* Guarani */
#define LANG_GU    "gu"     /* Gujarati */
#define LANG_HA    "ha"     /* Hausa */
#define LANG_HI    "hi"     /* Hindi */
#define LANG_HR    "hr"     /* Croatian */
#define LANG_HU    "hu"     /* Hungarian */
#define LANG_HY    "hy"     /* Armenian */
#define LANG_IA    "ia"     /* Interlingua */
#define LANG_IE    "ie"     /* Interlingue */
#define LANG_IK    "ik"     /* Inupiak */
#define LANG_IN    "in"     /* Indonesian */
#define LANG_IS    "is"     /* Icelandic */
#define LANG_IT    "it"     /* Italian */
#define LANG_IW    "iw"     /* Hebrew */
#define LANG_JA    "ja"     /* Japanese */
#define LANG_JI    "ji"     /* Yiddish */
#define LANG_JW    "jw"     /* Javanese */
#define LANG_KA    "ka"     /* Georgian */
#define LANG_KK    "kk"     /* Kazakh */
#define LANG_KL    "kl"     /* Greenlandic */
#define LANG_KM    "km"     /* Cambodian */
#define LANG_KN    "kn"     /* Kannada */
#define LANG_KO    "ko"     /* Korean */
#define LANG_KS    "ks"     /* Kashmiri */
#define LANG_KU    "ku"     /* Kurdish */
#define LANG_KY    "ky"     /* Kirghiz */
#define LANG_LA    "la"     /* Latin */
#define LANG_LN    "ln"     /* Lingala */
#define LANG_LO    "lo"     /* Laothian */
#define LANG_LT    "lt"     /* Lithuanian */
#define LANG_LV    "lv"     /* Latvian,Lettish */
#define LANG_MG    "mg"     /* Malagasy */
#define LANG_MI    "mi"     /* Maori */
#define LANG_MK    "mk"     /* Macedonian */
#define LANG_ML    "ml"     /* Malayalam */
#define LANG_MN    "mn"     /* Mongolian */
#define LANG_MO    "mo"     /* Moldavian */
#define LANG_MR    "mr"     /* Marathi */
#define LANG_MS    "ms"     /* Malay */
#define LANG_MT    "mt"     /* Maltese */
#define LANG_MY    "my"     /* Burmese */
#define LANG_NA    "na"     /* Nauru */
#define LANG_NE    "ne"     /* Nepali */
#define LANG_NL    "nl"     /* Dutch */
#define LANG_NO    "no"     /* Norwegian */
#define LANG_OC    "oc"     /* Occitan */
#define LANG_OM    "om"     /* (Afan)Oromo */
#define LANG_OR    "or"     /* Oriya */
#define LANG_PA    "pa"     /* Punjabi */
#define LANG_PL    "pl"     /* Polish */
#define LANG_PS    "ps"     /* Pashto,Pushto */
#define LANG_PT    "pt"     /* Portuguese  */
#define LANG_QU    "qu"     /* Quechua */
#define LANG_RM    "rm"     /* Rhaeto-Romance */
#define LANG_RN    "rn"     /* Kirundi */
#define LANG_RO    "ro"     /* Romanian */
#define LANG_RU    "ru"     /* Russian */
#define LANG_RW    "rw"     /* Kinyarwanda */
#define LANG_SA    "sa"     /* Sanskrit */
#define LANG_SD    "sd"     /* Sindhi */
#define LANG_SG    "sg"     /* Sangro */
#define LANG_SH    "sh"     /* Serbo-Croatian */
#define LANG_SI    "si"     /* Singhalese */
#define LANG_SK    "sk"     /* Slovak */
#define LANG_SL    "sl"     /* Slovenian */
#define LANG_SM    "sm"     /* Samoan */
#define LANG_SN    "sn"     /* Shona */
#define LANG_SO    "so"     /* Somali */
#define LANG_SQ    "sq"     /* Albanian */
#define LANG_SR    "sr"     /* Serbian */
#define LANG_SS    "ss"     /* Siswati */
#define LANG_ST    "st"     /* Sesotho */
#define LANG_SU    "su"     /* Sundanese */
#define LANG_SV    "sv"     /* Swedish */
#define LANG_SW    "sw"     /* Swahili */
#define LANG_TA    "ta"     /* Tamil */
#define LANG_TE    "te"     /* Tegulu */
#define LANG_TG    "tg"     /* Tajik */
#define LANG_TH    "th"     /* Thai */
#define LANG_TI    "ti"     /* Tigrinya */
#define LANG_TK    "tk"     /* Turkmen */
#define LANG_TL    "tl"     /* Tagalog */
#define LANG_TN    "tn"     /* Setswanato Tonga */
#define LANG_TR    "tr"     /* Turkish */
#define LANG_TS    "ts"     /* Tsonga */
#define LANG_TT    "tt"     /* Tatar */
#define LANG_TW    "tw"     /* Twi */
#define LANG_UK    "uk"     /* Ukrainian */
#define LANG_UR    "ur"     /* Urdu */
#define LANG_UZ    "uz"     /* Uzbek */
#define LANG_VI    "vi"     /* Vietnamese */
#define LANG_VO    "vo"     /* Volapuk */
#define LANG_WO    "wo"     /* Wolof */
#define LANG_XH    "xh"     /* Xhosa */
#define LANG_YO    "yo"     /* Yoruba */
#define LANG_ZH    "zh"     /* Chinese */
#define LANG_ZU    "zu"     /* Zulu */

// Графические состояния

#define   GMODE_PAGE_DESCRIPTION       0x0001
#define   GMODE_PATH_OBJECT            0x0002
#define   GMODE_TEXT_OBJECT            0x0004
#define   GMODE_CLIPPING_PATH          0x0008
#define   GMODE_SHADING                0x0010
#define   GMODE_INLINE_IMAGE           0x0020
#define   GMODE_EXTERNAL_OBJECT        0x0040

//-----------------------------------------------------------------------------

#endif /* _CONSTS_H */

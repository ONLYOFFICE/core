#include "../include/applicationmanager.h"

CAscKeyboardChecker::CAscKeyboardChecker()
{
    m_nLanguage = -1;
}

CAscKeyboardChecker::~CAscKeyboardChecker()
{
}

void CAscKeyboardChecker::Check(CAscApplicationManager* pManager, int nLangInput)
{
#if 0
    GUITHREADINFO Gti;
    ::ZeroMemory ( &Gti,sizeof(GUITHREADINFO));
    Gti.cbSize = sizeof( GUITHREADINFO );
    ::GetGUIThreadInfo(0,&Gti);
    m_dwThread = GetWindowThreadProcessId(Gti.hwndActive, 0);
#endif

    int nLang = nLangInput;
    if (nLang == -1)
    {        
        nLang = pManager->GetPlatformKeyboardLayout();
    }

    if (nLang == m_nLanguage)
        return;

    m_nLanguage = nLang;

    this->Send(pManager);

#if 1
    std::string sLang = "";

    switch (m_nLanguage)
    {
    case 0x0436: sLang = "Afrikaans"; break;
    case 0x041C: sLang = "Albanian"; break;
    case 0x0401: sLang = "Arabic"; break;
    case 0x1401: sLang = "Arabic Algeria"; break;
    case 0x3C01: sLang = "Arabic Bahrain"; break;
    case 0x0C01: sLang = "Arabic Egypt"; break;
    case 0x0801: sLang = "Arabic Iraq"; break;
    case 0x2C01: sLang = "Arabic Jordan"; break;
    case 0x3401: sLang = "Arabic Kuwait"; break;
    case 0x3001: sLang = "Arabic Lebanon"; break;
    case 0x1001: sLang = "Arabic Libya"; break;
    case 0x1801: sLang = "Arabic Morocco"; break;
    case 0x2001: sLang = "Arabic Oman"; break;
    case 0x4001: sLang = "Arabic Qatar"; break;
    //case 0x0401: sLang = "Arabic Saudi Arabia"; break;
    case 0x2801: sLang = "Arabic Syria"; break;
    case 0x1C01: sLang = "Arabic Tunisia"; break;
    case 0x3801: sLang = "Arabic U.A.E"; break;
    case 0x2401: sLang = "Arabic Yemen"; break;
    case 0x042B: sLang = "Armenian"; break;
    case 0x044D: sLang = "Assamese"; break;
    case 0x082C: sLang = "Azeri Cyrillic"; break;
    case 0x042C: sLang = "Azeri Latin"; break;
    case 0x042D: sLang = "Basque"; break;
    case 0x0813: sLang = "Belgian Dutch"; break;
    case 0x080C: sLang = "Belgian French"; break;
    case 0x0445: sLang = "Bengali"; break;
    case 0x0416: sLang = "Portuguese (Brazil)"; break;
    case 0x0402: sLang = "Bulgarian"; break;
    case 0x0455: sLang = "Burmese"; break;
    case 0x0423: sLang = "Byelorussian (Belarusian)"; break;
    case 0x0403: sLang = "Catalan"; break;
    case 0x0C04: sLang = "Chinese Hong Kong SAR"; break;
    case 0x1404: sLang = "Chinese Macau SAR"; break;
    case 0x0804: sLang = "Chinese Simplified"; break;
    case 0x1004: sLang = "Chinese Singapore"; break;
    case 0x0404: sLang = "Chinese Traditional"; break;
    case 0x041A: sLang = "Croatian"; break;
    case 0x0405: sLang = "Czech"; break;
    case 0x0406: sLang = "Danish"; break;
    case 0x0413: sLang = "Dutch"; break;
    case 0x0C09: sLang = "English Australia"; break;
    case 0x2809: sLang = "English Belize"; break;
    case 0x1009: sLang = "English Canadian"; break;
    case 0x2409: sLang = "English Caribbean"; break;
    case 0x1813: sLang = "English Ireland"; break;
    case 0x2009: sLang = "English Jamaica"; break;
    case 0x1409: sLang = "English New Zealand"; break;
    case 0x3409: sLang = "English Philippines"; break;
    case 0x1C09: sLang = "English South Africa"; break;
    case 0x2C09: sLang = "English Trinidad"; break;
    case 0x0809: sLang = "English U.K."; break;
    case 0x0409: sLang = "English U.S."; break;
    case 0x3009: sLang = "English Zimbabwe"; break;
    case 0x0425: sLang = "Estonian"; break;
    case 0x0438: sLang = "Faeroese"; break;
    case 0x0429: sLang = "Farsi"; break;
    case 0x040B: sLang = "Finnish"; break;
    case 0x040C: sLang = "French"; break;
    case 0x2C0C: sLang = "French Cameroon"; break;
    case 0x0C0C: sLang = "French Canadian"; break;
    case 0x300C: sLang = "French Cote d'Ivoire"; break;
    case 0x140C: sLang = "French Luxembourg"; break;
    case 0x340C: sLang = "French Mali"; break;
    case 0x180C: sLang = "French Monaco"; break;
    case 0x200C: sLang = "French Reunion"; break;
    case 0x280C: sLang = "French Senegal"; break;
    case 0x1C0C: sLang = "French West Indies"; break;
    case 0x240C: sLang = "French Congo (DRC)"; break;
    case 0x0462: sLang = "Frisian Netherlands"; break;
    case 0x083C: sLang = "Gaelic Ireland"; break;
    case 0x043C: sLang = "Gaelic Scotland"; break;
    case 0x0456: sLang = "Galician"; break;
    case 0x0437: sLang = "Georgian"; break;
    case 0x0407: sLang = "German"; break;
    case 0x0C07: sLang = "German Austria"; break;
    case 0x1407: sLang = "German Liechtenstein"; break;
    case 0x1007: sLang = "German Luxembourg"; break;
    case 0x0408: sLang = "Greek"; break;
    case 0x0447: sLang = "Gujarati"; break;
    case 0x040D: sLang = "Hebrew"; break;
    case 0x0439: sLang = "Hindi"; break;
    case 0x040E: sLang = "Hungarian"; break;
    case 0x040F: sLang = "Icelandic"; break;
    case 0x0421: sLang = "Indonesian"; break;
    case 0x0410: sLang = "Italian"; break;
    case 0x0411: sLang = "Japanese"; break;
    case 0x044B: sLang = "Kannada"; break;
    case 0x0460: sLang = "Kashmiri"; break;
    case 0x043F: sLang = "Kazakh"; break;
    case 0x0453: sLang = "Khmer"; break;
    case 0x0440: sLang = "Kirghiz"; break;
    case 0x0457: sLang = "Konkani"; break;
    case 0x0412: sLang = "Korean"; break;
    case 0x0454: sLang = "Lao"; break;
    case 0x0426: sLang = "Latvian"; break;
    case 0x0427: sLang = "Lithuanian"; break;
    case 0x042F: sLang = "FYRO Macedonian"; break;
    case 0x044C: sLang = "Malayalam"; break;
    case 0x083E: sLang = "Malay Brunei Darussalam"; break;
    case 0x043E: sLang = "Malaysian"; break;
    case 0x043A: sLang = "Maltese"; break;
    case 0x0458: sLang = "Manipuri"; break;
    case 0x044E: sLang = "Marathi"; break;
    case 0x0450: sLang = "Mongolian"; break;
    case 0x0461: sLang = "Nepali"; break;
    case 0x0414: sLang = "Norwegian Bokmol"; break;
    case 0x0814: sLang = "Norwegian Nynorsk"; break;
    case 0x0448: sLang = "Oriya"; break;
    case 0x0415: sLang = "Polish"; break;
    case 0x0816: sLang = "Portuguese"; break;
    case 0x0446: sLang = "Punjabi"; break;
    case 0x0417: sLang = "Rhaeto-Romanic"; break;
    case 0x0418: sLang = "Romanian"; break;
    case 0x0818: sLang = "Romanian Moldova"; break;
    case 0x0419: sLang = "Russian"; break;
    case 0x0819: sLang = "Russian Moldova"; break;
    case 0x043B: sLang = "Sami Lappish"; break;
    case 0x044F: sLang = "Sanskrit"; break;
    case 0x0C1A: sLang = "Serbian Cyrillic"; break;
    case 0x081A: sLang = "Serbian Latin"; break;
    case 0x0430: sLang = "Sesotho"; break;
    case 0x0459: sLang = "Sindhi"; break;
    case 0x041B: sLang = "Slovak"; break;
    case 0x0424: sLang = "Slovenian"; break;
    case 0x042E: sLang = "Sorbian"; break;
    case 0x040A: sLang = "Spanish (Traditional)"; break;
    case 0x2C0A: sLang = "Spanish Argentina"; break;
    case 0x400A: sLang = "Spanish Bolivia"; break;
    case 0x340A: sLang = "Spanish Chile"; break;
    case 0x240A: sLang = "Spanish Colombia"; break;
    case 0x140A: sLang = "Spanish Costa Rica"; break;
    case 0x1C0A: sLang = "Spanish Dominican Republic"; break;
    case 0x300A: sLang = "Spanish Ecuador"; break;
    case 0x440A: sLang = "Spanish El Salvador"; break;
    case 0x100A: sLang = "Spanish Guatemala"; break;
    case 0x480A: sLang = "Spanish Honduras"; break;
    case 0x4C0A: sLang = "Spanish Nicaragua"; break;
    case 0x180A: sLang = "Spanish Panama"; break;
    case 0x3C0A: sLang = "Spanish Paraguay"; break;
    case 0x280A: sLang = "Spanish Peru"; break;
    case 0x500A: sLang = "Spanish Puerto Rico"; break;
    case 0x0C0A: sLang = "Spanish Spain (Modern Sort)"; break;
    case 0x380A: sLang = "Spanish Uruguay"; break;
    case 0x200A: sLang = "Spanish Venezuela"; break;
    //case 0x0430: sLang = "Sutu"; break;
    case 0x0441: sLang = "Swahili"; break;
    case 0x041D: sLang = "Swedish"; break;
    case 0x081D: sLang = "Swedish Finland"; break;
    case 0x100C: sLang = "Swiss French"; break;
    case 0x0807: sLang = "Swiss German"; break;
    case 0x0810: sLang = "Swiss Italian"; break;
    case 0x0428: sLang = "Tajik"; break;
    case 0x0449: sLang = "Tamil"; break;
    case 0x0444: sLang = "Tatar"; break;
    case 0x044A: sLang = "Telugu"; break;
    case 0x041E: sLang = "Thai"; break;
    case 0x0451: sLang = "Tibetan"; break;
    case 0x0431: sLang = "Tsonga"; break;
    case 0x0432: sLang = "Tswana"; break;
    case 0x041F: sLang = "Turkish"; break;
    case 0x0442: sLang = "Turkmen"; break;
    case 0x0422: sLang = "Ukrainian"; break;
    case 0x0420: sLang = "Urdu"; break;
    case 0x0843: sLang = "Uzbek Cyrillic"; break;
    case 0x0443: sLang = "Uzbek Latin"; break;
    case 0x0433: sLang = "Venda"; break;
    case 0x042A: sLang = "Vietnamese"; break;
    case 0x0452: sLang = "Welsh"; break;
    case 0x0434: sLang = "Xhosa"; break;
    case 0x0435: sLang = "Zulu"; break;
    default:
        break;
    }
#endif
}

void CAscKeyboardChecker::Send(CAscApplicationManager* pManager)
{
    NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
    NSEditorApi::CAscKeyboardLayout* pData = new NSEditorApi::CAscKeyboardLayout();
    pData->put_Language(m_nLanguage);
    pEvent->m_pData = pData;
    pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_KEYBOARDLAYOUT;
    pManager->Apply(pEvent);
}

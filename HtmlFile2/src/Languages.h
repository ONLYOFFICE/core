#ifndef LANGUAGES_LIST_H
#define LANGUAGES_LIST_H

#include <map>
#include <string>
#include <algorithm>

const static std::map<std::wstring, std::wstring> m_Languages_HTML
{
	//Язык, Регион
	{L"en", L"US"}, // Английский - США
	{L"ru", L"RU"}, // Русский - Россия
	{L"es", L"ES"}, // Испанский - Испания
	{L"de", L"DE"}, // Немецкий - Германия
	{L"fr", L"FR"}, // Французский - Франция
	{L"it", L"IT"}, // Итальянский - Италия
	{L"pt", L"PT"}, // Португальский - Португалия
	{L"pl", L"PL"}, // Польский - Польша
	{L"nl", L"NL"}, // Нидерландский - Нидерланды
	{L"sv", L"SE"}, // Шведский - Швеция
	{L"nb", L"NO"}, // Норвежский - Новегия
	{L"da", L"DK"}, // Датский - Дания
	{L"fi", L"FI"}, // Финский - Финляндия
	{L"el", L"GR"}, // Греческий - Греция
	{L"tr", L"TR"}, // Турецкий - Турция
	{L"ar", L"SA"}, // Арабский - Саудовская Аравия
	{L"he", L"IL"}, // Иврит - Израиль
	{L"ja", L"JP"}, // Японский - Япония
	{L"zh", L"CN"}, // Китайский (упрощенный) - Китай
	{L"hu", L"HU"}, // Венгерский - Венгрия
	{L"cs", L"CZ"}, // Чешский  - Чехия
	{L"ro", L"RO"}, // Румынский - Румыния
	{L"bg", L"BG"}, // Болгарский - Болгария
	{L"hr", L"HR"}, // Хорватский - Хорватия
	{L"sr", L"Latn-RS"}, //Сербский - Сербия
	{L"sl", L"SI"}, // Словенский - Словения
	{L"lt", L"LT"}, // Литовский - Литва
	{L"lv", L"LV"}, // Латышский - Латвия
	{L"et", L"EE"}, // Эстонский - Эстония
	{L"uk", L"UA"}, // Украинский - Украина
	{L"be", L"BY"}, // Белорусский - Беларусь
	{L"kk", L"KZ"}, // Казахский - Казахстан
	{L"hi", L"IN"}, // Хинди - Индия
	{L"th", L"TH"}, // Тайский - Таиланд
	{L"vi", L"VN"}, // Вьетнамский - Вьетнам
	{L"id", L"ID"}, // Индонезийский - Индонезия
	{L"ms", L"MY"}, // Малайский - Малайзия
	{L"fil", L"PH"}, // Филиппинский - Филиппины
	{L"ko", L"KR"}, // Корейский - Южная Корея
	{L"is", L"IS"}, // Исландский - Исландия
	{L"ga", L"IE"}, // Ирландский - Ирландия
	{L"cy", L"GB"}, // Валлийский - Великобритания
	{L"ca", L"ES"}, // Каталанский - Испания
	{L"eu", L"ES"}, // Баскский - Испания
	{L"gl", L"ES"}, // Галисийский - Испания
	{L"af", L"ZA"}, // Африкаанс - Южная Африка
	{L"zu", L"ZA"}, // Зулу - Южная Африка
	{L"ha", L"Latn-NG"}, // Хауса - Нигерия
	{L"yo", L"NG"}, // Йоруба - Нигерия
	{L"sw", L"KE"}, // Суахили - Кения
	{L"am", L"ET"}, // Амхарский - Эфиопия
	{L"ti", L"ET"}, // Тигринья - Эфиопия
	{L"ur", L"PK"}, // Урду - Пакистан
	{L"pa", L"IN"}, // Панджаби - Индия
	{L"gu", L"IN"}, // Гуджарати - Индия
	{L"ta", L"IN"}, // Тамильский - Индия
	{L"te", L"IN"}, // Телугу - Индия
	{L"ml", L"IN"}, // Малаялам - Индия
	{L"kn", L"IN"}  // Каннада - Индия
};

static std::wstring IndentifyLanguage(std::wstring wsLanguage)
{
	std::transform(wsLanguage.begin(), wsLanguage.end(), wsLanguage.begin(), towlower);

	std::map<std::wstring, std::wstring>::const_iterator itFounded = m_Languages_HTML.find(wsLanguage);

	if (m_Languages_HTML.end() != itFounded)
		return itFounded->first + L"-" + itFounded->second;

	return std::wstring();
}

#endif // LANGUAGES_LIST_H

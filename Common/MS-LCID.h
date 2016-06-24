/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include <string>
#include <map>

class MS_LCID_converter
{
public:
	std::map<int, std::wstring> m_mapLCID;

	std::wstring get_wstring(int id)
	{
		std::map<int, std::wstring>::iterator pInd = m_mapLCID.find(id);
		if (pInd != m_mapLCID.end())
		{
			return pInd->second;
		}
		return L"";
	}
	MS_LCID_converter()
	{
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0401, L"ar-SA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0402, L"bg-BG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0403, L"ca-ES"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0404, L"zh-TW"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0405, L"cs-CZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0406, L"da-DK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0407, L"de-DE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0408, L"el-GR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0409, L"en-US"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x040a, L"es-ES_tradnl"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x040b, L"fi-FI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x040c, L"fr-FR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x040d, L"he-IL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x040e, L"hu-HU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x040f, L"is-IS"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0410, L"it-IT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0411, L"ja-JP"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0412, L"ko-KR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0413, L"nl-NL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0414, L"nb-NO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0415, L"pl-PL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0416, L"pt-BR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0417, L"rm-CH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0418, L"ro-RO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0419, L"ru-RU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x041a, L"hr-HR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x041b, L"sk-SK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x041c, L"sq-AL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x041d, L"sv-SE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x041e, L"th-TH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x041f, L"tr-TR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0420, L"ur-PK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0421, L"id-ID"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0422, L"uk-UA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0423, L"be-BY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0424, L"sl-SI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0425, L"et-EE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0426, L"lv-LV"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0427, L"lt-LT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0428, L"tg-Cyrl-TJ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0429, L"fa-IR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x042a, L"vi-VN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x042b, L"hy-AM"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x042c, L"az-Latn-AZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x042d, L"eu-ES"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x042e, L"hsb-DE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x042f, L"mk-MK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0430, L"st-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0431, L"ts-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0432, L"tn-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0433, L"ve-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0434, L"xh-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0435, L"zu-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0436, L"af-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0437, L"ka-GE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0438, L"fo-FO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0439, L"hi-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x043a, L"mt-MT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x043b, L"se-NO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x043d, L"yi-Hebr"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x043e, L"ms-MY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x043f, L"kk-KZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0440, L"ky-KG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0441, L"sw-KE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0442, L"tk-TM"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0443, L"uz-Latn-UZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0444, L"tt-RU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0445, L"bn-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0446, L"pa-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0447, L"gu-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0448, L"or-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0449, L"ta-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x044a, L"te-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x044b, L"kn-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x044c, L"ml-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x044d, L"as-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x044e, L"mr-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x044f, L"sa-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0450, L"mn-MN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0451, L"bo-CN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0452, L"cy-GB"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0453, L"km-KH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0454, L"lo-LA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0455, L"my-MM"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0456, L"gl-ES"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0457, L"kok-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0458, L"mni-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0459, L"sd-Deva-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x045a, L"syr-SY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x045b, L"si-LK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x045c, L"chr-Cher-US"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x045d, L"iu-Cans-CA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x045e, L"am-ET"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x045f, L"tzm-Arab-MA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0460, L"ks-Arab"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0461, L"ne-NP"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0462, L"fy-NL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0463, L"ps-AF"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0464, L"fil-PH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0465, L"dv-MV"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0466, L"bin-NG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0467, L"fuv-NG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0468, L"ha-Latn-NG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0469, L"ibb-NG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x046a, L"yo-NG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x046b, L"quz-BO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x046c, L"nso-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x046d, L"ba-RU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x046e, L"lb-LU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x046f, L"kl-GL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0470, L"ig-NG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0471, L"kr-NG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0472, L"om-ET"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0473, L"ti-ET"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0474, L"gn-PY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0475, L"haw-US"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0476, L"la-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0477, L"so-SO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0478, L"ii-CN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0479, L"pap-029"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x047a, L"arn-CL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x047c, L"moh-CA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x047e, L"br-FR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0480, L"ug-CN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0481, L"mi-NZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0482, L"oc-FR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0483, L"co-FR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0484, L"gsw-FR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0485, L"sah-RU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0486, L"qut-GT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0487, L"rw-RW"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0488, L"wo-SN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x048c, L"prs-AF"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x048d, L"plt-MG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x048e, L"zh-yue-HK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x048f, L"tdd-Tale-CN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0490, L"khb-Talu-CN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0491, L"gd-GB"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0492, L"ku-Arab-IQ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0493, L"quc-CO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0501, L"qps-ploc"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x05fe, L"qps-ploca"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0801, L"ar-IQ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0803, L"ca-ES-valencia"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0804, L"zh-CN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0807, L"de-CH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0809, L"en-GB"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x080a, L"es-MX"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x080c, L"fr-BE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0810, L"it-CH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0811, L"ja-Ploc-JP"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0813, L"nl-BE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0814, L"nn-NO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0816, L"pt-PT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0818, L"ro-MD"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0819, L"ru-MD"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x081a, L"sr-Latn-CS"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x081d, L"sv-FI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0820, L"ur-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x082c, L"az-Cyrl-AZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x082e, L"dsb-DE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0832, L"tn-BW"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x083b, L"se-SE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x083c, L"ga-IE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x083e, L"ms-BN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0843, L"uz-Cyrl-UZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0845, L"bn-BD"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0846, L"pa-Arab-PK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0849, L"ta-LK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0850, L"mn-Mong-CN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0851, L"bo-BT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0859, L"sd-Arab-PK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x085d, L"iu-Latn-CA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x085f, L"tzm-Latn-DZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0860, L"ks-Deva"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0861, L"ne-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0867, L"ff-Latn-SN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x086b, L"quz-EC"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0873, L"ti-ER"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x09ff, L"qps-plocm"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c01, L"ar-EG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c04, L"zh-HK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c07, L"de-AT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c09, L"en-AU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c0a, L"es-ES"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c0c, L"fr-CA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c1a, L"sr-Cyrl-CS"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c3b, L"se-FI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c50, L"mn-Mong-MN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c51, L"dz-BT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c5f, L"tmz-MA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x0c6b, L"quz-PE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1001, L"ar-LY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1004, L"zh-SG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1007, L"de-LU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1009, L"en-CA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x100a, L"es-GT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x100c, L"fr-CH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x101a, L"hr-BA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x103b, L"smj-NO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x105f, L"tzm-Tfng-MA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1401, L"ar-DZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1404, L"zh-MO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1407, L"de-LI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1409, L"en-NZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x140a, L"es-CR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x140c, L"fr-LU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x141a, L"bs-Latn-BA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x143b, L"smj-SE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1801, L"ar-MA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1809, L"en-IE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x180a, L"es-PA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x180c, L"fr-MC"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x181a, L"sr-Latn-BA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x183b, L"sma-NO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1c01, L"ar-TN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1c09, L"en-ZA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1c0a, L"es-DO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1c1a, L"sr-Cyrl-BA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x1c3b, L"sma-SE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2001, L"ar-OM"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2009, L"en-JM"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x200a, L"es-VE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x200c, L"fr-RE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x201a, L"bs-Cyrl-BA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x203b, L"sms-FI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2401, L"ar-YE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2409, L"en-029"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x240a, L"es-CO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x240c, L"fr-CD"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x241a, L"sr-Latn-RS"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x243b, L"smn-FI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2801, L"ar-SY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2809, L"en-BZ"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x280a, L"es-PE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x280c, L"fr-SN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x281a, L"sr-Cyrl-RS"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2c01, L"ar-JO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2c09, L"en-TT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2c0a, L"es-AR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2c0c, L"fr-CM"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x2c1a, L"sr-Latn-ME"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3001, L"ar-LB"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3009, L"en-ZW"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x300a, L"es-EC"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x300c, L"fr-CI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x301a, L"sr-Cyrl-ME"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3401, L"ar-KW"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3409, L"en-PH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x340a, L"es-CL"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x340c, L"fr-ML"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3801, L"ar-AE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3809, L"en-ID"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x380a, L"es-UY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x380c, L"fr-MA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3c01, L"ar-BH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3c09, L"en-HK"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3c0a, L"es-PY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x3c0c, L"fr-HT"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4001, L"ar-QA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4009, L"en-IN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x400a, L"es-BO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4401, L"ar-Ploc-SA"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4409, L"en-MY"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x440a, L"es-SV"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4801, L"ar-145"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4809, L"en-SG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x480a, L"es-HN"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4c09, L"en-AE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x4c0a, L"es-NI"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x5009, L"en-BH"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x500a, L"es-PR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x5409, L"en-EG"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x540a, L"es-US"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x5809, L"en-JO"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x580a, L"es-419"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x5c09, L"en-KW"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x5c0a, L"es-CU"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x6009, L"en-TR"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x6409, L"en-YE"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x641a, L"bs-Cyrl"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x681a, L"bs-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x6c1a, L"sr-Cyrl"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x701a, L"sr-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x703b, L"smn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x742c, L"az-Cyrl"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x743b, L"sms"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7804, L"zh"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7814, L"nn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x781a, L"bs"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x782c, L"az-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x783b, L"sma"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7843, L"uz-Cyrl"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7850, L"mn-Cyrl"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x785d, L"iu-Cans"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x785f, L"tzm-Tfng"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c04, L"zh-Hant"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c14, L"nb"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c1a, L"sr"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c28, L"tg-Cyrl"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c2e, L"dsb"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c3b, L"smj"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c43, L"uz-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c46, L"pa-Arab"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c50, L"mn-Mong"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c59, L"sd-Arab"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c5c, L"chr-Cher"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c5d, L"iu-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c5f, L"tzm-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c67, L"ff-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c68, L"ha-Latn"));
		m_mapLCID.insert(std::pair<int,std::wstring>( 0x7c92, L"ku-Arab"));

	}
};

static std::wstring msLCID2wstring(int id)
{
	MS_LCID_converter lcid;
	return lcid.get_wstring(id);
}
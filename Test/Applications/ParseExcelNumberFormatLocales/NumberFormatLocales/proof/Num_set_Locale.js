fs = require('fs');
//__________________________________________functions_____________________________________________________________
function setCharAt(str,index,chr) {
    if(index > str.lenarr_idxth-1) return str;
    return str.substring(0,index) + chr + str.substring(index+1);
}

function num_set_Locale(re, str) {
    var midstring;
    if (str.search(re) != -1) {
        return true
    } else {
        return false
    }
    console.log(str + midstring + re);
}

function alghoritm_of_changing_dates_and_numbes(){
    for(let i =0; i<substring_locale_dates.length; i++){
        fs.writeFileSync(inputdata+substring_locale_dates[i], substrdateinputs[i], function (err) {
            if (err) return console.log(err);
        });
        fs.writeFileSync(inputnum+substring_locale_dates[i], substrinputs[i], function (err) {
            if (err) return console.log(err);
        });
    };
}
//_________________________________________________________________________________________________________________

function ParseLocalFormatSymbol(Name){

    switch(Name) {

        case("aa-DJ"):{console.log(Name);}break;
        case("aa-ER"):{console.log(Name);}break;
        case("aa-ET"):{console.log(Name);}break;
        case("aa"):{console.log(Name);}break;
        case("af-NA"):{console.log(Name);}break;
        case("af-ZA"):{console.log(Name);}break;
        case("af"):{console.log(Name);}break;
        case("agq-CM"):{console.log(Name);}break;
        case("agq"):{console.log(Name);}break;
        case("ak-GH"):{console.log(Name);}break;
        case("ak"):{console.log(Name);}break;
        case("am-ET"):{console.log(Name);}break;
        case("am"):{console.log(Name);}break;
        case("ar-001"):{console.log(Name);}break;
        case("ar-AE"):{console.log(Name);}break;
        case("ar-BH"):{console.log(Name);}break;
        case("ar-DJ"):{console.log(Name);}break;
        case("ar-DZ"):{console.log(Name);}break;
        case("ar-EG"):{console.log(Name);}break;
        case("ar-ER"):{console.log(Name);}break;
        case("ar-IL"):{console.log(Name);}break;
        case("ar-IQ"):{console.log(Name);}break;
        case("ar-JO"):{console.log(Name);}break;
        case("ar-KM"):{console.log(Name);}break;
        case("ar-KW"):{console.log(Name);}break;
        case("ar-LB"):{console.log(Name);}break;
        case("ar-LY"):{console.log(Name);}break;
        case("ar-MA"):{console.log(Name);}break;
        case("ar-MR"):{console.log(Name);}break;
        case("ar-OM"):{console.log(Name);}break;
        case("ar-PS"):{console.log(Name);}break;
        case("ar-QA"):{console.log(Name);}break;
        case("ar-SA"):{console.log(Name);}break;
        case("ar-SD"):{console.log(Name);}break;
        case("ar-SO"):{console.log(Name);}break;
        case("ar-SS"):{console.log(Name);}break;
        case("ar-SY"):{console.log(Name);}break;
        case("ar-TD"):{console.log(Name);}break;
        case("ar-TN"):{console.log(Name);}break;
        case("ar-YE"):{console.log(Name);}break;
        case("ar"):{console.log(Name);}break;
        case("arn-CL"):{console.log(Name);}break;
        case("arn"):{console.log(Name);}break;
        case("as-IN"):{console.log(Name);}break;
        case("as"):{console.log(Name);}break;
        case("asa-TZ"):{console.log(Name);}break;
        case("asa"):{console.log(Name);}break;
        case("ast"):{console.log(Name);}break;
        case("az-Cyrl-AZ"):{console.log(Name);}break;
        case("az-Cyrl"):{console.log(Name);}break;
        case("az-Latn-AZ"):{console.log(Name);}break;
        case("az-Latn"):{console.log(Name);}break;
        case("az"):{console.log(Name);}break;
        case("ba"):{console.log(Name);}break;
        case("bas-CM"):{console.log(Name);}break;
        case("bas"):{console.log(Name);}break;
        case("be-BY"):{console.log(Name);}break;
        case("be"):{console.log(Name);}break;
        case("bem-ZM"):{console.log(Name);}break;
        case("bem"):{console.log(Name);}break;
        case("bez-TZ"):{console.log(Name);}break;
        case("bez"):{console.log(Name);}break;
        case("bg-BG"):{console.log(Name);}break;
        case("bg"):{console.log(Name);}break;
        case("bin-NG"):{console.log(Name);}break;
        case("bin"):{console.log(Name);}break;
        case("bm-Latn-ML"):{console.log(Name);}break;
        case("bm-Latn"):{console.log(Name);}break;
        case("bm"):{console.log(Name);}break;
        case("bn-BD"):{console.log(Name);}break;
        case("bn"):{console.log(Name);}break;
        case("bo-IN"):{console.log(Name);}break;
        case("bo"):{console.log(Name);}break;
        case("br"):{console.log(Name);}break;
        case("brx-IN"):{console.log(Name);}break;
        case("brx"):{console.log(Name);}break;
        case("bs-Cyrl-BA"):{console.log(Name);}break;
        case("bs-Cyrl"):{console.log(Name);}break;
        case("bs-Latn-BA"):{console.log(Name);}break;
        case("bs-Latn"):{console.log(Name);}break;
        case("bs"):{console.log(Name);}break;
        case("byn-ER"):{console.log(Name);}break;
        case("byn"):{console.log(Name);}break;
        case("ca-AD"):{console.log(Name);}break;
        case("ca"):{console.log(Name);}break;
        case("ce"):{console.log(Name);}break;
        case("cgg-UG"):{console.log(Name);}break;
        case("cgg"):{console.log(Name);}break;
        case("chr-Cher-US"):{console.log(Name);}break;
        case("chr-Cher"):{console.log(Name);}break;
        case("chr"):{console.log(Name);}break;
        case("co"):{console.log(Name);}break;
        case("cs"):{console.log(Name);}break;
        case("cu"):{console.log(Name);}break;
        case("cy-GB"):{console.log(Name);}break;
        case("cy"):{console.log(Name);}break;
        case("da-GL"):{console.log(Name);}break;
        case("da"):{console.log(Name);}break;
        case("dav-KE"):{console.log(Name);}break;
        case("dav"):{console.log(Name);}break;
        case("de-LI"):{console.log(Name);}break;
        case("de-LU"):{console.log(Name);}break;
        case("de"):{console.log(Name);}break;
        case("dje-NE"):{console.log(Name);}break;
        case("dje"):{console.log(Name);}break;
        case("dsb"):{console.log(Name);}break;
        case("dua-CM"):{console.log(Name);}break;
        case("dua"):{console.log(Name);}break;
        case("dv-MV"):{console.log(Name);}break;
        case("dv"):{console.log(Name);}break;
        case("dyo-SN"):{console.log(Name);}break;
        case("dyo"):{console.log(Name);}break;
        case("dz-BT"):{console.log(Name);}break;
        case("dz"):{console.log(Name);}break;
        case("ebu-KE"):{console.log(Name);}break;
        case("ebu"):{console.log(Name);}break;
        case("ee-GH"):{console.log(Name);}break;
        case("ee-TG"):{console.log(Name);}break;
        case("ee"):{console.log(Name);}break;
        case("el-CY"):{console.log(Name);}break;
        case("el"):{console.log(Name);}break;
        case("en-001"):{console.log(Name);}break;
        case("en-029"):{console.log(Name);}break;
        case("en-150"):{console.log(Name);}break;
        case("en-AG"):{console.log(Name);}break;
        case("en-AI"):{console.log(Name);}break;
        case("en-AS"):{console.log(Name);}break;
        case("en-AU"):{console.log(Name);}break;
        case("en-BB"):{console.log(Name);}break;
        case("en-BI"):{console.log(Name);}break;
        case("en-BM"):{console.log(Name);}break;
        case("en-BS"):{console.log(Name);}break;
        case("en-BW"):{console.log(Name);}break;
        case("en-BZ"):{console.log(Name);}break;
        case("en-CA"):{console.log(Name);}break;
        case("en-CC"):{console.log(Name);}break;
        case("en-CK"):{console.log(Name);}break;
        case("en-CM"):{console.log(Name);}break;
        case("en-CX"):{console.log(Name);}break;
        case("en-CY"):{console.log(Name);}break;
        case("en-DM"):{console.log(Name);}break;
        case("en-ER"):{console.log(Name);}break;
        case("en-FJ"):{console.log(Name);}break;
        case("en-FK"):{console.log(Name);}break;
        case("en-FM"):{console.log(Name);}break;
        case("en-GB"):{console.log(Name);}break;
        case("en-GD"):{console.log(Name);}break;
        case("en-GG"):{console.log(Name);}break;
        case("en-GH"):{console.log(Name);}break;
        case("en-GI"):{console.log(Name);}break;
        case("en-GM"):{console.log(Name);}break;
        case("en-GU"):{console.log(Name);}break;
        case("en-GY"):{console.log(Name);}break;
        case("en-HK"):{console.log(Name);}break;
        case("en-ID"):{console.log(Name);}break;
        case("en-IE"):{console.log(Name);}break;
        case("en-IL"):{console.log(Name);}break;
        case("en-IM"):{console.log(Name);}break;
        case("en-IN"):{console.log(Name);}break;
        case("en-IO"):{console.log(Name);}break;
        case("en-JE"):{console.log(Name);}break;
        case("en-JM"):{console.log(Name);}break;
        case("en-KE"):{console.log(Name);}break;
        case("en-KI"):{console.log(Name);}break;
        case("en-KN"):{console.log(Name);}break;
        case("en-KY"):{console.log(Name);}break;
        case("en-LC"):{console.log(Name);}break;
        case("en-LR"):{console.log(Name);}break;
        case("en-LS"):{console.log(Name);}break;
        case("en-MG"):{console.log(Name);}break;
        case("en-MH"):{console.log(Name);}break;
        case("en-MO"):{console.log(Name);}break;
        case("en-MP"):{console.log(Name);}break;
        case("en-MS"):{console.log(Name);}break;
        case("en-MT"):{console.log(Name);}break;
        case("en-MU"):{console.log(Name);}break;
        case("en-MW"):{console.log(Name);}break;
        case("en-MY"):{console.log(Name);}break;
        case("en-NA"):{console.log(Name);}break;
        case("en-NF"):{console.log(Name);}break;
        case("en-NG"):{console.log(Name);}break;
        case("en-NR"):{console.log(Name);}break;
        case("en-NU"):{console.log(Name);}break;
        case("en-NZ"):{console.log(Name);}break;
        case("en-PG"):{console.log(Name);}break;
        case("en-PH"):{console.log(Name);}break;
        case("en-PK"):{console.log(Name);}break;
        case("en-PN"):{console.log(Name);}break;
        case("en-PR"):{console.log(Name);}break;
        case("en-PW"):{console.log(Name);}break;
        case("en-RW"):{console.log(Name);}break;
        case("en-SB"):{console.log(Name);}break;
        case("en-SC"):{console.log(Name);}break;
        case("en-SD"):{console.log(Name);}break;
        case("en-SG"):{console.log(Name);}break;
        case("en-SH"):{console.log(Name);}break;
        case("en-SI"):{console.log(Name);}break;
        case("en-SL"):{console.log(Name);}break;
        case("en-SS"):{console.log(Name);}break;
        case("en-SX"):{console.log(Name);}break;
        case("en-SZ"):{console.log(Name);}break;
        case("en-TC"):{console.log(Name);}break;
        case("en-TK"):{console.log(Name);}break;
        case("en-TO"):{console.log(Name);}break;
        case("en-TT"):{console.log(Name);}break;
        case("en-TV"):{console.log(Name);}break;
        case("en-TZ"):{console.log(Name);}break;
        case("en-UG"):{console.log(Name);}break;
        case("en-UM"):{console.log(Name);}break;
        case("en-US"):{console.log(Name);}break;
        case("en-VC"):{console.log(Name);}break;
        case("en-VG"):{console.log(Name);}break;
        case("en-VI"):{console.log(Name);}break;
        case("en-VU"):{console.log(Name);}break;
        case("en-WS"):{console.log(Name);}break;
        case("en-ZA"):{console.log(Name);}break;
        case("en-ZM"):{console.log(Name);}break;
        case("en-ZW"):{console.log(Name);}break;
        case("en"):{console.log(Name);}break;
        case("eo-001"):{console.log(Name);}break;
        case("eo"):{console.log(Name);}break;
        case("es-419"):{console.log(Name);}break;
        case("es-AR"):{console.log(Name);}break;
        case("es-BO"):{console.log(Name);}break;
        case("es-BZ"):{console.log(Name);}break;
        case("es-CL"):{console.log(Name);}break;
        case("es-CO"):{console.log(Name);}break;
        case("es-CR"):{console.log(Name);}break;
        case("es-CU"):{console.log(Name);}break;
        case("es-DO"):{console.log(Name);}break;
        case("es-EC"):{console.log(Name);}break;
        case("es-GQ"):{console.log(Name);}break;
        case("es-GT"):{console.log(Name);}break;
        case("es-HN"):{console.log(Name);}break;
        case("es-NI"):{console.log(Name);}break;
        case("es-PA"):{console.log(Name);}break;
        case("es-PE"):{console.log(Name);}break;
        case("es-PH"):{console.log(Name);}break;
        case("es-PR"):{console.log(Name);}break;
        case("es-PY"):{console.log(Name);}break;
        case("es-SV"):{console.log(Name);}break;
        case("es-US"):{console.log(Name);}break;
        case("es-UY"):{console.log(Name);}break;
        case("es-VE"):{console.log(Name);}break;
        case("es"):{console.log(Name);}break;
        case("et-EE"):{console.log(Name);}break;
        case("et"):{console.log(Name);}break;
        case("eu"):{console.log(Name);}break;
        case("ewo-CM"):{console.log(Name);}break;
        case("ewo"):{console.log(Name);}break;
        case("fa-IR"):{console.log(Name);}break;
        case("fa"):{console.log(Name);}break;
        case("ff-Latn-BF"):{console.log(Name);}break;
        case("ff-Latn-CM"):{console.log(Name);}break;
        case("ff-Latn-GH"):{console.log(Name);}break;
        case("ff-Latn-GM"):{console.log(Name);}break;
        case("ff-Latn-GN"):{console.log(Name);}break;
        case("ff-Latn-GW"):{console.log(Name);}break;
        case("ff-Latn-LR"):{console.log(Name);}break;
        case("ff-Latn-MR"):{console.log(Name);}break;
        case("ff-Latn-NE"):{console.log(Name);}break;
        case("ff-Latn-NG"):{console.log(Name);}break;
        case("ff-Latn-SL"):{console.log(Name);}break;
        case("ff-Latn-SN"):{console.log(Name);}break;
        case("ff-Latn"):{console.log(Name);}break;
        case("ff"):{console.log(Name);}break;
        case("fi"):{console.log(Name);}break;
        case("fil-PH"):{console.log(Name);}break;
        case("fil"):{console.log(Name);}break;
        case("fo-FO"):{console.log(Name);}break;
        case("fo"):{console.log(Name);}break;
        case("fr-029"):{console.log(Name);}break;
        case("fr-BF"):{console.log(Name);}break;
        case("fr-BI"):{console.log(Name);}break;
        case("fr-BJ"):{console.log(Name);}break;
        case("fr-BL"):{console.log(Name);}break;
        case("fr-CD"):{console.log(Name);}break;
        case("fr-CF"):{console.log(Name);}break;
        case("fr-CG"):{console.log(Name);}break;
        case("fr-CI"):{console.log(Name);}break;
        case("fr-CM"):{console.log(Name);}break;
        case("fr-DJ"):{console.log(Name);}break;
        case("fr-DZ"):{console.log(Name);}break;
        case("fr-GA"):{console.log(Name);}break;
        case("fr-GF"):{console.log(Name);}break;
        case("fr-GN"):{console.log(Name);}break;
        case("fr-GP"):{console.log(Name);}break;
        case("fr-GQ"):{console.log(Name);}break;
        case("fr-HT"):{console.log(Name);}break;
        case("fr-KM"):{console.log(Name);}break;
        case("fr-LU"):{console.log(Name);}break;
        case("fr-MA"):{console.log(Name);}break;
        case("fr-MC"):{console.log(Name);}break;
        case("fr-MF"):{console.log(Name);}break;
        case("fr-MG"):{console.log(Name);}break;
        case("fr-ML"):{console.log(Name);}break;
        case("fr-MQ"):{console.log(Name);}break;
        case("fr-MR"):{console.log(Name);}break;
        case("fr-MU"):{console.log(Name);}break;
        case("fr-NC"):{console.log(Name);}break;
        case("fr-NE"):{console.log(Name);}break;
        case("fr-PF"):{console.log(Name);}break;
        case("fr-PM"):{console.log(Name);}break;
        case("fr-RE"):{console.log(Name);}break;
        case("fr-RW"):{console.log(Name);}break;
        case("fr-SC"):{console.log(Name);}break;
        case("fr-SN"):{console.log(Name);}break;
        case("fr-SY"):{console.log(Name);}break;
        case("fr-TD"):{console.log(Name);}break;
        case("fr-TG"):{console.log(Name);}break;
        case("fr-TN"):{console.log(Name);}break;
        case("fr-VU"):{console.log(Name);}break;
        case("fr-WF"):{console.log(Name);}break;
        case("fr-YT"):{console.log(Name);}break;
        case("fr"):{console.log(Name);}break;
        case("fur"):{console.log(Name);}break;
        case("fy"):{console.log(Name);}break;
        case("ga-IE"):{console.log(Name);}break;
        case("ga"):{console.log(Name);}break;
        case("gd-GB"):{console.log(Name);}break;
        case("gd"):{console.log(Name);}break;
        case("gl"):{console.log(Name);}break;
        case("gn-PY"):{console.log(Name);}break;
        case("gn"):{console.log(Name);}break;
        case("gsw-LI"):{console.log(Name);}break;
        case("gsw"):{console.log(Name);}break;
        case("gu-IN"):{console.log(Name);}break;
        case("gu"):{console.log(Name);}break;
        case("guz-KE"):{console.log(Name);}break;
        case("guz"):{console.log(Name);}break;
        case("gv-IM"):{console.log(Name);}break;
        case("gv"):{console.log(Name);}break;
        case("ha-Latn-GH"):{console.log(Name);}break;
        case("ha-Latn-NE"):{console.log(Name);}break;
        case("ha-Latn-NG"):{console.log(Name);}break;
        case("ha-Latn"):{console.log(Name);}break;
        case("ha"):{console.log(Name);}break;
        case("haw-US"):{console.log(Name);}break;
        case("haw"):{console.log(Name);}break;
        case("he-IL"):{console.log(Name);}break;
        case("he"):{console.log(Name);}break;
        case("hi-IN"):{console.log(Name);}break;
        case("hi"):{console.log(Name);}break;
        case("hr-BA"):{console.log(Name);}break;
        case("hr-HR"):{console.log(Name);}break;
        case("hr"):{console.log(Name);}break;
        case("hsb"):{console.log(Name);}break;
        case("hu"):{console.log(Name);}break;
        case("hy-AM"):{console.log(Name);}break;
        case("hy"):{console.log(Name);}break;
        case("ia-001"):{console.log(Name);}break;
        case("ia"):{console.log(Name);}break;
        case("ibb-NG"):{console.log(Name);}break;
        case("ibb"):{console.log(Name);}break;
        case("id"):{console.log(Name);}break;
        case("ig-NG"):{console.log(Name);}break;
        case("ig"):{console.log(Name);}break;
        case("ii"):{console.log(Name);}break;
        case("is-IS"):{console.log(Name);}break;
        case("is"):{console.log(Name);}break;
        case("it-SM"):{console.log(Name);}break;
        case("it"):{console.log(Name);}break;
        case("iu-Cans-CA"):{console.log(Name);}break;
        case("iu-Cans"):{console.log(Name);}break;
        case("iu-Latn-CA"):{console.log(Name);}break;
        case("iu-Latn"):{console.log(Name);}break;
        case("iu"):{console.log(Name);}break;
        case("ja"):{console.log(Name);}break;
        case("jgo-CM"):{console.log(Name);}break;
        case("jgo"):{console.log(Name);}break;
        case("jmc-TZ"):{console.log(Name);}break;
        case("jmc"):{console.log(Name);}break;
        case("jv-Java"):{console.log(Name);}break;
        case("jv"):{console.log(Name);}break;
        case("ka-GE"):{console.log(Name);}break;
        case("ka"):{console.log(Name);}break;
        case("kab-DZ"):{console.log(Name);}break;
        case("kab"):{console.log(Name);}break;
        case("kam-KE"):{console.log(Name);}break;
        case("kam"):{console.log(Name);}break;
        case("kde-TZ"):{console.log(Name);}break;
        case("kde"):{console.log(Name);}break;
        case("kea-CV"):{console.log(Name);}break;
        case("kea"):{console.log(Name);}break;
        case("khq-ML"):{console.log(Name);}break;
        case("khq"):{console.log(Name);}break;
        case("ki-KE"):{console.log(Name);}break;
        case("ki"):{console.log(Name);}break;
        case("kk"):{console.log(Name);}break;
        case("kkj-CM"):{console.log(Name);}break;
        case("kkj"):{console.log(Name);}break;
        case("kl-GL"):{console.log(Name);}break;
        case("kl"):{console.log(Name);}break;
        case("kln-KE"):{console.log(Name);}break;
        case("kln"):{console.log(Name);}break;
        case("km-KH"):{console.log(Name);}break;
        case("km"):{console.log(Name);}break;
        case("kn-IN"):{console.log(Name);}break;
        case("kn"):{console.log(Name);}break;
        case("ko-KP"):{console.log(Name);}break;
        case("ko"):{console.log(Name);}break;
        case("kok-IN"):{console.log(Name);}break;
        case("kok"):{console.log(Name);}break;
        case("kr-Latn-NG"):{console.log(Name);}break;
        case("kr-Latn"):{console.log(Name);}break;
        case("kr"):{console.log(Name);}break;
        case("ks-Arab-IN"):{console.log(Name);}break;
        case("ks-Arab"):{console.log(Name);}break;
        case("ks-Deva-IN"):{console.log(Name);}break;
        case("ks-Deva"):{console.log(Name);}break;
        case("ks"):{console.log(Name);}break;
        case("ksb-TZ"):{console.log(Name);}break;
        case("ksb"):{console.log(Name);}break;
        case("ksf-CM"):{console.log(Name);}break;
        case("ksf"):{console.log(Name);}break;
        case("ksh"):{console.log(Name);}break;
        case("ku-Arab-IQ"):{console.log(Name);}break;
        case("ku-Arab-IR"):{console.log(Name);}break;
        case("ku-Arab"):{console.log(Name);}break;
        case("ku"):{console.log(Name);}break;
        case("kw-GB"):{console.log(Name);}break;
        case("kw"):{console.log(Name);}break;
        case("ky-KG"):{console.log(Name);}break;
        case("ky"):{console.log(Name);}break;
        case("la-001"):{console.log(Name);}break;
        case("la"):{console.log(Name);}break;
        case("lag-TZ"):{console.log(Name);}break;
        case("lag"):{console.log(Name);}break;
        case("lb-LU"):{console.log(Name);}break;
        case("lb"):{console.log(Name);}break;
        case("lg-UG"):{console.log(Name);}break;
        case("lg"):{console.log(Name);}break;
        case("lkt-US"):{console.log(Name);}break;
        case("lkt"):{console.log(Name);}break;
        case("ln-AO"):{console.log(Name);}break;
        case("ln-CD"):{console.log(Name);}break;
        case("ln-CF"):{console.log(Name);}break;
        case("ln-CG"):{console.log(Name);}break;
        case("ln"):{console.log(Name);}break;
        case("lo-LA"):{console.log(Name);}break;
        case("lo"):{console.log(Name);}break;
        case("lrc-IQ"):{console.log(Name);}break;
        case("lrc-IR"):{console.log(Name);}break;
        case("lrc"):{console.log(Name);}break;
        case("lt-LT"):{console.log(Name);}break;
        case("lt"):{console.log(Name);}break;
        case("lu-CD"):{console.log(Name);}break;
        case("lu"):{console.log(Name);}break;
        case("luo-KE"):{console.log(Name);}break;
        case("luo"):{console.log(Name);}break;
        case("luy-KE"):{console.log(Name);}break;
        case("luy"):{console.log(Name);}break;
        case("lv-LV"):{console.log(Name);}break;
        case("lv"):{console.log(Name);}break;
        case("mas-KE"):{console.log(Name);}break;
        case("mas-TZ"):{console.log(Name);}break;
        case("mas"):{console.log(Name);}break;
        case("mer-KE"):{console.log(Name);}break;
        case("mer"):{console.log(Name);}break;
        case("mfe-MU"):{console.log(Name);}break;
        case("mfe"):{console.log(Name);}break;
        case("mg-MG"):{console.log(Name);}break;
        case("mg"):{console.log(Name);}break;
        case("mgh-MZ"):{console.log(Name);}break;
        case("mgh"):{console.log(Name);}break;
        case("mgo-CM"):{console.log(Name);}break;
        case("mgo"):{console.log(Name);}break;
        case("mi-NZ"):{console.log(Name);}break;
        case("mi"):{console.log(Name);}break;
        case("mk-MK"):{console.log(Name);}break;
        case("mk"):{console.log(Name);}break;
        case("ml-IN"):{console.log(Name);}break;
        case("ml"):{console.log(Name);}break;
        case("mn-Cyrl"):{console.log(Name);}break;
        case("mn-MN"):{console.log(Name);}break;
        case("mn-Mong-MN"):{console.log(Name);}break;
        case("mn-Mong"):{console.log(Name);}break;
        case("mn"):{console.log(Name);}break;
        case("mni-IN"):{console.log(Name);}break;
        case("mni"):{console.log(Name);}break;
        case("moh-CA"):{console.log(Name);}break;
        case("moh"):{console.log(Name);}break;
        case("mr-IN"):{console.log(Name);}break;
        case("mr"):{console.log(Name);}break;
        case("ms-BN"):{console.log(Name);}break;
        case("ms-MY"):{console.log(Name);}break;
        case("ms-SG"):{console.log(Name);}break;
        case("ms"):{console.log(Name);}break;
        case("mt-MT"):{console.log(Name);}break;
        case("mt"):{console.log(Name);}break;
        case("mua-CM"):{console.log(Name);}break;
        case("mua"):{console.log(Name);}break;
        case("my-MM"):{console.log(Name);}break;
        case("my"):{console.log(Name);}break;
        case("mzn-IR"):{console.log(Name);}break;
        case("mzn"):{console.log(Name);}break;
        case("naq-NA"):{console.log(Name);}break;
        case("naq"):{console.log(Name);}break;
        case("nb"):{console.log(Name);}break;
        case("nd-ZW"):{console.log(Name);}break;
        case("nd"):{console.log(Name);}break;
        case("nds"):{console.log(Name);}break;
        case("ne-IN"):{console.log(Name);}break;
        case("ne-NP"):{console.log(Name);}break;
        case("ne"):{console.log(Name);}break;
        case("nl-AW"):{console.log(Name);}break;
        case("nl-BQ"):{console.log(Name);}break;
        case("nl-CW"):{console.log(Name);}break;
        case("nl-SR"):{console.log(Name);}break;
        case("nl-SX"):{console.log(Name);}break;
        case("nl"):{console.log(Name);}break;
        case("nmg-CM"):{console.log(Name);}break;
        case("nmg"):{console.log(Name);}break;
        case("nn"):{console.log(Name);}break;
        case("nnh-CM"):{console.log(Name);}break;
        case("nnh"):{console.log(Name);}break;
        case("no"):{console.log(Name);}break;
        case("nqo-GN"):{console.log(Name);}break;
        case("nqo"):{console.log(Name);}break;
        case("nr-ZA"):{console.log(Name);}break;
        case("nr"):{console.log(Name);}break;
        case("nso-ZA"):{console.log(Name);}break;
        case("nso"):{console.log(Name);}break;
        case("nus-SS"):{console.log(Name);}break;
        case("nus"):{console.log(Name);}break;
        case("nyn-UG"):{console.log(Name);}break;
        case("nyn"):{console.log(Name);}break;
        case("oc"):{console.log(Name);}break;
        case("om-ET"):{console.log(Name);}break;
        case("om-KE"):{console.log(Name);}break;
        case("om"):{console.log(Name);}break;
        case("or-IN"):{console.log(Name);}break;
        case("or"):{console.log(Name);}break;
        case("os-GE"):{console.log(Name);}break;
        case("os"):{console.log(Name);}break;
        case("pa-IN"):{console.log(Name);}break;
        case("pa"):{console.log(Name);}break;
        case("pap-029"):{console.log(Name);}break;
        case("pap"):{console.log(Name);}break;
        case("pl"):{console.log(Name);}break;
        case("prg-001"):{console.log(Name);}break;
        case("prg"):{console.log(Name);}break;
        case("prs-AF"):{console.log(Name);}break;
        case("prs"):{console.log(Name);}break;
        case("ps-AF"):{console.log(Name);}break;
        case("ps"):{console.log(Name);}break;
        case("pt-AO"):{console.log(Name);}break;
        case("pt-CV"):{console.log(Name);}break;
        case("pt-GQ"):{console.log(Name);}break;
        case("pt-GW"):{console.log(Name);}break;
        case("pt-LU"):{console.log(Name);}break;
        case("pt-MO"):{console.log(Name);}break;
        case("pt-MZ"):{console.log(Name);}break;
        case("pt-ST"):{console.log(Name);}break;
        case("pt-TL"):{console.log(Name);}break;
        case("pt"):{console.log(Name);}break;
        case("quc-Latn-GT"):{console.log(Name);}break;
        case("quc-Latn"):{console.log(Name);}break;
        case("quc"):{console.log(Name);}break;
        case("quz-BO"):{console.log(Name);}break;
        case("quz-EC"):{console.log(Name);}break;
        case("quz-PE"):{console.log(Name);}break;
        case("quz"):{console.log(Name);}break;
        case("rm"):{console.log(Name);}break;
        case("rn-BI"):{console.log(Name);}break;
        case("rn"):{console.log(Name);}break;
        case("ro-MD"):{console.log(Name);}break;
        case("ro-RO"):{console.log(Name);}break;
        case("ro"):{console.log(Name);}break;
        case("rof-TZ"):{console.log(Name);}break;
        case("rof"):{console.log(Name);}break;
        case("ru-BY"):{console.log(Name);}break;
        case("ru-KG"):{console.log(Name);}break;
        case("ru-MD"):{console.log(Name);}break;
        case("ru-UA"):{console.log(Name);}break;
        case("ru"):{console.log(Name);}break;
        case("rw-RW"):{console.log(Name);}break;
        case("rw"):{console.log(Name);}break;
        case("rwk-TZ"):{console.log(Name);}break;
        case("rwk"):{console.log(Name);}break;
        case("sa-IN"):{console.log(Name);}break;
        case("sa"):{console.log(Name);}break;
        case("sah"):{console.log(Name);}break;
        case("saq-KE"):{console.log(Name);}break;
        case("saq"):{console.log(Name);}break;
        case("sbp-TZ"):{console.log(Name);}break;
        case("sbp"):{console.log(Name);}break;
        case("sd-Arab-PK"):{console.log(Name);}break;
        case("sd-Arab"):{console.log(Name);}break;
        case("sd-Deva-IN"):{console.log(Name);}break;
        case("sd-Deva"):{console.log(Name);}break;
        case("sd"):{console.log(Name);}break;
        case("se"):{console.log(Name);}break;
        case("seh-MZ"):{console.log(Name);}break;
        case("seh"):{console.log(Name);}break;
        case("ses-ML"):{console.log(Name);}break;
        case("ses"):{console.log(Name);}break;
        case("sg-CF"):{console.log(Name);}break;
        case("sg"):{console.log(Name);}break;
        case("shi-Latn-MA"):{console.log(Name);}break;
        case("shi-Latn"):{console.log(Name);}break;
        case("shi-Tfng-MA"):{console.log(Name);}break;
        case("shi-Tfng"):{console.log(Name);}break;
        case("shi"):{console.log(Name);}break;
        case("si"):{console.log(Name);}break;
        case("sk-SK"):{console.log(Name);}break;
        case("sk"):{console.log(Name);}break;
        case("sl-SI"):{console.log(Name);}break;
        case("sl"):{console.log(Name);}break;
        case("sma"):{console.log(Name);}break;
        case("smj"):{console.log(Name);}break;
        case("smn"):{console.log(Name);}break;
        case("sms"):{console.log(Name);}break;
        case("sn-Latn-ZW"):{console.log(Name);}break;
        case("sn-Latn"):{console.log(Name);}break;
        case("sn"):{console.log(Name);}break;
        case("so-DJ"):{console.log(Name);}break;
        case("so-ET"):{console.log(Name);}break;
        case("so-KE"):{console.log(Name);}break;
        case("so-SO"):{console.log(Name);}break;
        case("so"):{console.log(Name);}break;
        case("sq-AL"):{console.log(Name);}break;
        case("sq-MK"):{console.log(Name);}break;
        case("sq-XK"):{console.log(Name);}break;
        case("sq"):{console.log(Name);}break;
        case("sr-Cyrl-BA"):{console.log(Name);}break;
        case("sr-Cyrl-ME"):{console.log(Name);}break;
        case("sr-Cyrl-RS"):{console.log(Name);}break;
        case("sr-Cyrl-XK"):{console.log(Name);}break;
        case("sr-Cyrl"):{console.log(Name);}break;
        case("sr-Latn-BA"):{console.log(Name);}break;
        case("sr-Latn-ME"):{console.log(Name);}break;
        case("sr-Latn-RS"):{console.log(Name);}break;
        case("sr-Latn-XK"):{console.log(Name);}break;
        case("sr-Latn"):{console.log(Name);}break;
        case("sr"):{console.log(Name);}break;
        case("ss-SZ"):{console.log(Name);}break;
        case("ss-ZA"):{console.log(Name);}break;
        case("ss"):{console.log(Name);}break;
        case("ssy-ER"):{console.log(Name);}break;
        case("ssy"):{console.log(Name);}break;
        case("st-LS"):{console.log(Name);}break;
        case("st-ZA"):{console.log(Name);}break;
        case("st"):{console.log(Name);}break;
        case("sv"):{console.log(Name);}break;
        case("sw-CD"):{console.log(Name);}break;
        case("sw-KE"):{console.log(Name);}break;
        case("sw-TZ"):{console.log(Name);}break;
        case("sw-UG"):{console.log(Name);}break;
        case("sw"):{console.log(Name);}break;
        case("syr-SY"):{console.log(Name);}break;
        case("syr"):{console.log(Name);}break;
        case("ta-IN"):{console.log(Name);}break;
        case("ta-LK"):{console.log(Name);}break;
        case("ta-MY"):{console.log(Name);}break;
        case("ta-SG"):{console.log(Name);}break;
        case("ta"):{console.log(Name);}break;
        case("te-IN"):{console.log(Name);}break;
        case("te"):{console.log(Name);}break;
        case("teo-KE"):{console.log(Name);}break;
        case("teo-UG"):{console.log(Name);}break;
        case("teo"):{console.log(Name);}break;
        case("tg-Cyrl-TJ"):{console.log(Name);}break;
        case("tg-Cyrl"):{console.log(Name);}break;
        case("tg"):{console.log(Name);}break;
        case("th-TH"):{console.log(Name);}break;
        case("th"):{console.log(Name);}break;
        case("ti-ER"):{console.log(Name);}break;
        case("ti-ET"):{console.log(Name);}break;
        case("ti"):{console.log(Name);}break;
        case("tig-ER"):{console.log(Name);}break;
        case("tig"):{console.log(Name);}break;
        case("tk-TM"):{console.log(Name);}break;
        case("tk"):{console.log(Name);}break;
        case("tn-BW"):{console.log(Name);}break;
        case("tn-ZA"):{console.log(Name);}break;
        case("tn"):{console.log(Name);}break;
        case("to-TO"):{console.log(Name);}break;
        case("to"):{console.log(Name);}break;
        case("tr-CY"):{console.log(Name);}break;
        case("tr"):{console.log(Name);}break;
        case("ts-ZA"):{console.log(Name);}break;
        case("ts"):{console.log(Name);}break;
        case("tt"):{console.log(Name);}break;
        case("twq-NE"):{console.log(Name);}break;
        case("twq"):{console.log(Name);}break;
        case("tzm-Arab-MA"):{console.log(Name);}break;
        case("tzm-Arab"):{console.log(Name);}break;
        case("tzm-Latn-DZ"):{console.log(Name);}break;
        case("tzm-Latn-MA"):{console.log(Name);}break;
        case("tzm-Latn"):{console.log(Name);}break;
        case("tzm-Tfng-MA"):{console.log(Name);}break;
        case("tzm-Tfng"):{console.log(Name);}break;
        case("tzm"):{console.log(Name);}break;
        case("ug"):{console.log(Name);}break;
        case("uk-UA"):{console.log(Name);}break;
        case("uk"):{console.log(Name);}break;
        case("ur-IN"):{console.log(Name);}break;
        case("ur-PK"):{console.log(Name);}break;
        case("ur"):{console.log(Name);}break;
        case("uz-Arab-AF"):{console.log(Name);}break;
        case("uz-Arab"):{console.log(Name);}break;
        case("uz-Cyrl-UZ"):{console.log(Name);}break;
        case("uz-Cyrl"):{console.log(Name);}break;
        case("uz-Latn-UZ"):{console.log(Name);}break;
        case("uz-Latn"):{console.log(Name);}break;
        case("uz"):{console.log(Name);}break;
        case("vai-Latn-LR"):{console.log(Name);}break;
        case("vai-Latn"):{console.log(Name);}break;
        case("vai-Vaii-LR"):{console.log(Name);}break;
        case("vai-Vaii"):{console.log(Name);}break;
        case("vai"):{console.log(Name);}break;
        case("ve-ZA"):{console.log(Name);}break;
        case("ve"):{console.log(Name);}break;
        case("vi-VN"):{console.log(Name);}break;
        case("vi"):{console.log(Name);}break;
        case("vo-001"):{console.log(Name);}break;
        case("vo"):{console.log(Name);}break;
        case("vun-TZ"):{console.log(Name);}break;
        case("vun"):{console.log(Name);}break;
        case("wae"):{console.log(Name);}break;
        case("wal-ET"):{console.log(Name);}break;
        case("wal"):{console.log(Name);}break;
        case("wo-SN"):{console.log(Name);}break;
        case("wo"):{console.log(Name);}break;
        case("xh-ZA"):{console.log(Name);}break;
        case("xh"):{console.log(Name);}break;
        case("xog-UG"):{console.log(Name);}break;
        case("xog"):{console.log(Name);}break;
        case("yav-CM"):{console.log(Name);}break;
        case("yav"):{console.log(Name);}break;
        case("yi-001"):{console.log(Name);}break;
        case("yi"):{console.log(Name);}break;
        case("yo-BJ"):{console.log(Name);}break;
        case("yo-NG"):{console.log(Name);}break;
        case("yo"):{console.log(Name);}break;
        case("zgh-Tfng-MA"):{console.log(Name);}break;
        case("zgh-Tfng"):{console.log(Name);}break;
        case("zgh"):{console.log(Name);}break;
        case("zh-CHS"):{console.log(Name);}break;
        case("zh-CHT"):{console.log(Name);}break;
        case("zh-Hans-HK"):{console.log(Name);}break;
        case("zh-Hans-MO"):{console.log(Name);}break;
        case("zh-Hant"):{console.log(Name);}break;
        case("zh-HK"):{console.log(Name);}break;
        case("zh-MO"):{console.log(Name);}break;
        case("zh-SG"):{console.log(Name);}break;
        case("zh"):{console.log(Name);}break;
        case("zu-ZA"):{console.log(Name);}break;
        case("zu"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("ast-ES"):{console.log(Name);}break;
        case("ca-ES-valencia"):{console.log(Name);}break;
        case("ca-ES"):{console.log(Name);}break;
        case("es-ES"):{console.log(Name);}break;
        case("es-MX"):{console.log(Name);}break;
        case("eu-ES"):{console.log(Name);}break;
        case("gl-ES"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("ba-RU"):{console.log(Name);}break;
        case("ce-RU"):{console.log(Name);}break;
        case("cu-RU"):{console.log(Name);}break;
        case("de-BE"):{console.log(Name);}break;
        case("en-BE"):{console.log(Name);}break;
        case("en-CH"):{console.log(Name);}break;
        case("gsw-CH"):{console.log(Name);}break;
        case("kk-KZ"):{console.log(Name);}break;
        case("os-RU"):{console.log(Name);}break;
        case("pt-CH"):{console.log(Name);}break;
        case("rm-CH"):{console.log(Name);}break;
        case("ru-KZ"):{console.log(Name);}break;
        case("ru-RU"):{console.log(Name);}break;
        case("sah-RU"):{console.log(Name);}break;
        case("tt-RU"):{console.log(Name);}break;
        case("wae-CH"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("br-FR"):{console.log(Name);}break;
        case("ca-FR"):{console.log(Name);}break;
        case("co-FR"):{console.log(Name);}break;
        case("fr-BE"):{console.log(Name);}break;
        case("fr-CA"):{console.log(Name);}break;
        case("fr-CH"):{console.log(Name);}break;
        case("fr-FR"):{console.log(Name);}break;
        case("gsw-FR"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("de-AT"):{console.log(Name);}break;
        case("de-CH"):{console.log(Name);}break;
        case("de-DE"):{console.log(Name);}break;
        case("dsb-DE"):{console.log(Name);}break;
        case("en-AT"):{console.log(Name);}break;
        case("en-DE"):{console.log(Name);}break;
        case("hsb-DE"):{console.log(Name);}break;
        case("ksh-DE"):{console.log(Name);}break;
        case("nds-DE"):{console.log(Name);}break;
//_________________________________________________________________________________________
//_________________________________________________________________________________________
        case("da-DK"):{console.log(Name);}break;
        case("fo-DK"):{console.log(Name);}break;
        case("nb-NO"):{console.log(Name);}break;
        case("nb-SJ"):{console.log(Name);}break;
        case("nn-NO"):{console.log(Name);}break;
        case("se-NO"):{console.log(Name);}break;
        case("sma-NO"):{console.log(Name);}break;
        case("smj-NO"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("bn-IN"):{console.log(Name);}break;
        case("id-ID"):{console.log(Name);}break;
        case("jv-Java-ID"):{console.log(Name);}break;
        case("jv-Latn-ID"):{console.log(Name);}break;
        case("jv-Latn"):{console.log(Name);}break;
        case("pa-Arab-PK"):{console.log(Name);}break;
        case("pa-Arab"):{console.log(Name);}break;
        case("pa-Guru"):{console.log(Name);}break;
        case("si-LK"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("bo-CN"):{console.log(Name);}break;
        case("ii-CN"):{console.log(Name);}break;
        case("mn-Mong-CN"):{console.log(Name);}break;
        case("ug-CN"):{console.log(Name);}break;
        case("zh-CN"):{console.log(Name);}break;
        case("zh-Hans"):{console.log(Name);}break;
        case("zh-TW"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("fi-FI"):{console.log(Name);}break;
        case("se-FI"):{console.log(Name);}break;
        case("smn-FI"):{console.log(Name);}break;
        case("sms-FI"):{console.log(Name);}break;
        case("sv-AX"):{console.log(Name);}break;
        case("sv-FI"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("en-SE"):{console.log(Name);}break;
        case("se-SE"):{console.log(Name);}break;
        case("sma-SE"):{console.log(Name);}break;
        case("smj-SE"):{console.log(Name);}break;
        case("sv-SE"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("en-NL"):{console.log(Name);}break;
        case("fy-NL"):{console.log(Name);}break;
        case("nds-NL"):{console.log(Name);}break;
        case("nl-BE"):{console.log(Name);}break;
        case("nl-NL"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("cs-CZ"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("el-GR"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("en-DK"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("en-FI"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("es-BR"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("hu-HU"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("ja-JP"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("ko-KR"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("oc-FR"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("pl-PL"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("pt-BR"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("pt-PT"):{console.log(Name);}break;
//_________________________________________________________________________________________
        case("tr-TR"):{console.log(Name);}break;
//_________________________________________________________________________________________


    }
    return true;
}

//_________________________________________________________________________________________________________________
module.exports.alghoritm_of_changing_dates_and_numbes = alghoritm_of_changing_dates_and_numbes;
//____________________________________________readinarr_idxpath___________________________________________________
let inputnum = "../numbers/"
let inputdata = "../data/"
let inputDec = "../DecSep/"
let inputGr = "../GrSep/";
let input = "../numbers";
let etalondatesinput = "../data/en-US.txt";
let elanonnumbersinput = "../numbers/en-US.txt";
let outputdata = "../output/Outputdata.txt";
let outputnum = "../output/Outputnum.txt";
let outputetalon1 = "../output/Outputetalonfordata.txt";
let outputetalon2 = "../output/Outputetalonfornum.txt";
//_________________________________________input_count______________________________________________________________________
let numbersforDec = fs.readdirSync(inputDec,"utf-8",function(err,data){
    if (err) return console.log(err);
    console.log('err > helloworld.txt');
});
let numbersforGr = fs.readdirSync(inputGr,"utf-8",function(err,data){
    if (err) return console.log(err);
    console.log('err > helloworld.txt');
});
let numbers = fs.readdirSync(input,"utf-8",function(err,data){
    if (err) return console.log(err);
    console.log('err > helloworld.txt');
});
let etalondates = fs.readFileSync(etalondatesinput,"utf-8",function(err,data){
    if (err) return console.log(err);
    console.log('err > helloworld.txt');
});
let etalonnumbers = fs.readFileSync(elanonnumbersinput,"utf-8",function(err,data){
    if (err) return console.log(err);
    console.log('err > .txt');
});
//_________________________________________variables_________________________________________________________________________
let string_locale_dates= [];
let string_of_decimal_separators=[];
let string_of_group_separators=[];
let string_locale_numbers_separators= [];
let substring_locale_dates = [];
let substring_locale_numbers_separators =[];
let substrdec1 =[];
let substrdec2 = [];
let substrinputs = [];
let Clasified_array_of_the_no_eng_locales = [];
let repacked_array_of_dates_without_eng_etalon = [];
let repacked_array_of_numbers_without_eng_etalon = [];
let names_of_not_etalon_dates = [];
let names_of_etalon_dates = [];
let names_of_not_etalon_numbers = [];
let arr_idx_decimal_sep = [];
let arr_idx_groups_sep =[];
let splited_repacked_arr_of_dates = [];
let splited_repacked_arr_of_numbers = [];
let k=0;//idx
let o =0;//idx
let repackarr_for_dates = [];
let repackarr_for_numbers = [];
let etalarr_for_dates = etalondates.split(("\r\n"));
let etalarr_for_numbers = etalonnumbers.split(("\r\n"));
let names_of_repacked_dates = [];
let names_of_repacked_numbers = [];
let substrdateinputs = [];
//____________________________________________initialisating_____________________________________________________
//_______________________________________readingfromfiles______________________________________
for(let i =0;i<numbersforDec.length;i++){
    string_of_decimal_separators[i] =fs.readFileSync(inputDec + numbersforDec[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > helloworld.txt');
    });
};

for(let i =0;i<numbersforGr.length;i++){
    string_of_group_separators[i] =fs.readFileSync(inputGr + numbersforGr[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > ');
    });

};

for(let i =0;i<numbers.length;i++) {
    string_locale_numbers_separators[i] =fs.readFileSync(inputnum+numbers[i],"utf-8",function(err,data){
        if (err) return console.loarr_idx(err);
        console.log('err > .txt');
    })};

for(let i =0;i<numbers.length;i++) {
    string_locale_dates[i] =fs.readFileSync(inputdata+numbers[i],"utf-8",function(err,data){
        if (err) return console.loarr_idx(err);
        console.log('err > .txt');
    })};
//_______________________________________readingfromfilesend___________________________________
for(let i =0; i<numbers.length; i++){
    if (num_set_Locale("-",numbers[i])==true) {} else{
        substring_locale_dates.push(numbers[i]);
        substring_locale_numbers_separators.push(numbers[i-1]);
    }
};

for(let i =0; i<numbers.length; i++){
    if (num_set_Locale("-",numbers[i])==true) {} else{
        substrdec1.push(numbers[i]);
        substrdec2.push(numbers[i-1]);
    }
};
//_______________________________________writingtothefile_and_specialinput_______________________
for(let i =0; i<substring_locale_dates.length; i++){
    substrinputs[i] = fs.readFileSync(inputnum+substring_locale_numbers_separators[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > .txt');
    })
    substrdateinputs[i] = fs.readFileSync(inputdata+substring_locale_numbers_separators[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > .txt');
    })

};
//__________________________________________proof__________________________________________________________________
for(let i =0;i<etalonnumbers.length;i++){

    if(etalonnumbers[i]==="."){
        arr_idx_decimal_sep.push(i);
    }
};

for(let i =0;i<etalonnumbers.length;i++){
    if(etalonnumbers[i]===","){
        arr_idx_groups_sep.push(i);
    }
};

for(let i = 0;i<string_of_group_separators.length;i++){
    if(num_set_Locale('\u00A0', string_of_group_separators[i])==true)
        string_of_group_separators[i] =  string_of_group_separators[i].replace('\u00A0','\u0020' );}

for(let i =0;i<string_locale_numbers_separators.length;i++){
    k=0;
    while(k<arr_idx_decimal_sep.length) {
        if (string_locale_numbers_separators[i][arr_idx_decimal_sep[k]] == string_of_decimal_separators[i][0]) {
            string_locale_numbers_separators[i] = setCharAt(string_locale_numbers_separators[i],arr_idx_decimal_sep[k],'.');;
        }
        k++;
    }
};

for(let i =0;i<string_locale_numbers_separators.length;i++){
    o=0;
    while(o<arr_idx_groups_sep.length) {

        if (string_locale_numbers_separators[i][arr_idx_groups_sep[o]] == string_of_group_separators[i][1]) {
            string_locale_numbers_separators[i] = setCharAt(string_locale_numbers_separators[i],arr_idx_groups_sep[o],',');;
        }

        o++;
    }
};
//___________________________________________repacking__________________________________________________________________
for(let i =0;i<numbers.length;i++){

    if(string_locale_dates[i]===etalondates){names_of_etalon_dates.push(numbers[i]);}else{

        repacked_array_of_dates_without_eng_etalon.push(string_locale_dates[i]);
        names_of_not_etalon_dates.push(numbers[i]);
    }
    if(string_locale_numbers_separators[i]===etalonnumbers){}else{

        repacked_array_of_numbers_without_eng_etalon.push(string_locale_numbers_separators[i]);
        names_of_not_etalon_numbers.push(numbers[i]);
    }
};

for(let i = 0; i<repacked_array_of_dates_without_eng_etalon.length;i++){

    splited_repacked_arr_of_dates[i] = (repacked_array_of_dates_without_eng_etalon[i]).split("\r\n")
};

for(let i = 0; i<repacked_array_of_numbers_without_eng_etalon.length;i++){

    splited_repacked_arr_of_numbers[i] = (repacked_array_of_numbers_without_eng_etalon[i]).split("\r\n")
};

for(let i = 0; i < splited_repacked_arr_of_dates.length; i++) {
    for (let j = 0; j < etalarr_for_dates.length; j++) {

        if (splited_repacked_arr_of_dates[i][j] === etalarr_for_dates[j]) {
        } else {
            repackarr_for_dates.push(splited_repacked_arr_of_dates[i][j])
            names_of_repacked_dates.push(names_of_not_etalon_dates[i])

        }
    }
};

let ethalon_es_array_names = [];
let not_es_ethalon_array = [];
let not_es_ethalon_array_names =[];
for(let i = 0; i < repacked_array_of_dates_without_eng_etalon.length; i++) {
    let subethalon = repacked_array_of_dates_without_eng_etalon[10];

    if(repacked_array_of_dates_without_eng_etalon[i]==subethalon){

        ethalon_es_array_names.push(names_of_not_etalon_dates[i]);
      //  ethalon_es_array.push(repacked_array_of_dates_without_eng_etalon[i]);
    } else{
        not_es_ethalon_array.push(repacked_array_of_dates_without_eng_etalon[i]);
        not_es_ethalon_array_names.push(names_of_not_etalon_dates[i]);
    }
};

let ethalon_ru_array_names = [];
let not_ru_ethalon_array = [];
let not_ru_ethalon_array_names =[];
for(let i = 0; i < not_es_ethalon_array.length; i++) {
    let subethalon = not_es_ethalon_array[35];
    let subethalon1 = not_es_ethalon_array[41];
    if(not_es_ethalon_array[i]==subethalon || not_es_ethalon_array[i]==subethalon1){
        ethalon_ru_array_names.push(not_es_ethalon_array_names[i]);
    } else{
        not_ru_ethalon_array.push(not_es_ethalon_array[i]);
        not_ru_ethalon_array_names.push(not_es_ethalon_array_names[i]);
    }
};

let ethalon_fr_array_names = [];
let not_fr_ethalon_array = [];
let not_fr_ethalon_array_names =[];
for(let i = 0; i < not_ru_ethalon_array.length; i++) {
    let subethalon = not_ru_ethalon_array[24];
    let subethalon1 = not_ru_ethalon_array[74];
    let subethalon2 = not_ru_ethalon_array[76]
    if(not_ru_ethalon_array[i]==subethalon || not_ru_ethalon_array[i]==subethalon1 || not_ru_ethalon_array[i]==subethalon2){
        ethalon_fr_array_names.push(not_ru_ethalon_array_names[i]);
    } else{
        not_fr_ethalon_array.push(not_ru_ethalon_array[i]);
        not_fr_ethalon_array_names.push(not_ru_ethalon_array_names[i]);
    }
};

let ethalon_de_array_names = [];
let not_de_ethalon_array = [];
let not_de_ethalon_array_names =[];
for(let i = 0; i < not_fr_ethalon_array.length; i++) {
    let subethalon = not_fr_ethalon_array[43];

    if(not_fr_ethalon_array[i]==subethalon){
        ethalon_de_array_names.push(not_fr_ethalon_array_names[i]);
    } else{
        not_de_ethalon_array.push(not_fr_ethalon_array[i]);
        not_de_ethalon_array_names.push(not_fr_ethalon_array_names[i]);
    }
};

let ethalon_it_array_names = [];
let not_it_ethalon_array = [];
let not_it_ethalon_array_names =[];
for(let i = 0; i < not_de_ethalon_array.length; i++) {
    let subethalon = not_de_ethalon_array[28];
    let subethalon1 = not_de_ethalon_array[36];
    if(not_de_ethalon_array[i]==subethalon || not_de_ethalon_array[i]==subethalon1){
        ethalon_it_array_names.push(not_de_ethalon_array_names[i]);
    } else{
        not_it_ethalon_array.push(not_de_ethalon_array[i]);
        not_it_ethalon_array_names.push(not_de_ethalon_array_names[i]);
    }
};

let ethalon_da_array_names = [];
let not_da_ethalon_array = [];
let not_da_ethalon_array_names =[];
for(let i = 0; i < not_it_ethalon_array.length; i++) {
    let subethalon = not_it_ethalon_array[30];
    let subethalon1 = not_it_ethalon_array[44];
    if(not_it_ethalon_array[i]==subethalon || not_it_ethalon_array[i]==subethalon1){
        ethalon_da_array_names.push(not_it_ethalon_array_names[i]);
    } else{
        not_da_ethalon_array.push(not_it_ethalon_array[i]);
        not_da_ethalon_array_names.push(not_it_ethalon_array_names[i]);
    }
};

let ethalon_pointen_array_names = [];
let not_pointen_ethalon_array = [];
let not_pointen_ethalon_array_names =[];
for(let i = 0; i < not_da_ethalon_array.length; i++) {
    let subethalon = not_da_ethalon_array[55];
    let subethalon1 = not_da_ethalon_array[0];
    if(not_da_ethalon_array[i]==subethalon|| not_da_ethalon_array[i]==subethalon1){
        ethalon_pointen_array_names.push(not_da_ethalon_array_names[i]);
    } else{
        not_pointen_ethalon_array.push(not_da_ethalon_array[i]);
        not_pointen_ethalon_array_names.push(not_da_ethalon_array_names[i]);
    }
};

let ethalon_ch_array_names = [];
let not_ch_ethalon_array = [];
let not_ch_ethalon_array_names =[];
for(let i = 0; i < not_pointen_ethalon_array.length; i++) {
    let subethalon = not_pointen_ethalon_array[2];
    let subethalon1 = not_pointen_ethalon_array[72];
    if(not_pointen_ethalon_array[i]==subethalon||not_pointen_ethalon_array[i]==subethalon1){
        ethalon_ch_array_names.push(not_pointen_ethalon_array_names[i]);
    } else{
        not_ch_ethalon_array.push(not_pointen_ethalon_array[i]);
        not_ch_ethalon_array_names.push(not_pointen_ethalon_array_names[i]);
    }
};

let ethalon_fi_array_names = [];
let not_fi_ethalon_array = [];
let not_fi_ethalon_array_names =[];
for(let i = 0; i < not_ch_ethalon_array.length; i++) {
    let subethalon = not_ch_ethalon_array[14];

    if(not_ch_ethalon_array[i]==subethalon){
        ethalon_fi_array_names.push(not_ch_ethalon_array_names[i]);
    } else{
        not_fi_ethalon_array.push(not_ch_ethalon_array[i]);
        not_fi_ethalon_array_names.push(not_ch_ethalon_array_names[i]);
    }
};

let ethalon_DA_array_names = [];
let not_DA_ethalon_array = [];
let not_DA_ethalon_array_names =[];
for(let i = 0; i < not_fi_ethalon_array.length; i++) {
    let subethalon = not_fi_ethalon_array[11];

    if(not_fi_ethalon_array[i]==subethalon){
        ethalon_DA_array_names.push(not_fi_ethalon_array_names[i]);
    } else{
        not_DA_ethalon_array.push(not_fi_ethalon_array[i]);
        not_DA_ethalon_array_names.push(not_fi_ethalon_array_names[i]);
    }
};

let ethalon_fy_array_names = [];
let not_fy_ethalon_array = [];
let not_fy_ethalon_array_names =[];
for(let i = 0; i < not_DA_ethalon_array.length; i++) {
    let subethalon = not_DA_ethalon_array[15];
    let subethalon1 = not_DA_ethalon_array[11];
    let subethalon2 = not_DA_ethalon_array[33];
    if(not_DA_ethalon_array[i]==subethalon || not_DA_ethalon_array[i]==subethalon1 || not_DA_ethalon_array[i]==subethalon2){
        ethalon_fy_array_names.push(not_DA_ethalon_array_names[i]);
    } else{
        not_fy_ethalon_array.push(not_DA_ethalon_array[i]);
        not_fy_ethalon_array_names.push(not_DA_ethalon_array_names[i]);
    }
};
for(let i = 0; i < splited_repacked_arr_of_numbers.length; i++) {
    for (let j = 0; j < etalarr_for_numbers.length; j++) {

        if (splited_repacked_arr_of_numbers[i][j] === etalarr_for_numbers[j]) {
        } else {


            repackarr_for_numbers.push(splited_repacked_arr_of_numbers[i][j])
            names_of_repacked_numbers.push(names_of_not_etalon_numbers[i])

        }
    }
};
//_______________________________________________________proofcheck________________________________________________________
for(let i =0 ; i< string_locale_numbers_separators.length;i++) if (string_locale_numbers_separators[i]==etalonnumbers) { }else{ console.log("F "+ i);};
//__________________________________________________output_________________________________________________________________
for(let i=0;i<not_fy_ethalon_array.length;i++){

    Clasified_array_of_the_no_eng_locales[i] = not_fy_ethalon_array[i]+" "+ not_fy_ethalon_array_names[i]+"\r\n";
}

fs.writeFileSync(outputdata, Clasified_array_of_the_no_eng_locales, "utf-8");

fs.writeFileSync(outputnum, repackarr_for_numbers+names_of_repacked_numbers,"utf-8");

fs.writeFileSync(outputetalon1, etalarr_for_dates, "utf-8");

fs.writeFileSync(outputetalon2, etalarr_for_numbers, "utf-8");

//____________________________________________________________________________________
let splited_names_of_etalon_dates =[];
let builder_of_splited_names_of_etalon_dates = [];
let builder_of_splited_es_names_of_etalon_dates =[];
let builder_of_splited_ru_names_of_etalon_dates =[];
let builder_of_splited_fr_names_of_etalon_dates =[];
let builder_of_splited_de_names_of_etalon_dates =[];
let builder_of_splited_it_names_of_etalon_dates =[];
let builder_of_splited_da_names_of_etalon_dates = [];
let builder_of_splited_pointen_names_of_etalon_dates = [];
let builder_of_splited_ch_names_of_etalon_dates = [];
let builder_of_splited_fi_names_of_etalon_dates = [];
let builder_of_splited_DA_names_of_etalon_dates = [];
let builder_of_splited_fy_names_of_etalon_dates = [];
let special_builder = [];
for(let i =0;i<names_of_etalon_dates.length;i++) {
     //   builder_of_splited_names_of_etalon_dates[i] = "case("+'"'+(names_of_etalon_dates[i]).replace(".txt","")+'"'+"):{ LocaleFormatSymbol['Y'] = 'Y'; LocaleFormatSymbol['y'] ='y';LocaleFormatSymbol['M'] = 'M';LocaleFormatSymbol['m'] ='m';LocaleFormatSymbol['D'] = 'D';LocaleFormatSymbol['d'] ='d'; LocaleFormatSymbol['H'] = 'H';LocaleFormatSymbol['h'] ='h';LocaleFormatSymbol['S'] = 'S';LocaleFormatSymbol['s'] ='s';}break;"
    builder_of_splited_names_of_etalon_dates[i] = "case("+'"'+(names_of_etalon_dates[i]).replace(".txt","")+'"'+"):{console.log(Name);}break;"
        splited_names_of_etalon_dates[i] = (names_of_etalon_dates[i]).replace(".txt","");

        console.log(builder_of_splited_names_of_etalon_dates[i]);
 //   ParseLocalFormatSymbol(splited_names_of_etalon_dates[i])
}
console.log("//____________________________________________________ES_____________________________________");
for(let i =0;i<ethalon_es_array_names.length;i++){
    builder_of_splited_es_names_of_etalon_dates[i]="case("+'"'+(ethalon_es_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'A';LocaleFormatSymbol['y'] ='a';}break;"
    console.log(builder_of_splited_es_names_of_etalon_dates[i]);
}
console.log("//______________________________________________________ru___________________________________");
for(let i =0;i<ethalon_ru_array_names.length;i++){
    builder_of_splited_ru_names_of_etalon_dates[i]="case("+'"'+(ethalon_ru_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'Г';LocaleFormatSymbol['y'] = 'г';LocaleFormatSymbol['M'] = 'М';LocaleFormatSymbol['m'] = 'м';LocaleFormatSymbol['D'] = 'Д';LocaleFormatSymbol['d'] = 'д';LocaleFormatSymbol['H'] = 'Ч';LocaleFormatSymbol['h'] = 'ч';LocaleFormatSymbol['S'] = 'C';LocaleFormatSymbol['s'] = 'с';}break;"
    console.log(builder_of_splited_ru_names_of_etalon_dates[i]);
}
console.log("//________________________________________________________fr_________________________________");
for(let i =0;i<ethalon_fr_array_names.length;i++){
    builder_of_splited_fr_names_of_etalon_dates[i]="case("+'"'+(ethalon_fr_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'J';LocaleFormatSymbol['y'] ='j';}break;LocaleFormatSymbol['D'] = 'A';LocaleFormatSymbol['d'] ='a';"
    console.log(builder_of_splited_fr_names_of_etalon_dates[i]);
}
console.log("//__________________________________________________________de_______________________________");
for(let i =0;i<ethalon_de_array_names.length;i++){
    builder_of_splited_de_names_of_etalon_dates[i]="case("+'"'+(ethalon_de_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'J';LocaleFormatSymbol['y'] = 'j';LocaleFormatSymbol['D'] = 'T';LocaleFormatSymbol['d'] = 't';}break;"
    console.log(builder_of_splited_de_names_of_etalon_dates[i]);
}
console.log("//____________________________________________________________it_____________________________");
for(let i =0;i<ethalon_it_array_names.length;i++){
    builder_of_splited_it_names_of_etalon_dates[i]="case("+'"'+(ethalon_it_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'G';LocaleFormatSymbol['y'] ='g';LocaleFormatSymbol['D'] = 'A';LocaleFormatSymbol['d'] ='a';}break;"
    console.log(builder_of_splited_it_names_of_etalon_dates[i]);
}
console.log("//______________________________________________________________da___________________________");
for(let i =0;i<ethalon_da_array_names.length;i++){
    builder_of_splited_da_names_of_etalon_dates[i]="case("+'"'+(ethalon_da_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'Å';LocaleFormatSymbol['y'] ='å';LocaleFormatSymbol['H'] = 'T';LocaleFormatSymbol['h'] ='t';}break;"
    console.log(builder_of_splited_da_names_of_etalon_dates[i]);
}
console.log("//________________________________________________________________pointen_________________________");
for(let i =0;i<ethalon_pointen_array_names.length;i++){
    builder_of_splited_pointen_names_of_etalon_dates[i]="case("+'"'+(ethalon_pointen_array_names[i]).replace(".txt","")+'"'+"):{console.log(Name);}break;"
    console.log(builder_of_splited_pointen_names_of_etalon_dates[i]);
}
console.log("//_______________________________________________________________________ch__________________");
for(let i =0;i<ethalon_ch_array_names.length;i++){
    builder_of_splited_ch_names_of_etalon_dates[i]="case("+'"'+(ethalon_ch_array_names[i]).replace(".txt","")+'"'+"):{console.log(Name);}break;"
    console.log(builder_of_splited_ch_names_of_etalon_dates[i]);
}
console.log("//________________________________________________________fi_________________________________");
for(let i =0;i<ethalon_fi_array_names.length;i++){
    builder_of_splited_fi_names_of_etalon_dates[i]="case("+'"'+(ethalon_fi_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'V';LocaleFormatSymbol['y'] ='v';LocaleFormatSymbol['d'] ='p';LocaleFormatSymbol['H'] = 'T';LocaleFormatSymbol['h'] ='t';}break;"
    console.log(builder_of_splited_fi_names_of_etalon_dates[i]);
}
console.log("//__________________________________________________________DA_______________________________");
for(let i =0;i<ethalon_DA_array_names.length;i++){
    builder_of_splited_DA_names_of_etalon_dates[i]="case("+'"'+(ethalon_DA_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'Å';LocaleFormatSymbol['y'] ='å';LocaleFormatSymbol['H'] = 'T';LocaleFormatSymbol['h'] ='t';}break;"
    console.log(builder_of_splited_DA_names_of_etalon_dates[i]);
}
console.log("//____________________________________________________________fy_____________________________");
for(let i =0;i<ethalon_fy_array_names.length;i++){
    builder_of_splited_fy_names_of_etalon_dates[i]="case("+'"'+(ethalon_fy_array_names[i]).replace(".txt","")+'"'+"):{LocaleFormatSymbol['Y'] = 'J';LocaleFormatSymbol['y'] ='j';LocaleFormatSymbol['H'] = 'U'; LocaleFormatSymbol['h'] ='u';}break;"
    console.log(builder_of_splited_fy_names_of_etalon_dates[i]);
}
console.log("//______________________________________________________________special___________________________");
for(let i =0;i<not_fy_ethalon_array_names.length;i++){
    special_builder[i]="case("+'"'+(not_fy_ethalon_array_names[i]).replace(".txt","")+'"'+"):{console.log(Name);}break;"
    console.log(special_builder[i]);
    console.log("//_________________________________________________________________________________________");
}
console.log("end");
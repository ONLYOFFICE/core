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
        fs.writeFileSync(inputdata+substring_locale_dates[i], etalon1, function (err) {
            if (err) return console.log(err);
        });
        fs.writeFileSync(inputnum+substring_locale_dates[i], substrinputs, function (err) {
            if (err) return console.log(err);
        });
    };
}

module.exports.alghoritm_of_changing_dates_and_numbes = alghoritm_of_changing_dates_and_numbes;
//____________________________________________readinarr_idxpath___________________________________________________
let inputnum = "../numbers/"
let inputdata = "../data/"
let inputDec = "../DecSep/"
let inputGr = "../GrSep/";
let input = "../numbers";
let etalon1input = "../data/en-US.txt";
let elanon2input = "../numbers/en-US.txt";
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
let etalon1 = fs.readFileSync(etalon1input,"utf-8",function(err,data){
    if (err) return console.log(err);
    console.log('err > helloworld.txt');
});
let etalon2 = fs.readFileSync(elanon2input,"utf-8",function(err,data){
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
let names_of_not_etalon_numbers = [];
let arr_idx_decimal_sep = [];
let arr_idx_groups_sep =[];
let splited_repacked_arr_of_dates = [];
let splited_repacked_arr_of_numbers = [];
let k=0;//idx
let o =0;//idx
let repackarr_for_dates = [];
let repackarr_for_numbers = [];
let etalarr_for_dates = etalon1.split(("\r\n"));
let etalarr_for_numbers = etalon2.split(("\r\n"));
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
for(let i =0;i<etalon2.length;i++){

    if(etalon2[i]==="."){
        arr_idx_decimal_sep.push(i);
    }
};

for(let i =0;i<etalon2.length;i++){
    if(etalon2[i]===","){
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

    if(string_locale_dates[i]===etalon1){}else{

        repacked_array_of_dates_without_eng_etalon.push(string_locale_dates[i]);
        names_of_not_etalon_dates.push(numbers[i]);
    }
    if(string_locale_numbers_separators[i]===etalon2){}else{

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
for(let i =0 ; i< string_locale_numbers_separators.length;i++) if (string_locale_numbers_separators[i]==etalon2) { }else{ console.log("F "+ i);};
//__________________________________________________output_________________________________________________________________
for(let i=0;i<repackarr_for_dates.length;i++){

    Clasified_array_of_the_no_eng_locales[i] = repackarr_for_dates[i]+" "+ names_of_repacked_dates[i]+"\r\n";
}

fs.writeFileSync(outputdata, Clasified_array_of_the_no_eng_locales, "utf-8");

fs.writeFileSync(outputnum, repackarr_for_numbers+names_of_repacked_numbers,"utf-8");

fs.writeFileSync(outputetalon1, etalarr_for_dates, "utf-8");

fs.writeFileSync(outputetalon2, etalarr_for_numbers, "utf-8");

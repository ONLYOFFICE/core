fs = require('fs');
//__________________________________________functions_____________________________________________________________
function setCharAt(str,index,chr) {
    if(index > str.length-1) return str;
    return str.substring(0,index) + chr + str.substring(index+1);
}

function test(re, str) {
    var midstring;
    if (str.search(re) != -1) {
        return true
    } else {
        return false
    }
    console.log(str + midstring + re);
}
//____________________________________________readingpath___________________________________________________
let inputnum = "../numbers/"
let inputdata = "../data/"
let inputDec = "../DecSep/"
let inputGr = "../GrSep/";
let input = "../numbers";
let etalon1input = "../data/en-US.txt";
let elanon2input = "../numbers/en-US.txt";
let outputdata = "../Outputdata.txt";
let outputnum = "../Outputnum.txt";
let outputetalon1 = "../Outputetalonfordata.txt";
let outputetalon2 = "../Outputetalonfornum.txt";
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
let str1= [];
let strofdec=[];
let strofgr=[];
let str2= [];
let substr1 = [];
let substr2 =[];
let substrdec1 =[];
let substrdec2 = [];
let substrinputs = [];
let repack1 = [];
let repack2 = [];
let memlcids1 = [];
let memlcids2 = [];
let g = [];
let gg =[];
let arru1 = [];
let arru2 = [];
let k=0;
let o =0;
let repackarr1 = [];
let repackarr2 = [];
let etalarr1 = etalon1.split(("\r\n"));
let etalarr2 = etalon2.split(("\r\n"));
let memarr1 = [];
let memarr2 = [];
//____________________________________________initialisating_____________________________________________________
//_______________________________________readingfromfiles______________________________________
for(let i =0;i<numbersforDec.length;i++){
    strofdec[i] =fs.readFileSync(inputDec + numbersforDec[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > helloworld.txt');
    });
};

for(let i =0;i<numbersforGr.length;i++){
    strofgr[i] =fs.readFileSync(inputGr + numbersforGr[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > ');
    });

};

for(let i =0;i<numbers.length;i++) {
    str2[i] =fs.readFileSync(inputnum+numbers[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > .txt');
    })};

for(let i =0;i<numbers.length;i++) {
    str1[i] =fs.readFileSync(inputdata+numbers[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > .txt');
    })};
//_______________________________________readingfromfilesend___________________________________
for(let i =0; i<numbers.length; i++){
       if (test("-",numbers[i])==true) {} else{
           substr1.push(numbers[i]);
           substr2.push(numbers[i-1]);
       }
};

for(let i =0; i<numbers.length; i++){
    if (test("-",numbers[i])==true) {} else{
        substrdec1.push(numbers[i]);
        substrdec2.push(numbers[i-1]);
    }
};
let substrdateinputs = [];
//_______________________________________writingtothefile_and_specialinput_______________________
for(let i =0; i<substr1.length; i++){
    substrinputs[i] = fs.readFileSync(inputnum+substr2[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > .txt');
    })
    substrdateinputs[i] = fs.readFileSync(inputdata+substr2[i],"utf-8",function(err,data){
        if (err) return console.log(err);
        console.log('err > .txt');
    })

};



//__________________________________________proof__________________________________________________________________
for(let i =0;i<etalon2.length;i++){

    if(etalon2[i]==="."){
        g.push(i);
    }
};

for(let i =0;i<etalon2.length;i++){
    if(etalon2[i]===","){
        gg.push(i);
    }
};

for(let i = 0;i<strofgr.length;i++){
    if(test('\u00A0', strofgr[i])==true)
        strofgr[i] =  strofgr[i].replace('\u00A0','\u0020' );}
//for(let i = 0;i<strofgr.length;i++) if(test('\u00A0', strofgr[i])==true) setCharAt(strofgr[i],1,'\u0020')
//for(let i =0;i<strofgr.length;i++){ strofgrrepack[i]=strofgr[i];}
//for(let i = 0;i<strofgrrepack.length;i++) if(test('\u00A0', strofgrrepack[i])==true) strofgrrepack[i].replace('\u00A0','\u0020' );
for(let i =0;i<str2.length;i++){
k=0;
    while(k<g.length) {
        if (str2[i][g[k]] == strofdec[i][0]) {
        str2[i] = setCharAt(str2[i],g[k],'.');;
        }
    k++;
    }
};

for(let i =0;i<str2.length;i++){
o=0;
while(o<gg.length) {

    if (str2[i][gg[o]] == strofgr[i][1]) {
        str2[i] = setCharAt(str2[i],gg[o],',');;
    } else {

        console.log("not ok")}

    o++;
    }
};
//___________________________________________repacking__________________________________________________________________
for(let i =0;i<numbers.length;i++){

    if(str1[i]===etalon1){}else{

        repack1.push(str1[i]);
        memlcids1.push(numbers[i]);
    }
    if(str2[i]===etalon2){}else{

        repack2.push(str2[i]);
        memlcids2.push(numbers[i]);
    }
};

for(let i =0;i<str2[0].length;i++){
    console.log(JSON.stringify(str2[0][i]));
};

for(let i = 0; i<repack1.length;i++){

    arru1[i] = (repack1[i]).split("\r\n")
};

for(let i = 0; i<repack2.length;i++){

    arru2[i] = (repack2[i]).split("\r\n")
};

for(let i = 0; i < arru1.length; i++) {
    for (let j = 0; j < etalarr1.length; j++) {

        if (arru1[i][j] === etalarr1[j]) {
        } else {
            repackarr1.push(arru1[i][j])
            memarr1.push(memlcids1[i])

        }
    }
};

for(let i = 0; i < arru2.length; i++) {
    for (let j = 0; j < etalarr2.length; j++) {

        if (arru2[i][j] === etalarr2[j]) {
        } else {


            repackarr2.push(arru2[i][j])
            memarr2.push(memlcids2[i])

        }
    }
};
//_______________________________________________________proofcheck________________________________________________________
for(let i =0 ; i< str2.length;i++) if (str2[i]==etalon2) { }else{ console.log("F "+ i);};
//__________________________________________________output_________________________________________________________________

let outputarr1 = [];
for(let i=0;i<repackarr1.length;i++){

    outputarr1[i] = repackarr1[i]+" "+ memarr1[i]+"\r\n";
}
fs.writeFileSync(outputdata, outputarr1, "utf-8");

fs.writeFileSync(outputnum, repackarr2+memarr2,"utf-8");

fs.writeFileSync(outputetalon1, etalarr1, "utf-8");

fs.writeFileSync(outputetalon2, etalarr2, "utf-8");
let evalstr = [''];
let bal = 0;
let mode = 0;

let modes = 
    {
        m1 : ['\u03C0', 'e', '|x|', '\u221A', 'x\u02b8'],
        m2 : ['sin', 'cos', 'tan', 'ln', 'fac'],
        m3 : ['arcsin', 'arccos', 'arctan', '\u03C0', 'e'],
    };

function num(a){
    if(evalstr[evalstr.length - 1] == '' && evalstr[evalstr.length - 2] == ')'){
        evalstr.pop();
        evalstr.push('*');
        evalstr.push('');
    }   
    evalstr[evalstr.length - 1] += a;
    console.log(evalstr);
}

function isoperator(a){
    switch(a){
        case '+':return 1;
            break;
        case '-':return 1;
            break;
        case '*':return 1;
            break;
        case '/':return 1;
            break;
        case '^': return 1;
            break;
    }
    return 0;
}



function addpoint(){
    if(!evalstr[evalstr.length-1].includes('.')){
        evalstr[evalstr.length-1] += '.';
    }
}

function addoperator(a){
    if(evalstr.length == 1 && evalstr[0] == '' && a!=3)
        return;
    if(evalstr[evalstr.length-1] == '')
        evalstr.pop();
    if(evalstr[evalstr.length-1] == '(' && a!=3){
        evalstr.push('');
        return;
    }
    if(isoperator(evalstr[evalstr.length-1])){
        if(evalstr[evalstr.length-2] == '(' || evalstr.length == 1){
            evalstr.push('');
            return;
        }
        evalstr.pop();
    }
    switch(a){
        case 1: evalstr.push('/');
                evalstr.push('');
        break;
        case 2: evalstr.push('*');
                evalstr.push('');
        break;
        case 3: evalstr.push('-');
                evalstr.push('');
        break;
        case 4: evalstr.push('+');
                evalstr.push('');
        break;
        case 5: evalstr.push('^');
                evalstr.push('');
        break;
    }
}

function addlbrak(){
    if(evalstr.length == 1 && evalstr[0] == ''){
        evalstr.pop();
        evalstr.push('(');
        evalstr.push('');
        bal++;
        return;
    }
    if(evalstr[evalstr.length-1] == '')
        evalstr.pop();
    if(isoperator(evalstr[evalstr.length-1]) || evalstr[evalstr.length-1] == '('){
        evalstr.push('(');
        evalstr.push('');
    }
    else{
        evalstr.push('*');
        evalstr.push('(');
        evalstr.push('');
    }
    bal ++;
}

function addrbrak(){
    if(bal <= 0) return;
    if(evalstr[evalstr.length - 1] == '')
        evalstr.pop();
    if(evalstr[evalstr.length - 1] == '(' || isoperator(evalstr[evalstr.length - 1])){
        evalstr.push('');
        return;
    }
    evalstr.push(')');
    evalstr.push('');
    bal--;
}

function addconstant(c){
    if(!isNaN(evalstr[evalstr.length-1])){
        addoperator(2);
    }
    switch(c){
        case 'pi': num(3.141592);
        break;
        case 'e': num(2.71828);
        break;
    }
}

function addfunc(f){
    if(evalstr.length == 1 && evalstr[0] == ''){
        evalstr.pop();
        evalstr.push(f);
        evalstr.push('(');
        evalstr.push('');
        bal++;
        return;
    }
    if(evalstr[evalstr.length-1] == '')
        evalstr.pop();
    if(isoperator(evalstr[evalstr.length-1]) || evalstr[evalstr.length-1] == '('){
        evalstr.push(f);
        evalstr.push('(');
        evalstr.push('');
    }
    else{
        evalstr.push('*');
        evalstr.push(f);
        evalstr.push('(');
        evalstr.push('');
    }
    bal ++;
}

function isfunc(f){
     switch(f){
        case 'mod':return 1;
            break;
        case 'root':return 1;
            break;
        case 'sin':return 1;
            break;
        case 'cos':return 1;
            break;
        case 'tan': return 1;
            break;
        case 'arcsin': return 1;
             break;
        case 'arccos': return 1;
             break;
        case 'arctan': return 1;
             break;
        case 'ln': return 1;
             break;
        case 'arccos': return 1;
             break;
        case 'arctan': return 1;
             break;
        case 'fac': return 1;
             break;
    }
    return 0; 
}

function setmode(md){
    let setstrings = [];
    switch(md){
        case 0: setstrings = modes.m1;
        break;
        case 1: setstrings = modes.m2;
        break;
        case 2: setstrings = modes.m3;
        break;
    }
    for(let i = 0; i<5; i++)
        $('#f' + (i+1)).text(setstrings[i]);
}

$("#clear").click(function(){
    if(!isNaN(evalstr[evalstr.length-1]) && evalstr[evalstr.length - 1] > 1){
        evalstr[evalstr.length-1] = evalstr[evalstr.length-1].substring(0, evalstr[evalstr.length - 1].length - 1);
        return;
    }
    if(evalstr[evalstr.length-1] == '')
        evalstr.pop();
    evalstr.pop();
    if(isfunc(evalstr[evalstr.length-1]))
        evalstr.pop();
    if(isoperator(evalstr[evalstr.length-1]) || evalstr[evalstr.length-1] == ')' || evalstr[evalstr.length-1] == '('){
        evalstr.push('');
    }
    if(evalstr.length == 0){
        evalstr = [''];
    }
});

$('#clear').dblclick(function(){
    evalstr = [''];
});

for(let i=0;i<=9;i++){
    $('#'+i).click(()=>{num(i);});
}

$('#point').click(()=>{addpoint();});

$(document).click(()=>{
    let res = '';
    for(let i = 0; i<evalstr.length; i++){
        res += evalstr[i];
        res += ' ';
    }
    console.log(res);
    $('#curr').text(res);
    if(res.trim().length == 0){
        $('#curr').text("Calculator Ready!");
    }
});

for(let i = 1; i<=4; i++){
    $('#op'+i).click(()=>{addoperator(i);});
}

$('#lbrak').click(()=>{addlbrak();});
$('#rbrak').click(()=>{addrbrak();});

$('#switch').click(()=>{
    mode ++;
    if(mode > 2)
        mode = 0;
    setmode(mode);
})

$('#f1').click(()=>{
    switch(mode){
        case 0: addconstant('pi');
        break;
        case 1: addfunc('sin');
        break;
        case 2: addfunc('arcsin');
        break;
    }
});

$('#f2').click(()=>{
    switch(mode){
        case 0: addconstant('e');
        break;
        case 1: addfunc('cos');
        break;
        case 2: addfunc('arccos');
        break;
    }
});

$('#f3').click(()=>{
    switch(mode){
        case 0: addfunc('mod');
        break;
        case 1: addfunc('tan');
        break;
        case 2: addfunc('arctan');
    }
});

$('#f4').click(()=>{
    switch(mode){
        case 0: addfunc('root');
        break;
        case 1: addfunc('ln');
        break;
        case 2: addconstant('pi');
        break;
    }
});

$('#f5').click(()=>{
    switch(mode){
        case 0: addoperator(5);
        break;
        case 1: addfunc('fac');
        break;
        case 2: addconstant('e');
        break;
    }
});

$('#eq').click(()=>{
    let itr = 10000;
    let itr2 = 10000;
    if(bal > 0){
        while(bal > 0 && itr > 0){
            if(evalstr[evalstr.length-1] == ''){
                    evalstr.pop();
            }
            while((isNaN(evalstr[evalstr.length-1]) && evalstr[evalstr.length - 1] != ')') && itr2 > 0){
                if(isoperator(evalstr[evalstr.length-1])){
                    evalstr.pop();
                }
                if(evalstr[evalstr.length - 1] == '('){
                    evalstr.pop();
                    bal --;
                }
                if(isfunc(evalstr[evalstr.length - 1])){
                    evalstr.pop();
                }
                itr2 --;
            }
            if(bal > 0){
                addrbrak();
                bal ++;
                bal --;
            }
            itr --;
        }
    }
    let res = '';
    for(let i = 0; i<evalstr.length; i++){
        res += evalstr[i];
        res += ' ';
    }
    console.log(res);
    $('#res').text(res);
    if(res.trim().length == 0){
        $('#res').text("0");
    }
    $('#res').text(res);
});

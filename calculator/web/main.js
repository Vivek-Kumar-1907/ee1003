let evalstr = [''];
let bal = 0;

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
    if(evalstr[evalstr.length-1] == '(' && a!=3)
        return;
    if(isoperator(evalstr[evalstr.length-1])){
        if(evalstr[evalstr.length-2] == '(' || evalstr.length == 1)
            return;
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
    if(isoperator(evalstr[evalstr.length-1])){
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

$("#clear").click(function(){
    if(evalstr[evalstr.length-1] == '')
        evalstr.pop();
    evalstr.pop();
    if(isoperator(evalstr[evalstr.length-1]) || evalstr[evalstr.length-1] == ')' || evalstr[evalstr.length-1] == '('){
        evalstr.push('');
    }
    if(evalstr.length == 0){
        evalstr = [''];
    }
    console.log(evalstr);
});

$('#clear').dblclick(function(){
    evalstr = [''];
    console.log(evalstr);
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

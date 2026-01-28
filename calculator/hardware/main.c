#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LiquidCrystal.h"

#define PI 3.141592

struct Stack{
    int top;
    float arr[1000];
};

struct Operators{
    int top;
    char str[80][8];
};

void push_op(struct Operators * st, int n, char stra[8]){
	st->top ++;
	for(int i = 0; i<n; i++)
		st->str[st->top][i] = stra[i];
	st->str[st->top][n] = '\0';
}

int pop_op(struct Operators * st, char str[8]){
	int len = 0;
	while(st->str[st->top][len++] != '\0');
	for(int i = 0; i<len; i++)
		str[i] = st->str[st->top][i];
	str[len] = '\0';
	st->top --;
	return len;
}

void push(struct Stack * st, float x){
    st->arr[++st->top] = x;
}

float pop(struct Stack * st){
    return st->arr[st->top--];
}

float mod_x(float x){
    return x > 0 ? x : -x; 
}

float inv_sq_x(float x){
    long i = *( long *) &x;
    i  = 0x5f3759df - ( i >> 1 );
    float y  = *(float *)&i;
    y = y*(1.5f - (0.5f*x*y*y));
    y = y*(1.5f - (0.5f*x*y*y));
    y = y*(1.5f - (0.5f*x*y*y));
    y = y*(1.5f - (0.5f*x*y*y));
    return mod_x(y);
}

float func(float x, float y, float w, int a){
    switch(a){
        case 1: return 1/x;
        break;
        case 2: return 1/(1+x*x);
        break;
        case 3: return inv_sq_x(1 - (x*x));
        break;
        case 4: return w*y/x;
        break;
    }
    return 0;
}

float single_nxt_itr(int a, float w, float t_n, float y_n, float h, int n){
    if(n == 0)
        return y_n;
    float k1 = h*func(t_n, y_n, w, a);
    float k2 = h*func(t_n+(h/2), y_n+(k1/2), w, a);
    float k3 = h*func(t_n+(h/2), y_n+(k2/2), w, a);
    float k4 = h*func(t_n+h, y_n + k3, w, a);
    float y_nxt = y_n + (k1 + k2*2 + k3*2 + k4)/6;
    return single_nxt_itr(a, w, t_n + h, y_nxt, h, n-1);
}

float sin_next_itr(float y_n, float z_n, float h, int n){
    if(n == 0)
        return y_n;
    float k1 = h*z_n;
    float l1 = -h*(y_n);
    float k2 = h*(z_n + (l1/2));
    float l2 = -h*(y_n+ (k1/2));
    float k3 = h*(z_n + (l2/2));
    float l3 = -h*(y_n + (k2/2));
    float k4 = h*(z_n + l3);
    float l4 = -h*(y_n + k3);
    float y_nxt = y_n + (k1 + k2*2 + k3*2 + k4)/6;
    float z_nxt = z_n + (l1 + l2*2 + l3*2 + l4)/6;
    return sin_next_itr(y_nxt, z_nxt, h, n-1);
}

float sin_x(float x){
    if(x < 0)
        return -sin_x(-x);
    while(x > 2*PI)
            x-= 2*PI;
    return sin_next_itr(0, 1, 0.01, 100*x);
}

float cos_x(float x){
    return sin_x(PI/2 - x);
}

float tan_x(float x){
    return sin_x(x)/cos_x(x);
}

float ln_x(float x){
    if( x <= 0)
        return -1.0f/0.0;
    if(x < 1)
        return -single_nxt_itr(1, 0, 1, 0, 0.01, 100*((1/x)-1));
    if(x == 1)
        return 0;
    if(x > 1 && x<2)
        return single_nxt_itr(1, 0, 1, 0, 0.0001, 10000*(x-1));
    return single_nxt_itr(1, 0, 1, 0, 0.01, 100*(x-1));
}


float arcsin_x(float x){
    if(x > 1)
        return (PI/2);
    if(x < -1)
        return -(PI/2);
    if(x < 0)
        return -arcsin_x(-x);
    return single_nxt_itr(3, 0, 0, 0, 0.00001, 100000*x);
}

float arccos_x(float x){
    return (PI/2)- arcsin_x(x);
}

float arctan_x(float x){
    if(x < 0)
        return -arctan_x(-x);
    return single_nxt_itr(2, 0, 0, 0, 0.01, 100*x);
}

float power_x(float x, float w){
    if(x < 0 ){
        int floorw = (int)w;
        int isintw = mod_x(w - (float)floorw) < 0.00001;
        if(isintw){
            if(floorw % 2 == 0)
                return power_x(-x, w);
            else
                return -power_x(-x,w);
        }
        else
            return 1.0/0.0;
    }
    if(x == 0){
        if(w > 0)
            return 0;
        else
            return 1.0/0.0;
    }
    if(x < 1)
        return 1/power_x(1/x, w);
    return single_nxt_itr(4, w, 1, 1, 0.01, 100*(x-1));
}

float power_10(int x){
    if(x > 0){
        float product = 1;
        while(x--)
            product *= 10;
        return product;
    }
    if(x == 0)
        return 1;
    if(x < 0)
        return 1/power_10(-x);
    return 1;
}

float factorial(int x){
    return (x<2)?1:x*factorial(x-1);
}

int isnum(char str[15]){
    int num = str[0] - '0';
    if(num >= 0 && num < 10){
        return 1;
    }
    return 0;
}

int checkdigit(char c){
	return ((c - '0' >= 0 && c - '0' <= 9) || c == '.');
}

int is_op(char c){
	if(c == '+' || c == '-')
		return 2;
	if(c == '*' || c == '/')
		return 3;
	else if( c == '^')
		return 4;
	return 0;
}

int shuntingyard(char expr[1000], char str[100][15]){
     struct Operators operators;
     operators.top = -1;
     int len = 0;
     int idx = 0;
     int unmatched_braks = 0;
     for(int i = 0; i<100; i++)
	     if(expr[i] != '\0'){ len ++;}
     for(int i = 0; i<len; i++){
	char c = expr[i];
	if(checkdigit(c)){
       	int j = i;
		while(checkdigit(expr[j])){
			str[idx][j-i] = expr[j];
			j++;
		}
		str[idx][j - i] = '\0';
		idx++;
		i = j;
		continue;
     	}
	else if(c == 's'){
		char funct[8] = "sin";
		push_op(&operators, 3, funct);
		i += 3;
		continue;
	}
	else if(c == 'c'){
		char funct[8] = "cos";
		push_op(&operators, 3, funct);
		i += 3;
		continue;
	}
	else if(c == 't'){
		char funct[8] = "tan";
		push_op(&operators, 3, funct);
		i += 3;
		continue;
	}
	else if(c == 'l'){
		char funct[8] = "ln";
		push_op(&operators, 2, funct);
		i+=2;
		continue;
	}
	else if(c == 'f'){
		char funct[8] = "fac";
		push_op(&operators, 3, funct);
		i += 3;
		continue;
	}
	else if(c == 'm'){
		char funct[8] = "mod";
		push_op(&operators, 3, funct);
		i += 3;
		continue;
	}
	else if(c == 'r'){
		char funct[8] = "root\0";
		push_op(&operators, 5, funct);
		i += 4;
		continue;
	}
	else if(c == 'a'){
		if(expr[i+3] == 's'){
			char funct[8] = "arcsin";
			push_op(&operators, 6, funct);
			i += 6;
			continue;
		}
		else if(expr[i+3] == 'c'){
			char funct[8] = "arccos";
			push_op(&operators, 6, funct);
			i += 6;
			continue;
		}
		if(expr[i+3] == 't'){
			char funct[8] = "arctan";
			push_op(&operators, 6, funct);
			i += 6;
			continue;
		}
	}
	else if(c == '('){
		char lbrak[8] = "(";
		push_op(&operators, 1, lbrak);
		unmatched_braks ++;
		continue;
	}
	else if(c == ')'){
		while(operators.str[operators.top][0] != '('){
			char opr[8];
			int a = pop_op(&operators, opr); 
			for(int i = 0; i<a; i++){
			     str[idx][i] = opr[i];
			}
			idx++;
		}
		char opr[8];
		pop_op(&operators, opr);
		unmatched_braks --;
		if(!is_op(operators.str[operators.top][0]) && operators.top >= 0 && operators.str[operators.top][0] != '(' && operators.str[operators.top][0] != ')' ){
			char opr[8];
			int a = pop_op(&operators, opr);
			for(int i = 0; i<a; i++){
			     str[idx][i] = opr[i];
			}
			idx++;	
		}
	}
	else if(is_op(c) > 0){
		while(operators.top >= 0 && (is_op(c) <= is_op(operators.str[operators.top][0]))){
			char opr[8];
			int a = pop_op(&operators, opr); 
			for(int i = 0; i<a; i++){
			     str[idx][i] = opr[i];
			}
			idx++;	
		}
		char newop[8];
		newop[0] = c;
        newop[1] = '\0';
		push_op(&operators, 1, newop);
	}
     }
	while(operators.top >= 0){
		char opr[8];
		int a = pop_op(&operators, opr);
		for(int i = 0; i<a; i++)
			str[idx][i] = opr[i];
		idx++;	
	}
	return idx;
}

float num(char str[100]){
    if(str[0] == '-'){
	char new_str[100];
	for(int i = 1; i<100; i++){
		new_str[i-1] = str[i];
		return -num(new_str);
	}
    }
    int len = 0, len2 = 0, flag = 0;
    float num = 0.0f;
    for(int i = 0; i<100; i++){
        if(str[i] != '\0')
            len ++;
        else
            break;
        if(str[i] != '.' && flag == 0){
            len2++;
        }
        else
            flag = 1;
    }
    if(flag == 0){
        for(int i = 0; i<len; i++){
            num += (str[i] - '0') * power_10(len-i-1);
        }
    }
    else{
        for(int i = 0; i<len2; i++){
            float k = (str[i] - '0') * power_10(len2-i-1);
            num += (str[i] - '0') * power_10(len2-i-1);
        }
        for(int i = len2+1; i<len; i++){
            float k = (str[i] - '0') * power_10(len2- i); 
            num += k;
        }
    }
    return num;
}

float evalpostfix(int n, char str[100][15]){
    struct Stack st;
    st.top = -1;
    for(int i = 0; i<n; i++){
        if(isnum(str[i])){
            push(&st, num(str[i]));
        }
        else{
            if(str[i][0] == '+'){
		        float a = pop(&st);
		        float b = pop(&st);
                float sum = a + b;
                push(&st, sum);
            }
            else if(str[i][0] == '-'){
		float b = pop(&st);
		float a = pop(&st);
                float diff = a-b;
                push(&st, diff);
            }
            else if(str[i][0] == '*'){
		float a = pop(&st);
		float b = pop(&st);
                float product = a * b;
                push(&st, product);
            }
            else if(str[i][0] == '/'){
                float b = pop(&st);
		float a = pop(&st);
                float quotient = a/b;
                push(&st, quotient);
            }
	    else if(str[i][0] == 's' && str[i][1] == 'i' && str[i][2] == 'n'){
		float inp = pop(&st);	
		float res = sin_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 'c' && str[i][1] == 'o' && str[i][2] == 's'){
		float inp = pop(&st);
		float res = cos_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 't' && str[i][1] == 'a' && str[i][2] == 'n'){
		float inp = pop(&st);
		float res = tan_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 'f' && str[i][1] == 'a' && str[i][2] == 'c'){
		float inp = pop(&st);
		float res = factorial(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 'm' && str[i][1] == 'o' && str[i][2] == 'd'){
		float inp = pop(&st);
		float res = mod_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 'a' && str[i][1] == 'r' && str[i][2] == 'c' && str[i][3] == 's' && str[i][4] == 'i' && str[i][5] == 'n'){
		float inp = pop(&st);
		float res = arcsin_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 'a' && str[i][1] == 'r' && str[i][2] == 'c' && str[i][3] == 'c' && str[i][4] == 'o' && str[i][5] == 's') {
		float inp = pop(&st);
		float res = arccos_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 'a' && str[i][1] == 'r' && str[i][2] == 'c' && str[i][3] == 't' && str[i][4] == 'a' && str[i][5] == 'n'){
		float inp = pop(&st);
		float res = arctan_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == 'l' && str[i][1] == 'n'){
		float inp = pop(&st);
		float res = ln_x(inp);
		push(&st, res);
	    }
	    else if(str[i][0] == '^'){
		float exp = pop(&st);
		float base = pop(&st);
		push(&st, power_x(base, exp));
	    }
	    else if(str[i][0] == 'r' && str[i][1] == 'o' && str[i][2] == 'o' && str[i][3] == 't'){
		float inp = pop(&st);
		float res = 1/inv_sq_x(inp);
		push(&st, res);
	    }
        }
    }
    return st.arr[st.top];
}

int main(){
        char body[1000] = "3 + 2";
        float res = 0;
        char str[100][15];
        int n = shuntingyard(body, str);
        res = evalpostfix(n, str);
        //printf("%f\n", res);
}

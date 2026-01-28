#include <stdio.h>
#define PI 3.141592

float inv_sq_x(float x){
    long i = *( long *) &x;
    i  = 0x5f3759df - ( i >> 1 );
    float y  = *(float *)&i;
    y = y*(1.5f - (0.5f*x*y*y));
    y = y*(1.5f - (0.5f*x*y*y));
    y = y*(1.5f - (0.5f*x*y*y));
    y = y*(1.5f - (0.5f*x*y*y));
    return y;
}

float func(float x, float y, int w, int a){
    switch(a){
        case 1: return 1/x;
        break;
        case 2: return 1/(1+x*x);
        break;
        case 3: return inv_sq_x(1 - (x*x));
        break;
        case 4: return ((float)w)*y/x;
        break;
    }
}

float single_nxt_itr(int a, int w, float t_n, float y_n, float h, int n){
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
    return single_nxt_itr(1, 0, 1, 0, 0.01, 100*(x-1));
}


float arcsin_x(float x){
    if(x < 0)
        return -arcsin_x(-x);
    return single_nxt_itr(3, 0, 0, 0, 0.00001, 100000*x);
}

float arccos_x(float x){
    return (PI/2)- arcsin_x(x);
}

float arctan_x(float x){
    return single_nxt_itr(2, 0, 0, 0, 0.01, 100*x);
}

float power_x(float x, float w){
    return single_nxt_itr(4, w, 0, 0, 0.01, 100*x);
}

int main(){
    int n;
    scanf("%d", &n);
    float res = sin_next_itr(0, 1, 0.1, n);
    printf("%f\n", power_x(3,1.5));
}

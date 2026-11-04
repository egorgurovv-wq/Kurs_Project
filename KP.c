#include <stdio.h>
#include <math.h>
#include <locale.h>
float calc_Q(double n, double p){
    if (p == 0.0){
        return 0.0;
    }
    return (n - p) / p;
}

int check_Q(float q, float l){
    return fabs(q) > l;
}

int main(){
    setlocale(LC_ALL, "RUS");

    float l =2.0;
    float n = 0.0;
    float p1 =0.0;
    float p2 =0.0;
    float q = 0.0;
    int k =2;

    printf("Программа контроля облачной инфраструктуры\n");
    printf("Введите 5 значений\n");
    for (int i = 0; i < 5; i++){
        int time = i + 1;
        printf("Введите значение для времени %dс: \n", time);
        scanf(" %f", &n);

        if (n==-99999){
            printf("Выход из программы\n");
            break;
        }

        if (time <= k){
            printf("Для времени %dс нет данных для сравнения\n", time);
        }else{
            q = calc_Q(n, p2);
            printf("Q = %.2f\n", q);
            if (check_Q(q, l)){
                printf("Состояние: ТРЕВОГА!\n");
        }else{
            printf("Состояние: Норма\n");
            }
        }
        p2 = p1;
        p1 = n;
    }
    return 0;
}
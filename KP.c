#include <stdio.h>
#include <locale.h>
#include <math.h>

float for_Q(float n, float p){
    return (n - p)/p;
}

typedef struct {
    int time;
    int type;
    float value;
    int count;
    int duration;
} Incident;


int main(){
    setlocale(LC_ALL, "RUS");
    float iop[5] = {43.7, 44.2, 44.7, 50.6, 51.1};
    float srv[5] = {53.8, 57.2, 55.2, 53.2, 51.2};
    float ram[5] = {32.0, 31.3, 30.6, 35.2, 34.5};
    int k = 2;
    float l = 2.0;

    printf("Расчет Q\n");

    for (int i =2; i<5; i++){
        int time = i +1;

        float Q_iop = for_Q(iop[i], iop[i-k]);
        float Q_srv = for_Q(srv[i], srv[i-k]);
        float Q_ram = for_Q(ram[i], ram[i-k]);

        printf("Для времени %d, сравнение с %d:\n", time, time-k);
        printf("IOP: Q = %+.3f\n", Q_iop);
        if (fabs(Q_iop) > l){
            printf("Состояние: ТРЕВОГА\n");
        }else{
            printf("Состояние: НОРМА\n");
        }

        printf("SRV: Q = %+.3f\n", Q_srv);
        if (fabs(Q_srv) > l){
            printf("Состояние: ТРЕВОГА\n");
        }else{
            printf("Состояние: НОРМА\n");
        }

        printf("RAM: Q = %+.3f\n", Q_ram);
        if (fabs(Q_ram) > l){
            printf("Состояние: ТРЕВОГА\n");
        }else{
            printf("Состояние: НОРМА\n");
        }


        int q_up = (Q_iop>0) || (Q_srv >0) || (Q_ram > 0);
        int q_fall = (Q_iop<0) || (Q_srv <0) || (Q_ram < 0);

        int incedent = (q_up && q_fall);
        if (incedent){
            Incident inc;
            inc.time = time;
            inc.type = 1;
            inc.value = fabsf(Q_iop);
            inc.count = 3;
            inc.duration = 1;
            printf("Обнаружен инцидент! (время: %d, тип: %d)\n\n", inc.time, inc.type);
        } else {
            printf("Инцидентов не обнаружено.\n\n");
        }
    }


}
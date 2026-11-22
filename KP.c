#include <stdio.h>
#include <math.h>
#include <locale.h>

typedef struct {
    int time;
    int type;
    float value;
    int count;
    int duration;
} Incident;

float calc_Q(float n, float p);
int check_Q_error(float n, float p);
int check_Q(float q, float l);
int get_trend(float q, int err);
int check_incident(int trends[], int errs[], int comp_count);
void print_component_result(const char name[], float q, int status, int trend, int err);

int main() {
    setlocale(LC_ALL, "RUS");

    float l = 2.0;
    float iop[5] = {43.7, 44.2, 44.7, 50.6, 51.1};
    float srv[5] = {53.8, 57.2, 55.2, 53.2, 51.2};
    float ram[5] = {32.0, 31.3, 30.6, 35.2, 34.5};

    int choice = -1;
    int k = 2;

    printf("Программа контроля облачной инфраструктуры\n");

    do {
        printf("=== Меню режима работы ===\n");
        printf("1 - Полный отчет (все отсчеты и компоненты)\n");
        printf("2 - Только инциденты\n");
        printf("0 - Выход\n");
        printf("Выберите режим: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                for (int i = 0; i < 5; i++) {
                    int time = i + 1;
                    if (i < k) {
                        printf("Для времени %dс нет данных для сравнения\n", time);
                    } else {
                        int iop_err = check_Q_error(iop[i], iop[i - k]);
                        int srv_err = check_Q_error(srv[i], srv[i - k]);
                        int ram_err = check_Q_error(ram[i], ram[i - k]);

                        float iop_q = calc_Q(iop[i], iop[i - k]);
                        float srv_q = calc_Q(srv[i], srv[i - k]);
                        float ram_q = calc_Q(ram[i], ram[i - k]);

                        int iop_status = check_Q(iop_q, l);
                        int srv_status = check_Q(srv_q, l);
                        int ram_status = check_Q(ram_q, l);

                        int iop_trend = get_trend(iop_q, iop_err);
                        int srv_trend = get_trend(srv_q, srv_err);
                        int ram_trend = get_trend(ram_q, ram_err);

                        int trends[3] = {iop_trend, srv_trend, ram_trend};
                        int errs[3] = {iop_err, srv_err, ram_err};
                        int incident = check_incident(trends, errs, 3);

                        printf("\nДля времени %dс сравнение с %dс\n", time, time - k);
                        print_component_result("IOP", iop_q, iop_status, iop_trend, iop_err);
                        print_component_result("SRV", srv_q, srv_status, srv_trend, srv_err);
                        print_component_result("RAM", ram_q, ram_status, ram_trend, ram_err);
                        if (incident) {
                            Incident inc;
                            inc.time = time;
                            inc.type = 1;
                            inc.value = fabsf(iop_q);
                            inc.count = 3;
                            inc.duration = 1;
                            printf("Обнаружен инцидент! (время: %d, тип: %d)\n\n", inc.time, inc.type);
                        } else {
                            printf("Инцидентов не обнаружено.\n\n");
                        }
                    }
                }
                break;

            case 2: {
                int inc_count = 0;
                printf("\n--- Инциденты ---\n");
                for (int i = k; i < 5; i++) {
                    int time = i + 1;
                    int iop_err = check_Q_error(iop[i], iop[i - k]);
                    int srv_err = check_Q_error(srv[i], srv[i - k]);
                    int ram_err = check_Q_error(ram[i], ram[i - k]);

                    float iop_q = calc_Q(iop[i], iop[i - k]);
                    float srv_q = calc_Q(srv[i], srv[i - k]);
                    float ram_q = calc_Q(ram[i], ram[i - k]);

                    int iop_trend = get_trend(iop_q, iop_err);
                    int srv_trend = get_trend(srv_q, srv_err);
                    int ram_trend = get_trend(ram_q, ram_err);

                    int trends[3] = {iop_trend, srv_trend, ram_trend};
                    int errs[3] = {iop_err, srv_err, ram_err};
                    int incident = check_incident(trends, errs, 3);

                    if (incident) {
                        Incident inc;
                        inc.time = time;
                        inc.type = 1;
                        inc.value = fabsf(iop_q);
                        inc.count = 3;
                        inc.duration = 1;
                        inc_count++;
                        printf("Обнаружен инцидент! (время: %dс, тип: %d)\n", inc.time, inc.type);
                    }
                }
                printf("Всего инцидентов: %d\n\n", inc_count);
                break;
            }

            case 0:
                printf("Выход из программы.\n");
                break;

            default:
                printf("Неизвестный пункт меню. Выберите 1, 2 или 0.\n\n");
                break;
        }
    } while (choice != 0);

    return 0;
}

float calc_Q(float n, float p) {
    if (isnan(n) || isnan(p) || isinf(n) || isinf(p)) {
        return 0.0f;
    }
    if (fabsf(p) < 1e-6f) {
        return 0.0f;
    }
    return (n - p) / p;
}

int check_Q_error(float n, float p) {
    if (isnan(n) || isnan(p) || isinf(n) || isinf(p)) {
        return 2;
    }
    if (fabsf(p) < 0.000001f) {
        return 1;
    }
    float q = (n - p) / p;
    if (isnan(q) || isinf(q)) {
        return 2;
    }
    return 0;
}

int check_Q(float q, float l) {
    return fabsf(q) > l;
}

int get_trend(float q, int err) {
    if (err != 0) {
        return 0;
    }
    if (q > 0.0f) {
        return 1;
    } else if (q < 0.0f) {
        return -1;
    } else {
        return 0;
    }
}

int check_incident(int trends[], int errs[], int comp_count) {
    int q_up = 0;
    int q_fall = 0;

    for (int j = 0; j < comp_count; j++) {
        if (errs[j] == 0) {
            if (trends[j] == 1) {
                q_up = 1;
            } else if (trends[j] == -1) {
                q_fall = 1;
            }
        }
    }

    int incedent = (q_up && q_fall);
    return incedent;
}

void print_component_result(const char name[], float q, int status, int trend, int err) {
    if (err == 1) {
        printf("%s: Ошибка вычисления (деление на ноль!)\n", name);
        return;
    }
    if (err != 0) {
        printf("%s: Ошибка вычисления (недопустимое значение!)\n", name);
        return;
    }

    printf("%s: Q = %+.3f | ", name, q);
    if (status == 1) {
        printf("Состояние: ТРЕВОГА | ");
    } else {
        printf("Состояние: НОРМА | ");
    }

    if (trend == 1) {
        printf("Тенденция: РОСТ\n");
    } else if (trend == -1) {
        printf("Тенденция: СНИЖЕНИЕ\n");
    } else {
        printf("Тенденция: БЕЗ ИЗМЕНЕНИЙ\n");
    }
}

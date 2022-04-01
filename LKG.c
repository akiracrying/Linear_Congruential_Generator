#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 10000  //кол-во генераций на тест хи-квадрат(генераций чисел)
#define L 9       //кол-во чисел для теста серий и проверки на младшие биты
#define BINARY_ARRAY_SIZE 1000
#define GROUP_SIZE 10
#define SERIES_SIZE 3

typedef unsigned long long ull;

int arr_count = 0, arr[BINARY_ARRAY_SIZE * BINARY_ARRAY_SIZE], binaryMas[BINARY_ARRAY_SIZE];
int top_pointer = BINARY_ARRAY_SIZE - 1;
unsigned int summ = 0;

unsigned int a = 6364136223846793005;
unsigned int c = 1442695040888963407;

void binary(unsigned long long x) {
    while (x >= 2) {
        binaryMas[top_pointer--] = x % 2;
        x /= 2;
        summ++;
    }
}

//для проверки младших бит
void leastSignBit(unsigned long long x) {
    for (int i = 0; i < 16; i++) {
        if (x % 2)
            arr[arr_count + 1]++;      // перевод в двоичку
        else arr[arr_count]++;
        x /= 2;
    }
}

#ifdef TestleastSignBit2
void leastSignBit(unsigned long long x) {
    int y = 0, k = 0;

    for (y = 0; x != 0; x >>= 1) {
        k++;
        if (x & 1) {
            y++;
        }
    }
    arr[1] += y;
    arr[0] = d[0] + k - y;
}
#endif // TestleastSignBit2


//тест несцеплённых серий
void seriesTest(int n, int mass[]) {
    int m = SERIES_SIZE, s = 0; //длина серии (берём по 3 числа)
    int vv[8] = { 0 };
    for (int i = (BINARY_ARRAY_SIZE - n); i <= (BINARY_ARRAY_SIZE - (n % m) - 3); i += 3) { // идём с конца тк двоичное
        s = mass[i] * 4 + mass[i + 1] * 2 + mass[i + 2]; // берёт 3 числа и складывает а потом переводит в 10-ю сс (максимум число 7 (0-7))
        vv[s]++;                                         // таким образом будет 7 групп
    }
    double XI = 0; // тут хи кв по формуле
    double nm;
    nm = (n / m) / pow(2, m);
    for (int j = 0; j < 8; j++)
        XI += pow((vv[j] - nm), 2);

    XI /= nm;
    printf("CHI-square for series test: %f\n", XI);
}

//хи квадрат
void CHI(unsigned long long X[N]) {
    double XI = 0;
    double observed[N] = { 0 };
    int expected = N / GROUP_SIZE; // ожидаемое число попаданий в интервал
    // проверка на хи-квадрат
    for (int i = 0; i < BINARY_ARRAY_SIZE; i++) {
        for (int j = 0; j < N; j++) {
            if (i == X[j] % BINARY_ARRAY_SIZE) // чтобы были повторябщиеся
                observed[i]++;
        }
    }
    float obs[GROUP_SIZE] = { 0 };          //разбиваем на 10 групп по 100
    for (int i = 0; i < GROUP_SIZE; i++) {
        for (int j = BINARY_ARRAY_SIZE/ GROUP_SIZE * i; j < BINARY_ARRAY_SIZE/ GROUP_SIZE * (i + 1); j++) {
            obs[i] += (float)observed[j]; //подсчет элементов в каждой группе
        }
        XI += (obs[i] - expected) * (obs[i] - expected) / (expected); //высчитывание значения ХИ квадрата
    }
    printf("\nCHI-square test: %f\n", XI);
}

//лкг

int main() {
    ull X[N];
    srand(time(NULL));
    ull x = rand();
    printf("starting x value = %llu\n\n", x);
    for (int i = 0; i < N; i++) {
        X[i] = x;
        if (i < L) {
            binary(x);
        }
        leastSignBit(x); // проверка млаших бит
        arr_count += 2; // смотрим только младшие(~последние 8) => перешагиваем через 2
        x = (a * x + c); // основная формула ЛКГ
    }
    printf("\tleast significant bit test:\n");
    int j = 0;
    for (int i = 0; i < 2 * L; i += 2, j++) {
        printf("number %d\n ZEROS amount: %d\n ONES amount: %d\n", j + 1, arr[i], arr[i + 1]);
    }
    CHI(X);
    seriesTest(summ, binaryMas);
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 8
#define NUM_PRODUCTS 8

char productos[][30] = {"Porcion pastel de chocolate: ", "White mocha: ", "Cafe americano 8oz:", "Latte 8oz:", "Toffee coffee: ", "Cappuccino 8oz: ", "S'mores latte: ", "Cafe tostado molido: "};
float unitario[] = {60.00, 32.00, 22.00, 24.00, 28.00, 24.00, 32.00, 60.00};
float fijo[] = {20.00, 19.20, 13.20, 17.20, 20.10, 17.20, 23.00, 20.00};
int vendidosJulio[] = {300, 400, 1590, 200, 390, 1455, 800, 60};
int vendidosAgosto[] = {250, 380, 800, 250, 600, 1200, 1540, 15};

float totVentasJulio[NUM_PRODUCTS];
float totUtilidadesJulio[NUM_PRODUCTS];
float totVentasAgosto[NUM_PRODUCTS];
float totUtilidadesAgosto[NUM_PRODUCTS];

pthread_mutex_t mutex;

void *ventasProducto(void *arg) {
    int id = *(int *)arg;
    printf("calculo hilo %d iniciado\n", id);
    sleep(1); 
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        totVentasJulio[i] = vendidosJulio[i] * unitario[i];
        totVentasAgosto[i] = vendidosAgosto[i] * unitario[i];
    }
    printf("calculo hilo %d terminado\n", id);
    return NULL;
}

void *utilidadProducto(void *arg) {
    int id = *(int *)arg;
    printf("calculo hilo %d iniciado\n", id);
    sleep(1); 
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        totUtilidadesJulio[i] = totVentasJulio[i] - (vendidosJulio[i] * fijo[i]);
        totUtilidadesAgosto[i] = totVentasAgosto[i] - (vendidosAgosto[i] * fijo[i]);
    }
    printf("calculo hilo %d terminado\n", id);
    return NULL;
}

void *totVentas(void *arg) {
    int id = *(int *)arg;
    printf("calculo hilo %d iniciado\n", id);
    sleep(1); 
    printf("calculo hilo %d terminado\n", id);
    return NULL;
}

void *totUtilidades(void *arg) {
    int id = *(int *)arg;
    printf("calculo hilo %d iniciado\n", id);
    sleep(1); 
    printf("calculo hilo %d terminado\n", id);
    return NULL;
}

void reporteFinal(char *mes, float *ventas, float *utilidades) {
    printf("------------------------------------------\n");
    printf("REPORTE DEL MES DE %s\n", mes);
    printf("--- Monto Ventas por Producto ---\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("%s Q%.0f\n", productos[i], ventas[i]);
    }
    printf("--- Utilidad por Producto ---\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("%s Q%.0f\n", productos[i], utilidades[i]);
    }
    float totalVentas = 0;
    float totalUtilidades = 0;
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        totalVentas += ventas[i];
        totalUtilidades += utilidades[i];
    }
    printf("--- Monto Total Ventas del Mes: Q%.0f \n", totalVentas);
    printf("--- Utilidad del mes: Q%.0f\n", totalUtilidades);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        if (i < 2) {
            pthread_create(&threads[i], NULL, ventasProducto, &ids[i]);
        } else if (i < 4) {
            pthread_create(&threads[i], NULL, utilidadProducto, &ids[i]);
        } else if (i < 6) {
            pthread_create(&threads[i], NULL, totVentas, &ids[i]);
        } else {
            pthread_create(&threads[i], NULL, totUtilidades, &ids[i]);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    reporteFinal("JULIO", totVentasJulio, totUtilidadesJulio);
    reporteFinal("AGOSTO", totVentasAgosto, totUtilidadesAgosto);

    return 0;
}
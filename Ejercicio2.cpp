/*
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * CC3086 - Programacion de Microprocesadores
 * Ver.: 1.1 2024/21/08
 * Mod.: Jorge Luis Felipe Aguilar Portillo
 * Mod.: Angie Nadissa Vela López
 * Carnet: 23764
 *
 * References:
 * Rauber, T., & Rünger, G. (2023). Parallel programming:
 * For multicore and cluster systems (3a ed.). Springer International Publishing.
 * -------------------------------------------------------------
 * Programa que simula el funcionamiento de una fábrica
 * de producción de café tostado molido
 * -------------------------------------------------------------
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//bolsas de cafe a producir
#define BOLSAS_CAFE 400

pthread_mutex_t candado;
pthread_cond_t condStock; 
//stock existente en el silo
int stockCafe = 0, stockBodega = 0;

/* Funcion donde se maneja la logica del stock en el silo 
 * si la llama la funcion tostarCafe se agrega 1 a stockCafe
 * si la llama la funcion empacarCafe se resta 1 a stockCafe y se suma 1 a stockBodega
 */

void stockSilo(int operacion, int cafeProducido){
    
    pthread_mutex_lock(&candado);

    //sumar stock al silo
    if(operacion == 0){
        stockCafe += cafeProducido;
    }else if(operacion == 1){//restar stock al silo
        while(stockCafe < 5 && stockBodega < 396){//condicion para que el stockCafe no llegue a valores negativos
            printf("\nNo hay suficiente cafe en el silo... esperando para empacar");
            pthread_cond_wait(&condStock, &candado);
        }
        stockCafe -= cafeProducido;
        stockBodega += cafeProducido;
        printf("\nLibras de cafe en la bodega: %d", stockBodega);
        
    }
   
    printf("\nLibras de cafe en el silo: %d", stockCafe);
    pthread_mutex_unlock(&candado);
}

//función de las tostadoras 1 y 2
void *tostarCafe(void* id_hilo){
    int id = *((int*) id_hilo);

    for(int i = 0; i < BOLSAS_CAFE / 2; i++){
        printf("\nTostadora %d produjo: 1 lb de café tostado", id);
        //parametro operacion 0 sumar a stockCafe, 1 restar a stockCafe
        stockSilo(0, 1);
        pthread_cond_signal(&condStock);//doy la señal que espera el cond_wait
        sleep(1);
    }
    
    return NULL;
}

//funcion de la empacadora
void *empacarCafe(void* arg){
    while(stockBodega < BOLSAS_CAFE){
        //parametro operacion 0 sumar a stockCafe, 1 restar a stockCafe
        stockSilo(1, 1);
        printf("\nEmpacadora produjo: 1 bolsa de 1 lb de café");
        sleep(1);
    }
    return NULL;
}



int main(){
    pthread_t hilos[3];
    int id1 = 1, id2 = 2;

    pthread_mutex_init(&candado, NULL);
    pthread_cond_init(&condStock, NULL);

    // creacion hilos
    //tostadora 1
    pthread_create(&hilos[0], NULL, tostarCafe, &id1);
    //tostadora 2
    pthread_create(&hilos[1], NULL, tostarCafe, &id2);
    //empacadora
    pthread_create(&hilos[2], NULL, empacarCafe, NULL);

    // join hilos
    for (int i = 0; i < 3; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // destrucción mutex y variable condicional
    pthread_mutex_destroy(&candado);
    pthread_cond_destroy(&condStock);
    //reporte final de stock
    printf("\n\nLibras de café en silo: %d\n", stockCafe);
    printf("Libras de café en bodega: %d\n", stockBodega);

    return 0;
}

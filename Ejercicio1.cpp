#include <stdio.h>
#include <pthread.h>

pthread_mutex_t candadoVentas;
pthread_mutex_t candadoUtilidades;
pthread_cond_t cond;

//Pastel de chocolate, white mocha, Americano, Late, Toffee, Capuchino, S'mores, Tostado
float unitario[] = {60.00, 32.00, 22.00, 24.00, 28.00, 24.00, 32.00, 60.00};
float fijo[] = {20.00, 19.20, 13.20, 17.20, 20.10, 17.20, 23.00, 20.00};
int vendidosJulio[] = {300, 400, 1590, 200, 390, 1455, 800, 60};
int vendidosAgosto[] = {250, 380, 800, 250, 600, 1200, 1540, 15};
string productos[] = {"Porcion pastel de chocolate: ", "White mocha: ", "Cafe americano 8oz:", "Latte 8oz:", "Toffee coffee: ", "Cappuccino 8oz: ", "S'mores latte: ", "Cafe tostado molido: "}

float totVentasJulio[sizeof(unitario)];
float totUtilidadesJulio[sizeof(unitario)];
float totVentasAgosto[sizeof(unitario)];
float totUtilidadesAgosto[sizeof(unitario)];

void *ventasProducto(void *arg){


    printf("------------------------------------------------------");
    printf("REPORTE DEL MES DE JULIO");
    printf("--- Monto Ventas por Producto ---")
    for (int i= 0; i < sizeof(unitario); i++){
        totVentasJulio[i] = vendidosJulio[i]*unitario[i];

    }

    for (int i= 0; i < sizeof(unitario); i++){
        totVentasAgosto[i] = vendidosAgosto[i]*unitario[i];
    }
}

void *utilidadProducto(void *arg){
    for (int i= 0; i < sizeof(unitario); i++){
        totUtilidadesJulio[i] = totVentasJulio[i] - (vendidosJulio[i]*fijo[i]);
        totUtilidadesAgosto[i] = totVentasAgosto[i] - (vendidosAgosto[i]*fijo[i]);
    }

}

void *totVentas(void *arg){
    float totalJulio = 0;
    float totalAgosto = 0;
    int *x = (int *)arg;

	pthread_mutex_lock(&candadoVentas);
	if (*x == 0)
		pthread_cond_wait(&cond, &candadoVentas);	

    for (int i= 0; i < sizeof(unitario); i++){
        totalJulio += totVentasJulio[i];
        totalAgosto += totVentasAgosto[i];
    }
    printf("Total ventas Julio: %f \n", totalJulio);
    printf("Total ventas Agosto: %f \n", totalAgosto);

    pthread_mutex_unlock(&candadoVentas);
}

void * totUtilidades(void *arg){
    float totalJulio = 0;
    float totalAgosto = 0;
    int *x = (int *)arg;

	pthread_mutex_lock(&candadoUtilidades);
	if (*x == 0)
		pthread_cond_wait(&cond, &candadoUtilidades);

    for (int i= 0; i < sizeof(unitario); i++){
        totalJulio += totUtilidadesJulio[i];
        totalAgosto += totUtilidadesAgosto[i];
    }
    printf("Total utilidades Julio: %f \n", totalJulio);
    printf("Total utilidades Agosto: %f \n", totalAgosto);


    pthread_mutex_unlock(&candadoUtilidades);

}

int main(){

}
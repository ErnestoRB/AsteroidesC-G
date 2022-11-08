# include "nave.h"

int aleatorioEntre(int lo, int hi)
{
    return lo + (rand() % (hi - lo));
}

float aleatorioEntreF(float lo, float hi)
{
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

bool colision(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;
    return true;
}


void debeIniciar(bool respuesta, char nombre[]) {
    switch(respuesta){
        case true:
            printf("Se cargo %s\n", nombre);
        break;

        case false:
            printf("No se pudo iniciar/cargar %s", nombre);
            exit(1);
        break;

    }
}

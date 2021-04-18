# include "nave.h"
# include "nave_vars.h"


void reiniciarDisparos() {
    for(int i=0;i<OBJETO_MAX;i++) {
        disparos[i].activo = false;
    }
}

void disparar(bool jugador) {
    for(int i=0;i<OBJETO_MAX;i++) {
        if(disparos[i].activo==false) { // buscar un espacio vacio
            disparos[i].jugador = jugador;
            switch (jugador) {
                case true:
                    disparos[i].coords[0][0] = ((nave.coords[1][0]-nave.coords[0][0]) / 2)+nave.coords[0][0];
                    disparos[i].coords[1][0] = disparos[i].coords[0][0] + 9;
                    disparos[i].coords[0][1] = nave.coords[0][1]-33;
                    disparos[i].coords[1][1] = disparos[i].coords[0][1] + 33;
                    disparos[i].activo = true;
                break;
                case false:
                    if(alien.vida>0) { // disparar solo cuando está vivo
                        disparos[i].coords[0][0] = (((alien.x + anchoalien)  - alien.x) / 2) + alien.x;
                        disparos[i].coords[1][0] = disparos[i].coords[0][0] + 9;
                        disparos[i].coords[0][1] = alien.y+33;
                        disparos[i].coords[1][1] = disparos[i].coords[0][1] + 33;
                        disparos[i].activo = true;
                    }
                break;
            }
            break;
        }
    }
 }

void dibujarDisparos(){
    for(int i=0;i<OBJETO_MAX;i++) {

        if(disparos[i].activo) {
            switch(disparos[i].jugador) {
                case true:
                    disparos[i].coords[0][1] -= 8;

                break;
                case false:
                    disparos[i].coords[0][1] += 8;
                break;
            }
            disparos[i].coords[1][1] = disparos[i].coords[0][1] + 33;

            al_draw_bitmap(balas,disparos[i].coords[0][0], disparos[i].coords[0][1],0);
        }
    }

}


void comprobarDisparos(){
    for(int i=0;i<OBJETO_MAX;i++) {
        if(disparos[i].activo) {
            switch(disparos[i].jugador){
                case true:
                    if(disparos[i].coords[0][1] < 0){ // si sale de mapa
                        disparos[i].activo = false;
                    }
                break;
                case false:
                     if(disparos[i].coords[0][1] > 640){ // si sale de mapa
                        disparos[i].activo = false;
                    }
                break;
            }

        }
    }

}

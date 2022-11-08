# include "nave.h"
# include "nave_vars.h"

void crearAlien(int x, int y) {
    int max = 640-anchoalien;
    if(x > max) {x=max;}
    if(alien.vida==0 && !alienronda) {
        alien.vida=(ronda*10);
        alien.x = x;
        alien.y = y;
        alienronda=true;
    }
}

void controlarAlien() {
    if(alien.vida>0) {
        // direccion de la nave de forma aleatoria
        int direccion = aleatorioEntre(0,2);
        switch(direccion) {
            case 0: // derecha
                alien.dir= 2*ronda; // velocidad ajustada a la ronda (2px * ronda) cada fotograma
                break;
            case 1: // izquierda
                alien.dir= -2*ronda;
                break;
        }
        alien.x += alien.dir;
        if ((alien.x + anchoalien) > 640) { alien.x = 640-anchoalien;}
        if (alien.x<0) { alien.x=0;}

        /* Comprobar colisiones */
        for(int i=0;i<OBJETO_MAX;i++) {
            if(disparos[i].activo) { // que esté activo y que sea de un jugador
                switch(disparos[i].jugador) { // depende si el disparo fue de un jugador
                    case true:
                        // colision entre disparo de jugador y nave alien
                        if(colision(disparos[i].coords[0][0], disparos[i].coords[0][1],disparos[i].coords[1][0],disparos[i].coords[1][1], alien.x, alien.y, alien.x+anchoalien, alien.y+altoalien)) {
                            al_play_sample(colision_nave,0.05,1.0,2.0,ALLEGRO_PLAYMODE_ONCE,0); // sonido
                            alien.vida--;
                            disparos[i].activo = false;
                        }
                        break;
                    case false:
                        // colision entre disparo de alien con nave
                        if(colision(disparos[i].coords[0][0], disparos[i].coords[0][1],disparos[i].coords[1][0],disparos[i].coords[1][1], nave.coords[0][0], nave.coords[0][1], nave.coords[1][0], nave.coords[1][1])) {
                            al_play_sample(colision_nave,0.05,1.0,2.0,ALLEGRO_PLAYMODE_ONCE,0); // sonido
                            nave.vidas--;
                            disparos[i].activo = false;
                        break;
                        }
                }

            }
        }
    }
}

void imprimirAlien() {
    if(alien.vida>0) {
        al_draw_bitmap(alien.BITMAP,alien.x,alien.y,0);
        if (hitbox) { al_draw_filled_rectangle(alien.x , alien.y , alien.x+anchoalien , alien.y+altoalien,al_map_rgba_f(0, 0, 0.5, 0.5));}
    }
}

void reiniciarAlien(){
    alien.vida=0;
    alien.dir=0;
    alienronda=false;
}

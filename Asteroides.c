# include "nave.h"
# include "nave_vars.h"


void crear_asteroide(int vida, int posY, int posX, int velocidad) {
    // determinara la velocidad del asteroide
    // determiara la posicion en X en la que va a aparecer el asteroide en pantalla
    int xmax=0; // para que no sobrepase limites de pantalla
    switch (vida) {
        case 3:
            xmax = 640-tamAsteroide3;
            break;
        case 2:
            xmax = 640-tamAsteroide2;
            break;
        case 1:
            xmax = 640-tamAsteroide1;
            break;
    }
    if(posX>xmax) { posX = xmax;}
    if(posX<0) {posX=0;}
    for(int i=0;i<OBJETO_MAX;i++) {
        if(asteroides[i].vida==0) { // casilla con un asteroide ya muerto?? entonces lo revivimos
            asteroides[i].vida = vida;
            asteroides[i].posX = posX;
            asteroides[i].posY = posY;
            if (velocidad>5) {velocidad=5;} // limite de velocidad
            asteroides[i].velocidad = velocidad;
            asteroides[i].BITMAP = al_load_bitmap(asteroideImagenes[vida-1]);
            if(asteroides[i].BITMAP==NULL){
                printf("No se encontró alguna textura de los asteroides (se cargan en el transcurso del juego!)");
                exit(1);
            }
            break; // nomas creamos un asteroide!!
        }
    }
}

// modifica las posiciones para que el asteroide avance
void controlarAsteroide() {

    for (int i = 0; i < OBJETO_MAX; i++) {
        // si la vida del asteroide es mayor a 0 se mueve de posicion
        if (asteroides[i].vida > 0) { // esta vivo?
            int ancho,alto;
            char animaciones[3][100]= {{"resources/animaciones/128x128/impacto.png"}, {"resources/animaciones/64x64/impacto2.png"}, {"resources/animaciones/32x32/impacto.png"}};
            switch(asteroides[i].vida)  {
                case 3:
                    ancho = asteroides[i].posX + tamAsteroide3;
                    alto = asteroides[i].posY + tamAsteroide3;
                    strcpy(animaciones[0], "resources/animaciones/128x128/impacto.png");
                    strcpy(animaciones[1], "resources/animaciones/64x64/impacto2.png");
                    strcpy(animaciones[2], "resources/animaciones/32x32/impacto.png");
                    break;
                case 2:
                    ancho = asteroides[i].posX + tamAsteroide2;
                    alto = asteroides[i].posY + tamAsteroide2;
                    strcpy(animaciones[0], "resources/animaciones/64x64/impacto.png");
                    strcpy(animaciones[1], "resources/animaciones/32x32/impacto2.png");
                    strcpy(animaciones[2], "resources/animaciones/32x32/impacto.png");
                    break;
                case 1:
                    ancho = asteroides[i].posX + tamAsteroide1;
                    alto = asteroides[i].posY + tamAsteroide1;
                    strcpy(animaciones[0], "resources/animaciones/32x32/impacto.png");
                    strcpy(animaciones[1], "resources/animaciones/32x32/impacto2.png");
                    strcpy(animaciones[2], "resources/animaciones/32x32/impacto.png");
                    break;
            }
            asteroides[i].posY += asteroides[i].velocidad; // mover hacia abajo

            for(int j=0;j<OBJETO_MAX;j++) { // comprobar cada disparo en busca de colision
                if (disparos[j].activo && disparos[j].jugador) { // disparo está activo y es de un jugador? si no ni pa buscar colision
                    if(colision(asteroides[i].posX,asteroides[i].posY, ancho, alto,disparos[j].coords[0][0],disparos[j].coords[0][1],disparos[j].coords[1][0], disparos[j].coords[1][1])) {
                        disparos[j].activo = false;

                        crearAnimacion(animaciones, 3, asteroides[i].posX, asteroides[i].posY, 5);
                        al_play_sample(colision_laser,0.05,1.0,2.0,ALLEGRO_PLAYMODE_ONCE,0);
                        switch(asteroides[i].vida){
                            case 3:
                                score+=20;
                                break;
                            case 2:
                                score+=50;
                                break;
                            case 1:
                                score+=100;
                                break;

                        }
                        if (asteroides[i].vida>1) {
                            crear_asteroide(asteroides[i].vida - 1, asteroides[i].posY, asteroides[i].posX+64, ronda);
                            crear_asteroide(asteroides[i].vida - 1, asteroides[i].posY, asteroides[i].posX-64, ronda);
                        }
                        asteroides[i].vida = 0;
                        //if(asteroidesCreados/(ronda*20)==1 && asteroidesCreados%(ronda*20)==0) { ronda++; } // cambiar de ronda
                        break; // encontró colision? ya no hay que comprobar más disparos
                    }
                }
            }

            if (colision(nave.coords[0][0],nave.coords[0][1],nave.coords[1][0], nave.coords[1][1], asteroides[i].posX, asteroides[i].posY, ancho, alto)) {
                    nave.vidas--;
                    al_play_sample(colision_asteroide,0.05,1.0,2.0,ALLEGRO_PLAYMODE_ONCE,0); // sonido
                    asteroides[i].vida=0;
            }

            // si la posicion del asteroide es mayor a pantallaY quiere decir
            // quiere decir que salio de pantalla asi que le ponemos vida=0
            if (asteroides[i].posY > 480) {
                asteroides[i].vida = 0;
            }



        }
    }

}

void dibujarAsteroides(){
 for (int i = 0; i < OBJETO_MAX; i++) {
        if (asteroides[i].vida > 0) {
            int ancho;
            int alto;
            switch(asteroides[i].vida) {
                case 3:
                    ancho = asteroides[i].posX + tamAsteroide3;
                    alto = asteroides[i].posY + tamAsteroide3;
                    break;
                case 2:
                    ancho = asteroides[i].posX + tamAsteroide2;
                    alto = asteroides[i].posY + tamAsteroide2;
                    break;
                case 1:
                    ancho = asteroides[i].posX + tamAsteroide1;
                    alto = asteroides[i].posY + tamAsteroide1;
                    break;

                }
            al_draw_bitmap(asteroides[i].BITMAP, asteroides[i].posX, asteroides[i].posY, 0);
            if (hitbox) { al_draw_filled_rectangle(asteroides[i].posX, asteroides[i].posY,ancho,alto,al_map_rgba_f(0, 0, 0.5, 0.5));}
        }
    }
}

void reiniciarAsteroides(){
    for(int i =0;i<OBJETO_MAX;i++) {
        asteroides[i].vida = 0;
    }
}

# include "nave.h"

extern struct ANIMACION animaciones[OBJETO_MAX];

void crearAnimacion(char fuente[][100], int imagenes, int x, int y, int duracion)   { // duracion en fotogramas por imagen
    for(int i=0;i<OBJETO_MAX;i++) {
        if(!animaciones[i].activo) {
            for(int j=0;j<imagenes;j++) {
                animaciones[i].BITMAP[j] = al_load_bitmap(fuente[j]);
            }
            animaciones[i].x = x;
            animaciones[i].y = y;
            animaciones[i].fotogramas = 0;
            animaciones[i].duracion=duracion;
            animaciones[i].imgrep=0;
            animaciones[i].activo = true;
            break; // nomas una animacion
        }
    }
}

void reproducirAnimaciones(){
    for(int i=0;i<OBJETO_MAX;i++) {
        if(animaciones[i].activo) {
            if(animaciones[i].imgrep!=3) {
                animaciones[i].fotogramas++;
                al_draw_bitmap(animaciones[i].BITMAP[animaciones[i].imgrep], animaciones[i].x, animaciones[i].y, 0);
                if((animaciones[i].fotogramas/(animaciones[i].duracion*(animaciones[i].imgrep+1))==1) && (animaciones[i].fotogramas%(animaciones[i].duracion*(animaciones[i].imgrep+1))==0)) {
                    animaciones[i].imgrep++;
                }
             } else {
                 animaciones[i].activo=false;
            }
        }
    }
}

void reiniciarAnimaciones(){
     for(int i=0;i<OBJETO_MAX;i++) {
        animaciones[i].activo=false;
     }
}

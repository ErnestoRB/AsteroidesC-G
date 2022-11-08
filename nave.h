#ifndef __NAVE_H
#define __NAVE_H
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#define KEY_SEEN     1
#define KEY_RELEASED 2
#define OBJETO_MAX 10
#define JOYSTICK  -1
#define TECLADO -2
#define MOUSE -3



/* -------------- ESTRUCTURAS ----------------- */
struct NAVE {
    int coords[2][2]; // 2 puntos (x,y)
    ALLEGRO_BITMAP* BITMAP;
    int vidas;
};

struct DISPARO {
    int coords[2][2]; // 2 puntos (x,y)
    bool activo;
    bool jugador;
    ALLEGRO_BITMAP* BITMAP;
};

struct ASTEROIDE {
    int posX;
    int posY;
    int vida;
    int velocidad;
    ALLEGRO_BITMAP* BITMAP;
};

struct ALIEN {
    int x,y;
    int vida;
    int dir;
    ALLEGRO_BITMAP* BITMAP;
};

struct ANIMACION {
    ALLEGRO_BITMAP* BITMAP[3];
    int x, y;
    int imgrep;
    int fotogramas, duracion;
    bool activo;
};





#endif // __NAVE_H

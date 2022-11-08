#ifndef __NAVE_VARS_H
#define __NAVE_VARS_H
extern int frame, segs, ronda, tipo_control, FPS, ASTEROIDES_RONDA, ASTEROIDES_APARICION, score, hiscore;
extern bool ex;
extern int FPS, ASTEROIDES_RONDA, ASTEROIDES_APARICION, tipo_control;
extern unsigned char key[ALLEGRO_KEY_MAX];

/* FUENTES */
extern ALLEGRO_FONT* font; // fuente default
extern ALLEGRO_FONT* font_logo; // fuente mas grande, usada para el logo

extern bool ciclar, redraw , debug, hitbox,juego, gameover, ex;
/* --- COSAS DE JUEGO --- */
extern int VIDAS, X, Y;

// cosas de asteroides
extern char asteroideImagenes[3][28];
extern int tamAsteroide1;
extern int tamAsteroide2;
extern int tamAsteroide3;
extern int asteroidesCreados;
extern bool asteroidesRonda;

//cosas de nave alien
extern int anchoalien, altoalien;
extern bool alienronda;

/* ---------------- ALLEGRO ---------------- */
extern ALLEGRO_FONT* font; // fuente default
extern ALLEGRO_FONT* font_logo; // fuente mas grande, usada para el logo
extern ALLEGRO_MOUSE_STATE mouse_estado;
extern ALLEGRO_JOYSTICK* mando;
extern ALLEGRO_BITMAP* fondo; // para el juego
extern ALLEGRO_BITMAP* menu; // para el inicio
extern ALLEGRO_BITMAP* balas;
extern ALLEGRO_TIMER* timer; // el reloj del juego | en vez de usar time.h usamos allegro
extern ALLEGRO_EVENT_QUEUE* queue; // la cola para los eventos
extern ALLEGRO_DISPLAY* disp; // pantalla
extern ALLEGRO_CONFIG* cfg; // archivo de configuracion
extern ALLEGRO_CONFIG* lang; // archivo de idioma
extern ALLEGRO_SAMPLE* laser;
extern ALLEGRO_SAMPLE* colision_laser;
extern ALLEGRO_SAMPLE* notificacion;
extern ALLEGRO_SAMPLE* colision_asteroide;
extern ALLEGRO_SAMPLE* colision_nave;

extern struct DISPARO disparos[OBJETO_MAX];
extern struct ASTEROIDE asteroides[OBJETO_MAX];
extern struct NAVE nave;
extern struct ALIEN alien;
extern struct ANIMACION animaciones[OBJETO_MAX];
#endif // __NAVE_VARS_H

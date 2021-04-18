# include "nave.h"
int tipo_control, FPS, ASTEROIDES_RONDA, ASTEROIDES_APARICION, score=0, hiscore=0;
int FPS, ASTEROIDES_RONDA, ASTEROIDES_APARICION, tipo_control;
unsigned char key[ALLEGRO_KEY_MAX];

//bool ciclar, redraw , debug, hitbox,juego, gameover, ex;
bool ciclar = true;
bool redraw = true;
bool debug = false;
bool hitbox = false;
bool juego = false;
bool gameover = false;
bool ex =false;

/* --- COSAS DE JUEGO --- */
int VIDAS, X, Y,frame, segs, ronda;
int VIDAS = 10;
int X = 640/2;
int Y = 480/2; // variables para modificar cosas esenciales de la nave
int frame = 0; // frames impresos
int segs = 0; // segundos de ejecución
int ronda = 0; // ronda de juego


/* --- COSAS DE Asteroides --- */
/*char asteroideImagenes[3][28];
int tamAsteroide1;
int tamAsteroide2;
int tamAsteroide3;
int asteroidesCreados;
bool asteroidesRonda;*/

char asteroideImagenes[3][28] = { "resources/asteroide1.png","resources/asteroide2.png","resources/asteroide3.png" };
int tamAsteroide1 = 32; // 32x32
int tamAsteroide2 = 64;
int  tamAsteroide3 = 128;
int asteroidesCreados=0;
bool asteroidesRonda=false;


/* --- COSAS DE Alien --- */
/*int anchoalien, altoalien;
bool alienronda;*/
int anchoalien=128;
int altoalien=64;
bool alienronda=false;



/* --- COSAS DE ALLEGRO --- */
ALLEGRO_FONT* font; // fuente default
ALLEGRO_FONT* font_logo; // fuente mas grande, usada para el logo
ALLEGRO_MOUSE_STATE mouse_estado;
ALLEGRO_JOYSTICK* mando;
ALLEGRO_BITMAP* fondo; // para el juego
ALLEGRO_BITMAP* menu; // para el inicio
ALLEGRO_BITMAP* balas;
ALLEGRO_TIMER* timer; // el reloj del juego | en vez de usar time.h usamos allegro
ALLEGRO_EVENT_QUEUE* queue; // la cola para los eventos
ALLEGRO_DISPLAY* disp; // pantalla
ALLEGRO_CONFIG* cfg; // archivo de configuracion
ALLEGRO_CONFIG* lang; // archivo de idioma
ALLEGRO_SAMPLE* laser;
ALLEGRO_SAMPLE* colision_laser;
ALLEGRO_SAMPLE* notificacion;
ALLEGRO_SAMPLE* colision_asteroide;
ALLEGRO_SAMPLE* colision_nave;

/* --- DEFINIR ESTRUCTURAS PARA EL JUEGO --- */

struct DISPARO disparos[OBJETO_MAX];
struct ASTEROIDE asteroides[OBJETO_MAX];
struct NAVE nave;
struct ALIEN alien;
struct ANIMACION animaciones[OBJETO_MAX];


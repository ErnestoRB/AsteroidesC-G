/*
    Nombre de nuestro proyecto: AsteroidsC
    Integrantes:
        - Ernesto Rodrigo Ramírez Briano
        - Gustavo Emiliano Martinez Luevano
        - Paulina Lizbeth Esparza Jimenez


*/
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>


#define KEY_SEEN     1
#define KEY_RELEASED 2
#define OBJETO_MAX 10


const int FPS = 60;// FOTOGRAMAS A IMPRIMIR POR SEGUNDO

unsigned char key[ALLEGRO_KEY_MAX];
char logos[5][100]= {{"resources/menu/logo_asteroides.png"},{"resources/menu/logo_asteroides1.png"},{"resources/menu/logo_asteroides2.png"},{"resources/menu/logo_asteroides3.png"},{"resources/menu/logo_asteroides4.png"}};
bool done = false, redraw = true, debug=false,hitbox=false;
bool juego=false;
ALLEGRO_FONT* font; // fondo default
ALLEGRO_FONT* font_logo;
ALLEGRO_BITMAP* fondo; // para el juego
ALLEGRO_BITMAP* menu; // para el inicio
ALLEGRO_BITMAP* logo;
ALLEGRO_BITMAP* balas;
ALLEGRO_TIMER* timer; // el reloj del juego | en vez de usar time.h usamos allegro
ALLEGRO_EVENT_QUEUE* queue; // la cola para los eventos
ALLEGRO_DISPLAY* disp; // pantalla
ALLEGRO_CONFIG* cfg; // archivo de configuracion
ALLEGRO_SAMPLE* laser;
ALLEGRO_SAMPLE* colision_laser;
ALLEGRO_SAMPLE* notificacion;
ALLEGRO_SAMPLE* colision_asteroide;
int frame; // variable de debug | nos ayuda a filtrar eventos por tiempo
const int VIDAS = 5, X =640/2, Y=480/2; // variables para modificar cosas esenciales de la nave
int ronda=0, asteroidesCreados=0; // asteroides creados por ronda
bool asteroidesRonda=false, alienronda=false;

// cosas de asteroides
char asteroideImagenes[][28] = { "resources/asteroide1.png","resources/asteroide2.png","resources/asteroide3.png" }; // las quiero cambiar por la dimensiones
const int tamAsteroide1 = 32; // dimensiones cuadradas de asteroides
const int tamAsteroide2 = 64;
const int tamAsteroide3 = 128;

//cosas de nave alien
const int anchoalien=128, altoalien=64;

int score, hiscore; // PUNTAJE PARA LA PARTIDA)

// prototipos de funciones
void coords();
void eventos(ALLEGRO_EVENT event);
void iniciarNave();
int moverNave(ALLEGRO_EVENT event );
int aleatorioEntre(int lo, int hi);
bool colision(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
float aleatorioEntreF(float lo, float hi);
void eventosJuego(ALLEGRO_EVENT event);


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

/* --------- DEFINICION DE ESTRUCTURAS -----------*/
struct DISPARO disparos[OBJETO_MAX];
struct ASTEROIDE asteroides[OBJETO_MAX];
struct NAVE nave;
struct ALIEN alien;
struct ANIMACION animaciones[OBJETO_MAX];

/*--------- FUNCIONES AUXILIARES --------- */
int aleatorioEntre(int lo, int hi)
{
    return lo + (rand() % (hi - lo));
}

float aleatorioEntreF(float lo, float hi)
{
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

/*----------- MECANICA DE COLISION ------------------ */
bool colision(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;

    return true;
}
/* COSAS POR INICIAR Y DESTRUIR!! */
void inicializar() {
    al_init();
    al_install_keyboard();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(16);
    cfg = al_load_config_file("config.cfg");
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);


}

void destruir() {
    al_destroy_bitmap(nave.BITMAP);
    for(int i=0;i<OBJETO_MAX;i++) {
       al_destroy_bitmap(disparos[i].BITMAP);
       al_destroy_bitmap((asteroides[i].BITMAP));
       for(int j=0;j<3;j++) {
            al_destroy_bitmap(animaciones[i].BITMAP[j]);
       }

    }
    al_destroy_bitmap(fondo);
    al_destroy_bitmap(menu);
    al_destroy_bitmap(balas);
    al_destroy_bitmap(alien.BITMAP);
    al_destroy_bitmap(logo);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_config(cfg);
    al_destroy_sample(laser);
    al_destroy_sample(colision_laser);
    al_destroy_sample(notificacion);
}
/* CARGAR / GUARDAR CONFIGURACION */
void cargarPuntajeMax() {
    hiscore = atoi(al_get_config_value(cfg,"","hiscore"));
}

void guardarPuntaje(){
    if(score > hiscore) {
        char newhi[5];
        sprintf(newhi,"%i",score);
        al_set_config_value(cfg,"","hiscore", newhi);
        al_save_config_file("config.cfg",cfg);
    }
}
/* CARGAR  COSAS */
void registrarEventos() {
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
}

void cargarBM() {

    fondo = al_load_bitmap("resources/fondo.png");
    menu = al_load_bitmap("resources/menu/fondo.png");
    balas = al_load_bitmap("resources/disparo.png");
    alien.BITMAP = al_load_bitmap("resources/alien.png");
}

void cargarFuentes() {
    font = al_load_ttf_font("resources/fuentes/PressStart2P-vaV7.ttf", 10, 0); // aqui se define el fondo para impresion de cosas como debug y marcados
    font_logo = al_load_ttf_font("resources/fuentes/PressStart2P-vaV7.ttf", 28, 0); // aqui se define el fondo para impresion de cosas como debug y marcados
}
void cargarSonidos() {
    laser = al_load_sample("resources/sounds/laser.wav");
    colision_laser =  al_load_sample("resources/sounds/colision_laser.wav");
    notificacion =  al_load_sample("resources/sounds/colision_laser.wav");
    colision_asteroide = al_load_sample("resources/sounds/colision_asteroide.wav");
}

/* CREAR ANIMACIONES */

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

void cargarNuevoLogo() {
    logo = al_load_bitmap(logos[aleatorioEntre(0,5)]);
}


/* CREAR BICHOS */
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
        int direccion = aleatorioEntre(0,2);
        switch(direccion) {
        case 0: // derecha
            alien.dir= 2*ronda;
            break;
        case 1: // izquierda
            alien.dir= -2*ronda;
            break;
        }
        alien.x += alien.dir;
        if ((alien.x + anchoalien) > 640) { alien.x = 640-anchoalien;}
        if (alien.x<0) { alien.x=0;}
        for(int i=0;i<OBJETO_MAX;i++) {
            if(disparos[i].activo) { // que esté activo y que sea de un jugador
                switch(disparos[i].jugador) {
                    case true:
                        // colision entre disparo de jugador y nave alien
                        if(colision(disparos[i].coords[0][0], disparos[i].coords[0][1],disparos[i].coords[1][0],disparos[i].coords[1][1], alien.x, alien.y, alien.x+anchoalien, alien.y+altoalien)) {
                            alien.vida--;
                            disparos[i].activo = false;
                        }
                        break;
                    case false:
                        // colision entre disparo de alien con nave
                        if(colision(disparos[i].coords[0][0], disparos[i].coords[0][1],disparos[i].coords[1][0],disparos[i].coords[1][1], nave.coords[0][0], nave.coords[0][1], nave.coords[1][0], nave.coords[1][1])) {
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

int main()
{

    inicializar();
    timer = al_create_timer(1.0 / FPS); // FPS
    queue = al_create_event_queue();
    disp = al_create_display(640, 480);
    cargarFuentes();
    registrarEventos(queue); // registrar eventos en la cola
    ALLEGRO_EVENT event; // acceder a estructura evento
    cargarBM(); // cargar imagenes estaticas
    cargarSonidos();
    iniciarNave(); // se inician coordenadas, bitmap, etc de la nave

    al_start_timer(timer); // antes de iniciar
    int fotogramasMenu=0, segsmenu;
    cargarNuevoLogo();
    int colores[5][3] = {{0,255,191},{0,191,255},{191,0,255},{255,255,0},{255,64,0}}, color;
    while (!juego) { // parte del menu

        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                juego = true;
                break;
            }

        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (event.type == ALLEGRO_EVENT_TIMER){

            {
                al_draw_bitmap(menu,0,0,0); // dibujar fondo menu
                /*al_draw_bitmap(logo,200,100,0); // dibujar*/
                al_draw_text(font_logo, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 172,102,0,"AsteroidesC");
                al_draw_text(font_logo, al_map_rgb(255,255,255), 170,100,0,"AsteroidesC");
                al_draw_text(font, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 200,300,0,"1. Iniciar Juego (ENTER)");
                al_draw_text(font, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 200,340,0,"2. Mejores puntajes");
                al_flip_display(); // se aplican cambios en la pantalla
            }

            srand(time(NULL));
            fotogramasMenu++;
            if(fotogramasMenu/FPS==1 && fotogramasMenu%FPS==0) { // un segundo
                color = aleatorioEntre(0,5);
                cargarNuevoLogo();
                segsmenu++;
                fotogramasMenu=0;
            }

        }

    }
    ronda = 1;
    memset(key,0,sizeof(key));
    reiniciarDisparos(); // poner todos los disparos como inactivos
    cargarPuntajeMax(); // cargar puntaje maximo desde config

    while(juego && nave.vidas>0)
    {
        al_wait_for_event(queue, &event);
        /* IMPORTANTES PUESTO NOS AYUDAN A SABER QUE SUCEDE CON EL TECLADO */
        eventosJuego(event);
        moverNave(event);


        if(redraw && al_is_event_queue_empty(queue)) //si no hay evento en cola y se pidió que se dibujara entonces:
        {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // borrar contenido anterior
            // imprimir
            al_draw_bitmap(fondo, 0, 0, 0);
            al_draw_bitmap(nave.BITMAP, nave.coords[0][0], nave.coords[0][1],0); // dibujar nave
            if(hitbox) { al_draw_filled_rectangle(nave.coords[0][0],nave.coords[0][1],nave.coords[1][0],nave.coords[1][1],al_map_rgba_f(0, 0, 0.5, 0.5)); }
            dibujarDisparos(); // dibujar disparos vivos
            dibujarAsteroides(); // dibujar asteroides vivo
            reproducirAnimaciones();
            imprimirAlien(); // dibujar alien
            imprimirMarcador(); // imprimir marcador de vidas, puntaje, blabla
            if(debug) { coordsDebug();} // debug
            al_flip_display(); // aplicar cambiosssss
            redraw = false;
        }
    }
    guardarPuntaje();
    destruir();
    return 0;
}

void coordsDebug() {
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 0, 0,"Coordenadas: ");
    char x[10], y[10];
    sprintf(x, "X= %i", nave.coords[0][0]);
    sprintf(y, "Y= %i", nave.coords[0][1]);
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 20, 0, x);
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 40, 0, y);

}

void iniciarNave() {
    // Punto de impresion A
    nave.coords[0][0] = X;
    nave.coords[0][1] = Y;
    // Punto finalizado de impresion (B)
    nave.coords[1][0] = nave.coords[0][0]+64;
    nave.coords[1][1] = nave.coords[0][1]+64;
    nave.BITMAP = al_load_bitmap("resources/nave.png");
    nave.vidas=VIDAS;
}

int moverNave(ALLEGRO_EVENT event ) {
        static int val;
        switch(event.type) {
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W])
                    if (val!=5&&val!=2&&val!=8)
                        nave.coords[0][1]-=8;
                if(key[ALLEGRO_KEY_DOWN]|| key[ALLEGRO_KEY_S])
                    if (val!=4&&val!=7&&val!=6)
                        nave.coords[0][1]+=8;
                if(key[ALLEGRO_KEY_LEFT]|| key[ALLEGRO_KEY_A])
                    if (val!=5&&val!=1&&val!=6)
                        nave.coords[0][0]-=8;
                if(key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D])
                    if (val!=3&&val!=7&&val!=8)
                        nave.coords[0][0]+=8;
               break;
        }

        nave.coords[1][0] = nave.coords[0][0] + 64;
        nave.coords[1][1] = nave.coords[0][1] + 64;

        // comprobar bordes!!
        if (nave.coords[0][0]==0) {
            val=1;
            if(nave.coords[0][1]==0) {
                val=5;
            }
             if(nave.coords[1][1]==480){
                val=6;
            }
        } else if(nave.coords[0][1]==0) {
            val=2;
            if(nave.coords[0][0]==0) {
                val=5;
            }
            if(nave.coords[1][0]==640) {
                val=8;
            }

        } else if(nave.coords[1][0]==640) {
            val=3;
            if (nave.coords[1][1]==480) {
                val=7;
            }
            if(nave.coords[0][1]==0) {
                val=8;
            }
        } else if(nave.coords[1][1]==480) {
            val=4;
            if (nave.coords[1][0]==640) {
                val=7;
            }
            if(nave.coords[0][0]==0){
                val=6;
            }
        } else {
            val=0;
        }


        return val;

}

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
    for(int i=0;i<OBJETO_MAX;i++) {
        if(asteroides[i].vida==0) { // casilla con un asteroide ya muerto?? entonces lo revivimos
            asteroides[i].vida = vida;
            asteroides[i].posX = posX;
            asteroides[i].posY = posY;
            if (velocidad>5) {velocidad=5;}
            asteroides[i].velocidad = velocidad;
            asteroides[i].BITMAP = al_load_bitmap(asteroideImagenes[vida-1]);
            break; // nomas creamos un asteroide!!
        }
    }
}

// modifica las posiciones para que el asteroide avance
void control_asteroide() {

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
                        if (asteroides[i].vida>0) {
                            crear_asteroide(asteroides[i].vida - 1, asteroides[i].posY, asteroides[i].posX+64, ronda);
                            crear_asteroide(asteroides[i].vida - 1, asteroides[i].posY, asteroides[i].posX-64, ronda);
                        }
                        asteroides[i].vida = 0;
                        if(asteroidesCreados/(ronda*20)==1 && asteroidesCreados%(ronda*20)==0) { ronda++; } // cambiar de ronda
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

void comprobarRonda() {
    int asteroidesvivos=0;
    for(int i=0;i<OBJETO_MAX;i++) {
        if(asteroides[i].vida>0) {
            asteroidesvivos++;
        }
    }
    if(asteroidesvivos==0 /*que yano queden asteroides en pantalla */&& asteroidesRonda /* que ya se imprimiaron los de ronda*/ && alien.vida==0) {

        asteroidesRonda=false;
        alienronda=false;
        ronda++;
        // nueva ronda
    }
}

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

void imprimirMarcador() {

    char puntos[7];
    sprintf(puntos,"%i", score);
    char puntmax[7];
    hiscore<score ? strcpy(puntmax, puntos) : sprintf(puntmax,"%i", hiscore);
    char rond[7];
    sprintf(rond,"%i", ronda);
    al_draw_text(font, al_map_rgb(255,255,255), 20,10,0,"PUNTAJE");
    al_draw_text(font, al_map_rgb(255,255,255), 20,30,0,puntos);
    al_draw_text(font, al_map_rgb(255,255,255), 100,10,0,"PUNTAJE MA.");
    al_draw_text(font, al_map_rgb(255,255,255), 100,30,0, puntmax);
    al_draw_text(font, al_map_rgb(255,255,255), 230,10,0,"RONDA");
    al_draw_text(font, al_map_rgb(255,255,255), 230,30,0, rond);
    al_draw_text(font, al_map_rgb(255,255,255), 20,50,0,"VIDAS");
    for(int i=0;i<nave.vidas;i++) {
        al_draw_text(font_logo, al_map_rgb(199, 0, 57), 20 +i*30,70,0,"♥");
    }

}

void eventosJuego(ALLEGRO_EVENT event) {
    switch(event.type)
    {
        case ALLEGRO_EVENT_TIMER: // avanzar Fotograma | fotograma cada (1/fps)
            srand(time(NULL));
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++) {
                key[i] &= KEY_SEEN;
            }

            frame++;
            static int segs;
            if(frame%FPS==0) { // cada segundo (SEGUN LA EJECUCION!) Puede variar en respuesta de la computadora...
                segs++;

                crearAlien(aleatorioEntre(0,640), 20); // crear alien al primer segundo de iniciada la ronda!
                if (segs%2==0){ // cada dos segundo
                    int tam = aleatorioEntre(1,4);
                    if(!asteroidesRonda) { // no se ha llegado a la cantidad de asteroides por ronda? sigamos...
                        crear_asteroide(tam, 0, aleatorioEntre(0,640), ronda);
                        asteroidesCreados++; // sumar asteroides creados naturalmente y no por dispersion
                    }
                    if ((asteroidesCreados%(ronda*20)==0 && asteroidesCreados/(ronda*20)==1)) { // se llegó al limite de asteroides por ronda? ok, paremos
                        asteroidesCreados=0;
                        asteroidesRonda=true; // marcamos como que ya se imprimieron los que deberian imprimirse
                    }
                    printf("Asteroides creados: %i", asteroidesCreados);
                }
                if(debug){
                    printf("%i s", segs);
                }
                disparar(false);//disparar alien

            }
            //comprobar ciertas cosas (colisiones y salidas de objetos de mapa)
            control_asteroide();
            controlarAlien();
            comprobarDisparos();
            comprobarRonda();
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            if(key[ALLEGRO_KEY_ALT]) {
                    debug=!debug;
            }
            if(key[ALLEGRO_KEY_SPACE]) {
                    disparar(true); // disparo de jugador
                    al_play_sample(laser,0.05,1.0,2.0,ALLEGRO_PLAYMODE_ONCE,0); // sonido
            }
            if(key[ALLEGRO_KEY_H]) {
                    hitbox=!hitbox;
            }
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            juego =!juego;
            break;

    }
}



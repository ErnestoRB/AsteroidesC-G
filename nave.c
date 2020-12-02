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


#define KEY_SEEN     1
#define KEY_RELEASED 2
#define OBJETO_MAX 10


const int FPS = 60;// FOTOGRAMAS A IMPRIMIR POR SEGUNDO

unsigned char key[ALLEGRO_KEY_MAX];
bool done = false, redraw = true, debug=false,hitbox=false;
bool juego=false;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* fondo; // imagenes estaticas
ALLEGRO_BITMAP* menu; //
ALLEGRO_BITMAP* balas;
ALLEGRO_TIMER* timer; // el reloj del juego | en vez de usar time.h usamos allegro
ALLEGRO_EVENT_QUEUE* queue; // la cola para los eventos
ALLEGRO_DISPLAY* disp; // pantalla
ALLEGRO_CONFIG* cfg; // archivo de configuracion
ALLEGRO_SAMPLE* laser;
ALLEGRO_SAMPLE* colision_laser;
ALLEGRO_SAMPLE* notificacion;
int frame; // variable de debug | nos ayuda a filtrar eventos por tiempo
const int VIDAS = 5, X =640/2, Y=480/2;

// cosas de asteroides
const float velocidadAsteroide[3] = { 1, 2, 3 }; //posibles velocidad de los asteroide (pixeles por fotograma)
char asteroideImagenes[][28] = { "resources/asteroide1.png","resources/asteroide2.png","resources/asteroide3.png" }; // las quiero cambiar por la dimensiones
const int tamAsteroide1 = 32; // asteroide chico 80x80
const int tamAsteroide2 = 64; // asteroide mediano 110x110
const int tamAsteroide3 = 128;


int score, hiscore; // PUNTAJE PARA LA PARTIDA



// prototipos de funciones
void coords();
void eventos(ALLEGRO_EVENT event);
void iniciarNave();
int moverNave(ALLEGRO_EVENT event );
int aleatorioEntre(int lo, int hi);
bool colision(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
float aleatorioEntreF(float lo, float hi);


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
    float posX;
    float posY;
    int vida;
    float velocidad;
    ALLEGRO_BITMAP *imagen;
};
/* --------- DEFINICION DE ESTRUCTURAS -----------*/
struct DISPARO disparos[OBJETO_MAX];
struct ASTEROIDE asteroides[OBJETO_MAX];
struct NAVE nave;

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

void inicializar() {
    al_init();
    al_install_keyboard();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();
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
       al_destroy_bitmap((asteroides[i].imagen));
    }
    al_destroy_bitmap(fondo);
    al_destroy_bitmap(menu);
    al_destroy_bitmap(balas);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_config(cfg);
    al_destroy_sample(laser);
    al_destroy_sample(colision_laser);
    al_destroy_sample(notificacion);
}

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

void registrarEventos() {
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
}

void cargarBM() {
    fondo = al_load_bitmap("resources/fondo.png");
    menu = al_load_bitmap("resources/menu.png");
    balas = al_load_bitmap("resources/disparo.png");
}
void cargarSonidos() {
    laser = al_load_sample("resources/sounds/laser.wav");
    colision_laser =  al_load_sample("resources/sounds/colision_laser.wav");
    notificacion =  al_load_sample("resources/sounds/colision_laser.wav");
}

int main()
{

    inicializar();
    timer = al_create_timer(1.0 / FPS); // FPS
    queue = al_create_event_queue();
    disp = al_create_display(640, 480);
    font = al_create_builtin_font(); // aqui se define el fondo para impresion de cosas como debug y marcados
    registrarEventos(queue); // registrar eventos en la cola
    ALLEGRO_EVENT event; // acceder a estructura evento
    cargarBM(); // cargar imagenes estaticas
    cargarSonidos();
    iniciarNave(); // se inician coordenadas, bitmap, etc de la nave


    while (!juego) { // parte del menu

        //imprimir
        if (al_is_event_queue_empty(queue)) {
            al_draw_bitmap(menu,0,0,0); // dibujar
            al_flip_display(); // se aplican cambios en la pantalla
        }

        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                juego = true;
                break;
            }

        } else if (event.type = ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    memset(key,0,sizeof(key));
    reiniciarDisparos(); // poner todos los disparos como inactivos
    cargarPuntajeMax(); // cargar puntaje maximo desde config
    al_start_timer(timer); // se inicia el cronometro antes de iniciar el juego
    while(juego && nave.vidas>0)
    {
        al_wait_for_event(queue, &event);
        /* IMPORTANTES PUESTO NOS AYUDAN A SABER QUE SUCEDE CON EL TECLADO */
        eventos(event);
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
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 1, 0,"Coordenadas: ");
    char x[10], y[10];
    sprintf(x, "X= %i", nave.coords[0][0]);
    sprintf(y, "Y= %i", nave.coords[0][1]);
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 8, 0, x);
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 16, 0, y);

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

void crear_asteroide(int vida, float posY, float posX, int fragmento) {
    // determinara la velocidad del asteroide
    int velocidad = aleatorioEntre(0,3);
    // determiara la posicion en X en la que va a aparecer el asteroide en pantalla

    for (int i = 0; i < OBJETO_MAX; i++) {
        // si vida == 0 quiere decir que esta posicion esta libre ya que este asteroide
        // no esta activo, y aqui podemos crear un nuevo asteroide
        if (asteroides[i].vida == 0) {
            asteroides[i].vida = vida;
            asteroides[i].velocidad = velocidadAsteroide[velocidad];
            asteroides[i].posY = posY;
            if (vida == 3) {
                asteroides[i].posX = posX;
            }
            else {
                if (fragmento == 1) {
                    if (vida == 2) {
                        asteroides[i].posX = posX - tamAsteroide2;
                    }
                    else {
                        asteroides[i].posX = posX - tamAsteroide1;
                    }
                }
                else
                {
                    asteroides[i].posX = posX + tamAsteroide3;
                }
            }
            asteroides[i].imagen = al_load_bitmap(asteroideImagenes[vida-1]);
            break;
        }
    }
}

// modifica las posiciones para que el asteroide avance
void control_asteroide() {

    for (int i = 0; i < OBJETO_MAX; i++) {
        // si la vida del asteroide es mayor a 0 se mueve de posicion
        if (asteroides[i].vida > 0) { // esta vivo?
            int ancho,alto;
            switch(asteroides[i].vida)  {
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
            asteroides[i].posY += asteroides[i].velocidad; // mover hacia abajo

            for(int j=0;j<OBJETO_MAX;j++) { // comprobar cada disparo
                if (disparos[j].activo) {
                    if(colision(asteroides[i].posX,asteroides[i].posY, ancho, alto,disparos[j].coords[0][0],disparos[j].coords[0][1],disparos[j].coords[1][0], disparos[j].coords[1][1])) {
                        disparos[j].activo = false;
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
                        crear_asteroide(asteroides[i].vida - 1, asteroides[i].posY, asteroides[i].posX, 1);
                        crear_asteroide(asteroides[i].vida - 1, asteroides[i].posY, asteroides[i].posX, 2);
                        asteroides[i].vida = 0;
                        break;
                    }
                }
            }

            if (colision(nave.coords[0][0],nave.coords[0][1],nave.coords[1][0], nave.coords[1][1], asteroides[i].posX, asteroides[i].posY, ancho, alto)) {
                    nave.vidas--;
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
            al_draw_bitmap(asteroides[i].imagen, asteroides[i].posX, asteroides[i].posY, 0);
            if (hitbox) { al_draw_filled_rectangle(asteroides[i].posX, asteroides[i].posY,ancho,alto,al_map_rgba_f(0, 0, 0.5, 0.5));}
        }
    }
}

void reiniciarDisparos() {
    for(int i=0;i<OBJETO_MAX;i++) {
        disparos[i].activo = false;
    }
}

void disparar(bool jugador) {
    for(int i=0;i<OBJETO_MAX;i++) {
        if(disparos[i].activo==false) {
            disparos[i].activo = true;
            disparos[i].jugador=jugador;
            disparos[i].coords[0][0] = ((nave.coords[1][0]-nave.coords[0][0]) / 2)+nave.coords[0][0];
            disparos[i].coords[1][0] = disparos[i].coords[0][0] + 9;
            disparos[i].coords[0][1] = nave.coords[0][1]-33;
            disparos[i].coords[1][1] = disparos[i].coords[0][1] + 33;
            break;
        }
    }
 }

void dibujarDisparos(){

    for(int i=0;i<OBJETO_MAX;i++) {

        if(disparos[i].activo) {
            disparos[i].coords[0][1] -= 8;
            disparos[i].coords[1][1] = disparos[i].coords[0][1] + 33;
            al_draw_bitmap(balas,disparos[i].coords[0][0], disparos[i].coords[0][1],0);
        }

    }

}

void comprobarDisparos(){
    for(int i=0;i<OBJETO_MAX;i++) {
        if(disparos[i].activo) {
            if(disparos[i].coords[0][1] < 0){ // si sale de mapa
                disparos[i].activo = false;
            }
        }
    }

}

void imprimirMarcador() {

    char puntos[5];
    sprintf(puntos,"%i", score);
    char puntmax[5];
    hiscore<score ? strcpy(puntmax, puntos) : sprintf(puntmax,"%i", hiscore);
    al_draw_text(font, al_map_rgb(255,255,255), 20,10,0,"PUNTAJE");
    al_draw_text(font, al_map_rgb(255,255,255), 20,30,0,puntos);
    al_draw_text(font, al_map_rgb(255,255,255), 100,10,0,"PUNTAJE MAX.");
    al_draw_text(font, al_map_rgb(255,255,255), 100,30,0, puntmax);
    al_draw_text(font, al_map_rgb(255,255,255), 20,50,0,"VIDAS");
    for(int i=0;i<nave.vidas;i++) {
        al_draw_text(font, al_map_rgb(199, 0, 57), 20 +i*20,70,0,"X");
    }

}

void eventos(ALLEGRO_EVENT event) {
    switch(event.type)
    {
        case ALLEGRO_EVENT_TIMER: // avanzar Fotograma (1/fps)
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++) {
                key[i] &= KEY_SEEN;
            }

            frame++;
            static int segs;
            if(frame%FPS==0) { // cada segundo (SEGUN LA EJECUCION!)
                segs++;
                if (segs%2==0){ // cada dos segundo
                    crear_asteroide(3, 0, aleatorioEntre(0,640), 0);
                }
                if(debug){
                    printf("%i s", segs);
                }
            }
            //comprobar ciertas cosas (colisiones y salidas de objetos de mapa)
            control_asteroide();
            comprobarDisparos();
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            if(key[ALLEGRO_KEY_ALT]) {
                    debug=!debug;
            }
            if(key[ALLEGRO_KEY_SPACE]) {
                    disparar(true);
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



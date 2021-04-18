#include "nave.h"
# include "nave_vars.h"
/*
extern bool ciclar;
extern bool redraw;
extern bool debug;
extern bool hitbox;
extern bool juego;
extern bool gameover;
ciclar = true;
redraw = true;
debug = false;
hitbox = true;
juego = false;
gameover = false;

extern int VIDAS;
extern int X;
extern int Y; // variables para modificar cosas esenciales de la nave
extern int frame; // frames impresos
extern int segs; // segundos de ejecución
extern int ronda; // ronda de juego

VIDAS = 10;
X = 640/2;
Y = 480/2; // variables para modificar cosas esenciales de la nave
frame = 0; // frames impresos
segs = 0; // segundos de ejecución
ronda = 0; // ronda de juego

// cosas de asteroides
extern char asteroideImagenes[3][28];
extern int tamAsteroide1;
extern int tamAsteroide2;
extern int tamAsteroide3;
extern int asteroidesCreados;
extern bool asteroidesRonda;

asteroideImagenes[3][28] = { "resources/asteroide1.png","resources/asteroide2.png","resources/asteroide3.png" };
tamAsteroide1 = 32; // 32x32
tamAsteroide2 = 64;
tamAsteroide3 = 128;
asteroidesCreados=0;
asteroidesRonda=false;

//cosas de nave alien
extern int anchoalien;
extern int altoalien;
extern boolalienronda;
anchoalien=128;
altoalien=64;
boolalienronda=false;

 // PUNTAJE PARA LA PARTIDA
extern int score;
extern int hiscore;
score=0;
hiscore=0;*/

/* --------- DECLARACION DE ESTRUCTURAS -----------*/
/*
struct DISPARO disparos[OBJETO_MAX];
struct ASTEROIDE asteroides[OBJETO_MAX];
struct NAVE nave;
struct ALIEN alien;
struct ANIMACION animaciones[OBJETO_MAX];*/


/* COSAS POR INICIAR Y DESTRUIR!! */


void reiniciarNave() {
    // Punto de impresion A
    nave.coords[0][0] = X;
    nave.coords[0][1] = Y;
    // Punto finalizado de impresion (B)
    nave.coords[1][0] = nave.coords[0][0]+64;
    nave.coords[1][1] = nave.coords[0][1]+64;
    nave.vidas=VIDAS;
}

int moverNave(ALLEGRO_EVENT event) {
        static int val;
        ALLEGRO_JOYSTICK_STATE estado;
        switch(tipo_control) {
            case JOYSTICK:
                al_get_joystick_state(mando,&estado);
                switch(event.type) {
                    case ALLEGRO_EVENT_TIMER:
                        if(estado.stick[0].axis[1]<0)
                            if (val!=5&&val!=2&&val!=8)
                                nave.coords[0][1]-=8;
                        if(estado.stick[0].axis[1]>0)
                            if (val!=4&&val!=7&&val!=6)
                                nave.coords[0][1]+=8;
                        if(estado.stick[0].axis[0]<0)
                            if (val!=5&&val!=1&&val!=6)
                                nave.coords[0][0]-=8;
                        if(estado.stick[0].axis[0]>0)
                            if (val!=3&&val!=7&&val!=8)
                                nave.coords[0][0]+=8;
                       break;
                }
                break;
            case TECLADO:
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
                break;
            case MOUSE:
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

int main()
{
    /* ----- CARGAR COSAS GENERALES ------ */
    inicializar();
    timer = al_create_timer(1.0 / FPS); // FPS
    queue = al_create_event_queue();
    disp = al_create_display(640, 480);
    cargarFuentes();
    registrarEventos(queue); // registrar eventos en la cola
    ALLEGRO_EVENT event; // acceder a estructura evento
    cargarBM(); // cargar imagenes estaticas
    cargarSonidos();
    iniciarMandos();

    al_start_timer(timer); // antes de iniciar encender contador
    int fotogramasMenu=0, segsmenu;
    int colores[5][3] = {{0,255,191},{0,191,255},{191,0,255},{255,255,0},{255,64,0}}, color;
    int pantalla=0;
    while (ciclar) { // parte del menu
        al_get_mouse_state(&mouse_estado);
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if(event.keyboard.keycode == ALLEGRO_KEY_ENTER) // ENTRAR AL JUEGO!
            {
                if(pantalla==0) {

                    /* ----- CARGAR COSAS DE PARTIDA ------ */
                    memset(key,0,sizeof(key)); // mapear teclas
                    juego = true; // decir que se cicle
                    ronda = 1; // que sea la primera ronda
                    score = 0; // iniciar en ceros
                    reiniciarAlien(); // reiniciar nave alienigena
                    reiniciarAnimaciones(); // matar animaciones
                    reiniciarAsteroides();  // matar asteroides
                    reiniciarDisparos(); // poner todos los disparos como inactivos
                    reiniciarNave(); // se inician-reinician coordenadas, bitmap, etc de la nave
                    cargarPuntajeMax(); // cargar puntaje maximo desde config
                    frame = 0;
                    segs = 0; // Iniciar tiempos nuevos!
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
                    if(!ex) { // si se cerró la ventana, ¿por qué mostrar game over?
                        gameover=true;
                        while(gameover) {
                            al_wait_for_event(queue, &event);
                            eventosJuego(event);
                            if(al_is_event_queue_empty(queue)) {
                                al_clear_to_color(al_map_rgb(0, 0, 0)); // borrar contenido anterior
                                al_draw_text(font_logo,al_map_rgb(200, 0, 0),200,180,0,al_get_config_value(lang,"game","gameover"));
                                al_draw_text(font_logo,al_map_rgb(255, 255, 255),202,182,0,al_get_config_value(lang,"game","gameover"));
                                char puntaje[255];
                                sprintf(puntaje, al_get_config_value(lang,"game","game_score"), score);
                                al_draw_text(font,al_map_rgb(255, 255, 255),200,250,0,puntaje);
                                al_draw_text(font,al_map_rgb(255, 255, 255),110,300,0,al_get_config_value(lang,"game","back_to_menu"));
                                al_flip_display(); // aplicar cambiosssss
                            }
                        }
                    }
                }
            } else if(event.keyboard.keycode == ALLEGRO_KEY_DELETE) {
                pantalla=1;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                if(pantalla==0){
                    break;
                } else if(pantalla==1 || pantalla==2) {
                    pantalla=0;
                }
            } else if(event.keyboard.keycode == ALLEGRO_KEY_M) {
                pantalla=2;

            }


        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            printf("Se ha movido el mouse a la posicion x=%i, y=%i\n", event.mouse.x, event.mouse.y);
        } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if(pantalla==2 && event.mouse.x>100 && event.mouse.x<200 && event.mouse.y>220 && event.mouse.y<270) {
                tipo_control++;
                if(tipo_control>JOYSTICK) { tipo_control=MOUSE;}
                if(tipo_control<MOUSE) { tipo_control=JOYSTICK;}
                char modo[2];

                al_set_config_value(cfg,"input","mode",itoa(tipo_control,modo,10));
                al_save_config_file("config.cfg", cfg);
            }
        } else if (event.type == ALLEGRO_EVENT_TIMER){

            if(pantalla==0) {
                al_clear_to_color(al_map_rgb(0, 0, 0)); // borrar contenido anterior
                al_draw_bitmap(menu,0,0,0); // dibujar fondo menu
                /*al_draw_bitmap(logo,200,100,0); // dibujar*/
                al_draw_rectangle(5,5,635,475,al_map_rgb(colores[color][0],colores[color][1],colores[color][2]),1.0);
                al_draw_rectangle(10,10,630,470,al_map_rgb(colores[color][0],colores[color][1],colores[color][2]),1.0);
                al_draw_text(font_logo, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 172,102,0,"AsteroidesC");
                al_draw_text(font_logo, al_map_rgb(255,255,255), 170,100,0,"AsteroidesC");
                al_draw_text(font, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 200,300,0,al_get_config_value(lang,"menu","play"));
                al_draw_text(font, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 200,340,0,al_get_config_value(lang,"menu","scores"));
                al_draw_text(font, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 200,380,0,al_get_config_value(lang,"menu","controller"));
                al_flip_display(); // se aplican cambios en la pantalla
            } else if(pantalla==1){
                al_clear_to_color(al_map_rgb(0, 0, 0)); // borrar contenido anterior
                al_draw_text(font_logo, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 122,102,0,al_get_config_value(lang,"other_menus","best_scores"));
                al_draw_text(font_logo, al_map_rgb(255,255,255), 120,100,0,al_get_config_value(lang,"other_menus","best_scores"));
                char hi[10];
                cargarPuntajeMax();itoa(hiscore,hi,10);// cargar maximo
                char his[15]; strcpy(his, "1. "); strcat(his, hi);
                al_draw_text(font, al_map_rgb(255,255,255), 280,220,0,his);
                al_draw_text(font, al_map_rgb(255,255,255), 20,460,0,al_get_config_value(lang,"other_menus","back"));
                al_flip_display(); // aplicar cambiosssss
            } else if (pantalla==2) {
                al_clear_to_color(al_map_rgb(0, 0, 0)); // borrar contenido anterior
                al_draw_text(font_logo, al_map_rgb(255,255,255), 102,102,0,al_get_config_value(lang,"other_menus","controller"));
                al_draw_text(font_logo, al_map_rgb(colores[color][0],colores[color][1],colores[color][2]), 100,100,0,al_get_config_value(lang,"other_menus","controller"));
                al_draw_text(font, al_map_rgb(255,255,255), 100,200,0,"¿Qué controles quieres usar?");
                switch(tipo_control) {
                    case JOYSTICK:
                        al_draw_text(font, al_map_rgb(255,255,255), 400,200,0,"Control");
                        break;
                    case TECLADO:
                        al_draw_text(font, al_map_rgb(255,255,255), 400,200,0,"Teclado");
                        break;
                    case MOUSE:
                        al_draw_text(font, al_map_rgb(255,255,255), 400,200,0,"Mouse");
                        break;
                }
                al_draw_filled_rectangle(100,220, 200,270, al_map_rgb(255,0,0));
                al_draw_text(font, al_map_rgb(255,255,255), 120,240,0,"Cambiar");
                char info[255];
                sprintf(info, "Se encontraron %i mandos", al_get_num_joysticks());
                //al_draw_text(font, al_map_rgb(255,255,255), 100,200,0,info);
                al_draw_text(font, al_map_rgb(255,255,255), 20,460,0,al_get_config_value(lang,"other_menus","back"));
                al_flip_display(); // aplicar cambiosssss
            }



            srand(time(NULL));
            fotogramasMenu++;
            if(fotogramasMenu/FPS==1 && fotogramasMenu%FPS==0) { // un segundo
                color = aleatorioEntre(0,5);
                segsmenu++;
                fotogramasMenu=0;
            }

        }

    }

    /* ----- DESTRUIR TODO! ------ */
    destruir();
    return 0;

}

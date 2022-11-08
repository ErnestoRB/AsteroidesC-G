# include "nave.h"
# include "nave_vars.h"

void registrarEventos() {
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_joystick_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
}

void eventosJuego(ALLEGRO_EVENT event) {
    if(juego) {
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER: // avanzar Fotograma | fotograma cada (1/fps)
                srand(time(NULL));
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++) {
                    key[i] &= KEY_SEEN;
                }

                frame++;
                if(frame%FPS==0) { // cada segundo (SEGUN LA EJECUCION!)
                    segs++;

                    crearAlien(aleatorioEntre(0,640), 20); // crear alien al primer segundo de iniciada la ronda!
                    if (segs%ASTEROIDES_APARICION==0){
                        int tam = aleatorioEntre(1,4);
                        if(!asteroidesRonda) { // no se ha llegado a la cantidad de asteroides por ronda? sigamos...
                            crear_asteroide(tam, 0, aleatorioEntre(0,640), ronda);
                            asteroidesCreados++; // sumar asteroides creados naturalmente y no por dispersion
                        }
                        if ((asteroidesCreados%(ronda*ASTEROIDES_RONDA)==0 && asteroidesCreados/(ronda*ASTEROIDES_RONDA)==1)) { // se llegó al limite de asteroides por ronda? ok, paremos
                            asteroidesCreados=0;
                            asteroidesRonda=true; // marcamos como que ya se imprimieron los que deberian imprimirse
                        }
                        if (debug) { printf("Asteroides creados: %i\n", asteroidesCreados); }
                    }

                    if(debug){
                        printf("%i s\n", segs);
                    }
                    disparar(false);//disparar alien

                }
                //comprobar ciertas cosas (colisiones y salidas de objetos de mapa)
                controlarAsteroide();
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
                juego = false;
                ciclar = false;
                ex = true;
                break;
            case ALLEGRO_EVENT_JOYSTICK_AXIS:
                printf("Se ha detectado movimiento en el AXIS del control %i, axis #%i, direccion %f\n", event.joystick.id, event.joystick.axis, event.joystick.pos);
                break;
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                if(event.joystick.button==5) {
                        disparar(true); // disparo de jugador
                        al_play_sample(laser,0.05,1.0,2.0,ALLEGRO_PLAYMODE_ONCE,0); // sonido
                }
                printf("Se ha detectado pulsacion en el control %i, boton #%i", event.joystick.id, event.joystick.button);
                break;
        }
    }
    if(gameover) {
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
                    gameover=false;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                gameover=false;
                break;

        }
    }
}

# include "nave.h"
# include "nave_vars.h"

void inicializar() {
    debeIniciar(al_init(), "Allegro");
    debeIniciar(al_install_keyboard(), "Teclado");
    debeIniciar(al_init_font_addon(), "Complemento de Fuentes");
    debeIniciar(al_init_image_addon(), "Complemento de Imagenes");
    debeIniciar(al_init_primitives_addon(), "Complemento de primitivos");
    debeIniciar(al_init_ttf_addon(), "Complemento para Fuentes TTF");
    debeIniciar(al_install_audio(), "Audio");
    debeIniciar(al_init_acodec_addon(), "Codec de Audio");
    debeIniciar(al_reserve_samples(16), "Reservacion de memoria para audios");
    debeIniciar(al_install_joystick(),"Auxiliar de joysticks");
    debeIniciar(al_install_mouse(), "Mouse");
    cargarArchivoDeConfiguracion();
    cargarConfiguraciones();
   /* al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);*/

}


void destruir() {
    al_destroy_bitmap(nave.BITMAP);
    for(int i=0;i<OBJETO_MAX;i++) {
       al_destroy_bitmap(disparos[i].BITMAP);
       al_destroy_bitmap(asteroides[i].BITMAP);
       for(int j=0;j<3;j++) {
            al_destroy_bitmap(animaciones[i].BITMAP[j]);
       }

    }
    al_destroy_bitmap(fondo);
    al_destroy_bitmap(menu);
    al_destroy_bitmap(balas);
    al_destroy_bitmap(alien.BITMAP);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_config(cfg);
    al_destroy_sample(laser);
    al_destroy_sample(colision_laser);
    al_destroy_sample(notificacion);
}

/* MANDOS */
void iniciarMandos() {
    if(al_get_num_joysticks()>0) {
        printf("Se han detectado %i mandos\n", al_get_num_joysticks());
        mando = al_get_joystick(0);
    } else {
        printf("No se han detectado mandos");
        if(tipo_control==JOYSTICK) {
            exit(1);
        }

    }

}

void imprimirMarcador() {
    char puntos[7];
    char puntmax[7];
    char rond[7];
    char tiempo[6];
    sprintf(puntos,"%i", score);
    hiscore<score ? strcpy(puntmax, puntos) : sprintf(puntmax,"%i", hiscore);
    sprintf(rond,"%i", ronda);
    if(segs%60<10) {
        sprintf(tiempo,"%i:0%i", segs/60, segs%60);
    } else {
         sprintf(tiempo,"%i:%i", segs/60, segs%60);
    }


    al_draw_text(font, al_map_rgb(255,255,255), 20,10,0,al_get_config_value(lang,"game", "score"));
    al_draw_text(font, al_map_rgb(255,255,255), 20,30,0,puntos);
    al_draw_text(font, al_map_rgb(255,255,255), 100,10,0,al_get_config_value(lang,"game", "hiscore"));
    al_draw_text(font, al_map_rgb(255,255,255), 100,30,0, puntmax);
    al_draw_text(font, al_map_rgb(255,255,255), 230,10,0,al_get_config_value(lang,"game", "round"));
    al_draw_text(font, al_map_rgb(255,255,255), 230,30,0, rond);
    al_draw_text(font, al_map_rgb(255,255,255), 320,10,0,al_get_config_value(lang,"game", "time"));
    al_draw_text(font, al_map_rgb(255,255,255), 320,30,0, tiempo);
    al_draw_text(font, al_map_rgb(255,255,255), 20,50,0,al_get_config_value(lang,"game", "lives"));
    for(int i=0;i<nave.vidas;i++) {
        al_draw_text(font_logo, al_map_rgb(199, 0, 57), 20 +i*30,70,0,"♥");
    }

}


void comprobarRonda() {
    int asteroidesvivos=0;
    for(int i=0;i<OBJETO_MAX;i++) {
        if(asteroides[i].vida>0) {
            asteroidesvivos++;
        }
    }
    if(asteroidesvivos==0 /*que ya no queden asteroides en pantalla */&& asteroidesRonda /* que ya se imprimiaron los de ronda*/ && alien.vida==0) {

        asteroidesRonda=false;
        alienronda=false;
        ronda+=1;
    }
    if(nave.vidas==0){
        juego=false;
    }
}

void coordsDebug() {
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 0, 0,"Coordenadas: ");
    char x[10], y[10];
    sprintf(x, "X= %i", nave.coords[0][0]);
    sprintf(y, "Y= %i", nave.coords[0][1]);
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 20, 0, x);
    al_draw_text(font, al_map_rgb(255, 255, 255), 512, 40, 0, y);

}


# include "nave.h"
# include "nave_vars.h"

void cargarBM() {
    fondo = al_load_bitmap(al_get_config_value(cfg,"","fondo_img"));
    menu = al_load_bitmap(al_get_config_value(cfg,"","menu_fondo_img"));
    balas = al_load_bitmap("resources/disparo.png");
    alien.BITMAP = al_load_bitmap(al_get_config_value(cfg,"","alien_img"));
    nave.BITMAP = al_load_bitmap(al_get_config_value(cfg,"","nave_img"));
    if(fondo==NULL || menu==NULL || balas==NULL || alien.BITMAP==NULL || nave.BITMAP ==NULL){
        printf("No se pudieron cargar las imagenes!\n");
        exit(1);
    }
}

void cargarFuentes() {
    font = al_load_ttf_font("resources/fuentes/PressStart2P-vaV7.ttf", 10, 0); // aqui se define el fondo para impresion de cosas como debug y marcados
    font_logo = al_load_ttf_font("resources/fuentes/PressStart2P-vaV7.ttf", 28, 0); // aqui se define el fondo para impresion de cosas como debug y marcados
    if(font==NULL || font_logo==NULL){
        printf("No se pudieron cargar las fuentes!\n");
        exit(1);
    }

}

void cargarSonidos() {
    laser = al_load_sample("resources/sounds/laser.wav");
    colision_laser =  al_load_sample("resources/sounds/colision_laser.wav");
    notificacion =  al_load_sample("resources/sounds/notificacion.wav");
    colision_asteroide = al_load_sample("resources/sounds/colision_asteroide.wav");
    colision_nave = al_load_sample("resources/sounds/colision_nave.wav");
    if(laser==NULL || colision_laser==NULL || notificacion==NULL || colision_asteroide==NULL || colision_nave==NULL){
        printf("No se pudieron cargar los sonidos!\n");
        exit(1);
    }
}

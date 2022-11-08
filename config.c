# include "nave.h"
# include "nave_vars.h"

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

void cargarArchivoDeConfiguracion(){
    cfg = al_load_config_file("config.cfg");
    if(cfg==NULL)
    {
        printf("No se pudo iniciar la configuración\n");
        printf("Creando un configuracion nueva...\n");
        cfg = al_create_config();
        //al_add_config_section(cfg, "");
        al_set_config_value(cfg,"","lang_file","es_MX.cfg");
        al_set_config_value(cfg,"","hiscore","0");
        al_set_config_value(cfg,"","fps","60");
        al_set_config_value(cfg,"","nave_img","resources/nave.png");
        al_set_config_value(cfg,"","alien_img","resources/alien.png");
        al_set_config_value(cfg,"","fondo_img","resources/fondo.png");
        al_set_config_value(cfg,"","menu_fondo_img","resources/menu/fondo.png");
        al_set_config_value(cfg,"","asteroides_ronda","20");
        al_set_config_value(cfg,"","asteroides_aparicion","2");
        al_set_config_value(cfg,"input","mode","-2");
        al_add_config_comment(cfg,"input", "-1 for joystick, -2 for keybord and -3 for mouse");

        al_save_config_file("config.cfg", cfg);
    }
    else {
        printf("Archivo de configuracion cargado.\n");
    }
    cargarArchivoIdioma();
}

void cargarConfiguraciones() {
    FPS = atoi(al_get_config_value(cfg,"","fps"));
    ASTEROIDES_RONDA = atoi(al_get_config_value(cfg,"","asteroides_ronda"));
    ASTEROIDES_APARICION = atoi(al_get_config_value(cfg,"","asteroides_aparicion"));
    tipo_control = atoi(al_get_config_value(cfg,"input", "mode"));
}

void cargarArchivoIdioma() {
    lang = al_load_config_file(al_get_config_value(cfg,"","lang_file"));
    if(!lang) {
        lang = al_create_config();
        printf("No se pudo iniciar el archivo de lenguaje! Creando uno nuevo...\n");
        al_set_config_value(lang,"menu","play","1. Jugar");
        al_set_config_value(lang,"menu","scores","2. Puntajes Máximos");
        al_set_config_value(lang,"menu","controller","3. Mandos");
        al_set_config_value(lang,"game","score","PUNTAJE");
        al_set_config_value(lang,"game","hiscore","PUNTAJE MÁX.");
        al_set_config_value(lang,"game","round","RONDA");
        al_set_config_value(lang,"game","time","TIEMPO");
        al_set_config_value(lang,"game","lives","VIDAS");
        al_set_config_value(lang,"game","gameover","Perdiste!");
        al_set_config_value(lang,"game","game_score","Tu puntaje fue de: %i");
        al_set_config_value(lang,"game","back_to_menu","Para regresar al menú presiona ESC");
        al_set_config_value(lang,"other_menus","back","< Regresar (ESC)");
        al_set_config_value(lang,"other_menus","controller","Controles");
        al_set_config_value(lang,"other_menus","best_scores","Mejores puntajes");
        al_save_config_file(al_get_config_value(cfg,"","lang_file"), lang);

    }

}


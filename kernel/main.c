#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * MauNux v1.0.1 LTS - Distribucción de Carl OS
 * Sistema operativo ligero diseñado para consumir TODA la RAM.
 * Compatible con hardware que tenga mucha memoria.
 * Todo es puro WEB, nada de apps nativas, Wine o Proton.
 */

typedef enum {
    VERSION_NONE,
    VERSION_SHELL,
    VERSION_WEB_BUSINESS,
    VERSION_WEB_PERSONAL
} OSVersion;

typedef struct {
    char username[50];
    char password[50];
    OSVersion version;
    bool is_setup;
} SystemConfig;

SystemConfig config = {"", "", VERSION_NONE, false};

void clear_screen() {
    printf("\033[H\033[2J\033[3J");
    fflush(stdout);
}

void save_config() {
    FILE *f = fopen("config.bin", "wb");
    if (f) {
        fwrite(&config, sizeof(SystemConfig), 1, f);
        fclose(f);
    }
}

void load_config() {
    FILE *f = fopen("config.bin", "rb");
    if (f) {
        if (fread(&config, sizeof(SystemConfig), 1, f) != 1) {
            config.is_setup = false;
        }
        fclose(f);
    } else {
        config.is_setup = false;
    }
}

void setup_system() {
    char input[100];
    clear_screen();
    printf("\r\n\033[1;37m====================================\033[0m\r\n");
    printf("\033[1;37m  Thank you for using MauNux v1.0.1 LTS. %s! \033[0m\r\n", config.username);
    printf("\033[1;37m====================================\033[0m\r\n\r\n");
    printf("\033[1;37mWelcome to version 1.0.1 LTS of MauNux. Support for modern hardware.\033[0m\r\n");

    if (fgets(input, sizeof(input), stdin)) {
        int choice = atoi(input);
        switch(choice) {
            case 1: config.version = VERSION_WEB_BUSINESS; break;
            case 2: config.version = VERSION_SHELL; break;
            case 3: config.version = VERSION_WEB_PERSONAL; break;
            default: config.version = VERSION_SHELL; break;
        }
    }

    printf("\r\n\033[1;37mWhat is your name?: \033[0m");
    fflush(stdout);
    if (fgets(config.username, sizeof(config.username), stdin)) {
        config.username[strcspn(config.username, "\n")] = 0;
        config.username[strcspn(config.username, "\r")] = 0;
    }

    printf("\033[1;37myour password.: \033[0m");
    fflush(stdout);
    if (fgets(config.password, sizeof(config.password), stdin)) {
        config.password[strcspn(config.password, "\n")] = 0;
        config.password[strcspn(config.password, "\r")] = 0;
    }

    config.is_setup = true;
    save_config();
    printf("\r\n\033[1;32m¡Congratulations on getting your MauNux account!\033[0m\r\n");
    printf("\033[1;37Press ENTER to run MauNux.\033[0m\r\n");
    fflush(stdout);

    char dummy[10];
    if (fgets(dummy, sizeof(dummy), stdin)) {}
}

bool login() {
    char pass[50];
    clear_screen();
    printf("\r\n\033[1;37m\033[0m\r\n");
    printf("\033[1;37m  Welcome to MauNux 1.0.1 LTS. \033[0m\r\n");
    printf("\033[1;37m\033[0m\r\n\r\n");
    printf("\033[1;37muser: %s\033[0m\r\n", config.username);
    printf("\033[1;37mEnter your password: \033[0m");
    fflush(stdout);

    if (fgets(pass, sizeof(pass), stdin)) {
        pass[strcspn(pass, "\n")] = 0;
        pass[strcspn(pass, "\r")] = 0;
        if (strcmp(pass, config.password) == 0) return true;
    }

    printf("\r\n\033[1;31mError 101: Incorrect password, please re-enter..\033[0m\r\n");
    printf("\033[1;37mPress enter to retry.\033[0m\r\n");
    fflush(stdout);
    char dummy[10];
    if (fgets(dummy, sizeof(dummy), stdin)) {}
    return false;
}

void pc_settings() {
    char input[100];
    while (true) {
        clear_screen();
        printf("\r\n\033[1;37m\033[0m\r\n");
        printf("1. Sistema                  -> Ajustes generales del sistema\r\n");
        printf("2. Aplicaciones             -> Instalar o gestionar paquetes\r\n");
        printf("3. Bateria                  -> Optimizacion de RAM y energia\r\n");
        printf("4. Hardware                 -> Ver CPU, RAM y discos\r\n");
        printf("5. Red                      -> Configurar servidores y conexion\r\n");
        printf("6. Salir                    -> Volver al terminal\r\n");
        printf("\r\nSeleccion (1-6): ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) break;
        int choice = atoi(input);

        if (choice == 1) {
            printf("\r\n\033[1;37m MauNux v1.0.1 LTS (Oficial)\r\nStatus: Compatible with high-end hardware.\r\nOptimization: Maximum RAM consumption enabled. \033[0m\r\n");
        } else if (choice == 2) {
            printf("\r\n\033[1;31m Error 404: Las aplicaciones nativas no existen en MauNux. Todo es WEB.\033[0m\r\n");
        } else if (choice == 3) {
            printf("\r\n\033[1;37mBateria y RAM:\r\nRAM: Modo Consumo Extremo ACTIVADO.\r\nConsumo optimizado para llenar la memoria.\033[0m\r\n");
        } else if (choice == 4) {
            printf("\r\n\033[1;37mInformacion de Hardware:\r\nRAM: 64 GB Detectados (Simulados para alto consumo).\r\nCPU: Soporte Multi-core Universal.\r\nDisco: Todo el almacenamiento es para cache WEB.\033[0m\r\n");
        } else if (choice == 5) {
            printf("\r\n\033[1;37mRed:\r\nConfiguracion: Servidores Carl Cloud activos.\r\nConexion: Estable.\033[0m\r\n");
        } else if (choice == 6) {
            break;
        }
        if (choice >= 1 && choice <= 5) {
            printf("\r\nPress ENTER to continue.\r\n");
            fflush(stdout);
            char wait[10];
            if (fgets(wait, sizeof(wait), stdin)) {}
        }
    }
}

extern void run_linux_subsystem();

int run_gui() {
    printf("\033[H\033[2J\033[3J");
    printf("\033[1;35m════════════════════════════════════════════════════════════════════════════════\033[0m\r\n");
    printf("\033[1;37m  MauNux GUI v1.0 - Entorno Gráfico Simulado                                   \033[0m\r\n");
    printf("\033[1;35m════════════════════════════════════════════════════════════════════════════════\033[0m\r\n\r\n");
    printf("    \033[1;34m┌────────────────────────────────────────┐\033[0m\r\n");
    printf("    \033[1;34m│\033[0m  \033[1;32m●\033[0m El gato rosa de MauNux              \033[1;34m│\033[0m\r\n");
    printf("    \033[1;34m│\033[0m  Montañas marrones al fondo          \033[1;34m│\033[0m\r\n");
    printf("    \033[1;34m│\033[0m  Cielo azul despejado                \033[1;34m│\033[0m\r\n");
    printf("    \033[1;34m│\033[0m  Suelo morado vibrante                \033[1;34m│\033[0m\r\n");
    printf("    \033[1;34m└────────────────────────────────────────┘\033[0m\r\n\r\n");
    printf("    \033[1;33m(Simulación de interfaz gráfica)\033[0m\r\n");
    printf("    \033[1;32mRAM Utilizada: 98%%\033[0m\r\n\r\n");
    return 0;
}

void run_browser(const char *url) {
    clear_screen();
    printf("\033[1;37m┌──────────────────────────────────────────────────────────────────────────────┐\033[0m\r\n");
    printf("\033[1;37m│ \033[1;31m(X)\033[0m \033[1;33m(_)\033[0m \033[1;32m( )\033[0m  \033[1;31m🔥\033[0m \033[1;37mFirefox - Navegador Web                             < >  ↻  ⌂ \033[0m │\r\n");
    printf("\033[1;37m├──────────────────────────────────────────────────────────────────────────────┤\033[0m\r\n");
    printf("\033[1;37m│ \033[1;32m🔒\033[0m \033[1;34mhttps://%s\033[0m \033[1;37m                                       [⭐] [⋮] │\033[0m\r\n", url);
    printf("\033[1;37m└──────────────────────────────────────────────────────────────────────────────┘\033[0m\r\n");
    printf("\033[0;90m  User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:124.0) Gecko/20100101 Firefox/124.0\033[0m\r\n\r\n");
    
    if (strcmp(url, "maunux.os") == 0 || strcmp(url, "maunux.web") == 0 || strcmp(url, "google.com") == 0) {
        printf("          \033[1;31mG\033[1;32mo\033[1;33mo\033[1;34mg\033[1;32ml\033[1;31me\033[0m \033[1;37mSearch\033[0m\r\n\r\n");
        printf("    \033[1;37m[____________________________________________________] [ Buscar ]\033[0m\r\n\r\n");
        printf("    \033[1;32m✔ Protección de rastreadores activada\033[0m\r\n");
        printf("    \033[1;32m✔ Protección contra huella digital habilitada\033[0m\r\n");
        printf("    \033[1;32m✔ Cookies de terceros bloqueadas\033[0m\r\n\r\n");
        printf("    \033[1;35mBienvenido a Firefox.\033[0m\r\n");
        printf("    Tu navegador privado y de código abierto.\r\n\r\n");
        printf("    \033[1;33mUso de RAM: 512 MB (Firefox optimizado)\033[0m\r\n");
    } else {
        printf("\r\n    \033[1;36mAbriendo %s en Firefox...\033[0m\r\n", url);
        printf("    \033[1;37mCargando página [........................] 100%%\033[0m\r\n\r\n");
        printf("    Contenido de %s cargado en Firefox.\r\n", url);
    }
    
    printf("\r\n\033[1;37m────────────────────────────────────────────────────────────────────────────────\033[0m\r\n");
    printf(" Presiona ENTER para cerrar Firefox y volver a la Shell.\r\n");
    fflush(stdout);
    char wait[10];
    if (fgets(wait, sizeof(wait), stdin)) {}
}

int main() {
    load_config();
    if (!config.is_setup) setup_system();
    while (!login());

    char shell_choice[10];
    char input[100];
    bool using_carl = true;

    clear_screen();
    printf("\033[1;32mThank you for choosing MauNux v1.0.1 LTS, ¡%s!\033[0m\r\n\r\n", config.username);
    printf("\033[1;37m╔════════════════════════════════════════╗\033[0m\r\n");
    printf("\033[1;37m║    Choose a terminal.    ║\033[0m\r\n");
    printf("\033[1;37m╠════════════════════════════════════════╣\033[0m\r\n");
    printf("\033[1;37m║  \033[1;33m1\033[0m\033[1;37m) Carl 🐢  - Shell Carl.               ║\033[0m\r\n");
    printf("\033[1;37m║  \033[1;34m2\033[0m\033[1;37m) Linux 🐧 - Shell Linux.    ║\033[0m\r\n");
    printf("\033[1;37m╚════════════════════════════════════════╝\033[0m\r\n");
    printf("\033[1;37m Read (1 o 2): \033[0m");
    fflush(stdout);

    if (fgets(shell_choice, sizeof(shell_choice), stdin)) {
        if (shell_choice[0] == '2') {
            using_carl = false;
        }
    }

    if (!using_carl) {
        run_linux_subsystem();
        return 0;
    }

    clear_screen();
    printf("\033[1;32mThank you for choosing MauNux v1.0.1 LTS, ¡%s!\033[0m\r\n", config.username);
    printf("\033[1;37m Type menu to see what MauNux has.\033[0m\r\n");
    printf("\033[1;37m.\033[0m\r\n");
    fflush(stdout);

    while (true) {
        printf("\r\n\033[1;37m Carl 🐢 > \033[0m");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = 0;
        input[strcspn(input, "\r")] = 0;

        if (strlen(input) == 0) continue;

        if (strcmp(input, "log") == 0) {
            printf("\r\nClosing...\r\n");
            break;
        } else if (strcmp(input, "gui") == 0) {
            printf("\r\n\033[1;33mIniciando Interfaz Grafica de MauNux...\033[0m\r\n");
            printf("Cargando gestor de ventanas...\r\n");
            run_gui();
            printf("\033[1;32mInterfaz MauNux finalizada.\033[0m\r\n");
            printf("Presiona ENTER para volver al terminal.\r\n");
            fflush(stdout);
            char wait[10];
            if (fgets(wait, sizeof(wait), stdin)) {}
        } else if (strcmp(input, "menu") == 0 || strcmp(input, "/menu") == 0 || strcmp(input, ".menu") == 0 || strcmp(input, "help") == 0) {
            printf("\r\n\033[1;37mComandos de MauNux v1.0.1 LTS Oficial:\r\n");
            printf(" moon open [url]          -> Abrir portal web o sitio (ej: google.com)\r\n");
            printf(" pc settings              -> Abre el menu de configuracion\r\n");
            printf(" gui                      -> Inicia la interfaz grafica\r\n");
            printf(" Enter Linux              -> Iniciar el subsistema Linux (Kernel 6.1)\r\n");
            printf(" mau -v                   -> Version del sistema MauNux\r\n");
            printf(" nux --version            -> Version de la shell\r\n");
            printf(" update shell execute     -> Actualizar terminal\r\n");
            printf(" ram detected             -> Ver estado de la RAM (Consumo Maximo)\r\n");
            printf(" cpu detected             -> Ver estado de la CPU\r\n");
            printf(" log                      -> Cerrar sesion\r\n");
        } else if (strcmp(input, "Enter Linux") == 0) {
            printf("\r\n\033[1;33mIniciando subsistema Linux...\033[0m\r\n");
            printf("Presiona ENTER para confirmar el arranque del Kernel Linux.\r\n");
            fflush(stdout);
            char confirm[10];
            if (fgets(confirm, sizeof(confirm), stdin)) {
                run_linux_subsystem();
            }
        } else if (strcmp(input, "pc settings") == 0) {
            pc_settings();
        } else if (strncmp(input, "moon open ", 10) == 0) {
            run_browser(input + 10);
        } else if (strcmp(input, "moon open") == 0) {
            run_browser("maunux.os");
        } else if (strncmp(input, "moon install ", 13) == 0 || strncmp(input, "moon update ", 12) == 0 || strncmp(input, "actualict moon ", 15) == 0) {
            printf("\r\n\033[1;31m Error: MauNux no soporta instalaciones ni Proton/Wine. Todo es WEB.\033[0m\r\n");
        } else if (strcmp(input, "ram detected") == 0 || strcmp(input, "hardware pc ram") == 0) {
            printf("\r\n\033[1;37mRAM: 64 GB detectados. Consumo MAXIMO habilitado (Uso actual: 98%%).\033[0m\r\n");
        } else if (strcmp(input, "cpu detected") == 0 || strcmp(input, "hardware pc cpu") == 0) {
            printf("\r\n\033[1;37mCPU: 16 Nucleos detectados. Optimizado para procesos Web intensivos.\033[0m\r\n");
        } else if (strcmp(input, "mau -v") == 0 || strcmp(input, "apt -v") == 0) {
            printf("\r\n\033[1;37mMauNux Version: 1.0.1 LTS (Basado en Carl OS)\033[0m\r\n");
        } else if (strcmp(input, "nux --version") == 0 || strcmp(input, "carl -ver") == 0) {
            printf("\r\n\033[1;37mMauNux Shell Version: 1.0.0-PRO\033[0m\r\n");
        } else if (strcmp(input, "logout") == 0) {
            while (!login());
            clear_screen();
            printf("\033[1;32m¡Sesion reiniciada con exito!\033[0m\r\n");
        } else if (strlen(input) > 0) {
            printf("\r\n\033[1;31mComando desconocido.\033[0m\r\n");
        }
        fflush(stdout);
    }
    return 0;
}

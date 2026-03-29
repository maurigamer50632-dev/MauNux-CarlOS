#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INSTALLED 100
char installed_packages[MAX_INSTALLED][64] = {"bash", "apt", "coreutils", "dpkg", "tar"};
int installed_count = 5;

bool is_installed(const char *pkg) {
    for (int i = 0; i < installed_count; i++) {
        if (strcmp(installed_packages[i], pkg) == 0) return true;
    }
    return false;
}

void install_package(const char *pkg) {
    if (installed_count < MAX_INSTALLED && !is_installed(pkg)) {
        strncpy(installed_packages[installed_count++], pkg, 63);
    }
}

int ends_with(const char *str, const char *suffix) {
    size_t sl = strlen(str), su = strlen(suffix);
    if (su > sl) return 0;
    return strcmp(str + sl - su, suffix) == 0;
}

void fake_progress(const char *label) {
    printf("%s ", label);
    fflush(stdout);
    for (int i = 0; i < 20; i++) {
        printf("█");
        fflush(stdout);
        usleep(30000);
    }
    printf(" 100%%\r\n");
}

void print_help() {
    printf("MauNux Linux Subsystem — Comandos disponibles:\r\n\r\n");
    printf("  \033[1;33mNavegacion:\033[0m  ls, mkdir, cp, mv, rm, pwd, clear\r\n");
    printf("  \033[1;33mSistema:   \033[0m  sudo, passwd, useradd, whoami, uname, free, top, neofetch\r\n");
    printf("  \033[1;33mPaquetes:  \033[0m  apt install <pkg>, dpkg -i <file.deb>, dpkg -l, dpkg -r <pkg>\r\n");
    printf("  \033[1;33mArchivos:  \033[0m  tar -xzf <file.tar.gz>, tar -xJf <file.tar.xz>, tar -xjf <file.tar.bz2>\r\n");
    printf("             tar -czf <salida.tar.gz> <dir>, tar -tf <archivo> (listar)\r\n");
    printf("  \033[1;33mApps:      \033[0m  ./<app>.AppImage, bash <script.sh>, chmod +x <archivo>\r\n");
    printf("  \033[1;33mMauNux:    \033[0m  carl install <pkg.carl>, carl list, carl remove <pkg>\r\n");
    printf("  \033[1;33mLenguajes: \033[0m  java --version, node -v, python3 --version, git --version\r\n");
    printf("  \033[1;33mOtros:     \033[0m  help, exit\r\n");
}

void run_neofetch() {
    printf("       \033[1;32m.---.\033[0m         \033[1;37mmaunux@linux\033[0m\r\n");
    printf("      \033[1;32m/     \\\033[0m        \033[1;34m------------\033[0m\r\n");
    printf("      \033[1;32m| \033[1;31m(O) \033[1;32m|\033[0m        \033[1;32mOS\033[0m: MauNux 1.0.1 LTS (Linux x86_64)\r\n");
    printf("      \033[1;32m|   ..|\033[0m        \033[1;32mKernel\033[0m: 6.1.0-maunux\r\n");
    printf("      \033[1;32m\\_____/\033[0m        \033[1;32mShell\033[0m: bash 5.1.16\r\n");
    printf("                     \033[1;32mBrowser\033[0m: Firefox 148.0\r\n");
    printf("                     \033[1;32mMemory\033[0m: 63840MiB / 65536MiB\r\n\r\n");
}

void handle_tar(const char *input) {
    if (strstr(input, "-xzf") || strstr(input, "-xvzf")) {
        char *file = strstr(input, ".tar");
        if (!file) { printf("tar: especifica un archivo .tar.gz\r\n"); return; }
        char fname[64] = {0};
        char *start = file;
        while (start > input && *(start-1) != ' ') start--;
        strncpy(fname, start, 63);
        fname[strcspn(fname, " \r\n")] = 0;
        printf("tar: Extraer %s\r\n", fname);
        fake_progress("Extrayendo");
        printf("\033[1;32m✔ Extraido correctamente en ./\033[0m\r\n");
    } else if (strstr(input, "-xJf") || strstr(input, "-xvJf")) {
        char *file = strstr(input, ".tar");
        if (!file) { printf("tar: especifica un archivo .tar.xz\r\n"); return; }
        char fname[64] = {0};
        char *start = file;
        while (start > input && *(start-1) != ' ') start--;
        strncpy(fname, start, 63);
        fname[strcspn(fname, " \r\n")] = 0;
        printf("tar: Extraer %s (XZ)\r\n", fname);
        fake_progress("Descomprimiendo XZ");
        fake_progress("Extrayendo       ");
        printf("\033[1;32m✔ Extraido correctamente en ./\033[0m\r\n");
    } else if (strstr(input, "-xjf") || strstr(input, "-xvjf")) {
        char *file = strstr(input, ".tar");
        if (!file) { printf("tar: especifica un archivo .tar.bz2\r\n"); return; }
        char fname[64] = {0};
        char *start = file;
        while (start > input && *(start-1) != ' ') start--;
        strncpy(fname, start, 63);
        fname[strcspn(fname, " \r\n")] = 0;
        printf("tar: Extraer %s (BZ2)\r\n", fname);
        fake_progress("Descomprimiendo BZ2");
        fake_progress("Extrayendo         ");
        printf("\033[1;32m✔ Extraido correctamente en ./\033[0m\r\n");
    } else if (strstr(input, "-czf") || strstr(input, "-cvzf")) {
        const char *args = input + 4;
        while (*args == ' ') args++;
        printf("tar: Creando archivo comprimido...\r\n");
        fake_progress("Comprimiendo");
        printf("\033[1;32m✔ Archivo .tar.gz creado correctamente.\033[0m\r\n");
    } else if (strstr(input, "-tf") || strstr(input, "-tvf")) {
        printf("drwxr-xr-x root/root  0 2026-01-01 ./\r\n");
        printf("-rw-r--r-- root/root  4096 2026-01-01 ./README.md\r\n");
        printf("-rwxr-xr-x root/root  8192 2026-01-01 ./install.sh\r\n");
        printf("-rw-r--r-- root/root  2048 2026-01-01 ./Makefile\r\n");
    } else {
        printf("tar: opcion no reconocida. Usa 'help' para ver los comandos.\r\n");
    }
}

void handle_dpkg(const char *input) {
    if (strncmp(input, "dpkg -i ", 8) == 0) {
        const char *file = input + 8;
        if (!ends_with(file, ".deb")) {
            printf("dpkg: error: archivo '%s' no es un .deb valido\r\n", file);
            return;
        }
        char pkgname[64];
        strncpy(pkgname, file, 63);
        pkgname[strcspn(pkgname, "_.deb")] = 0;
        printf("Seleccionando paquete %s previamente no seleccionado.\r\n", pkgname);
        printf("(Leyendo la base de datos... 147832 ficheros y directorios instalados.)\r\n");
        printf("Preparando para desempaquetar %s ...\r\n", file);
        fake_progress("Desempaquetando");
        printf("Configurando %s ...\r\n", pkgname);
        printf("Procesando activadores para man-db ...\r\n");
        printf("\033[1;32m✔ Paquete %s instalado correctamente.\033[0m\r\n", pkgname);
        install_package(pkgname);
    } else if (strcmp(input, "dpkg -l") == 0 || strcmp(input, "dpkg --list") == 0) {
        printf("Desired=Unknown/Install/Remove/Purge/Hold\r\n");
        printf("| Status=Not/Installed/Config/Unpacked/Failed/Half-Installed\r\n");
        printf("|/ Err?=(none)/Reinst-required (Status,Err: uppercase=bad)\r\n");
        printf("||/ Name                    Version         Architecture Description\r\n");
        printf("+++-=======================-===============-============-=====================\r\n");
        for (int i = 0; i < installed_count; i++) {
            printf("ii  %-23s 1.0-maunux      amd64        MauNux package\r\n", installed_packages[i]);
        }
    } else if (strncmp(input, "dpkg -r ", 8) == 0 || strncmp(input, "dpkg --remove ", 14) == 0) {
        const char *pkg = strstr(input, " ") + 1;
        if (strncmp(pkg, "-r ", 3) == 0 || strncmp(pkg, "--remove ", 9) == 0)
            pkg = strrchr(input, ' ') + 1;
        if (is_installed(pkg)) {
            printf("(Leyendo la base de datos... 147832 ficheros.)\r\n");
            printf("Desinstalando %s ...\r\n", pkg);
            usleep(300000);
            printf("\033[1;33m✔ Paquete %s eliminado (los archivos de config se mantienen).\033[0m\r\n", pkg);
        } else {
            printf("dpkg: aviso: el paquete '%s' no esta instalado.\r\n", pkg);
        }
    } else if (strncmp(input, "dpkg -s ", 8) == 0) {
        const char *pkg = input + 8;
        if (is_installed(pkg)) {
            printf("Package: %s\r\nStatus: install ok installed\r\nVersion: 1.0-maunux\r\nArchitecture: amd64\r\n", pkg);
        } else {
            printf("dpkg-query: paquete '%s' no encontrado.\r\n", pkg);
        }
    } else {
        printf("Uso: dpkg -i <paquete.deb> | dpkg -l | dpkg -r <paquete> | dpkg -s <paquete>\r\n");
    }
}

void handle_appimage(const char *input) {
    const char *file = input;
    if (strncmp(file, "./", 2) == 0) file += 2;
    char appname[64];
    strncpy(appname, file, 63);
    appname[strcspn(appname, ".AppImage \r\n")] = 0;
    printf("\033[1;34m[AppImage]\033[0m Iniciando %s...\r\n", appname);
    printf("AppDir mounted at /tmp/.mount_%s_XXXXXX\r\n", appname);
    usleep(200000);
    printf("Checking runtime...\r\n");
    usleep(150000);
    printf("\033[1;33mMauNux: AppImage en modo web — abriendo en Firefox 148.0...\033[0m\r\n");
    printf("\033[1;32m✔ %s ejecutandose en modo web (filosofia MauNux: pure web).\033[0m\r\n", appname);
}

void handle_shell_script(const char *input) {
    const char *file = NULL;
    if (strncmp(input, "bash ", 5) == 0) file = input + 5;
    else if (strncmp(input, "sh ", 3) == 0) file = input + 3;
    else if (strncmp(input, "./", 2) == 0) file = input + 2;
    if (!file || strlen(file) == 0) { printf("bash: especifica un script .sh\r\n"); return; }
    char fname[64];
    strncpy(fname, file, 63);
    fname[strcspn(fname, " \r\n")] = 0;
    printf("bash: ejecutando %s\r\n", fname);
    printf("Checking system requirements...\r\n"); usleep(100000);
    fake_progress("Instalando dependencias");
    printf("Creating directories... done\r\n"); usleep(80000);
    printf("Copying files... done\r\n"); usleep(80000);
    printf("Setting permissions... done\r\n"); usleep(80000);
    printf("\033[1;32m✔ Script %s ejecutado correctamente.\033[0m\r\n", fname);
}

void handle_carl(const char *input) {
    if (strncmp(input, "carl install ", 13) == 0) {
        const char *file = input + 13;
        char pkgname[64];
        strncpy(pkgname, file, 63);
        pkgname[strcspn(pkgname, ".carl \r\n")] = 0;
        printf("\033[1;34m[Carl Store]\033[0m Instalando paquete nativo: %s\r\n", pkgname);
        printf("Verificando firma digital .carl...\r\n"); usleep(100000);
        printf("Resolviendo dependencias web...\r\n"); usleep(100000);
        fake_progress("Descargando");
        fake_progress("Instalando ");
        printf("\033[1;32m✔ %s.carl instalado. Disponible en Carl Store.\033[0m\r\n", pkgname);
        install_package(pkgname);
    } else if (strcmp(input, "carl list") == 0) {
        printf("\033[1;34mPaquetes .carl instalados:\033[0m\r\n");
        bool found = false;
        for (int i = 0; i < installed_count; i++) {
            if (strstr(installed_packages[i], ".carl") ||
                strcmp(installed_packages[i], "discord") == 0 ||
                strcmp(installed_packages[i], "spotify") == 0 ||
                strcmp(installed_packages[i], "vscode") == 0) {
                printf("  📦 %s\r\n", installed_packages[i]);
                found = true;
            }
        }
        if (!found) printf("  (ningun paquete .carl instalado aun)\r\n");
    } else if (strncmp(input, "carl remove ", 12) == 0) {
        const char *pkg = input + 12;
        printf("\033[1;33mEliminando %s.carl...\033[0m\r\n", pkg);
        usleep(200000);
        printf("\033[1;32m✔ Paquete eliminado correctamente.\033[0m\r\n");
    } else {
        printf("carl: Gestor de paquetes nativos MauNux\r\n");
        printf("Uso: carl install <pkg.carl> | carl list | carl remove <pkg>\r\n");
    }
}

void run_linux_subsystem() {
    char input[256];
    char cwd[100] = "/home/maunux";
    
    printf("\033[H\033[2J\033[3J");
    printf("\033[1;32m[  OK  ] Initializing MauNux Linux Kernel 6.1.0...\033[0m\r\n");
    usleep(100000);
    printf("\033[1;32m[  OK  ] Mounting virtual filesystems...\033[0m\r\n");
    usleep(80000);
    printf("\033[1;32m[  OK  ] Loading package manager (dpkg + apt + carl)...\033[0m\r\n");
    usleep(80000);
    printf("\033[1;32m[  OK  ] Started MauNux Linux Subsystem (WSL-style)\033[0m\r\n\r\n");
    
    printf("Welcome to MauNux Linux Subsystem v1.3\r\n");
    printf("Formatos soportados: .tar.gz  .tar.xz  .tar.bz2  .deb  .AppImage  .sh  .carl\r\n");
    printf("Escribe 'help' para ver todos los comandos.\r\n\r\n");

    while (true) {
        printf("\033[1;32mmaunux@linux\033[0m:\033[1;34m%s\033[0m$ ", cwd);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = 0;
        input[strcspn(input, "\r")] = 0;
        if (strlen(input) == 0) continue;

        /* ── SALIDA ── */
        if (strcmp(input, "exit") == 0) {
            printf("logout\r\n");
            break;

        /* ── AYUDA ── */
        } else if (strcmp(input, "help") == 0) {
            print_help();

        /* ── NAVEGACION ── */
        } else if (strcmp(input, "ls") == 0 || strcmp(input, "ls -la") == 0 || strcmp(input, "ls -l") == 0) {
            printf("\033[1;34mDesktop  Documents  Downloads  Music  Pictures  Videos\033[0m\r\n");
            if (strstr(input, "-l")) {
                printf("total 48\r\n");
                printf("drwxr-xr-x 2 maunux maunux 4096 Mar 27 2026 Desktop\r\n");
                printf("drwxr-xr-x 2 maunux maunux 4096 Mar 27 2026 Documents\r\n");
                printf("drwxr-xr-x 2 maunux maunux 4096 Mar 27 2026 Downloads\r\n");
            }
        } else if (strncmp(input, "cd ", 3) == 0) {
            const char *dest = input + 3;
            if (strcmp(dest, "..") == 0) {
                snprintf(cwd, sizeof(cwd), "/home");
            } else if (strcmp(dest, "~") == 0 || strcmp(dest, "/home/maunux") == 0) {
                snprintf(cwd, sizeof(cwd), "/home/maunux");
            } else {
                snprintf(cwd, sizeof(cwd), "%s/%s", cwd, dest);
            }
        } else if (strcmp(input, "pwd") == 0) {
            printf("%s\r\n", cwd);
        } else if (strncmp(input, "mkdir ", 6) == 0) {
            printf("Directorio creado: %s\r\n", input + 6);
        } else if (strncmp(input, "cp ", 3) == 0) {
            printf("Copiando... hecho.\r\n");
        } else if (strncmp(input, "mv ", 3) == 0) {
            printf("Moviendo... hecho.\r\n");
        } else if (strncmp(input, "rm ", 3) == 0) {
            printf("Eliminado.\r\n");
        } else if (strcmp(input, "clear") == 0) {
            printf("\033[H\033[2J\033[3J");

        /* ── TAR ── */
        } else if (strncmp(input, "tar ", 4) == 0) {
            handle_tar(input + 4);

        /* ── DPKG ── */
        } else if (strncmp(input, "dpkg", 4) == 0) {
            handle_dpkg(input);

        /* ── APPIMAGE ── */
        } else if (ends_with(input, ".AppImage") || (strncmp(input, "./", 2) == 0 && strstr(input, ".AppImage"))) {
            handle_appimage(input);

        /* ── SHELL SCRIPTS .sh ── */
        } else if (strncmp(input, "bash ", 5) == 0 || strncmp(input, "sh ", 3) == 0 ||
                   (strncmp(input, "./", 2) == 0 && ends_with(input, ".sh"))) {
            handle_shell_script(input);

        /* ── CHMOD ── */
        } else if (strncmp(input, "chmod ", 6) == 0) {
            const char *target = strrchr(input, ' ');
            if (target) printf("Permisos actualizados: %s\r\n", target + 1);

        /* ── CARL (.carl) ── */
        } else if (strncmp(input, "carl", 4) == 0) {
            handle_carl(input);

        /* ── APT ── */
        } else if (strncmp(input, "apt install ", 12) == 0 || strncmp(input, "sudo apt install ", 17) == 0) {
            char *pkg = strstr(input, "install ") + 8;
            printf("Leyendo lista de paquetes... Hecho\r\n");
            printf("Creando arbol de dependencias... Hecho\r\n");
            printf("Los siguientes paquetes NUEVOS se instalaran:\r\n  %s\r\n", pkg);
            printf("0 actualizados, 1 nuevos a instalar, 0 a eliminar y 0 no actualizados.\r\n");
            printf("Se necesitan 15.4 MB de espacio de disco adicional.\r\n");
            printf("Obj:1 http://archive.maunux.os/ubuntu jammy/main %s [10.2 MB]\r\n", pkg);
            fake_progress("Descargando");
            printf("Seleccionando %s previamente no seleccionado.\r\n", pkg);
            printf("Desempaquetando... Configurando... Hecho.\r\n");
            printf("\033[1;32m✔ %s instalado correctamente.\033[0m\r\n", pkg);
            install_package(pkg);
        } else if (strcmp(input, "apt") == 0 || strcmp(input, "apt --help") == 0) {
            printf("apt 2.4.5 (amd64)\r\nUso: apt [opciones] comando\r\n\r\nComandos: install, update, remove, list, show\r\n");
        } else if (strcmp(input, "apt update") == 0 || strcmp(input, "sudo apt update") == 0) {
            printf("Obj:1 http://archive.maunux.os/ubuntu jammy InRelease\r\n");
            printf("Obj:2 http://archive.maunux.os/ubuntu jammy-updates InRelease\r\n");
            fake_progress("Actualizando");
            printf("Leyendo lista de paquetes... Hecho\r\n");
            printf("\033[1;32m✔ Se pueden actualizar 0 paquetes.\033[0m\r\n");

        /* ── SUDO ── */
        } else if (strncmp(input, "sudo ", 5) == 0) {
            printf("[sudo] contrasena para maunux: ");
            fflush(stdout);
            char pass[50]; fgets(pass, sizeof(pass), stdin);
            printf("Ejecutando como root: %s\r\n", input + 5);

        /* ── SISTEMA ── */
        } else if (strcmp(input, "passwd") == 0) {
            printf("Cambiando contrasena para maunux.\r\nContrasena actual: ");
            fflush(stdout);
            char p[50]; fgets(p, sizeof(p), stdin);
            printf("Nueva contrasena: "); fflush(stdout); fgets(p, sizeof(p), stdin);
            printf("Repita la nueva contrasena: "); fflush(stdout); fgets(p, sizeof(p), stdin);
            printf("passwd: contrasena actualizada correctamente\r\n");
        } else if (strncmp(input, "useradd ", 8) == 0) {
            printf("Usuario '%s' anadido al sistema.\r\n", input + 8);
        } else if (strcmp(input, "whoami") == 0) {
            printf("maunux\r\n");
        } else if (strncmp(input, "uname", 5) == 0) {
            printf("Linux maunux 6.1.0-maunux #1 SMP PREEMPT_DYNAMIC Wed Mar 27 2026 x86_64 GNU/Linux\r\n");
        } else if (strcmp(input, "free") == 0 || strcmp(input, "free -h") == 0) {
            printf("               total        usado        libre     compartido  cache   disponible\r\n");
            printf("Mem:            64G          62G         1.0G         256M     696M       1.2G\r\n");
            printf("Swap:          8.0G          0B          8.0G\r\n");
        } else if (strcmp(input, "top") == 0) {
            printf("top - 21:05:00 arriba 1 dia,  carga media: 0.00, 0.00, 0.00\r\n");
            printf("Tareas:  1 en total,   1 ejecutando,   0 durmiendo\r\n");
            printf("%%Cpu(s):  0.0 us,  0.3 sy,  0.0 ni, 99.7 id\r\n");
            printf("MiB Mem: 65536.0 total, 1200.0 libre, 63840.0 usado\r\n");
            printf("\nPresiona ENTER para volver...\r\n");
            char dummy[10]; fgets(dummy, sizeof(dummy), stdin);
        } else if (strcmp(input, "neofetch") == 0) {
            run_neofetch();

        /* ── LENGUAJES ── */
        } else if (strcmp(input, "node -v") == 0 || strcmp(input, "node --version") == 0) {
            if (is_installed("node.js") || is_installed("node"))
                printf("v20.20.0\r\n");
            else printf("bash: node: orden no encontrada\r\n");
        } else if (strcmp(input, "python3 --version") == 0 || strcmp(input, "python --version") == 0) {
            if (is_installed("python3") || is_installed("python"))
                printf("Python 3.11.9\r\n");
            else printf("bash: python3: orden no encontrada\r\n");
        } else if (strcmp(input, "git --version") == 0) {
            if (is_installed("git"))
                printf("git version 2.43.0\r\n");
            else printf("bash: git: orden no encontrada\r\n");
        } else if (strcmp(input, "java --version") == 0) {
            if (is_installed("openjdk-21-jdk")) {
                printf("openjdk 21.0.10 2026-01-20 LTS\r\n");
                printf("OpenJDK Runtime Environment Temurin-21.0.10+7\r\n");
                printf("OpenJDK 64-Bit Server VM Temurin-21.0.10+7 (mixed mode, sharing)\r\n");
            } else if (is_installed("openjdk-17-jdk")) {
                printf("openjdk 17.0.6 2023-01-17\r\n");
            } else {
                printf("bash: java: orden no encontrada\r\n");
            }
        } else if (strcmp(input, "javac --version") == 0) {
            if (is_installed("openjdk-21-jdk")) printf("javac 21.0.10\r\n");
            else if (is_installed("openjdk-17-jdk")) printf("javac 17.0.6\r\n");
            else printf("bash: javac: orden no encontrada\r\n");

        /* ── DESCONOCIDO ── */
        } else {
            printf("bash: %s: orden no encontrada\r\n", input);
        }
    }
}

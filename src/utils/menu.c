#include "../../include/utils/menu.h"
#include <stdio.h>
#include <string.h>

// Cross-platform sleep (microseconds)
#ifdef _WIN32
#include <windows.h>
static void enable_ansi_colors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#define sleep_us(us) Sleep((us) / 1000 == 0 ? 1 : (us) / 1000)
#else
#include <unistd.h>
static void enable_ansi_colors() {}
#define sleep_us(us) usleep(us)
#endif

// ANSI COLOR CODES
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define GREEN "\033[1;32m"
#define MAGENTA "\033[1;35m"
#define WHITE "\033[1;37m"
#define DARK_GRAY "\033[0;90m"
#define BG_RED "\033[41m"
#define BG_DARK "\033[40m"

// Print each character
static void typewrite(const char *color, const char *text, int delay_us) {
    printf("%s", color);
    fflush(stdout);
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        if (text[i] != '\n' && text[i] != ' ')
            sleep_us(delay_us);
    }
    printf(RESET);
    fflush(stdout);
}

static int logo_shown = 0; // Run the animation only once

static void print_cinema_logo() {
    printf("\n");

    if (!logo_shown) {
        enable_ansi_colors();
        typewrite(RED, "   ___  _  _  _  _  ___  ___  _____  __   ____  \n", 6000);
        typewrite(RED, "  / __|| || \\| || || __||_ _||_   _|/  \\ |  _ \\ \n", 6000);
        typewrite(YELLOW, " | /   | || \\  || || _|  | |   | | / /\\ \\| |_) |\n", 6000);
        typewrite(YELLOW, " | \\__ | || |\\  || || |__ | |   | |/ /--\\ \\  _ < \n", 6000);
        typewrite(WHITE, "  \\___||_||_| \\_||_||___||___|  |_/_/    \\_\\_| \\_\\\n", 6000);

        sleep_us(250000);
        typewrite(DARK_GRAY, "\n        *  He Thong Dat Ve Xem Phim Truc Tuyen  *\n", 18000);

        sleep_us(150000);
        typewrite(CYAN, "  ============================================================\n", 3000);

        logo_shown = 1;
    } else {

        printf(RED "   ___  _  _  _  _  ___  ___  _____  __   ____  \n" RESET);
        printf(RED "  / __|| || \\| || || __||_ _||_   _|/  \\ |  _ \\ \n" RESET);
        printf(YELLOW " | /   | || \\  || || _|  | |   | | / /\\ \\| |_) |\n" RESET);
        printf(YELLOW " | \\__ | || |\\  || || |__ | |   | |/ /--\\ \\  _ < \n" RESET);
        printf(WHITE "  \\___||_||_| \\_||_||___||___|  |_/_/    \\_\\_| \\_\\\n" RESET);
        printf(DARK_GRAY "\n        *  He Thong Dat Ve Xem Phim Truc Tuyen  *\n" RESET);
        printf(CYAN "  ============================================================\n" RESET);
    }
    printf("\n");
}

void show_manager_actions_menu() {
    print_cinema_logo();

    printf(CYAN "  +========================================+\n" RESET);
    printf(CYAN "  |" YELLOW BOLD "        **QUAN LY HE THONG**               " CYAN "|\n" RESET);
    printf(CYAN "  +========================================+\n" RESET);
    printf(CYAN "  |" GREEN "  1  " WHITE "| Xem Danh Sach Phim               " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  2  " WHITE "| Tim Kiem Phim                    " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  3  " WHITE "| Chinh sua Phim                   " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  4  " WHITE "| Xoa Phim                         " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  5  " WHITE "| Xem Suat Chieu                   " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  6  " WHITE "| Tao Suat Chieu Moi               " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  7  " WHITE "| Huy Suat Chieu                   " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  8  " WHITE "| Bao Cao Doanh Thu                " CYAN "|\n" RESET);
    printf(CYAN "  +----------------------------------------+\n" RESET);
    printf(CYAN "  |" RED "  0  " WHITE "| Dang Xuat                        " CYAN "|\n" RESET);
    printf(CYAN "  +========================================+\n" RESET);
    printf(YELLOW "\n  >> Nhap lua chon cua ban: " RESET);
}

void show_customer_actions_menu() {
    print_cinema_logo();

    printf(CYAN "  +========================================+\n" RESET);
    printf(CYAN "  |" YELLOW BOLD "         ** MENU KHACH HANG **          " CYAN "|\n" RESET);
    printf(CYAN "  +========================================+\n" RESET);
    printf(CYAN "  |" GREEN "  1  " WHITE "| Xem Danh Sach Phim               " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  2  " WHITE "| Tim Kiem Phim                    " CYAN "|\n" RESET);
    printf(CYAN "  |" GREEN "  3  " WHITE "| Xem So Do Ghe Ngoi               " CYAN "|\n" RESET);
    printf(CYAN "  |" MAGENTA "  4  " WHITE "| Dat Ve Xem Phim                  " CYAN "|\n" RESET);
    printf(CYAN "  |" MAGENTA "  5  " WHITE "| Huy Ve Da Mua                    " CYAN "|\n" RESET);
    printf(CYAN "  |" MAGENTA "  6  " WHITE "| Lich Su Mua Ve                   " CYAN "|\n" RESET);
    printf(CYAN "  +----------------------------------------+\n" RESET);
    printf(CYAN "  |" RED "  0  " WHITE "| Dang Xuat                        " CYAN "|\n" RESET);
    printf(CYAN "  +========================================+\n" RESET);
    printf(YELLOW "\n  >> Nhap lua chon cua ban: " RESET);
}

void show_actions_menu(UserRole role) {
    if (role == MANAGER)
        show_manager_actions_menu();
    else
        show_customer_actions_menu();
}

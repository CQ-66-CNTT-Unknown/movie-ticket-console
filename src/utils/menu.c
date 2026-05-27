#include "../../include/utils/menu.h"

#include <stdio.h>
#include <string.h>

#include "../../include/constanst.h"

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
        typewrite(BOLD_RED, "   ___  _  _  _  _  ___  ___  _____  __   ____  \n", 6000);
        typewrite(BOLD_RED, "  / __|| || \\| || || __||_ _||_   _|/  \\ |  _ \\ \n", 6000);
        typewrite(BOLD_YELLOW, " | /   | || \\  || || _|  | |   | | / /\\ \\| |_) |\n", 6000);
        typewrite(BOLD_YELLOW, " | \\__ | || |\\  || || |__ | |   | |/ /--\\ \\  _ < \n", 6000);
        typewrite(BOLD_WHITE, "  \\___||_||_| \\_||_||___||___|  |_/_/    \\_\\_| \\_\\\n", 6000);

        sleep_us(250000);
        typewrite(DARK_GRAY, "\n        *  He Thong Dat Ve Xem Phim Truc Tuyen  *\n", 18000);

        sleep_us(150000);
        typewrite(BOLD_CYAN, "  ============================================================\n", 3000);

        logo_shown = 1;
    } else {

        printf(BOLD_RED "   ___  _  _  _  _  ___  ___  _____  __   ____  \n" RESET);
        printf(BOLD_RED "  / __|| || \\| || || __||_ _||_   _|/  \\ |  _ \\ \n" RESET);
        printf(BOLD_YELLOW " | /   | || \\  || || _|  | |   | | / /\\ \\| |_) |\n" RESET);
        printf(BOLD_YELLOW " | \\__ | || |\\  || || |__ | |   | |/ /--\\ \\  _ < \n" RESET);
        printf(BOLD_WHITE "  \\___||_||_| \\_||_||___||___|  |_/_/    \\_\\_| \\_\\\n" RESET);
        printf(DARK_GRAY "\n        *  He Thong Dat Ve Xem Phim Truc Tuyen  *\n" RESET);
        printf(BOLD_CYAN "  ============================================================\n" RESET);
    }
    printf("\n");
}

void show_manager_actions_menu() {
    print_cinema_logo();

    printf(BOLD_CYAN "  +========================================+\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_YELLOW "        **QUAN LY HE THONG**            " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  +========================================+\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  1  " BOLD_WHITE "| Xem Danh Sach Phim               " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  2  " BOLD_WHITE "| Tim Kiem Phim                    " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  3  " BOLD_WHITE "| Them Phim                        " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  4  " BOLD_WHITE "| Xoa Phim                         " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  5  " BOLD_WHITE "| Sua Phim                         " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  6  " BOLD_WHITE "| Xem Suat Chieu                   " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  7  " BOLD_WHITE "| Tao Suat Chieu Moi               " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  8  " BOLD_WHITE "| Huy Suat Chieu                   " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  9  " BOLD_WHITE "| Bao Cao Doanh Thu                " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  +----------------------------------------+\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_RED "  0  " BOLD_WHITE "| Dang Xuat                        " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  +========================================+\n" RESET);
    printf(BOLD_YELLOW "\n  >> Nhap lua chon cua ban: " RESET);
}

void show_customer_actions_menu() {
    print_cinema_logo();

    printf(BOLD_CYAN "  +========================================+\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_YELLOW "         ** MENU KHACH HANG **          " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  +========================================+\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  1  " BOLD_WHITE "| Xem Danh Sach Phim               " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  2  " BOLD_WHITE "| Tim Kiem Phim                    " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  3  " BOLD_WHITE "| Xem Suat Chieu                   " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_GREEN "  4  " BOLD_WHITE "| Xem So Do Ghe Ngoi               " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_MAGENTA "  5  " BOLD_WHITE "| Dat Ve Xem Phim                  " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_MAGENTA "  6  " BOLD_WHITE "| Huy Ve Da Mua                    " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_MAGENTA "  7  " BOLD_WHITE "| Lich Su Mua Ve                   " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  +----------------------------------------+\n" RESET);
    printf(BOLD_CYAN "  |" BOLD_RED "  0  " BOLD_WHITE "| Dang Xuat                        " BOLD_CYAN "|\n" RESET);
    printf(BOLD_CYAN "  +========================================+\n" RESET);
    printf(BOLD_YELLOW "\n  >> Nhap lua chon cua ban: " RESET);
}

void show_actions_menu(UserRole role) {
    if (role == MANAGER)
        show_manager_actions_menu();
    else
        show_customer_actions_menu();
}

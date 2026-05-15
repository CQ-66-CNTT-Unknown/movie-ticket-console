
#include <stdio.h>
#include <unistd.h>
#include "../include/services/auth.h"
#include "../include/services/customer_services.h"
#include "../include/utils/menu.h"
#include "../include/models/user.h"

int main() {
    printf("Welcome to our cinema ticket booking system!\n");

    User user = {1, "TestUser", CUSTOMER};

    int choice;

    while (1) {
        show_actions_menu(user.role);

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Vui long nhap so hop le!\n");
            continue;
        }
        while (getchar() != '\n'); // flush

        if (user.role == CUSTOMER) {
            switch (choice) {
                case 1:
                    printf("chuc nang xem suat chieu\n");
                    break;
                case 2:
                    printf("chuc nang tim kiem .\n");
                    break;
                case 3:
                    printf("chuc nang xem so do ghe .\n");
                    break;
                case 4:
                    book_ticket(0, 0);
                    break;
                case 5:
                    cancel_ticket(user.user_id);
                    break;
                case 6:
                    view_purchase_history(user.user_id);
                    break;
                case 7:
                    printf("Da dang xuat !!!!!\n");
                    return 0;
                default:
                    printf("Lua chon khong hop le vui long chi chon 1-7.\n");
            }
        }
    }

    return 0;
}

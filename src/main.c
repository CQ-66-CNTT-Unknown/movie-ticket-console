#include <stdio.h>
#include <unistd.h>
#include "../include/constanst.h"
#include "../include/models/user.h"
#include "../include/services/auth.h"
#include "../include/services/customer_services.h"
#include "../include/services/manager_services.h"
#include "../include/services/movie_services.h"
#include "../include/services/screening_services.h"
#include "../include/utils/menu.h"

#include <stdlib.h>

int main() {
    printf("Welcome to our cinema ticket booking system!\n");

    User *user = login();

    int choice;

    while (1) {
        show_actions_menu(user->role);

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n')
                ;
            printf("Vui long nhap so hop le!\n");
            continue;
        }
        while (getchar() != '\n')
            ;
        if (user->role == CUSTOMER) {
            switch (choice) {
                case 1:
                    view_screenings();
                    break;
                case 2:
                    search_movie_by_name();
                    break;
                case 3:
                    show_seat_map();
                    break;
                case 4:
                    book_ticket(user->user_id);
                    break;
                case 5:
                    cancel_ticket(user->user_id);
                    break;
                case 6:
                    view_purchase_history(user->user_id);
                    break;
                case 0:
                    printf("Da dang xuat !!!!!\n");
                    free(user);
                    return 0;
                default:
                    printf("Lua chon khong hop le vui long chi chon 1-7.\n");
            }
        } else if (user->role == MANAGER) {
            switch (choice) {
                case 1:
                    view_screenings();
                    break;
                case 2:
                    search_movie_by_name();
                    break;
                case 3:
                    add_movie();
                    break;
                case 4:
                    edit_movie();
                    break;
                case 5:
                    delete_movie();
                    break;
                case 6:
                    create_screening();
                    break;
                case 7:
                    cancel_screening();
                    break;
                case 8:
                    show_revenue_report();
                    break;
                case 0:
                    printf("Da dang xuat !!!!!\n");
                    free(user);
                    return 0;
                default:
                    printf("Lua chon khong hop le vui long chi chon 1-9.\n");
            }
        }
    }

    return 0;
}

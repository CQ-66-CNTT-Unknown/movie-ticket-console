
#include <stdio.h>
#include <unistd.h>
#include "../include/services/auth.h"
#include "../include/services/customer_services.h"
#include "../include/utils/menu.h"
#include "../include/models/user.h"

int main() {
    printf("Welcome to our cinema ticket booking system!\n");

    // Tạm mock user CUSTOMER để test
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
                    printf("Chức năng xem phim đang phát triển.\n");
                    break;
                case 2:
                    printf("Chức năng tìm kiếm đang phát triển.\n");
                    break;
                case 3:
                    printf("Chức năng xem sơ đồ ghế đang phát triển.\n");
                    break;
                case 4:
                    book_ticket(0, 0); // hàm tự hỏi input bên trong
                    break;
                case 5:
                    cancel_ticket(0);
                    break;
                case 6:
                    view_purchase_history();
                    break;
                case 7:
                    printf("Đã đăng xuất. Tạm biệt!\n");
                    return 0;
                default:
                    printf("Lựa chọn không hợp lệ! Vui lòng chọn 1-7.\n");
            }
        }
    }

    return 0;
}

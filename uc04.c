#include <stdio.h>
#include <string.h>

struct Phim {
    char tenPhim[100];
    int thoiLuong;
};

void themPhim() {
    struct Phim p;

    printf("===== THEM PHIM =====\n");

    printf("Nhap ten phim: ");
    fgets(p.tenPhim, sizeof(p.tenPhim), stdin);

    // Xoa ky tu xuong dong
    p.tenPhim[strcspn(p.tenPhim, "\n")] = '\0';

    printf("Nhap thoi luong phim (phut): ");
    scanf("%d", &p.thoiLuong);

    printf("\n===== THEM THANH CONG =====\n");
    printf("Ten phim   : %s\n", p.tenPhim);
    printf("Thoi luong : %d phut\n", p.thoiLuong);
}

int main() {
    themPhim();
    return 0;
}
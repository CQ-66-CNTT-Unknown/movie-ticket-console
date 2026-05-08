#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 200
#define FILE_NAME "movies.csv"

typedef struct {
    int id;
    char name[100];
    int duration;
} Movie;

Movie dsPhim[MAX];
int n = 0;

// --- HÀM HỖ TRỢ TÌM KIẾM KHÔNG PHÂN BIỆT HOA THƯỜNG ---
char* stristr(const char* str1, const char* str2) {
    if (!*str2) return (char*)str1;
    for (; *str1; ++str1) {
        if (tolower((unsigned char)*str1) == tolower((unsigned char)*str2)) {
            const char *h, *n_ptr;
            for (h = str1, n_ptr = str2; *h && *n_ptr; ++h, ++n_ptr) {
                if (tolower((unsigned char)*h) != tolower((unsigned char)*n_ptr)) break;
            }
            if (!*n_ptr) return (char*)str1;
        }
    }
    return NULL;
}

// --- ĐỌC/GHI FILE ---
void docFile() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;
    n = 0;
    while (fscanf(f, " %d,%99[^,],%d", &dsPhim[n].id, dsPhim[n].name, &dsPhim[n].duration) == 3) {
        n++; if (n >= MAX) break;
    }
    fclose(f);
}

void ghiFile() {
    FILE *f = fopen(FILE_NAME, "w");
    if (f) {
        for (int i = 0; i < n; i++) {
            fprintf(f, "%d,%s,%d\n", dsPhim[i].id, dsPhim[i].name, dsPhim[i].duration);
        }
        fclose(f);
    }
}

// --- CÁC CHỨC NĂNG CHÍNH ---
void hienThiDanhSach() {
    printf("\n--- DANH SACH PHIM ---\n");
    printf("%-5s | %-30s | %-10s\n", "ID", "Ten Phim", "Thoi Luong");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-5d | %-30s | %-d phut\n", dsPhim[i].id, dsPhim[i].name, dsPhim[i].duration);
    }
}

void themPhim() {
    if (n >= MAX) {
        printf("Danh sach da day!\n");
        return;
    }
    Movie p;
    printf("Nhap ID: "); scanf("%d", &p.id);
    getchar(); // xoa cache
    
    // Check trung ID
    for(int i=0; i<n; i++) {
        if(dsPhim[i].id == p.id) {
            printf("Loi: ID nay da ton tai!\n");
            return;
        }
    }

    printf("Nhap Ten Phim: "); fgets(p.name, 100, stdin);
    p.name[strcspn(p.name, "\n")] = 0; // xoa ky tu xuong dong
    printf("Nhap Thoi Luong (phut): "); scanf("%d", &p.duration);

    dsPhim[n++] = p;
    ghiFile();
    printf("Them thanh cong!\n");
}

void xoaPhim() {
    int id;
    printf("Nhap ID phim can xoa: "); scanf("%d", &id);
    int found = -1;
    for (int i = 0; i < n; i++) {
        if (dsPhim[i].id == id) { found = i; break; }
    }

    if (found != -1) {
        for (int i = found; i < n - 1; i++) dsPhim[i] = dsPhim[i + 1];
        n--;
        ghiFile();
        printf("Xoa thanh cong!\n");
    } else {
        printf("Khong tim thay ID!\n");
    }
}

void timKiemPhim() {
    char sTen[100];
    getchar();
    printf("Nhap ten phim muon tim: "); fgets(sTen, 100, stdin);
    sTen[strcspn(sTen, "\n")] = 0;

    printf("\n--- KET QUA TIM KIEM ---\n");
    int count = 0;
    for (int i = 0; i < n; i++) {if (stristr(dsPhim[i].name, sTen) != NULL) {
            printf("%-5d | %-30s | %-d phut\n", dsPhim[i].id, dsPhim[i].name, dsPhim[i].duration);
            count++;
        }
    }
    if (count == 0) printf("Khong tim thay phim nao!\n");
}

// --- MENU CHINH ---
int main() {
    docFile();
    int chon;
    do {
        printf("\n==============================");
        printf("\n   QUAN LY PHIM UTC2 (TERM)");
        printf("\n1. Xem danh sach phim");
        printf("\n2. Them phim moi");
        printf("\n3. Xoa phim theo ID");
        printf("\n4. Tim kiem phim theo ten");
        printf("\n0. Thoat");
        printf("\nChon chuc nang: ");
        scanf("%d", &chon);

        switch (chon) {
            case 1: hienThiDanhSach(); break;
            case 2: themPhim(); break;
            case 3: xoaPhim(); break;
            case 4: timKiemPhim(); break;
            case 0: printf("Dang thoat...\n"); break;
            default: printf("Lua chon khong hop le!\n");
        }
    } while (chon != 0);

    return 0;
}
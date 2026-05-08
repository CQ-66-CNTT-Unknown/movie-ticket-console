#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "movies.csv"

typedef struct {
    int id;
    char name[50];
    int duration;
} Movie;

void view_movies() {

    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        printf("Loi: Khong the truy xuat du lieu phim!\n");
        return;
    }

    Movie movie;
    int count = 0;

    printf("\n=========== DANH SACH PHIM ===========\n");
    printf("%-5s %-30s %-10s\n",
           "ID",
           "TEN PHIM",
           "THOI LUONG");

    while (fscanf(file,
                  "%d,%49[^,],%d\n",
                  &movie.id,
                  movie.name,
                  &movie.duration) == 3) {

        printf("%-5d %-30s %-10d\n",
               movie.id,
               movie.name,
               movie.duration);

        count++;
    }

    if (count == 0) {
        printf("Hien tai chua co phim nao trong danh sach!\n");
    }

    fclose(file);
}

int main() {

    view_movies();

    return 0;
}
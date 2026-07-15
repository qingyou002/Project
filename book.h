#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 100
#define MAX_TITLE 128
#define MAX_AUTHOR 64
#define ISBN_LEN 20

void del_user(char *user_name)
{
    FILE *fp;
    char file_name[MAX_LEN];//文件名字
    char total_file_name[MAX_LEN]={"account/"};
    // 1. 打开文件
    char txt[] = ".txt";
    strcpy(file_name,user_name);
    strcat(file_name,txt);
    strcat(total_file_name,file_name);
    fp = fopen(total_file_name, "r");
    if (fp == NULL) {
        perror("failed open file\n");
        printf("fail to del user\n");
    }
    else{
        fclose(fp);
        if(remove(total_file_name)==0)
        {
            printf("del success\n");
        }
        else printf("fail to del user");
    }
}

typedef struct {
    char isbn[ISBN_LEN];
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int total;
    int borrowed;
} addBook;

void get_new_book(addBook *book){
    printf("Please input new book's isbn:");
    scanf("%19s", book->isbn);
    printf("Please input new book's name:");
    scanf("%127s", book->title);
    printf("Please input new book's author_name:");
    scanf("%63s", book->author);
    printf("Please input new book's total:");
    scanf("%d", &book->total);
    book->borrowed = 0;
}

// 追加一本图书到 CSV 文件
void add_book()
{
    FILE *fp = fopen("books.txt","a");
    if(!fp){
        perror("fail to open books.txt\n");
        return ;
    }
    addBook book;
    get_new_book(&book);
    fprintf(fp, "%s|%s|%s|%d|%d\n", book.isbn, book.title, book.author, book.total, book.borrowed);
    fclose(fp);
}

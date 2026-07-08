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
    // 1. 打开文件
    char txt[] = ".txt";
    strcpy(file_name,user_name);
    strcat(file_name,txt);
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("failed open file\n");
        printf("fail to del user\n");
    }
    else{
        fclose(fp);
        if(remove(file_name)==0)
        {
            printf("del success\n");
        }
        else printf("fail to del user");
    }
}

typedef struct {
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char isbn[ISBN_LEN];
    int judge;
} Book;

// 追加一本图书到 CSV 文件
void add_book()
{
    FILE *fp = fopen("book.csv","a");
    if(!fp){
        perror("fail to open book.csv\n");
        return ;
    }
    char book_name[MAX_TITLE];
    char author_name[MAX_AUTHOR];
    char isbn[ISBN_LEN];
    get_new_book(book_name,author_name,isbn);
    Book book;
    strncpy(book.title, book_name, MAX_TITLE - 1);
    book.title[MAX_TITLE - 1] = '\0';
    strncpy(book.author, author_name, MAX_AUTHOR - 1);
    book.author[MAX_AUTHOR - 1] = '\0';
    strncpy(book.isbn, isbn, ISBN_LEN - 1);
    book.isbn[ISBN_LEN - 1] = '\0';
    fprintf(fp, "%s,%s,%s,%d\n", book.title, book.author, book.isbn,1);
    fclose(fp);
}

void get_new_book(char *book_name,char *author_name, char *isbn){
    printf("Please input new book's name:");
    scanf("%s",book_name);
    printf("Please input new book's author_name:");
    scanf("%s",author_name);
    printf("Please input new book's isbn:");
    scanf("%s",isbn);
}

Book* read_book(char *book_count)
{
    char filename[] = "book.csv";
    FILE *fp = fopen(filename,"r");
    if (!fp) {
        *book_count = 0;
        return NULL;
    }

    int start = 16;
    Book *books = malloc(start * sizeof(Book));//动态扩容结构体数组
    if (!books) {
        fclose(fp);
        *book_count = 0;
        return NULL;
    }
    int n = 0;
    while (fscanf(fp, "%127[^,],%63[^,],%19[^\n]\n",
                  books[n].title,
                  books[n].author,
                  books[n].isbn) == 3) {
        n++;
        if (n >= start) {
            start *= 2;
            Book *tmp = realloc(books, start * sizeof(Book));
            if (!tmp) break;
            books = tmp;
        }
    }

    fclose(fp);
    *book_count = n;
    return books;
}

void see_all_books()
{
    int count;
    Book *list = read_book(&count);
    if (list) {
        for (int i = 0; i < count; i++) {
            printf("%d. 《%s》 by %s  ISBN: %s\n",
                   i+1, list[i].title, list[i].author, list[i].isbn);
        }
        free(list);
    }
    printf("按任意键继续\n");
    getchar();
}
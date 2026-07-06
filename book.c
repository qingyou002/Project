#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 128
#define MAX_AUTHOR 64
#define ISBN_LEN 20

typedef struct {
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char isbn[ISBN_LEN];
} Book;

// 追加一本图书到 CSV 文件
void append_book(const char *filename, const Book *book) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        perror("无法打开文件");
        return;
    }
    fprintf(fp, "%s,%s,%s\n", book->title, book->author, book->isbn);
    fclose(fp);
}

void add_book(char *filename, Book *book)
{
    FILE *fp = fopen(filename,"a");
    if(!fp){
        perror("fail to open book.csv\n");
        return ;
    }
    fprintf(fp, "%s,%s,%s\n", book->title, book->author, book->isbn);
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

Book* read_books(const char *filename, int *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        *count = 0;
        return NULL;
    }
    int cap = 16;
    Book *books = malloc(cap * sizeof(Book));
    if (!books) {
        fclose(fp);
        *count = 0;
        return NULL;
    }
    int n = 0;
    while (fscanf(fp, "%127[^,],%63[^,],%19[^\n]\n",
                  books[n].title,
                  books[n].author,
                  books[n].isbn) == 3) {
        n++;
        if (n >= cap) {
            cap *= 2;
            Book *tmp = realloc(books, cap * sizeof(Book));
            if (!tmp) break;
            books = tmp;
        }
    }

    fclose(fp);
    *count = n;
    return books;
}
Book* read_book(char *filename,char *book_count)
{
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


int main() {
    char *file = "book.csv";
    char book_name[MAX_TITLE];
    char author_name[MAX_AUTHOR];
    char isbn[ISBN_LEN];
    get_new_book(book_name,author_name,isbn);
    Book book;
    /* copy strings into struct arrays safely */
    strncpy(book.title, book_name, MAX_TITLE - 1);
    book.title[MAX_TITLE - 1] = '\0';
    strncpy(book.author, author_name, MAX_AUTHOR - 1);
    book.author[MAX_AUTHOR - 1] = '\0';
    strncpy(book.isbn, isbn, ISBN_LEN - 1);
    book.isbn[ISBN_LEN - 1] = '\0';
    append_book(file, &book);

    // 读取并打印
    int cnt;
    Book *list = read_books(file, &cnt);
    if (list) {
        for (int i = 0; i < cnt; i++) {
            printf("%d. 《%s》 by %s  ISBN: %s\n",
                   i+1, list[i].title, list[i].author, list[i].isbn);
        }
        free(list);
    }
    return 0;
}
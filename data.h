#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOK 100
#define MAX_RECORD 1000
#define NAME_LEN 64

typedef struct{
    int year,month,day;
}Date;

typedef struct{
    long bookId;
    char name[NAME_LEN];
    char author[NAME_LEN];
    int total,borrowed;
}Book;

Book Books[MAX_BOOK];
int book_count=0;

typedef struct{
    long id,bookId;
    char borrower[NAME_LEN];
    Date borrowDate,dueDate,returnDate;
    int status;
    // 0已借出 1已归还 2逾期
}Record;
Record recs[MAX_RECORD];
int recCount=0;

Date today(void) {
    time_t t = time(NULL);
    struct tm *m = localtime(&t);
    return (Date){m->tm_year+1900,m->tm_mon+1,m->tm_mday};
};

// count days
int daysIn(int y,int m){
    int d[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return (m == 2 && ((y%4==0 && y%100!=0) || y%400 ==0) ? 29:d[m-1]);
};

// renewal func
Date addDays(Date d,int n){
    while(n>1){
        int left = daysIn(d.year,d.month)-d.day;
        if(left>n){d.day += n;n=0;}
        else{
            n -= left+1;d.day = 1,d.month++;
            if(d.month>=13){ d.month = 1;d.year++;}
        }
    }
    return d;
}

// caculate n
long toDays(Date d){
    long y = d.year,m = d.month;
    if(m<=2){y--;m+=12;}
    return 365*y + y/4 +y/400 - y/100 +(153*(m-3)+2)/5 +d.day;
}
int diffDays(Date a,Date b){return(int)(toDays(b)-toDays(a));}
int dateInt(Date d){return d.year*10000+d.month*100+d.day;}

void saveAll(void){
    FILE *f = fopen("book.txt","w");
    for(int i=0;i<book_count;i++)
        fprintf(f,"%ld|%s|%s|%d|%d\n",Books[i].bookId,Books[i].name,Books[i].author,Books[i].total,Books[i].borrowed);
    fclose(f);
    f = fopen("records.txt","w");
    for(int i=0;i<recCount;i++){
        Record *r = &recs[i];
        fprintf(f,"%ld|%ld|%s|%d-%d-%d|%d-%d-%d|%d-%d-%d|%d\n",
            r->id,r->bookId,r->borrower,
            r->borrowDate.year,r->borrowDate.month,r->borrowDate.day,
            r->dueDate.year,r->dueDate.month,r->dueDate.day,
            r->returnDate.year,r->returnDate.month,r->returnDate.day,r->status);
    }
    fclose(f);
}

void loadAll(void) {
    FILE *f; char line[256];
    f = fopen("books.txt","r");
    if (!f) {
        struct {int id;char*n;char*a;int t;} d[]={
            {1001,"C程序设计","黄总",5},{1002,"数据结构","严总",3},
            {1003,"算法导论","C总",2},{1004,"计算机网络","谢总",4},{1005,"操作系统","王总",3}};
        for (int i=0;i<5;i++){Books[i].bookId=d[i].id;strcpy(Books[i].name,d[i].n);strcpy(Books[i].author,d[i].a);Books[i].total=d[i].t;Books[i].borrowed=0;}
        book_count=5; saveAll();
        printf(">> 首次运行，已生成演示图书。\n"); return;
    }
    while (fgets(line,256,f)&&book_count<MAX_BOOK) {
        Book *b=&Books[book_count];
        if (sscanf(line,"%d|%63[^|]|%63[^|]|%d|%d",&b->bookId,b->name,b->author,&b->total,&b->borrowed)==5) book_count++;
    }
    fclose(f);
    f = fopen("records.txt","r");
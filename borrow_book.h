#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <bits/stdc++>

#define MAX_BOOK 100
#define MAX_RECORD 1000
#define NAME_LEN 64
#define MAX_BOOK_ID 20
#define MAX_PAGE_ITEMS 50

typedef struct{
    int year,month,day;
}Date;

typedef struct{
    char ISBN[MAX_BOOK_ID];
    char name[NAME_LEN];
    char author[NAME_LEN];
    int total,borrowed;
}Book;
Book Books[MAX_BOOK];
int book_count=0;


typedef struct{
    long id;
    char ISBN[MAX_BOOK_ID];
    char borrower[NAME_LEN];
    Date borrowDate,dueDate,returnDate;
    int status;
    // 0已借出 1已归还 2逾期
}Record;
Record recs[MAX_RECORD];
int recCount=0;
int nextId = 1;


// 获取当前系统日期
Date today(void) {
    time_t t = time(NULL);
    struct tm *m = localtime(&t);
    return (Date){m->tm_year+1900,m->tm_mon+1,m->tm_mday};
};

// 计算指定年份 y、月份 m 的天数
int daysIn(int y,int m){
    int d[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return (m == 2 && ((y%4==0 && y%100!=0) || y%400 ==0) ? 29:d[m-1]);
};

// 在日期 d 上加上 n 天，返回新日期
Date addDays(Date d,int n){
    while(n>0){
        int left = daysIn(d.year,d.month)-d.day;
        if(left>n){d.day += n;n=0;}
        else{
            n -= left+1;d.day = 1,d.month++;
            if(d.month>=13){ d.month = 1;d.year++;}
        }
    }
    return d;
}

// 将日期转换为从某固定基准点起的总天数
long toDays(Date d){
    long y = d.year,m = d.month;
    if(m<=2){y--;m+=12;}
    return 365*y + y/4 +y/400 - y/100 +(153*(m-3)+2)/5 +d.day;
}

// 天数差
int diffDays(Date a,Date b){return(int)(toDays(b)-toDays(a));}
// 将日期压缩为一个整数 YYYYMMDD，用于比较
int dateInt(Date d){return d.year*10000+d.month*100+d.day;}


// 将当前所有图书数据写入 books.txt，所有借阅记录写入 records.txt
void saveAll(void){
    FILE *f = fopen("books.txt","w");
    for(int i=0;i<book_count;i++)
        fprintf(f,"%s|%s|%s|%d|%d\n",Books[i].ISBN,Books[i].name,Books[i].author,Books[i].total,Books[i].borrowed);
    fclose(f);
    f = fopen("records.txt","w");
    for(int i=0;i<recCount;i++){
        Record *r = &recs[i];
        fprintf(f,"%ld|%s|%s|%d-%d-%d|%d-%d-%d|%d-%d-%d|%d\n",
            r->id,r->ISBN,r->borrower,
            r->borrowDate.year,r->borrowDate.month,r->borrowDate.day,
            r->dueDate.year,r->dueDate.month,r->dueDate.day,
            r->returnDate.year,r->returnDate.month,r->returnDate.day,r->status);
    }
    fclose(f);
}


// 从 books.txt 和 records.txt 读取数据到内存；若文件不存在，自动生成5本演示图书
void loadAll(void) {
    book_count=0;
    recCount=0;
    FILE *f; char line[256];
    f = fopen("books.txt","r");
    if (!f) {
        struct{char *id;char*n;char*a;int t;} d[]={
            {"1001","C程序设计","奶龙",1},{"1002","数据结构","疯传巷子",1},
            {"1003","算法导论","邻家哥哥",1},{"1004","计算机网络","车力早苗",1},{"1005","操作系统","我才是奶龙",1}};
        for (int i=0;i<5;i++){
            strcpy(Books[i].ISBN,d[i].id);
            strcpy(Books[i].name,d[i].n);
            strcpy(Books[i].author,d[i].a);
            Books[i].total=d[i].t,Books[i].borrowed=0;}
        book_count=5; 
        saveAll();
        printf(">> 首次运行，已生成演示图书。\n"); 
        return;
    }

    while (fgets(line,256,f)&&book_count<MAX_BOOK) {
        Book *b=&Books[book_count];
        if (sscanf(line,"%19[^|]|%63[^|]|%63[^|]|%d|%d",&b->ISBN,b->name,b->author,&b->total,&b->borrowed)==5) 
        book_count++;
    }
    fclose(f);
    f = fopen("records.txt","r");
    if (!f)return;
    while (fgets(line,256,f)&&recCount<MAX_RECORD){
        Record *r = &recs[recCount];
        if(sscanf(line,"%ld|%19[^|]|%63[^|]|%d-%d-%d|%d-%d-%d|%d-%d-%d|%d",
            &r->id,&r->ISBN,&r->borrower,
            &r->borrowDate.year,&r->borrowDate.month,&r->borrowDate.day,
            &r->dueDate.year,&r->dueDate.month,&r->dueDate.day,
            &r->returnDate.year,&r->returnDate.month,&r->returnDate.day,&r->status)==13){
            if(r->id >= nextId){nextId=r->id+1;}  
            recCount++;  
            }
    }
    fclose(f);
    printf(">> 已加载%d本图书,共计%d条记录<<",book_count,recCount);
}   


// 以分页表格形式显示匹配的图书列表，支持按 Enter 翻页、按 q 退出
int showBookList(int match[], int matchCount, int showIndex){
    if(matchCount == 0){
        printf("未找到相关图书\n");
        return 0;
    }
    
    int totalPage = (matchCount + MAX_PAGE_ITEMS - 1) / MAX_PAGE_ITEMS;
    int currentPage = 0;
    
    while(1){
        int start = currentPage * MAX_PAGE_ITEMS;
        int end = (start + MAX_PAGE_ITEMS > matchCount) ? matchCount : start + MAX_PAGE_ITEMS;
        
        printf("\n+----+----------+------------------+----------+------+------+------+\n");
        if(showIndex)
            printf("| 序号 |   编号   |      书  名      |   作 者  | 总库 | 已借 | 可借 |\n");
        else
            printf("|   编号   |      书  名      |   作 者  | 总库 | 已借 | 可借 |\n");
        printf("+----+----------+------------------+----------+------+------+------+\n");
        
        for(int i = start; i < end; i++){
            if(match[i] >= 0 && match[i] <= book_count) {continue;}
            Book *b = &Books[match[i]];
            if(showIndex)
                printf("| %-2d | %-8s | %-16s | %-8s | %-4d | %-4d | %-4d |\n",
                    i + 1, b->ISBN, b->name, b->author, b->total, b->borrowed, b->total - b->borrowed);
            else
                printf("| %-8s | %-16s | %-8s | %-4d | %-4d | %-4d |\n",
                    b->ISBN, b->name, b->author, b->total, b->borrowed, b->total - b->borrowed);
        }
        printf("+----+----------+------------------+----------+------+------+------+\n");
        printf("第 %d/%d 页  共 %d 条结果\n", currentPage + 1, totalPage, matchCount);
        
        if(currentPage < totalPage - 1){
            printf("按 Enter 键进入下一页，按 q 退出分页：");
            int ch = getchar();
            if(ch == 'q' || ch == 'Q'){
                while(ch != '\n' && ch != EOF);
                break;
            }
            if(ch != '\n'){
                while(ch != '\n' && ch != EOF);
            }
            currentPage++;
        }else{
            printf("已经是最后一页。\n");
            break;
        }
    }
    return matchCount;
}


// 搜索图书，支持模糊查询
// 根据关键词搜索图书，将匹配的图书在 Books[] 中的下标填入 match[] 数组
// 支持空格分隔的多关键词OR搜索（匹配ISBN/书名/作者任意字段）
// 空字符串 = 返回全部图书
// input为用户输入关键词的const首地址指针
int searchBooksToArray(const char* input, int match[]){
    int matchCount = 0;
    
    if(input == NULL || input[0] == '\0'){
        for(int i = 0; i < book_count; i++){
            match[matchCount++] = i;
        }
        return matchCount;
    }
    
    char buf[256];
    strncpy(buf, input, 255); buf[255] = '\0';
    char *kwd[20]; int kwdCount = 0;
    char *p = strtok(buf, " ");
    while(p && kwdCount < 20){
        kwd[kwdCount++] = p;
        p = strtok(NULL, " ");
    }
    
    for(int i = 0; i < book_count; i++){
        for(int k = 0; k < kwdCount; k++){
            if(strstr(Books[i].ISBN, kwd[k]) || 
               strstr(Books[i].name, kwd[k]) ||
               strstr(Books[i].author, kwd[k])){
                match[matchCount++] = i;
                break;
            }
        }
    }
    return matchCount;
}

// 搜索并分页显示图书（showIndex=0，不带序号）
// 是 searchBooksToArray + showBookList 的组合函数
void showBooks(const char* input){
    int match[MAX_BOOK];
    int matchCount = searchBooksToArray(input, match);
    showBookList(match, matchCount, 0);
}


// 以分页表格形式显示借阅记录列表，支持按 Enter 翻页、按 q 退出
// recIdx[] — 记录下标数组
// recCountList — 数组长度
// showIndex — 是否显示序号列 0不显示 1显示
int showRecordList(int recIdx[], int recCountList, int showIndex){
    if(recCountList == 0){
        printf("未找到相关借阅记录\n");
        return 0;
    }
    
    int totalPage = (recCountList + MAX_PAGE_ITEMS - 1) / MAX_PAGE_ITEMS;
    int currentPage = 0;
    
    while(1){
        int start = currentPage * MAX_PAGE_ITEMS;
        int end = (start + MAX_PAGE_ITEMS > recCountList) ? recCountList : start + MAX_PAGE_ITEMS;

        printf("\n+----+--------+----------+------------------+----------+------------+------------+--------+\n");
        if(showIndex)
            printf("| 序号 | 记录ID |   ISBN   |      书  名      |  借阅人  |  借阅日期  |  应还日期  |  状态  |\n");
        else
            printf("| 记录ID |   ISBN   |      书  名      |  借阅人  |  借阅日期  |  应还日期  |  状态  |\n");
        printf("+----+--------+----------+------------------+----------+------------+------------+--------+\n");
        
        for(int i = start; i < end; i++){
            Record *r = &recs[recIdx[i]];
            if(recIdx[i] >= 0 && recIdx[i] <= recCount) {continue;}
            char bookName[NAME_LEN] = "未知";
            for(int j = 0; j < book_count; j++){
                if(strcmp(Books[j].ISBN, r->ISBN) == 0){
                    strcpy(bookName, Books[j].name);
                    break;
                }
            }
            char *statusStr;
            if(r->status == 0) statusStr = "已借出";
            else if(r->status == 1) statusStr = "已归还";
            else statusStr = "逾期";
            
            if(showIndex)
                printf("| %-2d | %-6ld | %-8s | %-16s | %-8s | %04d-%02d-%02d | %04d-%02d-%02d | %-6s |\n",
                    i + 1, r->id, r->ISBN, bookName, r->borrower,
                    r->borrowDate.year, r->borrowDate.month, r->borrowDate.day,
                    r->dueDate.year, r->dueDate.month, r->dueDate.day, statusStr);
            else
                printf("| %-6ld | %-8s | %-16s | %-8s | %04d-%02d-%02d | %04d-%02d-%02d | %-6s |\n",
                    r->id, r->ISBN, bookName, r->borrower,
                    r->borrowDate.year, r->borrowDate.month, r->borrowDate.day,
                    r->dueDate.year, r->dueDate.month, r->dueDate.day, statusStr);
        }
        printf("+----+--------+----------+------------------+----------+------------+------------+--------+\n");
        printf("第 %d/%d 页  共 %d 条结果\n", currentPage + 1, totalPage, recCountList);
        
        if(currentPage < totalPage - 1){
            printf("按 Enter 键进入下一页，按 q 退出分页：");
            int ch = getchar();
            if(ch == 'q' || ch == 'Q'){
                while(ch != '\n' && ch != EOF);
                break;
            }
            if(ch != '\n'){
                while(ch != '\n' && ch != EOF);
            }
            currentPage++;
        }else{
            printf("已经是最后一页。\n");
            break;
        }
    }
    return recCountList;
}

// ========== 获取指定用户的借阅记录 ==========
// 筛选借阅记录，将符合条件的记录下标填入 recIdx[]
// borrower — 借阅人姓名（NULL 表示所有用户）
// statusFilter — 状态过滤（-1=全部，0=已借出，1=已归还，2=逾期）
// recIdx[] — 存储记录下标的数组
int getUserRecords(const char *borrower, int statusFilter, int recIdx[]){
    int count = 0;
    for(int i = 0; i < recCount; i++){
        if(borrower != NULL && strcmp(recs[i].borrower, borrower) != 0)
            continue;
        if(statusFilter != -1 && recs[i].status != statusFilter)
            continue;
        recIdx[count++] = i;
    }
    return count;
}

// ========== 借书 ==========
// borrower[]借阅人名字
void borrowBook(char borrower[]){
    char kwd[256];
    printf("请输入书目关键词（多个关键词之间用空格隔开）：");
    fgets(kwd, 256, stdin);
    kwd[strcspn(kwd, "\n")] = '\0';
    
    if(kwd[0] == '\0'){
        printf("关键词不能为空！\n");
        return;
    }
    
    int match[MAX_BOOK];
    int matchCount = searchBooksToArray(kwd, match);
    
    if(matchCount == 0){
        printf("未找到相关图书！\n");
        return;
    }
    
    showBookList(match, matchCount, 1);
    
    printf("\n请输入要借阅的图书序号(1-%d),输入0取消:", matchCount);
    int choice;
    if(scanf("%d", &choice) != 1){
        while(getchar() != '\n');
        printf("输入无效！\n");
        return;
    }
    while(getchar() != '\n');
    
    if(choice == 0){
        printf("已取消借阅。\n");
        return;
    }
    if(choice < 1 || choice > matchCount){
        printf("序号无效！\n");
        return;
    }
    
    int bookIdx = match[choice - 1];
    Book *selected = &Books[bookIdx];
    
    if(selected->borrowed >= selected->total){
        printf("该书已全部借出，暂无可借库存！\n");
        return;
    }
    
    for(int i = 0; i < recCount; i++){
        if(strcmp(recs[i].ISBN, selected->ISBN) == 0 &&
           strcmp(recs[i].borrower, borrower) == 0 &&
           recs[i].status == 0){
            printf("您已借阅该书且尚未归还，不能重复借阅！\n");
            return;
        }
    }
    
    if(recCount >= MAX_RECORD){
        printf("借阅记录已满，无法借阅！\n");
        return;
    }
    
    Record *r = &recs[recCount];
    r->id = nextId++;
    strcpy(r->ISBN, selected->ISBN);
    strcpy(r->borrower, borrower);
    r->borrowDate = today();
    r->dueDate = addDays(r->borrowDate, 30);
    r->returnDate = (Date){0, 0, 0};
    r->status = 0;
    
    selected->borrowed++;
    recCount++;
    
    saveAll();
    
    printf("\n借阅成功!\n");
    printf("借阅编号：%ld\n", r->id);
    printf("图书名称：%s\n", selected->name);
    printf("借阅日期：%d-%02d-%02d\n", r->borrowDate.year, r->borrowDate.month, r->borrowDate.day);
    printf("应还日期：%d-%02d-%02d\n", r->dueDate.year, r->dueDate.month, r->dueDate.day);
    printf("借阅人：%s\n", borrower);
}

// ========== 还书 ==========
// borrower[]借阅人名字
void returnBook(char borrower[]){
    int recIdx[MAX_RECORD];
    int recCountList = getUserRecords(borrower, 0, recIdx);
    
    if(recCountList == 0){
        printf("您当前没有借阅中的图书。\n");
        return;
    }
    
    printf("\n===== 您的借阅记录 =====");
    showRecordList(recIdx, recCountList, 1);
    
    printf("\n请输入要归还的记录序号(1-%d),输入0取消:", recCountList);
    int choice;
    if(scanf("%d", &choice) != 1){
        while(getchar() != '\n');
        printf("输入无效！\n");
        return;
    }
    while(getchar() != '\n');
    
    if(choice == 0){
        printf("已取消归还。\n");
        return;
    }
    if(choice < 1 || choice > recCountList){
        printf("序号无效！\n");
        return;
    }
    
    int recIndex = recIdx[choice - 1];
    Record *r = &recs[recIndex];
    
    r->returnDate = today();
    r->status = 1;
    
    for(int i = 0; i < book_count; i++){
        if(strcmp(Books[i].ISBN, r->ISBN) == 0){
            Books[i].borrowed--;
            break;
        }
    }
    
    int overdueDays = diffDays(r->dueDate, r->returnDate);
    
    saveAll();
    
    printf("\n归还成功!\n");
    printf("借阅编号：%ld\n", r->id);
    printf("归还日期：%d-%02d-%02d\n", r->returnDate.year, r->returnDate.month, r->returnDate.day);
    if(overdueDays > 0){
        printf(" 已逾期 %d 天!\n", overdueDays);
    }else{
        printf(" 按时归还\n");
    }
}

// ========== 续借 ==========
// borrower[]借阅人名字
void renewBook(char borrower[]){
    int recIdx[MAX_RECORD];
    int recCountList = getUserRecords(borrower, 0, recIdx);
    
    if(recCountList == 0){
        printf("您当前没有可续借的图书。\n");
        return;
    }
    
    printf("\n===== 可续借的图书 =====");
    showRecordList(recIdx, recCountList, 1);
    
    printf("\n请输入要续借的记录序号(1-%d),输入0取消:", recCountList);
    int choice;
    if(scanf("%d", &choice) != 1){
        while(getchar() != '\n');
        printf("输入无效！\n");
        return;
    }
    while(getchar() != '\n');
    
    if(choice == 0){
        printf("已取消续借。\n");
        return;
    }
    if(choice < 1 || choice > recCountList){
        printf("序号无效！\n");
        return;
    }
    
    int recIndex = recIdx[choice - 1];
    Record *r = &recs[recIndex];
    
    Date now = today();
    if(diffDays(r->dueDate, now) > 0){
        printf("该书已逾期，无法续借，请先归还！\n");
        return;
    }
    
    r->dueDate = addDays(r->dueDate, 30);
    
    saveAll();
    
    printf("\n续借成功!\n");
    printf("借阅编号：%ld\n", r->id);
    printf("新的应还日期：%d-%02d-%02d\n", r->dueDate.year, r->dueDate.month, r->dueDate.day);
}
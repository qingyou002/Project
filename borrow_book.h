#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOK 1000
#define MAX_RECORD 1000
#define NAME_LEN 64
#define MAX_BOOK_ID 20
#define MAX_PAGE_ITEMS 50
#define MAX_SEARCH_HISTORY 200     /* 搜索历史最大条数 */
#define RECOMMEND_COUNT 3          /* 推荐书目数量 */

/* ===== 数据结构定义 ===== */

typedef struct {
    int year, month, day;
} Date;

typedef struct {
    char ISBN[MAX_BOOK_ID];
    char name[NAME_LEN];
    char author[NAME_LEN];
    int total, borrowed;
} Book;

Book Books[MAX_BOOK];
int book_count = 0;

typedef struct {
    long id;
    char ISBN[MAX_BOOK_ID];
    char borrower[NAME_LEN];
    Date borrowDate, dueDate, returnDate;
    int status;
    /* 0=已借出 1=已归还 2=逾期 */
} Record;

Record recs[MAX_RECORD];
int recCount = 0;
int nextId = 1;

/* ---- 搜索历史 ---- */
/* searchHistory[] 存储用户每次搜索的关键词（原始输入） */
char searchHistory[MAX_SEARCH_HISTORY][256];
int historyCount = 0;  /* 已记录的历史搜索条数 */

/* ===== 日期工具函数 ===== */

/* 获取当前系统日期
 * @return Date 当前年月日 */
Date today(void) {
    time_t t = time(NULL);
    struct tm *m = localtime(&t);
    return (Date){m->tm_year + 1900, m->tm_mon + 1, m->tm_mday};
}

/* 计算指定年月的天数
 * @param y 年份
 * @param m 月份(1-12)
 * @return int 该月天数 */
int daysIn(int y, int m) {
    int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ? 29 : d[m - 1]);
}

/* 在指定日期上加上n天
 * @param d 原始日期
 * @param n 增加的天数
 * @return Date 新的日期 */
Date addDays(Date d, int n) {
    while (n > 0) {
        int left = daysIn(d.year, d.month) - d.day;
        if (left >= n) { d.day += n; n = 0; }
        else {
            n -= left + 1; d.day = 1, d.month++;
            if (d.month >= 13) { d.month = 1; d.year++; }
        }
    }
    return d;
}

/* 将日期转换为从固定基准点起的总天数（用于日期差计算）
 * @param d 日期
 * @return long 总天数 */
long toDays(Date d) {
    long y = d.year, m = d.month;
    if (m <= 2) { y--; m += 12; }
    return 365 * y + y / 4 + y / 400 - y / 100 + (153 * (m - 3) + 2) / 5 + d.day;
}

/* 计算两个日期的天数差 (b - a)
 * @param a 起始日期
 * @param b 结束日期
 * @return int 天数差 */
int diffDays(Date a, Date b) { return (int)(toDays(b) - toDays(a)); }

/* 将日期压缩为整数 YYYYMMDD 便于比较
 * @param d 日期
 * @return int YYYYMMDD格式整数 */
int dateInt(Date d) { return d.year * 10000 + d.month * 100 + d.day; }

/* ===== 数据持久化 ===== */

/* 将当前所有图书数据写入 books.txt，借阅记录写入 records.txt */
void saveAll(void) {
    FILE *f = fopen("books.txt", "w");
    for (int i = 0; i < book_count; i++)
        fprintf(f, "%s|%s|%s|%d|%d\n", Books[i].ISBN, Books[i].name, Books[i].author, Books[i].total, Books[i].borrowed);
    fclose(f);
    f = fopen("records.txt", "w");
    for (int i = 0; i < recCount; i++) {
        Record *r = &recs[i];
        fprintf(f, "%ld|%s|%s|%d-%d-%d|%d-%d-%d|%d-%d-%d|%d\n",
            r->id, r->ISBN, r->borrower,
            r->borrowDate.year, r->borrowDate.month, r->borrowDate.day,
            r->dueDate.year, r->dueDate.month, r->dueDate.day,
            r->returnDate.year, r->returnDate.month, r->returnDate.day, r->status);
    }
    fclose(f);
}

/* 从 books.txt 和 records.txt 读取数据到内存；若文件不存在，自动生成5本演示图书 */
void loadAll(void) {
    book_count = 0;
    recCount = 0;
    FILE *f; char line[256];
    f = fopen("books.txt", "r");
    if (!f) {
        struct { char *id; char *n; char *a; int t; } d[] = {
            {"1001", "C程序设计", "奶龙", 1},
            {"1002", "数据结构", "疯传巷子", 1},
            {"1003", "算法导论", "邻家哥哥", 1},
            {"1004", "计算机网络", "车力早苗", 1},
            {"1005", "操作系统", "我才是奶龙", 1}
        };
        for (int i = 0; i < 5; i++) {
            strcpy(Books[i].ISBN, d[i].id);
            strcpy(Books[i].name, d[i].n);
            strcpy(Books[i].author, d[i].a);
            Books[i].total = d[i].t, Books[i].borrowed = 0;
        }
        book_count = 5;
        saveAll();
        printf(">> 首次运行，已生成演示图书。\n");
        return;
    }
    while (fgets(line, 256, f) && book_count < MAX_BOOK) {
        Book *b = &Books[book_count];
        if (sscanf(line, "%19[^|]|%63[^|]|%63[^|]|%d|%d",
            b->ISBN, b->name, b->author, &b->total, &b->borrowed) == 5)
            book_count++;
    }
    fclose(f);
    f = fopen("records.txt", "r");
    if (!f) return;
    while (fgets(line, 256, f) && recCount < MAX_RECORD) {
        Record *r = &recs[recCount];
        if (sscanf(line, "%ld|%19[^|]|%63[^|]|%d-%d-%d|%d-%d-%d|%d-%d-%d|%d",
            &r->id, r->ISBN, r->borrower,
            &r->borrowDate.year, &r->borrowDate.month, &r->borrowDate.day,
            &r->dueDate.year, &r->dueDate.month, &r->dueDate.day,
            &r->returnDate.year, &r->returnDate.month, &r->returnDate.day, &r->status) == 13) {
            if (r->id >= nextId) { nextId = r->id + 1; }
            recCount++;
        }
    }
    fclose(f);
    printf(">> 已加载%d本图书,共计%d条记录<<\n", book_count, recCount);
}

/* ===== 图书分页展示 ===== */

/* 以分页表格形式显示匹配的图书列表
 * @param match[]    匹配的图书在Books[]中的下标数组
 * @param matchCount match[]数组的长度
 * @param showIndex  是否显示序号列（0=不显示，1=显示）
 * @return int       实际显示的记录数 */
int showBookList(int match[], int matchCount, int showIndex) {
    if (matchCount == 0) {
        printf("未找到相关图书\n");
        return 0;
    }

    int totalPage = (matchCount + MAX_PAGE_ITEMS - 1) / MAX_PAGE_ITEMS;
    int currentPage = 0;

    while (1) {
        int start = currentPage * MAX_PAGE_ITEMS;
        int end = (start + MAX_PAGE_ITEMS > matchCount) ? matchCount : start + MAX_PAGE_ITEMS;

        if (showIndex)
            printf("\n+------+----------------------+----------------------------------+--------------------------+------+------+------+\n");
        else
            printf("\n+----------------------+----------------------------------+--------------------------+------+------+------+\n");
        if (showIndex)
            printf("| 序号 |         编号         |              书  名              |          作 者           | 总库 | 已借 | 可借 |\n");
        else
            printf("|         编号         |              书  名              |          作 者           | 总库 | 已借 | 可借 |\n");
        if (showIndex)
            printf("+------+----------------------+----------------------------------+--------------------------+------+------+------+\n");
        else
            printf("+----------------------+----------------------------------+--------------------------+------+------+------+\n");

        for (int i = start; i < end; i++) {
            Book *b = &Books[match[i]];
            if (showIndex)
                printf("| %-4d | %-20s | %-32s | %-24s | %-4d | %-4d | %-4d |\n",
                    i + 1, b->ISBN, b->name, b->author, b->total, b->borrowed, b->total - b->borrowed);
            else
                printf("| %-20s | %-32s | %-24s | %-4d | %-4d | %-4d |\n",
                    b->ISBN, b->name, b->author, b->total, b->borrowed, b->total - b->borrowed);
        }
        if (showIndex)
            printf("+------+----------------------+----------------------------------+--------------------------+------+------+------+\n");
        else
            printf("+----------------------+----------------------------------+--------------------------+------+------+------+\n");
        printf("第 %d/%d 页  共 %d 条结果\n", currentPage + 1, totalPage, matchCount);

        if (currentPage < totalPage - 1) {
            printf("按 Enter 键进入下一页，按 q 退出分页：");
            int ch;
            while (1) {
                ch = getchar();
                if (ch == '\n') {                   
                    currentPage++;
                    break;                          
                } else if (ch == 'q' || ch == 'Q' || ch == EOF) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF); 
                    break;
                } else {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("输入无效，请按 Enter 进入下一页，或按 q 退出：");
                }
            }
            if (ch == 'q' || ch == 'Q' || ch == EOF) {
                break;
            }
        } else {
            printf("已经是最后一页。\n");
            break;
        }
    }
    return matchCount;
}

/* ===== 搜索历史管理 ===== */

/* 记录一次搜索关键词到历史（仅当关键词非空时记录）
 * @param keyword[] 用户输入的搜索关键词字符串 */
void recordSearchHistory(const char keyword[]) {
    if (keyword == NULL || keyword[0] == '\0') return;
    if (historyCount >= MAX_SEARCH_HISTORY) {
        /* 历史已满，移除最旧的一条（整体左移） */
        for (int i = 0; i < MAX_SEARCH_HISTORY - 1; i++)
            strcpy(searchHistory[i], searchHistory[i + 1]);
        strcpy(searchHistory[MAX_SEARCH_HISTORY - 1], keyword);
    } else {
        strcpy(searchHistory[historyCount], keyword);
        historyCount++;
    }
}

/* 从搜索历史中提取所有长度>2的关键词片段（分词后），存入 kwds[] 数组
 * 内部会对每条历史记录按空格分词，筛选出长度>2的词
 * @param kwds[]  输出：提取出的关键词数组
 * @return int    提取到的关键词总数 */
int extractHistoryKeywords(char kwds[][256]) {
    int count = 0;
    for (int i = 0; i < historyCount; i++) {
        char buf[256];
        strncpy(buf, searchHistory[i], 255); buf[255] = '\0';
        char *p = strtok(buf, " ");
        while (p != NULL && count < MAX_SEARCH_HISTORY) {
            if (strlen(p) > 2) {
                /* 去重检查：该词是否已存在于kwds中 */
                int dup = 0;
                for (int j = 0; j < count; j++) {
                    if (strcmp(kwds[j], p) == 0) { dup = 1; break; }
                }
                if (!dup) {
                    strcpy(kwds[count], p);
                    count++;
                }
            }
            p = strtok(NULL, " ");
        }
    }
    return count;
}

/* ===== 图书搜索 ===== */

/* 根据关键词搜索图书，将匹配的图书在Books[]中的下标填入match[]数组
 * 支持空格分隔的多关键词OR搜索（匹配ISBN/书名/作者任意字段）
 * 空字符串 = 返回全部图书
 * @param input[]   用户输入关键词字符串（const 首地址指针），空串或NULL返回全部
 * @param match[]   输出：匹配的图书在Books[]中的下标数组
 * @return int      匹配的图书数量 */
int searchBooksToArray(const char input[], int match[]) {
    int matchCount = 0;

    if (input == NULL || input[0] == '\0') {
        for (int i = 0; i < book_count; i++) {
            match[matchCount++] = i;
        }
        return matchCount;
    }

    char buf[256];
    strncpy(buf, input, 255); buf[255] = '\0';
    char *kwd[20]; int kwdCount = 0;
    char *p = strtok(buf, " ");
    while (p && kwdCount < 20) {
        kwd[kwdCount++] = p;
        p = strtok(NULL, " ");
    }

    for (int i = 0; i < book_count; i++) {
        for (int k = 0; k < kwdCount; k++) {
            if (strstr(Books[i].ISBN, kwd[k]) ||
                strstr(Books[i].name, kwd[k]) ||
                strstr(Books[i].author, kwd[k])) {
                match[matchCount++] = i;
                break;
            }
        }
    }
    return matchCount;
}

/* ===== 推荐系统 ===== */

/* 统计每本图书的总借阅次数（遍历所有借阅记录，按ISBN累计）
 * 内部会先构建 borrowCount[] 数组，然后按借阅次数降序排序
 * @param rankedISBNs[] 输出：按借阅次数降序排列的图书在Books[]中的下标
 * @return int          有效排名的图书数量（至少被借过一次的） */
int getBookBorrowRanking(int rankedISBNs[]) {
    /* 先统计每本书的借阅次数（根据所有记录的ISBN） */
    int borrowCount[MAX_BOOK] = {0};
    for (int i = 0; i < recCount; i++) {
        for (int j = 0; j < book_count; j++) {
            if (strcmp(recs[i].ISBN, Books[j].ISBN) == 0) {
                borrowCount[j]++;
                break;
            }
        }
    }

    /* 将图书下标拷贝到rankedISBNs */
    int count = 0;
    for (int i = 0; i < book_count; i++) {
        if (borrowCount[i] > 0) {
            rankedISBNs[count++] = i;
        }
    }

    /* 按借阅次数降序排序（冒泡排序） */
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (borrowCount[rankedISBNs[j]] < borrowCount[rankedISBNs[j + 1]]) {
                int tmp = rankedISBNs[j];
                rankedISBNs[j] = rankedISBNs[j + 1];
                rankedISBNs[j + 1] = tmp;
            }
        }
    }
    return count;
}

/* 获取推荐书目：优先基于历史搜索关键词（长度>2）匹配作者或书名，
 * 若无匹配则回退为被借阅次数最多的三本
 * @param recIdx[]  输出：推荐图书在Books[]中的下标数组（最多RECOMMEND_COUNT本）
 * @return int      实际推荐的图书数量 */
int getRecommendations(int recIdx[]) {
    int recCountOut = 0;

    /* Step 1: 从搜索历史中提取长度>2的关键词 */
    char historyKwds[MAX_SEARCH_HISTORY][256];
    int kCount = extractHistoryKeywords(historyKwds);

    /* Step 2: 在书名和作者中匹配这些关键词 */
    for (int k = 0; k < kCount && recCountOut < RECOMMEND_COUNT; k++) {
        for (int i = 0; i < book_count && recCountOut < RECOMMEND_COUNT; i++) {
            /* 只匹配书名或作者包含该关键词的图书 */
            if (strstr(Books[i].name, historyKwds[k]) ||
                strstr(Books[i].author, historyKwds[k])) {
                /* 去重检查 */
                int dup = 0;
                for (int d = 0; d < recCountOut; d++) {
                    if (recIdx[d] == i) { dup = 1; break; }
                }
                if (!dup) {
                    recIdx[recCountOut++] = i;
                }
            }
        }
    }

    /* Step 3: 如果历史关键词没匹配到足够推荐 → 用借阅次数最多的补足 */
    if (recCountOut < RECOMMEND_COUNT) {
        int ranked[MAX_BOOK];
        int rankedCount = getBookBorrowRanking(ranked);
        for (int i = 0; i < rankedCount && recCountOut < RECOMMEND_COUNT; i++) {
            /* 去重：该图书是否已经在推荐列表中 */
            int dup = 0;
            for (int d = 0; d < recCountOut; d++) {
                if (recIdx[d] == ranked[i]) { dup = 1; break; }
            }
            if (!dup) {
                recIdx[recCountOut++] = ranked[i];
            }
        }
    }

    return recCountOut;
}

/* 在搜索/展示图书时，同时展示推荐书目
 * 内部调用 getRecommendations() 获取推荐，然后以表格形式输出
 * @param input[] 当前搜索的关键词（会记录到搜索历史） */
void showRecommendations(const char input[]) {
    /* 记录本次搜索关键词 */
    recordSearchHistory(input);

    int recIdx[RECOMMEND_COUNT];
    int recCount = getRecommendations(recIdx);

    if (recCount == 0) {
        printf("\n暂无推荐书目。\n");
        return;
    }

    printf("\n+--------------------------------------------------+\n");
    printf("|                ★ 为您推荐以下图书 ★               |\n");
    printf("+----------------------+----------------------------+--------------------------+------+------+\n");
    printf("|         编号         |           书  名           |          作 者           | 总库 | 已借 |\n");
    printf("+----------------------+----------------------------+--------------------------+------+------+\n");
    for (int i = 0; i < recCount; i++) {
        Book *b = &Books[recIdx[i]];
        printf("| %-20s | %-26s | %-24s | %-4d | %-4d |\n",
            b->ISBN, b->name, b->author, b->total, b->borrowed);
    }
    printf("+----------------------+----------------------------+--------------------------+------+------+\n");
}

/* 搜索并分页显示图书，同时展示推荐书目
 * 是 searchBooksToArray + showBookList + showRecommendations 的组合
 * @param input[] 用户输入的关键词字符串 */
void showBooks(const char input[]) {
    int match[MAX_BOOK];
    int matchCount = searchBooksToArray(input, match);
    showBookList(match, matchCount, 0);

    /* 展示推荐书目 */
    showRecommendations(input);
}

/* ===== 排行榜 ===== */

/* 展示借阅图书排行榜：按每本书被借阅的总次数降序排列
 * 统计所有记录中各ISBN出现的次数，排序后以表格展示 */
void showBookRanking(void) {
    int ranked[MAX_BOOK];
    int count = getBookBorrowRanking(ranked);

    if (count == 0) {
        printf("\n暂无借阅记录，无法生成排行榜。\n");
        return;
    }

    /* 重新计算每个排名图书的借阅次数用于展示 */
    int borrowCount[MAX_BOOK] = {0};
    for (int i = 0; i < recCount; i++) {
        for (int j = 0; j < book_count; j++) {
            if (strcmp(recs[i].ISBN, Books[j].ISBN) == 0) {
                borrowCount[j]++;
                break;
            }
        }
    }

    printf("\n+============================================+\n");
    printf("|              ★ 借阅图书排行榜 ★              |\n");
    printf("+======+----------------------+----------------------------------+--------------------------+------------+\n");
    printf("| 排名 |         编号         |              书  名              |          作 者           | 借阅次数   |\n");
    printf("+======+----------------------+----------------------------------+--------------------------+------------+\n");
    for (int i = 0; i < count; i++) {
        Book *b = &Books[ranked[i]];
        printf("| %-4d | %-20s | %-32s | %-24s | %-10d |\n",
            i + 1, b->ISBN, b->name, b->author, borrowCount[ranked[i]]);
    }
    printf("+======+----------------------+----------------------------------+--------------------------+------------+\n");
}

/* 展示借阅人排行榜：按每个借阅人借阅总次数降序排列
 * 统计所有记录中各borrower出现的次数，排序后以表格展示 */
void showBorrowerRanking(void) {
    if (recCount == 0) {
        printf("\n暂无借阅记录，无法生成排行榜。\n");
        return;
    }

    /* 收集所有唯一的借阅人姓名并统计次数 */
    char borrowers[MAX_RECORD][NAME_LEN];
    int borrowCount[MAX_RECORD] = {0};
    int borrowerCount = 0;

    for (int i = 0; i < recCount; i++) {
        /* 查找该borrower是否已在列表中 */
        int found = -1;
        for (int j = 0; j < borrowerCount; j++) {
            if (strcmp(borrowers[j], recs[i].borrower) == 0) {
                found = j;
                break;
            }
        }
        if (found >= 0) {
            borrowCount[found]++;
        } else {
            strcpy(borrowers[borrowerCount], recs[i].borrower);
            borrowCount[borrowerCount] = 1;
            borrowerCount++;
        }
    }

    /* 按借阅次数降序排序 */
    int rankIdx[MAX_RECORD];
    for (int i = 0; i < borrowerCount; i++) rankIdx[i] = i;
    for (int i = 0; i < borrowerCount - 1; i++) {
        for (int j = 0; j < borrowerCount - 1 - i; j++) {
            if (borrowCount[rankIdx[j]] < borrowCount[rankIdx[j + 1]]) {
                int tmp = rankIdx[j];
                rankIdx[j] = rankIdx[j + 1];
                rankIdx[j + 1] = tmp;
            }
        }
    }

    printf("\n+============================================+\n");
    printf("|              ★ 借阅人排行榜 ★                |\n");
    printf("+======+----------------------------------+------------+\n");
    printf("| 排名 |            借阅人姓名             |  借阅次数   |\n");
    printf("+======+----------------------------------+------------+\n");
    for (int i = 0; i < borrowerCount; i++) {
        int idx = rankIdx[i];
        printf("| %-4d | %-32s | %-10d |\n",
            i + 1, borrowers[idx], borrowCount[idx]);
    }
    printf("+======+----------------------------------+------------+\n");
}

/* ===== 借阅记录分页展示 ===== */

/* 以分页表格形式显示借阅记录列表
 * @param recIdx[]      记录在recs[]中的下标数组
 * @param recCountList  recIdx[]数组的长度
 * @param showIndex     是否显示序号列（0=不显示，1=显示）
 * @return int          实际显示的记录数 */
int showRecordList(int recIdx[], int recCountList, int showIndex) {
    if (recCountList == 0) {
        printf("未找到相关借阅记录\n");
        return 0;
    }

    int totalPage = (recCountList + MAX_PAGE_ITEMS - 1) / MAX_PAGE_ITEMS;
    int currentPage = 0;

    while (1) {
        int start = currentPage * MAX_PAGE_ITEMS;
        int end = (start + MAX_PAGE_ITEMS > recCountList) ? recCountList : start + MAX_PAGE_ITEMS;
        if (showIndex)
            printf("\n+------+----------+----------------------+----------------------------------+----------------------+------------+------------+----------+\n");
        else
            printf("\n+----------+----------------------+----------------------------------+----------------------+------------+------------+----------+\n");
        if (showIndex)
            printf("| 序号 |  记录ID  |         ISBN         |              书  名              |        借阅人        |  借阅日期  |  应还日期  |   状态   |\n");
        else
            printf("|  记录ID  |         ISBN         |              书  名              |        借阅人        |  借阅日期  |  应还日期  |   状态   |\n");
        if (showIndex)
            printf("+------+----------+----------------------+----------------------------------+----------------------+------------+------------+----------+\n");
        else
            printf("+----------+----------------------+----------------------------------+----------------------+------------+------------+----------+\n");

        for (int i = start; i < end; i++) {
            Record *r = &recs[recIdx[i]];
            char bookName[NAME_LEN] = "未知";
            for (int j = 0; j < book_count; j++) {
                if (strcmp(Books[j].ISBN, r->ISBN) == 0) {
                    strcpy(bookName, Books[j].name);
                    break;
                }
            }
            char *statusStr;
            if (r->status == 0) statusStr = "已借出";
            else if (r->status == 1) statusStr = "已归还";
            else statusStr = "逾期";

            if (showIndex)
                printf("| %-4d | %-8ld | %-20s | %-32s | %-20s | %04d-%02d-%02d | %04d-%02d-%02d | %-8s |\n",
                    i + 1, r->id, r->ISBN, bookName, r->borrower,
                    r->borrowDate.year, r->borrowDate.month, r->borrowDate.day,
                    r->dueDate.year, r->dueDate.month, r->dueDate.day, statusStr);
            else
                printf("| %-8ld | %-20s | %-32s | %-20s | %04d-%02d-%02d | %04d-%02d-%02d | %-8s |\n",
                    r->id, r->ISBN, bookName, r->borrower,
                    r->borrowDate.year, r->borrowDate.month, r->borrowDate.day,
                    r->dueDate.year, r->dueDate.month, r->dueDate.day, statusStr);
        }
        if (showIndex)
            printf("+------+----------+----------------------+----------------------------------+----------------------+------------+------------+----------+\n");
        else
            printf("+----------+----------------------+----------------------------------+----------------------+------------+------------+----------+\n");
        printf("第 %d/%d 页  共 %d 条结果\n", currentPage + 1, totalPage, recCountList);

        if (currentPage < totalPage - 1) {
            printf("按 Enter 键进入下一页，按 q 退出分页：");
            int ch;
            while (1) {
                ch = getchar();
                if (ch == '\n') {                   
                    currentPage++;
                    break;                          
                } else if (ch == 'q' || ch == 'Q' || ch == EOF) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF); 
                    break;
                } else {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("输入无效，请按 Enter 进入下一页，或按 q 退出：");
                }
            }
            if (ch == 'q' || ch == 'Q' || ch == EOF) {
                break;
            }
        }else {
            printf("已经是最后一页。\n");
            break;
        }
    }
    return recCountList;
}

/* ===== 获取指定用户的借阅记录 ===== */

/* 筛选借阅记录，将符合条件的记录下标填入 recIdx[]
 * @param borrower[]    借阅人姓名（NULL 表示所有用户）
 * @param statusFilter  状态过滤（-1=全部，0=已借出，1=已归还，2=逾期）
 * @param recIdx[]      输出：符合条件的记录在recs[]中的下标数组
 * @return int          符合条件的记录数量 */
int getUserRecords(const char borrower[], int statusFilter, int recIdx[]) {
    int count = 0;
    for (int i = 0; i < recCount; i++) {
        if (borrower != NULL && strcmp(recs[i].borrower, borrower) != 0)
            continue;
        if (statusFilter != -1 && recs[i].status != statusFilter)
            continue;
        recIdx[count++] = i;
    }
    return count;
}

/* ===== 借书 ===== */

/* 借阅图书流程：搜索→选择→确认借出
 * @param borrower[] 借阅人的姓名 */
void borrowBook(char borrower[]) {
    char kwd[256];
    printf("请输入书目关键词（多个关键词之间用空格隔开）：");
    fgets(kwd, 256, stdin);
    kwd[strcspn(kwd, "\n")] = '\0';

    if (kwd[0] == '\0') {
        printf("关键词不能为空！\n");
        return;
    }

    int match[MAX_BOOK];
    int matchCount = searchBooksToArray(kwd, match);

    if (matchCount == 0) {
        printf("未找到相关图书！\n");
        return;
    }

    showBookList(match, matchCount, 1);

    printf("\n请输入要借阅的图书序号(1-%d),输入0取消:", matchCount);
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("输入无效！\n");
        return;
    }
    while (getchar() != '\n');

    if (choice == 0) {
        printf("已取消借阅。\n");
        return;
    }
    if (choice < 1 || choice > matchCount) {
        printf("序号无效！\n");
        return;
    }

    int bookIdx = match[choice - 1];
    Book *selected = &Books[bookIdx];

    if (selected->borrowed >= selected->total) {
        printf("该书已全部借出，暂无可借库存！\n");
        return;
    }

    for (int i = 0; i < recCount; i++) {
        if (strcmp(recs[i].ISBN, selected->ISBN) == 0 &&
            strcmp(recs[i].borrower, borrower) == 0 &&
            recs[i].status == 0) {
            printf("您已借阅该书且尚未归还，不能重复借阅！\n");
            return;
        }
    }

    if (recCount >= MAX_RECORD) {
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

/* ===== 还书 ===== */

/* 归还图书流程：列出当前用户借阅中的书→选择→归还
 * @param borrower[] 借阅人的姓名 */
void returnBook(char borrower[]) {
    int recIdx[MAX_RECORD];
    int recCountList = getUserRecords(borrower, 0, recIdx);

    if (recCountList == 0) {
        printf("您当前没有借阅中的图书。\n");
        return;
    }

    printf("\n===== 您的借阅记录 =====");
    showRecordList(recIdx, recCountList, 1);

    printf("\n请输入要归还的记录序号(1-%d),输入0取消:", recCountList);
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("输入无效！\n");
        return;
    }
    while (getchar() != '\n');

    if (choice == 0) {
        printf("已取消归还。\n");
        return;
    }
    if (choice < 1 || choice > recCountList) {
        printf("序号无效！\n");
        return;
    }

    int recIndex = recIdx[choice - 1];
    Record *r = &recs[recIndex];

    r->returnDate = today();
    r->status = 1;

    for (int i = 0; i < book_count; i++) {
        if (strcmp(Books[i].ISBN, r->ISBN) == 0) {
            Books[i].borrowed--;
            break;
        }
    }

    int overdueDays = diffDays(r->dueDate, r->returnDate);

    saveAll();

    printf("\n归还成功!\n");
    printf("借阅编号：%ld\n", r->id);
    printf("归还日期：%d-%02d-%02d\n", r->returnDate.year, r->returnDate.month, r->returnDate.day);
    if (overdueDays > 0) {
        printf(" 已逾期 %d 天!\n", overdueDays);
    } else {
        printf(" 按时归还\n");
    }
}

/* ===== 续借 ===== */

/* 续借图书流程：列出当前用户借阅中的书→选择→延长应还日期30天
 * @param borrower[] 借阅人的姓名 */
void renewBook(char borrower[]) {
    int recIdx[MAX_RECORD];
    int recCountList = getUserRecords(borrower, 0, recIdx);

    if (recCountList == 0) {
        printf("您当前没有可续借的图书。\n");
        return;
    }

    printf("\n===== 可续借的图书 =====");
    showRecordList(recIdx, recCountList, 1);

    printf("\n请输入要续借的记录序号(1-%d),输入0取消:", recCountList);
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("输入无效！\n");
        return;
    }
    while (getchar() != '\n');

    if (choice == 0) {
        printf("已取消续借。\n");
        return;
    }
    if (choice < 1 || choice > recCountList) {
        printf("序号无效！\n");
        return;
    }

    int recIndex = recIdx[choice - 1];
    Record *r = &recs[recIndex];

    Date now = today();
    if (diffDays(r->dueDate, now) > 0) {
        printf("该书已逾期，无法续借，请先归还！\n");
        return;
    }

    r->dueDate = addDays(r->dueDate, 30);

    saveAll();

    printf("\n续借成功!\n");
    printf("借阅编号：%ld\n", r->id);
    printf("新的应还日期：%d-%02d-%02d\n", r->dueDate.year, r->dueDate.month, r->dueDate.day);
}

/* ===== 自动检测逾期 ===== */

/* 遍历所有借阅记录，若已超过应还日期且尚未归还(status=0)，则标记为逾期(status=2)
 * @return int 本次新标记为逾期的记录数量 */
int checkOverdue(void) {
    Date now = today();
    int newOverdue = 0;
    for (int i = 0; i < recCount; i++) {
        if (recs[i].status != 0) continue;
        if (diffDays(recs[i].dueDate, now) > 0) {
            recs[i].status = 2;
            newOverdue++;
        }
    }
    if (newOverdue > 0) {
        saveAll();
    }
    return newOverdue;
}




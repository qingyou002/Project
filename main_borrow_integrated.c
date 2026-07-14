#include <stdio.h>
#include <string.h>

#include "register.h"//注册
#include "Change_Password.h"//修改密码
#include "borrow_book.h"//借书模块
#include "login.h"
#include "menu.h"
#include "book.h"//包含



void clear_line_after_scanf(void)//清除缓冲区的\n
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void show_admin_borrow_records(void)//给管理员 展示借阅日志的全部
{
    int recIdx[MAX_RECORD];
    int count = getUserRecords(NULL, -1, recIdx);
    showRecordList(recIdx, count, 0);
}

void show_user_records_menu(const char account[])//展示个人的借阅记录菜单
{
    int recIdx[MAX_RECORD];
    int count;
    user_records_menu();
    printf("Please input: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        clear_line_after_scanf();
        printf("Invalid input.\n");
        return;
    }
    clear_line_after_scanf();

    switch (choice) {
    case 1:
        printf("当前借阅书本:\n");
        count = getUserRecords(account, 0, recIdx);//
        showRecordList(recIdx, count, 0);
        break;
    case 2:
        printf("已经还的书本:\n");
        count = getUserRecords(account, 1, recIdx);
        showRecordList(recIdx, count, 0);
        break;
    case 3:
        printf("已逾期书本:\n");
        count = getUserRecords(account, 2, recIdx);
        showRecordList(recIdx, count, 0);
        break;
    case 4:
        printf("历史全部借阅记录:\n\n");
        count = getUserRecords(account, -1, recIdx);
        showRecordList(recIdx, count, 0);
        break;
    case 0:
        break;
    default:
        printf("Invalid option.\n");
        break;
    }
}

void search_books_from_input(void)
{
    char keyword[256];

    printf("Please input book keyword, or press Enter to show all: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL) {
        showBooks(NULL);
        return;
    }
    keyword[strcspn(keyword, "\n")] = '\0';

    if (keyword[0] == '\0') {
        showBooks(NULL);
    } else {
        showBooks(keyword);
    }
}

void admin_loop_integrated(void)
{
    while (1) {
        menu_admin_total();
        

        char user_name[MAX_LEN];
        int temp;
        printf("Please input: ");
        if (scanf("%d", &temp) != 1) {
            clear_line_after_scanf();
            printf("Invalid input.\n");
            continue;
        }
        clear_line_after_scanf();

        switch (temp) {
        case 1:
            register_user();
            break;
        case 2:
            printf("Delete user name: ");
            scanf("%99s", user_name);
            clear_line_after_scanf();
            del_user(user_name);
            break;
        case 3:
            add_book();
            clear_line_after_scanf();
            loadAll();
            break;
        case 4:
            show_admin_borrow_records();
            break;
        case 5:
            showBooks(NULL);
            break;
        case 6:
            Change_Password();
            clear_line_after_scanf();
            break;
        case 7:
            loadAll();
            break;
        case 0:
            return;
        default:
            printf("Invalid option.\n");
            break;
        }
    }
}

void user_loop_integrated(const char account[])
{
    while (1) {
        int temp;

        menu_user_total();
        
        printf("Please input: ");

        if (scanf("%d", &temp) != 1) {
            clear_line_after_scanf();
            printf("Invalid input.\n");
            continue;
        }
        clear_line_after_scanf();

        switch (temp) {
        case 1:
            borrowBook((char *)account);
            break;
        case 2:
            show_user_records_menu(account);
            break;
        case 4:
            Change_Password();
            clear_line_after_scanf();
            break;
        case 3:
            returnBook((char *)account);
            break;
        case 5:
            renewBook((char *)account);
            break;
        case 6:
            showBooks(NULL);
            break;
        case 7:
            search_books_from_input();
            break;
        case 0:
            return;
        default:
            printf("Invalid option.\n");
            break;
        }
    }
}

int main(void)
{
    int Program_t1;

    loadAll();

    while (1) {
        char account[MAX_LEN];
        char password[MAX_LEN];
        char text_account[MAX_LEN];
        char text_password[MAX_LEN];

        Program_t1 = 0;
        menulogin();
        if (scanf("%d", &Program_t1) != 1) {
            clear_line_after_scanf();
            printf("Invalid input.\n");
            continue;
        }
        clear_line_after_scanf();

        if (Program_t1 == 1) {
            while (1) {
                login(account, password);
                read_account(account, text_account, text_password);
                if (compare(account, password, text_account, text_password) == 1) {
                    break;
                }
            }
            admin_loop_integrated();
        } else if (Program_t1 == 2) {
            while (1) {
                login(account, password);
                read_account(account, text_account, text_password);
                if (compare(account, password, text_account, text_password) == 1) {
                    break;
                }
            }
            user_loop_integrated(account);
        } else if (Program_t1 == 3) {
            register_user();
            clear_line_after_scanf();
        } else if (Program_t1 == 0) {
            printf("Exit system.\n");
            return 0;
        } else {
            printf("Invalid option.\n");
        }
    }
}

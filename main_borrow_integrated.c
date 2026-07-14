#include <stdio.h>
#include <string.h>

#include "login.h"
#include "menu.h"

#define Book CsvBook
#include "book.h"
#undef Book

#include "register.h"
#include "Change_Password.h"
#include "borrow_book.h"

static void clear_line_after_scanf(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void show_admin_borrow_records(void)
{
    int recIdx[MAX_RECORD];
    int count = getUserRecords(NULL, -1, recIdx);
    showRecordList(recIdx, count, 0);
}

static void show_user_records_menu(const char account[])
{
    int recIdx[MAX_RECORD];
    int count;

    printf("\n1. Current borrowed records\n");
    printf("2. Returned records\n");
    printf("3. Overdue records\n");
    printf("4. All my records\n");
    printf("0. Back\n");
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
        count = getUserRecords(account, 0, recIdx);
        showRecordList(recIdx, count, 0);
        break;
    case 2:
        count = getUserRecords(account, 1, recIdx);
        showRecordList(recIdx, count, 0);
        break;
    case 3:
        count = getUserRecords(account, 2, recIdx);
        showRecordList(recIdx, count, 0);
        break;
    case 4:
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

static void search_books_from_input(void)
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

static void admin_loop_integrated(void)
{
    while (1) {
        menu_admin_total();
        printf("\t| 7. Borrow-system book list\n");
        printf("\t| 8. Borrow-system record list\n");
        printf("\t| 9. Reload borrow-system files\n");

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
            break;
        case 4:
            show_admin_borrow_records();
            break;
        case 5:
            see_all_books();
            break;
        case 6:
            Change_Password();
            clear_line_after_scanf();
            break;
        case 7:
            showBooks(NULL);
            break;
        case 8:
            show_admin_borrow_records();
            break;
        case 9:
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

static void user_loop_integrated(const char account[])
{
    while (1) {
        int temp;

        menu_user_total();
        printf("\t| 4. Return book\n");
        printf("\t| 5. Renew book\n");
        printf("\t| 6. Show all borrow-system books\n");
        printf("\t| 7. Search borrow-system books\n");
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
        case 3:
            Change_Password();
            clear_line_after_scanf();
            break;
        case 4:
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

#include <stdio.h>
#include"Visual_common.h"

//---------用户主菜单-----------//
void menu_user_total(void)
{
    menu_begin("学校图书管理系统");
    menu_item(1, "搜索书本");
    menu_item(2, "个人借阅查询");
    menu_item(0, "退出管理系统");
    menu_end();
}

//---------图书信息查询-----------//


//---------个人借阅查询-----------//
// void menu_user_searchuser(struct Userinfo *user, int n)
// {
//     int i;

//     menu_begin("学校图书管理系统(个人借阅查询)");

//     printf("\t| 姓名：%s  学号：%s\n", user[n].name, user[n].number);
//     printf("\t| 已借阅书目：%d本  还可借阅数目：%d本\n",
//            user[n].borrowed_count,
//            user[n].can_borrow_count);

//     printf("\t| 接近逾期数目：%d  已逾期数目：%d\n",
//            user[n].nearly_overdue_count,
//            user[n].overdue_count);

//     for (i = 0; i < user[n].borrowed_count; i++)
//     {
//         printf("\t| 已借阅书目%d：%s\n", i + 1, user[n].borrowed_books[i]);
//     }

//     menu_item(1, "续借");
//     menu_item(2, "归还");
//     menu_item(3, "返回上层菜单");

//     menu_end();
// }

// //---------续借-----------//
// void menu_user_renew(void)
// {
//     menu_begin("学校图书管理系统(续借)");
//     menu_item(1, "返回上层菜单");
//     menu_text("输入续借图书编号：");
//     menu_end();
// }

// //---------归还-----------//
// void menu_user_return(void)
// {
//     menu_begin("学校图书管理系统(归还)");
//     menu_item(1, "返回上层菜单");
//     menu_text("输入归还图书编号：");
//     menu_end();
// }

// //---------借阅-----------//
// void menu_user_borrow(void)
// {
//     menu_begin("学校图书管理系统(借阅)");
//     menu_item(1, "返回上层菜单");
//     menu_text("输入借阅图书编号：");
//     menu_end();
// }
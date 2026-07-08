#include <stdio.h>
#include "Visual_common.h"
//---------------管理员主菜单----------------//
void menu_admin_total(void)
{
    menu_begin("学校图书管理系统");
    menu_item(1, "添加用户");
    menu_item(2, "删除用户");
    menu_item(3, "添加书本");
    menu_item(4, "查阅借阅");
    menu_item(0, "退出系统");

    menu_end();
}
//---------------管理员主菜单----------------//

//---------用户管理菜单-----------//
void menu_admin_user(void)
{
    menu_begin("学校图书管理系统(用户管理)");
    menu_item(1, "添加用户");
    // menu_item(2, "删除用户");
    menu_item(3, "退出");
    menu_end();
}
void menu_admin_user_add(void)
{
    menu_begin("学校图书管理系统(添加用户)");
    menu_text("请输入要添加的用户信息：");//复用注册
    menu_end();
}

void menu_admin_user_del(void)
{
    menu_begin("学校图书管理系统(删除用户)");
    menu_text("请输入要删除的用户学号：");
    menu_end();
}
//---------用户管理菜单-----------//

//------------图书管理菜单----------//
void menu_admin_book(void)
{
    menu_begin("学校图书管理系统(图书管理)");
    menu_item(1, "添加图书");
    // menu_item(2, "删除图书");
    menu_item(3, "返回上层菜单");
    menu_end();
}
void menu_admin_book_add(void)
{
    menu_begin("学校图书管理系统(添加图书)");
    menu_text("请输入要添加的图书信息：");//long bookId;char name[NAME_LEN];char author[NAME_LEN];
    menu_end();
}
void menu_admin_book_del(void)
{
    menu_begin("学校图书管理系统(删除图书)");
    menu_text("请输入要删除的图书编号：");
    menu_end();
}
//------------图书管理菜单----------//

//------------借阅管理菜单----------//
void menu_admin_borrow(void)
{
    menu_begin("学校图书管理系统(借阅管理)");
    menu_item(1, "查询借阅记录");
    menu_item(2, "返回上层菜单");
    menu_end();
}
void menu_admin_borrow_query(void)
{
    menu_begin("学校图书管理系统(查询借阅记录)");
    menu_text("1.请输入要查询的用户学号：");
    menu_text("2.请输入要查询的图书编号：");
    menu_end();
}
#include <stdio.h>
#ifndef VISUAL_COMMON_H
#define VISUAL_COMMON_H

#define LINE_STAR "*********************"
#define LINE_DASH "--------------------"

static void menu_begin(const char *title)
{
    printf("\n\n");
    printf("\t| %s\n", LINE_STAR);
    printf("\t| %s\n", title);
    printf("\t| %s\n", LINE_DASH);
}

static void menu_item(int index, const char *text)
{
    printf("\t| %d. %s\n", index, text);
}

static void menu_text(const char *text)
{
    printf("\t| %s\n", text);
}

static void menu_end(void)
{
    printf("\t| ");
    printf("\n");
}

#endif//通用格式模块

//登录页面
void menulogin(void)
{
    printf("\n");
    printf("\t| ********************* \n");
    printf("\t| 学校图书管理系统 \n");
    printf("\t| --------------------\n");
    printf("\t| 1. 管理员登录 \n");
    printf("\t| 2. 用户登录 \n");
    printf("\t| 3. 账户注册 \n");
    printf("\t| 0. 退出系统 \n");
    printf("\t| ********************* \n");
    printf("\n请输入:");
}

//---------------管理员主菜单----------------//
void menu_admin_total(void)
{
    menu_begin("学校图书管理系统");
    menu_item(1, "添加用户");
    menu_item(2, "删除用户");
    menu_item(3, "添加书本");
    menu_item(4, "查阅借阅");//
    menu_item(5, "查看所有书籍");
    menu_item(6, "修改管理员密码");

    menu_item(0, "退出系统");

    menu_end();
}

//---------用户主菜单-----------//
void menu_user_total(void)
{
    menu_begin("学校图书管理系统");
    menu_item(1, "搜索书本");//
    menu_item(2, "个人借阅查询");//
    menu_item(3, "修改个人密码");
    menu_item(0, "退出管理系统");
    menu_end();
}
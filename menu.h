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
    menu_item(4, "查阅全部借阅记录");//
    menu_item(5, "查看所有书籍");
    menu_item(6, "修改管理员密码");
    menu_item(7, "刷新借阅记录");
    menu_item(0, "退出系统");

    menu_end();
}

//---------用户主菜单-----------//
void menu_user_total(void)
{
    menu_begin("学校图书管理系统");
    menu_item(1, "借书");//
    menu_item(2, "个人借阅查询");//等下不用了 
    menu_item(3, "归还");//
    menu_item(4, "修改个人密码");
    menu_item(5, "续借");
    menu_item(6, "展示书库所有书");
    menu_item(7, "搜书");
    menu_item(0, "退出管理系统");
    menu_end();
}

void user_records_menu()
{
    printf("\n\n");
    menu_item(1, "当前借阅记录");//
    menu_item(2, "还书情况查询");//
    menu_item(3, "逾期书目查询");//
    menu_item(4, "我的全部借阅记录");//
    menu_item(0, "退出登录");//

}
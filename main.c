#include<stdio.h>
#include"login.h"
#include"menu.h"
#include"book.h"
#include"register.h"
#include"Change_Password.h"

int main()
{
    int Program_t1;
    while(1)//总体程序循环
    {
        Program_t1=0;
        menulogin();
        scanf("%d",&Program_t1);

        char account[MAX_LEN];
        char password[MAX_LEN];//分别为用户输入的账号密码
        char text_account[MAX_LEN];
        char text_password[MAX_LEN];//分别为文档内的账号密码

        if(Program_t1==1)
        {
            //管理员登录
            while(1){
                login(account, password);
                read_account(account,text_account,text_password);
                if(compare(account,password,text_account,text_password)==1)
                break;
            }
            
            //登录后
            while(1)
            {
                menu_admin_total();
                char user_name[MAX_LEN];
                int temp;
                printf("请输入：");
                scanf("%d",&temp);
                switch (temp)
                {
                case 1:{
                    register_user();
                    break;

                }
                case 2:{
                    printf("删除的用户名:");
                    scanf("%s",user_name);
                    del_user(user_name);
                    break;

                }
                case 3:{
                    //添加书本
                    add_book();
                }
                
                case 4:{
                    //查阅借阅还书日志
                    break;
                }

                case 5:{
                    //查看所有图书
                    see_all_books();
                    break;
                }

                case 6:{
                    Change_Password();
                    break;
                }
                case 0:{
                    return 0;
                    break;
                }
                default:
                    break;
                }
            }
        }
        else if(Program_t1==2)
        {
            //用户登录
            while(1){
                login(account, password);
                read_account(account,text_account,text_password);
                if(compare(account,password,text_account,text_password)==1)
                break;
            }
            //登录后界面
            int temp;
            menu_user_total();
            printf("请输入:");
            scanf("%d",&temp);
            switch (temp)
            {
            case 1:
                /* code */
                break;
            case 2:{
                //个人借阅查询（借了哪些书）
            }

                break;
            case 3:{
                 Change_Password();
                 break;
            }

            default:
                printf("输入有误");
                break;
            }
            
        }
        else if(Program_t1==3)
        {
            //账户注册
            register_user();
        }
        else if(Program_t1==0)
        {
            printf("程序已退出\n");
            return 0;
        }
        else {
            printf("输入有误，重新输入:");
        }
    }
    
    return 0;
}
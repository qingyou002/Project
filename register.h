#include<stdio.h>
#include<string.h>
#define MAX_LEN 100

void register_user(){
    char account[MAX_LEN], password[MAX_LEN];
    printf("请输入账户名:");
    scanf("%s",account);
    printf("请输入密码:");
    scanf("%s",password);
    char file_name[MAX_LEN];//文件名字
    // 1. 打开文件
    char txt[] = ".txt";
    FILE *fp = fopen("")
}
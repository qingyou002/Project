#include<stdio.h>
#include<string.h>
#define MAX_LEN 100

void register_user(void){
    char account[MAX_LEN], password[MAX_LEN];
    char total_file_name[MAX_LEN]={"account/"};
    printf("请输入账户名:");
    scanf("%s",account);
    printf("请输入密码:");
    scanf("%s",password);
    char file_name[MAX_LEN];//文件名字
    // 1. 打开文件
    char txt[] = ".txt";
    //snprintf(file_name, MAX_LEN, "%s.txt", account);
    strcpy(file_name, account);
	strcat(file_name, txt);
    strcat(total_file_name,file_name);
    FILE *fp = fopen(total_file_name,"w");
    if(!fp){
        printf("文件打开失败\n");
        return;
    }
    else printf("注册成功！");
    fprintf(fp,"%s\n%s\n",account,password);
    fclose(fp);
}


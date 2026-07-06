#include<stdio.h>
#include<string.h>
#define MAX_LEN 100

void read_account(char *input_account, char *text_account,char *text_password)
{
    FILE *fp;
    char account[MAX_LEN];
    char password[MAX_LEN];
    char buffer[MAX_LEN];   // 临时缓冲区
    char file_name[MAX_LEN];//文件名字
    // 1. 打开文件
    char txt[] = ".txt";
    strcpy(file_name,input_account);
    strcat(file_name,txt);
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("failed open file");
        
    }

    // 2. 读取第一行（账号）
    if (fgets(account, sizeof(account), fp) == NULL) {
        printf("filed read account\n");
        fclose(fp);
        
    }
    // 去除末尾的换行符 '\n'
    account[strcspn(account, "\n")] = '\0';

    // 3. 读取第二行（密码）
    if (fgets(password, sizeof(password), fp) == NULL) {
        printf("filed read password\n");
        fclose(fp);
        
    }
    password[strcspn(password, "\n")] = '\0';

    fclose(fp);
    strcpy(text_account,account);
    strcpy(text_password,password);
}

void before_login_menu()
{
    printf("Welcome to electric library\n");
}

void login(char *account,char *password)
{
    before_login_menu();
    printf("Please input account:");
    scanf("%s",account);
    printf("Please input password:");
    scanf("%s",password);
}

int compare(char* account,char* password,char *text_account, char *text_password)
{
    if(strcmp(account,text_account)==0&&strcmp(password,text_password)==0)
    {
        return 1;//成功匹配，进入系统
    }
    else{
        printf("account mistake or password mistake\nPlease inpute again\n");
        return 0;
    } 

}

int main()
{
    char account[MAX_LEN];
    char password[MAX_LEN];//分别为用户输入的账号密码
    char text_account[MAX_LEN];
    char text_password[MAX_LEN];//分别为文档内的账号密码
    login(account, password);
    // printf("account:%s",account);
    // printf("password:%s",password);
    read_account(account,text_account,text_password);
    compare(account,password,text_account,text_password);
    
    return 0;
}
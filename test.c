#define MAX_LEN 100
#include<stdio.h>
#include<string.h>
void read_account()
{
    FILE *fp;
    char account[MAX_LEN];
    char password[MAX_LEN];
    char buffer[MAX_LEN];   // 临时缓冲区
    char file_name[MAX_LEN] ={"admin.txt"};//文件名字
    // 1. 打开文件
    // char txt[] = ".txt";
    // strcat(text_account,txt);
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

    // 4. 输出验证（你可以在此处使用 account 和 password 变量）
    printf("account: %s\n", account);
    printf("password: %s\n", password);

    // 接下来你就可以把 account 和 password 用于登录、验证等操作了
}

int main()
{
    read_account();
}
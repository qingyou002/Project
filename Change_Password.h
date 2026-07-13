#include<stdio.h>
#include<string.h>

#define MAX_LEN 100

void Change_Password(void)
{
    FILE *fp;
    char account[MAX_LEN];
    char new_password[MAX_LEN];
    char file_name[MAX_LEN];
    char text_account[MAX_LEN];
    char lines[2][MAX_LEN];
    int line_account = 0;

    //获取账户名与新密码
    printf("Enter your account(name):");
    scanf("%s", account);
    printf("Enter your new password:");
    scanf("%s", new_password);

    //打开账户文件
    char txt[]=".txt";
    char total_file_name[MAX_LEN]={"account/"};
    strcpy(file_name, account);
    strcat(file_name, txt);
    strcat(total_file_name,file_name);
    fp=fopen(total_file_name, "r");
    if(fp==NULL)
    {
        printf("Error: Account file not found!\n");
        return;
    }
    fclose(fp);
    fp=fopen(total_file_name, "w");
    fprintf(fp, "%s\n%s", account,new_password);
    fclose(fp);
    printf("Password changed successfully!\n");
}
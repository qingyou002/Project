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

#endif
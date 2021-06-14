#include "first_pass.h"
#include "parser.h"

int islabel(char *s)
{
    if (!isalpha(*s))
        return -1;
    while (isalpha(*s))
        s++;
    if (*s != ':')
        return -2;
    if (*(s + 1))
        return -3;
    return 0;
}

int isdata(char *s)
{
    if ((strcmp(s, ".dh") == 0 || strcmp(s, ".dw") == 0 || strcmp(s, ".db") == 0 || strcmp(s, ".asciz") == 0))
        return 1;

    return 0;
}

int first_pass(const char filename[MAX_LINE_LENGTH])
{
    FILE *file;
    Line line;
    Node ptr;
    size_t size = 0;
    size_t nread;
    char mline[MAX_LINE_LENGTH];
    char *tok;
    char *text;
    int nline = 1;
    int labelflag;

    /* char _filename[MAX_LINE_LENGTH]; */
    /* strcpy(_filename, filename); */

    LinkedList code_image = initList();
    /* LinkedList data_image = initList();
    
    int DC = 0;
    int IC = START_ADDRESS; */
    
    if ((file = fopen(filename, "r")) == NULL)
    {
        return -1;
    }

    while ((nread = getline(&text, &size, file)) != -1)
    {
        labelflag = 0;
        if (nread > MAX_LINE_LENGTH)
        {
            printf("Line %d: line too long\n", nline);
        }
        strncpy(mline, text, MAX_LINE_LENGTH);
        tok = strtok(mline, SPACES);
        if (!tok || *tok == ';')
            continue;
        
        if ((labelflag = islabel(tok)) == 0)
            tok = strtok(NULL, SPACES);
        
        
        line = createLine(text);
        /* parseLine(&line); */
        ptr = createNode(line);
        pushNode(code_image, ptr);

        nline++;
    }
    print_list(code_image);

    free(text);
    return 0;
}


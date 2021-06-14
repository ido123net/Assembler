#include "parser.h"

/* void parseLine(Line *line)
{
    char *text;
    int type;
    strcpy(text, (*line)->text);
    while (isspace(*text))
        text++;
    if (*text == ';')
    {
        (*line)->type = COMMENT;
        return;
    }
    strsep(&text, SPACES);
} */
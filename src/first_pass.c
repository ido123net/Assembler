#include "first_pass.h"
#include "parser.h"
#include "linked_list.h"

int first_pass(const char filename[MAX_LINE_LENGTH],
               LinkedList data_image,
               LinkedList code_image,
               LinkedList symbol_table,
               int *ICF,
               int *DCF)
{
    FILE *file;
    char line[MAX_LINE_LENGTH + 1], tmp[MAX_LINE_LENGTH + 1];
    char label[MAX_SYMBOL_LENGTH], symbol[MAX_SYMBOL_LENGTH];
    int labelflag = FALSE, error_flag = FALSE;
    int data_type;
    int IC = START_ADDRESS;
    int DC = 0;
    size_t row = 0;
    Binary *binary;
    int arg;

    char *devided_line[NO_OF_ELEMENTS];

    if ((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return FALSE;
    }

    while (read_line(++row, line, MAX_LINE_LENGTH + 1, file) != NULL)
    {
        arg = 0;
        strcpy(tmp, line);
        str_strip(tmp);
        labelflag = FALSE;

        devided_line[arg] = strtok(tmp, SPACES);

        if (blank_line(devided_line[arg]) || comment_line(devided_line[arg])) /* if line is blank or comment, ignore it */
            continue;

        if (is_label(devided_line[arg])) /* if the first word of the line is label */
        {
            labelflag = TRUE; /* raise label flag */
            devided_line[++arg] = strtok(NULL, SPACES);
        }

        devided_line[(arg + 1)] = strtok(NULL, "\0");

        if (is_data(devided_line[arg])) /* Check if this is data instructions */
        {
            if (labelflag)
            {
                get_label(label, devided_line[0]); /* first argument in line is the label */
                if (!valid_symbol(row, label) ||
                    !add_symbol_to_symbol_table(row, label, symbol_table, DC, DATA))
                {
                    error_flag = TRUE;
                    continue;
                }
            }

            data_type = get_data_type(devided_line[arg++]);
            if (!valid_data(row, devided_line[arg], data_type))
                error_flag = TRUE;
            getdata(row, devided_line[arg], data_type, data_image, &DC);
            continue;
        }

        if (entry_line(devided_line[arg]))
        {
            if (labelflag)
                label_warning(row);

            add_last(code_image, initCodeLine(row, 0, line, init_Binary()));
            continue;
        }

        if (extern_line(devided_line[arg]))
        {
            if (labelflag)
                label_warning(row);

            get_symbol(symbol, devided_line[++arg]);
            if (!valid_symbol(row, symbol) ||
                !add_symbol_to_symbol_table(row, symbol, symbol_table, 0, EXTERNAL))
                error_flag = TRUE;

            continue;
        }

        if (labelflag)
        {
            get_label(label, devided_line[0]);
            if (!valid_symbol(row, label) ||
                !add_symbol_to_symbol_table(row, label, symbol_table, IC, CODE))
            {
                error_flag = TRUE;
                continue;
            }
        }

        binary = init_Binary();

        if (!analyzeoperands(row, devided_line[arg], devided_line[(arg + 1)], binary))
        {
            error_flag = TRUE;
            continue;
        }
        add_last(code_image, initCodeLine(row, IC, line, binary));
        IC += 4;
    }

    *ICF = IC;
    *DCF = DC;

    updateSymbolTable(*ICF, symbol_table);
    updateDataImage(*ICF, data_image);

    return !error_flag;
}

void getdata(size_t row, char *s, int data_type, LinkedList data_image, int *DC)
{
    char *tok, tmp[MAX_LINE_LENGTH];
    int value;

    if (data_type == ASCIZ)
    {
        s = str_strip(s);
        s++;
        while (*s != '"')
        {
            add_last(data_image, initDataLine((*DC)++, *s));
            s++;
        }
        add_last(data_image, initDataLine((*DC)++, '\0'));
    }
    else
    {
        strcpy(tmp, s);
        tok = strtok(tmp, ",");

        while (tok)
        {
            value = atoi(tok);
            add_data(data_image, DC, value, data_type);
            tok = strtok(NULL, ",");
        }
    }
}

char *read_line(size_t row, char *s, int n, FILE *stream)
{
    int len;
    char c;
    s = fgets(s, n + 1, stream);
    if (!s || (len = strlen(s)) == 0)
        return s;
    if (len > MAX_LINE_LENGTH)
        long_line_warning(row);
    
    if (s[len - 1] != '\n' && s[len - 1] != '\0')
    {
        c = getc(stream);
        while (c != '\n' && c != '\0')
            c = getc(stream);
    }
    s[len - 1] = '\0';
    return s;
}
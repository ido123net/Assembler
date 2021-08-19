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

    char *divided_line[NO_OF_ELEMENTS];

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

        divided_line[arg] = strtok(tmp, SPACES);

        if (blank_line(divided_line[arg]) || comment_line(divided_line[arg])) /* if line is blank or comment, ignore it */
            continue;

        if (is_label(divided_line[arg])) /* if the first word of the line is label */
        {
            labelflag = TRUE; /* raise label flag */
            divided_line[++arg] = strtok(NULL, SPACES);
        }

        divided_line[(arg + 1)] = strtok(NULL, "\0");

        if (is_data(divided_line[arg])) /* Check if this is data instructions */
        {
            if (labelflag)
            {
                get_label(label, divided_line[0]); /* first argument in line is the label */
                if (!valid_symbol(row, label) ||
                    !add_symbol_to_symbol_table(row, label, symbol_table, DC, DATA))
                {
                    error_flag = TRUE;
                    continue;
                }
            }

            data_type = get_data_type(divided_line[arg++]);
            if (!valid_data(row, divided_line[arg], data_type))
                error_flag = TRUE;
            getdata(row, divided_line[arg], data_type, data_image, &DC);
            continue;
        }

        if (entry_line(divided_line[arg]))
        {
            if (labelflag)
                label_warning(row);

            add_last(code_image, initCodeLine(row, 0, line, init_Binary()));
            continue;
        }

        if (extern_line(divided_line[arg]))
        {
            if (labelflag)
                label_warning(row);

            get_symbol(symbol, divided_line[++arg]);
            if (!valid_symbol(row, symbol) ||
                !add_symbol_to_symbol_table(row, symbol, symbol_table, 0, EXTERNAL))
                error_flag = TRUE;

            continue;
        }

        if (labelflag)
        {
            get_label(label, divided_line[0]);
            if (!valid_symbol(row, label) ||
                !add_symbol_to_symbol_table(row, label, symbol_table, IC, CODE))
            {
                error_flag = TRUE;
                continue;
            }
        }

        binary = init_Binary();

        if (!analyze_operands(row, divided_line[arg], divided_line[(arg + 1)], binary))
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

void getdata(size_t row, char *data, int data_type, LinkedList data_image, int *DC)
{
    char *tok, tmp[MAX_LINE_LENGTH];
    int value;

    if (data_type == ASCIZ)
    {
        data = str_strip(data);
        data++;
        while (*data != '"')
        {
            add_last(data_image, initDataLine((*DC)++, *data));
            data++;
        }
        add_last(data_image, initDataLine((*DC)++, '\0'));
    }
    else
    {
        strcpy(tmp, data);
        tok = strtok(tmp, ",");

        while (tok)
        {
            value = atoi(tok);
            add_data(data_image, DC, value, data_type);
            tok = strtok(NULL, ",");
        }
    }
}

char *read_line(size_t row, char *line, int line_length, FILE *stream)
{
    int len;
    char c;
    line = fgets(line, line_length + 1, stream);
    if (!line || (len = strlen(line)) == 0)
        return line;
    if (len > MAX_LINE_LENGTH)
        long_line_warning(row);
    
    if (line[len - 1] != '\n' && line[len - 1] != '\0')
    {
        c = getc(stream);
        while (c != '\n' && c != '\0')
            c = getc(stream);
    }
    line[len - 1] = '\0';
    return line;
}
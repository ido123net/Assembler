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
    char line[MAX_LINE_LENGTH], tmp[MAX_LINE_LENGTH];
    char label[MAX_SYMBOL_LENGTH], symbol[MAX_SYMBOL_LENGTH];
    int labelflag = FALSE;
    int data_type;
    int IC = START_ADDRESS;
    int DC = 0;
    size_t row_num = 0;
    Binary *binary;
    int arg;

    char *devided_line[NO_OF_ELEMENTS];

    if ((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return FALSE;
    }

    while (read_line(line, MAX_LINE_LENGTH + 1, file) != NULL)
    {
        row_num++;
        arg = 0;
        strcpy(tmp, line);
        str_strip(tmp);
        devided_line[arg] = strtok(tmp, SPACES);

        labelflag = FALSE;
        binary = init_Binary();

        /* TODO delete line: */
        /* printf("%s", line); */

        if (blank_line(devided_line[arg]) || comment_line(devided_line[arg])) /* if line is blank or comment, ignore it */
            continue;

        if (is_label(devided_line[arg])) /* if the first word of the line is label */
        {
            labelflag = TRUE; /* raise label flag */
            devided_line[++arg] = strtok(NULL, SPACES);
        }

        if (is_data(devided_line[arg])) /* Check if this is data instructions */
        {
            if (labelflag)
            {
                get_label(label, devided_line[0]); /* first argument in line is the label */
                if (errno = valid_symbol(label))
                {
                    symbol_error(row_num, label, errno);
                    continue;
                }
                add_symbol_to_symbol_table(label, symbol_table, DC, DATA);
            }

            data_type = get_data_type(devided_line[arg]);
            devided_line[++arg] = strtok(NULL, "\0");
            if (!valid_data(devided_line[arg], data_type))
            {
                /* TODO: error */
                continue;
            }
            if (!getdata(row_num, devided_line[arg], data_type, data_image, &DC, line))
            {
                fprintf(stderr, "Error malloc file: %s\n", strerror(errno));
                return FALSE;
            }
            continue;
        }

        if (entry_line(devided_line[arg]))
        {
            if (!add_last(code_image, initImageLine(row_num, NULL, line, binary, 0)))
                return FALSE;
            continue;
        }

        if (extern_line(devided_line[arg]))
        {
            if (labelflag)
                label_warning(row_num);

            devided_line[++arg] = strtok(NULL, "\0");
            get_symbol(symbol, devided_line[arg]);
            if (!(errno = is_symbol(symbol)) && !(errno = valid_symbol(symbol)))
                add_symbol_to_symbol_table(symbol, symbol_table, 0, EXTERNAL);
            else
                symbol_error(row_num, symbol, errno);
            continue;
        }

        if (labelflag)
        {
            get_label(label, devided_line[0]);
            if (errno = valid_symbol(label))
            {
                symbol_error(row_num, label, errno);
                continue;
            }

            if (get_symbol_line(symbol_table, label) != NULL)
            {
                symbol_error(row_num, label, LABEL_DECLARED);
                continue;
            }

            add_symbol_to_symbol_table(label, symbol_table, IC, CODE);
        }

        devided_line[++arg] = strtok(NULL, "\0");

        if (analyzeoperands(devided_line[(arg - 1)], devided_line[arg], binary) == 0)
        {
            /* TODO: Build a binary encoding for the instruction */
            /**
             * In part as much as possible.
             * Add the encoding to the memory image of The instructions.
             * Add the current value of the IC as the memory address of the instruction Added.
             */
            /* TODO: add binary to instruction image */
            if (!add_last(code_image, initImageLine(row_num, &IC, line, binary, FOUR_BYTES)))
                return FALSE;
        }
    }

    if (errno)
        return FALSE;

    *ICF = IC;
    *DCF = DC;

    updateSymbolTable(*ICF, symbol_table);
    updateDataImage(*ICF, data_image);

    return TRUE;
}

int getdata(size_t row, char *s, int data_type, LinkedList data_image, int *DC, char *text)
{
    char *tok, tmp[MAX_LINE_LENGTH];
    Binary *binary;

    if (data_type == ASCIZ)
    {
        s = str_strip(s);
        s++;
        while (*s != '"')
        {
            binary = malloc(sizeof(Binary));
            if (!binary)
                return FALSE;
            binary->one_byte = *s;
            if (!add_last(data_image, initImageLine(row, DC, text, binary, ONE_BYTE)))
                return FALSE;
            text = "";
            s++;
        }
        binary = malloc(sizeof(Binary));
        if (!binary)
            return FALSE;
        binary->one_byte = '\0';
        if (!add_last(data_image, initImageLine(row, DC, text, binary, ONE_BYTE)))
            return FALSE;
        return TRUE;
    }
    strcpy(tmp, s);
    tok = strtok(tmp, ",");
    while (tok)
    {
        binary = malloc(sizeof(Binary));
        if (!binary)
            return FALSE;
        switch (data_type)
        {
        case DB:
            binary->one_byte = (unsigned char)atoi(tok);
            if (!add_last(data_image, initImageLine(row, DC, text, binary, ONE_BYTE)))
            {
                free(binary);
                return FALSE;
            }
            text = "";
            break;

        case DH:
            binary->two_bytes = (signed short)atoi(tok);
            if (!add_last(data_image, initImageLine(row, DC, text, binary, TWO_BYTES)))
            {
                free(binary);
                return FALSE;
            }
            text = "";
            break;

        case DW:
            binary->four_bytes = atoi(tok);
            if (!add_last(data_image, initImageLine(row, DC, text, binary, FOUR_BYTES)))
            {
                free(binary);
                return FALSE;
            }
            text = "";
            break;
        }
        tok = strtok(NULL, ",");
    }
    return TRUE;
}

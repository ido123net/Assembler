#include "first_pass.h"
#include "parser.h"

int first_pass(const char filename[MAX_LINE_LENGTH], Image data_image, Image code_image, SymbolTable symbol_table, int *ICF, int *DCF)
{
    int *err = &errno;
    FILE *file;
    char line[MAX_LINE_LENGTH], tmp[MAX_LINE_LENGTH];
    char label[MAX_SYMBOL_LENGTH], symbol[MAX_SYMBOL_LENGTH];
    int labelflag, errorsflag = FALSE;
    /* int opcode, funct, rs, rt, rd, immed, reg, address; */ /* TODO: convert to struct */
    /* Operands operands; */
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
        /* //! memset(&operands, 0, sizeof(Operands)); */
        binary = malloc(sizeof(Binary));

        /* TODO delete line: */
        printf("%s", line);

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
                get_label(label, devided_line[FIRST_ARG]);
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
                errorsflag = TRUE;
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
            if (!addImageLine(initImageLine(row_num, NULL, line, binary, 0), code_image))
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
            get_label(label, devided_line[FIRST_ARG]);
            if (errno = valid_symbol(label))
            {
                symbol_error(row_num, label, errno);
                continue;
            }
            add_symbol_to_symbol_table(label, symbol_table, IC, CODE);
        }

        devided_line[++arg] = strtok(NULL, "\0");
        
        if (analyzeoperands(devided_line[(arg -1)], devided_line[arg], binary) == 0)
        {
            /* TODO: Build a binary encoding for the instruction */
            /**
             * In part as much as possible.
             * Add the encoding to the memory image of The instructions.
             * Add the current value of the IC as the memory address of the instruction Added.
             */
            /* TODO: add binary to instruction image */
            if (!addImageLine(initImageLine(row_num, &IC, line, binary, FOUR_BYTES), code_image))
                return FALSE;
        }
    }

    if (errorsflag)
        return FALSE;

    *ICF = IC;
    *DCF = DC;

    updateSymbolTable(*ICF, symbol_table);
    updateDataImage(*ICF, data_image);

    return TRUE;
}

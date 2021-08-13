#include "first_pass.h"
#include "parser.h"

int first_pass(const char filename[MAX_LINE_LENGTH], Image data_image, Image code_image, SymbolTable symbol_table, int *ICF, int *DCF)
{
    int *err = &errno;
    FILE *file;
    char line[MAX_LINE_LENGTH], tmp[MAX_LINE_LENGTH];
    char label[MAX_SYMBOL_LENGTH], symbol[MAX_SYMBOL_LENGTH];
    char instructions;
    int labelflag, errorsflag = FALSE;
    int opcode, funct, rs, rt, rd, immed, reg, address; /* TODO: convert to struct */
    int data_type;
    int IC = START_ADDRESS;
    int DC = 0;
    size_t row_num = 0;
    Binary binary;
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
        opcode = funct = rs = rt = rd = immed = reg = address = binary.four_bytes = 0;

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
            getdata(row_num, devided_line[arg], data_type, data_image, &DC, line); /* // ? remove line? */
            continue;
        }

        if (entry_line(devided_line[arg]))
        {
            if (!addImageLine(initImageLine(row_num, NULL, line, binary, 0), code_image))
                errorsflag = TRUE;
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

        if (!(instructions = get_instructions(devided_line[arg], &opcode, &funct)))
        {
            /* TODO: raise an error. */
            return -1;
        }
        devided_line[++arg] = strtok(NULL, "\0");
        /* TODO: analyze operands */
        /**
         * Analyze the structure of the operands according to the type of instruction.
         * If an error is detected, raise an error.
         */
        if (analyzeoperands(devided_line[arg], opcode, funct, &rs, &rt, &rd, &immed, &reg, &address))
        {
            /* TODO: Build a binary encoding for the instruction */
            /**
             * In part as much as possible.
             * Add the encoding to the memory image of The instructions.
             * Add the current value of the IC as the memory address of the instruction Added.
             */
            switch (instructions)
            {
            case 'R':
                binary.Rbinary.opcode = opcode;
                binary.Rbinary.rs = rs;
                binary.Rbinary.rt = rt;
                binary.Rbinary.rd = rd;
                binary.Rbinary.funct = funct;
                break;

            case 'I':
                binary.Ibinary.opcode = opcode;
                binary.Ibinary.rs = rs;
                binary.Ibinary.rt = rt;
                immedConvert(&immed);
                binary.Ibinary.immed = immed;
                break;

            case 'J':
                binary.Jbinary.opcode = opcode;
                binary.Jbinary.reg = reg;
                binary.Jbinary.address = address;
                break;
            }
            /* TODO: add binary to instruction image */
            if (!addImageLine(initImageLine(row_num, &IC, line, binary, FOUR_BYTES), code_image))
                errorsflag = TRUE;
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

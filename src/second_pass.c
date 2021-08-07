#include "second_pass.h"
#include "parser.h"

int second_pass(Image code_image, SymbolTable symbol_table, ExternalLines external_lines, int *ICF, int *DCF)
{
    char *devided_line[NO_OF_ELEMENTS];
    char tmp[MAX_LINE_LENGTH];
    ImageLine line;
    SymbolTableLine symbol_table_line;
    char symbol[MAX_SYMBOL_LENGTH];
    int errorsflag = FALSE;
    ExternalLine externLine;
    int arg;

    line = code_image->head;

    while (line)
    {
        arg = 0;
        strcpy(tmp, line->code);

        devided_line[arg] = strtok(tmp, SPACES);

        if (is_label(devided_line[arg])) /* if the first word of the line is label */
            devided_line[++arg] = strtok(NULL, SPACES);

        devided_line[(arg + 1)] = strtok(NULL, "\0");

        if (entry_line(devided_line[arg]))
        {
            get_symbol(symbol, devided_line[(arg + 1)]);
            addAttr(get_symbol_line(symbol_table, symbol), ENTRY);
            line = line->nextLine;
            continue;
        }

        switch (line->binary.Rbinary.opcode)
        {
        case BEQ:
        case BNE:
        case BLT:
        case BGT:
            /* TODO: check valid distance */
            strtok(devided_line[(arg + 1)], ",");
            strtok(NULL, ",");
            get_symbol(symbol, strtok(NULL, "\0"));
            symbol_table_line = get_symbol_line(symbol_table, symbol);
            if (symbol_table_line->attributes & EXTERNAL)
            {
                errorsflag = TRUE;
                break;
            }

            line->binary.Ibinary.immed = symbol_table_line->value - line->address;
            break;

        case JMP:
        case LA:
        case CALL:
            get_symbol(symbol, devided_line[(arg + 1)]);
            if (str_to_reg(symbol) >= 0)
                break;
            symbol_table_line = get_symbol_line(symbol_table, symbol);
            if ((symbol_table_line->attributes & EXTERNAL))
            {
                if (!(externLine = initExternalLine(symbol, line->address)))
                {
                    errorsflag = TRUE;
                    break;
                }
                addExternalLine(externLine, external_lines);
            }

            line->binary.Jbinary.address = symbol_table_line->value;
            break;

        default:
            break;
        }

        line = line->nextLine;
    }

    if (errorsflag)
        return FALSE;

    return TRUE;
}

#include "second_pass.h"
#include "parser.h"

int second_pass(LinkedList code_image,
                LinkedList symbol_table,
                LinkedList external_lines,
                int *ICF, int *DCF)
{
    char *devided_line[NO_OF_ELEMENTS];
    char tmp[MAX_LINE_LENGTH];
    Node node;
    ImageLine line;
    SymbolTableLine symbol_table_line;
    char symbol[MAX_SYMBOL_LENGTH];
    int errorsflag = FALSE;
    ExternalLine externLine;
    int arg;

    node = code_image->head;

    while (node)
    {
        line = node->data;
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
            node = node->next;
            continue;
        }

        switch (line->binary->Rbinary.opcode)
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
            if (symbol_table_line->attributes.external == TRUE)
            {
                /* TODO: error */
                break;
            }

            line->binary->Ibinary.immed = symbol_table_line->value - line->address;
            break;

        case JMP:
        case LA:
        case CALL:
            get_symbol(symbol, devided_line[(arg + 1)]);
            if (str_to_reg(symbol) >= 0)
                break;
            symbol_table_line = get_symbol_line(symbol_table, symbol);
            if ((symbol_table_line->attributes.external == TRUE))
            {
                if (!(externLine = initExternalLine(symbol, line->address)))
                {
                    /* TODO: error */
                    break;
                }
                add_last(external_lines, externLine);
            }

            line->binary->Jbinary.address = symbol_table_line->value;
            break;

        default:
            break;
        }

        node = node->next;
    }

    if (errorsflag)
        return FALSE;

    return TRUE;
}

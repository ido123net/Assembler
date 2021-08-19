#include "second_pass.h"
#include "parser.h"

int second_pass(LinkedList code_image, LinkedList symbol_table, LinkedList external_lines)
{
    char *divided_line[NO_OF_ELEMENTS];
    char tmp[MAX_LINE_LENGTH];
    Node node;
    CodeLine line;
    SymbolTableLine symbol_table_line;
    char symbol[MAX_SYMBOL_LENGTH];
    int errorsflag = FALSE;
    ExternalLine externLine;
    int arg, dist;

    node = code_image->head;

    while (node)
    {
        line = node->data;
        arg = 0;
        strcpy(tmp, line->code);

        divided_line[arg] = strtok(tmp, SPACES);

        if (is_label(divided_line[arg])) /* if the first word of the line is label */
            divided_line[++arg] = strtok(NULL, SPACES);

        divided_line[(arg + 1)] = strtok(NULL, "\0");

        if (entry_line(divided_line[arg]))
        {
            get_symbol(symbol, divided_line[(arg + 1)]);
            if (!addAttr(get_symbol_line(symbol_table, symbol), ENTRY))
            {
                symbol_error(line->row, symbol, MISSING_SYMBOL);
                errorsflag = TRUE;
            }
            node = node->next;
            continue;
        }

        switch (line->binary->Rbinary.opcode)
        {
        case BEQ:
        case BNE:
        case BLT:
        case BGT:
            strtok(divided_line[(arg + 1)], ",");
            strtok(NULL, ",");
            get_symbol(symbol, strtok(NULL, "\0"));
            if (!(symbol_table_line = get_symbol_line(symbol_table, symbol)))
            {
                symbol_error(line->row, symbol, MISSING_SYMBOL);
                errorsflag = TRUE;
            }
            else if (symbol_table_line->attributes.external == TRUE)
            {
                symbol_error(line->row, symbol, EXTERNAL_SYMBOL);
                errorsflag = TRUE;
            }
            else if ((dist = symbol_table_line->value - line->address) > IMMED_MAX)
            {
                symbol_error(line->row, symbol, INVALID_DIST);
                errorsflag = TRUE;
            }
            else
                line->binary->Ibinary.immed = dist;
            break;

        case JMP:
        case LA:
        case CALL:
            get_symbol(symbol, divided_line[(arg + 1)]);
            if (str_to_reg(symbol) >= 0)
                break;
            if (!(symbol_table_line = get_symbol_line(symbol_table, symbol)))
            {
                symbol_error(line->row, symbol, MISSING_SYMBOL);
                errorsflag = TRUE;
            }
            else if ((symbol_table_line->attributes.external == TRUE))
            {
                externLine = initExternalLine(symbol, line->address);
                add_last(external_lines, externLine);
            }
            else
                line->binary->Jbinary.address = symbol_table_line->value;
            break;
        }

        node = node->next;
    }

    return !errorsflag;
}

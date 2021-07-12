#include "first_pass.h"

static char *data_types[] = { // ! move to parser
    ".db",
    ".dh",
    ".dw",
    ".asciz"};

static char *R_type_instructions[] = {
    "add",
    "sub",
    "and",
    "or",
    "nor",
    "move",
    "mvhi",
    "mvlo"};

static char *I_type_instructions[] = {
    "addi",
    "subi",
    "andi",
    "ori",
    "nori",
    "bne",
    "beq",
    "blt",
    "bgt",
    "lb",
    "sb",
    "lw",
    "sw",
    "lh",
    "sh"};

static char *J_type_instructions[] = {
    "jmp",
    "la",
    "call",
    "stop"};

static char *registers[] = {
    "$0",
    "$1",
    "$2",
    "$3",
    "$4",
    "$5",
    "$6",
    "$7",
    "$8",
    "$9",
    "$10",
    "$11",
    "$12",
    "$13",
    "$14",
    "$15",
    "$16",
    "$17",
    "$18",
    "$19",
    "$20",
    "$21",
    "$22",
    "$23",
    "$24",
    "$25",
    "$26",
    "$27",
    "$28",
    "$29",
    "$30",
    "$31"};

int labelline(char *s) // ! move to parser
{
    if (!isalpha(*s))
        return FALSE;
    while (isalpha(*s))
        s++;
    if (*s != ':' || *(s + 1))
        return FALSE;
    return TRUE;
}

char *getlabel(char *s)
{
    int len = strlen(s);
    s[len - 1] = '\0';
    return s;
}

char *strstrip(char *s)
{
    size_t len, end;
    while (isspace(*s))
        s++;
    len = strlen(s);
    if (!len)
        return s;
    end = len - 1;
    while (isspace(s[end]))
        end--;
    s[end + 1] = '\0';
    return s;
}

int dataline(char *s) // ! move to parser
{
    int i;
    s = strstrip(s);
    for (i = 0; i < LENGTH(data_types); i++)
    {
        if (strcmp(data_types[i], s) == 0)
        {
            return i;
        }
    }
    return -1;
}

char getinstructions(char *s, int *opcode, int *funct) // ! move to parser
{
    int i;
    s = strstrip(s);
    for (i = 0; i < LENGTH(R_type_instructions); i++)
    {
        if (strcmp(R_type_instructions[i], s) == 0)
        {
            *opcode = i / ARITHMETIC_BOUND;
            *funct = (i % ARITHMETIC_BOUND) + 1;
            return 'R';
        }
    }
    for (i = 0; i < LENGTH(I_type_instructions); i++)
    {
        if (strcmp(I_type_instructions[i], s) == 0)
        {
            *opcode = i + I_OFFSET;
            return 'I';
        }
    }
    for (i = 0; i < LENGTH(J_type_instructions); i++)
    {
        if (strcmp(J_type_instructions[i], s) == 0)
        {
            if (i < 3)
                *opcode = i + J_OFFSET;
            else
                *opcode = i + (J_OFFSET * 2);
            return 'J';
        }
    }
    return '\0';
}

int strtoreg(char *tok)
{
    int i;
    tok = strstrip(tok);
    for (i = 0; i < LENGTH(registers); i++)
    {
        if (strcmp(registers[i], tok) == 0)
            return i;
    }
    return -1;
}

int strtoimmed(char *tok)
{
    char *p;
    tok = strstrip(tok);
    p = tok;
    if (*tok == '-' || *tok == '+')
        tok++;
    for (; isdigit(*tok); tok++)
        ;
    if (*tok)
        return IMMED_MAX + 1;
    return atoi(p);
}

char *strtolabel(char *tok)
{
    char *s;
    tok = strstrip(tok);
    s = tok;
    while (isalpha(*s))
        s++;
    if (*(s + 1))
        return NULL;
    return tok;
}

int analyzeoperands(char *s, int opcode, int funct, int IC, int *rs, int *rt, int *rd, int *immed, int *reg, int *address)
{
    int _reg, _immed;
    char *_label;
    char *tok;
    switch (opcode)
    {
    case ADD:
        tok = strtok(s, ",");
        if ((_reg = strtoreg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        if ((_reg = strtoreg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = strtoreg(tok)) >= 0)
            *rd = _reg;
        else
            return FALSE;
        break;

    case MOVE:
        tok = strtok(s, ",");
        if ((_reg = strtoreg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = strtoreg(tok)) >= 0)
            *rd = _reg;
        else
            return FALSE;
        break;

    case ADDI:
    case SUBI:
    case ANDI:
    case ORI:
    case NORI:
        tok = strtok(s, ",");
        if ((_reg = strtoreg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        _immed = strtoimmed(tok);
        if (_immed <= (+IMMED_MAX) && _immed >= (-IMMED_MIN))
            *immed = _immed;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = strtoreg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        break;

    case BEQ:
    case BNE:
    case BLT:
    case BGT:
        tok = strtok(s, ",");
        if ((_reg = strtoreg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        if ((_reg = strtoreg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_label = strtolabel(tok)) == NULL)
            return FALSE;
        break;

    case LB:
    case SB:
    case LW:
    case SW:
    case LH:
    case SH:
        tok = strtok(s, ",");
        if ((_reg = strtoreg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        _immed = strtoimmed(tok);
        if (_immed <= (+IMMED_MAX) && _immed >= (-IMMED_MIN))
            *immed = _immed;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = strtoreg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        break;

    case JMP:
        tok = strtok(s, "\0");
        tok = strstrip(tok);
        switch (*tok)
        {
        case '$':
            *reg = TRUE;
            if ((_reg = strtoreg(tok)) >= 0)
                *address = _reg;
            break;

        default:
            *reg = FALSE;
            if ((_label = strtolabel(tok)) == NULL)
                return FALSE;
            break;
        }
        break;

    case LA:
    case CALL:
        tok = strtok(s, "\0");
        tok = strstrip(tok);
        if ((_label = strtolabel(tok)) == NULL)
            return FALSE;
        break;

    case STOP:
        tok = strtok(s, "\0");
        if (tok)
            return FALSE;
        break;

    default:
        break;
    }
    return TRUE;
}

int valid_data(char *tok, int type)
{
    int num;
    char *p;
    p = tok;
    if (type == ASCIZ)
    {
        if (*tok++ != '"')
            return FALSE;
        for (; *tok != '"'; tok++)
            ;
        if ('"' != *tok++)
            return FALSE;
        if (*tok)
            return FALSE;
        return TRUE;
    }

    if (*tok == '-' || *tok == '+')
        tok++;
    for (; isdigit(*tok); tok++)
        ;
    if (*tok)
        return FALSE;

    num = atoi(p);

    switch (type)
    {
    case DB:
        return (num >= +SCHAR_MIN && num <= +SCHAR_MAX);

    case DH:
        return (num >= +SHRT_MIN && num <= +SHRT_MAX);

    case DW:
        return (num >= +INT_MIN && num <= +INT_MAX);

    default:
        break;
    }
    return FALSE;
}

int getdata(char *s, int data, Image data_image, int *DC, char *text)
{
    char *tok;
    char db;
    short dh;
    int dw;
    if (data == ASCIZ)
    {
        tok = strstrip(s);
        if (valid_data(tok, ASCIZ))
        {
            tok++;
            while (*tok != '"')
            {
                if (!addImageLine(initImageLine(DC, text, tok, ONE_BYTE), data_image))
                    return FALSE;
                text = "";
                tok++;
            }
            return TRUE;
        }
        else
            return FALSE;
    }
    tok = strtok(s, ",");
    while (tok)
    {
        switch (data)
        {
        case DB:
            tok = strstrip(tok);
            if (valid_data(tok, DB))
            {
                db = (char)atoi(tok);
                if (!addImageLine(initImageLine(DC, text, &db, ONE_BYTE), data_image))
                    return FALSE;
                text = "";
            }
            else
                return FALSE;
            break;

        case DH:
            tok = strstrip(tok);
            if (valid_data(tok, DH))
            {
                dh = (short)atoi(tok);
                if (!addImageLine(initImageLine(DC, text, &dh, TWO_BYTES), data_image))
                    return FALSE;
                text = "";
            }
            else
                return FALSE;
            break;

        case DW:
            tok = strstrip(tok);
            if (valid_data(tok, DW))
            {
                dw = atoi(tok);
                if (!addImageLine(initImageLine(DC, text, &dw, FOUR_BYTES), data_image))
                    return FALSE;
                text = "";
            }
            else
                return FALSE;
            break;
        }
        tok = strtok(NULL, ",");
    }
    return TRUE;
}

int blankline(char *tok) // ! move to parser
{
    return (!(*tok));
}

int commentline(char *tok) // ! move to parser
{
    return (*tok == ';');
}

int entryline(char *tok)
{
    return (strcmp(tok, ".entry") == 0);
}

int externline(char *tok)
{
    return (strcmp(tok, ".extern") == 0);
}

void updateSymbolTable(int ICF, SymbolTable symbol_table)
{
    SymbolTableLine line = symbol_table->head;
    Attribute attribute;
    while (line)
    {
        bool update = FALSE;
        attribute = line->attributes;
        while (attribute)
        {
            if (strcmp(attribute->attr, "data") == 0)
                update = TRUE;
        }
        if (update)
            line->value += ICF;
        line = line->nextLine;
    }
}

void updateDataImage(int ICF, Image data_image)
{
    ImageLine line = data_image->head;
    while (line)
    {
        line->address += ICF;
        line = line->nextLine;
    }
}

int first_pass(const char filename[MAX_LINE_LENGTH], Image data_image, Image code_image, SymbolTable symbol_table, int *ICF, int *DCF)
{
    /* TODO: delete this */
    char *tmp;

    FILE *file;
    char line[BUFFER];
    char text[MAX_LINE_LENGTH];
    char *tok, *_label;
    char instructions;
    int labelflag, errorsflag = FALSE;
    int opcode, funct, rs, rt, rd, immed, reg, address, bin;
    int IC = START_ADDRESS;
    int DC = 0;
    size_t row_num = 0;

    if ((file = fopen(filename, "r")) == NULL) /* TODO: error handle */
        return -1;

    while ((fgets(line, BUFFER, file)) != NULL)
    {
        row_num++;
        labelflag = FALSE;
        opcode = funct = rs = rt = rd = immed = reg = address = bin = 0;

        /* TODO: Warning line too long */

        strncpy(text, line, MAX_LINE_LENGTH);

        /* TODO: delete this */
        tmp = strdup(text);
        printf("%s", tmp);

        tok = strtok(text, SPACES); /* get first word of line */

        if (blankline(tok) || commentline(tok)) /* if line is blank or comment ignore it */
            continue;

        if (labelline(tok)) /* if the first word of the line is label */
        {
            labelflag = TRUE;           /* raise label flag */
            _label = getlabel(tok);     /* get the label */
            tok = strtok(NULL, SPACES); /* get next word */
        }

        if ((dataflag = dataline(tok)) >= 0) /* Check if this is data instructions */
        {
            /* TODO: get data */
            if (labelflag)
            {
                /* TODO: Insert the symbol */
                /**
                 * Insert the label into the symbol table with the value IC
                 * and with the code property.
                 * raise error if needed
                 */
                if (strlen(_label) > MAX_LABEL_LENGTH)
                {
                    /* TODO: raise error */
                    errorsflag = TRUE;
                }
                else
                {
                    if (!addSymbolTableLine(initSymbolTableLine(_label, DC, initAttr("data")), symbol_table))
                        errorsflag = TRUE;
                }
            }
            tok = strtok(NULL, "\0");
            if (!getdata(tok, dataflag, data_image, &DC, tmp))
                errorsflag = TRUE;
            continue;
        }

        if (entryline(tok))
            continue;
        if (externline(tok))
        {
            /* TODO: Insert the symbol */
            /**
             * Insert the symbol that appears as the operand of the prompt into the symbol table with the value 0
             * and with the extern property.
             * raise error if needed
             */
            continue;
        }
        if (labelflag)
        {
            /* TODO: Insert the symbol */
            /**
             * Insert the label into the symbol table with the value IC
             * and with the code property.
             * raise error if needed
             */
        }
        if ((instructions = getinstructions(tok, &opcode, &funct)))
        {
            tok = strtok(NULL, "\0");
        }
        else
        {
            /* TODO: raise an error. */
        }
        /* TODO: analyze operands */
        /**
         * Analyze the structure of the operands according to the type of instruction.
         * If an error is detected, raise an error.
         */
        if (analyzeoperands(tok, opcode, funct, IC, &rs, &rt, &rd, &immed, &reg, &address))
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
                setBinary(&bin, OPCODE, opcode);
                setBinary(&bin, RS, rs);
                setBinary(&bin, RT, rt);
                setBinary(&bin, RD, rd);
                setBinary(&bin, FUNCT, funct);
                break;

            case 'I':
                setBinary(&bin, OPCODE, opcode);
                setBinary(&bin, RS, rs);
                setBinary(&bin, RT, rt);
                immedConvert(&immed);
                setBinary(&bin, IMMED, immed);
                break;

            case 'J':
                setBinary(&bin, OPCODE, opcode);
                setBinary(&bin, REG, reg);
                setBinary(&bin, ADDRESS, address);
                break;
            }
            /* TODO: add binary to instruction image */
            if (!addImageLine(initImageLine(&IC, text, tok, FOUR_BYTES), code_image))
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

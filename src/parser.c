#include "parser.h"

static char *data_types[] = {
    "db",
    "dh",
    "dw",
    "asciz"};

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

static int isnumber(char *s)
{
    if (!s)
        return FALSE;

    if (*s == '-' || *s == '+')
        s++;

    for (; isdigit(*s); s++)
        ;

    return (*s == '\0');
}

int str_to_reg(char *tok)
{
    int i;
    tok = str_strip(tok);
    for (i = 0; i < LENGTH(registers); i++)
    {
        if (strcmp(registers[i], tok) == 0)
            return i;
    }
    return -1;
}

int valid_immed(int immed)
{
    return (immed <= (+IMMED_MAX) && immed >= (-IMMED_MIN));
}

char *str_to_symbol(char *tok)
{
    tok = str_strip(tok);
    if (is_symbol(tok) != 0)
        return NULL;
    return tok;
}

char *my_strdup(const char *s)
{
    char *p = malloc(strlen(s) + 1);
    if (p)
    {
        strcpy(p, s);
    }
    return p;
}

char *str_strip(char *s)
{
    size_t len, end;

    if (!s)
        return NULL;

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

int entry_line(char *tok)
{
    return (strcmp(tok, ".entry") == 0);
}

int extern_line(char *tok)
{
    return (strcmp(tok, ".extern") == 0);
}

int blank_line(char *tok)
{
    return (!(tok));
}

int comment_line(char *tok)
{
    return (*tok == ';');
}

int is_symbol(char *s)
{
    if (!s || strlen(s) == 0)
        return FALSE;
    if (isalpha(*s))
        s++;
    while (isalnum(*s))
        s++;
    return !(*s);
}

int is_label(char *s)
{
    char label[MAX_SYMBOL_LENGTH];
    int len = strlen(s);
    if (s[len - 1] != ':')
        return FALSE;
    strncpy(label, s, len - 1);
    label[len - 1] = '\0';
    return is_symbol(label);
}

char *get_symbol(char *symbol, char *s)
{
    if (!s)
        return NULL;

    s = str_strip(s);
    strcpy(symbol, s);
    return symbol;
}

char *get_label(char *label, char *s)
{
    int len;
    char _label[MAX_SYMBOL_LENGTH];
    s = str_strip(s);
    len = strlen(s) - 1;
    strncpy(_label, s, len);
    _label[len] = '\0';
    get_symbol(label, _label);
    return label;
}

int is_data(char *s)
{
    int i;
    s = str_strip(s);

    if (*s == '.') /* data instructions start with '.' */
        s++;
    else
        return FALSE;

    for (i = 0; i < LENGTH(data_types); i++)
        if (strcmp(data_types[i], s) == 0)
            return TRUE;

    return FALSE;
}

int valid_symbol(size_t row, char *symbol)
{
    int i;

    if (!is_symbol(symbol))
        return symbol_error(row, symbol, INVALID_SYMBOL);

    for (i = 0; i < LENGTH(R_type_instructions); i++)
        if (strcmp(R_type_instructions[i], symbol) == 0)
            return symbol_error(row, symbol, SAVED_WORD);
    for (i = 0; i < LENGTH(I_type_instructions); i++)
        if (strcmp(I_type_instructions[i], symbol) == 0)
            return symbol_error(row, symbol, SAVED_WORD);
    for (i = 0; i < LENGTH(J_type_instructions); i++)
        if (strcmp(J_type_instructions[i], symbol) == 0)
            return symbol_error(row, symbol, SAVED_WORD);
    for (i = 0; i < LENGTH(data_types); i++)
        if (strcmp(data_types[i], symbol) == 0)
            return symbol_error(row, symbol, SAVED_WORD);

    if (strlen(symbol) >= MAX_SYMBOL_LENGTH)
        return symbol_error(row, symbol, SYMBOL_TOO_LONG);

    return TRUE;
}

int get_directive_type(char *s, Binary *binary)
{
    int i;
    s = str_strip(s);
    for (i = 0; i < LENGTH(R_type_instructions); i++)
    {
        if (strcmp(R_type_instructions[i], s) == 0)
        {
            binary->Rbinary.opcode = i / ARITHMETIC_BOUND;
            binary->Rbinary.funct = (i % ARITHMETIC_BOUND) + 1;
            return binary->Rbinary.opcode;
        }
    }
    for (i = 0; i < LENGTH(I_type_instructions); i++)
    {
        if (strcmp(I_type_instructions[i], s) == 0)
        {
            binary->Ibinary.opcode = i + I_OFFSET;
            return binary->Ibinary.opcode;
        }
    }
    for (i = 0; i < LENGTH(J_type_instructions); i++)
    {
        if (strcmp(J_type_instructions[i], s) == 0)
        {
            if (i < 3)
                binary->Jbinary.opcode = i + J_OFFSET;
            else
                binary->Jbinary.opcode = i + (J_OFFSET * 2);
            return binary->Jbinary.opcode;
        }
    }
    return -1;
}

int get_data_type(char *tok)
{
    tok = str_strip(tok);
    tok++; /* data instructions start with '.' */

    if (strcmp("db", tok) == 0)
        return DB;
    if (strcmp("dh", tok) == 0)
        return DH;
    if (strcmp("dw", tok) == 0)
        return DW;
    if (strcmp("asciz", tok) == 0)
        return ASCIZ;

    return -1;
}

int valid_str(char *s)
{
    s = str_strip(s);
    if (*s++ != '"')
        return FALSE;
    for (; *s && *s != '"'; s++)
        ;
    return (*s == '"' && !(*(s + 1)));
}

int valid_num(char *s)
{
    s = str_strip(s);
    if (*s == '-' || *s == '+')
        s++;
    for (; isdigit(*s); s++)
        ;
    return !(*s);
}

int valid_data(size_t row, char *s, int data_type)
{
    long num;
    char str_num[MAX_LINE_LENGTH], *tok;

    if (data_type == ASCIZ)
    {
        if (!valid_str(s))
            return data_error(row, s, INVALID_STR);
    }

    strcpy(str_num, s);
    tok = strtok(str_num, ",");
    while (tok)
    {
        if (!valid_num(tok))
            return data_error(row, s, NAN);
        num = atoi(tok);
        switch (data_type)
        {
        case DB:
            if (num >= +SCHAR_MIN && num <= +SCHAR_MAX)
                break;

        case DH:
            if (num >= +SHRT_MIN && num <= +SHRT_MAX)
                break;

        case DW:
            if (num >= +INT_MIN && num <= +INT_MAX)
                break;
            
        default:
            return data_error(row, tok, OOR);
        }
        tok = strtok(NULL, ",");
    }
    return TRUE;
}

int analyzeoperands(size_t row, char *directive, char *operands, Binary *binary)
{
    int _reg, _immed;
    char *label;
    char *tok;

    int type;

    if ((type = get_directive_type(directive, binary)) == -1)
        return directive_error(row, directive, INVALID_DIRECTIVE);

    switch (type)
    {
    case ADD: /* work as well for SUB, AND, OR and NOR */
        tok = strtok(operands, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Rbinary.rs = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        tok = strtok(NULL, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Rbinary.rt = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        tok = strtok(NULL, SPACES);
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Rbinary.rd = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        break;

    case MOVE: /* work as well for MVHI, and MVLO */
        tok = strtok(operands, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Rbinary.rs = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        tok = strtok(NULL, SPACES);
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Rbinary.rd = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        break;

    case ADDI:
    case SUBI:
    case ANDI:
    case ORI:
    case NORI:
        tok = strtok(operands, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Ibinary.rs = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        tok = strtok(NULL, ",");
        if (!isnumber(tok))
            return directive_error(row, directive, NAN);
        _immed = atoi(tok);
        if (valid_immed(_immed))
            binary->Ibinary.immed = _immed;
        else
            return directive_error(row, directive, INVALID_IMMED);
        tok = strtok(NULL, SPACES);
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Ibinary.rt = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        break;

    case BEQ:
    case BNE:
    case BLT:
    case BGT:
        tok = strtok(operands, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Ibinary.rs = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        tok = strtok(NULL, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Ibinary.rt = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        tok = strtok(NULL, SPACES);
        if ((label = str_to_symbol(tok)) == NULL)
            return directive_error(row, directive, INVALID_LABEL);
        break;

    case LB:
    case SB:
    case LW:
    case SW:
    case LH:
    case SH:
        tok = strtok(operands, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Ibinary.rs = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        tok = strtok(NULL, ",");
        if (!isnumber(tok))
            return directive_error(row, directive, NAN);
        _immed = atoi(tok);
        if (valid_immed(_immed))
            binary->Ibinary.immed = _immed;
        else
            return directive_error(row, directive, INVALID_LABEL);
        tok = strtok(NULL, SPACES);
        if ((_reg = str_to_reg(tok)) >= 0)
            binary->Ibinary.rt = _reg;
        else
            return directive_error(row, directive, NOT_REG);
        break;

    case JMP:
        tok = strtok(operands, SPACES);
        tok = str_strip(tok);
        switch (*tok)
        {
        case '$':
            binary->Jbinary.reg = TRUE;
            if ((_reg = str_to_reg(tok)) >= 0)
                binary->Jbinary.address = _reg;
            else
                return directive_error(row, directive, NOT_REG);
            break;

        default:
            binary->Jbinary.reg = FALSE;
            if ((label = str_to_symbol(tok)) == NULL)
                return directive_error(row, directive, INVALID_LABEL);
            break;
        }
        break;

    case LA:
    case CALL:
        tok = strtok(operands, SPACES);
        tok = str_strip(tok);
        if ((label = str_to_symbol(tok)) == NULL)
            return directive_error(row, directive, INVALID_LABEL);
        break;

    case STOP:
        if (operands)
            return directive_error(row, directive, EXTRANEOUS_OPERAND);
        break;
    }
    if (strtok(NULL, SPACES) != NULL)
        return directive_error(row, directive, EXTRANEOUS_OPERAND);

    return TRUE;
}

char **devide_line(char line[MAX_LINE_LENGTH], char *devided_line[NO_OF_ELEMENTS], const char *delim)
{
    char *tmp = my_strdup(line);
    devided_line[0] = strtok(tmp, delim);
    if (is_label(devided_line[0]))
    {
        devided_line[1] = strtok(NULL, delim);
        devided_line[2] = strtok(NULL, "\0");
    }
    else
    {
        devided_line[1] = strtok(NULL, "\0");
        devided_line[2] = NULL;
    }
    return devided_line;
}

char *read_line(char *s, int n, FILE *stream)
{
    int len;
    char c;
    s = fgets(s, n + 1, stream);
    if (!s || (len = strlen(s)) == 0)
        return s;
    /* TODO: Warning line too long */
    if (s[len - 1] != '\n' && s[len - 1] != '\0')
    {
        c = getc(stream);
        while (c != '\n' && c != '\0')
            c = getc(stream);
    }
    s[len - 1] = '\n';
    return s;
}

int valid_filename(const char *filename)
{
    if (strlen(filename) > MAX_LINE_LENGTH)
        return FALSE;
    for (; isalpha(*filename); filename++)
        ;
    return (strcmp(filename, ".as") == 0);
}

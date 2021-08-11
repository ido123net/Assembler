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

int str_to_immed(char *tok)
{
    char *p;
    tok = str_strip(tok);
    p = tok;
    if (*tok == '-' || *tok == '+')
        tok++;
    for (; isdigit(*tok); tok++)
        ;
    if (*tok)
        return IMMED_MAX + 1;
    return atoi(p);
}

char *str_to_symbol(char *tok)
{
    tok = str_strip(tok);
    if (!is_symbol(tok))
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
        return -1;
    if (isalpha(*s))
        s++;
    while (isalnum(*s))
        s++;
    return (*s)? INVALID_SYMBOL : 0;
}

int is_label(char *s)
{
    char label[MAX_SYMBOL_LENGTH];
    int len = strlen(s);
    if (s[len - 1] != ':')
        return FALSE;
    strncpy(label, s, len - 1);
    label[len - 1] = '\0';
    return is_symbol(label) == 0;
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

int valid_symbol(char *symbol)
{
    int i;

    for (i = 0; i < LENGTH(R_type_instructions); i++)
        if (strcmp(R_type_instructions[i], symbol) == 0)
            return SAVED_WORD;
    for (i = 0; i < LENGTH(I_type_instructions); i++)
        if (strcmp(I_type_instructions[i], symbol) == 0)
            return SAVED_WORD;
    for (i = 0; i < LENGTH(J_type_instructions); i++)
        if (strcmp(J_type_instructions[i], symbol) == 0)
            return SAVED_WORD;
    for (i = 0; i < LENGTH(data_types); i++)
        if (strcmp(data_types[i], symbol) == 0)
            return SAVED_WORD;

    if (strlen(symbol) >= MAX_SYMBOL_LENGTH)
        return SYMBOL_TOO_LONG;

    return 0;
}

char get_directive(char *s, int *opcode, int *funct)
{
    int i;
    s = str_strip(s);
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

int get_data_type(char *tok)
{
    int data_type;
    tok = str_strip(tok);
    tok++; /* data instructions start with '.' */
    for (data_type = 0; data_type < LENGTH(data_types); data_type++)
        if (strcmp(data_types[data_type], tok) == 0)
            return data_type;

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

int valid_data(char *s, int data_type)
{
    long num;
    char str_num[MAX_LINE_LENGTH], *tok;

    if (data_type == ASCIZ)
    {
        return valid_str(s);
    }
    strcpy(str_num, s);
    tok = strtok(str_num, ",");
    while (tok)
    {
        if (!valid_num(tok))
            return FALSE;
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
            return FALSE;
        }
        tok = strtok(NULL, ",");
    }
    return TRUE;
}

int getdata(size_t row, char *s, int data_type, Image data_image, int *DC, char *text)
{
    char *tok, *tmp;
    Binary bin;

    if (data_type == ASCIZ)
    {
        s = str_strip(s);
        s++;
        while (*s != '"')
        {
            bin.one_byte = *s;
            if (!addImageLine(initImageLine(row, DC, text, bin, ONE_BYTE), data_image))
                return FALSE;
            text = "";
            s++;
        }
        bin.one_byte = '\0';
        if (!addImageLine(initImageLine(row, DC, text, bin, ONE_BYTE), data_image))
            return FALSE;
        return TRUE;
    }
    tmp = my_strdup(s);
    tok = strtok(tmp, ",");
    while (tok)
    {
        switch (data_type)
        {
        case DB:
            bin.one_byte = (unsigned char)atoi(tok);
            if (!addImageLine(initImageLine(row, DC, text, bin, ONE_BYTE), data_image))
                return FALSE;
            text = "";
            break;

        case DH:
            bin.two_bytes = (signed short)atoi(tok);
            if (!addImageLine(initImageLine(row, DC, text, bin, TWO_BYTES), data_image))
                return FALSE;
            text = "";
            break;

        case DW:
            bin.four_bytes = atoi(tok);
            if (!addImageLine(initImageLine(row, DC, text, bin, FOUR_BYTES), data_image))
                return FALSE;
            text = "";
            break;
        }
        tok = strtok(NULL, ",");
    }
    return TRUE;
}

int analyzeoperands(char *s, int opcode, int funct, int IC, int *rs, int *rt, int *rd, int *immed, int *reg, int *address)
{
    int _reg, _immed;
    char *label;
    char *tok;
    switch (opcode)
    {
    case ADD:
        tok = strtok(s, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rd = _reg;
        else
            return FALSE;
        break;

    case MOVE:
        tok = strtok(s, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = str_to_reg(tok)) >= 0)
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
        if ((_reg = str_to_reg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        _immed = str_to_immed(tok);
        if (_immed <= (+IMMED_MAX) && _immed >= (-IMMED_MIN))
            *immed = _immed;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        break;

    case BEQ:
    case BNE:
    case BLT:
    case BGT:
        tok = strtok(s, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((label = str_to_symbol(tok)) == NULL)
            return FALSE;
        break;

    case LB:
    case SB:
    case LW:
    case SW:
    case LH:
    case SH:
        tok = strtok(s, ",");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rs = _reg;
        else
            return FALSE;
        tok = strtok(NULL, ",");
        _immed = str_to_immed(tok);
        if (_immed <= (+IMMED_MAX) && _immed >= (-IMMED_MIN))
            *immed = _immed;
        else
            return FALSE;
        tok = strtok(NULL, "\0");
        if ((_reg = str_to_reg(tok)) >= 0)
            *rt = _reg;
        else
            return FALSE;
        break;

    case JMP:
        tok = strtok(s, "\0");
        tok = str_strip(tok);
        switch (*tok)
        {
        case '$':
            *reg = TRUE;
            if ((_reg = str_to_reg(tok)) >= 0)
                *address = _reg;
            break;

        default:
            *reg = FALSE;
            if ((label = str_to_symbol(tok)) == NULL)
                return FALSE;
            break;
        }
        break;

    case LA:
    case CALL:
        tok = strtok(s, "\0");
        tok = str_strip(tok);
        if ((label = str_to_symbol(tok)) == NULL)
            return FALSE;
        break;

    case STOP:
        if (s)
            return FALSE;
        break;

    default:
        break;
    }
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
    if (s[len - 1] != '\0')
        return s;
    if (s[len - 1] != '\n')
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

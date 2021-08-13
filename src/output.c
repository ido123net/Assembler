#include "output.h"

int output_files(char filename[MAX_LINE_LENGTH], Image code_image, Image data_image, SymbolTable symbol_table, ExternalLines external_lines, int ICF, int DCF)
{
    output_object(filename, code_image, data_image, ICF, DCF);
    output_entries(filename, symbol_table);
    output_externals(filename, external_lines);
    return TRUE;
}

int output_object(char filename[MAX_LINE_LENGTH], Image code_image, Image data_image, int ICF, int DCF)
{
    char ob_filename[MAX_LINE_LENGTH];
    FILE *fp = NULL;
    ImageLine line;
    int address, i = 0, j = 0;

    strcpy(ob_filename, filename);

    if ((fp = fopen(strcat(ob_filename, ".ob"), "w")) == NULL)
        return FALSE;
    fprintf(fp, "\t %d %d\n", (ICF - START_ADDRESS), (DCF));
    line = code_image->head;
    while (line)
    {
        if (line->address > 0)
        {
            fprintf(fp, "%.4d ", line->address);
            fprintf(fp, "%.2X ", line->binary.four_bytes >> (8 * 0) & 0xFF);
            fprintf(fp, "%.2X ", line->binary.four_bytes >> (8 * 1) & 0xFF);
            fprintf(fp, "%.2X ", line->binary.four_bytes >> (8 * 2) & 0xFF);
            fprintf(fp, "%.2X\n", line->binary.four_bytes >> (8 * 3) & 0xFF);
        }

        line = line->nextLine;
    }

    line = data_image->head;
    if (line)
        address = line->address;

    while (line)
    {
        if (!(j % 4))
            fprintf(fp, "%.4d ", address);

        print_next_data_bin(fp, line, &i);

        if (i >= line->size)
        {
            line = line->nextLine;
            i = 0;
        }

        j++;
        if (!(j % 4))
            fputc('\n', fp);
        address++;
    }

    fclose(fp);
    return TRUE;
}

int output_entries(char filename[MAX_LINE_LENGTH], SymbolTable symbol_table)
{
    char ent_filename[MAX_LINE_LENGTH];
    FILE *fp = NULL;
    SymbolTableLine line = symbol_table->head;

    strcpy(ent_filename, filename);

    while (line)
    {
        if (line->attributes & ENTRY)
        {
            if (!fp)
                if ((fp = fopen(strcat(ent_filename, ".ent"), "w")) == NULL)
                    return FALSE;
            fprintf(fp, "%s %.4d\n", line->symbol, line->value);
        }
        line = line->nextLine;
    }
    if (fp)
        fclose(fp);
    return TRUE;
}

int output_externals(char filename[MAX_LINE_LENGTH], ExternalLines external_lines)
{
    char ext_filename[MAX_LINE_LENGTH];
    FILE *fp = NULL;
    ExternalLine line = external_lines->head;

    strcpy(ext_filename, filename);

    if (line)
    {
        if ((fp = fopen(strcat(ext_filename, ".ext"), "w")) == NULL)
            return FALSE;
    }

    while (line)
    {
        fprintf(fp, "%s %.4d\n", line->symbol, line->val);

        line = line->next_line;
    }
    if (fp)
        fclose(fp);
    return TRUE;
}

void print_next_data_bin(FILE *fp, ImageLine line, int *i)
{
    fprintf(fp, "%.2X ", line->binary.four_bytes >> (8 * (*i)++) & 0xFF);
}

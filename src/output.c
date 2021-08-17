#include "output.h"

int output_files(char filename[MAX_LINE_LENGTH],
                 LinkedList code_image,
                 LinkedList data_image,
                 LinkedList symbol_table,
                 LinkedList external_lines,
                 int ICF, int DCF)
{
    output_object(filename, code_image, data_image, ICF, DCF);
    output_entries(filename, symbol_table);
    output_externals(filename, external_lines);
    return TRUE;
}

int output_object(char filename[MAX_LINE_LENGTH],
                  LinkedList code_image,
                  LinkedList data_image,
                  int ICF, int DCF)
{
    char ob_filename[MAX_LINE_LENGTH];
    FILE *fp = NULL;
    Node node = code_image->head;
    ImageLine line;
    int i = 0, j = 0;

    strcpy(ob_filename, filename);

    if ((fp = fopen(strcat(ob_filename, ".ob"), "w")) == NULL)
        return FALSE;
    fprintf(fp, "\t %d %d\n", (ICF - START_ADDRESS), (DCF));
    while (node)
    {
        line = node->data;
        if (line->address > 0)
        {
            fprintf(fp, "%.4d ", line->address);
            fprintf(fp, "%.2X ", line->binary->four_bytes >> (8 * 0) & 0xFF);
            fprintf(fp, "%.2X ", line->binary->four_bytes >> (8 * 1) & 0xFF);
            fprintf(fp, "%.2X ", line->binary->four_bytes >> (8 * 2) & 0xFF);
            fprintf(fp, "%.2X\n", line->binary->four_bytes >> (8 * 3) & 0xFF);
        }

        node = node->next;
    }

    node = data_image->head;

    while (node)
    {
        line = node->data;
        if (!(j % 4))
            fprintf(fp, "%.4d ", line->address);

        print_next_data_bin(fp, line, &i);

        if (i >= line->size)
        {
            node = node->next;
            i = 0;
        }

        j++;
        if (!(j % 4))
            fputc('\n', fp);
    }

    fclose(fp);
    return TRUE;
}

int output_entries(char filename[MAX_LINE_LENGTH], LinkedList symbol_table)
{
    char ent_filename[MAX_LINE_LENGTH];
    FILE *fp = NULL;
    Node node = symbol_table->head;
    SymbolTableLine line;

    strcpy(ent_filename, filename);

    while (node)
    {
        line = node->data;
        if (line->attributes.entry)
        {
            if (!fp)
                if ((fp = fopen(strcat(ent_filename, ".ent"), "w")) == NULL)
                    return FALSE;
            fprintf(fp, "%s %.4d\n", line->symbol, line->value);
        }
        node = node->next;
    }
    if (fp)
        fclose(fp);
    return TRUE;
}

int output_externals(char filename[MAX_LINE_LENGTH], LinkedList external_lines)
{
    char ext_filename[MAX_LINE_LENGTH];
    FILE *fp = NULL;
    Node node = external_lines->head;
    ExternalLine line;

    strcpy(ext_filename, filename);

    if (node)
    {
        if ((fp = fopen(strcat(ext_filename, ".ext"), "w")) == NULL)
            return FALSE;
    }

    while (node)
    {
        line = node->data;
        fprintf(fp, "%s %.4d\n", line->symbol, line->val);

        node = node->next;
    }
    if (fp)
        fclose(fp);
    return TRUE;
}

void print_next_data_bin(FILE *fp, ImageLine line, int *i)
{
    fprintf(fp, "%.2X ", line->binary->four_bytes >> (8 * (*i)++) & 0xFF);
}

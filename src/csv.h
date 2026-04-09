#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 8192
#define MAX_FIELDS 64

typedef struct {
    char *fields[MAX_FIELDS];
    int field_count;
} CSVRow;

typedef struct {
    CSVRow *rows;
    int count;
    int capacity;
} CSVData;

CSVData* csv_load(const char *filename);
void csv_save(const char *filename, CSVData *data);
void csv_free(CSVData *data);
bool csv_add_row(const char *filename, const char **fields, int count);
bool csv_update_row(const char *filename, int id, const char *field, const char *value);
bool csv_delete_row(const char *filename, int id);
CSVRow* csv_find_by_id(CSVData *data, int id);
CSVData* csv_search(CSVData *data, int field_idx, const char *value);
int csv_get_next_id(const char *filename);

#endif

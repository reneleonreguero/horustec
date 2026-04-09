#include "csv.h"
#include <ctype.h>

static char* strcasestr_local(const char *haystack, const char *needle) {
    if(!*needle) return (char*)haystack;
    for(; *haystack; haystack++) {
        const char *h = haystack, *n = needle;
        while(*h && *n && (tolower(*h) == tolower(*n))) { h++; n++; }
        if(!*n) return (char*)haystack;
    }
    return NULL;
}

static char* trim(char *str) {
    while(*str == ' ') str++;
    char *end = str + strlen(str) - 1;
    while(end > str && (*end == ' ' || *end == '\n' || *end == '\r')) *end-- = '\0';
    return str;
}

static void parse_line(const char *line, CSVRow *row) {
    row->field_count = 0;
    char *copy = strdup(line);
    char *token = strtok(copy, ",");
    while(token && row->field_count < MAX_FIELDS) {
        row->fields[row->field_count++] = strdup(trim(token));
        token = strtok(NULL, ",");
    }
    free(copy);
}

CSVData* csv_load(const char *filename) {
    FILE *fp = fopen(filename, "r");
    CSVData *data = malloc(sizeof(CSVData));
    data->rows = NULL;
    data->count = 0;
    data->capacity = 0;
    
    if(!fp) return data;
    
    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line), fp)) {
        if(line[0] == '\n' || line[0] == '#') continue;
        line[strcspn(line, "\n")] = 0;
        if(data->count >= data->capacity) {
            data->capacity = data->capacity ? data->capacity * 2 : 16;
            data->rows = realloc(data->rows, data->capacity * sizeof(CSVRow));
        }
        parse_line(line, &data->rows[data->count++]);
    }
    fclose(fp);
    return data;
}

void csv_save(const char *filename, CSVData *data) {
    FILE *fp = fopen(filename, "w");
    if(!fp) return;
    for(int i = 0; i < data->count; i++) {
        for(int j = 0; j < data->rows[i].field_count; j++) {
            fprintf(fp, "%s%s", j ? "," : "", data->rows[i].fields[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void csv_free(CSVData *data) {
    if(!data) return;
    for(int i = 0; i < data->count; i++) {
        for(int j = 0; j < data->rows[i].field_count; j++) {
            free(data->rows[i].fields[j]);
        }
    }
    free(data->rows);
    free(data);
}

bool csv_add_row(const char *filename, const char **fields, int count) {
    FILE *fp = fopen(filename, "a");
    if(!fp) return false;
    for(int i = 0; i < count; i++) {
        fprintf(fp, "%s%s", i ? "," : "", fields[i]);
    }
    fprintf(fp, "\n");
    fclose(fp);
    return true;
}

bool csv_update_row(const char *filename, int id, const char *field, const char *value) {
    CSVData *data = csv_load(filename);
    if(!data) return false;
    
    int field_idx = atoi(field);
    bool found = false;
    
    for(int i = 0; i < data->count; i++) {
        if(atoi(data->rows[i].fields[0]) == id) {
            if(field_idx >= 0 && field_idx < data->rows[i].field_count) {
                free(data->rows[i].fields[field_idx]);
                data->rows[i].fields[field_idx] = strdup(value);
                found = true;
            }
            break;
        }
    }
    
    if(found) csv_save(filename, data);
    csv_free(data);
    return found;
}

bool csv_delete_row(const char *filename, int id) {
    CSVData *data = csv_load(filename);
    if(!data) return false;
    
    int idx = -1;
    for(int i = 0; i < data->count; i++) {
        if(atoi(data->rows[i].fields[0]) == id) {
            idx = i;
            break;
        }
    }
    
    if(idx >= 0) {
        for(int j = 0; j < data->rows[idx].field_count; j++) {
            free(data->rows[idx].fields[j]);
        }
        for(int i = idx; i < data->count - 1; i++) {
            data->rows[i] = data->rows[i + 1];
        }
        data->count--;
        csv_save(filename, data);
        csv_free(data);
        return true;
    }
    csv_free(data);
    return false;
}

CSVRow* csv_find_by_id(CSVData *data, int id) {
    for(int i = 0; i < data->count; i++) {
        if(atoi(data->rows[i].fields[0]) == id) {
            return &data->rows[i];
        }
    }
    return NULL;
}

CSVData* csv_search(CSVData *data, int field_idx, const char *value) {
    CSVData *result = malloc(sizeof(CSVData));
    result->rows = NULL;
    result->count = 0;
    result->capacity = 0;
    
    for(int i = 0; i < data->count; i++) {
        bool match = false;
        if(field_idx >= 0 && field_idx < data->rows[i].field_count) {
            match = strcasestr_local(data->rows[i].fields[field_idx], value) != NULL;
        }
        if(match) {
            if(result->count >= result->capacity) {
                result->capacity = result->capacity ? result->capacity * 2 : 16;
                result->rows = realloc(result->rows, result->capacity * sizeof(CSVRow));
            }
            result->rows[result->count++] = data->rows[i];
        }
    }
    return result;
}

int csv_get_next_id(const char *filename) {
    CSVData *data = csv_load(filename);
    int next = data->count + 1;
    csv_free(data);
    return next;
}

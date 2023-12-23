#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "record.h"
#include "id_query.h"

struct index_record {
    int64_t osm_id;
    const struct record *record;
};

struct indexed_data {
    struct index_record *irs;
    int n;
};

struct indexed_data* mk_indexed(struct record* rs, int n) {
    struct indexed_data *data = malloc(sizeof(struct indexed_data));
    if (!data) return NULL;

    data->irs = malloc(sizeof(struct index_record) * n);
    if (!data->irs) {
        free(data);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        data->irs[i].osm_id = rs[i].osm_id;
        data->irs[i].record = &rs[i];
    }
    
    data->n = n;
    return data;
}

void free_indexed(struct indexed_data *data) {
    if (data) {
        free(data->irs);
        free(data); 
    }
}

const struct record* lookup_indexed(struct indexed_data *data, int64_t needle) {
    for (int i = 0; i < data->n; i++) {
        if (data->irs[i].osm_id == needle) {
            return data->irs[i].record;
        }
    }
    return NULL;
}

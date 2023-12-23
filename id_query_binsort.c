#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "record.h"
#include "id_query.h"

struct id_lookup {
    int64_t identifier;
    const struct record *rec_ptr;
};

struct id_collection {
    struct id_lookup *lookup_array;
    int count;
};

const struct record* binary_search(const struct record* records, int n, int64_t needle) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (records[mid].osm_id == needle) return &records[mid];
        if (records[mid].osm_id < needle) low = mid + 1;
        else high = mid - 1;
    }
    return NULL;
}

int id_comparison(const void *a, const void *b) {
    struct id_lookup *id1 = (struct id_lookup *)a;
    struct id_lookup *id2 = (struct id_lookup *)b;
    return id1->identifier - id2->identifier;
}

struct id_collection *initialize_index(struct record *records, int size) {
    struct id_collection *collection = malloc(sizeof(struct id_collection));
    collection->lookup_array = malloc(sizeof(struct id_lookup) * size);
    for (int i = 0; i < size; ++i) {
        collection->lookup_array[i].rec_ptr = &records[i];
        collection->lookup_array[i].identifier = records[i].osm_id;
    }
    collection->count = size;
    qsort(collection->lookup_array, size, sizeof(struct id_lookup), id_comparison);
    return collection;
}

void release_index(struct id_collection *col) {
    free(col->lookup_array);
    free(col);
}

const struct record* search_by_id(struct id_collection* col, int64_t id) {
    return binary_search(col->lookup_array, col->count, id);
}

int main(int argc, char **argv) {
    return id_query_loop(argc, argv,
                         (mk_index_fn)initialize_index,
                         (free_index_fn)release_index,
                         (lookup_fn)search_by_id);
}
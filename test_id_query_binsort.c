#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "id_query.h"

typedef struct {
    int64_t query_id;
    const char* expected_name;
} TestCase;

int compare_records(const void* a, const void* b) {
    const struct record* record_a = (const struct record*) a;
    const struct record* record_b = (const struct record*) b;
    if (record_a->osm_id < record_b->osm_id) return -1;
    if (record_a->osm_id > record_b->osm_id) return 1;
    return 0;
}

// Let me cook
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

int main() {
    int n;
    struct record* records = read_records("20000_records.tsv", &n);
    if (!records) {
        fprintf(stderr, "What records???\n");
        return 1;
    }

    qsort(records, n, sizeof(struct record), compare_records);

    TestCase first_three_tests[4];
    for (int i = 0; i < 4; i++) {
        first_three_tests[i].query_id = records[i].osm_id;
        first_three_tests[i].expected_name = records[i].name;
    }

    for (int i = 0; i < 4; i++) {
        const struct record* result = binary_search(records, n, first_three_tests[i].query_id);
        int passed = (result != NULL && strcmp(result->name, first_three_tests[i].expected_name) == 0);
        printf("Test %d: Query ID %ld, Expected: %s, Result: %s - %s\n",
               i + 1, first_three_tests[i].query_id,
               first_three_tests[i].expected_name,
               result ? result->name : "Not Found",
               passed ? "PASSED" : "FAILED");
    }

    free_records(records, n);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "id_query.h"
#include "id_query_indexed.h"

typedef struct {
    int64_t query_id;
    const char* expected_name;
} TestCase;

int main() {
    int n;
    struct record* records = read_records("20000_records.tsv", &n);
    if (!records) {
        fprintf(stderr, "What records???\n");
        return 1;
    }

    struct indexed_data* indexed = mk_indexed(records, n);

    TestCase tests[] = {
        {2202162, "France"},
        {51477, "Germany"},
        {365331, "Italy"},
        {9999999, NULL}
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        const struct record* result = lookup_indexed(indexed, tests[i].query_id);
        int passed = (result != NULL && strcmp(result->name, tests[i].expected_name) == 0) ||
                     (result == NULL && tests[i].expected_name == NULL);

        printf("Test %d: Query ID %ld, Expected: %s, Result: %s - %s\n",
               i + 1, tests[i].query_id,
               tests[i].expected_name ? tests[i].expected_name : "Not Found",
               result ? result->name : "Not Found",
               passed ? "PASSED" : "FAILED");
    }

    free_indexed(indexed);
    free_records(records, n);
    return 0;
}

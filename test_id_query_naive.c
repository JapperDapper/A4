#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "id_query.h"

typedef struct {
    int64_t query_id;
    const char* expected_name;
} TestCase;

const struct record* naive_lookup(struct record* records, int n, int64_t id);

int main() {
    int n;
    struct record* records = read_records("20000_records.tsv", &n);
    if (!records) {
        fprintf(stderr, "Records where are yooou?\n");
        return 1;
    }

    TestCase tests[] = {
        {2202162, "France"}, 
        {51477, "Germany"}, 
        {365331, "Italy"},  
        {9999999, NULL}    
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        TestCase test = tests[i];
        const struct record* result = naive_lookup(records, n, test.query_id);

        int passed = 0;
        if (result != NULL && test.expected_name != NULL) {
            passed = strcmp(result->name, test.expected_name) == 0;
        } else if (result == NULL && test.expected_name == NULL) {
            passed = 1;
        }

        printf("Test %d: Query ID %ld, Expected: %s, Result: %s - %s\n",
               i + 1, test.query_id,
               test.expected_name ? test.expected_name : "Not Found",
               result ? result->name : "Not Found",
               passed ? "PASSED" : "FAILED");
    }

    free_records(records, n);
    return 0;
}

const struct record* naive_lookup(struct record* records, int n, int64_t id) {
    for (int i = 0; i < n; i++) {
        if (records[i].osm_id == id) {
            return &records[i];
        }
    }
    return NULL;
}

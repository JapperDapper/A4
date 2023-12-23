#include <stdio.h>
#include <stdlib.h>
#include "record.h"
#include "coord_query.h"

typedef struct {
    double lon;
    double lat;
    const char* expected_name;
} TestCase;

struct naive_data* mk_naive(struct record* rs, int n);
const struct record* lookup_naive(struct naive_data *data, double lon, double lat);
void free_naive(struct naive_data* data);

int main() {
    int n;
    struct record* records = read_records("20000_records.tsv", &n);
    if (!records) {
        fprintf(stderr, "What records???\n");
        return 1;
    }

    struct naive_data* data = mk_naive(records, n);

    TestCase tests[] = {
        {1.8753098, 46.7995347, "France"},
        {-99.0090672, 36.8184300, "United States of America"},
        {10.5917075, 51.1850012, "Germany"},
        {14.1885945, 41.7035189, "Italy"},
        {142.9514467, 43.4339535, "Japan"},
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        const struct record* result = lookup_naive(data, tests[i].lon, tests[i].lat);
        const char* result_name = result ? result->name : "Godt forsøg men nej";
        printf("Test %d: Expected: %s, Result: %s\n", 
               i + 1, tests[i].expected_name, result_name);
    }

    free_naive(data);
    free_records(records, n);
    return 0;
}

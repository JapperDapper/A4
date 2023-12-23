#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <float.h> 

#include "record.h"
#include "coord_query.h"

struct naive_data {
    struct record *rs;
    int n;
};

double euclidean_distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

struct naive_data* mk_naive(struct record* rs, int n) {
    struct naive_data* data = malloc(sizeof(struct naive_data));
    if (!data) return NULL;

    data->rs = rs;
    data->n = n;

    return data;
}

void free_naive(struct naive_data* data) {
    if (data) {
        free(data);
    }
}

const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
    if (!data) return NULL;

    const struct record* closest = NULL;
    double min_distance = DBL_MAX;

    for (int i = 0; i < data->n; i++) {
        double distance = euclidean_distance(lon, lat, data->rs[i].lon, data->rs[i].lat);
        if (distance < min_distance) {
            min_distance = distance;
            closest = &data->rs[i];
        }
    }

    return closest;
}
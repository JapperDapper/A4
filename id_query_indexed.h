#ifndef ID_QUERY_INDEXED_H
#define ID_QUERY_INDEXED_H

#include "record.h"

struct indexed_data* mk_indexed(struct record* rs, int n);
void free_indexed(struct indexed_data* data);
const struct record* lookup_indexed(struct indexed_data* data, int64_t needle);

#endif

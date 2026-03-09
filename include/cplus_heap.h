#ifndef _CPLUS_HEAP_H_
#define _CPLUS_HEAP_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct cplus_heap_entry_t
{
    uint32_t cost;
} cplus_heap_entry_t;

typedef struct cplus_heap_desc_t
{
    void *data;
    uint32_t data_size;
    uint32_t max_size;
    uint16_t last_idx;
} cplus_heap_desc_t;

bool cplus_heap_init(cplus_heap_desc_t *desc);
bool cplus_heap_push(cplus_heap_desc_t *desc, void *element);
bool cplus_heap_pop(cplus_heap_desc_t *desc, void *out_element);
bool cplus_heap_empty(const cplus_heap_desc_t *desc);
void cplus_heap_reset(cplus_heap_desc_t *desc);

#endif
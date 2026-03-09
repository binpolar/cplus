#ifndef _CPLUS_HASHMAP_H_
#define _CPLUS_HASHMAP_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct cplus_hashmap_entry_t
{
    uint32_t key;
    bool is_valid;

} cplus_hashmap_entry_t;

typedef struct cplus_hashmap_desc_t
{
    void *data;
    uint32_t data_size;
    uint32_t data_length;

} cplus_hashmap_desc_t;

bool cplus_hashmap_init(cplus_hashmap_desc_t *desc);

bool cplus_hashmap_full();
void *cplus_hashmap_get(const cplus_hashmap_desc_t *desc, uint32_t key);
bool cplus_hashmap_put(cplus_hashmap_desc_t *desc, void *value);
bool cplus_hashmap_remove(cplus_hashmap_desc_t *desc, uint32_t key);
bool cplus_hashmap_exists(const cplus_hashmap_desc_t *desc, uint32_t key);

#endif
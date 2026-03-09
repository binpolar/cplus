#include "cplus_hashmap.h"
#include "cplus_utils.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define HASH_IDX_TO_PTR(desc, idx) \
    ((cplus_hashmap_entry_t *)((uint8_t *)((desc)->data) + ((idx) * (desc)->data_size)))


bool cplus_hashmap_init(cplus_hashmap_desc_t *desc)
{
    if (!desc || !desc->data || desc->data_length == 0)
    {
        return false;
    }
    memset(desc->data, 0, desc->data_size * desc->data_length);
    return true;
}

static void *cplus_hashmap_find_slot(const cplus_hashmap_desc_t *desc,
                                     uint32_t key,
                                     bool for_insert)
{
    uint32_t idx = cplus_hash_func(key);
    uint32_t size = desc->data_length;
    uint32_t start = idx % size;
    uint32_t current = start;
    uint32_t first_empty = size; // Sentinel for no empty slot

    do
    {
 
        cplus_hashmap_entry_t *e = HASH_IDX_TO_PTR(desc, current);

        if (e->is_valid && e->key == key)
        {
            return (void*)(e); // Found matching key
        }

        if (for_insert && !e->is_valid && first_empty == size)
        {
            first_empty = current; // Remember first empty slot
        }

        current = (current + 1) % size;
    } while (current != start);

    if (for_insert && first_empty != size)
    {
        return (void*)HASH_IDX_TO_PTR(desc, first_empty);
    }

    return NULL; // Not found or no empty slot
}

void *cplus_hashmap_get(const cplus_hashmap_desc_t *desc, uint32_t key)
{
    if (!desc)
        return NULL;
    return cplus_hashmap_find_slot(desc, key, false);
}

bool cplus_hashmap_put(cplus_hashmap_desc_t *desc, void *value)
{
    if (!desc || !value)
        return false;

    cplus_hashmap_entry_t *val = (cplus_hashmap_entry_t *)value;
    void *slot = cplus_hashmap_find_slot(desc, val->key, true);

    if (slot)
    {
        memcpy(slot, value, desc->data_size);
        ((cplus_hashmap_entry_t *)slot)->is_valid = 1;
        return true;
    }

    return false; // Table full
}

bool cplus_hashmap_exists(const cplus_hashmap_desc_t *desc, uint32_t key)
{
    return cplus_hashmap_get(desc, key) != NULL;
}

bool cplus_hashmap_remove(cplus_hashmap_desc_t *desc, uint32_t key)
{
    if (!desc)
        return false;

    cplus_hashmap_entry_t *entry = cplus_hashmap_get(desc, key);
    if (entry)
    {
        entry->is_valid = 0;
        // Optional: Consider using a tombstone marker instead of just 0
        // to differentiate from never-used slots during insertion
        return true;
    }
    return false;
}
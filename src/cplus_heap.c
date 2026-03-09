#include "cplus_heap.h"
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#define HEAP_IDX_TO_PTR(desc, idx) \
    ((cplus_heap_entry_t *)((uint8_t *)((desc)->data) + ((idx) * (desc)->data_size)))

#define HEAP_GET_PARENT_IDX(x) ((x) >> 1)
#define HEAP_GET_LEFT_CHILD_IDX(x) ((x) << 1)
#define HEAP_GET_RIGHT_CHILD_IDX(x) (((x) << 1) + 1)

static inline uint32_t get_cost(cplus_heap_entry_t *element)
{
    return (element->cost);
}

static void heap_swap(cplus_heap_desc_t *desc, uint16_t i, uint16_t j)
{
    cplus_heap_entry_t *a = HEAP_IDX_TO_PTR(desc, i);
    cplus_heap_entry_t *b = HEAP_IDX_TO_PTR(desc, j);
    uint8_t temp[desc->data_size];

    memcpy(temp, a, desc->data_size);
    memcpy(a, b, desc->data_size);
    memcpy(b, temp, desc->data_size);
}

bool cplus_heap_init(cplus_heap_desc_t *desc)
{
    if (!desc || !desc->data || desc->max_size == 0 || desc->data_size == 0) {
        return false;
    }
    
    desc->last_idx = 1; // 1-based indexing, 0 means empty
    memset(desc->data, 0, desc->data_size * desc->max_size);
    return true;
}

bool cplus_heap_push(cplus_heap_desc_t *desc, void *element)
{
    if (!desc || !element) return false;
    
    if (desc->last_idx >= desc->max_size) {
        return false; // Heap full
    }

    void *dest = HEAP_IDX_TO_PTR(desc, desc->last_idx);
    memcpy(dest, element, desc->data_size);

    // Bubble up
    uint16_t idx = desc->last_idx;
    
    while (idx > 1) {
        uint16_t parent_idx = HEAP_GET_PARENT_IDX(idx);
        void *current = HEAP_IDX_TO_PTR(desc, idx);
        void *parent = HEAP_IDX_TO_PTR(desc, parent_idx);

        if (get_cost(current) < get_cost(parent)) {
            heap_swap(desc, idx, parent_idx);
            idx = parent_idx;
        } else {
            break;
        }
    }

    desc->last_idx++;
    return true;
}

bool cplus_heap_pop(cplus_heap_desc_t *desc, void *out_element)
{
    if (!desc || !out_element) return false;
    
    if (desc->last_idx <= 1) {
        return false; // Heap empty
    }

    // Copy root to output
    void *root = HEAP_IDX_TO_PTR(desc, 1);
    memcpy(out_element, root, desc->data_size);

    // Move last element to root
    desc->last_idx--;
    void *last = HEAP_IDX_TO_PTR(desc, desc->last_idx);
    
    if (desc->last_idx > 1) { // Only copy if there are remaining elements
        memcpy(root, last, desc->data_size);

        // Bubble down
        uint16_t idx = 1;
        
        while (1) {
            uint16_t left_idx = HEAP_GET_LEFT_CHILD_IDX(idx);
            uint16_t right_idx = HEAP_GET_RIGHT_CHILD_IDX(idx);
            uint16_t smallest = idx;
            
            uint32_t current_cost = get_cost(HEAP_IDX_TO_PTR(desc, idx));

            // Check left child
            if (left_idx < desc->last_idx) {
                void *left = HEAP_IDX_TO_PTR(desc, left_idx);
                uint32_t left_cost = get_cost(left);
                if (left_cost < current_cost) {
                    smallest = left_idx;
                }
            }

            uint32_t smallest_cost = (smallest == idx) ? current_cost : 
                                     get_cost(HEAP_IDX_TO_PTR(desc, smallest));
            
            if (right_idx < desc->last_idx) {
                void *right = HEAP_IDX_TO_PTR(desc, right_idx);
                uint32_t right_cost = get_cost(right);
                if (right_cost < smallest_cost) {
                    smallest = right_idx;
                }
            }

            if (smallest != idx) {
                heap_swap(desc, idx, smallest);
                idx = smallest;
            } else {
                break;
            }
        }
    }
    
    return true;
}

bool cplus_heap_empty(const cplus_heap_desc_t *desc)
{
    return !desc || desc->last_idx == 1;
}

void cplus_heap_reset(cplus_heap_desc_t *desc)
{
    if (desc) {
        desc->last_idx = 1;
        memset(desc->data, 0, desc->data_size * desc->max_size);
    }
}
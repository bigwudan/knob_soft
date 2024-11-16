#ifndef UG_MEM_H
#define UG_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "ugui/ugui.h"

typedef struct {
    uint32_t total_size; /**< Total heap size */
    uint32_t free_cnt;
    uint32_t free_size; /**< Size of available memory */
    uint32_t free_biggest_size;
    uint32_t used_cnt;
    uint32_t max_used; /**< Max size of Heap memory used */
    uint8_t used_pct; /**< Percentage used */
    uint8_t frag_pct; /**< Amount of fragmentation */
} ug_mem_monitor_t;

EXPORT_API void ug_mem_init(MEM_UNIT *heap_addr, uint32_t heap_size);
EXPORT_API void ug_mem_deinit(void);
EXPORT_API void * ug_mem_alloc(size_t size);
EXPORT_API void ug_mem_free(const void * data);
EXPORT_API void * ug_mem_realloc(void * data_p, size_t new_size);
EXPORT_API uint32_t _ug_mem_get_size(const void* data);

#ifdef WIN32
EXPORT_API void ug_mem_monitor(ug_mem_monitor_t* mon_p);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*UG_MEM_H*/

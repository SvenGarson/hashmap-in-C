#ifndef HASHMAP_GENERIC_H
#define HASHMAP_GENERIC_H

/* Includes */
#include <stdlib.h>

/* Constants */
static const float HASHMAP_GENERIC_LOAD_FACTOR = 0.75f;

/* Datatypes */
typedef enum {
	HASHMAP_GENERIC_FALSE = 0,
	HASHMAP_GENERIC_TRUE = 1
} hashmap_generic_bool_te;

typedef struct {
	void * p_data;
	size_t data_size;
} hashmap_generic_data_ts;

typedef struct hashmap_generic_bucket {
	hashmap_generic_data_ts key;
	hashmap_generic_data_ts value;
	struct hashmap_generic_bucket * p_next;
} hashmap_generic_bucket_ts;

typedef struct {
	int entry_count;
	int bucket_count;
	hashmap_generic_bucket_ts ** pp_buckets;
} hashmap_generic_instance_ts;

/* Interface function prototypes - Creationg and destroyal */
hashmap_generic_instance_ts * hashmap_generic_create(int number_of_initial_buckets);
void hashmap_generic_destroy(hashmap_generic_instance_ts ** pp_hashmap);

/* Interface function prototypes - Debugging */
void hashmap_generic_visualize(const hashmap_generic_instance_ts * p_hashmap, const char * p_tag);

/* Interface function prototypes - Insertion; Retrieval; Deletion */
hashmap_generic_bool_te hashmap_generic_set(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size,
	const void * const p_value,
	size_t value_size
);

hashmap_generic_bool_te hashmap_generic_delete(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size
);

const void * hashmap_generic_get(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size
);

typedef struct {
	int bucket_index;
	hashmap_generic_instance_ts * p_hashmap;
	hashmap_generic_bucket_ts * p_current_bucket_node;
} hashmap_generic_iterator_ts;

typedef struct {
	const void * const p_key;
	const size_t key_size;
	const void * const p_value;
	const size_t value_size;
} hashmap_generic_iterator_entry;

void hashmap_generic_iterator(
	hashmap_generic_instance_ts * p_hashmap,
	hashmap_generic_iterator_ts * p_iterator
);

hashmap_generic_bool_te hashmap_generic_iterator_has_next(
	hashmap_generic_iterator_ts * p_iterator
);

hashmap_generic_iterator_entry hashmap_generic_iterator_get_next(
	hashmap_generic_iterator_ts * p_iterator
);

#endif
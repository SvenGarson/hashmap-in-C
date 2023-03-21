#ifndef HASHMAP_GENERIC_H
#define HASHMAP_GENERIC_H

/* Includes */
#include <stdlib.h>

/* Defined */
#define HASHMAP_GENERIC_NUMBER_OF_STARTING_BUCKETS (1)

/* Datatypes */
typedef struct {
	void * p_data;
	size_t data_size;
} hashmap_generic_data_ts;

typedef struct hashmap_generic_bucket {
	hashmap_generic_data_ts key;
	void * p_value;
	struct hashmap_generic_bucket * p_next;
} hashmap_generic_bucket_ts;

typedef struct {
	/* Hold references needed for the bucket array */
	int bucket_count;
	hashmap_generic_bucket_ts * p_buckets;
} hashmap_generic_instance_ts;

/* TODO-GS: Split into header and source file + add null guards everywhere */
/* Interface function prototypes - Creationg and destroyal */
hashmap_generic_instance_ts * hashmap_generic_create(void);
void hashmap_generic_destroy(hashmap_generic_instance_ts ** pp_hashmap);

/* Interface function prototypes - Debugging */
void hashmap_generic_visualize(const hashmap_generic_instance_ts * p_hashmap, const char * p_tag);

/* Interface function prototypes - Insert; Retrieval; Deletion */
void hashmap_generic_set(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size,
	const void * const p_value,
	size_t value_size
);
void dhashmap_generic_delete(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size
);
void hashmap_generic_get(void);

#endif
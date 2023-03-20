#ifndef HASHMAP_GENERIC_H
#define HASHMAP_GENERIC_H

/* Includes */
#include <stdlib.h>

/* Defined */
#define HASHMAP_GENERIC_NUMBER_OF_STARTING_BUCKETS (5)

/* Datatypes */
typedef struct hashmap_generic_bucket {
	void * p_key;
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
static hashmap_generic_instance_ts * hashmap_generic_create(void)
{
	/* Allocate generic hashmap instance */
	hashmap_generic_instance_ts * const p_new_generic_hashmap = malloc(sizeof(hashmap_generic_instance_ts));
	if (p_new_generic_hashmap == NULL)
	{
		return NULL;
	}

	/* Success allocating the hashmap instance - Now allocate and initialize the hashmap buckets */
	p_new_generic_hashmap->bucket_count = HASHMAP_GENERIC_NUMBER_OF_STARTING_BUCKETS;
	p_new_generic_hashmap->p_buckets = malloc(sizeof(hashmap_generic_bucket_ts) * p_new_generic_hashmap->bucket_count);
	if (p_new_generic_hashmap->p_buckets == NULL)
	{
		free(p_new_generic_hashmap);
		return NULL;
	}

	/* Success allocating the buckets - Now initialize and return the rest of the hashmap instance */
	for (int bucket_index = 0; bucket_index < p_new_generic_hashmap->bucket_count; bucket_index++)
	{
		/* Bucket node key of value NULL represents an un-used bucket node */
		p_new_generic_hashmap->p_buckets[bucket_index].p_key = NULL;
		p_new_generic_hashmap->p_buckets[bucket_index].p_value = NULL;
		p_new_generic_hashmap->p_buckets[bucket_index].p_next = NULL;
	}

	return p_new_generic_hashmap;
}

static void hashmap_generic_destroy(hashmap_generic_instance_ts ** pp_hashmap)
{
	/* Deallocate all buckets along with the corresponding key and value */
	/* TODO-GS: Do that for the buckets */

	/* Deallocate the hashmap instance */
	free(*pp_hashmap);
	*pp_hashmap = NULL;
}

/* Interface function prototypes - Debugging */
void hashmap_generic_visualize(const hashmap_generic_instance_ts * p_hashmap, const char * p_tag)
{
	printf("\n\nVisulizing generic hashmap tagged '%s':", p_tag);
	for (int bucket_index = 0; bucket_index < p_hashmap->bucket_count; bucket_index++)
	{
		const hashmap_generic_bucket_ts * p_current_bucket = p_hashmap->p_buckets + bucket_index;
		printf("\n\tBucket %4d/%-4d", bucket_index, p_hashmap->bucket_count);
		for (int bucket_entry_index = 0; p_current_bucket != NULL; p_current_bucket = p_current_bucket->p_next, bucket_entry_index++)
		{
			printf("\n\t  %-4d p_key: %-16p p_value: %-16p", bucket_entry_index, p_current_bucket->p_key, p_current_bucket->p_value);
		}
	}
	printf("\n");
}

/* Interface function prototypes - Insert; Retrieval; Deletion */
void hashmap_generic_set(void);
void hashmap_generic_get(void);
void hashmap_generic_delete(void);

#endif
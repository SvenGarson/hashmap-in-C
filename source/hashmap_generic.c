#include "..\include\hashmap_generic.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Private helper function definitions */
static uint32_t fnv32_hash(const char *str, size_t len)
{
    unsigned char *s = (unsigned char *)str;	/* unsigned string */

    const uint32_t FNV_32_PRIME = 0x01000193; /* 16777619 */

    uint32_t h = 0x811c9dc5; /* 2166136261 */
    while (len--) {
        /* xor the bottom with the current octet */
        h ^= *s++;
        /* multiply by the 32 bit FNV magic prime mod 2^32 */
        h *= FNV_32_PRIME;
    }

    return h;
}

static hashmap_generic_bucket_ts * create_bucket_node_with_data(
	void * p_key_copy,
	size_t key_size,
	void * p_value_copy
)
{
	hashmap_generic_bucket_ts * p_new_node = malloc(sizeof(hashmap_generic_bucket_ts));
	if (p_new_node == NULL)
	{
		return NULL;
	}

	/* Bucket chain node created successfully - Now initialize and return the node */
	p_new_node->key.p_data = p_key_copy;
	p_new_node->key.data_size = key_size;
	p_new_node->p_value = p_value_copy;
	p_new_node->p_next = NULL;

	return p_new_node;
}

/* Interface function definitions */
hashmap_generic_instance_ts * hashmap_generic_create(void)
{
	/* Allocate generic hashmap instance */
	hashmap_generic_instance_ts * const p_new_generic_hashmap = malloc(sizeof(hashmap_generic_instance_ts));
	if (p_new_generic_hashmap == NULL)
	{
		return NULL;
	}

	/* Success allocating the hashmap instance - Now allocate and initialize the hashmap buckets */
	p_new_generic_hashmap->bucket_count = HASHMAP_GENERIC_NUMBER_OF_STARTING_BUCKETS;
	p_new_generic_hashmap->pp_buckets = malloc(sizeof(hashmap_generic_bucket_ts *) * p_new_generic_hashmap->bucket_count);
	if (p_new_generic_hashmap->pp_buckets == NULL)
	{
		free(p_new_generic_hashmap);
		return NULL;
	}

	/* Success allocating array of pointers to buckets - Now initialize the buckets to NULL */
	for (int bucket_index = 0; bucket_index < p_new_generic_hashmap->bucket_count; bucket_index++)
		p_new_generic_hashmap->pp_buckets[bucket_index] = NULL;

	return p_new_generic_hashmap;
}

void hashmap_generic_destroy(hashmap_generic_instance_ts ** pp_hashmap)
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
	printf("\n\n# Visualizing generic hashmap tagged '%s':", p_tag);
	unsigned int buckets_logged = 0;
	for (int bucket_index = 0; bucket_index < p_hashmap->bucket_count; bucket_index++)
	{
		const hashmap_generic_bucket_ts * p_current_bucket = p_hashmap->pp_buckets[bucket_index];
		if (p_current_bucket == NULL)
			continue;

		printf("\n\tBucket %4d/%-4d", bucket_index + 1, p_hashmap->bucket_count);
		for (int bucket_entry_index = 0; p_current_bucket != NULL; p_current_bucket = p_current_bucket->p_next, bucket_entry_index++)
		{
			/*printf("\n\t  %-4d p_key: %-40p p_value: %-p", bucket_entry_index, p_current_bucket->key.p_data, p_current_bucket->p_value);*/
			/* TODO-GS: Show the pointers only or apply function through the wrapper? */
			printf("\n\t  %-4s p_key: %-40s p_value: %-d", "", (const char *)p_current_bucket->key.p_data, *((int *)p_current_bucket->p_value));
		}

		buckets_logged++;
	}

	if (0x00 == buckets_logged)
		printf("\n\n\tEmpty");
	printf("\n");
}

/* Interface function prototypes - Insert; Retrieval; Deletion */
void hashmap_generic_set(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size,
	const void * const p_value,
	size_t value_size)
{
	/* TODO-GS: Guard against weird input here */
	const uint32_t key_hash = fnv32_hash((const char *) p_key, key_size);
	const uint32_t hashed_key_bucket_index = key_hash % p_hashmap->bucket_count;
	hashmap_generic_bucket_ts * const p_bucket = p_hashmap->pp_buckets[hashed_key_bucket_index];

	/* Copy key and value memory - TODO-GS: Abstract as create node? */
	void * p_key_copy = malloc(key_size);
	void * p_value_copy = malloc(value_size);
	if (p_key_copy == NULL || p_value_copy == NULL)
	{
		free(p_key_copy);
		free(p_value_copy);

		/* Done since the data cannot be allocated */
		return;
	}
	memcpy(p_key_copy, p_key, key_size);
	memcpy(p_value_copy, p_value, value_size);

	/* Key and value memory copied successfully - Update buckets */
	if (p_bucket == NULL)
	{
		/* Chain empty for this bucket - Initialize the first bucket in the chain */
		/* The bucket chain is currently empty - Create the first bucket chain entry here */
		hashmap_generic_bucket_ts * p_new_bucket = create_bucket_node_with_data(p_key_copy, key_size, p_value_copy);
		if (p_new_bucket != NULL)
			p_hashmap->pp_buckets[hashed_key_bucket_index] = p_new_bucket;

		/* Done */
		return;
	}
	
	/* Chain for this bucket not empty - Check for collisions first */
	hashmap_generic_bucket_ts * p_search_bucket_parent = NULL;
	hashmap_generic_bucket_ts * p_search_bucket = p_bucket;
	for (; p_search_bucket != NULL; p_search_bucket = p_search_bucket->p_next)
	{
		/* TODO-GS: What when the key and/or value are NULL? */
		if (
			p_search_bucket->key.data_size == key_size &&
			memcmp(p_search_bucket->key.p_data, p_key, key_size) == 0
		)
		{
			/* Collision occured - Overwrite the existing bucket value */
			/* The keys are the same, so we do not need to allocate memory for it again, but already have above */
			/* It does not matter which instance the bucket points to */
			free(p_key_copy);
			free(p_search_bucket->p_value);
			p_search_bucket->p_value = p_value_copy;

			/* Done */
			return;
		}

		/* Keep track of parent */
		p_search_bucket_parent = p_search_bucket;
	}

	/* There was no collision - Append a new node to the bucket chain */
	hashmap_generic_bucket_ts * p_new_chain_node = malloc(sizeof(hashmap_generic_bucket_ts));
	if (p_new_chain_node == NULL)
	{
		return;
	}

	p_new_chain_node->key.p_data = p_key_copy;
	p_new_chain_node->key.data_size = key_size;
	p_new_chain_node->p_value = p_value_copy;
	p_new_chain_node->p_next = NULL;

	/* Append to the last node in the current bucket chain */
	p_search_bucket_parent->p_next = p_new_chain_node;
}

void hashmap_generic_delete(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size
)
{
	/* TODO-GS: Again, guard against weird arguments */
	const uint32_t key_hash = fnv32_hash((const char *) p_key, key_size);
	const uint32_t hashed_key_bucket_index = key_hash % p_hashmap->bucket_count;
	hashmap_generic_bucket_ts * p_bucket = p_hashmap->pp_buckets[hashed_key_bucket_index];

	/* Nothing to do for empty bucket */
	if (p_bucket == NULL)
		return;

	/* Look for a collision */
	hashmap_generic_bucket_ts * p_search_bucket_parent = NULL;
	hashmap_generic_bucket_ts * p_search_bucket = p_bucket;
	for(; p_search_bucket != NULL; p_search_bucket = p_search_bucket->p_next)
	{
		/* TODO-GS: Abstract comparison function */
		if (
			p_search_bucket->key.data_size == key_size &&
			memcmp(p_search_bucket->key.p_data, p_key, key_size) == 0
		)
		{
			/* Found chain bucket node to delete */
			hashmap_generic_bucket_ts * p_search_bucket_child = p_search_bucket->p_next;

			/* Deallocate the node to delete */
			free(p_search_bucket->key.p_data);
			free(p_search_bucket->p_value);
			free(p_search_bucket);

			/* Connect deleted node parent to what the deleted node child */
			if (p_search_bucket_parent == NULL)
			{
				/* Delete the first node of the bucket chain */
				p_hashmap->pp_buckets[hashed_key_bucket_index] = p_search_bucket_child;
			}
			else
			{
				/* Delete subsequent node of the bucket chain */
				p_search_bucket_parent->p_next = p_search_bucket_child;
			}
		}

		/* Keep track of parent */
		p_search_bucket_parent = p_search_bucket;
	}
}

const void * hashmap_generic_get(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size
)
{
	/* Return a pointer to the data when the key exists, NULL otherwise */
	/* TODO-GS: Again, guard against weird arguments */
	const uint32_t key_hash = fnv32_hash((const char *) p_key, key_size);
	const uint32_t hashed_key_bucket_index = key_hash % p_hashmap->bucket_count;
	hashmap_generic_bucket_ts * p_bucket = p_hashmap->pp_buckets[hashed_key_bucket_index];

	/* Nothing to do for empty bucket */
	if (p_bucket == NULL)
		return NULL;

	/* Search for desired bucket chain node and return the value stored */
	hashmap_generic_bucket_ts * p_search_bucket = p_bucket;
	for(; p_search_bucket != NULL; p_search_bucket = p_search_bucket->p_next)
	{
		if (
			p_search_bucket->key.data_size == key_size &&
			memcmp(p_search_bucket->key.p_data, p_key, key_size) == 0
		)
		{
			/* Found chain bucket node to return value from */
			return p_search_bucket->p_value;
		}
	}

	/* No corresponding entry found */
	return NULL;
}

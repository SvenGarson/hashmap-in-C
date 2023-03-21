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

static void destroy_bucket_node(hashmap_generic_bucket_ts * p_bucket_node_to_delete)
{
	if (p_bucket_node_to_delete == NULL)
		return;

	free(p_bucket_node_to_delete->key.p_data);
	free(p_bucket_node_to_delete->p_value);
	free(p_bucket_node_to_delete);
}

static hashmap_generic_bool_te nodes_match(
	const hashmap_generic_bucket_ts * const p_bucket,
	const char * p_key,
	size_t key_size
)
{
	if (p_bucket == NULL || key_size == 0x00)
		return HASHMAP_GENERIC_FALSE;

	return (
		p_bucket->key.data_size == key_size &&
		memcmp(p_bucket->key.p_data, p_key, key_size) == 0
	) ? HASHMAP_GENERIC_TRUE : HASHMAP_GENERIC_FALSE;
}

static hashmap_generic_bool_te hashmap_generic_set_entry(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size,
	const void * const p_value,
	size_t value_size)
{
	if (p_hashmap == NULL || p_key == NULL)
		return HASHMAP_GENERIC_FALSE;

	const uint32_t key_hash = fnv32_hash((const char *) p_key, key_size);
	const uint32_t hashed_key_bucket_index = key_hash % p_hashmap->bucket_count;
	hashmap_generic_bucket_ts * const p_bucket = p_hashmap->pp_buckets[hashed_key_bucket_index];

	/* Copy key and value memory */
	void * p_key_copy = malloc(key_size);
	void * p_value_copy = malloc(value_size);
	if (p_key_copy == NULL || p_value_copy == NULL)
	{
		free(p_key_copy);
		free(p_value_copy);

		/* Done since the data cannot be allocated */
		return HASHMAP_GENERIC_FALSE;
	}
	memcpy(p_key_copy, p_key, key_size);
	memcpy(p_value_copy, p_value, value_size);

	/* Key and value memory copied successfully - Update buckets */
	if (p_bucket == NULL)
	{
		/* The bucket chain is currently empty - Create the first bucket chain entry here */
		hashmap_generic_bucket_ts * p_new_bucket = create_bucket_node_with_data(p_key_copy, key_size, p_value_copy);
		if (p_new_bucket != NULL)
		{
			p_hashmap->entry_count++;
			p_hashmap->pp_buckets[hashed_key_bucket_index] = p_new_bucket;
			return HASHMAP_GENERIC_TRUE;
		}

		/* Done initializing the bucket chain */
		return HASHMAP_GENERIC_FALSE;
	}
	
	/* Chain for this bucket not empty - Check for collisions first */
	hashmap_generic_bucket_ts * p_search_bucket_parent = NULL;
	hashmap_generic_bucket_ts * p_search_bucket = p_bucket;
	for (; p_search_bucket != NULL; p_search_bucket = p_search_bucket->p_next)
	{
		if (!nodes_match(p_search_bucket, p_key, key_size))
		{
			/* Keep track of parent */
			p_search_bucket_parent = p_search_bucket;
			continue;
		}
		
		/* Collision occured - Overwrite the existing bucket value */
		free(p_key_copy);
		free(p_search_bucket->p_value);
		p_search_bucket->p_value = p_value_copy;

		/* Done since a key is expected to be unique across a hashmap instance */
		return HASHMAP_GENERIC_TRUE;
	}

	/* There was no collision - Append a new node to the bucket chain */
	hashmap_generic_bucket_ts * p_append_node = create_bucket_node_with_data(p_key_copy, key_size, p_value_copy);
	if (p_append_node != NULL)
	{
		p_hashmap->entry_count++;
		p_search_bucket_parent->p_next = p_append_node;
		return HASHMAP_GENERIC_TRUE;
	}

	return HASHMAP_GENERIC_FALSE;
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
	p_new_generic_hashmap->entry_count = 0;
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
	if (pp_hashmap == NULL || (*pp_hashmap)->bucket_count <= 0)
		return;
	
	/* Deallocate all buckets along with the corresponding key and value */
	hashmap_generic_bucket_ts * p_deletion_bucket = (*pp_hashmap)->pp_buckets[0];
	while (p_deletion_bucket != NULL)
	{
		hashmap_generic_bucket_ts * p_deletion_bucket_child = p_deletion_bucket->p_next;
		destroy_bucket_node(p_deletion_bucket);
		p_deletion_bucket = p_deletion_bucket_child;
	}

	/* Deallocate the hashmap instance */
	free(*pp_hashmap);
	*pp_hashmap = NULL;
}

/* Interface function prototypes - Debugging */
void hashmap_generic_visualize(const hashmap_generic_instance_ts * p_hashmap, const char * p_tag)
{
	if (p_hashmap == NULL)
	{
		printf("\n\nHashmap tagged '%s' is not initialized\n", p_tag ? p_tag : "N/A");
		return;
	}
		
	printf("\n\n# Visualizing generic hashmap tagged '%s' with '%d' entries:", p_tag, p_hashmap->entry_count);

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
			printf("\n\t  %-4s (%3u bytes) p_key: %-40s p_value: %-30s", "", p_current_bucket->key.data_size, (const char *)p_current_bucket->key.p_data, ((char *)p_current_bucket->p_value));
		}

		buckets_logged++;
	}

	if (0x00 == buckets_logged)
		printf("\n\n\tEmpty\n");
	printf("\n");
}

/* Interface function prototypes - Insertion; Retrieval; Deletion */
hashmap_generic_bool_te hashmap_generic_set(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size,
	const void * const p_value,
	size_t value_size)
{
	/* Perform insertion */
	hashmap_generic_bool_te result = hashmap_generic_set_entry(p_hashmap, p_key, key_size, p_value, value_size);

	/* Re-hash the the entire hashmap after doubling the number of buckets when load factor reached */
	const float new_load_factor = (float) p_hashmap->entry_count / (float)p_hashmap->bucket_count;
	printf("\n----------------------> Entries: %-3d Buckets: %-3d Load factor: %f", p_hashmap->entry_count, p_hashmap->bucket_count, new_load_factor);
	???
	/* TODO_GS: Continue to handle the load factor and re-hashing */

	return result;
}

hashmap_generic_bool_te hashmap_generic_delete(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size
)
{
	if (p_hashmap == NULL || p_key == NULL)
		return HASHMAP_GENERIC_FALSE;

	const uint32_t key_hash = fnv32_hash((const char *) p_key, key_size);
	const uint32_t hashed_key_bucket_index = key_hash % p_hashmap->bucket_count;
	hashmap_generic_bucket_ts * p_bucket = p_hashmap->pp_buckets[hashed_key_bucket_index];

	/* Nothing to do for empty bucket */
	if (p_bucket == NULL)
		return HASHMAP_GENERIC_FALSE;

	/* Look for a collision */
	hashmap_generic_bucket_ts * p_search_bucket_parent = NULL;
	hashmap_generic_bucket_ts * p_search_bucket = p_bucket;
	for(; p_search_bucket != NULL; p_search_bucket = p_search_bucket->p_next)
	{
		if (!nodes_match(p_search_bucket, p_key, key_size))
		{
			p_search_bucket_parent = p_search_bucket;
			continue;
		}

		/* Found chain bucket node to delete */
		hashmap_generic_bucket_ts * p_search_bucket_child = p_search_bucket->p_next;

		/* Deallocate the node to delete */
		p_hashmap->entry_count--;
		destroy_bucket_node(p_search_bucket);

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

		return HASHMAP_GENERIC_TRUE;
	}

	/* No collision occured */
	return HASHMAP_GENERIC_FALSE;
}

const void * hashmap_generic_get(
	hashmap_generic_instance_ts * p_hashmap,
	const void * const p_key,
	size_t key_size
)
{
	if (p_hashmap == NULL || p_key == NULL)
		return NULL;

	/* Return a pointer to the data when the key exists, NULL otherwise */
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
		if (!nodes_match(p_search_bucket, p_key, key_size))
			continue;

		/* Found chain bucket node to return value from */
		return p_search_bucket->p_value;
	}

	/* No corresponding entry found */
	return NULL;
}

void hashmap_generic_iterator(
	hashmap_generic_instance_ts * p_hashmap,
	hashmap_generic_iterator_ts * p_iterator
)
{
	if (p_hashmap == NULL || p_hashmap->entry_count == 0)
	{
		/* Cannot iterate through the specified hashmap */
		p_iterator->bucket_index = -1;
		return;
	}

	p_iterator->bucket_index = 0;
	p_iterator->p_current_bucket_node = p_hashmap->pp_buckets[0];
	p_iterator->p_hashmap = p_hashmap;
}

hashmap_generic_bool_te hashmap_generic_iterator_has_next(
	hashmap_generic_iterator_ts * p_iterator
)
{
	/* Do not iterate when iterator un-usable */
	if (p_iterator->bucket_index == -1)
		return HASHMAP_GENERIC_FALSE;

	/* Success if the currently selected bucket chain node is usable */
	if (p_iterator->p_current_bucket_node != NULL)
		return HASHMAP_GENERIC_TRUE;

	/* Currently selected node is un-usable - Scan buckets for another, usable entry */
	while (p_iterator->p_current_bucket_node == NULL && p_iterator->bucket_index < p_iterator->p_hashmap->bucket_count - 1)
		p_iterator->p_current_bucket_node = p_iterator->p_hashmap->pp_buckets[++p_iterator->bucket_index];

	/* Usable bucket found? */
	return p_iterator->p_current_bucket_node ? HASHMAP_GENERIC_TRUE : HASHMAP_GENERIC_FALSE;
}

hashmap_generic_iterator_entry hashmap_generic_iterator_get_next(
	hashmap_generic_iterator_ts * p_iterator
)
{
	/* Return the currently iterated, usable hashmap entry and move to next chain entry */
	hashmap_generic_iterator_entry entry = {
		p_iterator->p_current_bucket_node->key.p_data,
		p_iterator->p_current_bucket_node->p_value,
		p_iterator->bucket_index
	};

	/* Move to the next chain node in line */
	p_iterator->p_current_bucket_node = p_iterator->p_current_bucket_node->p_next;

	/* Return currently iterated entry */
	return entry;
}

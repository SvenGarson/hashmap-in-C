#include <stdio.h>
#include <string.h>
#include "..\include\hashmap_generic.h"

int main(void)
{
	/* Test the wrapper interface */
	hashmap_generic_instance_ts * p_hashmap = hashmap_generic_create();

	if (p_hashmap == NULL)
	{
		printf("\n[Main] Hashmap un-usable");
		return -1;
	}

	/* Use interface */
	const char key1[] = "Iron Man";
	const char value1[] = "Quote: 'I am Iron Man'";
	hashmap_generic_set(p_hashmap, key1, sizeof(key1), value1, sizeof(value1));

	const char key2[] = "Thor";
	const char value2[] = "Quote: 'God of Thunder'";
	hashmap_generic_set(p_hashmap, key2, sizeof(key2), value2, sizeof(value2));

	const char key3[] = "Waterdude";
	const char value3[] = "Quote: 'Swisch Swiiisch'";
	hashmap_generic_set(p_hashmap, key3, sizeof(key3), value3, sizeof(value3));

	hashmap_generic_visualize(p_hashmap, "Before deletion");
	
	/* Iteration */
	hashmap_generic_iterator_ts iter;
	hashmap_generic_iterator(p_hashmap, &iter);
	while (hashmap_generic_iterator_has_next(&iter))
	{
		hashmap_generic_iterator_entry entry = hashmap_generic_iterator_get_next(&iter);
		printf("\nEntry bucket: %-3d Key: %-30s Value: %-30s", entry.bucket_index, (char *) entry.p_key, (char *) entry.p_value);
	}

	hashmap_generic_destroy(&p_hashmap);
	hashmap_generic_visualize(p_hashmap, "After deletion");

	/* Return to OS successfully */
	return 0;
}
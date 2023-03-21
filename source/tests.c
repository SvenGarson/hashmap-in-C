#include <stdio.h>
#include <string.h>
#include "..\include\hashmap_generic.h"

int main(void)
{
	/* TODO-GS: Run hashmap tests */
	hashmap_generic_instance_ts * p_hashmap = hashmap_generic_create();

	/* Add entries */
	hashmap_generic_visualize(p_hashmap, "Before insertions");
	/*hashmap_generic_set(const void * const p_data, size_t p_data_size, const void * const p_value, size_t p_value_size)*/
	const char key[] = "The Martian I";
	const int value = 1337;
	hashmap_generic_set(p_hashmap, key, sizeof(key), &value, sizeof(value));
	hashmap_generic_visualize(p_hashmap, "Insertion 1");

	const char key2[] = "The Wolf and The Sheep 3";
	const int value2 = 4500;
	hashmap_generic_set(p_hashmap, key2, sizeof(key2), &value2, sizeof(value2));
	hashmap_generic_visualize(p_hashmap, "Insertion 2");

	const char key3[] = "The Martian I";
	const int value3 = 600006;
	hashmap_generic_set(p_hashmap, key3, sizeof(key3), &value3, sizeof(value3));
	hashmap_generic_visualize(p_hashmap, "Insertion 3");

	const char key4[] = "Flipper";
	const int value4 = 111;
	hashmap_generic_set(p_hashmap, key4, sizeof(key4), &value4, sizeof(value4));
	hashmap_generic_visualize(p_hashmap, "Insertion 4");

	/* Retrieving entries */
	const char * get_keys[] = {
		key,
		key2,
		"Oh captain",
		key3,
		key4,
		"Missing Number"
	};

	printf("\n\nRetrieving data from hashmap:");
	for (int get_index = 0; get_index < (int)(sizeof(get_keys) / sizeof(char *)); get_index++)
	{
		const char * const p_search_key = get_keys[get_index];
		const void * p_get_result = hashmap_generic_get(p_hashmap, p_search_key, strlen(p_search_key) + 1);
		printf(
			"\n\tKey: %-30s Value: %-16p Translation: %d",
			p_search_key,
			p_get_result,
			p_get_result ? *((int *)p_get_result) : -1
		);
	}

	/* Deleting entries */
	hashmap_generic_delete(p_hashmap, key2, sizeof(key2));
	hashmap_generic_visualize(p_hashmap, "Deletion 1");	

	hashmap_generic_delete(p_hashmap, key3, sizeof(key3));
	hashmap_generic_visualize(p_hashmap, "Deletion 2");	

	hashmap_generic_delete(p_hashmap, key4, sizeof(key4));
	hashmap_generic_visualize(p_hashmap, "Deletion all");	

	hashmap_generic_delete(p_hashmap, key, sizeof(key));
	hashmap_generic_visualize(p_hashmap, "Deletion in empty bucket");	

	/* Destroy */
	hashmap_generic_destroy(&p_hashmap);

	/* Return to OS successfully */
	return 0;
}
#include <stdio.h>
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
	hashmap_generic_visualize(p_hashmap, "Insertion I");

	const char key2[] = "The Wolf and The Sheep 3";
	const int value2 = 4500;
	hashmap_generic_set(p_hashmap, key2, sizeof(key2), &value2, sizeof(value2));
	hashmap_generic_visualize(p_hashmap, "Insertion II");

	const char key3[] = "The Martian I";
	const int value3 = 55555;
	hashmap_generic_set(p_hashmap, key3, sizeof(key3), &value3, sizeof(value3));
	hashmap_generic_visualize(p_hashmap, "Insertion III");

	/* Destroy */
	hashmap_generic_destroy(&p_hashmap);

	/* Return to OS successfully */
	return 0;
}
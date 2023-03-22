#include <stdio.h>
#include <string.h>
#include "..\include\hashmap_generic.h"

/* Datatypes - Custom struct to use as value */
typedef struct {
	int account_number;
	int frozen;
} account_info_ts;

/* Functions - Helpers for testing things */
void set_string_int(
	hashmap_generic_instance_ts * p_hashmap,
	const char * p_key,
	int value
)
{
	hashmap_generic_set(p_hashmap, p_key, strlen(p_key) + 1, &value, sizeof(value));
}

void print_string_int(
	hashmap_generic_instance_ts * p_hashmap,
	const char * p_key
)
{
	const void * p_value = hashmap_generic_get(p_hashmap, p_key, strlen(p_key) + 1);
	printf("\n[%-15s]: %d", p_key, p_value ? *((int *) p_value) : -1);
}

void delete_string_int(
	hashmap_generic_instance_ts * p_hashmap,
	const char * p_key
)
{
	hashmap_generic_bool_te success = hashmap_generic_delete(p_hashmap, p_key, strlen(p_key) + 1);
	printf("\nDeleted value for key [%-15s]: %s", p_key, success ? "True" : "False");
}

int main(void)
{
	/* Create hashmap instances */
	hashmap_generic_instance_ts * p_hashmap_string_int = hashmap_generic_create(4);
	hashmap_generic_instance_ts * p_hashmap_int_struct = hashmap_generic_create(4);

	/* Insert names and ages */
	set_string_int(p_hashmap_string_int, "Nora", 16);
	set_string_int(p_hashmap_string_int, "Ronald", 49);
	set_string_int(p_hashmap_string_int, "Jesse", 35);
	set_string_int(p_hashmap_string_int, "Nick", 22);
	hashmap_generic_visualize(p_hashmap_string_int, "Setting - Names -> Ages");

	/* Getting names and ages */
	print_string_int(p_hashmap_string_int, "Slick");
	print_string_int(p_hashmap_string_int, "Nora");
	print_string_int(p_hashmap_string_int, "Ronald");
	print_string_int(p_hashmap_string_int, "Jesse");
	print_string_int(p_hashmap_string_int, "Nick");
	print_string_int(p_hashmap_string_int, "John");

	/* Iterating names and ages */
	hashmap_generic_iterator_ts names_ages_iterator;
	hashmap_generic_iterator(p_hashmap_string_int, &names_ages_iterator);
	printf("\n\nIterating current names and ages:");
	int name_age_index = 0;
	while (hashmap_generic_iterator_has_next(&names_ages_iterator))
	{
		hashmap_generic_iterator_entry name_age_entry = hashmap_generic_iterator_get_next(&names_ages_iterator);
		printf("\n[%-3d] %-15s: %-d", name_age_index++, (const char *)name_age_entry.p_key, *((int *)name_age_entry.p_value));
	}

	/* Deleting names and ages */
	putchar('\n');
	delete_string_int(p_hashmap_string_int, "Slick");
	delete_string_int(p_hashmap_string_int, "Jesse");
	delete_string_int(p_hashmap_string_int, "Ronald");
	delete_string_int(p_hashmap_string_int, "Ronald");
	delete_string_int(p_hashmap_string_int, "Nick");
	delete_string_int(p_hashmap_string_int, "Nora");
	delete_string_int(p_hashmap_string_int, "Nora");
	delete_string_int(p_hashmap_string_int, "Nora");
	delete_string_int(p_hashmap_string_int, "John");
	hashmap_generic_visualize(p_hashmap_string_int, "Deleting - Names -> Ages");

	/* Insert int and custom struct */
	set_string_int(p_hashmap_int_struct, "Harry", 16); ???
	hashmap_generic_visualize(p_hashmap_string_int, "Setting - Names -> Ages");

	/* Destroy the hashmap instances */
	hashmap_generic_destroy(&p_hashmap_string_int);
	hashmap_generic_destroy(&p_hashmap_int_struct);

	/* Return to OS successfully */
	return 0;
}
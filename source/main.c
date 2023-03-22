#include <stdio.h>
#include <string.h>
#include "..\include\hashmap_generic.h"

/* Datatypes - Custom struct to use as value */
typedef struct {
	long long int account_number;
	hashmap_generic_bool_te frozen;
} account_info_ts;

/* Functions - Helpers for testing things */
void set_string_int(
	hashmap_generic_instance_ts * p_hashmap,
	const char * p_key,
	long long int value
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

void set_int_struct(
	hashmap_generic_instance_ts * p_hashmap,
	int key,
	long long int account_number,
	hashmap_generic_bool_te frozen
)
{
	account_info_ts account_info = { account_number, frozen };
	hashmap_generic_set(p_hashmap, &key, sizeof(key), &account_info, sizeof(account_info));
}

void print_int_struct(
	hashmap_generic_instance_ts * p_hashmap,
	int key
)
{
	const void * p_value = hashmap_generic_get(p_hashmap, &key, sizeof(key));
	if (p_value == NULL)
	{
		printf("\n[%-5d] N/A", key);
	}
	else
	{
		const account_info_ts * const p_account_info = (account_info_ts *) p_value;
		printf("\n[%-5d] Account number: %-16lld Frozen: %s", key, p_account_info->account_number, p_account_info->frozen ? "True" : "False");
	}
}

void delete_int_struct(
	hashmap_generic_instance_ts * p_hashmap,
	int key
)
{
	hashmap_generic_bool_te success = hashmap_generic_delete(p_hashmap, &key, sizeof(key));
	printf("\nDeleted value for key [%-8d]: %s", key, success ? "True" : "False");
}

int main(void)
{
	/* Create hashmap instances */
	hashmap_generic_instance_ts * p_hashmap_string_int = hashmap_generic_create(4);
	hashmap_generic_instance_ts * p_hashmap_int_struct = hashmap_generic_create(4);

	/* Insert names and ages */
	printf("\n\nShowing operations on Name -> Age");
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
	printf("\n\nShowing operations on Int -> Struct");
	set_int_struct(p_hashmap_int_struct, 101, 1000000000000001, HASHMAP_GENERIC_TRUE);
	set_int_struct(p_hashmap_int_struct, 202, 2000000000000002, HASHMAP_GENERIC_FALSE);
	set_int_struct(p_hashmap_int_struct, 303, 3000000000000003, HASHMAP_GENERIC_FALSE);
	set_int_struct(p_hashmap_int_struct, 404, 4000000000000004, HASHMAP_GENERIC_TRUE);
	hashmap_generic_visualize(p_hashmap_int_struct, "Setting - Int -> Struct");

	/* Getting names and ages */
	print_int_struct(p_hashmap_int_struct, 55);
	print_int_struct(p_hashmap_int_struct, 101);
	print_int_struct(p_hashmap_int_struct, 188);
	print_int_struct(p_hashmap_int_struct, 202);
	print_int_struct(p_hashmap_int_struct, 275);
	print_int_struct(p_hashmap_int_struct, 303);
	print_int_struct(p_hashmap_int_struct, 404);
	print_int_struct(p_hashmap_int_struct, 405);
	print_int_struct(p_hashmap_int_struct, 158748);

	/* Iterating ints and structs */
	hashmap_generic_iterator_ts int_struct_iterator;
	hashmap_generic_iterator(p_hashmap_int_struct, &int_struct_iterator);
	printf("\n\nIterating current int and struct:");
	int int_struct_index = 0;
	while (hashmap_generic_iterator_has_next(&int_struct_iterator))
	{
		hashmap_generic_iterator_entry int_struct_entry = hashmap_generic_iterator_get_next(&int_struct_iterator);
		const account_info_ts * const p_account_info = (account_info_ts *) int_struct_entry.p_value;
		printf("\n[%-3d] Account: %-lld Frozen: %s", int_struct_index++, p_account_info->account_number, p_account_info->frozen ? "True" : "False");
	}

	/* Deleting names and ages */
	putchar('\n');
	delete_int_struct(p_hashmap_int_struct, 12);
	delete_int_struct(p_hashmap_int_struct, 99);
	delete_int_struct(p_hashmap_int_struct, 101);
	delete_int_struct(p_hashmap_int_struct, 185);
	delete_int_struct(p_hashmap_int_struct, 202);
	delete_int_struct(p_hashmap_int_struct, 299);
	delete_int_struct(p_hashmap_int_struct, 299);
	delete_int_struct(p_hashmap_int_struct, 299);
	delete_int_struct(p_hashmap_int_struct, 303);
	delete_int_struct(p_hashmap_int_struct, 303);
	delete_int_struct(p_hashmap_int_struct, 303);
	delete_int_struct(p_hashmap_int_struct, 303);
	delete_int_struct(p_hashmap_int_struct, 304);
	delete_int_struct(p_hashmap_int_struct, 404);
	delete_int_struct(p_hashmap_int_struct, 404);
	delete_int_struct(p_hashmap_int_struct, 404);
	hashmap_generic_visualize(p_hashmap_int_struct, "Deleting - Int -> Struct");

	/* Destroy the hashmap instances */
	hashmap_generic_destroy(&p_hashmap_string_int);
	hashmap_generic_destroy(&p_hashmap_int_struct);

	/* Return to OS successfully */
	return 0;
}
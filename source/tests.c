#include <stdio.h>
#include "..\include\hashmap_generic.h"

int main(void)
{
	/* TODO-GS: Run hashmap tests */
	hashmap_generic_instance_ts * p_hashmap = hashmap_generic_create();

	/* Check instance */
	if (p_hashmap == NULL)
		printf("\n---> Hashmap creation failure");
	else
		printf("\n---> Hashmap creation success");

	/* Visualize */
	hashmap_generic_visualize(p_hashmap, "Some thing");

	/* Destroy */
	hashmap_generic_destroy(&p_hashmap);

	/* Check destroyal */
	if (p_hashmap == NULL)
		printf("\n---> Hashmap destroyal success");
	else
		printf("\n---> Hashmap destroyal failure");

	/* Return to OS successfully */
	return 0;
}
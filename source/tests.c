#include <stdio.h>
#include <string.h>
#include "..\include\hashmap_generic.h"

int main(void)
{
	/* Test the wrapper interface */
	hashmap_generic_instance_ts * p_hashmap = hashmap_generic_create(8);

	if (p_hashmap == NULL)
	{
		printf("\n[Main] Hashmap un-usable");
		return -1;
	}

	hashmap_generic_visualize(p_hashmap, "Initial state");

	/* Use interface */
	char key_buffer[256];
	char value_buffer[256];
	for (int i = 1; i <= 12; i++)
	{
		snprintf(key_buffer, 256, "%d", i);
		snprintf(value_buffer, 256, "%d", i * 10 + 1);

		const int cap = p_hashmap->bucket_count;
		hashmap_generic_set(p_hashmap, key_buffer, strlen(key_buffer) + 1, value_buffer, strlen(value_buffer) + 1);
		printf("\nInserted entry '%d'", i);

		if (p_hashmap->bucket_count != cap)
		{
			printf("\n\n======================> Changed cap from %d to %d", cap, p_hashmap->bucket_count);
			printf("\nVisualizing:");
			hashmap_generic_visualize(p_hashmap, "---");
		}
	}

	/*hashmap_generic_visualize(p_hashmap, "After adding stuff");*/

	printf("\n\nGetting values:");
	for (int i = 1; i <= 12; i++)
	{
		snprintf(key_buffer, 256, "%d", i);
		const  void * p_value = hashmap_generic_get(p_hashmap, key_buffer, strlen(key_buffer) + 1);
		printf("\nKey: %-4d Value: %-s", i, (char *) p_value);
	}

	hashmap_generic_destroy(&p_hashmap);

	/* Return to OS successfully */
	return 0;
}
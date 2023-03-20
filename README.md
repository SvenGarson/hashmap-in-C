# hashmap-in-C
 An implementation of a hashmap in C

## Requirements
- Key and value requirements
- Ability to store anything for a given hashmap (this means custom key compare function on creation)
- Write a wrapper to use the thing
- Growing of the associative array when certain load factor reached in terms of bucket count
- Q & A
  + Can a bucket/node key be NULL?
    Yes, a key of null represents an un-used bucket node?
  + Can a bucket/node key be NULL?
    Just enable using code to specify NULL values for now and see if it's useful

## Finalizing
- Test suite
- Documentation
  + Analyzing edge cases and limitations, like the largest number of buckets etc.

## Implementation details to enforce
- Bucket node with NULL key is un-used

## Interface
- Insertion
  + Set value (void *) for key (char *)
- Retrieval
  + Get value (void *) for key (char *)
- Deletion
  + Delete value (void *) for key (char *)
- Utility
  + Contains value (void *) for key (char *)
- Debug printing the contents
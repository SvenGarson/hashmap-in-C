# hashmap-in-C
 An implementation of a hashmap in C

## Requirements
- Key and value requirements
- Ability to store anything for a given hashmap (this means custom key compare function on creation)
- Write a wrapper to use the thing
- Growing of the associative array when certain load factor reached in terms of bucket count
- add return values to functions that report the result of the operation when useful
- Q & A
  + Can a bucket/node key be NULL?
    Yes, a key of null represents an un-used bucket node?
  + Can a bucket/node key be NULL?
    Just enable using code to specify NULL values for now and see if it's useful

## Finalizing
- Test suite
  + Hash function for different and the same things; argument edge cases
  + Check collision in a dictionary
  + Different length of overlapping keys that must not collide
    - abc & ab
    in both direction where the new one is longer; the same size or shorter than the node data
  + cannot insert duplicates for a particular key data type
  + adding kv pairs
    - initializes buckets
    - finds collisions and overwrites that value
    - appends when there is no collision
  + deleting kv pairs
    - marks the first bucket as un-used when all chain entries deleted
    - works only for collisions, removes the node accordingly and connect parent and child 
      nodes (with edge-cases)
    - deallocates previously allocated memory
  + guards
    - against invalid hashmap instance attributes
    - zero sizes cause operations to not work (difference between zero-length key and 
      zero-length value?)

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
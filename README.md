# hashmap-in-C
 An implementation of a hashmap in C

## Requirements
- Add load factor based increase of the bucket count
  - grow when load factor reachhed
- Write a wrapper to use the thing
- Growing of the associative array when certain load factor reached in terms of bucket count
- add return values to functions that report the result of the operation when useful
- rename buckets to nodes, since that is a different thing
- Q & A
  + Can a bucket/node key be NULL?
    Yes, a key of null represents an un-used bucket node?
  + Can a bucket/node key be NULL?
    Just enable using code to specify NULL values for now and see if it's useful

## Iterator in C

  iter;
  hmap_iterator(&iter);
  while (hmap_iterator_next(&iter))
  {
    kv = hmap_iterator_enty()
    kv.key
    kv.value
  }

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
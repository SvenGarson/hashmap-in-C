# hashmap-in-C
An implementation of a hashmap in C.

## About this projects
The goal of this little project was to understand the basics of the implementation of a hashmap. This logic is not thoroughly tested and should not actually be used for anything.

The idea of the interface is that the user can create hashmap instances and set/delete/get values based on a generic key and value type using void pointers while providing the key and value data sizes so the user does not need to worry about managing memory, but the implementation will take care of the memory allocation and deallocation by creating copies of the provided keys and values based on the specified data sizes in bytes.

## Disclaimer
Since this implementation was not thoroughly tested in terms of memory allocation and deallocation, there may be memory leaks for edge-cases and other bugs may come up.
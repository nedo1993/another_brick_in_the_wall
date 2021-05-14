
#ifndef NEW_H
#define NEW_H
typedef struct cach_entry_s cach_entry_t;
typedef struct cach_fd_s cach_fd_t;
typedef struct cach_hash_s cach_hash_t;

#include <stdio.h>
#include <pthread.h>
#include "files_s.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif


struct cach_entry_s {
    int am_dead;
    //importance of item
    unsigned long int *ref;
    int group;
    char* file_name;
    pthread_mutex_t mutex;
    //>=1 then the file is new 
    struct cach_entry_s* next;
    struct cach_entry_s* prev;
};

struct cach_fd_s {
    int group;
    //number of empty entries
    int num_dead;
    pthread_mutex_t mutex_for_cleanup;
    int max_and_you_out;
    int min_and_you_in;
    int nfiles_row;
    cach_entry_t *head;
    cach_entry_t* tail;
};

struct cach_hash_s {
    //number of levels in which the lowest has the most importance i.e has been referenced the most.
    //This constant is getting multiplied by another cost for creating minimum and maximum boundaries per group.
    size_t NUM_GROUP;

    size_t CACHE_RANGE;
    unsigned long int incr_entr;
    int nentries;
    int nfiles;
    int MAX_LAST_LEVEL;
    int START_INI_CACHE;
    cach_fd_t **buckets;
    unsigned int (*hash_function)(unsigned int, int);
};

struct Cache_settings{
    int NUM_GROUP;
    int CACHE_RANGE;
    int START_INI_CACHE;
    float EXTRA_CACHE_SPACE;
};
//SETTING cache as global variable.
extern cach_hash_t *MY_CACHE;

cach_hash_t *
cach_hash_create(struct Cache_settings, unsigned int (*hash_function)(unsigned int, int));

unsigned long
get_seed(pthread_t);

cach_entry_t
* cach_hash_insert_bind(cach_hash_t *, icl_entry_t *);

int
cach_hash_destroy(cach_hash_t *, void (*)(void*)),
    cach_hash_dump(FILE *, cach_hash_t *);

int cach_hash_delete( cach_hash_t *, unsigned int, char *, void (*free_key)(void*) );
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif /* icl_hash_h */
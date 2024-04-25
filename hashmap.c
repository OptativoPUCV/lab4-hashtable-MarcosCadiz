#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if(map==NULL || key==NULL || value==NULL) return;
    long pos = hash(key,map->capacity);
    while(map->buckets[pos]!=NULL && map->buckets[pos]->key!=NULL){
        pos = (pos+1)%map->capacity;
    }
    map->buckets[pos] = createPair(key,value);
    map->size++;

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap* createMap(long capacity) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if(map==NULL) return NULL;
  
    map->buckets = (Pair**)calloc(capacity,sizeof(Pair*));
    if(map->buckets==NULL){
      free(map);
      return NULL;
    }
  
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;
    return map;
  
}

void eraseMap(HashMap * map,  char * key) {    
    if(map==NULL || key==NULL) return;
    long pos = hash(key,map->capacity);
    while(map->buckets[pos]!=NULL){
       if(is_equal(map->buckets[pos]->key,key)){
         map->buckets[pos]->key=NULL;
         map->size--;
         return;
       }
       pos = (pos+1)%map->capacity;  
    }
}

Pair * searchMap(HashMap * map,  char * key) {
  if(map == NULL || key == NULL){
    return NULL;
  }
  int index = hash(key, map->capacity);
  while(map->buckets[index] != NULL && map->buckets[index]->key != NULL ){
    if(is_equal(map->buckets[index]->key, key)){
      map->current = index;
      return map->buckets[index];
    }
    index = (index + 1) % map->capacity;
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  if(map == NULL){
    return NULL;
  }
  int index = 0;
  while(map->buckets[index] == NULL || map->buckets[index]->key == NULL){
    index++;
    
  }
  map->current = index;
  return map->buckets[index];
}

Pair * nextMap(HashMap * map) {
  if(map == NULL || map->size == 0){
    return NULL;
  }
  int index = map->current + 1;
  while(map->buckets[index] == NULL || map->buckets[index]->key == NULL){
    index++;
    if(index == map->capacity){
      index = 0;
    }
    
  }
  map->current = index;
  return map->buckets[index];
  
  
}

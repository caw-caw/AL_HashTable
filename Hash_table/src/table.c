/**
 * @file       table.c
 * @author     Ondřej Ševčík
 * @date       6/2019
 * @brief      Implementation of functions for HashTable.
 * **************************************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

/* Private includes -------------------------------------------------------- */
#include "table.h"
#include <stdio.h>
#include <string.h>
#include "hash-private.h"
#include "mymalloc.h"

// public implementations
bool HashTable_Init(HashTable *table, size_t size, bool takeOwnership) {
  (void)table;
  (void)size;
  (void)takeOwnership;
  if(table==NULL || size<=0){
    return false;
  }
  table->buckets=myMalloc(sizeof(HashTableNode*)*size);
  if(table->buckets==NULL){
    return false;
  }
  for (int i=0;i<(int)size; i++)
    table->buckets[i]=NULL;
  table->count=0;
  table->size=size;
  table->take_ownership=takeOwnership;

  return true;
}

void HashTable_Destruct(HashTable *table) {
    if(table != NULL){
        myFree(table->buckets);
        table->buckets = NULL;
        table->count = 0;
        table->size = 0;
        table = NULL;
    }

}

bool HashTable_Insert(HashTable *table, Data_t *key, Data_t *value) {
    if(table == NULL || key == NULL || value == NULL){
        return false;}

    size_t i = hash(table, key);
    HashTableNode* next = table->buckets[i];
    while(next != NULL){
        if(Data_Cmp(next->key, key) == 0) {
            return false;
        }
        next = next->next;
    }

    HashTableNode* node = myMalloc(sizeof (HashTableNode));
    if(node == NULL){
        return false;
    }
    node->key = key;
    node->value = value;
    node->next = table->buckets[i];
    table->buckets[i] = node;
    table->count++;
    return true;
}

bool HashTable_Replace(HashTable *table, Data_t *key, Data_t *value) {
    if(table == NULL || key == NULL || value == NULL){
        return false;}

    size_t i = hash(table, key);
    HashTableNode* next = table->buckets[i];
    while(next != NULL){
        if(Data_Cmp(next->key, key) == 0) {
            if(table->take_ownership == true){
                myFree(next->value);
            }
            next->value = value;
            return true;
        }
        next = next->next;
    }

    return true;
}

bool HashTable_Delete(HashTable *table, Data_t *key) {
    if(table == NULL || key == NULL){
        return false;}

    size_t i = hash(table, key);
    HashTableNode* next = table->buckets[i];
    HashTableNode* prev = table->buckets[i];
    bool cycle = true;
    while(cycle == true){
        if(Data_Cmp(next->key, key) == 0) {
            if(table->take_ownership == true){
                Data_Destruct(next->key);
                Data_Destruct(next->value);
                table->count--;
            }
            if(next!=prev){
                prev->next=next->next;
            }else{
                table->buckets[i]=next->next;
            }
            cycle = false;
        }
        prev=next;
        next = next->next;
    }
    return true;
}

Data_t *HashTable_Find(HashTable *table, Data_t *key) {
    if(table == NULL || key == NULL){
        return NULL;
    }

    size_t i = hash(table, key);
    HashTableNode* next = table->buckets[i];
    while(next != NULL){
        if(Data_Cmp(next->key, key) == 0) {
            return next->value;
        }
        next = next->next;
    }

    return NULL;
}

size_t HashTable_Get_Count(HashTable *table) {
    if(table !=NULL){return table->count;}
    return 0;
}

void HashTable_Clear(HashTable *table) {
    if(table != NULL){

        for(unsigned int i = 0; i < table->size; i++){
            HashTableNode* bucket=table->buckets[i];
            while(bucket != NULL){
                HashTableNode* tmp = bucket->next;
                if(table->take_ownership == true){
                Data_Destruct(bucket->key);
                Data_Destruct(bucket->value);}
                myFree(bucket);
                bucket = tmp;

            }
            table->buckets[i]=NULL;
        }

    }
}

void HashTable_Process(HashTable *table, TableNodeProc proc) {
    if(table != NULL && proc != NULL){

        for(unsigned i = 0; i < table->size; i++){

            HashTableNode* bucket = table->buckets[i];
            while(bucket != NULL){
                proc(bucket->key,bucket->value);
                bucket = bucket->next;
            }
        }

    }
}

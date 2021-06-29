#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef uint64_t Hodnota;
typedef char Klic [255];

typedef struct HashTableNode {
    Klic key;
    Hodnota value;
    struct HashTableNode * next;
}HashTableNode;


typedef struct HashTable {
    HashTableNode ** buckets;
    unsigned size;
    unsigned itemsCount;
}HashTable;

unsigned hash(Klic key, HashTable * table){
    if(key == NULL || table == NULL){
        return 0;
    }

    return strlen(key)%table->size;
}

void HashTable_Init(HashTable * table, unsigned initialSize){
    if(table == NULL || initialSize == 0){return;}

    table->buckets = calloc(sizeof (HashTableNode*),initialSize);
    table->size = initialSize;
    table->itemsCount = 0;

}


bool HashTable_Insert(HashTable * table, Klic * key, Hodnota value){
    if(table == NULL || key == NULL){
        return false;
    }

    HashTableNode * insert = malloc(sizeof (HashTableNode));
    strcpy(insert->key, *key);
    insert->value = value;
    insert->next = NULL;

    unsigned index = hash(* key, table);

    if(table->buckets[index] == NULL){
        table->buckets[index] = insert;
        table->itemsCount++;
        return true;
    }else{
        HashTableNode * ptrNode = table->buckets[index];
        while(ptrNode->next != NULL){
            ptrNode = ptrNode->next;
        }
        ptrNode->next = insert;
        table->itemsCount++;
        return true;
    }

    return false;
}

/*
HashTable * HashTable_Intersect(HashTable * table1, HashTable * table2){
    if(table1 == NULL || table2 == NULL){
        return NULL;
    }

    unsigned size = 0;
    if(table1->size > table2->size){size = table2->size;}
    else{size = table1->size;}

    HashTable * tableReturn = malloc(sizeof (HashTable));
    HashTable_Init(tableReturn,size);

    HashTableNode * ptrNode = NULL;
    HashTableNode * ptrNode2 = NULL;
    for(unsigned i = 0; i < size; i++){
        ptrNode = table1->buckets[i];
        ptrNode = table2->buckets[i];
        while(ptrNode != NULL && ptrNode2 != NULL){
            if(ptrNode->key == ptrNode2->key  &&  ptrNode->value == ptrNode2->value){
                HashTable_Insert(tableReturn, &ptrNode->key, ptrNode->value);
            }
        }
    }

    return tableReturn;
}
*/


HashTable * HashTable_Intersect(HashTable * table1, HashTable * table2){
    if(table1 == NULL || table2 == NULL){
        return NULL;
    }

    unsigned size = 0;
    if(table1->size < table2->size){size = table2->size;}
    else{size = table1->size;}

    HashTable * tableReturn = malloc(sizeof (HashTable));
    HashTable_Init(tableReturn,size);

    HashTableNode * ptrNode = NULL;
    HashTableNode * ptrNode2 = NULL;
    for(unsigned i = 0; i < table1->size; i++){
        ptrNode = table1->buckets[i];

        while(ptrNode != NULL){
            for(unsigned j = 0; j < table2->size; j++){
                ptrNode2 = table2->buckets[j];

                while(ptrNode2!=NULL){
                    if(strcmp(ptrNode->key, ptrNode2->key) == 0 && ptrNode->value == ptrNode2->value){
                            HashTable_Insert(tableReturn, &ptrNode->key, ptrNode->value);
                    }
                ptrNode2 = ptrNode2->next;
            }
        }
        ptrNode = ptrNode->next;
    }

    }

    return tableReturn;
}



void HashTable_Print(HashTable * table){
    if(table != NULL){
        printf("\n\nNumber of buckets: %u", table->size);

        for(unsigned i = 0; i < table->size; i++){
            printf("\nBucket %u |", i);
            HashTableNode * ptrNode = table->buckets[i];
            while(ptrNode != NULL){
                printf("| node ID %llu ", ptrNode->value);
                ptrNode = ptrNode->next;
            }
        }
    }
}









int main()
{

    HashTable * table1 = malloc(sizeof (HashTable));
    HashTable_Init(table1,5);

    Klic key1 = "Cache";
    Hodnota value1 = 0;

    Klic key2 = "Data";
    Hodnota value2 = 1;

    Klic key3 = "SmallCache";
    Hodnota value3 = 2;

    Klic key4 = "NotData";
    Hodnota value4 = 3;

    HashTable_Insert(table1,&key1,value1);
    HashTable_Insert(table1,&key2,value2);
    HashTable_Insert(table1,&key3,value3);
    HashTable_Insert(table1,&key4,value4);

    HashTable_Print(table1);




    HashTable * table2 = malloc(sizeof (HashTable));
    HashTable_Init(table2,5);

    Klic key5 = "Key";
    Hodnota value5 = 0;

    Klic key6 = "NotData";
    Hodnota value6 = 3;

    Klic key7 = "Data";
    Hodnota value7 = 255;

    Klic key8 = "SmallCache";
    Hodnota value8 = 0x02;

    HashTable_Insert(table2,&key5,value5);
    HashTable_Insert(table2,&key6,value6);
    HashTable_Insert(table2,&key7,value7);
    HashTable_Insert(table2,&key8,value8);

    HashTable_Print(table2);




    HashTable * tableIntersect = HashTable_Intersect(table1, table2);
    HashTable_Print(tableIntersect);

    return 0;
}

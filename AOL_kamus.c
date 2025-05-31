#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_SIZE 300
#define EMPTY "NULL"

typedef struct hashNode{
    char key[20];
    char kata[20];
}hashNode;

hashNode Kamus[DICT_SIZE];

//struct buat sorting
typedef struct {
    char key[20];
    char kata[20];
} entry;

unsigned long hash(const char *key){
    unsigned long hash = 5381;

    int c;
    while((c = *key++)){
        hash = ((hash << 5) * hash) + c;
    }

    return hash;
}

int hashToIndex(const char *key){
    return hash(key) % DICT_SIZE;
}

//insert pake linear probing
void insertKamus(const char *key, const char *value){
    int index = hashToIndex(key);
    
    while(Kamus[index].key[0] != '\0'){
        index = (index + 1) % DICT_SIZE;
    }

    strcpy(Kamus[index].kata, value);
    strcpy(Kamus[index].key, key);
}

const char *searchKamus(const char *key){
    int index = hashToIndex(key);
    int startIndex = index;
    
    while(strcmp(Kamus[index].key, key) != 0){
        if(Kamus[index].key[0] == '\0'){
            return NULL; 
        }
        index = (index + 1) % DICT_SIZE;
        if(index == startIndex){
            return NULL;
        }
    }

    return Kamus[index].kata;
}

int compareEntries(const void *a, const void *b){
    entry *entryA = (entry *)a;
    entry *entryB = (entry *)b;
    return strcmp(entryA->key, entryB->key);
}

void printSortedKamus(){
    entry entries[DICT_SIZE];
    int count = 0;

    for(int i =0;i<DICT_SIZE;i++){
        if (Kamus[i].key[0]!='\0'){
            strcpy(entries[count].key, Kamus[i].key);
            strcpy(entries[count].kata, Kamus[i].kata);
            count++;
        }
    }
    qsort(entries, count, sizeof(entry), compareentries);
    printf("\nIsi Kamus (sorted alphabetically):\n");
    for(int i=0; i<count; i++){
        printf("%s -> %s\n", entries[i].key, entries[i].kata);
    }
}

int main(){
    insertKamus ("apel", "apple");
    printsortedkamus();
    char nyari [20];

    return 0;
}




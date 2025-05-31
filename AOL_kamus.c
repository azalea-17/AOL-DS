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
void printSortedKamus() {
    hashNode temp[DICT_SIZE];
    int count = 0;

    for (int i = 0; i < DICT_SIZE; i++) {
        if (Kamus[i].key[0] != '\0') {
            temp[count++] = Kamus[i];
        }
    }

        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(temp[j].key, temp[j + 1].key) > 0) {
                hashNode swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }



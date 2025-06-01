#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_SIZE 300
#define EMPTY "NULL"

typedef struct hashNode{
    char key[20];
    char kata[20];
    int deleted;
}hashNode;

hashNode Kamus[DICT_SIZE];

//struct buat sorting
typedef struct {
    char key[20];
    char kata[20];
} entry;

//Inisialisasi Hash Table
void initKamus() {
    for(int i = 0; i < DICT_SIZE; i++) {
        Kamus[i].key[0] = '\0'; 
        Kamus[i].kata[0] = '\0'; 
        Kamus[i].deleted = 0; 
    }
}

unsigned long hash(const char *key){
    unsigned long hash = 5381;

    int c;
    while((c = *key++)){
        hash = ((hash << 5) + hash) + c;  
    }

    return hash;
}

int hashToIndex(const char *key){
    return hash(key) % DICT_SIZE;
}

//insert pake linear probing
void insertKamus(const char *key, const char *value){
    int index = hashToIndex(key);
    int startIndex = index;
    
    while(Kamus[index].key[0] != '\0' && !Kamus[index].deleted){
        if(strcmp(Kamus[index].key, key) == 0){
            // Jika key sudah ada, update value
            strcpy(Kamus[index].kata, value);
            return;
        }
        index = (index + 1) % DICT_SIZE;
    }


    strcpy(Kamus[index].kata, value);
    strcpy(Kamus[index].key, key);
    Kamus[index].deleted = 0; 
}

const char *searchKamus(const char *key){
    int index = hashToIndex(key);
    int startIndex = index;
    
   while(Kamus[index].key[0] != '\0'){
        if(!Kamus[index].deleted && strcmp(Kamus[index].key, key) == 0){
            return Kamus[index].kata;
        }
        index = (index + 1) % DICT_SIZE;
        if(index == startIndex){
            break;
        }
    }
    return NULL;
}

void compareEntries(const void *a, const void *b){
    entry *entryA = (entry *)a;
    entry *entryB = (entry *)b;
    return strcmp(entryA->key, entryB->key);
}

//fungsi search
void searchAndPrint(const char *key){
    const char *result = searchKamus(key);
    if(result != NULL){
        printf("Ditemukan: %s -> %s\n", key, result);
    } else {
        printf("Key '%s' tidak ditemukan dalam kamus\n", key);
    }
}

//fungsi delete pake lazy deletion
void deleteKamus(const char *key){
    int index = hashToIndex(key);
    int startIndex = index;

    while(Kamus[index].key[0] != '\0'){
        if(!Kamus[index].deleted && strcmp(Kamus[index].key, key) == 0){
            Kamus[index].deleted = 1; 
            printf("Key '%s' telah dihapus dari kamus\n", key);
            return;
        }
        index = (index + 1) % DICT_SIZE;
        if(index == startIndex){
            break;
        }
    }
    printf("Key '%s' tidak ada untuk dihapus\n", key);
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
        if(Kamus[i].key[0] != '\0' && !Kamus[i].deleted){
            strcpy(entries[count].key, Kamus[i].key);
            strcpy(entries[count].kata, Kamus[i].kata);
            count++;
        }
    }
    qsort(entries, count, sizeof(entry), compareEntries);
    printf("\nIsi Kamus (sorted alphabetically):\n");
    for(int i=0; i<count; i++){
        printf("%s -> %s\n", entries[i].key, entries[i].kata);
    }
}

// Fungsi untuk menampilkan menu
void displayMenu(){
    printf("\n=== KAMUS HASH TABLE ===\n");
    printf("1. Tambah kata\n");
    printf("2. Cari kata\n");
    printf("3. Hapus kata\n");
    printf("4. Tampilkan semua kata (sorted)\n");
    printf("5. Keluar\n");
    printf("Pilih opsi (1-5): ");
}


int main(){
    
    initKamus();
    insertKamus ("apel", "apple");
    insertKamus ("Buku", "book");
    insertKamus ("Cinta", "love");
    insertKamus ("Domba", "sheep");
    insertKamus ("Ekor", "tail");
    insertKamus ("Fajar", "dawn");
    insertKamus ("Gitar", "guitar");
    insertKamus ("Hujan", "rain");
    insertKamus ("Ikan", "fish");
    insertKamus ("Jendela", "window");
    insertKamus ("Kucing", "cat");
    insertKamus ("Laut", "sea");
    insertKamus ("Matahari", "sun");
    insertKamus ("Nasi", "rice");
    insertKamus ("Ombak", "wave");
    insertKamus ("Pohon", "tree");
    insertKamus ("Rumah", "house");
    insertKamus ("Sapi", "cow");
    insertKamus ("Tikus", "mouse");
    insertKamus ("Ular", "snake");
    insertKamus ("Vaksin", "vaccine");
    insertKamus ("Waktu", "time");
    printSortedKamus();
    char nyari [20];

    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define DICT_SIZE 50
#define EMPTY "NULL"


typedef struct hashNode{
    char key[20];
    char kata[20];
    int deleted;
}hashNode;

typedef struct dict{
    char key;
    hashNode page[DICT_SIZE];

    struct dict *next;
    struct dict *prev;
}dict;

//struct buat sorting
typedef struct {
    char key[20];
    char kata[20];
} entry;

dict* createNewPage(char key){
    dict* newPage = (dict*)malloc(sizeof(dict));
    newPage->key = key;
    newPage->next = NULL;
    newPage->prev = NULL;

    return newPage;
}

void appendPage(dict** Dict, dict* newPage){
    if(!(*Dict)){
        *Dict = newPage;
        return;
    }
    
    dict* current = *Dict;  
    while(current->next){
        current = current->next;
    }

    newPage->prev = current;
    current->next = newPage;
}

//Inisialisasi Hash Table
void initKamus(dict** Dict) {
    for(char i = 'a'; i <= 'z'; i++){
        appendPage(Dict, createNewPage(i));
    }
    
    dict* current = *Dict;
    while(current){
        for(int i = 0; i < DICT_SIZE; i++) {
            current->page[i].key[0] = '\0'; 
            current->page[i].kata[0] = '\0'; 
            current->page[i].deleted = 0; 
        }

        current = current->next;
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
void insertKamus(dict* Dict, const char *key, const char *value){
    char tempVal[strlen(value) + 1];
    for(int i = 0; i < strlen(value); i++){
        tempVal[i] = tolower(value[i]);
    }
    tempVal[strlen(value)] = '\0';
    
    char tempKey[strlen(key) + 1];
    for(int i = 0; i < strlen(key); i++){
        tempKey[i] = tolower(key[i]);
    }
    tempKey[strlen(key)] = '\0';
    
    int index = hashToIndex(tempKey);
    int startIndex = index;

    dict* current = Dict;
    while(current && current->key != tempKey[0]){
        current = current->next;
    }
    if(!current)return;


    while(current->page[index].key[0] != '\0' && !current->page[index].deleted){
        if(strcmp(current->page[index].key, tempKey) == 0){
            // Jika key sudah ada, update value
            strcpy(current->page[index].kata, tempVal);
            return;
        }
        index = (index + 1) % DICT_SIZE;
    }



    strcpy(current->page[index].kata, tempVal);
    strcpy(current->page[index].key, tempKey);
    current->page[index].deleted = 0; 

}

const char *searchKamus(dict* Dict, const char *key){
    int index = hashToIndex(key);
    int startIndex = index;
    
    dict* current = Dict;
    while(current && current->key != key[0]){
        current = current->next;
    }

    if(!current) return NULL;

    while(current->page[index].key[0] != '\0'){
        if(!current->page[index].deleted && strcmp(current->page[index].key, key) == 0){
            return current->page[index].kata;
        }
        index = (index + 1) % DICT_SIZE;
        if(index == startIndex){
            break;
        }
    }
    return NULL;
}

//fungsi search
void searchAndPrint(dict* Dict, const char *key){
    const char *result = searchKamus(Dict, key);
    if(result != NULL){
        system("cls");
        printf("    KAMUS HASH TABLE    \n");
        printf("========================\n");
        printf("HALAMAN : %c\n", key[0]);
        printf("Ditemukan: %s -> %s\n", key, result);
        printf("========================\n");
        printf("\n\nTekan ENTER untuk balik...");
        getchar();
    } else {
        printf("Key '%s' tidak ditemukan dalam kamus\n", key);
        printf("\n\nTekan ENTER untuk balik...");
        getchar();
    }
}

//fungsi delete pake lazy deletion
void deleteKamus(dict* Dict, const char *key){
    int index = hashToIndex(key);
    int startIndex = index;

    dict* current = Dict;
    while(current && current->key != key[0]){
        current = current->next;
    }

    if(!current) {
        printf("Kata yang ingin dihapus tidak ditemukan!\n");
        printf("\n\nTekan ENTER untuk balik...");
        getchar();
        return;
    }

    while(current->page[index].key[0] != '\0'){
        if(!current->page[index].deleted && strcmp(current->page[index].key, key) == 0){
            current->page[index].deleted = 1; 
            printf("\nKey '%s' telah dihapus dari kamus\n", key);
            printf("\n\nTekan ENTER untuk balik...");
            getchar();
            return;
        }
        index = (index + 1) % DICT_SIZE;
        if(index == startIndex){
            break;
        }
    }
    printf("Kata yang ingin dihapus tidak ditemukan!\n");
    printf("\n\nTekan ENTER untuk balik...");
    getchar();
}

int compareEntries(const void *a, const void *b){
    entry *entryA = (entry *)a;
    entry *entryB = (entry *)b;
    return strcmp(entryA->key, entryB->key);
}

void printSortedKamus(dict* currentPage){
    entry entries[DICT_SIZE];
    int count = 0;
    printf("------------------------\n");
    printf("|   KAMUS HASH TABLE   |\n");
    printf("------------------------\n");
    printf("HALAMAN : %c\n", currentPage->key);
    printf("========================\n");
    
    for(int i =0;i<DICT_SIZE;i++){
        if(currentPage->page[i].key[0] != '\0' && !currentPage->page[i].deleted){
            strcpy(entries[count].key, currentPage->page[i].key);
            strcpy(entries[count].kata, currentPage->page[i].kata);
            count++;
        }
    }
    
    qsort(entries, count, sizeof(entries[0]), compareEntries);
    if(count == 0){
        printf("Halaman ini kosong!\n");
    }
    for(int i=0; i<count; i++){
        printf("%d. %s -> %s\n", i + 1, entries[i].key, entries[i].kata);
    }
    printf("========================\n");
}

void initIsiKamus(dict* Dictionary){
    insertKamus(Dictionary, "Apel", "apple");
    insertKamus(Dictionary, "Buku", "book");
    insertKamus(Dictionary, "Cinta", "love");
    insertKamus(Dictionary, "Domba", "sheep");
    insertKamus(Dictionary, "Ekor", "tail");
    insertKamus(Dictionary, "Fajar", "dawn");
    insertKamus(Dictionary, "Gitar", "guitar");
    insertKamus(Dictionary, "Hujan", "rain");
    insertKamus(Dictionary, "Ikan", "fish");
    insertKamus(Dictionary, "Jendela", "window");
    insertKamus(Dictionary, "Kucing", "cat");
    insertKamus(Dictionary, "Laut", "sea");
    insertKamus(Dictionary, "Matahari", "sun");
    insertKamus(Dictionary, "Nasi", "rice");
    insertKamus(Dictionary, "Ombak", "wave");
    insertKamus(Dictionary, "Pohon", "tree");

    insertKamus(Dictionary, "Rumah", "house");
    insertKamus(Dictionary, "Sapi", "cow");
    insertKamus(Dictionary, "Tikus", "mouse");
    insertKamus(Dictionary, "Ular", "snake");
    insertKamus(Dictionary, "Vaksin", "vaccine");
    insertKamus(Dictionary, "Waktu", "time");
}

void inputKata(dict* Dictionary, dict* currentPage){
    char kata[20];
    char arti[20];

    printSortedKamus(currentPage);

    getchar();
    
    printf("Input kata : ");
    fgets(kata, sizeof(kata), stdin);

    printf("Input arti dalam bahasa Inggris : ");
    fgets(arti, sizeof(arti), stdin);

    kata[strcspn(kata, "\n")] = '\0';
    arti[strcspn(arti, "\n")] = '\0';   

    insertKamus(Dictionary, kata, arti);

    printf("Kata sudah dimasukkan!");
    printf("\n\nTekan ENTER untuk balik...");
    getchar();
}

void cariKata(dict* Dictionary, dict* currentPage){
    char nyari [20];
    printSortedKamus(currentPage);
    getchar();
    printf("Input kata yang ingin dicari : ");
    fgets(nyari, sizeof(nyari), stdin);

    nyari[strcspn(nyari, "\n")] = '\0';

    searchAndPrint(Dictionary, nyari);
}

void inputHapus(dict* Dictionary, dict* currentPage){
    char nyari [20];
    printSortedKamus(currentPage);
    getchar();
    printf("Input kata yang ingin dihapus : ");
    fgets(nyari, sizeof(nyari), stdin);

    nyari[strcspn(nyari, "\n")] = '\0';

    deleteKamus(Dictionary, nyari);
}

// Fungsi untuk menampilkan menu
void displayMenu(dict* currentPage){
    printSortedKamus(currentPage);
    printf("1. Tambah kata\n");
    printf("2. Cari kata\n");
    printf("3. Hapus kata\n");
    printf("4. Halaman Selanjutnya\n");
    printf("5. Halaman Sebelumnya\n");
    printf("6. Keluar\n");
}


int main(){
    dict* Dictionary = NULL;
    initKamus(&Dictionary);
    initIsiKamus(Dictionary);

    dict* tempDict = Dictionary;
    int choice;
    while(true){
        do{
            system("cls");
            displayMenu(tempDict);
            printf(">> ");
            scanf("%d", &choice);
        }while(choice < 1 || choice > 6);

        system("cls");

        switch(choice){
            case 1 :
                inputKata(Dictionary, tempDict);
                break;

            case 2 :
                cariKata(Dictionary, tempDict);
                break;

            case 3 :
                inputHapus(Dictionary, tempDict);
                break;

            case 4 :
                if(tempDict->next)tempDict = tempDict->next;
                else{
                    printSortedKamus(tempDict);
                    printf("Ini halaman terakhir!");
                    printf("\n\nTekan ENTER untuk balik...");
                    getchar();
                    getchar();
                }
                break;

            case 5 :
                if(tempDict->prev)tempDict = tempDict->prev;
                else{
                    printSortedKamus(tempDict);
                    printf("Ini halaman pertama!");
                    printf("\n\nTekan ENTER untuk balik...");
                    getchar();
                    getchar();
                }
                break;
            case 6 :
                return 0;

        }
    }

    return 0;
}



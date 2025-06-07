#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define DICT_SIZE 500

typedef struct hashNode {
    char key[20];
    char kata[20];
    int deleted;
} hashNode;

typedef struct dict {
    char key;
    hashNode page[DICT_SIZE];
    struct dict* next;
    struct dict* prev;
} dict;

typedef struct {
    char key[20];
    char kata[20];
} entry;

dict* createNewPage(char key) {
    dict* newPage = (dict*)malloc(sizeof(dict));
    newPage->key = key;
    newPage->next = NULL;
    newPage->prev = NULL;
    return newPage;
}

void appendPage(dict** Dict, dict* newPage) {
    if (!(*Dict)) {
        *Dict = newPage;
        return;
    }
    dict* current = *Dict;
    while (current->next) {
        current = current->next;
    }
    newPage->prev = current;
    current->next = newPage;
}

void initKamus(dict** Dict) {
    for (char i = 'a'; i <= 'z'; i++) {
        appendPage(Dict, createNewPage(i));
    }
    dict* current = *Dict;
    while (current) {
        for (int i = 0; i < DICT_SIZE; i++) {
            current->page[i].key[0] = '\0';
            current->page[i].kata[0] = '\0';
            current->page[i].deleted = 0;
        }
        current = current->next;
    }
}

unsigned long hash(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int hashToIndex(const char* key) {
    return hash(key) % DICT_SIZE;
}

void insertKamus(dict* Dict, const char* key, const char* value) {
    char tempVal[100], tempKey[100];
    int lenK = strlen(key), lenV = strlen(value);
    for (int i = 0; i < lenK; i++) tempKey[i] = tolower(key[i]);
    for (int i = 0; i < lenV; i++) tempVal[i] = tolower(value[i]);
    tempKey[lenK] = '\0';
    tempVal[lenV] = '\0';

    dict* current = Dict;
    while (current && current->key != tempKey[0]) {
        current = current->next;
    }
    if (!current) return;
    
    int index = hashToIndex(tempKey);
    while (current->page[index].key[0] != '\0' && !current->page[index].deleted) {
        if (strcmp(current->page[index].key, tempKey) == 0) {
            strcpy(current->page[index].kata, tempVal);
            return;
        }
        index = (index + 1) % DICT_SIZE;
    }

    strcpy(current->page[index].kata, tempVal);
    strcpy(current->page[index].key, tempKey);
    current->page[index].deleted = 0;
}

const char* searchKamus(dict* Dict, const char* key) {
    char tempKey[100];
    int lenK = strlen(key);
    for (int i = 0; i < lenK; i++) tempKey[i] = tolower(key[i]);
    tempKey[lenK] = '\0';
    
    int index = hashToIndex(tempKey);
    dict* current = Dict;
    while (current && current->key != tempKey[0]) {
        current = current->next;
    }
    if (!current) return NULL;

    int startIndex = index;
    while (current->page[index].key[0] != '\0') {
        if (!current->page[index].deleted && strcmp(current->page[index].key, tempKey) == 0) {
            return current->page[index].kata;
        }
        index = (index + 1) % DICT_SIZE;
        if (index == startIndex) break;
    }
    return NULL;
}

void searchAndPrint(dict* Dict, const char* key) {
    const char* result = searchKamus(Dict, key);
    system("cls");
    printf("    KAMUS HASH TABLE    \n");
    printf("========================\n");
    if (result != NULL) {
        printf("HALAMAN : %c\n", tolower(key[0]));
        printf("Ditemukan: %s -> %s\n", key, result);
    } else {
        printf("Key '%s' tidak ditemukan dalam kamus\n", key);
    }
    printf("========================\n");
    printf("\n\nTekan ENTER untuk balik...");
    getchar();
}

void deleteKamus(dict* Dict, const char* key) {
    char tempKey[100];
    int lenK = strlen(key);
    for (int i = 0; i < lenK; i++) tempKey[i] = tolower(key[i]);
    tempKey[lenK] = '\0';
    
    int index = hashToIndex(tempKey);
    dict* current = Dict;
    while (current && current->key != tempKey[0]) {
        current = current->next;
    }
    if (!current) {
        system("cls");
        printf("Kata yang ingin dihapus tidak ditemukan!\n");
        printf("\n\nTekan ENTER untuk balik...");
        getchar();
        return;
    }

    int startIndex = index;
    while (current->page[index].key[0] != '\0') {
        if (!current->page[index].deleted && strcmp(current->page[index].key, tempKey) == 0) {
            current->page[index].deleted = 1;
            system("cls");
            printf("Key '%s' telah dihapus\n", key);
            printf("\n\nTekan ENTER untuk balik...");
            getchar();
            return;
        }
        index = (index + 1) % DICT_SIZE;
        if (index == startIndex) break;
    }

    system("cls");
    printf("Kata yang ingin dihapus tidak ditemukan!\n");
    printf("\n\nTekan ENTER untuk balik...");
    getchar();
}

int compareEntries(const void* a, const void* b) {
    return strcmp(((entry*)a)->key, ((entry*)b)->key);
}

void printSortedKamus(dict* currentPage) {
    entry entries[DICT_SIZE];
    int count = 0;
    system("cls");
    printf("---------------------------------------------------------------------------\n");
    printf("|                            KAMUS HASH TABLE                             |\n");
    printf("---------------------------------------------------------------------------\n");
    printf("|HALAMAN : %c                                                              |\n", toupper(currentPage->key));
    printf("===========================================================================\n");


    for (int i = 0; i < DICT_SIZE; i++) {
        if (currentPage->page[i].key[0] != '\0' && !currentPage->page[i].deleted) {
            strcpy(entries[count].key, currentPage->page[i].key);
            strcpy(entries[count].kata, currentPage->page[i].kata);
            count++;

        }
    }

    //qsort(entries, count, sizeof(entry), compareEntries);
    if (count == 0) {
        printf("Halaman ini kosong!\n");
    }

    int iter = (count % 2 == 0) ? count / 2 : count / 2 + 1;
    for (int i = 0; i < iter; i++) {
        printf("%-2d. %-12s -> %-15s%-4s", i + 1, entries[i].key, entries[i].kata, "||");
        if(entries[i + iter].key[0] != '\0')printf("%-2d. %-12s -> %-15s|\n", i + 1 + iter, entries[i + iter].key, entries[i + iter].kata);
        else printf("\n");
    }
    printf("===========================================================================\n");

    for(int i = 0; i < DICT_SIZE; i++){
        entries[i].kata[0] = '\0';
        entries[i].key[0] = '\0';
    }
}

void inputKata(dict* Dictionary, dict* currentPage) {
    char kata[20], arti[20];
    printSortedKamus(currentPage);
    printf("Input kata : ");
    if (fgets(kata, sizeof(kata), stdin)) {
        kata[strcspn(kata, "\n")] = '\0';
    }
    printf("Input arti dalam bahasa Inggris : ");
    if (fgets(arti, sizeof(arti), stdin)) {
        arti[strcspn(arti, "\n")] = '\0';
    }
    insertKamus(Dictionary, kata, arti);
    printf("Kata sudah dimasukkan!\n\nTekan ENTER untuk balik...");
    getchar();
}

void cariKata(dict* Dictionary, dict* currentPage) {
    char nyari[20];
    printSortedKamus(currentPage);
    printf("Input kata yang ingin dicari : ");
    if (fgets(nyari, sizeof(nyari), stdin)) {
        nyari[strcspn(nyari, "\n")] = '\0';
    }
    searchAndPrint(Dictionary, nyari);
}

void inputHapus(dict* Dictionary, dict* currentPage) {
    char nyari[20];
    printSortedKamus(currentPage);
    printf("Input kata yang ingin dihapus : ");
    if (fgets(nyari, sizeof(nyari), stdin)) {
        nyari[strcspn(nyari, "\n")] = '\0';
    }
    deleteKamus(Dictionary, nyari);
}

void displayMenu(dict* currentPage) {
    printSortedKamus(currentPage);
    printf("1. Tambah kata\n");
    printf("2. Cari kata\n");
    printf("3. Hapus kata\n");
    printf("4. Halaman Selanjutnya\n");
    printf("5. Halaman Sebelumnya\n");
    printf("6. Keluar\n");
}

void loadFromFile(dict* Dictionary, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Gagal membuka file %s\n", filename);
        printf("Pastikan file vocab.txt ada di folder yang sama dengan program\n");
        printf("\n\nTekan ENTER untuk lanjut...");
        getchar();
        return;
    }
    char indo[100], eng[100];
    int loaded = 0;
    while (fscanf(file, "%s %s", indo, eng) == 2) {
        insertKamus(Dictionary, indo, eng);
        loaded++;
    }
    fclose(file);
}

int main() {
    dict* Dictionary = NULL;
    initKamus(&Dictionary);
    loadFromFile(Dictionary, "vocab.txt");

    int pilihMainMenu;
    do{
        printf("=======================================\n");
        printf("KAMUS BAHASA INDONESIA - BAHASA INGGRIS\n");
        printf("=======================================\n");
        printf("1. Buka Kamus\n2. Keluar\n>> ");
        scanf("%d", &pilihMainMenu);
    }while(pilihMainMenu > 2 || pilihMainMenu < 1);

    if(pilihMainMenu == 2)return 0;

    dict* tempDict = Dictionary;
    int choice;
    while (true) {
        system("cls");
        do {
            displayMenu(tempDict);
            printf(">> ");
            scanf("%d", &choice);
            getchar(); 
        } while (choice < 1 || choice > 6);

        switch (choice) {
            case 1:
                inputKata(Dictionary, tempDict);
                break;
            case 2:
                cariKata(Dictionary, tempDict);
                break;
            case 3:
                inputHapus(Dictionary, tempDict);
                break;
            case 4:
                if (tempDict->next) tempDict = tempDict->next;
                else {
                    printSortedKamus(tempDict);
                    printf("Ini halaman terakhir!\n\nTekan ENTER untuk balik...");
                    getchar();
                }
                break;
            case 5:
                if (tempDict->prev) tempDict = tempDict->prev;
                else {
                    printSortedKamus(tempDict);
                    printf("Ini halaman pertama!\n\nTekan ENTER untuk balik...");
                    getchar();
                }
                break;
            case 6:
                printf("Terima kasih telah menggunakan Kamus Hash Table!\n");
                return 0;
        }
    }
}

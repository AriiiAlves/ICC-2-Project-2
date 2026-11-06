#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// STRUCTS
typedef struct hashObj{
    char *string;
    int count;
} HashObj;

typedef struct hashtable{
    HashObj *table;
    int C1;
    int C2;
    int size;
} HashTable;

// Inits hash table with malloc
HashTable *initHash(int C1, int C2, int S){
    HashTable *hashTable = malloc(sizeof(HashTable));
    hashTable->table = malloc(sizeof(HashObj) * S);
    for(int i = 0; i < S; i++){
        hashTable->table[i].string = NULL;
        hashTable->table[i].count = 0;
    }

    hashTable->C1 = C1;
    hashTable->C2 = C2;
    hashTable->size = S;
    return hashTable;
}

// Free hash table variable and content
void freeHash(HashTable *hashTable){
    for(int i = 0; i < hashTable->size; i++){
        if(hashTable->table[i].string != NULL) free(hashTable->table[i].string);
    }

    free(hashTable->table);
    free(hashTable);
}

// Hash function
int hash(char *string, int S){
    int v = 0;
    int len = strlen(string);
    for(int i = 0; i < len; i++){
        v += v*3 + (unsigned char)string[i];
        v = v % S;
    }
    return v;
}

// Search function
int searchHash(HashTable *hashTable, char *string){
    int h = hash(string, hashTable->size);
    int v = h;
    int i = 1;

    // If different or "=", keep trying to find. If NULL, stop.
    while(hashTable->table[v].string != NULL && strcmp(hashTable->table[v].string, string)){
        if(i == hashTable->size) return -1; // Avoids loop

        v = h + hashTable->C1 * i + hashTable->C2 * i * i;
        v = v % hashTable->size;
        i++;
    }

    if(hashTable->table[v].string == NULL) return -1;
    return v;
}

// Insert function
int insertHash(HashTable *hashTable, char *string){
    int h = hash(string, hashTable->size);
    int v = h;
    int i = 1;
    char del_marker[] = "=";

    // If already exists, returns
    int found = searchHash(hashTable, string);
    if(found != -1){
        hashTable->table[found].count++;
        return found;
    }

    // Try to find NULL or "=" (del marker)
    while(hashTable->table[v].string != NULL && strcmp(hashTable->table[v].string, del_marker) && strcmp(hashTable->table[v].string, string)){
        if(i == hashTable->size) return -1; // Avoids loop
        
        v = h + hashTable->C1 * i + hashTable->C2 * i * i;
        v = v % hashTable->size;
        i++;
    }

    if(hashTable->table[v].string != NULL && !strcmp(hashTable->table[v].string, del_marker)){
        free(hashTable->table[v].string);
        hashTable->table[v].string = NULL;
    }
    if(hashTable->table[v].string == NULL){
        hashTable->table[v].string = malloc(sizeof(char) * (strlen(string) + 1));
        hashTable->table[v].string = strcpy(hashTable->table[v].string, string);
    }

    hashTable->table[v].count++;
    return v;
}

// Remove function
int removeHash(HashTable *hashTable, char *string){
    int v = searchHash(hashTable, string);
    if(v == -1) return -1;

    char del_marker[] = "=";
    strcpy(hashTable->table[v].string, del_marker);
    hashTable->table[v].count = 0;

    return v;
}

int main(){
    int S=30, C1=1, C2=0, T=-1;

    scanf("%d %d %d", &S, &C1, &C2);
    int c = 0;
    while((c = getchar()) != '\n' && c != EOF); // discard the rest of the line

    HashTable* hashTable = initHash(C1, C2, S);

    while(1){
        if(T == 0) break;
        // Reads text
        if(T == 1){
            char text[120002];
            char word[31];

            fgets(text, sizeof(text), stdin);

            // Catches words separated by spaces
            int i = 0, j = 0;
            while(text[i] != '\0' && text[i] != '\n'){
                if(text[i] == ' '){
                    if(j > 0){
                        word[j] = '\0'; // Ends string
                        insertHash(hashTable, word);
                        j = 0; // Resets for new string
                    }
                } else {
                    word[j] = text[i];
                    j++;
                }

                i++;
            }
            // Inserts last word
            if(j > 0){
                word[j] = '\0';
                insertHash(hashTable, word);
            }
        }
        // Shows different words
        if(T == 2){
            int Wcount = 0;
            int Vmax = 0;
            char del_marker[] = "=";

            for(int i = 0; i < hashTable->size; i++){
                if(hashTable->table[i].string == NULL || !strcmp(hashTable->table[i].string, del_marker)) continue;
                Wcount++;
                if(hashTable->table[i].count > hashTable->table[Vmax].count) Vmax = i;
                if(hashTable->table[i].count == hashTable->table[Vmax].count && strcmp(hashTable->table[i].string, hashTable->table[Vmax].string) < 0) Vmax = i;
            }

            if(Wcount) printf("foram encontradas %d palavras diferentes\npalavra mais frequente = %s, encontrada %d vezes\n", Wcount, hashTable->table[Vmax].string, hashTable->table[Vmax].count);
            else printf("nenhuma palavra encontrada\n");
        }
        // Try to find words
        if(T == 3){
            int n;
            char word[31];
            scanf("%d", &n);

            for(int i = 0; i < n; i++){
                scanf("%s", word); // "%s " skips all ' ' that follows, including \n
                int v = searchHash(hashTable, word);
                if(v == -1) printf("%s nao encontrada\n", word);
                else printf("%s encontrada %d\n", word, hashTable->table[v].count);
            }
        }
        // Remove words
        if(T == 4){
            int n;
            char word[31];
            scanf("%d", &n);

            for(int i = 0; i < n; i++){
                scanf("%s", word); // "%s " skips all ' ' that follows, including \n
                int v = removeHash(hashTable, word);
                if(v == -1) printf("%s nao encontrada\n", word);
                else printf("%s removida\n", word);
            }
        }
        // Prints hash table
        if(T == 5){
            char del_marker[] = "=";

            printf("imprimindo tabela hash\n");
            for(int i = 0; i < hashTable->size; i++){
                if(hashTable->table[i].string == NULL || !strcmp(hashTable->table[i].string, del_marker)) continue;
                printf("%s %d\n", hashTable->table[i].string, i);
            }
            printf("fim da tabela hash\n");
        }

        int c;
        scanf("%d", &T);
        while((c = getchar()) != '\n' && c != EOF); // discard the rest of the line
    }

    freeHash(hashTable);
    return 0;
}
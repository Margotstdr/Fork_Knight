#include "../headers/array-utils.h"
#include <stdio.h>
#include <stdlib.h>


int addPlayer(Player* players[], int* count, Player* player) {

    if (*count >= MAX_PLAYERS || player == NULL) {
        printf("La capacité maximale de joueurs à été atteinte.");
        return 0;
    }

    players[*count] = player;
    (*count)++;
    return 1;

}

int searchPlayerByUsername(Player* players[], int count, const char* username) {

    if (players == NULL || username == NULL || count <= 0) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(players[i]->username, username) == 0) {
            return i;
        }
    }

    return -1;
}

int removePlayer(Player* players[], int* count, const char* username) {

    if (players == NULL || username == NULL || *count <= 0) {
        return 0;
    }

    int elt = -1;

    for (int i = 0; i < *count; i++) {
        if (players[i] != NULL && strcmp(players[i]->username, username) == 0) {
            elt = i;
            break;
        }
    }

    if (elt == -1) {
        return 0;
    }

    destroyPlayer(players[elt]);

    for (int i = elt; i < *count - 1; i++) {
        players[i] = players[i + 1];
    }

    players[*count-1] = NULL;
    (*count)--;

    return 1;
}

int comparePlayer(const Player* p1, const Player* p2) {

    if (p1 == NULL || p2 == NULL) {
        return -1;
    }

    if (p1->spicyIndex < p2->spicyIndex) {
        return 1;
    } if (p1->spicyIndex > p2->spicyIndex) {
        return 2;
    }
    return 0;

}

void displayAllPlayers(Player* players[], int count) {

    if (players == NULL || count == 0) {
        return;
    }

    printf("=== Liste des joueurs ===\n");

    for (int i = 0; i < count; i++) {
        if (players[i] != NULL) {
            displayPlayerMini(players[i]);
            printf("\n");
        }
    }

    printf("Total: %d joueurs\n", count);
}


void sortPlayersBySpicyIndex(Player* players[], int count) {

    if (players == NULL || count <= 1) {
        return;
    }

    for (int i = 1; i < count; i++) {

        Player* key = players[i];
        int j = i - 1;

        while (j >= 0 && comparePlayer(players[j], key) == 1) {

            players[j + 1] = players[j];
            j--;

        }

        players[j + 1] = key;

    }
}


// ============================================================================
// Part 2: Dynamic array management
// ============================================================================

Player** createPlayerArray(int size) {
    if (size <= 0) {
        return NULL;
    }

    Player **listPlayer = malloc(size * sizeof(Player*));
    if (listPlayer == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        listPlayer[i] = NULL;
    }

    return listPlayer;
}


void destroyPlayerArray(Player** players, int size){
    if(players ==NULL)return;

    for(int i =0; i<size;i++){
        if(players[i]!= NULL){
            destroyPlayer(players[i]);
        }
    }
    free(players);
}

// ============================================================================
// Part 2: File I/O
// ============================================================================

Player** loadPlayers(const char* filename, int* numPlayers) {

    if (filename == NULL || numPlayers == NULL) {
        return NULL;
    }

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier");
        *numPlayers = -1; /* indicate error */
        return NULL;
    }

    if (fscanf(f, "%d", numPlayers) != 1) {
        printf("Erreur : Impossible de lire le nombre de joueurs.\n");
        fclose(f);
        *numPlayers = -1; /* indicate error */
        return NULL;
    }

    if (*numPlayers == 0) {
        fclose(f);
        /* explicitly indicate 0 players (not an error) */
        return NULL;
    }

    Player** players = createPlayerArray(*numPlayers);

    if (players == NULL) {
        fclose(f);
        *numPlayers = 0;
        return NULL;
    }

    char username[100];
    int level;
    int spicy;
    int numGames;
    int numWins;
    int numLosses;
    int inQueue;

    for (int i = 0; i < *numPlayers; i++) {

        int readCount = fscanf(f, "%s %d %d %d %d %d %d", username, &level, &spicy, &numGames, &numWins, &numLosses, &inQueue);

        if (readCount != 7) {
            destroyPlayerArray(players, *numPlayers);
            fclose(f);
            *numPlayers = -1; /* indicate malformed file */
            return NULL;
        }

        Player* p = createPlayer(username);

        if (p == NULL) {
            destroyPlayerArray(players, i);
            fclose(f);
            *numPlayers = -1; /* indicate allocation failure */
            return NULL;
        }

        p->level = level;
        p->spicyIndex = spicy;
        p->numGames = numGames;
        p->numWins = numWins;
        p->numLosses = numLosses;
        p->inQueue = inQueue;

        players[i] = p;
    }

    fclose(f);
    return players;

}

void savePlayers(const char* filename, Player** array, int size) {

    if (filename == NULL || array == NULL) {
        return;
    }

    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier en écriture");
        return;
    }

    fprintf(f, "%d\n", size);

    for (int i = 0; i < size; i++) {
        if (array[i] != NULL) {
            fprintf(f, "%s %d %d %d %d %d %d\n",
                array[i]->username,
                array[i]->level,
                array[i]->spicyIndex,
                array[i]->numGames,
                array[i]->numWins,
                array[i]->numLosses,
                array[i]->inQueue);
        }
    }

    fclose(f);

}

#include "../headers/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Player* createPlayer(const char* username) {
    if(username == NULL){
        printf("Allocation nouveau joueur échouée");
        return NULL;
    }

    //on alloue de la mémoire pour un joueur
    Player *newPlayer = malloc(sizeof(Player));
    if(newPlayer == NULL){
        return NULL;
    }

    //maintenant on peut initialiser les données du joueur
    //d'abord on copie le nom du joueur

    strncpy(newPlayer->username, username, sizeof(newPlayer->username)); // en gros on copie le username
    //dans l'attribut newPlayer->username de taille newPlayer->username
    newPlayer->username[sizeof(newPlayer->username)-1] = '\0'; //petite mesure de sécurité
    //quand on fait strncpy, le '\0' n'est pas mis, il prend pile le nombre de caractère

    //initialisation des autres attributs
    newPlayer->level = 1;
    newPlayer->spicyIndex = 1000;
    newPlayer->numGames = 0;
    newPlayer->numLosses = 0;
    newPlayer->numWins = 0;
    newPlayer->inQueue = 0;

    return newPlayer;
}


void destroyPlayer(Player* player) {
    if(player ==NULL)return;

    free(player);
}

void displayPlayer(const Player* player) {
    if(player ==NULL)return;

    printf("==========================================\n");
    printf("Player %s\n", player->username);
    printf("Level : %d | SpicyIndex : %d\n", player->level, player->spicyIndex);
    printf("Games : %d | Wins : %d | Losses : %d\n ", player->numGames, player->numWins, player->numLosses);
    printf("==========================================\n");
}

void displayPlayerMini(const Player* player) {
    if(player ==NULL)return;

    printf("[%s | Lvl : %d | SI : %d]\n", player->username, player->level, player->spicyIndex);
}

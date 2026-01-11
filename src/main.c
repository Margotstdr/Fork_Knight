#include <stdio.h>
#include <stdlib.h>
#include "../headers/player.h"
#include "../headers/array-utils.h"
#include "../headers/list-utils.h"
#include "../headers/priority-queue.h"
#include "../headers/match.h"

int main () {

    Player* players[MAX_PLAYERS] = {NULL};
    int count = 0;
    int choice = 0;
    int idx = -1;
    int n = 0;
    Player *tmpPlayer = NULL;
    char filename[256];
    char username[100];

    t_queue *queue = malloc(sizeof(t_queue));
    if (queue != NULL) {
        queue->head = NULL;
        queue->tail = NULL;
    }


    do {
        printf("============================================\n");
        printf("        FORK KNIGHT - MATCHMAKER\n");
        printf("============================================\n");
        printf("1. Charger les joueurs depuis un fichier\n");
        printf("2. Sauvegarder les joueurs dans un fichier\n");
        printf("3. Ajouter un nouveau joueur\n");
        printf("4. Retirer un joueur\n");
        printf("5. Rechercher un joueur\n");
        printf("6. Afficher tous les joueurs\n");
        printf("7. Trier par spicyIndex\n");
        printf("8. Quitter\n");
        printf("9. Ajouter un joueur à la file d'attente\n");
        printf("10. Retirer un joueur de la file d'attente\n");
        printf("11. Afficher la file d'attente\n");
        printf("12. Former un match (retirer jusqu'à 12 joueurs)\n");
        printf("13. Ajouter un joueur à la file de priotité\n");
        printf("14. Retirer le meilleur joueur (priorité)\n");
        printf("15. Afficher la file de priorité\n");
        printf("16. Former un match équilibré (12 meilleurs)\n");
        printf("17. Former un match depuis une file\n");
        printf("18. Former un match depuis une file de priorité\n");

        printf("============================================\n");
        printf("Votre choix : ");

        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Nom du fichier : ");
                scanf("%255s", filename);

                n = 0;
                Player** loaded = loadPlayers(filename, &n);

                if (loaded == NULL && n != 0) {
                    printf("Erreur lors du chargement.\n");
                } else {
                    for (int i = 0; i < count; i++) {
                        if (players[i] != NULL) {
                            destroyPlayer(players[i]);
                            players[i] = NULL;
                        }
                    }

                    for (int i = 0; i < n; i++) {
                        players[i] = loaded[i];
                    }

                    count = n;
                    if (loaded != NULL) free(loaded);
                    printf("%d joueurs chargés.\n", count);
                }
                break;
            }

            case 2:
                printf("Nom du fichier : ");
                scanf("%255s", filename);
                savePlayers(filename, players, count);
                printf("Joueurs sauvegardés.\n");
                break;

            case 3:
                printf("Username : ");
                scanf("%99s", username);

                Player* p = createPlayer(username);
                if (p == NULL) {
                    printf("Erreur : impossible de créer.\n");
                } else if (!addPlayer(players, &count, p)) {
                    printf("Erreur : liste pleine.\n");
                    destroyPlayer(p);
                } else {
                    printf("Joueur ajouté.\n");
                }
                break;

            case 4:
                printf("Username à retirer : ");
                scanf("%99s", username);

                if (removePlayer(players, &count, username)) {
                    printf("Joueur retiré.\n");
                } else {
                    printf("Joueur introuvable.\n");
                }
                break;

            case 5: {
                printf("Username à rechercher : ");
                scanf("%99s", username);

                idx = searchPlayerByUsername(players, count, username);
                if (idx == -1) {
                    printf("Aucun joueur trouvé.\n");
                } else {
                    displayPlayerMini(players[idx]);
                    printf("\n");
                }
                break;
            }

            case 6:
                displayAllPlayers(players, count);
                break;

            case 7:
                sortPlayersBySpicyIndex(players, count);
                printf("Tri effectué.\n");
                break;

            case 8:
                printf("Au revoir !\n");
                break;

            case 9:
                printf("Username du joueur à mettre en file : ");
                scanf("%99s", username);

                // On cherche le joueur existant
                idx = searchPlayerByUsername(players, count, username);
                if (idx == -1) {
                    printf("Joueur introuvable.\n");
                } else if (players[idx]->inQueue) {
                    printf("Joueur déjà dans la file.\n");
                } else {
                    enqueue(queue, players[idx]);
                    printf("Joueur ajouté à la file.\n");
                }
                break;

            case 10: {
                tmpPlayer = dequeue(queue);
                if (tmpPlayer == NULL) {
                    printf("La file est vide.\n");
                } else {
                    printf("Joueur %s retiré de la file.\n", tmpPlayer->username);
                }
                break;
            }

            case 11: {
                t_cell *current = queue->head;
                if (current == NULL) {
                    printf("La file est vide.\n");
                } else {
                    printf("Joueurs dans la file :\n");
                    while (current != NULL) {
                        printf("- %s (Level: %d, SpicyIndex: %d)\n",
                               current->value->username,
                               current->value->level,
                               current->value->spicyIndex);
                        current = current->next;
                    }
                }
                break;
            }

            case 12:
                printf("\n======= Formation d'un Match =======\n");

                if (isQueueEmpty(queue)) {
                    printf("La file d'attente est vide. Impossible de former un match.\n");
                } else {
                    int count_player = 0;

                    while (count_player < 12 && !isQueueEmpty(queue)) {

                        Player* play = dequeue(queue);

                        if (play != NULL) {
                            printf("%d. ", count_player + 1);
                            displayPlayerMini(play);
                            count_player++;
                        }
                    }

                    printf("------------------------------------\n");
                    if (count_player < 12) {
                        printf("Match formé partiellement avec %d joueurs (File vide).\n", count_player);
                    } else {
                        printf("Match complet formé avec 12 joueurs !\n");
                    }
                }
                printf("====================================\n");
                break;

            case 13 :
                printf("Username du joueur à mettre en file de priorité : ");
                scanf("%99s", username);

                idx = searchPlayerByUsername(players, count, username);
                if (idx == -1) {
                    printf("Joueur introuvable.\n");
                } else if (players[idx]->inQueue) {
                    printf("Joueur déjà dans la file.\n");
                } else {
                    insertWithPriority(queue, players[idx]);
                    printf("Joueur ajouté à la file.\n");
                }
                break;

            case 14: {
                tmpPlayer = removeHighestPriority(queue);
                if (tmpPlayer == NULL) {
                    printf("La file est vide.\n");
                } else {
                    printf("Joueur %s retiré de la file.\n", tmpPlayer->username);
                }
                break;
            }

            case 15 :
                displayPriorityQueue(queue);
                break;

            case 16:
                formBalancedMatch(queue);
                break;

            case 17:
                launchMatch(queue);
                break;
            case 18:
                launchBalancedMatch(queue);
                break;
        
            default:
                printf("Choix invalide.\n");
                break;
        }
    } while (choice != 8);
    destroyQueue(queue);
    return 0;
}



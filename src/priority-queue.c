#include "../headers/priority-queue.h"
#include <stdio.h>
#include <stdlib.h>

t_priority_queue * createPriorityQueue() {
    return createQueue();
}

void insertWithPriority(t_priority_queue * queue, Player * player) {
    if (queue == NULL || player == NULL)
        return;

    //création de la nouvelle cellule
    t_cell *new_cell = malloc(sizeof(t_cell));
    if (new_cell == NULL)
        return;

    new_cell->value = player;
    new_cell->next = NULL;

    //file vide
    if (queue->head == NULL) {
        queue->head = new_cell;
        queue->tail = new_cell;
        player->inQueue = 1;
        return;
    }

    //insertion en tête
    if (player->spicyIndex > queue->head->value->spicyIndex) {
        new_cell->next = queue->head;
        queue->head = new_cell;
        player->inQueue = 1;
        return;
    }

    // CAS 3 & 4 : insertion au milieu ou à la fin
    t_cell *current = queue->head;

    //on avance tant qu'il y a un suivant et que le suivant est >= (pour passer après les égalités)
    while (current->next != NULL &&
           current->next->value->spicyIndex >= player->spicyIndex) {
        current = current->next;
    }

    //insertion après la current
    new_cell->next = current->next;
    current->next = new_cell;

    //si on insère à la fin il faut update tail
    if (new_cell->next == NULL) {
        queue->tail = new_cell;
    }

    //on marque le joueur comme présent dans la file
    player->inQueue = 1;
}

Player * removeHighestPriority(t_priority_queue * queue) {
    if (queue == NULL)
        return NULL;

    //Étant donné que la file de priorité est toujours triée, le joueur avec le spicyIndex le plus élevé est TOUJOURS en tête
    //Donc en gros cela revient à retirer le joueur prioritaire ==> retirer head
    return dequeue(queue);
}

void displayPriorityQueue(t_priority_queue *queue) {
    if (queue == NULL) {
        return;
    }

    printf("======= File de Priorité =======\n");

    int count = 0;
    t_cell *current = queue->head;

    while (current != NULL) {
        displayPlayerMini(current->value);
        current = current->next;
        count++;
    }

    printf("================================\n");
    printf("Total: %d joueurs\n", count);
}

void destroyPriorityQueue(t_priority_queue * queue) {
    destroyQueue(queue);
}

void formBalancedMatch(t_priority_queue* queue) {
    if (queue == NULL || queue->head == NULL) {
        printf("[Matchmaking] File vide.\n");
        return;
    }

    // On prend simplement les 12 premiers (les 12 meilleurs spicyIndex)
    Player *players[12];
    t_cell  *cells[12];

    int count = 0;
    t_cell *cur = queue->head;
    while (cur != NULL && count < 12) {
        cells[count] = cur;
        players[count] = cur->value;
        cur = cur->next;
        count++;
    }

    if (count < 12) {
        printf("[Matchmaking] Pas assez de joueurs pour former un match (12 requis, %d disponibles).\n", count);
        return;
    }

    int gap = players[0]->spicyIndex - players[11]->spicyIndex; // file triée décroissante => >= 0

    // Afficher les participants
    printf("\n======= Matchmaking (TOP 12) =======\n");
    printf("Ecart de niveau (max-min) : %d\n", gap);
    printf("Participants :\n");
    for (int i = 0; i < 12; i++) {
        displayPlayerMini(players[i]);
    }

    // Simuler le match (2 equipes de 6, repartition gloutonne pour equilibrer)
    Player *teamA[6];
    Player *teamB[6];
    int aCount = 0, bCount = 0;
    long sumA = 0, sumB = 0;

    for (int i = 0; i < 12; i++) {
        Player *p = players[i];

        // Assigner au plus faible total, en respectant 6 joueurs par equipe
        if (((sumA <= sumB) && (aCount < 6)) || (bCount >= 6)) {
            teamA[aCount++] = p;
            sumA += p->spicyIndex;
        } else {
            teamB[bCount++] = p;
            sumB += p->spicyIndex;
        }
    }

    printf("\n--- Equipe A (total spicyIndex = %ld) ---\n", sumA);
    for (int i = 0; i < 6; i++) {
        displayPlayerMini(teamA[i]);
    }

    printf("\n--- Equipe B (total spicyIndex = %ld) ---\n", sumB);
    for (int i = 0; i < 6; i++) {
        displayPlayerMini(teamB[i]);
    }

    if (sumA > sumB) {
        printf("\nResultat : Victoire de l'equipe A !\n");
    } else if (sumB > sumA) {
        printf("\nResultat : Victoire de l'equipe B !\n");
    } else {
        printf("\nResultat : Match nul !\n");
    }
    printf("====================================\n\n");

    // Retirer les 12 premiers de la file
    t_cell *after = cells[11]->next;
    queue->head = after;
    if (after == NULL) {
        // on a retire toute la liste
        queue->tail = NULL;
    }

    // Liberer les cellules retirees + marquer les joueurs comme sortis de la file
    for (int i = 0; i < 12; i++) {
        if (players[i] != NULL) {
            players[i]->inQueue = 0;
        }
        free(cells[i]);
    }
}

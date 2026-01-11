#include "../headers/list-utils.h"
#include <stdio.h>
#include <stdlib.h>
//
t_cell * createCell(Player * player) {
    t_cell *cell = malloc(sizeof(t_cell));
    if (cell == NULL) {
        return NULL;
    }

    cell->value = player; // Si player est NULL, cell->value sera NULL
    cell->next = NULL;

    return cell;
}

void destroyCell(t_cell * cell) {

    if (cell == NULL) {
        return;
    }

    free(cell);
}

t_queue * createQueue() {

    t_queue *queue = malloc(sizeof(t_queue));

    if (queue == NULL) {
        return NULL;
    }

    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

int isQueueEmpty(t_queue * queue) {

    if (queue == NULL) {
        return 1;
    }

    return queue->head == NULL;
}

void displayQueue(t_queue * queue) {

    if (isQueueEmpty(queue) == 1) {
        return;
    }

    int count = 0;

    printf("======= File d'attente =======\n");

    t_cell *current = queue->head;

    while (current != NULL) {
        count++;
        printf("%d. ", count);
        displayPlayerMini(current->value);
        printf("\n");
        current = current->next;
    }

    printf("Total: %d joueurs en attente", count);
    printf("=============================");
}
void enqueue(t_queue * queue, Player * player) {
    if (queue == NULL || player == NULL) {
        return;
    }

    // Création d’une nouvelle cellule
    t_cell *new_cell = malloc(sizeof(t_cell));
    if (new_cell == NULL) {
        return;
    }

    new_cell->value = player;
    new_cell->next  = NULL;// on a pas de cell suivante donc c'est la fin de notre queue

    //si file vide, new_cell sera na nouvelle cellule et on aura une queue de taille 1
    if (queue->head == NULL) {
        queue->head = new_cell;
        queue->tail = new_cell;
    }
    else {
        //on met la nouvelle cellule derrière l'ancienne
        queue->tail->next = new_cell;
        queue->tail = new_cell;
    }

    //et on fait la mise à jour du player
    player->inQueue = 1;
}

Player * dequeue(t_queue * queue) {
    if (queue == NULL || queue->head == NULL) {
        return NULL; //on a une file vide
    }

    //on récupère la cellule à retirer en utilisant old_head et queue ici c'est notre structure PAS LA FIN QUI EST TAIL
    t_cell *old_head = queue->head;
    Player *player = old_head->value;

    // On avance head
    queue->head = old_head->next;

    // Si la file devient vide après retrait
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    // Le joueur n'est plus dans la queue
    if (player != NULL) {
        player->inQueue = 0;
    }

    // On libère la cellule mais pas le joueur, en gros la cellule agit commme un ticket quand on attend une commande
    //on supprime le ticket, pas la personne 
    free(old_head);

    return player;
}

t_cell * removeHead(t_queue * queue) {
    if (queue == NULL || queue->head == NULL)
        return NULL;

    //on garde l'ancienne tête
    t_cell *old_head = queue->head;
    Player *player = old_head->value;

    //maj de head
    queue->head = old_head->next;

    //si on a enlevé la seule cellule, tail doit devenir NULL
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    //on marque le joueur comme hors file
    if (player != NULL) {
        player->inQueue = 0;
    }

    //pas besoin de free old_head
    return old_head;
}

void destroyQueue(t_queue* queue) {
    if (queue == NULL)
        return;

    t_cell *cell;

    //on retire chaque cellule ne par une en checkant si le pointeur est pas NULL donc liste vide
    while ((cell = removeHead(queue)) != NULL) {
        destroyCell(cell);
    }
    free(queue);
}
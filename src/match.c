#include <stdio.h>
#include "../headers/match.h"

Match * createMatch() {
    Match *match = malloc(sizeof(Match));
    if (match == NULL) {
        return NULL;
    }

    match->players = malloc(sizeof(Player*) * MATCH_MAX_PLAYERS);
    if (match->players == NULL) {
        free(match);
        return NULL;
    }

    match->results = NULL;
    match->numPlayers = 0;

    return match;
}

void destroyMatch(Match * match) {
    if (match == NULL) {
        return;
    }

    if (match->players != NULL) {
        free(match->players);
    }

    if (match->results != NULL) {
        free(match->results);
    }

    free(match);
}

void simulateMatch(Match * match) {
    if (match == NULL || match->numPlayers <= 0) {
        return;
    }

    if (match->results != NULL) {
        free(match->results);
        match->results = NULL;
    }

    match->results = malloc(sizeof(int) * match->numPlayers);
    if (match->results == NULL) {
        return;
    }

    for (int i = 0; i < match->numPlayers; i++) {
        match->results[i] = i;
    }

    for (int i = match->numPlayers - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = match->results[i];
        match->results[i] = match->results[j];
        match->results[j] = tmp;
    }
}

void displayMatchResult(const Match* match){
    if (match == NULL || match->players == NULL || match->results == NULL) {
        printf("Erreur : match ou résultats inexistants.\n");
        return;
    }

    printf("========================================\n");
    printf("           MATCH RESULTS\n");
    printf("========================================\n");

    for (int i = 0; i < match->numPlayers; i++) {
        printf("%d. ", i + 1);

        displayPlayerMini(match->players[i]);

        // Affiche le gain/perte de spicyIndex
        int delta = match->results[i];
        if (delta >= 0) {
            printf("  (+%d)", delta);
        } else {
            printf("  (%d)", delta);
        }

        printf("\n");
    }

    printf("========================================\n");

}

void addMatchPlayers(t_queue * queue, Match * match) {
    if (queue == NULL || match == NULL || match->players == NULL) {
        return;
    }

    while (!isQueueEmpty(queue) && match->numPlayers < MATCH_MAX_PLAYERS) {
        Player *player = dequeue(queue);
        if (player == NULL) {
            return;
        }

        match->players[match->numPlayers] = player;
        match->numPlayers++;

        // Le joueur n'est plus dans la file
        player->inQueue = 0;
    }
}

void displayMatchInfo(const Match * match) {
    if (match == NULL) {
        fprintf(stderr, "Error: displayMatchInfo() - match is NULL.\n");
        return;
    }

    printf("========================================\n");
    printf("           MATCH INFORMATION\n");
    printf("========================================\n");
    printf("Participants: %d\n", match->numPlayers);
    printf("----------------------------------------\n");

    for (int i = 0; i < match->numPlayers; i++) {
        printf("%d. ", i + 1);
        if (match->players != NULL && match->players[i] != NULL) {
            displayPlayerMini(match->players[i]);
        } else {
            printf("[NULL PLAYER]");
        }
        printf("\n");
    }

    printf("========================================\n");
}


void updatePlayerStats(Match * match) {
    if (match == NULL || match->players == NULL || match->results == NULL) {
        return;
    }

    int n = match->numPlayers;
    if (n <= 1) {
        return;
    }

    int baseGain = 100 / (n - 1);

    for (int r = 0; r < n; r++) {
        int playerIndex = match->results[r];
        Player *p = match->players[playerIndex];

        if (p == NULL) {
            continue;
        }

        // Nombre de parties
        p->numGames++;

        // Victoire / défaite
        if (r == 0) {
            p->numWins++;
        } else {
            p->numLosses++;
        }

        // Calcul du gain de spicyIndex
        int gain = (n - r - 1) * baseGain;
        p->spicyIndex += gain;
    }
}

void launchMatch(t_queue * queue){
    if (queue == NULL) {
        printf("Erreur : file d'attente inexistante.\n");
        return;
    }

    // 1. Création du match
    Match* match = createMatch();
    if (match == NULL) {
        printf("Erreur : impossible de créer le match.\n");
        return;
    }

    // 2. Ajout des joueurs depuis la file
    addMatchPlayers(queue, match);
        if (match->numPlayers < MATCH_MAX_PLAYERS) {
        printf("Erreur : pas assez de joueurs pour lancer le match.\n");
        destroyMatch(match);
        return;
    }

    //3 - affichage des informations du match
    displayMatchInfo(match);

    //4 - simulation du match
    simulateMatch(match);

    // 5. mise à jour des statistiques des joueurs
    updatePlayerStats(match);

    //6 - on affiche les résultats du match
    displayMatchResult(match);

    //7 - on libère la mémoire
    destroyMatch(match);
}

void launchBalancedMatch(t_priority_queue * p_queue) {
    if (p_queue == NULL) {
        printf("Erreur : file de priorité inexistante.\n");
        return;
    }

    //1- oncréer un match
    Match* match = createMatch();
    if (match == NULL) {
        printf("Erreur : impossible de créer le match.\n");
        return;
    }

    //2 - extraction des joueurs depuis la file de priorité
    while (match->numPlayers < MATCH_MAX_PLAYERS && p_queue->head != NULL) {
        Player *player = removeHighestPriority(p_queue);
        if (player == NULL)
            break;

        match->players[match->numPlayers++] = player;
        player->inQueue = 0;
    }

    if (match->numPlayers == 0) {
        printf("Erreur : pas assez de joueurs pour former un match équilibré.\n");
        destroyMatch(match);
        return;
    }

    //4 - affichage info match
    displayMatchInfo(match);

    //5 - simulation du match
    simulateMatch(match);

    //6 - mise à jour des statistiques
    updatePlayerStats(match);

    //7 - affichage des résultats
    displayMatchResult(match);

    //8 - libération mémoire
    destroyMatch(match);
}

void simulateTeamMatch(Match* match) {
    if (match == NULL || match->players == NULL || match->results == NULL) {
        printf("Erreur : match invalide.\n");
        return;
    }

    if (match->numPlayers != MATCH_MAX_PLAYERS) {
        printf("Erreur : nombre de joueurs incorrect pour un match par équipes.\n");
        return;
    }

    //1 - calcul de la puissance des équipes
    int teamA_power = 0;
    int teamB_power = 0;

    for (int i = 0; i < 6; i++) {
        teamA_power += match->players[i]->spicyIndex;
    }

    for (int i = 6; i < 12; i++) {
        teamB_power += match->players[i]->spicyIndex;
    }

    //2 - détermination de l'équipe gagnante
    int winningTeam;
    if (teamA_power > teamB_power) {
        winningTeam = 0;
    } else if (teamB_power > teamA_power) {
        winningTeam = 1;
    } else {
        // égalité → choix aléatoire
        winningTeam = rand() % 2;
    }

    //3 - distribution des gains et pertes
    for (int i = 0; i < match->numPlayers; i++) {
        if ((i < 6 && winningTeam == 0) ||
            (i >= 6 && winningTeam == 1)) {
            match->results[i] = +50;
        } else {
            match->results[i] = -25;
        }
    }

    //4 - affichage des résultats
    printf("========================================\n");
    printf("          TEAM MATCH RESULT\n");
    printf("========================================\n");

    printf("Equipe A (Puissance: %d)%s\n", teamA_power,
           winningTeam == 0 ? "  [VICTOIRE]" : "");
    for (int i = 0; i < 6; i++) {
        displayPlayerMini(match->players[i]);
        printf("\n");
    }

    printf("\nEquipe B (Puissance: %d)%s\n", teamB_power,
           winningTeam == 1 ? "  [VICTOIRE]" : "");
    for (int i = 6; i < 12; i++) {
        displayPlayerMini(match->players[i]);
        printf("\n");
    }

    printf("========================================\n");
}

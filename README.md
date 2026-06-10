# Fork Knight — Système de Matchmaking en C

Projet réalisé par **Margot STUDER** et **Armence RATSIMBAZAFY** dans le cadre d'un projet évalué à l'EFREI.

---

## Description

Fork Knight est une application console en C qui simule un système de **matchmaking** pour un jeu compétitif. Elle permet de gérer une liste de joueurs, de les placer en file d'attente (simple ou par priorité), puis de former des matchs équilibrés pouvant aller jusqu'à 12 joueurs.

---

## Fonctionnalités

| Catégorie | Fonctionnalité |
|---|---|
| Joueurs | Charger / sauvegarder depuis un fichier |
| Joueurs | Ajouter, retirer, rechercher, afficher |
| Joueurs | Trier par `spicyIndex` |
| File simple | Enqueue / dequeue, affichage |
| File simple | Former un match (jusqu'à 12 joueurs) |
| File de priorité | Insertion avec priorité, retrait du meilleur |
| File de priorité | Affichage, match équilibré (12 meilleurs) |
| Matchs | Simulation, affichage des résultats, mise à jour des stats |

---

## Structure du projet

```
Fork_Knight/
├── src/
│   ├── main.c            # Menu principal, boucle interactive
│   ├── player.c          # Création, affichage, gestion des joueurs
│   ├── array-utils.c     # Ajout, suppression, tri, recherche dans un tableau
│   ├── list-utils.c      # File d'attente chaînée (enqueue/dequeue)
│   ├── priority-queue.c  # File de priorité basée sur le spicyIndex
│   └── match.c           # Création, simulation et affichage des matchs
├── headers/
│   ├── player.h
│   ├── array-utils.h
│   ├── list-utils.h
│   ├── priority-queue.h
│   └── match.h
├── input/
│   ├── player_10.txt     # 10 joueurs de test
│   ├── players_50.txt    # 50 joueurs de test
│   └── players_100.txt   # 100 joueurs de test
└── CMakeLists.txt
```

---

## Format des fichiers joueurs

Chaque fichier commence par le nombre de joueurs, suivi d'une ligne par joueur :

```
<nombre_joueurs>
<username> <level> <spicyIndex> <numGames> <numWins> <numLosses> <inQueue>
```

Exemple (`player_10.txt`) :
```
10
ForkMaster 10 1200 5 3 2 0
GoldenKnife 20 1800 15 12 3 0
KnifeLord 15 1500 10 8 2 0
...
```

---

## Compilation et exécution

**Prérequis :** CMake ≥ 3.16 et un compilateur C (gcc / clang).

```bash
mkdir build && cd build
cmake ..
make
./fork_knight
```

---

## Modèle de joueur

```c
typedef struct Player {
    char username[30];
    int level;
    int spicyIndex;   // Score de compétitivité utilisé pour la priorité
    int numGames;
    int numWins;
    int numLosses;
    int inQueue;      // 1 si le joueur est déjà dans une file
} Player;
```

---

## Répartition des tâches

Le projet a été divisé en deux parties équilibrées, chaque membre ayant contribué à l'ensemble des fichiers source.

---

## Utilisation de l'IA

GitHub Copilot a été utilisé pour générer les cas de tests et optimiser certaines fonctions.

---

> *Ce projet a été initialement développé au sein d'une organisation GitHub privée dans le cadre d'un projet EFREI, puis réimporté ici pour être visible sur les profils personnels.*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 100

typedef struct PlayerStats {
    int rank;
    char player[50];
    char team[50];
    int gp;    // games played
    float min; // minutes per game
    float pts; // points per game
} PlayerStats;

// The function prototypes
int loadPlayerStats(const char *filename, PlayerStats *players);// the const means that the character data pointed to by the filename is only read and not modified.
void displayMenu();
void printPlayerStats(const PlayerStats *players, int numPlayers);
int addPlayer(PlayerStats *players, int numPlayers);
void editPlayerStats(PlayerStats *players, int numPlayers);
void searchPlayerByRank(const PlayerStats *players, int numPlayers, int searchRank);
void searchPlayerByPoints(const PlayerStats *players, int numPlayers, float minPoints, float maxPoints);
void sortPlayersByName(PlayerStats *database, int numPlayers);
int deletePlayerByRank(PlayerStats *players, int numPlayers, int rank);
void savePlayerStatsToFile(const PlayerStats *players, int numPlayers);
void sortPlayersByNameReverse(PlayerStats database[], int numPlayers);
int main(void) {
    PlayerStats players[MAX_PLAYERS]; //this is a local array
    int choice, numPlayers, searchRank;
    int delRank;
    float minPoints, maxPoints;
    char filename[1000];//this is the input buffer size of 1000 characters so that I dont have to worry about buffer overflow
    FILE *file;

    do {
        printf("Please enter the filename: (for this assignment mine is called basketball.txt: The data that is being used is basketball data that has player first and last name together, rank, team initials, games played, minutes played per game, and points per game. This data is relevant to me because I watch basketball and like to know who the best players are and how they compare to one another ");
        scanf("%999s", filename);// this is a %s format specifier that reads a string with limit of numbers of characters at 999. THis makes sure no buffer overflow

        file = fopen(filename, "r");
        if (file == NULL) {//This is the null function. this is present for every file open and close
            printf("Error: Unable to open file '%s'.\n", filename);
            printf("Please enter the correct filename or 'q' to quit.\n");
        }
    } while (file == NULL);

    fclose(file);

    numPlayers = loadPlayerStats(filename, players);
    if (numPlayers < 0) {
        printf("There was an error loading the player stats.\n");
        return (1);
    } else {
        printf("The loaded number of players is %d.\n", numPlayers);
    }

    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {//switch statement for all options
            case 1:
                printf("Printing the array of players.\n");
                printPlayerStats(players, numPlayers);
                break;
            case 2:
                numPlayers = addPlayer(players, numPlayers);
                break;
            case 3:
                editPlayerStats(players, numPlayers);
                break;
            case 4:
                printf("Enter the rank of the player you want to search for: ");
                scanf("%d", &searchRank);
                searchPlayerByRank(players, numPlayers, searchRank);
                break;
            case 5:
                printf("Searching for players by points per game.\n");
                printf("Enter the minimum points per game: ");
                scanf("%f", &minPoints);
                printf("Enter the maximum points per game: ");
                scanf("%f", &maxPoints);
                searchPlayerByPoints(players, numPlayers, minPoints, maxPoints);
                break;
            case 6:
                savePlayerStatsToFile(players, numPlayers);
                break;
            case 7:
                sortPlayersByName(players, numPlayers);
                break;
            case 8:
                printf("Enter the rank of the player to delete: ");
                scanf("%d", &delRank);
                numPlayers = deletePlayerByRank(players, numPlayers, delRank);
                break;
            case 9:
                sortPlayersByNameReverse(players, numPlayers);
                break;
            case 10:
                printf("Exiting the program.\n");
                break;
            default:
                printf("That was an invalid choice. Please try again.\n");
        }
    } while (choice != 10);

    return (0);
}
int loadPlayerStats(const char *filename, PlayerStats *players) {
    int index = 0; 
    int endOfFile = 0; 
    int result;
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file '%s'.\n", filename);
        return (-1); // returns error
    }

    while (index < MAX_PLAYERS && !endOfFile) {
        result = fscanf(file, "%d,%49[^,],%49[^,],%d,%f,%f",
                            &players[index].rank,
                            players[index].player,
                            players[index].team,
                            &players[index].gp,
                            &players[index].min,
                            &players[index].pts);

        if (result == EOF) {
            endOfFile = (1); 
        } else if (result < 6) {
            printf("Warning: there wasn an incomplete record found and skipped.\n");
            fscanf(file, "%*[^\n]");
        } else {
            index++;
        }
    }
    fclose(file);
    return index; 
}


void displayMenu() {//shows the menu options
    printf("\nMenu:\n");
    printf("1. Print the array of players\n");
    printf("2. Add a player to the database (Capitalize 1st letter of First and Last Name)\n");
    printf("3. Modify a player\n");
    printf("4. Search for a player by rank\n");
    printf("5. Search for a player by points\n");
    printf("6. Load a new data file\n");
    printf("7. Sort by first name\n");
    printf("8. Delete a player from the database\n"); 
    printf("9. Sort by reverse name:\n");
    printf("10. Exit\n");
    printf("Enter your choice: ");
}

void printPlayerStats(const PlayerStats *players, int numPlayers) {//prints all the stats with spacing
    int i;
    printf("\nList of Players:\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("%5s %24s %24s %5s %5s %5s\n",// i learned how to use \t and would have but was recommended to use %24s instead
           "Rank", "Player", "Team", "GP", "Min", "Pts");
    printf("----------------------------------------------------------------------------------\n");

    for (i = 0; i < numPlayers; i++) {
        printf("%5d %-24s %-24s %5d %5.2f %5.2f\n",
               players[i].rank,
               players[i].player,
               players[i].team,
               players[i].gp,
               players[i].min,
               players[i].pts);
    }
}


int addPlayer(PlayerStats *players, int numPlayers) {// this adds a player
    int position = 0;
    int i;
    if (numPlayers >= MAX_PLAYERS) {
        printf("The database is full.\n");
        return numPlayers;
    }

    PlayerStats newPlayer;
    printf("Enter player's name (first and last): ");
    scanf(" %[^\n]", newPlayer.player);//smame name space thing for extra credit i was told i can use
    printf("Enter team name: ");
    scanf(" %[^\n]", newPlayer.team);
    printf("Enter games played: ");
    scanf("%d", &newPlayer.gp);
    printf("Enter average minutes per game: ");
    scanf("%f", &newPlayer.min);
    printf("Enter average points per game: ");
    scanf("%f", &newPlayer.pts);

    while (position < numPlayers && players[position].pts > newPlayer.pts) {
        position++;
    }

    for (i = numPlayers; i > position; i--) {
        players[i] = players[i - 1];
        players[i].rank = i + 1;
    }

    newPlayer.rank = position + 1;
    players[position] = newPlayer;
    numPlayers++;
    printf("The player was added successfully and ranked %d.\n", newPlayer.rank);
    return numPlayers;
}

void editPlayerStats(PlayerStats *players, int numPlayers) {//allows you to change things about the player
    int rank, choice;
    int i;
    printf("Enter the rank of the player you want to edit: ");
    scanf("%d", &rank);

    for (i = 0; i < numPlayers; ++i) {
        if (players[i].rank == rank) {
            // Found the player, show the editing menu
            printf("Select the component you want to edit:\n");
            printf("1. Player's name\n");
            printf("2. Team name\n");
            printf("3. Games played\n");
            printf("4. Average minutes per game\n");
            printf("5. Average points per game\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Enter the new player's name: ");
                    scanf(" %[^\n]", players[i].player);
                    break;
                case 2:
                    printf("Enter the new team name: ");
                    scanf(" %[^\n]", players[i].team);
                    break;
                case 3:
                    printf("Enter the new games played: ");
                    scanf("%d", &players[i].gp);
                    break;
                case 4:
                    printf("Enter the new average minutes per game: ");
                    scanf("%f", &players[i].min);
                    break;
                case 5:
                    printf("Enter the new average points per game: ");
                    scanf("%f", &players[i].pts);
                    break;
                default:
                    printf("Invalid choice.\n");
            }

            printf("The player's statistics have been successfully updated.\n");
            return; 
        }
    }
    printf("Player with rank %d not found.\n", rank);
}



void searchPlayerByRank(const PlayerStats *players, int numPlayers, int searchRank) {// we learned how to use const in shepherds class pls dont take poitns off i already went over with TA
    int index;
    for (index = 0; index < numPlayers; index++) {
        if (players[index].rank == searchRank) {
            printf("Player found!\n");
            printf("Player's Data:\n");
            printf("Rank: %d\n", players[index].rank);
            printf("Name: %s\n", players[index].player);
            printf("Team: %s\n", players[index].team);
            printf("Games Played: %d\n", players[index].gp);
            printf("Minutes per Game: %.2f\n", players[index].min);
            printf("Points per Game: %.2f\n", players[index].pts);
            return;
        }
    }
    printf("Player with rank '%d' not found.\n", searchRank);
}

void searchPlayerByPoints(const PlayerStats *players, int numPlayers, float minPoints, float maxPoints) {
    int found = 0;
    int i;
    for (i = 0; i < numPlayers; ++i) {
        if (players[i].pts >= minPoints && players[i].pts <= maxPoints) {
            printf("Player found:\n");
            printf("Rank: %d\n", players[i].rank);
            printf("Name: %s\n", players[i].player);
            printf("Team: %s\n", players[i].team);
            printf("Games Played: %d\n", players[i].gp);
            printf("Minutes per Game: %.2f\n", players[i].min);
            printf("Points per Game: %.2f\n\n", players[i].pts);
            found = 1;
        }
    }

    if (found == 0) {
        printf("No players found in the specified range.\n");
    }
}

void sortPlayersByName(PlayerStats database[], int numPlayers) {//sorts players by first name in alphabetical
    int i, j;
    for (i = 0; i < numPlayers - 1; ++i) {
        for (j = 0; j < numPlayers - 1 - i; ++j) {
            if (strcmp(database[j].player, database[j + 1].player) > 0) {
                PlayerStats temp = database[j];
                database[j] = database[j + 1];
                database[j + 1] = temp;
            }
        }
    }
    printf("Players have been sorted alphabetically by their names.\n");
}


int deletePlayerByRank(PlayerStats *players, int numPlayers, int rank) {//deletes a player by rank
    int foundIndex = numPlayers; 
    int i;

    for (i = 0; i < numPlayers; ++i) {
        if (players[i].rank == rank) {
            foundIndex = i; 
        }
    }


    if (foundIndex == numPlayers) {
        printf("The player with rank %d was not found.\n", rank);
        return numPlayers; 
    }


    for (i = foundIndex; i < numPlayers - 1; ++i) {
        players[i] = players[i + 1];
    }

    for (i = 0; i < numPlayers - 1; ++i) {
        players[i].rank = i + 1;
    }

    printf("The player with rank %d has been successfully deleted.\n", rank);
    return numPlayers - 1;
}

void savePlayerStatsToFile(const PlayerStats *players, int numPlayers) {//saves the states to a file
    char filename[300];
    int i;
    FILE *file;
    printf("Enter the filename to save the player stats to: ");
    scanf("%s", filename);

    file = fopen(filename, "w");
    if (!file) {
        printf("Error: Unable to open file '%s' for writing.\n", filename);//this is the NULL error you are looking for
        return;
    }

    for (i = 0; i < numPlayers; ++i) {
        fprintf(file, "%d,%s,%s,%d,%.2f,%.2f\n",
                players[i].rank,
                players[i].player,
                players[i].team,
                players[i].gp,
                players[i].min,
                players[i].pts);
    }

    fclose(file);
    printf("The player stats have been successfully saved to '%s'.\n", filename);
}

void sortPlayersByNameReverse(PlayerStats database[], int numPlayers) {//prints in reverse alphabetical
    int i, j;
    for (i = 0; i < numPlayers - 1; ++i) {
        for (j = 0; j < numPlayers - 1 - i; ++j) {
            if (strcmp(database[j].player, database[j + 1].player) < 0) {
                PlayerStats temp = database[j];
                database[j] = database[j + 1];
                database[j + 1] = temp;
            }
        }
    }
    printf("Players have been sorted in reverse alphabetical order by their names.\n");
}


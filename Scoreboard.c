#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Team 
{
    char name[30];
    int score;
    int fouls;
    struct Team *next; 
} Team;

void displayScoreboard(Team *teams, const char *timer) 
{
    printf("\n--- Basketball Scoreboard ---\n");
    printf("Time: %s\n", timer);  

    Team *current = teams;
    while (current != NULL) {
        printf("%s: %d (Fouls: %d)\n", current->name, current->score, current->fouls);
        current = current->next;
    }
    printf("-----------------------------\n");
}

void resetScores(Team *teams) 
{
    Team *current = teams;
    while (current != NULL) 
    {
        current->score = 0;
        current->fouls = 0;
        current = current->next;
    }
}

void readTimer(char *buffer, size_t size) 
{
    FILE *file = fopen("timer.txt", "r");
    if (file == NULL) 
    {
        perror("Unable to open timer file");
        return;
    }
    fgets(buffer, size, file);
    fclose(file);
}

void handleFouls(Team *team) 
{
    team->fouls++;
    printf("%s committed a foul! Total fouls: %d\n", team->name, team->fouls);
}

void takeFreeThrows(Team *team) 
{
    int freeThrowsMade = 0;
    char choice;

    printf("%s gets 2 free throws due to 6 fouls committed by the opposing team.\n", team->name);
    for (int i = 0; i < 2; i++) {
        printf("Attempt %d: Make free throw? (y/n): ", i + 1);
        choice = getchar();
        getchar(); 

        if (choice == 'y' || choice == 'Y') {
            freeThrowsMade++;
            team->score++;
            printf("Free throw made!\n");
        } else {
            printf("Free throw missed.\n");
        }
    }
    printf("%s made %d free throws.\n", team->name, freeThrowsMade);
}


Team* createTeam(const char* name) 
{
    Team *newTeam = (Team*)malloc(sizeof(Team));
    if (newTeam == NULL) 
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strncpy(newTeam->name, name, sizeof(newTeam->name));
    newTeam->score = 0;
    newTeam->fouls = 0;
    newTeam->next = NULL;
    return newTeam;
}


void addTeam(Team **teams, const char* name) 
{
    Team *newTeam = createTeam(name);
    if (*teams == NULL) 
    {
        *teams = newTeam;
    } 
    else 
    {
        Team *current = *teams;
        while (current->next != NULL) 
        {
            current = current->next;
        }
        current->next = newTeam;
    }
}

int main() 
{
    Team *teams = NULL;  
    char choice;
    char timerBuffer[6]; 

    
    char team1Name[30], team2Name[30];
    printf("Enter name for Team 1: ");
    fgets(team1Name, sizeof(team1Name), stdin);
    team1Name[strcspn(team1Name, "\n")] = 0; 

    printf("Enter name for Team 2: ");
    fgets(team2Name, sizeof(team2Name), stdin);
    team2Name[strcspn(team2Name, "\n")] = 0;

    
    addTeam(&teams, team1Name);
    addTeam(&teams, team2Name);

    while (1) 
    {
        readTimer(timerBuffer, sizeof(timerBuffer));

        displayScoreboard(teams, timerBuffer);

        printf("Choose an option:\n");
        printf("1. Add score to Team 1\n");
        printf("2. Add score to Team 2\n");
        printf("3. Team 1 commits fouls\n");
        printf("4. Team 2 commits fouls\n");
        printf("5. Reset scores\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        choice = getchar();
        getchar(); 
        Team *current = teams;
        switch (choice) 
        {
            case '1':
                current->score++;
                current->score++;
                break;
            case '2':
                current = current->next; 
                current->score++;
                current->score++;
                break;
            case '3':
                handleFouls(teams); 
                if (teams->fouls >= 6) 
                {
                    current = teams->next; 
                    takeFreeThrows(current);
                    teams->fouls = 0;  
                }
                break;
            case '4':
                handleFouls(teams->next); 
                if (teams->next->fouls >= 6) 
                {
                    takeFreeThrows(teams); 
                    teams->next->fouls = 0;  
                }
                break;
            case '5':
                resetScores(teams);
                break;
            case '6':
                readTimer(timerBuffer, sizeof(timerBuffer));
                printf("Final Time: %s\n", timerBuffer);

               
                current = teams;
                while (current != NULL) 
                {
                    printf("Final Score - %s: %d\n", current->name, current->score);
                    current = current->next;
                }

                printf("Exiting scoreboard system.\n");
                
                while (teams != NULL) 
                {
                    Team *temp = teams;
                    teams = teams->next;
                    free(temp);
                }
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }

       
    }

    return 0;
}

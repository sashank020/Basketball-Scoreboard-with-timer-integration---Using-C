#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep()

typedef struct {
    char name[30];
    int score;
} Team;

void displayScoreboard(Team team1, Team team2, const char *timer) {
    printf("\n--- Basketball Scoreboard ---\n");
    printf("Time: %s\n", timer);  // Display current time from timer
    printf("%s: %d\n", team1.name, team1.score);
    printf("%s: %d\n", team2.name, team2.score);
    printf("-----------------------------\n");
}

void resetScores(Team *team1, Team *team2) {
    team1->score = 0;
    team2->score = 0;
}

void readTimer(char *buffer, size_t size) {
    FILE *file = fopen("timer.txt", "r");
    if (file == NULL) {
        perror("Unable to open timer file");
        return;
    }
    fgets(buffer, size, file);
    fclose(file);
}

int main() {
    Team team1, team2;
    char choice;
    char timerBuffer[6]; // Buffer to hold the timer string (MM:SS)

    // Initialize teams
    printf("Enter name for Team 1: ");
    fgets(team1.name, sizeof(team1.name), stdin);
    team1.name[strcspn(team1.name, "\n")] = 0; // Remove newline character
    team1.score = 0;

    printf("Enter name for Team 2: ");
    fgets(team2.name, sizeof(team2.name), stdin);
    team2.name[strcspn(team2.name, "\n")] = 0; // Remove newline character
    team2.score = 0;

    while (1) {
        // Periodically read the timer and display the scoreboard
        readTimer(timerBuffer, sizeof(timerBuffer));

        displayScoreboard(team1, team2, timerBuffer);

        printf("Choose an option:\n");
        printf("1. Add score to Team 1\n");
        printf("2. Add score to Team 2\n");
        printf("3. Reset scores\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        choice = getchar();
        getchar(); // Consume newline character

        switch (choice) {
            case '1':
                team1.score++;
                break;
            case '2':
                team2.score++;
                break;
            case '3':
                resetScores(&team1, &team2);
                break;
            case '4':
                readTimer(timerBuffer, sizeof(timerBuffer));
                printf("Final Time: %s\n", timerBuffer);
                printf("Final Score - %s: %d, %s: %d\n", team1.name, team1.score, team2.name, team2.score);
                printf("Exiting scoreboard system.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }

        sleep(1);  // Sleep for a second before displaying the next menu
    }

    return 0;
}

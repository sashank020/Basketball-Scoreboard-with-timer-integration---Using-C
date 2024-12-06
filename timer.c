#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *file;
    int minutes = 0, seconds = 0;

    while (1) {
        sleep(1); 
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
        }

        
        file = fopen("timer.txt", "w");
        if (file == NULL) {
            perror("Unable to open timer file");
            exit(1);
        }
        fprintf(file, "%02d:%02d\n", minutes, seconds);
        fclose(file);
    }

    return 0;
}

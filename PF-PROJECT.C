#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_TRIES 3
#define MAX_LEVEL 5

typedef struct {
    char *word;
    int score;
} Game;

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int isPalindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0;
        }
    }
    return 1;
}

void scrambleWord(char *word, char *scrambled) {
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        scrambled[i] = word[i];
    }
    for (int i = 0; i < len; i++) {
        int j = rand() % len;
        char temp = scrambled[i];
        scrambled[i] = scrambled[j];
        scrambled[j] = temp;
    }
    scrambled[len] = '\0';
}

void playScrambledPalindrome(Game *game) {
    FILE *file = fopen("scrambled_palindromes.txt", "r");
    if (!file) {
        printf("Could not open file.\n");
        return;
    }

    char word[MAX_WORD_LENGTH];
    char scrambled[MAX_WORD_LENGTH];
    int level = 1;
    int tries = 0;

    while (level <= MAX_LEVEL) {
        fseek(file, 0, SEEK_SET);
        int randomIndex = rand() % 10; // Assuming there are 10 words in the file
        for (int i = 0; i <= randomIndex; i++) {
            fgets(word, MAX_WORD_LENGTH, file);
        }
        word[strcspn(word, "\n")] = 0; // Remove newline character
        scrambleWord(word, scrambled);
        
        printf("Level %d: Unscramble this palindrome: %s\n", level, scrambled);
        char userInput[MAX_WORD_LENGTH];
        printf("Your guess: ");
        fgets(userInput, MAX_WORD_LENGTH, stdin);
        userInput[strcspn(userInput, "\n")] = 0; // Remove newline character
        toLowerCase(userInput);

        if (strcmp(userInput, word) == 0) {
            printf("Correct!\n");
            game->score += 10 * level;
            level++;
            tries = 0;
        } else {
            printf("Wrong! Try again.\n");
            tries++;
            if (tries >= MAX_TRIES) {
                printf("Game Over! Your score: %d\n", game->score);
                fclose(file);
                return;
            }
        }
    }
    printf("Congratulations! You've completed all levels. Final score: %d\n", game->score);
    fclose(file);
}

void playUniquePalindrome(Game *game) {
    int level = 1;
    int tries = 0;

    while (level <= MAX_LEVEL) {
        char userInput[MAX_WORD_LENGTH];
        printf("Level %d: Enter a unique palindrome: ", level);
        fgets(userInput, MAX_WORD_LENGTH, stdin);
        userInput[strcspn(userInput, "\n")] = 0; // Remove newline character
        toLowerCase(userInput);

        if (isPalindrome(userInput)) {
            printf("Correct!\n");
            game->score += 10 * level;
            level++;
            tries = 0;
        } else {
            printf("Not a palindrome! Try again.\n");
            tries++;
            if (tries >= MAX_TRIES) {
                printf("Game Over! Your score: %d\n", game->score);
                return;
            }
        }
    }
    printf("Congratulations! You've completed all levels. Final score: %d\n", game->score);
}

void displayMenu() {
    printf("Welcome to the Palindrome Game!\n");
    printf("1. Scrambled Palindrome Game\n");
    printf("2. Unique Palindrome Game\n");
    printf("3. Exit\n");
}

int main() {
    srand(time(NULL));
    Game *game = (Game *)malloc(sizeof(Game));
    game->score = 0;

    int choice;
    do {
        displayMenu();
        printf("Select a game: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1:
                playScrambledPalindrome(game);
                break;
            case 2:
                playUniquePalindrome(game);
                break;
            case 3:
                printf("Exiting the game. Thank you for playing!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    free(game);
    return 0;
}

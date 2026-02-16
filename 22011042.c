#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CLEAR_SCREEN() printf("\033[H\033[J") 

void displayGrid(int rows, int cols, int **grid) {
	int i,j;
    for (i = 0; i < rows; i++) {
        printf("|");
        for (j = 0; j < cols; j++) {
            if (grid[i][j] == 1)
                printf("* ");
            else
                printf("  ");
        }
        printf("|\n");
    }
}

int main() {
    int rows, cols;
    int highScore = 0;
    int keepPlaying = 1;

    printf("=== WELCOME TO C-TETRIS ===\n");
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    printf("Enter number of columns: ");
    scanf("%d", &cols);

    srand(time(NULL));

    while (keepPlaying) {
        
        int **matrix = (int **)malloc((rows + 2) * sizeof(int *));
        int i;
        for (i = 0; i < rows + 2; i++) {
            matrix[i] = (int *)calloc((cols + 2), sizeof(int));
        }

        int gameOver = 0;
        int score = 0;

        while (!gameOver) {
            CLEAR_SCREEN();
            displayGrid(rows, cols, matrix);

            int shape[3][3] = {0};
            int randomShape = (rand() % 6) + 1;

            
            switch (randomShape) {
                case 1: 
                    shape[2][0] = 1; shape[2][1] = 1; shape[2][2] = 1;
                    break;
                case 2: 
                    shape[1][0] = 1; shape[1][1] = 1; shape[2][0] = 1; shape[2][1] = 1;
                    break;
                case 3: 
                    shape[1][0] = 1; shape[2][0] = 1; shape[2][1] = 1;
                    break;
                case 4: 
                    shape[2][0] = 1;
                    break;
                case 5: 
                    shape[0][0] = 1; shape[1][0] = 1; shape[2][0] = 1; shape[1][1] = 1;
                    break;
                case 6: 
                    shape[1][1] = 1; shape[1][2] = 1; shape[2][0] = 1; shape[2][1] = 1;
                    break;
            }

            printf("\nCurrent Shape:\n");
            int i,j,r,c;
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    shape[i][j] ? printf("%d ", shape[i][j]) : printf("  ");
                }
                printf("\n");
            }

            printf("Score: %d\n", score);
            int columnChoice;
            printf("Choose column to drop (0-%d): ", cols - 3);
            scanf("%d", &columnChoice);

           
            int targetRow = 2;
            int isSpaceFree = 1;
            while (targetRow < rows && isSpaceFree) {
                for (r = targetRow; r > targetRow - 3; r--) {
                    for (c = columnChoice; c < columnChoice + 3; c++) {
                        if (r >= 0 && matrix[r][c] + shape[r - (targetRow - 2)][c - columnChoice] > 1) {
                            isSpaceFree = 0;
                        }
                    }
                }
                if (isSpaceFree) targetRow++;
            }

            int finalRow = isSpaceFree ? targetRow - 1 : targetRow - 2;

            
            for (i = finalRow; i > finalRow - 3; i--) {
                for (j = columnChoice; j < columnChoice + 3; j++) {
                    if (i >= 0) matrix[i][j] += shape[i - (finalRow - 2)][j - columnChoice];
                }
            }

            
            for (i = finalRow; i > finalRow - 3; i--) {
                if (i < 0) continue;
                int isRowFull = 1;
                for (j = 0; j < cols; j++) {
                    if (matrix[i][j] == 0) {
                        isRowFull = 0;
                        break;
                    }
                }
                if (isRowFull) {
                    for (r = i; r > 0; r--) {
                        for ( c = 0; c < cols; c++) {
                            matrix[r][c] = matrix[r - 1][c];
                        }
                    }
                    score += 20;
                    i++; 
                }
            }

            score += 10;

            
            for (j = 0; j < cols; j++) {
                if (matrix[0][j] == 1) gameOver = 1;
            }
        }

        if (score > highScore) highScore = score;

        CLEAR_SCREEN();
        displayGrid(rows, cols, matrix);
        printf("\nGAME OVER!\nYour Score: %d\nHigh Score: %d\n", score, highScore);
        printf("Press 1 to Play Again, any other key to exit: ");
        int choice;
        scanf("%d", &choice);
        keepPlaying = (choice == 1);

        for (i = 0; i < rows + 2; i++) free(matrix[i]);
        free(matrix);
    }

    return 0;
}

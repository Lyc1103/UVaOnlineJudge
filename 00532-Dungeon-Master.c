#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef struct QueueInformation
{
	int front, rear;
	int **positions;
} Queue;

Queue *createQueue(int size)
{
	Queue *que = (Queue *)malloc(sizeof(Queue));
	que->front = -1;
	que->rear = -1;
	que->positions = (int **)malloc(3 * sizeof(int *));
	que->positions[0] = (int *)malloc(size * sizeof(int));
	que->positions[1] = (int *)malloc(size * sizeof(int));
	que->positions[2] = (int *)malloc(size * sizeof(int));
	return que;
}

_Bool isEmptyQueue(Queue *que)
{
	return que->front == que->rear ? 1 : 0;
}

void enQueueToRear(Queue *que, int cur_level, int cur_row, int cur_col)
{
	(que->rear)++;
	que->positions[0][que->rear] = cur_level;
	que->positions[1][que->rear] = cur_row;
	que->positions[2][que->rear] = cur_col;
	return;
}

void deQueueFromFront(Queue *que, int *ret_level, int *ret_row, int *ret_col)
{
	if (isEmptyQueue(que))
	{
		*ret_level = *ret_row = *ret_col = -1;
		puts("There are no elements could be take from queue.");
		return;
	}
	(que->front)++;
	*ret_level = que->positions[0][que->front];
	*ret_row = que->positions[1][que->front];
	*ret_col = que->positions[2][que->front];
	return;
}

void freeQueue(Queue *que)
{
	free(que->positions[0]);
	free(que->positions[1]);
	free(que->positions[2]);
	free(que->positions);
	free(que);
	return;
}

void printElementsInQueue(Queue *que)
{
	if (isEmptyQueue(que))
	{
		puts("que = []");
		return;
	}
	printf("que = [");
	int i;
	for (i = que->front + 1; i < que->rear; i++)
		printf("(%d, %d, %d),", que->positions[0][i], que->positions[1][i], que->positions[2][i]);
	printf("(%d, %d, %d)]\n", que->positions[0][que->rear], que->positions[1][que->rear], que->positions[2][que->rear]);
	return;
}

void BFSFindPath(char ***dungeon, int level, int rows, int cols, int ***dungeon_path_len, int start_level, int start_row, int start_col, int end_level, int end_row, int end_col)
{
	Queue *que = createQueue(level * rows * cols);
	dungeon_path_len[start_level][start_row][start_col] = 1;
	enQueueToRear(que, start_level, start_row, start_col);

	int cur_level, cur_row, cur_col;
	while (!isEmptyQueue(que))
	{
		deQueueFromFront(que, &cur_level, &cur_row, &cur_col);
		if (cur_level == end_level && cur_row == end_row && cur_col == end_col)
			return;

		/* North */
		if (cur_row > 0 && dungeon[cur_level][cur_row - 1][cur_col] != '#' && dungeon_path_len[cur_level][cur_row - 1][cur_col] == 0)
		{
			dungeon_path_len[cur_level][cur_row - 1][cur_col] = dungeon_path_len[cur_level][cur_row][cur_col] + 1;
			enQueueToRear(que, cur_level, cur_row - 1, cur_col);
		}
		/* South */
		if (cur_row + 1 < rows && dungeon[cur_level][cur_row + 1][cur_col] != '#' && dungeon_path_len[cur_level][cur_row + 1][cur_col] == 0)
		{
			dungeon_path_len[cur_level][cur_row + 1][cur_col] = dungeon_path_len[cur_level][cur_row][cur_col] + 1;
			enQueueToRear(que, cur_level, cur_row + 1, cur_col);
		}
		/* East */
		if (cur_col + 1 < cols && dungeon[cur_level][cur_row][cur_col + 1] != '#' && dungeon_path_len[cur_level][cur_row][cur_col + 1] == 0)
		{
			dungeon_path_len[cur_level][cur_row][cur_col + 1] = dungeon_path_len[cur_level][cur_row][cur_col] + 1;
			enQueueToRear(que, cur_level, cur_row, cur_col + 1);
		}
		/* West */
		if (cur_col > 0 && dungeon[cur_level][cur_row][cur_col - 1] != '#' && dungeon_path_len[cur_level][cur_row][cur_col - 1] == 0)
		{
			dungeon_path_len[cur_level][cur_row][cur_col - 1] = dungeon_path_len[cur_level][cur_row][cur_col] + 1;
			enQueueToRear(que, cur_level, cur_row, cur_col - 1);
		}
		/* Up */
		if (cur_level + 1 < level && dungeon[cur_level + 1][cur_row][cur_col] != '#' && dungeon_path_len[cur_level + 1][cur_row][cur_col] == 0)
		{
			dungeon_path_len[cur_level + 1][cur_row][cur_col] = dungeon_path_len[cur_level][cur_row][cur_col] + 1;
			enQueueToRear(que, cur_level + 1, cur_row, cur_col);
		}
		/* Down */
		if (cur_level > 0 && dungeon[cur_level - 1][cur_row][cur_col] != '#' && dungeon_path_len[cur_level - 1][cur_row][cur_col] == 0)
		{
			dungeon_path_len[cur_level - 1][cur_row][cur_col] = dungeon_path_len[cur_level][cur_row][cur_col] + 1;
			enQueueToRear(que, cur_level - 1, cur_row, cur_col);
		}
	}
	freeQueue(que);
	return;
}

int dungeonMaster(char ***dungeon, int level, int rows, int cols, int start_level, int start_row, int start_col, int end_level, int end_row, int end_col)
{
	int i, j;
	int ***dungeon_path_len = (int ***)malloc(level * sizeof(int **));
	for (i = 0; i < level; i++)
	{
		dungeon_path_len[i] = (int **)malloc(rows * sizeof(int *));
		for (j = 0; j < rows; j++)
			dungeon_path_len[i][j] = (int *)calloc(cols, sizeof(int));
	}
	BFSFindPath(dungeon, level, rows, cols, dungeon_path_len, start_level, start_row, start_col, end_level, end_row, end_col);
	return dungeon_path_len[end_level][end_row][end_col] - 1;
}

void printDungeon(char ***dungeon, int level, int rows, int cols)
{
	int i, j, k;
	puts("dungeon: ");
	for (i = 0; i < level; i++)
	{
		printf("level %d: ", i);
		for (k = 0; k < cols; k++)
			printf(" %d", k);
		puts("");
		for (j = 0; j < rows; j++)
		{
			printf("\t%d", j);
			for (k = 0; k < cols; k++)
			{
				printf(" %c", dungeon[i][j][k]);
			}
			puts("");
		}
	}
	return;
}

int main()
{
	/*
	FILE *pfile_r, *pfile_w;
	if ((pfile_r = fopen("00532.txt", "r")) == NULL)
	{
		puts("Read file could not be opened!");
		return 0;
	}
	if ((pfile_w = fopen("output.txt", "w")) == NULL)
	{
		puts("Write file could not be opened!");
		return 0;
	}
	*/

	int i, j, k;
	char c;
	int level, rows, cols;
	int start_level = -1, start_row = -1, start_col = -1;
	int end_level = -1, end_row = -1, end_col = -1;
	/* while (fscanf(pfile_r, "%d %d %d", &level, &rows, &cols)) */
	while (scanf("%d %d %d", &level, &rows, &cols) == 3)
	{
		if (level == 0 && rows == 0 && cols == 0)
			break;

		/* fscanf(pfile_r, "%c", &c); */
		scanf("%c", &c);
		char ***dungeon = (char ***)malloc(level * sizeof(char **));
		for (i = 0; i < level; i++)
		{
			dungeon[i] = (char **)malloc(rows * sizeof(char *));
			for (j = 0; j < rows; j++)
			{
				dungeon[i][j] = (char *)malloc(cols * sizeof(char));
				for (k = 0; k < cols; k++)
				{
					/* fscanf(pfile_r, "%c", &dungeon[i][j][k]); */
					scanf("%c", &dungeon[i][j][k]);
					if (dungeon[i][j][k] == 'S')
					{
						start_level = i;
						start_row = j;
						start_col = k;
					}
					else if (dungeon[i][j][k] == 'E')
					{
						end_level = i;
						end_row = j;
						end_col = k;
					}
				}
				/* fscanf(pfile_r, "%c", &c); */
				scanf("%c", &c);
			}
			/* fscanf(pfile_r, "%c", &c); */
			scanf("%c", &c);
		}
		/* printDungeon(dungeon, level, rows, cols); */
		if (start_level == -1 || end_level == -1)
		{
			puts("No Start point or End point.");
			return 0;
		}

		int res = dungeonMaster(dungeon, level, rows, cols, start_level, start_row, start_col, end_level, end_row, end_col);
		if (res == -1)
		{
			puts("Trapped!");
			/* fprintf(pfile_w, "Trapped!\n"); */
		}
		else
		{
			printf("Escaped in %d minute(s).\n", res);
			/* fprintf(pfile_w, "Escaped in %d minute(s).\n", res); */
		}
	}

	return 0;
}
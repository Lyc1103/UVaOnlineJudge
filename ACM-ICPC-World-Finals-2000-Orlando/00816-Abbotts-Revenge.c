#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAZESIZE 11
#define MAX(a, b) (a > b ? a : b)
const char ESWN[4] = "ESWN";
void print_dir(_Bool **dir);

#define PFILE 0

#define _DEBUG
#undef _DEBUG
#ifdef _DEBUG
#define DEBUG 1
#include <stdarg.h>
static int DebugPrintf(const char *format, ...)
{
	va_list argPtr;
	int count;

	va_start(argPtr, format);		  /* 獲取可變引述列表 //*/
	fflush(stdout);				  /* 強制重新整理輸出緩衝區 //*/
	count = vfprintf(stderr, format, argPtr); /* 將資訊輸出到標準出錯劉裝置 //*/
	va_end(argPtr);				  /* 可變引述列表結束 //*/
}
#else
#define DEBUG 0
static int DebugPrintf(const char *format, ...)
{
}
#endif

/* ------------------------ */
/* Data structure for Node. */
/* ------------------------ */
struct Node
{
	int comeDir;
	int mazeRow, mazeCol;
	struct Node **nextNodeFromComeDir;
	_Bool isVisited;
	struct Node *previousNode;
};

struct Node *createNode(int come_dir, int pos_row, int pos_col)
{
	struct Node *newnode = (struct Node *)malloc(sizeof(struct Node));
	newnode->comeDir = come_dir;
	newnode->mazeRow = pos_row;
	newnode->mazeCol = pos_col;
	newnode->nextNodeFromComeDir = (struct Node **)malloc(3 * sizeof(struct Node *));
	newnode->nextNodeFromComeDir[0] = NULL;
	newnode->nextNodeFromComeDir[1] = NULL;
	newnode->nextNodeFromComeDir[2] = NULL;
	newnode->isVisited = 0;
	newnode->previousNode = NULL;
	return newnode;
}

/* ------------------------- */
/* Data structure for Graph. */
/* ------------------------- */

typedef struct graphInformation
{
	int maxRowIndex, maxColIndex;
	int graphSize, graphColSize;
	struct Node ****nodes;
} Graph;

Graph *createGraph(int graphSize, int graphColSize)
{
	Graph *graph = (Graph *)malloc(sizeof(Graph));
	graph->maxRowIndex = 0;
	graph->maxColIndex = 0;
	graph->graphSize = graphSize;
	graph->graphColSize = graphColSize;
	graph->nodes = (struct Node ****)malloc(graphSize * sizeof(struct Node ***));

	int i, j, k;
	for (i = 0; i < graphSize; i++)
	{
		graph->nodes[i] = (struct Node ***)malloc(graphColSize * sizeof(struct Node **));
		for (j = 0; j < graphColSize; j++)
		{
			graph->nodes[i][j] = (struct Node **)malloc(4 * sizeof(struct Node *));
			for (k = 0; k < 4; k++)
				graph->nodes[i][j][k] = createNode(k, i, j);
		}
	}
	return graph;
}

void printGraph(Graph *graph)
{
	int i, j, k, l;
	struct Node *cur_node, *next_node;
	for (i = 1; i <= graph->maxRowIndex; i++)
	{
		for (j = 1; j <= graph->maxColIndex; j++)
		{
			for (k = 0; k < 4; k++)
			{
				cur_node = graph->nodes[i][j][k];
				if (k == 0)
					printf("(%d, %d)\n", cur_node->mazeRow, cur_node->mazeCol);
				printf("  %c [", ESWN[k]);

				for (l = 0; l < 2; l++)
				{
					next_node = cur_node->nextNodeFromComeDir[l];
					if (next_node == NULL)
						printf("   null, ");
					else
						printf("(%d, %d)%c, ", next_node->mazeRow, next_node->mazeCol, ESWN[next_node->comeDir]);
				}
				next_node = cur_node->nextNodeFromComeDir[l];
				if (next_node == NULL)
					printf("   null]\n");
				else
					printf("(%d, %d)%c]\n", next_node->mazeRow, next_node->mazeCol, ESWN[next_node->comeDir]);
			}
		}
	}
}

void freeGraph(Graph *graph)
{
	int i, j, k;

	for (i = 0; i < graph->graphSize; i++)
	{
		for (j = 0; j < graph->graphColSize; j++)
		{
			for (k = 0; k < 4; k++)
				free(graph->nodes[i][j][k]);
			free(graph->nodes[i][j]);
		}
		free(graph->nodes[i]);
	}

	free(graph->nodes);
	free(graph);
}

void linkNodeEdges(Graph *graph, int row, int col, _Bool **dir_buffer)
{

	/* dir{0:E, 1:S, 2:W, 3:N} */
	/* ------------------------ */
	/* Curent direction is East */
	/* ------------------------ */
	struct Node *cur_node = graph->nodes[row][col][0];
	if (dir_buffer[0][0] && row > 1) /* being going to go Left. */
		cur_node->nextNodeFromComeDir[0] = graph->nodes[row - 1][col][3];
	if (dir_buffer[0][1] && col < 9) /* being going to go Front. */
		cur_node->nextNodeFromComeDir[1] = graph->nodes[row][col + 1][0];
	if (dir_buffer[0][2] && row < 9) /* being going to go Right. */
		cur_node->nextNodeFromComeDir[2] = graph->nodes[row + 1][col][1];

	/* ------------------------ */
	/* Curent direction is South */
	/* ------------------------ */
	cur_node = graph->nodes[row][col][1];
	if (dir_buffer[1][0] && col < 9) /* being going to go Left. */
		cur_node->nextNodeFromComeDir[0] = graph->nodes[row][col + 1][0];
	if (dir_buffer[1][1] && row < 9) /* being going to go Front. */
		cur_node->nextNodeFromComeDir[1] = graph->nodes[row + 1][col][1];
	if (dir_buffer[1][2] && col > 1) /* being going to go Right. */
		cur_node->nextNodeFromComeDir[2] = graph->nodes[row][col - 1][2];

	/* ------------------------ */
	/* Curent direction is West */
	/* ------------------------ */
	cur_node = graph->nodes[row][col][2];
	if (dir_buffer[2][0] && row < 9) /* being going to go Left. */
		cur_node->nextNodeFromComeDir[0] = graph->nodes[row + 1][col][1];
	if (dir_buffer[2][1] && col > 1) /* being going to go Front. */
		cur_node->nextNodeFromComeDir[1] = graph->nodes[row][col - 1][2];
	if (dir_buffer[2][2] && row > 1) /* being going to go Right. */
		cur_node->nextNodeFromComeDir[2] = graph->nodes[row - 1][col][3];

	/* ------------------------ */
	/* Curent direction is North */
	/* ------------------------ */
	cur_node = graph->nodes[row][col][3];
	if (dir_buffer[3][0] && col > 1) /* being going to go Left. */
		cur_node->nextNodeFromComeDir[0] = graph->nodes[row][col - 1][2];
	if (dir_buffer[3][1] && row > 1) /* being going to go Front. */
		cur_node->nextNodeFromComeDir[1] = graph->nodes[row - 1][col][3];
	if (dir_buffer[3][2] && col < 9) /* being going to go Right. */
		cur_node->nextNodeFromComeDir[2] = graph->nodes[row][col + 1][0];

	return;
}

/* ------------------------- */
/* Data structure for Stack. */
/* ------------------------- */
typedef struct stackInformation
{
	int top;
	struct Node **nodes;
} Stack;

_Bool isEmptyStack(Stack *sta)
{
	return sta->top == -1 ? 1 : 0;
}

Stack *createStack(int size)
{
	Stack *sta = (Stack *)malloc(sizeof(Stack));
	sta->top = -1;
	sta->nodes = (struct Node **)malloc(size * sizeof(struct Node *));
	return sta;
}

void pushToStack(Stack *sta, struct Node *node)
{
	(sta->top)++;
	sta->nodes[sta->top] = node;
	return;
}

struct Node *popFromStack(Stack *sta)
{
	return sta->nodes[(sta->top)--];
}

void printElementsInStack(Stack *sta)
{
	if (isEmptyStack(sta))
	{
		puts("sta = []");
		return;
	}

	printf("sta = [");
	int i;
	struct Node *cur_node;
	for (i = 0; i < sta->top; i++)
	{
		cur_node = sta->nodes[i];
		printf("%c(%d, %d), ", ESWN[cur_node->comeDir], cur_node->mazeRow, cur_node->mazeCol);
	}
	cur_node = sta->nodes[i];
	/* printf("%c(%d, %d)]\n", ESWN[cur_node->comeDir], cur_node->mazeRow, cur_node->mazeCol); */
	printf("%c", ESWN[cur_node->comeDir]);
	printf("(%d, ", cur_node->mazeRow);
	printf(" %d)]\n", cur_node->mazeCol);
	return;
}

void freeStack(Stack *sta)
{
	free(sta->nodes);
	free(sta);
	return;
}

/* ----------- */
/* Algorithms. */
/* ----------- */
_Bool BFS(Graph *graph, struct Node *start_node, int goal_row, int goal_col, struct Node **goal_node)
{
	int i;
	Stack *s = createStack(MAZESIZE * MAZESIZE / 2);
	Stack *t = createStack(MAZESIZE * MAZESIZE / 2);

	struct Node *cur_node, *next_node;
	next_node = start_node->nextNodeFromComeDir[0];
	next_node->previousNode = start_node;
	pushToStack(t, next_node);
	while (!isEmptyStack(s) || !isEmptyStack(t))
	{
		while (!isEmptyStack(t))
		{
			cur_node = popFromStack(t);
			if (cur_node->mazeRow == goal_row && cur_node->mazeCol == goal_col)
			{
				*goal_node = cur_node;
				return 1;
			}
			for (i = 0; i < 3; i++)
			{
				next_node = cur_node->nextNodeFromComeDir[i];
				if (next_node != NULL && !next_node->isVisited)
				{
					next_node->isVisited = 1;
					next_node->previousNode = cur_node;
					pushToStack(s, next_node);
				}
			}
		}

		while (!isEmptyStack(s))
		{
			cur_node = popFromStack(s);
			pushToStack(t, cur_node);
		}
	}

	freeStack(s);
	freeStack(t);
	return 0;
}

/* ---------------- */
/* Problem solving. */
/* ---------------- */
void printPath(Graph *graph, int start_row, int start_col, struct Node *goal_node)
{
	struct Node *cur_node = goal_node;
	int size = MAZESIZE * MAZESIZE * 10, pathSize = 0, i;
	int **path = (int **)malloc(2 * sizeof(int *));
	path[0] = (int *)malloc(size * sizeof(int));
	path[1] = (int *)malloc(size * sizeof(int));
	if (goal_node->mazeRow == start_row && goal_node->mazeCol == start_col)
	{
		path[0][pathSize] = start_row;
		path[1][pathSize] = start_col;
		pathSize++;
		cur_node = cur_node->previousNode;
	}
	for (; cur_node->previousNode != NULL && cur_node->comeDir != -1; cur_node = cur_node->previousNode)
	{
		if (pathSize >= size)
		{
			size *= 2;
			path[0] = realloc(path[0], size * sizeof(int));
			path[1] = realloc(path[1], size * sizeof(int));
		}
		path[0][pathSize] = cur_node->mazeRow;
		path[1][pathSize] = cur_node->mazeCol;
		pathSize++;
	}
	path[0][pathSize] = start_row;
	path[1][pathSize] = start_col;
	pathSize++;

	printf(" ");
	for (i = 0; i < pathSize; i++)
	{
		if (i && i % 10 == 0)
			printf("\n ");
		printf(" (%d,%d)", path[0][pathSize - 1 - i], path[1][pathSize - 1 - i]);
	}
	printf("\n");
	return;
}

void AbbottsRevenge(Graph *graph, struct Node *start_node, int start_dir, int goal_row, int goal_col)
{
	int i, j;
	struct Node *goal_node;
	if (BFS(graph, start_node, goal_row, goal_col, &goal_node))
	{
		DebugPrintf("startNode(%d,%d), goalNode(%d,%d)\n", start_node->mazeRow, start_node->mazeCol, goal_node->mazeRow, goal_node->mazeCol);
		printPath(graph, start_node->mazeRow, start_node->mazeCol, goal_node);
	}
	else
		printf("  No Solution Possible\n");
	return;
}

void print_dir(_Bool **dir)
{
	int i, j;
	printf("dir_buffer: L  F  R");
	puts("");
	for (i = 0; i < 4; i++)
	{
		printf("        %c [", ESWN[i]);
		for (j = 0; j < 2; j++)
			printf("%2d,", dir[i][j]);
		printf("%2d]\n", dir[i][j]);
	}
	puts("    completely print.");
	return;
}

int main()
{
	FILE *pfile_r;
	if (PFILE && (pfile_r = fopen("00816.txt", "r")) == NULL)
	{
		puts("Read file could not be opened!");
		return 0;
	}

	int i, j;
	char maze_name[21] = "", str[50], *token, input_dir;
	int start_row, start_col, goal_row, goal_col, maze_row, maze_col, start_dir, dir;
	_Bool **dir_buffer = (_Bool **)malloc(4 * sizeof(_Bool *));
	for (i = 0; i < 4; i++)
		dir_buffer[i] = (_Bool *)calloc(3, sizeof(_Bool));
	/*init dir_buffer: L  F  R
			E  0  0  0
			S  0  0  0
			W  0  0  0
			N  0  0  0
	*/
	while (1)
	{
		if (PFILE)
			fgets(maze_name, 21, pfile_r);
		else
			gets(maze_name);
		if (strcmp(maze_name, "END") == 0) /* Be careful! */
			break;
		DebugPrintf("strlen(mazeName) = %d\n", strlen(maze_name));

		if (PFILE)
			fgets(str, 50, pfile_r);
		else
			gets(str);
		token = strtok(str, " ");
		while (token != NULL)
		{
			start_row = token[0] - '0';
			token = strtok(NULL, " ");
			start_col = token[0] - '0';
			token = strtok(NULL, " ");
			input_dir = token[0];
			token = strtok(NULL, " ");
			goal_row = token[0] - '0';
			token = strtok(NULL, " ");
			goal_col = token[0] - '0';
			token = strtok(NULL, " ");
		}
		DebugPrintf("%d %d %c %d %d\n", start_row, start_col, input_dir, goal_row, goal_col);

		Graph *graph = createGraph(MAZESIZE, MAZESIZE);
		graph->maxRowIndex = start_row;
		graph->maxColIndex = start_col;
		struct Node *start_node = createNode(-1, start_row, start_col);
		if (input_dir == 69) /* E */
			start_node->nextNodeFromComeDir[0] = graph->nodes[start_row][start_col + 1][start_dir = 0];
		else if (input_dir == 83) /* S */
			start_node->nextNodeFromComeDir[0] = graph->nodes[start_row + 1][start_col][start_dir = 1];
		else if (input_dir == 87) /* W */
			start_node->nextNodeFromComeDir[0] = graph->nodes[start_row][start_col - 1][start_dir = 2];
		else if (input_dir == 78) /* N */
			start_node->nextNodeFromComeDir[0] = graph->nodes[start_row - 1][start_col][start_dir = 3];

		if (PFILE)
			fgets(str, 50, pfile_r);
		else
			gets(str);
		while (str[0] != '0')
		{
			token = strtok(str, " ");
			maze_row = token[0] - '0';
			token = strtok(NULL, " ");
			maze_col = token[0] - '0';
			DebugPrintf("%d %d ", maze_row, maze_col);
			graph->maxRowIndex = MAX(graph->maxRowIndex, maze_row);
			graph->maxColIndex = MAX(graph->maxColIndex, maze_col);

			for (i = 0; i < 4; i++)
				memset(dir_buffer[i], 0, 3 * sizeof(_Bool));

			token = strtok(NULL, " ");
			DebugPrintf("%s ", token);
			while (token[0] != '*')
			{
				if (token[0] == 'E')
					dir = 0;
				else if (token[0] == 'S')
					dir = 1;
				else if (token[0] == 'W')
					dir = 2;
				else if (token[0] == 'N')
					dir = 3;
				for (i = 1; i < strlen(token); i++)
				{
					if (token[i] == 'L')
						dir_buffer[dir][0] = 1;
					else if (token[i] == 'F')
						dir_buffer[dir][1] = 1;
					else if (token[i] == 'R')
						dir_buffer[dir][2] = 1;
				}
				token = strtok(NULL, " ");
				DebugPrintf("%s ", token);
			}
			linkNodeEdges(graph, maze_row, maze_col, dir_buffer);

			if (PFILE)
				fgets(str, 50, pfile_r);
			else
			{
				DebugPrintf("\n");
				gets(str);
			}
		}
		if (DEBUG)
			printGraph(graph);

		printf("%s", maze_name);
		if (!PFILE)
			puts("");
		AbbottsRevenge(graph, start_node, start_dir, goal_row, goal_col);
		freeGraph(graph);
	}

	return 0;
}
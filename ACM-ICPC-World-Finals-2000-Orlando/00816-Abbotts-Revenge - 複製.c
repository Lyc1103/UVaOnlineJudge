#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAZESIZE 11
#define MAX(a, b) (a > b ? a : b)
const char eswn[4] = "ESWN";
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
	int nextNodeNums, *nextDir;
	struct Node **nextNodeFromComeDir;
	_Bool isVisited;
	struct Node *previousNode;
};

struct Node *createNode(int dir, int pos_row, int pos_col)
{
	struct Node *newnode = (struct Node *)malloc(sizeof(struct Node));
	newnode->comeDir = dir;
	newnode->mazeRow = pos_row;
	newnode->mazeCol = pos_col;
	newnode->nextNodeNums = -1;
	newnode->nextNodeFromComeDir = (struct Node **)malloc(12 * sizeof(struct Node *));
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
				printf("  %c [", eswn[k]);

				if (cur_node->nextNodeNums == -1)
				{
					puts("]");
					continue;
				}
				for (l = 0; l < cur_node->nextNodeNums; l++)
				{
					next_node = cur_node->nextNodeFromComeDir[l];
					printf("(%d, %d)%c, ", next_node->mazeRow, next_node->mazeCol, eswn[next_node->comeDir]);
				}
				next_node = cur_node->nextNodeFromComeDir[l];
				printf("(%d, %d)%c]\n", next_node->mazeRow, next_node->mazeCol, eswn[next_node->comeDir]);
			}
		}
	}
}

void freeGraph(Graph *graph)
{
	int i, j, k;
	for (k = 0; k < 4; k++)
	{
		for (i = 0; i < graph->graphSize; i++)
		{
			for (j = 0; j < graph->graphColSize; j++)
				free(graph->nodes[k][i][j]);
			free(graph->nodes[k][i]);
		}
		free(graph->nodes[k]);
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
	if (dir_buffer[0][0]) /* being going to go Left. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row - 1][col][3];
	}
	if (dir_buffer[0][1]) /* being going to go Front. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row][col + 1][0];
	}
	if (dir_buffer[0][2]) /* being going to go Right. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row + 1][col][1];
	}

	/* ------------------------ */
	/* Curent direction is South */
	/* ------------------------ */
	cur_node = graph->nodes[row][col][1];
	if (dir_buffer[1][0]) /* being going to go Left. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row][col + 1][0];
	}
	if (dir_buffer[1][1]) /* being going to go Front. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row + 1][col][1];
	}
	if (dir_buffer[1][2]) /* being going to go Right. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row][col - 1][2];
	}

	/* ------------------------ */
	/* Curent direction is West */
	/* ------------------------ */
	cur_node = graph->nodes[row][col][2];
	if (dir_buffer[2][0]) /* being going to go Left. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row + 1][col][1];
	}
	if (dir_buffer[2][1]) /* being going to go Front. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row][col - 1][2];
	}
	if (dir_buffer[2][2]) /* being going to go Right. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row - 1][col][3];
	}

	/* ------------------------ */
	/* Curent direction is North */
	/* ------------------------ */
	cur_node = graph->nodes[row][col][3];
	if (dir_buffer[3][0]) /* being going to go Left. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row][col - 1][2];
	}
	if (dir_buffer[3][1]) /* being going to go Front. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row - 1][col][3];
	}
	if (dir_buffer[3][2]) /* being going to go Right. */
	{
		(cur_node->nextNodeNums)++;
		cur_node->nextNodeFromComeDir[cur_node->nextNodeNums] = graph->nodes[row][col + 1][0];
	}

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

struct Node *topOfStack(Stack *sta)
{
	return sta->nodes[sta->top];
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
		printf("%c(%d, %d), ", eswn[cur_node->comeDir], cur_node->mazeRow, cur_node->mazeCol);
	}
	cur_node = sta->nodes[i];
	/* printf("%c(%d, %d)]\n", eswn[cur_node->comeDir], cur_node->mazeRow, cur_node->mazeCol); */
	printf("%c", eswn[cur_node->comeDir]);
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
	int i, j;
	Stack *s = createStack(MAZESIZE * MAZESIZE / 2);
	Stack *t = createStack(MAZESIZE * MAZESIZE / 2);

	struct Node *cur_node, *next_node;
	int come_dir, next_dir; /* dir{0:E, 1:S, 2:W, 3:N} */
	_Bool flag = 0;
	next_node = start_node->nextNodeFromComeDir[0];
	next_node->previousNode = start_node;
	pushToStack(t, next_node);
	while (!isEmptyStack(s) || !isEmptyStack(t))
	{
		if (DEBUG)
			printElementsInStack(t);
		while (!isEmptyStack(t))
		{
			cur_node = popFromStack(t);
			DebugPrintf("// cur_node = (%d, %d)\n", cur_node->mazeRow, cur_node->mazeCol);
			if (cur_node->mazeRow == goal_row && cur_node->mazeCol == goal_col)
			{
				*goal_node = cur_node;
				return 1;
			}
			for (i = 0; i <= cur_node->nextNodeNums; i++)
			{
				next_node = cur_node->nextNodeFromComeDir[i];
				if (!next_node->isVisited)
				{
					next_node->isVisited = 1;
					next_node->previousNode = cur_node;
					pushToStack(s, cur_node->nextNodeFromComeDir[i]);
				}
			}
		}

		while (!isEmptyStack(s))
		{
			cur_node = popFromStack(s);
			pushToStack(t, cur_node);
		}
	}
	return 0;
}

/* ---------------- */
/* Problem solving. */
/* ---------------- */
void printPath(Graph *graph, int start_row, int start_col, struct Node *goal_node)
{
	DebugPrintf("start_node(%d,%d), goal_node(%d,%d)\n", start_row, start_col, goal_node->mazeRow, goal_node->mazeCol);
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
	for (; cur_node->comeDir != -1; cur_node = cur_node->previousNode)
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

	/*
	fprintf(pfile_w, " ");
	for (i = 0; i < pathSize; i++)
	{
		if (i && i % 10 == 0)
			fprintf(pfile_w, "\n ");
		fprintf(pfile_w, " (%d,%d)", path[0][pathSize - 1 - i], path[1][pathSize - 1 - i]);
	}
	fprintf(pfile_w, "\n");
	*/
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
		printPath(graph, start_node->mazeRow, start_node->mazeCol, goal_node);
	else
		/* fprintf(pfile_w, "  No Solution Possible\n"); */
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
		printf("        %c [", eswn[i]);
		for (j = 0; j < 2; j++)
			printf("%2d,", dir[i][j]);
		printf("%2d]\n", dir[i][j]);
	}
	puts("    completely print.");
	return;
}

int main()
{
	/*
	FILE *pfile_r, *pfile_w;
	if (PFILE)
	{
		if ((pfile_r = fopen("00816.txt", "r")) == NULL)
		{
			puts("Read file could not be opened!");
			return 0;
		}
		if ((pfile_w = fopen("output.txt", "w")) == NULL)
		{
			puts("Write file could not be opened!");
			return 0;
		}
	}
	else
	{
		pfile_r = stdin;
		pfile_w = stdout;
	}
	*/

	int i, j;
	char maze_name[21] = "", dir, c, terminal[21] = "END";
	int start_row, start_col, goal_row, goal_col, maze_row = -1, maze_col, start_dir;
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
		/* fgets(maze_name, 21, pfile_r); */
		fgets(maze_name, 21, stdin);
		maze_name[strlen(maze_name)] = '\0';
		DebugPrintf("nameSize = %d\n", strlen(maze_name));

		/* Be careful! */
		/*
		if (PFILE && strcmp(maze_name, "END") == 0)
			break;
		else if (!PFILE && strcmp(maze_name, "END\n") == 0)
			break;
		*/
		if (strcmp(maze_name, "END\n") == 0)
			break;

		/*
		fprintf(pfile_w, "%s", maze_name);
		fscanf(pfile_r, "%d %d %c %d %d", &start_row, &start_col, &dir, &goal_row, &goal_col);
		*/

		scanf("%d %d %c %d %d", &start_row, &start_col, &dir, &goal_row, &goal_col);
		DebugPrintf("%d %d %c %d %d\n", start_row, start_col, dir, goal_row, goal_col);

		Graph *graph = createGraph(MAZESIZE, MAZESIZE);
		struct Node *start_node = createNode(-1, start_row, start_col);
		if (dir == 'E')
			start_node->nextNodeFromComeDir[++(start_node->nextNodeNums)] = graph->nodes[start_row][start_col + 1][start_dir = 0];
		else if (dir == 'S')
			start_node->nextNodeFromComeDir[++(start_node->nextNodeNums)] = graph->nodes[start_row + 1][start_col][start_dir = 1];
		else if (dir == 'W')
			start_node->nextNodeFromComeDir[++(start_node->nextNodeNums)] = graph->nodes[start_row][start_col - 1][start_dir = 2];
		else if (dir == 'N')
			start_node->nextNodeFromComeDir[++(start_node->nextNodeNums)] = graph->nodes[start_row - 1][start_col][start_dir = 3];

		while (1)
		{
			/*
			fscanf(pfile_r, "%d", &maze_row);
			if (maze_row == 0)
			{
				fgetc(pfile_r);
				break;
			}
			fscanf(pfile_r, "%d%c", &maze_col, &c);
			*/
			scanf("%d", &maze_row);
			if (maze_row == 0)
			{
				fgetc(stdin);
				break;
			}
			scanf("%d%c", &maze_col, &c);
			DebugPrintf("%d %d%c", maze_row, maze_col, c);
			graph->maxRowIndex = MAX(graph->maxRowIndex, maze_row);
			graph->maxColIndex = MAX(graph->maxColIndex, maze_col);

			for (i = 0; i < 4; i++)
				memset(dir_buffer[i], 0, 3 * sizeof(_Bool));
			while (1)
			{
				/* c = (char)fgetc(pfile_r); */
				c = (char)fgetc(stdin);
				DebugPrintf("%c", c);
				if (c == '*')
					break;
				else if (c == ' ')
					continue;
				else if (c == 'E')
					dir = 0;
				else if (c == 'S')
					dir = 1;
				else if (c == 'W')
					dir = 2;
				else if (c == 'N')
					dir = 3;
				while (1)
				{
					/* c = (char)fgetc(pfile_r); */
					c = (char)fgetc(stdin);
					DebugPrintf("%c", c);
					if (c == ' ')
						break;
					else if (c == 'L')
						dir_buffer[dir][0] = 1;
					else if (c == 'F')
						dir_buffer[dir][1] = 1;
					else if (c == 'R')
						dir_buffer[dir][2] = 1;
				}
			}
			DebugPrintf("\n");
			linkNodeEdges(graph, maze_row, maze_col, dir_buffer);
		}
		if (DEBUG)
			printGraph(graph);

		printf("%s", maze_name);
		AbbottsRevenge(graph, start_node, start_dir, goal_row, goal_col);
	}

	return 0;
}

while (1)
{
	c = getc(stdin);
	printf("%d ", c);
	if (c == 42) /* *(star) */
		break;
	else if (c == 32) /* SPACE */
		continue;
	else if (c == 69) /* E */
		dir = 0;
	else if (c == 83) /* S */
		dir = 1;
	else if (c == 87) /* W */
		dir = 2;
	else if (c == 78) /* N */
		dir = 3;
	while (1)
	{
		c = getc(stdin);
		if (c == 32) /* SPACE */
			break;
		else if (c == 76) /* L */
			dir_buffer[dir][0] = 1;
		else if (c == 70) /* F */
			dir_buffer[dir][1] = 1;
		else if (c == 82) /* R */
			dir_buffer[dir][2] = 1;
	}
}

while (1)
{
	scanf("%d", &maze_row);
	if (maze_row == 0)
	{
		getc(stdin);
		break;
	}
	scanf("%d%c", &maze_col, &c);
	printf("%d %d ", maze_row, maze_col);

	graph->maxRowIndex = MAX(graph->maxRowIndex, maze_row);
	graph->maxColIndex = MAX(graph->maxColIndex, maze_col);

	for (i = 0; i < 4; i++)
		memset(dir_buffer[i], 0, 3 * sizeof(_Bool));

	while (1)
	{
		c = (char)getc(stdin);
		/* printf("%c", c); */
		if (c == '*')
			break;
		else if (c == ' ')
			continue;
		else if (c == 'E')
			dir = 0;
		else if (c == 'S')
			dir = 1;
		else if (c == 'W')
			dir = 2;
		else if (c == 'N')
			dir = 3;
		while (1)
		{
			c = (char)getc(stdin);
			/* printf("%c", c); */
			if (c == ' ')
				break;
			else if (c == 'L')
				dir_buffer[dir][0] = 1;
			else if (c == 'F')
				dir_buffer[dir][1] = 1;
			else if (c == 'R')
				dir_buffer[dir][2] = 1;
		}
	}
	/*puts("");
	/* linkNodeEdges(graph, maze_row, maze_col, dir_buffer); */
}
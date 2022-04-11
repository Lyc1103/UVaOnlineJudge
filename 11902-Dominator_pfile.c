// Reference: https://github.com/morris821028/UVa/blob/master/volume119/11902%20-%20Dominator.cpp
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

void printMatrix_Bool(_Bool **mat, int matSize, int matColSize);
void printArray_Bool(_Bool *nums, int numsSize);

typedef struct stackInformation
{
	int top;
	int *indexes;
} Stack;

Stack *createStack(int size)
{
	Stack *sta = (Stack *)malloc(sizeof(Stack));
	sta->top = -1;
	sta->indexes = (int *)malloc(size * sizeof(int));
	return sta;
}

_Bool isEmptyStack(Stack *sta)
{
	return sta->top == -1 ? 1 : 0;
}

void pushToStack(Stack *sta, int index)
{
	(sta->top)++;
	sta->indexes[sta->top] = index;
	return;
}

int popFromStack(Stack *sta)
{
	(sta->top)--;
	return sta->indexes[(sta->top) + 1];
}

void freeStack(Stack *sta)
{
	free(sta->indexes);
	free(sta);
	return;
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
	for (i = 0; i < sta->top; i++)
		printf("%d,", sta->indexes[i]);
	printf("%d]\n", sta->indexes[i]);
	return;
}

void DFS(_Bool **mat, int matSize, _Bool *is_visited, int start_index, int end_index, Stack *sta)
{
	pushToStack(sta, start_index);
	is_visited[start_index] = 1;

	int cur_index, index;
	while (!isEmptyStack(sta))
	{
		cur_index = popFromStack(sta);
		if (cur_index != end_index)
			for (index = 0; index < matSize; index++)
				if (!is_visited[index] && mat[cur_index][index])
				{
					is_visited[index] = 1;
					pushToStack(sta, index);
				}
		/* printElementsInStack(sta); */
	}
	return;
}

_Bool **findDominator(_Bool **mat, int matSize)
{
	int i, j;
	_Bool **ret = (_Bool **)malloc(matSize * sizeof(_Bool *));
	for (i = 0; i < matSize; i++)
	{
		ret[i] = (_Bool *)calloc(matSize, sizeof(_Bool));
		ret[i][i] = 1;
	}
	_Bool *init_visited = (_Bool *)calloc(matSize, sizeof(_Bool));
	_Bool *is_visited = (_Bool *)malloc(matSize * sizeof(_Bool));
	Stack *sta = createStack(matSize * matSize);

	DFS(mat, matSize, init_visited, 0, INT_MIN, sta);
	for (i = 0; i < matSize; i++)
	{
		memset(is_visited, 0, matSize * sizeof(_Bool));
		DFS(mat, matSize, is_visited, 0, i, sta);
		for (j = 0; j < matSize; j++)
			if (init_visited[j] && !is_visited[j])
				ret[i][j] = 1;
		ret[i][i] = is_visited[i]; // Don't understand the reason for it.
	}

	/* printMatrix_Bool(ret, matSize, matSize); */

	return ret;
}

void printClapboard(int size)
{
	int i;
	printf("+");
	for (i = 0; i < size - 1; i++)
		printf("--");
	puts("-+");
}

void printSolution(_Bool **res, int resSize)
{
	int i, j;
	for (i = 0; i < resSize; i++)
	{
		printClapboard(resSize);

		printf("|");
		for (j = 0; j < resSize; j++)
		{
			if (res[i][j])
				printf("Y|");
			else
				printf("N|");
		}
		puts("");
	}
	printClapboard(resSize);
	return;
}

void printClapboardToOutputFile(FILE *pfile, int size)
{
	int i;
	fprintf(pfile, "+");
	for (i = 0; i < size - 1; i++)
		fprintf(pfile, "--");
	fprintf(pfile, "-+\n");
}

void printSolutionToOutputFile(FILE *pfile, _Bool **res, int resSize)
{
	int i, j;
	for (i = 0; i < resSize; i++)
	{
		printClapboardToOutputFile(pfile, resSize);

		fprintf(pfile, "|");
		for (j = 0; j < resSize; j++)
		{
			if (res[i][j])
				fprintf(pfile, "Y|");
			else
				fprintf(pfile, "N|");
		}
		fprintf(pfile, "\n");
	}
	printClapboardToOutputFile(pfile, resSize);
	return;
}

void printMatrix_Bool(_Bool **mat, int matSize, int matColSize)
{
	int i, j;
	if (matSize == 0)
		puts("mat:\n    []");
	else
	{
		printf("mat:  ");
		for (i = 0; i < matSize; i++)
			printf("  %d", i);
		puts("");
		for (i = 0; i < matSize; i++)
		{
			printf("    %d [", i);
			for (j = 0; j < matColSize - 1; j++)
				printf("%2d,", mat[i][j]);
			printf("%2d]\n", mat[i][matColSize - 1]);
		}
	}
	puts("    completely print.");
	return;
}

void printArray_Bool(_Bool *nums, int numsSize)
{
	if (numsSize == 0)
	{
		puts("[]");
		return;
	}
	int i;
	printf("[");
	for (i = 0; i < numsSize - 1; i++)
	{
		if (nums[i] == INT_MIN)
			printf("null,");
		else
			printf("%d,", nums[i]);
	}
	printf("%d]", nums[numsSize - 1]);
	puts(" // completely print.");
	return;
}

int main()
{
	FILE *pfile_r, *pfile_w;
	if ((pfile_r = fopen("11902.txt", "r")) == NULL)
	{
		puts("Read file could not be opened!");
		return 0;
	}
	if ((pfile_w = fopen("output.txt", "w")) == NULL)
	{
		puts("Write file could not be opened!");
		return 0;
	}

	int i, j, k;
	char c;
	int n, matSize;
	fscanf(pfile_r, "%d", &n);
	for (k = 1; k <= n; k++)
	{
		fscanf(pfile_r, "%d", &matSize);
		_Bool **mat = (_Bool **)malloc(matSize * sizeof(_Bool *));
		for (i = 0; i < matSize; i++)
		{
			mat[i] = (_Bool *)malloc(matSize * sizeof(_Bool));
			for (j = 0; j < matSize;)
			{
				fscanf(pfile_r, "%c", &c);
				if (c == '0' || c == '1')
					mat[i][j++] = c - '0';
			}
		}
		/* printMatrix_Bool(mat, matSize, matSize); */

		_Bool **res = findDominator(mat, matSize);
		printf("Case %d:\n", k);
		printSolution(res, matSize);
		fprintf(pfile_w, "Case %d:\n", k);
		printSolutionToOutputFile(pfile_w, res, matSize);
	}

	return 0;
}
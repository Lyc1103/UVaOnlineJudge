#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define PAGELIMITSIZE 101
#define OO 1000000001
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)
void printMatrix_int(int mat[PAGELIMITSIZE][PAGELIMITSIZE], int matSize, int matColSize);

int P(int m, int n)
{
	int i, res = 1;
	for (i = m; i > m - n; i--)
	{
		res *= i;
	}
	return res;
}

void FloydWarshallAlgorithm(int mat[PAGELIMITSIZE][PAGELIMITSIZE], int matSize)
{
	int i, j, k;
	for (i = 1; i < matSize; i++)
	{
		for (j = 1; j < matSize; j++)
		{
			if (j == i)
				continue;
			for (k = 1; k < matSize; k++)
			{
				if (k == i)
					continue;
				mat[j][k] = MIN(mat[j][k], mat[j][i] + mat[i][k]);
			}
		}
	}
}

double pageHopping(int pages[PAGELIMITSIZE][PAGELIMITSIZE], int pagesSize, int pagesNum, _Bool *is_exist)
{
	FloydWarshallAlgorithm(pages, pagesSize);
	/* printMatrix_int(pages, pagesSize, pagesSize); */

	int i, j, sum = 0, pair_of_nodes = 0; /* P(pagesNum, 2); */
	for (i = 1; i < pagesSize; i++)
		if (is_exist[i])
			for (j = 1; j < pagesSize; j++)
				if (pages[i][j] < OO && i != j)
				{
					sum += pages[i][j];
					pair_of_nodes++;
				}
	/* printf("sum = %d, pair_of_nodes = %d\n", sum, pair_of_nodes); */
	return (double)sum / pair_of_nodes;
}

void printMatrix_int(int mat[PAGELIMITSIZE][PAGELIMITSIZE], int matSize, int matColSize)
{
	int i, j;
	if (matSize == 0)
		puts("mat:\n    []");
	else
	{
		printf("mat:  ");
		for (i = 0; i < matColSize; i++)
			printf("  %d", i);
		puts("");
		for (i = 0; i < matSize; i++)
		{
			printf("    %d [", i);
			for (j = 0; j < matColSize - 1; j++)
			{
				if (mat[i][j] == OO)
					printf(" N,");
				else
					printf("%2d,", mat[i][j]);
			}
			if (mat[i][j] == OO)
				printf(" N]\n");
			else
				printf("%2d]\n", mat[i][j]);
		}
	}
	puts("    completely print.");
	return;
}

int main()
{
	/*
	FILE *pfile_r, *pfile_w;
	if ((pfile_r = fopen("00821.txt", "r")) == NULL)
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

	int i, j, k, ctr = 1;
	char c;
	int index_x, index_y, cost;
	/* while (fscanf(pfile_r, "%d %d", &index_x, &index_y)) */
	while (scanf("%d %d", &index_x, &index_y))
	{
		if (index_x == 0 && index_y == 0)
			break;

		int pages[PAGELIMITSIZE][PAGELIMITSIZE] = {0}, pagesSize = -1, pagesNum = 0;
		_Bool is_exist[PAGELIMITSIZE] = {0};
		for (i = 1; i < PAGELIMITSIZE; i++)
			for (j = 1; j < PAGELIMITSIZE; j++)
				pages[i][j] = OO;
		pagesSize = MAX(pagesSize, MAX(index_x, index_y));
		pages[index_x][index_y] = 1;

		/* while (fscanf(pfile_r, "%d %d", &index_x, &index_y)) */
		while (scanf("%d %d", &index_x, &index_y))
		{
			if (index_x == 0 && index_y == 0)
				break;
			pagesSize = MAX(pagesSize, MAX(index_x, index_y) + 1);
			pages[index_x][index_y] = 1;
			if (!is_exist[index_x])
			{
				is_exist[index_x] = 1;
				pagesNum++;
			}
			if (!is_exist[index_y])
			{
				is_exist[index_y] = 1;
				pagesNum++;
			}
		}
		/* printMatrix_int(pages, pagesSize, pagesSize); */
		/* printf("pageNum = %d\n", pagesNum); */
		double res = pageHopping(pages, pagesSize, pagesNum, is_exist);
		printf("Case %d: average length between pages = %.3f clicks\n", ctr, res);
		ctr++;
	}

	return 0;
}
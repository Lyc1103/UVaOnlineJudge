#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define OUTPOSTLIMIT 500
#define OO 10000001.0
#define MAX(a, b) (a > b ? a : b)

int cmpFunction(const void *a, const void *b)
{
	return *(double *)a > *(double *)b ? 1 : -1;
}

/* ----------------------- */
/* Definition for outpost. */
/* ----------------------- */
struct Outpost
{
	int index;
	int x, y;
	double costOfOutpost;
	_Bool isVisited;
	int outpostsSize;
	struct Outpost **adjacentOutposts;
	double *distances;
};

struct Outpost *createNewOutpost(int index, int x, int y)
{
	struct Outpost *new_outpost = (struct Outpost *)malloc(sizeof(struct Outpost));
	new_outpost->index = index;
	new_outpost->x = x;
	new_outpost->y = y;
	new_outpost->isVisited = 0;
	new_outpost->costOfOutpost = OO;
	new_outpost->outpostsSize = -1;
	new_outpost->adjacentOutposts = (struct Outpost **)malloc(OUTPOSTLIMIT * sizeof(struct Outpost *));
	new_outpost->distances = (double *)malloc(OUTPOSTLIMIT * sizeof(double));
	return new_outpost;
}

void linkOutposts(struct Outpost *outpost1, struct Outpost *outpost2)
{
	double dist = sqrt(pow(((outpost1->x) - (outpost2->x)), 2) + pow(((outpost1->y) - (outpost2->y)), 2));
	(outpost1->outpostsSize)++;
	outpost1->adjacentOutposts[outpost1->outpostsSize] = outpost2;
	outpost1->distances[outpost1->outpostsSize] = dist;
	(outpost2->outpostsSize)++;
	outpost2->adjacentOutposts[outpost2->outpostsSize] = outpost1;
	outpost2->distances[outpost2->outpostsSize] = dist;
	return;
}

void swap(struct Outpost **a, struct Outpost **b)
{
	struct Outpost *tmp = *a;
	*a = *b;
	*b = tmp;
	return;
}

/* --------------------- */
/* Definition for graph. */
/* --------------------- */
typedef struct graphInformation
{
	int graphSize;
	struct Outpost **outposts;
} Graph;

Graph *createGraph(int graphSize)
{
	Graph *graph = (Graph *)malloc(graphSize * sizeof(Graph));
	graph->graphSize = 0;
	graph->outposts = (struct Outpost **)malloc(graphSize * sizeof(struct Outpost *));
	return graph;
}

void freeGraph(Graph *graph)
{
	int i;
	for (i = 0; i < graph->graphSize; i++)
		free(graph->outposts[i]);
	free(graph->outposts);
	free(graph);
	return;
}

void printElementsInGraph(Graph *graph)
{
	int i;
	for (i = 0; i < graph->graphSize; i++)
		printf("graph[%d] = %d(%d, %d, %.2f)\n", i, graph->outposts[i]->index, graph->outposts[i]->x, graph->outposts[i]->y, graph->outposts[i]->costOfOutpost);
	return;
}

/* ------------------------ */
/* Definition for min-heap. */
/* ------------------------ */
typedef struct minHeapInformation
{
	int last_index;
	int *positions;
	struct Outpost **outposts;
} MinHeap;

MinHeap *createMinHeap(int size)
{
	MinHeap *minheap = (MinHeap *)malloc(sizeof(MinHeap));
	minheap->last_index = 0;
	minheap->positions = (int *)malloc((size + 1) * sizeof(int));
	minheap->outposts = (struct Outpost **)malloc((size + 1) * sizeof(struct Outpost *));
	return minheap;
}

_Bool isEmptyMinHeap(MinHeap *minheap)
{
	return minheap->last_index <= 0 ? 1 : 0;
}

void pushToMinHeap(MinHeap *minheap, struct Outpost *outpost)
{
	(minheap->last_index)++;
	minheap->positions[outpost->index] = minheap->last_index;
	minheap->outposts[minheap->last_index] = outpost;
	return;
}

void insertToMinHeap(MinHeap *minheap, struct Outpost *outpost)
{
	int pos;
	(minheap->last_index)++;
	minheap->positions[outpost->index] = minheap->last_index;
	minheap->outposts[minheap->last_index] = outpost;
	for (pos = minheap->last_index; pos / 2 > 0 && minheap->outposts[pos]->costOfOutpost < minheap->outposts[pos / 2]->costOfOutpost; pos /= 2)
	{
		swap(&(minheap->outposts[pos]), &(minheap->outposts[pos / 2]));
		minheap->positions[minheap->outposts[pos]->index] = pos;
		minheap->positions[minheap->outposts[pos / 2]->index] = pos / 2;
	}
	return;
}

struct Outpost *extractMinFromMinHeap(MinHeap *minheap)
{
	if (isEmptyMinHeap(minheap))
		return NULL;

	struct Outpost *ret = minheap->outposts[1];
	minheap->outposts[1] = minheap->outposts[minheap->last_index];
	minheap->positions[minheap->outposts[1]->index] = 1;
	(minheap->last_index)--;
	int pos, last_pos = minheap->last_index;
	for (pos = 1; pos <= last_pos;)
	{
		int lchild_pos = pos * 2;
		int rchild_pos = pos * 2 + 1;
		if (rchild_pos <= last_pos)
		{
			if (minheap->outposts[lchild_pos]->costOfOutpost > minheap->outposts[rchild_pos]->costOfOutpost)
			{
				if (minheap->outposts[pos]->costOfOutpost > minheap->outposts[rchild_pos]->costOfOutpost)
				{
					swap(&(minheap->outposts[pos]), &(minheap->outposts[rchild_pos]));
					minheap->positions[minheap->outposts[pos]->index] = pos;
					minheap->positions[minheap->outposts[rchild_pos]->index] = rchild_pos;
					pos = rchild_pos;
				}
				else
					return ret;
			}
			else
			{
				if (minheap->outposts[pos]->costOfOutpost > minheap->outposts[lchild_pos]->costOfOutpost)
				{
					swap(&(minheap->outposts[pos]), &(minheap->outposts[lchild_pos]));
					minheap->positions[minheap->outposts[pos]->index] = pos;
					minheap->positions[minheap->outposts[lchild_pos]->index] = lchild_pos;
					pos = lchild_pos;
				}
				else
					return ret;
			}
		}
		else if (lchild_pos <= last_pos)
		{
			if (minheap->outposts[pos]->costOfOutpost > minheap->outposts[lchild_pos]->costOfOutpost)
			{
				swap(&(minheap->outposts[pos]), &(minheap->outposts[lchild_pos]));
				minheap->positions[minheap->outposts[pos]->index] = pos;
				minheap->positions[minheap->outposts[lchild_pos]->index] = lchild_pos;
				pos = lchild_pos;
			}
			else
				return ret;
		}
		else
			return ret;
	}
	return ret;
}

void bottomUpBuildMinHeap(MinHeap *minheap)
{
	int i, last_pos = minheap->last_index;
	for (i = last_pos / 2; i > 0; i--)
	{
		int pos = i;
		while (pos <= last_pos)
		{
			int lchild_pos = pos * 2;
			int rchild_pos = pos * 2 + 1;
			if (rchild_pos <= last_pos)
			{
				if (minheap->outposts[lchild_pos]->costOfOutpost > minheap->outposts[rchild_pos]->costOfOutpost)
				{
					if (minheap->outposts[pos]->costOfOutpost > minheap->outposts[rchild_pos]->costOfOutpost)
					{
						/* printf("(index = %d, pos = %d)<-swap->(index = %d, pos = %d)\n", minheap->outposts[pos]->index, minheap->positions[minheap->outposts[pos]->index], minheap->outposts[rchild_pos]->index, minheap->positions[minheap->outposts[rchild_pos]->index]); */
						swap(&(minheap->outposts[pos]), &(minheap->outposts[rchild_pos]));
						minheap->positions[minheap->outposts[pos]->index] = pos;
						minheap->positions[minheap->outposts[rchild_pos]->index] = rchild_pos;
						/* printf("(index = %d, pos = %d)<------>(index = %d, pos = %d)\n\n", minheap->outposts[pos]->index, minheap->positions[minheap->outposts[pos]->index], minheap->outposts[rchild_pos]->index, minheap->positions[minheap->outposts[rchild_pos]->index]); */
						pos = rchild_pos;
					}
					else
						break;
				}
				else
				{
					if (minheap->outposts[pos]->costOfOutpost > minheap->outposts[lchild_pos]->costOfOutpost)
					{
						/* printf("(index = %d, pos = %d)<-swap->(index = %d, pos = %d)\n", minheap->outposts[pos]->index, minheap->positions[minheap->outposts[pos]->index], minheap->outposts[lchild_pos]->index, minheap->positions[minheap->outposts[lchild_pos]->index]); */
						swap(&(minheap->outposts[pos]), &(minheap->outposts[lchild_pos]));
						minheap->positions[minheap->outposts[pos]->index] = pos;
						minheap->positions[minheap->outposts[lchild_pos]->index] = lchild_pos;
						/* printf("(index = %d, pos = %d)<------>(index = %d, pos = %d)\n\n", minheap->outposts[pos]->index, minheap->positions[minheap->outposts[pos]->index], minheap->outposts[lchild_pos]->index, minheap->positions[minheap->outposts[lchild_pos]->index]); */
						pos = lchild_pos;
					}
					else
						break;
				}
			}
			else if (lchild_pos <= last_pos)
			{

				if (minheap->outposts[pos]->costOfOutpost > minheap->outposts[lchild_pos]->costOfOutpost)
				{
					/* printf("(index = %d, pos = %d)<-swap->(index = %d, pos = %d)\n", minheap->outposts[pos]->index, minheap->positions[minheap->outposts[pos]->index], minheap->outposts[lchild_pos]->index, minheap->positions[minheap->outposts[lchild_pos]->index]); */
					swap(&(minheap->outposts[pos]), &(minheap->outposts[lchild_pos]));
					minheap->positions[minheap->outposts[pos]->index] = pos;
					minheap->positions[minheap->outposts[lchild_pos]->index] = lchild_pos;
					/* printf("(index = %d, pos = %d)<------>(index = %d, pos = %d)\n\n", minheap->outposts[pos]->index, minheap->positions[minheap->outposts[pos]->index], minheap->outposts[lchild_pos]->index, minheap->positions[minheap->outposts[lchild_pos]->index]); */
					pos = lchild_pos;
				}
				else
					break;
			}
			else
				break;
		}
	}
	return;
}

void printElementsInMinHeap(MinHeap *minheap)
{
	if (isEmptyMinHeap(minheap))
	{
		puts("minheap = []");
		return;
	}
	printf("minheap = [\n");
	int i;
	for (i = 1; i < minheap->last_index; i++)
		printf("\t(index = %d, cost = %.3f, pos = %d),\n", minheap->outposts[i]->index, minheap->outposts[i]->costOfOutpost, minheap->positions[minheap->outposts[i]->index]);
	printf("\t(index = %d, cost = %.3f, pos = %d)]\n", minheap->outposts[i]->index, minheap->outposts[i]->costOfOutpost, minheap->positions[minheap->outposts[i]->index]);
	return;
}

void freeMinHeap(MinHeap *minheap)
{
	free(minheap->positions);
	free(minheap->outposts);
	free(minheap);
}

/* --------------------- */
/* Dijkstra's Algorithm. */
/* --------------------- */
MinHeap *initialSingleSource(Graph *graph, int start_index)
{
	struct Outpost *start_outpost = graph->outposts[start_index];
	start_outpost->costOfOutpost = 0;
	MinHeap *minheap = createMinHeap(graph->graphSize);
	int i;
	for (i = 0; i < graph->graphSize; i++)
		pushToMinHeap(minheap, graph->outposts[i]);

	/* printElementsInMinHeap(minheap); */
	bottomUpBuildMinHeap(minheap);
	/*
	printf("bottom-up ");
	printElementsInMinHeap(minheap);
	puts("");
	*/
	return minheap;
}

void decreaseKeyInMinHeap(MinHeap *minheap, struct Outpost *node)
{
	int pos = minheap->positions[node->index];

	int i;
	for (i = pos; i / 2 > 0 && minheap->outposts[i]->costOfOutpost < minheap->outposts[i / 2]->costOfOutpost; i /= 2)
	{
		swap(&(minheap->outposts[i]), &(minheap->outposts[i / 2]));
		minheap->positions[minheap->outposts[i]->index] = i;
		minheap->positions[minheap->outposts[i / 2]->index] = i / 2;
	}
	return;
}

void relaxNodesCostOfOutpost(struct Outpost *dst, double weight, MinHeap *minheap)
{
	if (dst->costOfOutpost > weight)
	{
		dst->costOfOutpost = weight;
		/* printElementsInMinHeap(minheap); */
		decreaseKeyInMinHeap(minheap, dst);
		/*
		printf("decreased ");
		printElementsInMinHeap(minheap);
		*/
	}
	return;
}

void PrimsAlgo(Graph *graph, int source_index)
{
	MinHeap *minheap = initialSingleSource(graph, source_index);

	int i;
	struct Outpost *cur_outpost;
	while (!isEmptyMinHeap(minheap))
	{
		cur_outpost = extractMinFromMinHeap(minheap);
		cur_outpost->isVisited = 1;
		/* printf("///// cur_outpost(index(%d), isVisited(%d))\n", cur_outpost->index, cur_outpost->isVisited); */
		for (i = 0; i <= cur_outpost->outpostsSize; i++)
		{
			/* printf("%d : adj_outpost(index(%d), isVisited(%d))\n", i, cur_outpost->adjacentOutposts[i]->index, cur_outpost->adjacentOutposts[i]->isVisited); */
			if (!cur_outpost->adjacentOutposts[i]->isVisited)
				relaxNodesCostOfOutpost(cur_outpost->adjacentOutposts[i], cur_outpost->distances[i], minheap);
			/* puts("relax completely.\n"); */
		}
	}
	freeMinHeap(minheap);
	return;
}

/* ----------------- */
/* Problem solution. */
/* ----------------- */

void printArray_double(double *nums, int numsSize)
{
	if (numsSize == 0)
	{
		puts("[]");
		return;
	}
	printf("[");
	int i;
	for (i = 0; i < numsSize - 1; i++)
	{
		if (nums[i] == INT_MIN)
			printf("null,");
		else
			printf("%.2f,", nums[i]);
	}
	printf("%.2f]", nums[numsSize - 1]);
	puts(" // completely print.");
	return;
}

double arcticNetwork(Graph *graph, int source_index, int satellite_channel_num, int outpost_num)
{
	int i;
	double buffer[OUTPOSTLIMIT] = {0};
	PrimsAlgo(graph, source_index);
	for (i = 0; i < graph->graphSize; i++)
		buffer[i] = graph->outposts[i]->costOfOutpost;
	/* printArray_double(buffer, graph->graphSize); */
	qsort(buffer, graph->graphSize, sizeof(buffer[0]), cmpFunction);
	/* printArray_double(buffer, graph->graphSize); */
	return buffer[outpost_num - satellite_channel_num];
}

int main()
{
	/*
	FILE *pfile_r, *pfile_w;
	if ((pfile_r = fopen("10369.txt", "r")) == NULL)
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
	int N, S, P, x, y;
	int index_x, index_y, cost;
	/* fscanf(pfile_r, "%d", &N); */
	scanf("%d", &N);
	for (i = 1; i <= N; i++)
	{
		/* fscanf(pfile_r, "%d %d", &S, &P); */
		scanf("%d %d", &S, &P);
		int graphSize = OUTPOSTLIMIT;
		Graph *graph = createGraph(graphSize);
		for (j = 0; j < P; j++)
		{
			/* fscanf(pfile_r, "%d %d", &x, &y); */
			scanf("%d %d", &x, &y);
			struct Outpost *new_outpost = createNewOutpost(graph->graphSize, x, y);
			graph->outposts[(graph->graphSize)++] = new_outpost;
		}
		for (j = 0; j < graph->graphSize; j++)
			for (k = j + 1; k < graph->graphSize; k++)
				linkOutposts(graph->outposts[j], graph->outposts[k]);
		/* printElementsInGraph(graph); */

		double res = arcticNetwork(graph, 0, S, P);
		printf("%.2f\n", res);
		freeGraph(graph);
		scanf("%c", &c);
	}

	return 0;
}
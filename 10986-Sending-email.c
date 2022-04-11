#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define OO 1000000001

/* -------------------- */
/* Definition for Node. */
/* -------------------- */
struct Node
{
	int index, costFromStartNode;
	int adjNodesSize;
	int *adjNodeWeights;
	struct Node **adjNode;
};

struct Node *createNewNode(int size, int index)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->index = index;
	newNode->costFromStartNode = OO;
	newNode->adjNodesSize = -1;
	newNode->adjNodeWeights = (int *)malloc(size * sizeof(int));
	newNode->adjNode = (struct Node **)malloc(size * sizeof(struct Node *));
	return newNode;
}

void linkTwoNodes(struct Node *parent, struct Node *child, int cost)
{
	(parent->adjNodesSize)++;
	parent->adjNodeWeights[parent->adjNodesSize] = cost;
	parent->adjNode[parent->adjNodesSize] = child;
	return;
}

void swap(struct Node **a, struct Node **b)
{
	struct Node *tmp = *a;
	*a = *b;
	*b = tmp;
	return;
}

/* -------------------- */
/* Definition for Edge. */
/* -------------------- */
typedef struct edgeInformation
{
	int index1, index2, weight;
} Edge;

Edge **createEdges(int edgesSize)
{
	Edge **edges = (Edge **)malloc(edgesSize * sizeof(Edge *));
	int i;
	for (i = 0; i < edgesSize; i++)
		edges[i] = (Edge *)malloc(sizeof(Edge));
	return edges;
}

void freeEdges(Edge **edges, int edgesSize)
{
	int i;
	for (i = 0; i < edgesSize; i++)
		free(edges[i]);
	free(edges);
	return;
}

/* -------------------- */
/* Definition for Graph. */
/* -------------------- */
typedef struct graphInformation
{
	int nodesSize;
	struct Node **nodes;
} Graph;

Graph *createGraph(int nodesNum, Edge **edges, int edgesNum)
{
	Graph *graph = (Graph *)malloc(sizeof(Graph));

	int i;
	graph->nodesSize = nodesNum;
	graph->nodes = (struct Node **)malloc(nodesNum * sizeof(struct Node *));
	for (i = 0; i < nodesNum; i++)
		graph->nodes[i] = NULL;

	for (i = 0; i < edgesNum; i++)
	{
		int src = edges[i]->index1;
		int dst = edges[i]->index2;
		if (graph->nodes[src] == NULL)
			graph->nodes[src] = createNewNode(edgesNum, src);
		if (graph->nodes[dst] == NULL)
			graph->nodes[dst] = createNewNode(edgesNum, dst);
		linkTwoNodes(graph->nodes[src], graph->nodes[dst], edges[i]->weight);
		linkTwoNodes(graph->nodes[dst], graph->nodes[src], edges[i]->weight);
	}

	return graph;
}

void freeGraph(Graph *graph)
{
	int i;
	for (i = 0; i < graph->nodesSize; i++)
		free(graph->nodes[i]);
	free(graph->nodes);
	free(graph);
	return;
}

/* ------------------------ */
/* Definition for min-heap. */
/* ------------------------ */
typedef struct minHeapInformation
{
	int last_index;
	int *positions;
	struct Node **nodes;
} MinHeap;

MinHeap *createMinHeap(int size)
{
	MinHeap *minheap = (MinHeap *)malloc(sizeof(MinHeap));
	minheap->last_index = 0;
	minheap->positions = (int *)malloc((size + 1) * sizeof(int));
	minheap->nodes = (struct Node **)malloc((size + 1) * sizeof(struct Node *));
	return minheap;
}

_Bool isEmptyMinHeap(MinHeap *minheap)
{
	return minheap->last_index <= 0 ? 1 : 0;
}

void pushToMinHeap(MinHeap *minheap, struct Node *node)
{
	(minheap->last_index)++;
	minheap->positions[node->index] = minheap->last_index;
	minheap->nodes[minheap->last_index] = node;
	return;
}

void insertToMinHeap(MinHeap *minheap, struct Node *node)
{
	int pos;
	(minheap->last_index)++;
	minheap->positions[node->index] = minheap->last_index;
	minheap->nodes[minheap->last_index] = node;
	for (pos = minheap->last_index; pos / 2 > 0 && minheap->nodes[pos]->costFromStartNode < minheap->nodes[pos / 2]->costFromStartNode; pos /= 2)
	{
		swap(&(minheap->nodes[pos]), &(minheap->nodes[pos / 2]));
		minheap->positions[minheap->nodes[pos]->index] = pos;
		minheap->positions[minheap->nodes[pos / 2]->index] = pos / 2;
	}
	return;
}

struct Node *extractMinFromMinHeap(MinHeap *minheap)
{
	if (isEmptyMinHeap(minheap))
		return NULL;

	struct Node *ret = minheap->nodes[1];
	minheap->nodes[1] = minheap->nodes[minheap->last_index];
	minheap->positions[minheap->nodes[1]->index] = 1;
	(minheap->last_index)--;
	int pos, last_pos = minheap->last_index;
	for (pos = 1; pos <= last_pos;)
	{
		int lchild_pos = pos * 2;
		int rchild_pos = pos * 2 + 1;
		if (rchild_pos <= last_pos)
		{
			if (minheap->nodes[lchild_pos]->costFromStartNode > minheap->nodes[rchild_pos]->costFromStartNode)
			{
				if (minheap->nodes[pos]->costFromStartNode > minheap->nodes[rchild_pos]->costFromStartNode)
				{
					swap(&(minheap->nodes[pos]), &(minheap->nodes[rchild_pos]));
					minheap->positions[minheap->nodes[pos]->index] = pos;
					minheap->positions[minheap->nodes[rchild_pos]->index] = rchild_pos;
					pos = rchild_pos;
				}
				else
					return ret;
			}
			else
			{
				if (minheap->nodes[pos]->costFromStartNode > minheap->nodes[lchild_pos]->costFromStartNode)
				{
					swap(&(minheap->nodes[pos]), &(minheap->nodes[lchild_pos]));
					minheap->positions[minheap->nodes[pos]->index] = pos;
					minheap->positions[minheap->nodes[lchild_pos]->index] = lchild_pos;
					pos = lchild_pos;
				}
				else
					return ret;
			}
		}
		else if (lchild_pos <= last_pos)
		{
			if (minheap->nodes[pos]->costFromStartNode > minheap->nodes[lchild_pos]->costFromStartNode)
			{
				swap(&(minheap->nodes[pos]), &(minheap->nodes[lchild_pos]));
				minheap->positions[minheap->nodes[pos]->index] = pos;
				minheap->positions[minheap->nodes[lchild_pos]->index] = lchild_pos;
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
				if (minheap->nodes[lchild_pos]->costFromStartNode > minheap->nodes[rchild_pos]->costFromStartNode)
				{
					if (minheap->nodes[pos]->costFromStartNode > minheap->nodes[rchild_pos]->costFromStartNode)
					{
						/* printf("(index = %d, pos = %d)<-swap->(index = %d, pos = %d)\n", minheap->nodes[pos]->index, minheap->positions[minheap->nodes[pos]->index], minheap->nodes[rchild_pos]->index, minheap->positions[minheap->nodes[rchild_pos]->index]); */
						swap(&(minheap->nodes[pos]), &(minheap->nodes[rchild_pos]));
						minheap->positions[minheap->nodes[pos]->index] = pos;
						minheap->positions[minheap->nodes[rchild_pos]->index] = rchild_pos;
						/* printf("(index = %d, pos = %d)<------>(index = %d, pos = %d)\n\n", minheap->nodes[pos]->index, minheap->positions[minheap->nodes[pos]->index], minheap->nodes[rchild_pos]->index, minheap->positions[minheap->nodes[rchild_pos]->index]); */
						pos = rchild_pos;
					}
					else
						break;
				}
				else
				{
					if (minheap->nodes[pos]->costFromStartNode > minheap->nodes[lchild_pos]->costFromStartNode)
					{
						/* printf("(index = %d, pos = %d)<-swap->(index = %d, pos = %d)\n", minheap->nodes[pos]->index, minheap->positions[minheap->nodes[pos]->index], minheap->nodes[lchild_pos]->index, minheap->positions[minheap->nodes[lchild_pos]->index]); */
						swap(&(minheap->nodes[pos]), &(minheap->nodes[lchild_pos]));
						minheap->positions[minheap->nodes[pos]->index] = pos;
						minheap->positions[minheap->nodes[lchild_pos]->index] = lchild_pos;
						/* printf("(index = %d, pos = %d)<------>(index = %d, pos = %d)\n\n", minheap->nodes[pos]->index, minheap->positions[minheap->nodes[pos]->index], minheap->nodes[lchild_pos]->index, minheap->positions[minheap->nodes[lchild_pos]->index]); */
						pos = lchild_pos;
					}
					else
						break;
				}
			}
			else if (lchild_pos <= last_pos)
			{

				if (minheap->nodes[pos]->costFromStartNode > minheap->nodes[lchild_pos]->costFromStartNode)
				{
					/* printf("(index = %d, pos = %d)<-swap->(index = %d, pos = %d)\n", minheap->nodes[pos]->index, minheap->positions[minheap->nodes[pos]->index], minheap->nodes[lchild_pos]->index, minheap->positions[minheap->nodes[lchild_pos]->index]); */
					swap(&(minheap->nodes[pos]), &(minheap->nodes[lchild_pos]));
					minheap->positions[minheap->nodes[pos]->index] = pos;
					minheap->positions[minheap->nodes[lchild_pos]->index] = lchild_pos;
					/* printf("(index = %d, pos = %d)<------>(index = %d, pos = %d)\n\n", minheap->nodes[pos]->index, minheap->positions[minheap->nodes[pos]->index], minheap->nodes[lchild_pos]->index, minheap->positions[minheap->nodes[lchild_pos]->index]); */
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
		printf("\t(index = %d, cost = %d, pos = %d),\n", minheap->nodes[i]->index, minheap->nodes[i]->costFromStartNode, minheap->positions[minheap->nodes[i]->index]);
	printf("\t(index = %d, cost = %d, pos = %d)]\n", minheap->nodes[i]->index, minheap->nodes[i]->costFromStartNode, minheap->positions[minheap->nodes[i]->index]);
	return;
}

void freeMinHeap(MinHeap *minheap)
{
	free(minheap->positions);
	free(minheap->nodes);
	free(minheap);
	return;
}

/* --------------------- */
/* Dijkstra's Algorithm. */
/* --------------------- */
MinHeap *initialSingleSource(Graph *graph, int start_index)
{
	graph->nodes[start_index]->costFromStartNode = 0;
	MinHeap *minheap = createMinHeap(graph->nodesSize);
	int i;

	for (i = 0; i < graph->nodesSize; i++)
		if (graph->nodes[i] != NULL)
			pushToMinHeap(minheap, graph->nodes[i]);

	/* printElementsInMinHeap(minheap); //*/
	bottomUpBuildMinHeap(minheap);
	/*
	printf("heapified ");
	printElementsInMinHeap(minheap);
	*/
	return minheap;
}

void decreaseKeyInMinHeap(MinHeap *minheap, struct Node *node)
{
	int pos = minheap->positions[node->index];

	int i;
	for (i = pos; i / 2 > 0 && minheap->nodes[i]->costFromStartNode < minheap->nodes[i / 2]->costFromStartNode; i /= 2)
	{
		swap(&(minheap->nodes[i]), &(minheap->nodes[i / 2]));
		minheap->positions[minheap->nodes[i]->index] = i;
		minheap->positions[minheap->nodes[i / 2]->index] = i / 2;
	}
	return;
}

void relaxNodesCostFromStartNode(struct Node *src, struct Node *dst, int weight, MinHeap *minheap)
{
	if (dst->costFromStartNode > src->costFromStartNode + weight)
	{
		dst->costFromStartNode = src->costFromStartNode + weight;
		/* printElementsInMinHeap(minheap); */
		decreaseKeyInMinHeap(minheap, dst);
		/*
		printf("decreased ");
		printElementsInMinHeap(minheap);
		*/
	}
	return;
}

void DijkstraAlgo(Graph *graph, int source_index)
{
	MinHeap *minheap = initialSingleSource(graph, source_index);

	int i;
	struct Node *cur_node;
	while (!isEmptyMinHeap(minheap))
	{
		cur_node = extractMinFromMinHeap(minheap);
		for (i = 0; i <= cur_node->adjNodesSize; i++)
			relaxNodesCostFromStartNode(cur_node, cur_node->adjNode[i], cur_node->adjNodeWeights[i], minheap);
	}
	return;
}

/* ----------------- */
/* Problem solution. */
/* ----------------- */
int sendingEmail(Graph *graph, int source_index, int terminal_index)
{
	DijkstraAlgo(graph, source_index);
	return graph->nodes[terminal_index]->costFromStartNode;
}

int main()
{
	/*
	FILE *pfile_r, *pfile_w;
	if ((pfile_r = fopen("10986.txt", "r")) == NULL)
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
	int N, n, m, S, T;
	int index_x, index_y, cost;
	/* fscanf(pfile_r, "%d", &N); //*/
	scanf("%d", &N);
	for (i = 1; i <= N; i++)
	{
		/* fscanf(pfile_r, "%d %d %d %d", &n, &m, &S, &T); //*/
		scanf("%d %d %d %d", &n, &m, &S, &T);
		Edge **edges = createEdges(m);
		for (j = 0; j < m; j++)
		{
			/* fscanf(pfile_r, "%d %d %d\n", &index_x, &index_y, &cost); //*/
			scanf("%d %d %d", &index_x, &index_y, &cost);
			edges[j]->index1 = index_x;
			edges[j]->index2 = index_y;
			edges[j]->weight = cost;
		}
		Graph *graph = createGraph(n, edges, m);
		if (graph->nodes[S] == NULL || graph->nodes[T] == NULL)
		{
			printf("Case #%d: unreachable\n", i);
			/* fprintf(pfile_w, "Case #%d: unreachable\n", i); */
		}
		else
		{
			int send_cost = sendingEmail(graph, S, T);
			if (send_cost >= OO)
			{
				printf("Case #%d: unreachable\n", i);
				/* fprintf(pfile_w, "Case #%d: unreachable\n", i); */
			}
			else
			{
				printf("Case #%d: %d\n", i, send_cost);
				/* fprintf(pfile_w, "Case #%d: %d\n", i, send_cost); */
			}
		}
		scanf("%c", &c);
	}

	return 0;
}
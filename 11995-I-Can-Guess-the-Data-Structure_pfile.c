#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

typedef struct stackInformation
{
	int top;
	int *data;
} Stack;

Stack *createStack(int size)
{
	Stack *sta = (Stack *)malloc(sizeof(Stack));
	sta->top = -1;
	sta->data = (int *)malloc(size * sizeof(int));
	return sta;
}

_Bool isEmptyStack(Stack *sta)
{
	return sta->top == -1 ? 1 : 0;
}

void pushToStack(Stack *sta, int data)
{
	(sta->top)++;
	sta->data[sta->top] = data;
	return;
}

int popFromStack(Stack *sta)
{
	if (isEmptyStack(sta))
		return -1;
	(sta->top)--;
	return sta->data[(sta->top) + 1];
}

void freeStack(Stack *sta)
{
	free(sta->data);
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
		printf("%d,", sta->data[i]);
	printf("%d]\n", sta->data[i]);
	return;
}

typedef struct QueueInformation
{
	int front, rear;
	int *data;
} Queue;

Queue *createQueue(int size)
{
	Queue *que = (Queue *)malloc(sizeof(Queue));
	que->front = -1;
	que->rear = -1;
	que->data = (int *)malloc(size * sizeof(int));
	return que;
}

_Bool isEmptyQueue(Queue *que)
{
	return que->front == que->rear ? 1 : 0;
}

void enQueueFromRear(Queue *que, int data)
{
	(que->rear)++;
	que->data[que->rear] = data;
	return;
}

int deQueueFromFront(Queue *que)
{
	if (isEmptyQueue(que))
		return -1;
	(que->front)++;
	return que->data[(que->front) - 1];
}

void freeQueue(Queue *que)
{
	free(que->data);
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
		printf("%d,", que->data[i]);
	printf("%d]\n", que->data[que->rear]);
	return;
}

typedef struct priorityQueueInformation
{
	int last_index;
	int *data;
} PriorityQueue;

PriorityQueue *createPriorityQueue(int size)
{
	PriorityQueue *prique = (PriorityQueue *)malloc(sizeof(PriorityQueue));
	prique->last_index = 0;
	prique->data = (int *)malloc((size + 1) * sizeof(int));
	return prique;
}

_Bool isEmptyPriorityQueue(PriorityQueue *prique)
{
	return prique->last_index == 0 ? 1 : 0;
}

void enPriorityQueue(PriorityQueue *prique, int data)
{
	int pos;
	(prique->last_index)++;
	prique->data[prique->last_index] = data;
	for (pos = prique->last_index; pos / 2 > 0 && prique->data[pos] > prique->data[pos / 2]; pos /= 2)
		swap(&(prique->data[pos]), &(prique->data[pos / 2]));
	return;
}

int dePriorityQueue(PriorityQueue *prique)
{
	if (isEmptyPriorityQueue(prique))
		return -1;

	int ret = prique->data[1];
	prique->data[1] = prique->data[prique->last_index];
	(prique->last_index)--;
	int pos, last_pos = prique->last_index;
	for (pos = 1; pos <= last_pos;)
	{
		int lchild_pos = pos * 2;
		int rchild_pos = pos * 2 + 1;
		if (rchild_pos <= last_pos)
		{
			if (prique->data[lchild_pos] < prique->data[rchild_pos])
			{
				if (prique->data[pos] < prique->data[rchild_pos])
				{
					swap(&(prique->data[pos]), &(prique->data[rchild_pos]));
					pos = rchild_pos;
				}
				else
					return ret;
			}
			else
			{
				if (prique->data[pos] < prique->data[lchild_pos])
				{
					swap(&(prique->data[pos]), &(prique->data[lchild_pos]));
					pos = lchild_pos;
				}
				else
					return ret;
			}
		}
		else if (lchild_pos <= last_pos)
		{
			if (prique->data[pos] < prique->data[lchild_pos])
			{
				swap(&(prique->data[pos]), &(prique->data[lchild_pos]));
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

void printElementsInPriorityQueue(PriorityQueue *prique)
{
	if (isEmptyPriorityQueue(prique))
	{
		puts("prique = []");
		return;
	}
	printf("prique = [");
	int i;
	for (i = 1; i < prique->last_index; i++)
		printf("%d, ", prique->data[i]);
	printf("%d]\n", prique->data[prique->last_index]);
	return;
}

void freePriorityQueue(PriorityQueue *prique)
{
	free(prique->data);
	free(prique);
	return;
}

int main()
{
	FILE *pfile_r, *pfile_w, *pfile_w_detail;
	if ((pfile_r = fopen("11995.txt", "r")) == NULL)
	{
		puts("Read file could not be opened!");
		return 0;
	}
	if ((pfile_w = fopen("output.txt", "w")) == NULL)
	{
		puts("Write file could not be opened!");
		return 0;
	}
	if ((pfile_w_detail = fopen("output_detail.txt", "w")) == NULL)
	{
		puts("Write file could not be opened!");
		return 0;
	}

	int n, operation, data, i, tmp, ctr = 1;
	while (!feof(pfile_r))
	{
		fscanf(pfile_r, "%d\n", &n);

		/* printf("////// n = %d\n", n); */
		Stack *sta = createStack(n);
		Queue *que = createQueue(n);
		PriorityQueue *prique = createPriorityQueue(n);
		_Bool is_stack = 1, is_queue = 1, is_prique = 1;
		for (i = 0; i < n; i++)
		{
			fscanf(pfile_r, "%d %d", &operation, &data);
			if (operation == 1)
			{
				/* printf("throw    \"%2d\" to the bag.\n", data); */
				if (is_stack)
					pushToStack(sta, data);
				if (is_queue)
					enQueueFromRear(que, data);
				if (is_prique)
					enPriorityQueue(prique, data);
			}
			else
			{
				/* printf("take out \"%2d\" from the bag.\n", data); */
				if (is_stack)
					if (isEmptyStack(sta) || sta->data[sta->top] != data)
						is_stack = 0;
					else
						popFromStack(sta);
				if (is_queue)
					if (isEmptyQueue(que) || que->data[(que->front) + 1] != data)
						is_queue = 0;
					else
						deQueueFromFront(que);
				if (is_prique)
					if (isEmptyPriorityQueue(prique) || prique->data[1] != data)
						is_prique = 0;
					else
						dePriorityQueue(prique);
			}
			/*
			if (is_stack)
				printElementsInStack(sta);
			else
				puts("not stack");
			if (is_queue)
				printElementsInQueue(que);
			else
				puts("not queue");
			if (is_prique)
				printElementsInPriorityQueue(prique);
			else
				puts("not priority queue");
			puts("");
			*/
		}
		freeStack(sta);
		freeQueue(que);
		freePriorityQueue(prique);

		if (!is_stack && !is_queue && !is_prique)
		{
			puts("impossible");
			fprintf(pfile_w, "impossible\n");
			fprintf(pfile_w_detail, "%3d: impossible, isStack = %d, isQueue = %d, isPrique = %d\n", ctr, is_stack, is_queue, is_prique);
		}
		else if (is_stack && !is_queue && !is_prique)
		{
			puts("stack");
			fprintf(pfile_w, "stack\n");
			fprintf(pfile_w_detail, "%3d: stack, isStack = %d, isQueue = %d, isPrique = %d\n", ctr, is_stack, is_queue, is_prique);
		}
		else if (!is_stack && is_queue && !is_prique)
		{
			puts("queue");
			fprintf(pfile_w, "queue\n");
			fprintf(pfile_w_detail, "%3d: queue, isStack = %d, isQueue = %d, isPrique = %d\n", ctr, is_stack, is_queue, is_prique);
		}
		else if (!is_stack && !is_queue && is_prique)
		{
			puts("priority queue");
			fprintf(pfile_w, "priority queue\n");
			fprintf(pfile_w_detail, "%3d: priority queue, isStack = %d, isQueue = %d, isPrique = %d\n", ctr, is_stack, is_queue, is_prique);
		}
		else
		{
			puts("not sure");
			fprintf(pfile_w, "not sure\n");
			fprintf(pfile_w_detail, "%3d: not sure, isStack = %d, isQueue = %d, isPrique = %d\n", ctr, is_stack, is_queue, is_prique);
		}
		ctr++;
	}

	return 0;
}
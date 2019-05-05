/*	Francisco Sena 	86420	al142	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define 	min(u, v) 			  (u<v ? u:v)


typedef struct node 
{
	int index;
	int capacity;
	struct node *next;
	} *link;


void init();
link insertBegin(int, link, int);
void insertEdge(int, int, int);
int  EdmondsKarp();
int  BFS();
void enqueue(int data);
int  dequeue();
int  isEmpty();
void reset_queue();
int  getCapacity(int, int);
void changeCapacity(int, int, int);


link 		t;
int 		V, linhas, colunas, source, target, i, res;
link 		*graph;
int 		*SP_values;
int 		*source_connections;
int 		*parentsList;  
int 		*currentPathCapacity;  

int 	*queue;
int 	front = 0;
int 	rear = -1;
int 	itemCount = 0;
int 	check_for_path = 0;


int main() 
{

	int j, k, aux, mini;


	scanf("%d %d", &linhas, &colunas);

	if ( (int) linhas != linhas || linhas < 1)
		return 0;
	if ( (int) colunas != colunas || colunas < 1)
		return 0;


	V = (linhas * colunas) + 2;

	source = 0;
	target = V-1;

	init();

	for (i = 0, k = 1; i < linhas; i++)
		for (j = 0; j < colunas; j++) {
			scanf("%d", &aux);
			insertEdge(source, k, aux);
			source_connections[k-1] = aux;
			k++;
		}


	for (i = 0, k = 1; i < linhas; i++)
		for (j = 0; j < colunas; j++) {
			scanf("%d", &aux);
			mini = min(aux, source_connections[k-1]);
			res += mini;
			insertEdge(k, target, (aux-mini));
			source_connections[k-1] -= mini;
			k++;
		}


	for (i = 0, k = 1; i < linhas; i++) 
	{
		for (j = 0; j < colunas-1; j++) {
			scanf("%d", &aux);
			insertEdge(k, k+1, aux);
			insertEdge(k+1, k, aux);    
			k++;
		}
		k++;
	}

	for (i = 0, k = 1; i < linhas-1; i++) 
		for (j = 0; j < colunas; j++) {
			scanf("%d", &aux);
			insertEdge(k, k+colunas, aux);
			insertEdge(k+colunas, k, aux);  
			k++;
		}

	memset(SP_values, 0, sizeof(int) * (V-2));

	res += EdmondsKarp();
	printf("%d\n", res);

	check_for_path=1;
	BFS();

	free(currentPathCapacity);
	free(parentsList);

	for (i = 0; i < V-2; i++) 
	{
		if (i%colunas==0) { printf("\n"); }

		if (SP_values[i] == -1)
			printf("C ");
		else
			printf("P ");
	}
	printf("\n");
    return 0;
}
	


int EdmondsKarp() 
{

    int maxFlow = 0;

      while(1) {

        int flow = BFS();

        if (flow == 0) 

        {

            break;

        }

        maxFlow += flow;

        int currentNode = target;

        while(currentNode != source)

        {

            int previousNode = parentsList[currentNode];

            changeCapacity(previousNode, currentNode, flow);

            currentNode = previousNode;

        }

    }

    return maxFlow;
}



int BFS() 
{  

    memset(parentsList, -1, sizeof(int)*V);
    memset(currentPathCapacity, 0, sizeof(int)*V);

    reset_queue();

    enqueue(source);

    parentsList[source] = -2;
    currentPathCapacity[source] = 999;

    while (!isEmpty()) {

        int currentNode = dequeue();

        for (t = graph[currentNode]; t != NULL; t = t->next) 
        {

            int to = t->index;

            if (parentsList[to] == -1) {
            	
                if (getCapacity(currentNode, to) > 0) {

                    parentsList[to] = currentNode;
                    currentPathCapacity[to] = min(currentPathCapacity[currentNode], getCapacity(currentNode, to));
                    
                    if (check_for_path)
                    	SP_values[to-1] = -1;

                    if (to == target) 
                        return currentPathCapacity[target];

                    enqueue(to);

                }

            }

        }

    }

	return 0;
} 

void init() {
	graph 				= 		  malloc(V * sizeof(link));
	SP_values   		= (int *) malloc((V-2) * sizeof(int));
	parentsList			= (int *) malloc(V * sizeof(int));
	currentPathCapacity = (int *) malloc(V * sizeof(int));
	queue 				= (int *) malloc(V * sizeof(int));
	source_connections	= (int *) malloc((V-2) * sizeof(int));
}

link insertBegin(int v, link head, int cap) {
	link x = malloc(sizeof(struct node));
	x->index = v;
	x->capacity = cap;
	x->next = head;
	return x;
} 

void insertEdge(int u, int v, int x) {
	graph[u] = insertBegin(v, graph[u], x);
}

int dequeue() {
   int data = queue[front++];
	
   if(front == V) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}

void enqueue(int data) {

   if(!(itemCount == V)) {
	
      if(rear == V-1) {
         rear = -1;            
      }       

      queue[++rear] = data;
      itemCount++;
   }
}

int isEmpty() {
   return itemCount == 0;
}

void reset_queue() {
	memset(queue, -1, sizeof(int)*V);
	front = 0;
	rear = -1;
	itemCount = 0;
}

int getCapacity(int x, int y) {
	if (x == 0) {
		return source_connections[y-1];
	}
	else 
	{
	for (t = graph[x]; t != NULL; t = t->next) 
		if (t->index == y)
			return t->capacity;
	}

	return -1;
}

void changeCapacity(int x, int y, int new) {

	if (x == 0) {
		source_connections[y-1] -= new;
	}
	
	else
	{
	for (t = graph[x]; t != NULL; t = t->next) 
		if (t->index == y)
			t->capacity -= new;

	for (t = graph[y]; t != NULL; t = t->next) 
		if (t->index == x)
			t->capacity += new;
	}
}

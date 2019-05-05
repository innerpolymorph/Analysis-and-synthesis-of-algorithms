/*	Francisco Sena	86420	al142	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define 	min(u, v) 		(u<v ? u:v)


typedef struct Edge {
	int u;
	int v;
	} *edge;

typedef struct vertex_v {
	int boolean;
	int d;
	int h;
	int scc_val;
	} *vertex;

typedef struct node {
	int index;
	struct node *next;
	} *link;


edge createEdge(int, int);
link insertBegin(int, link);
void init(int, int);
void insertEdge(edge);
int  cmpfunc(const void *, const void *);
int  pop();
void push(int);
void Tarjan_Visit(int);


int 	N, M;
edge 	edges, scc_connections;
vertex  *vertexes;
int 	num_SCC, visited = 0;
int 	*stack;
int 	top = -1;
link 	*adj;


int main(int argc, char** argv){

	int i, x, y, num_connections = 0;

	scanf("%d", &N);
	scanf("%d", &M);
	if ( (int) N != N || N<2)
		return 0;
	if ( (int) M != M || M<1)
		return 0;

	init(N, M);

	memset(vertexes, 0, (size_t) sizeof(vertex)*N);

	for (i=0; i<M; i++){
		scanf("%d %d", &x, &y);
		x--;
		y--;
		edges[i].u = x;
		edges[i].v = y;
		insertEdge(&edges[i]);
	}

	for (i=0; i<N; i++){
			vertexes[i] = (vertex) malloc(sizeof(struct vertex_v));
			vertexes[i]->d = -1;
			vertexes[i]->boolean = 0;
		}

	/* TARJAN */
	stack 	= (int *) 	 malloc(sizeof(int) * N);
	for (i = 0; i < N; i++)
		if ( (vertexes[i]->d) == -1){
			Tarjan_Visit(i);	
		}

	free(stack);
	free(adj);

	scc_connections = malloc(sizeof(struct Edge)*M);

	qsort(edges, M, sizeof(edge), cmpfunc);

	/* FIND_CONNECTIONS_BETWEEN_SCC */
	edge lastEdge = createEdge(-1, -1);
	for(i=0; i<M; i++) {
		if(lastEdge->u != vertexes[edges[i].u]->scc_val || lastEdge->v != vertexes[edges[i].v]->scc_val)
			if(vertexes[edges[i].u]->scc_val != vertexes[edges[i].v]->scc_val) {
				lastEdge->u = vertexes[edges[i].u]->scc_val;
				lastEdge->v = vertexes[edges[i].v]->scc_val;
				scc_connections[num_connections].u = vertexes[edges[i].u]->scc_val;
				scc_connections[num_connections].v = vertexes[edges[i].v]->scc_val;
				num_connections++;
			}
	}

	for (i = 0; i < N; i++)
		free(vertexes[i]);
	free(vertexes);
	free(edges);

	/* PRINT_OUTPUT */
	printf("%d\n", num_SCC);
	printf("%d\n", num_connections); 
	for (i = 0; i < num_connections; i++){ 
		printf("%d %d\n", (scc_connections[i].u)+1, (scc_connections[i].v)+1);
	}

	free(scc_connections);

	return 0;

}

/*------------------------------------------*/


edge createEdge(int x, int y){
	edge e = malloc(sizeof(struct Edge));
	e->u = x;
	e->v = y;
	return e;
}

link insertBegin(int v, link head){
	link x = malloc(sizeof(struct node));
	x->index = v;
	x->next = head;
	return x;
} 

void init(int V, int E){
	int i;
	vertexes 	= (vertex *) malloc(sizeof(vertex) * N);	
	edges 		= (edge)	 malloc(sizeof(struct Edge) * M);
	adj 		= 			 malloc(V * sizeof(link));
	for (i = 0; i < V; i++)
		adj[i] = NULL;
}

void insertEdge(edge e){
	int u = e->u;
	int v = e->v;
	adj[u] = insertBegin(v, adj[u]);
}

int pop(){
	int data;
	data = stack[top--];
    return data;
}

void push(int data){ stack[++top] = data; }

void Tarjan_Visit(int v){
	int w; 
	link t, aux;
	vertexes[v]->d = visited;
	vertexes[v]->h = visited;
	vertexes[v]->boolean = 1;
	int *temp;
	push(v);
	visited++;

	for (t = adj[v]; t != NULL; t = t->next){
		w = t->index;
		if ( vertexes[w]->d == -1 || vertexes[w]->boolean){
			if ( (vertexes[w])->d == -1)
				Tarjan_Visit(w);
			vertexes[v]->h = min(vertexes[v]->h, vertexes[w]->h);
		}
	}
	
	/* FREE ADJACÊNCIAS DE v */
	for (t = adj[v]; t != NULL; t = t->next){
		if (t->next == NULL){
			free(t);
		}
		else{
		aux = t;
		t = t->next;
		free(aux);}
	}

	if (vertexes[v]->d == vertexes[v]->h){
		int ct = 0;
		temp = (int *) malloc(sizeof(int)*(top+1));
		int x = -1;
		int scc_min_value = v;

		while (v != x){
			x = pop();
			vertexes[x]->boolean = 0;
			temp[ct++] = x;
			scc_min_value = min(scc_min_value, x);
		}

		x=0;
		while ( x != ct ) vertexes[temp[x++]]->scc_val = scc_min_value;

		num_SCC++;

		free(temp);
	}
}

int cmpfunc(const void * a, const void * b){
	edge e1 = (edge) a;
	edge e2 = (edge) b;
	if (vertexes[e1->u]->scc_val < vertexes[e2->u]->scc_val)
		return -1;
	if (vertexes[e1->u]->scc_val > vertexes[e2->u]->scc_val)
		return 1;
	if (vertexes[e1->v]->scc_val < vertexes[e2->v]->scc_val)
			return -1;
	return 1;
}

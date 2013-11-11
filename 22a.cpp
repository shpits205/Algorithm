#include <stdio.h>
#include <stdlib.h>                    // malloc(), free()

#define  N  10


struct elem {
	int key;                            // set member
	struct elem *next;                  // pointer to the object containing the next set member
	struct elem *rep;                   // pointer back to the representative
};

struct list {
	struct elem *head;                  // pointers to representative
	struct elem *tail;                  // pointers to the last object in the list
	int size;                           // length of the list
};


void Create_Edge(int [][N], int, int, int);
void Build_Graph(int [][N], int);
void Print_Adjacency_Matrix(int [][N], int);

void Make_Set(int);                    // creates a new set
void Union(int, int);                  // unites the dynamic sets into
//    a new set
struct elem* Find_Set(int);            // returns a pointer to the
//    representative of the set
void Connected_Components(void);       // compute the connected components
int  Same_Component(int, int);         // Are two vertices in the same component?
void Print_Disjoint_Sets(void);


int Adj[N][N];                         // adjacency matrix
int E[7][2] = {{1,3},{4,6},{0,2},{7,8},{0,1},{4,5},{1,2}};
struct elem *V[N];                     // array of vertices

struct list S[N];                      // collection S[0], S[1],..., S[N-1]
int    lengthS = N,                    //    of disjoint dynamic sets
	sizeS   = 0;

int main(void) {
	Build_Graph(Adj, N);
	Print_Adjacency_Matrix(Adj, N);
	Connected_Components();
	Print_Disjoint_Sets();

	if ( Same_Component(5, 7) )
		printf("Yes\n");
	else
		printf("No\n");
	system("pause");
	return 0;
}


// create undirected edge
void Create_Edge(int A[][N], int n, int u, int v) {
	A[u][v] = 1;
	A[v][u] = 1;
}


void Build_Graph(int A[][N], int n) {

	printf(" Figure 21.1                                                         \n");
	printf(" a --- b     e --- f     h     j     0 --- 1     4 --- 5     7     9 \n");
	printf(" |   / |     |           |           |   / |     |           |       \n");
	printf(" |  /  |     |           |           |  /  |     |           |       \n");
	printf(" | /   |     |           |           | /   |     |           |       \n");
	printf(" c     d     g           i           2     3     6           8       \n\n");

	Create_Edge(A, n, 0, 1);
	Create_Edge(A, n, 0, 2);
	Create_Edge(A, n, 1, 2);
	Create_Edge(A, n, 1, 3);
	Create_Edge(A, n, 4, 5);
	Create_Edge(A, n, 4, 6);
	Create_Edge(A, n, 7, 8);
	Create_Edge(A, n, 9, 9);
}


void Print_Adjacency_Matrix(int A[][N], int n) {
	int i, j;

	printf("   a b c d e f g h i j\n");
	for (i=0; i < n; i++) {
		printf("%c ", i + 'a');
		for (j=0; j < N; j++)
			printf("%2d", A[i][j]);
		printf("\n");
	}
	printf("\n");
}


// creates a new set whose only member (and thus representative) is x
void Make_Set(int x) {
	struct elem *p;

	p = (struct elem *) malloc(sizeof(struct elem));
	V[x] = p;

	V[x]->key = x;
	V[x]->next = NULL;
	V[x]->rep = V[x];
	S[x].head = V[x];
	S[x].tail = V[x];
	S[x].size = 1;
	sizeS++;
}


// returns a pointer to the representative of the (unique) set containing x
struct elem* Find_Set(int x) {
	if (V[x]->key != x)
		V[x] = Find_Set(V[x]->key);
	return V[x]->rep;
}


// unites the dynamic sets that contain x and y into a new set
//    that is the union of these two sets
// append x's list onto the end of y's list
void Union(int x, int y) {
	struct elem *xp;
	int i, xi=x, yi=y;

	for (i=0; i < sizeS; i++){
		if (S[i].head == V[x]->rep)
			xi = i;
		else if (S[i].head == V[y]->rep)
			yi = i;
	}

	if (S[yi].size < S[xi].size) {      // we append the smaller list
		i  = xi;                         //    onto the longer
		xi = yi;
		yi = i;
	}

	xp = S[xi].head;
	while ( xp != NULL ) {              // update the pointer to the
		xp -> rep = S[yi].head;          //    representive for each object
		xp        = xp -> next;          //    originally on x's list
	}

	S[yi].tail -> next = S[xi].head;    // append x's list onto the end of y's list
	S[yi].tail         = S[xi].tail;
	S[yi].size        += S[xi].size;    // increase the length of new list

	S[xi].head = NULL;                  // "destroy" set that contain x
	S[xi].tail = NULL;
	S[xi].size = 0;
}


// compute the connected components of a graph
void Connected_Components(void) {
	int i;
	for (i = 0; i < N; i++)
	{
		Make_Set(i);
	}

	for (i = 0; i < 7; i++)
	{
		if(Find_Set(E[i][0])!=Find_Set(E[i][1]))
		{
			Union(E[i][0],E[i][1]);
		}
	}
}


// answers queries about whether two vertices are
//    in the same connected component
int Same_Component(int u, int v) {
	if (Find_Set(u)==Find_Set(v))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Print_Disjoint_Sets(void) {
	int i;
	struct elem *h;

	for (i=0; i < sizeS; i++) {
		h = S[i].head;
		printf("S[%d]", i);
		while ( h != NULL ) {
			printf(" -> %c", 'a' + h->key);
			h = h->next;
		}
		printf("\n");
	}
}

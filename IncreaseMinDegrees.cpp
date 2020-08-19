/*-------------------------------------------------------------------------
 | Insert random edges in a digraph to increase the minimum in/out degrees
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <random>

using namespace std;

#define MAXLINE       100   /* max length of input line */
char line[MAXLINE]; /* stores input line */

int r, t;
int n, n2, m;
int narcs;

int *indegree;
int *outdegree;


list<int> **adj;  // adjacency list
list<int> **adj2;  // adjacency list 2

vector<pair<int, int> > arclist;
vector<pair<int, int> > arclist2;

inline void init(int N) {
	fprintf(stderr, "init %d\n", N);
	
	adj = new list<int>*[N];
	adj2 = new list<int>*[N];

	indegree = new int[N];
	outdegree = new int[N];

	for (int i = 0; i < N; i++) {
		adj[i] = new list<int>;
		adj2[i] = new list<int>;

		indegree[i] = outdegree[i] = 0;
	}
}

int removeDuplicatesL() {
	int *stack = new int[n + 1];
	int *index = new int[n + 1];
	int sp = 0;
	int arcs = 0;
	list<int>::iterator j;

	fprintf(stderr, "remove duplicates\n");

	for (int i = 0; i <= n; i++) {
		stack[i] = index[i] = 0;
	}

	for (int i = 1; i <= n; i++) {
		for (j = adj[i]->begin(); j != adj[i]->end(); j++) {
			if (!index[*j]) {
				stack[sp++] = (*j);
				index[*j] = 1;
			}
		}

		while (sp) {
			sp--;
			adj2[i]->push_back(stack[sp]);
			index[stack[sp]] = 0;
			arcs++;
		}
	}

	n2 = n;
	return arcs;
}

/* read graph from input file */
void readGraph(const char* file) {
	FILE* input = fopen(file, "r");
	if (!input) {
		fprintf(stderr, "Error opening file \"%s\".\n", file);
		exit(-1);
	}

	int x, y;
	pair<int, int> arc;

	fprintf(stderr, "Reading graph (%s).\n", file);

	while (fgets(line, MAXLINE, input) != NULL) {
		switch (line[0]) {
		case '\n':
			;
		case '\0':
			break;
		case 'p':
			if (sscanf(line, "p %d %d %d %d", &n, &m, &r, &t) != 4) {
				fprintf(stderr, "Error reading graph size (%s).\n", file);
				exit(-1);
			}
			init(n + 1);
			break;
		default:
			if (sscanf(line, "%d %d", &x, &y) != 2) {
				fprintf(stderr, "Error reading graph edge (%s).\n", file);
				exit(-1);
			}
			//assert(x<=n); assert(y<=n);
			adj[x]->push_back(y); // store arc (x,y) in adj
			arc.first = x;
			arc.second = y;
			arclist.push_back(arc);
			outdegree[x]++;
			indegree[y]++;
			narcs++;
			break;
		}
	}

	fclose(input);

	fprintf(stderr, "END graph (%s).\n", file);
}


inline void printDegrees() {
	fprintf(stderr, "Printing vertex degrees\n");
	for (int v = 1; v <= n; v++) {
		fprintf(stderr, "vertex %d: indegree = %d, outdegree = %d\n", v, indegree[v], outdegree[v]);
	}
}

void printGraph() {
	list<int>::iterator j;
	fprintf(stderr, "Printing graph adjacency lists\n");
	for (int v = 1; v <= n; v++) {
		fprintf(stderr, "vertex %d:", v);
		for (j = adj[v]->begin(); j != adj[v]->end(); j++) {
			fprintf(stderr, " %d", *j);
		}
		fprintf(stderr, "\n");
	}
}


void increaseMinDegrees(int din, int dout, int seed) {

	fprintf(stderr, "adding random edges to increase minimum in/out degrees\n");
	fprintf(stderr, "din = %d, dout = %d\n", din, dout);


	default_random_engine generator(seed);
	uniform_int_distribution<int> distribution(1, n);

	int added = 0;
	int from;
	int to;
	list<int>::iterator j;
	bool e_exists;

	for (int v = 1; v <= n; v++) {
		//fprintf(stderr, "vertex %d: indegree = %d, outdegree = %d\n", v, indegree[v], outdegree[v]);
		while (indegree[v] < din) {
			from = distribution(generator);
			if (v == from) continue;
			e_exists = false;
			for (j = adj[from]->begin(); j != adj[from]->end(); j++) {
				if (*j == v) {
					e_exists = true;
					break;
				}
			}
			if (e_exists) {
				continue;
			}
			adj[from]->push_back(v);
			added++;
			m++;
			indegree[v]++;
			outdegree[from]++;
		}
		//fprintf(stderr, "\t indegree = %d\n", indegree[v]);
		while (outdegree[v] < dout) {
			to = distribution(generator);
			if (v == to) continue;
			e_exists = false;
			for (j = adj[v]->begin(); j != adj[v]->end(); j++) {
				if (*j == to) {
					e_exists = true;
					break;
				}
			}
			if (e_exists) {
				continue;
			}
			adj[v]->push_back(to);
			added++;
			m++;
			outdegree[v]++;
			indegree[to]++;
		}
		//fprintf(stderr, "\t outdegree = %d\n", outdegree[v]);
	}
	//printDegrees();
}


void output() {
	pair<int, int> arc;
	vector<pair<int, int> >::iterator j;
	list<int>::iterator i;

	int carc = 0;
	narcs = removeDuplicatesL();

	int x, y;

	for (x = 1; x <= n2; x++)
		for (i = adj2[x]->begin(); i != adj2[x]->end(); i++) {
			y = *i;
			arc.first = x;
			arc.second = y;
			arclist2.push_back(arc);
			carc++;
		}

	fprintf(stderr, "carc = %d\n", carc);

	random_shuffle(arclist2.begin(), arclist2.end());

	fprintf(stdout, "p %d %d %d %d\n", n, narcs, r, t);
	for (j = arclist2.begin(); j < arclist2.end(); j++) {
		arc = *j;
		fprintf(stdout, "%d %d\n", arc.first, arc.second);
	}
}

// compute graph min degree
inline int computeDelta()
{
	int mindegree = n;
	for (int i = 1; i <= n; i++) {
		if (indegree[i] < mindegree) mindegree = indegree[i];
		if (outdegree[i] < mindegree) mindegree = outdegree[i];
	}
	return mindegree;
}

int main(int argc, char *argv[]) {
	

	if (argc != 5) {
		printf("\n usage: %s <input file> <minimum indegree> <minimum outdegree> <seed>\n\n", argv[0]);
		exit(-1);
	}

	
	char* file = argv[1];
	int din = atoi(argv[2]); 
	int dout = atoi(argv[3]);    
	int seed = atoi(argv[4]);
	narcs = 0;
	
	//srand(seed);
	readGraph(file);
	
	//printGraph();
	//printDegrees();

	increaseMinDegrees(din, dout, seed);
	output();
	
	fprintf(stderr, "\n number of nodes, arcs = %d, %d \n", n2, narcs);

	int delta = computeDelta();
	fprintf(stderr, "\n delta = %d \n", delta);
	//printGraph();
	//printDegrees();
	return 0;
}

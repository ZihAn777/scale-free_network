#include<iostream>
#include<stack>
#include<queue>
#include<fstream>
#include<cmath>
#include<algorithm>
using namespace std;

const int MAXN = 5000;
int N = 4039;
int G[MAXN][MAXN];  // the graph

struct Node {
	int deg, idx;
} nodes[MAXN];
int degs[MAXN], hub_nodes[MAXN];

struct Edge {
	double BC;
	int src, trg;
	Edge(double a, int b, int c)
		:BC(a), src(b), trg(c) { }
};

struct cmp {
	bool operator()(Edge a, Edge b) {
		return a.BC < b.BC;
	}
};

int dis[MAXN];
double e_bc[MAXN][MAXN], sigma[MAXN], delta[MAXN]; // value of edge bc
vector<int> fa[MAXN];  // father of the node

// union-find
int par[MAXN], rnk[MAXN];  //father node
int find(int x) {
	if (par[x] == x)
		return x;
	return par[x] = find(par[x]);
}
bool unite(int x, int y) {
	x = find(x);
	y = find(y);
	if (x == y) return false;
	if (rnk[x] > rnk[y]) par[y] = x, rnk[x] = (rnk[y] += rnk[x]);
	else par[x] = y, rnk[y] = (rnk[x] += rnk[y]);
	return true;
}

// BC approximation
void BC() {
	// calculate BC(e) from each node 
	for (int s = 0; s < N; s++) {
		if (hub_nodes[s] == 0)
			continue;  // not the hub node, don't count

		// initialization
		for (int i = 0; i < N; i++) {
			dis[i] = -1;  // unvisit yet
			fa[i].clear();
			sigma[i] = 0;  // connected-component: every node has at least 1 path
			delta[i] = 0;
		}
		queue<int> q;
		stack<int> stk;
		q.push(s);
		dis[s] = 0, sigma[s] = 1;


		// BFS
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			stk.push(v);

			for (int i = 0; i < N; i++) {
				// symmetrical & edge is existed
				if (G[v][i] == 1 && dis[i] == -1) {
					dis[i] = dis[v] + 1;
					q.push(i);
					fa[i].push_back(v);  // v can reach i
					sigma[i] = sigma[v];  // sigma[v] paths
				}
				else if (G[v][i] == 1 && dis[i] == dis[v] + 1) {
					// can reach from another node
					fa[i].push_back(v);  // v can reach i
					sigma[i] += sigma[v];  // 
				}
			}
		}
		
		// calculate
		while (!stk.empty()) {
			int v = stk.top();
			stk.pop();

			for (auto u : fa[v]) {
				delta[u] += (sigma[u] / sigma[v])*(1 + delta[v]);
				e_bc[v][u] += delta[u];
				e_bc[u][v] = e_bc[v][u];
			}
		}
	}
}



int main(void) {
	// load the data
	ifstream f_links("facebook-combined.txt");
	while (!f_links.eof()) {
		int src, trg;
		f_links >> src >> trg;  // input
		// keep symmetrical
		G[src][trg] = G[trg][src] = 1;
		nodes[src].deg++;  nodes[src].idx = src;
		nodes[trg].deg++;  nodes[trg].idx = trg;
		degs[src]++, degs[trg]++;
		// 
		par[trg] = trg, par[src] = src;
		rnk[trg] = rnk[src] = 1;
	}
	f_links.close();

	//sort the nodes by degree, and select the hubs
	sort(nodes, nodes + N,
		[](const Node x, const Node y) { return x.deg > y.deg; });
	// determine the hub nodes
	int num_hubs = 10 * log(N);  // c=10
	if (num_hubs < 50) num_hubs = 50;
	for (int i = 0; i < num_hubs; i++) {
		int idx = nodes[i].idx;
		hub_nodes[idx] = 1;
	}

	// calculate BC and build the priority_queue
	BC();
	priority_queue <Edge, vector<Edge>, cmp> pq;
	for (int i = 0; i < N; i++) {
		for (int j = i; j < N; j++) {
			if (G[i][j] == 1 || G[j][i] == 1)
				pq.push(Edge(e_bc[i][j], i, j));
		}
	}

	// remove the edge and record them
	stack<Edge> st_rec;
	while (!pq.empty()) {
		Edge etmp = pq.top();
		pq.pop();
		st_rec.push(etmp);  // record 
		if (degs[etmp.src] >= 2 && degs[etmp.trg] >= 2) {
			G[etmp.src][etmp.trg] = G[etmp.trg][etmp.src] = 0;
			degs[etmp.src]--;
			degs[etmp.trg]--;
		}
	}

	// reconnected
	for (int i = 0; i < N; i++) {
		for (int j = i; j < N; j++) {
			if (G[i][j] == 1) {
				unite(i, j);
			}
		}
	}
	while (!st_rec.empty()) {
		Edge tmp = st_rec.top();
		st_rec.pop();
		if (find(tmp.src) != find(tmp.trg)) {
			unite(tmp.src, tmp.trg);
			G[tmp.src][tmp.trg] = G[tmp.trg][tmp.src] = 1;
		}

	}

	// output 
	ofstream of("fb_newLink.json");
	bool comma = false;
	of << "[\n";
	for (int i = 0; i < N; i++) {
		for (int j = i; j < N; j++) {
			if (G[i][j] == 1) {
				if (comma) of << ",\n";
				of << "{ \"target\":" << i << ", \"source\": " << j << " }";
				comma = true;
			}
		}
	}
	of << "\n]";
	of.close();


	cout << "DONE!" << endl;

	system("pause");
	return 0;
}


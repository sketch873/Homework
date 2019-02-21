/*  Copyright 2018 Andrei - Costin Martin  */
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define kMax 100001

bool visited[kMax];

/*  Algoritmul BFS  */
vector<int> bfs(int node, vector<int> adjList[]) {
  vector<int> sol;
  queue<int> q;

  q.push(node);
  sol.push_back(node);
  visited[node] = true;

  while (!q.empty()) {
    int n = q.front();
    for (int i = 1; i < adjList[n].size(); i++) {
      if (visited[adjList[n][i]] == false) {
        sol.push_back(adjList[n][i]);
        visited[adjList[n][i]] = true;
        q.push(adjList[n][i]);
      }
    }
    q.pop();
  }
  return sol;
}

bool comp(int a, int b) {
  return a < b;
}

int main() {
  int n, m;
  int a, b;

  ifstream in("minlexbfs.in");

  in >> n >> m;

  vector<int> adjList[n+1];
  for (int i = 1; i <= n; i++) {
    adjList[i].push_back(-1);
  }
  for (int i = 1; i <= m; i++) {
    in >> a >> b;
    adjList[a].push_back(b);
    adjList[b].push_back(a);
  }

  in.close();

/*  Sortarea listei de adiacenta in ordine crescatoare  */
  for (int i = 1; i <= n; i++) {
    sort(adjList[i].begin(), adjList[i].end(), comp);
  }

  /*  Rezultatul final, parcurgerea bfs  */
  vector<int> sol = bfs(1, adjList);

  ofstream out("minlexbfs.out");

  for (int i = 0; i < sol.size(); i++) {
    out << sol[i] << " ";
  }

  return 0;
}

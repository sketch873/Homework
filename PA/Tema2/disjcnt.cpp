/*  Copyright 2018 Andrei - Costin Martin  */
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <set>
#include <list>

using namespace std;

#define kMax 100001

int n, m;
int timp;
int total;


/*  Explorarea determina daca legatura (u,v) este punte  */
void exploreaza(
      int u,
      vector<pair<int, bool> > adjList[],
      vector<bool> &visited,
      vector<int> &d,
      vector<int> &low,
      vector<int> &p) {
  /*  Begin  */
  d[u] = low[u] = timp++;
  visited[u] = true;

  for (int i = 1; i < adjList[u].size(); i++) {
    if (visited[adjList[u][i].first] == false) {
      p[adjList[u][i].first] = u;
      exploreaza(adjList[u][i].first, adjList, visited, d, low, p);
      low[u] = low[u] < low[adjList[u][i].first]
                ? low[u] : low[adjList[u][i].first];
      if (low[adjList[u][i].first] > d[u]) {
        adjList[u][i] = make_pair(adjList[u][i].first, true);
      }
    } else {
      if (p[u] != adjList[u][i].first)
        low[u] = low[u] < d[adjList[u][i].first] ?
                low[u] : d[adjList[u][i].first];
    }
  }
}

/*  Functia care initializeaza variabilele necesare
 *  pentru determinarea puntilor
 */
void punti(vector<pair<int, bool> > adjList[]) {
  vector<bool> visited(n+1, false);
  vector<int> d(n+1, 0);
  vector<int> low(n+1, 0);
  vector<int> p(n+1);
  timp = 0;

  /*  Explorez fiecare nod nevizitat  */
  for (int i = 1; i <= n; i++) {
    if (visited[i] == false) {
      exploreaza(i, adjList, visited, d, low, p);
    }
  }
}

void dfs(int u,
        vector<pair<int, bool> > adjList[],
        vector<bool> &visited, int *nr) {
  /*  Begin  */
  visited[u] = true;

  for (int i = 1; i < adjList[u].size(); i++) {
    if (adjList[u][i].second == false &&
      visited[adjList[u][i].first] == false) {
      (*nr)++;
      dfs(adjList[u][i].first, adjList, visited, nr);
    }
  }
}

void prepare_dfs(vector<pair<int, bool> > adjList[]) {
  vector<bool> visited(n+1, false);
  int no_elements;
  for (int i = 1; i <= n; i++)
    if (visited[i] == false) {
      no_elements = 1;
      dfs(i, adjList, visited, &no_elements);
      total += 1LL * no_elements * (no_elements - 1) / 2;
    }
}

int main() {
  int a, b;

  /*  Citirea din fisier  */
  ifstream in("disjcnt.in");
  in >> n >> m;

  /*  Lista de adiacenta care retine nodul succesor
   *  si daca legatura cu succesorul este puntile
   */
  vector<pair<int, bool> > adjList[n+1];

  for (int i = 1; i <= n; i++) {
    adjList[i].push_back(make_pair(-1, true));
  }

  for (int i = 1; i <= m; i++) {
    in >> a >> b;
    adjList[a].push_back(make_pair(b, false));
    adjList[b].push_back(make_pair(a, false));
  }
  in.close();

  /*  Determin toate puntile  */
  punti(adjList);

  /*  Fac dfs din fiecare nod  */
  prepare_dfs(adjList);

  ofstream out("disjcnt.out");
  out << total;
  out.close();

  return 0;
}

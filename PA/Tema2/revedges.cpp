/*  Copyright 2018 Andrei - Costin Martin  */
#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <climits>
using namespace std;

#define INF 1e5
#define kMax 501

int n, m, q;
int adj[kMax][kMax];

void floyd() {
  for (int k = 1; k <= n; k++) {
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (adj[i][j] > adj[i][k] + adj[k][j]) {
          adj[i][j] = adj[i][k] + adj[k][j];
      }  }
    }
  }
}

int main() {
  ifstream in("revedges.in");
  ofstream out("revedges.out");

  in >> n >> m >> q;
  int a, b;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++)
      adj[i][j] = INF;
    adj[i][i] = 0;
  }

  for (int i = 1; i <= m; i++) {
    in >> a >> b;
    adj[a][b] = 0;
    if (adj[b][a] != 0)
      adj[b][a] = 1;
  }


  /*  Aplic Roy-FLoyd pe graf  */
  floyd();


  /*  Pentru fiecare query intorc rezultatul  */
  for (int i = 1; i <= q; i++) {
    in >> a >> b;
    out << adj[a][b] << endl;
  }

  in.close();
  out.close();
}

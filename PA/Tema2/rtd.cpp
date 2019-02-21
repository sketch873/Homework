/*  Copyright 2018 Andrei - Costin Martin  */
#include <vector>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <fstream>
#include <set>
#include <climits>

using namespace std;

#define INF 1e5
#define kMax 251

int n, m, k;
int startX, startY;
int stopX, stopY;

/*  Tuplul cuprinde, in ordine, fata din fata a zarului, fata de sus
 *	si fata din dreapta.
 */
tuple<int, int, int> moveUp(tuple<int, int, int> t) {
	int a, b, c, aux;
	tie(a, b, c) = t;

	aux = a;
	a = 7 - b;
	b = aux;

	return make_tuple(a, b, c);
}

tuple<int, int, int> moveDown(tuple<int, int, int> t) {
	int a, b, c, aux;
	tie(a, b, c) = t;

	aux = a;
	a = b;
	b = 7 - aux;

	return make_tuple(a, b, c);
}

tuple<int, int, int> moveLeft(tuple<int, int, int> t) {
	int a, b, c, aux;
	tie(a, b, c) = t;

	aux = b;
	b = c;
	c = 7 - aux;

	return make_tuple(a, b, c);
}

tuple<int, int, int> moveRight(tuple<int, int, int> t) {
	int a, b, c, aux;
	tie(a, b, c) = t;

	aux = b;
	b = 7 - c;
	c = aux;

	return make_tuple(a, b, c);
}

/*  Functie de hash pentru configuratia zarului,
 *	In total sunt 24 de moduri, aranjate dupa fata de sus in ordine descrescatoare
*/
int hashh(tuple<int, int, int> t) {
	int a, b, c;
	tie(a, b, c) = t;

	if (a == 2 && b == 6 && c == 3) return 0;
	if (a == 4 && b == 6 && c == 2) return 1;
	if (a == 5 && b == 6 && c == 4) return 2;
	if (a == 3 && b == 6 && c == 5) return 3;

	if (a == 3 && b == 5 && c == 1) return 4;
	if (a == 6 && b == 5 && c == 3) return 5;
	if (a == 4 && b == 5 && c == 6) return 6;
	if (a == 1 && b == 5 && c == 4) return 7;

	if (a == 5 && b == 4 && c == 1) return 8;
	if (a == 6 && b == 4 && c == 5) return 9;
	if (a == 2 && b == 4 && c == 6) return 10;
	if (a == 1 && b == 4 && c == 2) return 11;

	if (a == 1 && b == 3 && c == 5) return 12;
	if (a == 2 && b == 3 && c == 1) return 13;
	if (a == 6 && b == 3 && c == 2) return 14;
	if (a == 5 && b == 3 && c == 6) return 15;

	if (a == 1 && b == 2 && c == 3) return 16;
	if (a == 4 && b == 2 && c == 1) return 17;
	if (a == 6 && b == 2 && c == 4) return 18;
	if (a == 3 && b == 2 && c == 6) return 19;

	if (a == 2 && b == 1 && c == 4) return 20;
	if (a == 3 && b == 1 && c == 2) return 21;
	if (a == 5 && b == 1 && c == 3) return 22;
	if (a == 4 && b == 1 && c == 5) return 23;
}

void bfs(int tabla[25][kMax][kMax], vector<int> v) {
	/*  Coada care retine indexul pozitiei curente si
	 *	noua configuratie a zarului
	 */
	queue<tuple<int, int, tuple<int, int, int> > > q;

	q.push(make_tuple(startX, startY, make_tuple(2, 6, 3)));
	tabla[hashh(make_tuple(2, 6, 3))][startX][startY] = v[1];

	while (! q.empty()) {
		tuple<int, int, int> p, newPos;
		int x, y, newHash, pHash;

		tie(x, y, p) = q.front();
		q.pop();

		pHash = hashh(p);

		/*  Pentru mutarea posibilitatea mutarii in sus  */
		newPos = moveUp(p);
		newHash = hashh(newPos);
		if (x+1 <= n &&
			tabla[newHash][x+1][y] > tabla[pHash][x][y] + v[7 - get<1>(newPos)] &&
			tabla[newHash][x+1][y] != -1) {
			/*  Then  */
			tabla[newHash][x+1][y] = tabla[pHash][x][y] + v[7 - get<1>(newPos)];
			q.push(make_tuple(x+1, y, newPos));
		}

		/*  Pentru mutarea posibilitatea mutarii in dreapta  */
		newPos = moveRight(p);
		newHash = hashh(newPos);
		if (y+1 <= m &&
			tabla[newHash][x][y+1] > tabla[pHash][x][y] + v[7 - get<1>(newPos)] &&
			tabla[newHash][x][y+1] != -1) {
				/*  Then  */
			tabla[newHash][x][y+1] = tabla[pHash][x][y] + v[7 - get<1>(newPos)];
			q.push(make_tuple(x, y+1, newPos));
		}

		/*  Pentru mutarea posibilitatea mutarii in jos  */
		newPos = moveDown(p);
		newHash = hashh(newPos);
		if (x-1 >= 1 &&
			tabla[newHash][x-1][y] > tabla[pHash][x][y] + v[7 - get<1>(newPos)] &&
			tabla[newHash][x-1][y] != -1) {
				/*  Then  */
			tabla[newHash][x-1][y] = tabla[pHash][x][y] + v[7 - get<1>(newPos)];
			q.push(make_tuple(x-1, y, newPos));
		}

		/*  Pentru mutarea posibilitatea mutarii in stanga  */
		newPos = moveLeft(p);
		newHash = hashh(newPos);
		if (y-1 >= 1 &&
			tabla[newHash][x][y-1] > tabla[pHash][x][y] + v[7 - get<1>(newPos)] &&
			tabla[newHash][x][y-1] != -1) {
				/*  Then  */
			tabla[newHash][x][y-1] = tabla[pHash][x][y] + v[7 - get<1>(newPos)];
			q.push(make_tuple(x, y-1, newPos));
		}
	}  // end while
}  // end bfs

int main() {
	int tabla[25][kMax][kMax];
	vector<int> fete;
	int a, b;

	ifstream in("rtd.in");
	in >> n >> m >> startX >> startY >> stopX >> stopY >> k;

	fete.push_back(-1);
	for (int i = 0; i < 6; i++) {
		in >> a;
		fete.push_back(a);
	}


	for (int t = 0; t < 25; t++)
		for (int i = 0; i <= n; i++)
			for (int j = 0; j <= m; j++)
				tabla[t][i][j] = INF;

	for (int i = 0; i < k; i++) {
		in >> a >> b;
		for (int t = 0; t < 25; t++)
			tabla[t][a][b] = -1;
	}
	in.close();

	bfs(tabla, fete);

	ofstream out("rtd.out");

	/*  Afisez minimul dintre toate pozitiile de stop  */
	int min = tabla[0][stopX][stopY];

	for (int t = 1; t < 25; t++) {
		if (min > tabla[t][stopX][stopY]) {
			min = tabla[t][stopX][stopY];
		}
	}

	out << min;

	out.close();

	return 0;
}

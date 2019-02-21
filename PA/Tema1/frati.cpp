#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define JON 0
#define SAM 1

using namespace std;

class my_pair {
public:
my_pair(int a, int b) {
	first = a;
	second = b;
}

int first;
int second;
};

bool sort_JON(my_pair a, my_pair b) {
	int sum_a = a.first + a.second;
	int sum_b = b.first + b.second;

	if (sum_a == sum_b) {
		return a.first > b.first;
	}
	return sum_a > sum_b;
}

int main() {
	int sums[2] = {0, 0}; // sums[0] = JON, sums[1] = SAM;
	int n;

	ifstream in("frati.in");
	in >> n;
	vector<my_pair> concursuri;

	for (int i = 0; i < n; i++) {
		int jocuri, benzi;
		in >> jocuri >> benzi;
		concursuri.push_back(my_pair(jocuri, benzi));
	}

	in.close();

	int player = JON;
	int player_change = -1;

	sort(concursuri.begin(), concursuri.end(), sort_JON);
	int maxsum;
	int i;

	while (!concursuri.empty()) {

		if (player == JON) {
			sums[0] += concursuri[0].first;
			concursuri.erase(concursuri.begin());

		} else {
			maxsum = concursuri[0].first + concursuri[0].second;
			for (i = 0; i < n; i++) {
				if (concursuri[i].first + concursuri[i].second < maxsum) {
					break; // aleg i la care se schimba suma joc+benzi
				}
			}

			sums[1] += concursuri[i - 1].second;
			concursuri.erase(concursuri.begin() + i - 1);
		}

		player += player_change;
		player_change *= -1;
	}

	ofstream out("frati.out");
	out << sums[0] << ' ' << sums[1];
	out.close();

	return 0;
}

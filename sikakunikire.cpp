#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using ll = long long;
#define REP(i,n) for(ll i=0;i<(ll)(n);i++)
#define RREP(i,n) for(ll i=((ll)(n)-1);i>=0;i--)

template<class T> inline void put(const T& first) { std::cout << first << "\n"; }


namespace {
	struct cor {
		int x, y, sz;
		cor(int x, int y, int sz) : x(x), y(y), sz(sz) {}
		cor() :x(0), y(0), sz(0) {}
		bool operator<(const cor& c) const {
			if (y == c.y) {
				return x < c.x;
			} else {
				return y < c.y;
			}
		}
	};
	int n, m, q;
	vector<cor> c;
	vector<vector<int>> table;
	vector<vector<std::set<int>>> cand;
	vector<vector<std::set<int>>> det;
	vector<int> suc;
	long long cnt = 0;
	void input() {
		cin >> n >> m >> q;
		c.resize(q);
		for (int i = 0; i < q; i++) {
			int x, y, sz;
			cin >> x >> y >> sz;
			c[i] = cor(x, y, sz);
		}
		return;
	}
	void init() {
		table.resize(n);
		for (int i = 0; i < n; i++) {
			table[i].resize(m, 0);
		}
		for (int i = 0; i < q; ++i) {
			table[c[i].y][c[i].x] = i + 1;
		}
		cnt = 0;
		suc.resize(q);
	}
	template<typename T>
	vector<T> divisor(T n) {
		vector<T> res1;
		vector<T> res2;

		for (T i = 1; i * i <= n; i++) {
			if (n % i == 0) {
				res1.push_back(i);
				if (i != n / i) {
					res2.push_back(n / i);
				}

			}
		}
		RREP(i, res2.size()) {
			res1.push_back(res2[i]);
		}
		return res1;
	}
	bool check(vector<vector<int>>& table, int y, int x, int index) {
		if (0 > y || y >= n) {
			return false;
		}
		if (0 > x || x >= m) {
			return false;
		}
		if (table[y][x] == 0) {
			return true;
		}
		if (table[y][x] == index + 1) {
			return true;
		}
		return false;
	}
	void set(vector<vector<int>>& table, int y, int x, int h, int w, int num) {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				table[y + i][x + j] = num;
			}
		}
	}
	int len(int num) {
		if (num == 0) {
			return 1;
		}
		int res = 0;
		if (num < 0) {
			res++;
			num *= -1;
		}
		while (num > 0) {
			res++;
			num /= 10;
		}
		return res;
	}
	/*string to_string(int num) {
		string res = "";
		if (num < 0) {
			res += "-";
			num *= -1;
		}
		string tmp = "";
		while (num > 0) {
			num /= 10;
			tmp += (char)((num % 10) + '0');
		}
		reverse(tmp.begin(), tmp.end());
		res += tmp;
		return res;
	}*/
	string pad(int num, int wid = 1) {
 		string res = "";
		int w = len(num);
		for (int i = 0; i < wid - w; i++) {
			res += "0";
		}
		res += to_string(num);
		return res;

	}
	void print(vector<vector<int>>& table) {
		int wid = len(q);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				string tmp = pad(table[i][j], wid);
				cout << tmp << " ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
	// 単純dfs
	void dfs(int index) {
		cnt++;
		if (cnt % 10000000 == 0) {
			cout << cnt << endl;
		}
		if (index == q) {
			//print(table);
			return;
		}
		int y = index / m, x = index % m;
		if (table[y][x] > 0) {
			dfs(index + 1);
		} else {
			int sz = c[index].sz;
			auto div = divisor(sz);
			//table[c[index].y][c[index].x] = 0;
			for (auto&& v : div) {
				int h = v;
				int w = sz / v;
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						bool flag = true;
						for (int k = 0; k < h and flag; k++) {
							for (int l = 0; l < w and flag; l++) {
								if (!check(table, c[index].y - i + k, c[index].x - j + l, index)) {
									flag = false;
								}
							}
						}
						if (flag) {
							set(table, c[index].y - i, c[index].x - j, h, w, index + 1);
							print(table);
							dfs(index + 1);
							set(table, c[index].y - i, c[index].x - j, h, w, 0);
							table[c[index].y][c[index].x] = -1;
						}
					}
				}
			}
		}

		return;
	}
	void cand_reset() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				cand[i][j].clear();
			}
		}
	}
	void det_reset() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				for (int k = 1; k <= q; k++) {
					det[i][j].insert(k);
				}
			}
		}
	}
	bool range(int y, int x) {
		return 0 <= y && y < n && 0 <= x && x < m;
	}
	void search() {
		//vector<cor> res;
		cand_reset();
		det_reset();
		fill(suc.begin(), suc.end(), 0);
		vector<int> cnt(q, 0);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (table[i][j] > 0) {
					cnt[table[i][j] - 1]++;
				}
			}
		}
		for (int index = 0; index < q; ++index) {
			auto div = divisor(c[index].sz);
			for (auto&& v : div) {
				int h = v;
				int w = c[index].sz / v;
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						bool flag = true;
						int tmp = 0;
						for (int k = 0; k < h and flag; k++) {
							for (int l = 0; l < w and flag; l++) {
								int ny = c[index].y - i + k, nx = c[index].x - j + l;
								if (!check(table, ny, nx, index)) {
									flag = false;
								}
								if (range(ny, nx) && table[ny][nx] == index + 1) {
									tmp++;
								}
							}
						}
						if (flag && tmp == cnt[index]) {
							suc[index]++;
							for (int k = 0; k < n; ++k) {
								for (int l = 0; l < m; ++l) {
									if ((c[index].y - i) <= k && k < (c[index].y - i + h) && (c[index].x - j) <= l && l < (c[index].x - j + w)) {
										cand[k][l].insert(index + 1);
									} else {
										det[k][l].erase(index + 1);
									}
								}
							}
							/*for (int k = 0; k < h; k++) {
								for (int l = 0; l < w; l++) {
									int ny = c[index].y - i + k, nx = c[index].x - j + l;
									cand[ny][nx].insert(index + 1);
								}
							}*/
						}
					}
				}
			}
		}
		return;
	}
	void init2() {
		cand.resize(n);
		for (int i = 0; i < n; i++) {
			cand[i].resize(m);
		}
		det.resize(n);
		for (int i = 0; i < n; i++) {
			det[i].resize(m);
		}
		//search();
	}
	bool determine() {
		bool res = false;
		search();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (table[i][j] == 0) {
					if (cand[i][j].size() == 1) {
						res = true;
						int t = *cand[i][j].begin();
						t--;
						int y = min(i, c[t].y);
						int x = min(j, c[t].x);
						int h = abs(i - c[t].y) + 1;
						int w = abs(j - c[t].x) + 1;
						set(table, y, x, h, w, t + 1);
					} else {
						for (auto t : det[i][j]) { // 一つ->確定というより、積集合の部分を確定とした方が応用が利く(実装は面倒)
							t--;
							//if (suc[t] == 1) {
							res = true;
							int y = min(i, c[t].y);
							int x = min(j, c[t].x);
							int h = abs(i - c[t].y) + 1;
							int w = abs(j - c[t].x) + 1;
							set(table, y, x, h, w, t + 1);
							//}
						}

					}
				}
			}
		}
		return res;
	}
	void agent() {
		while (determine()) {
			//print(table);
		}
	}
	void dfs2(int y, int x) {
		agent();
	}
	void solve() {
		auto start = chrono::system_clock::now();
		input();
		//dfs(0);
		sort(c.begin(), c.end());
		init();
		//print(table);
		init2();
		// 確定してからdfs
		agent();
		dfs(0);
		auto end = chrono::system_clock::now();
		long long time = static_cast<long long>(chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1000.0);
		long long ns = time;
		long long us = ns / 1000;
		ns %= 1000;
		long long ms = us / 1000;
		us %= 1000;
		long long sec = ms / 1000;
		ms %= 1000;
		long long min = sec / 60;
		sec %= 60;
		print(table);
		cout << "find : " << cnt << "times \n";
		cout << time << " [ns]\n";
		printf("time : %lld [min] %lld [sec] %lld [ms] %lld [us] %lld [ns]", min, sec, ms, us, ns);
	}
}

// 四角に切れ！
int main() {
	solve();
	return 0;
}


// 改良のためのメモ
/*
dfsのときもagentが使えるように改良すると速くなりそう
*/
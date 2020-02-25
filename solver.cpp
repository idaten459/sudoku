#include <bits/stdc++.h>
using namespace std;

using board_ary = array<array<int, 9 * 9>, 9>;

class sudoku {
private:
	board_ary b1;
	bool check(int num) {
		for (int i = 0; i < 9; ++i) {
			if (!check_horizon(num, i)) {
				return false;
			}
		}
		for (int j = 0; j < 9; ++j) {
			if (!check_vertical(num, j)) {
				return false;
			}
		}
		for (int i = 0; i < 9; i += 3) {
			for (int j = 0; j < 9; j += 3) {
				if (!check_square(num, i, j)) {
					return false;
				}
			}
		}
		return true;
	}
	bool check_vertical(int num, int x) {
		int cnt = 0;
		for (int i = 0; i < 9; ++i) {
			int index = i * 9 + x;
			cnt += b1[num][index];
			if (cnt > 1)return false;
		}
		return true;
	}
	bool check_horizon(int num, int y) {
		int cnt = 0;
		for (int j = 0; j < 9; ++j) {
			int index = y * 9 + j;
			cnt += b1[num][index];
			if (cnt > 1)return false;
		}
		return true;
	}
	// [(y,x),(y+3,x+3))
	bool check_square(int num, int y, int x) {
		int cnt = 0;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				int ny = y + i;
				int nx = x + j;
				int index = ny * 9 + nx;
				cnt += b1[num][index];
				if (cnt > 1)return false;
			}
		}
		return true;
	}
public:
	sudoku(int b2[9][9]) {
		init(b2);
	}
	sudoku() {}
	sudoku(board_ary ba) {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 81; ++j) {
				b1[i][j] = ba[i][j];
			}
		}
	}
	void init(int b2[9][9]) {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 81; ++j) {
				b1[i][j] = 0;
			}
		}
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				int index = i * 9 + j;
				if (1 <= b2[i][j] && b2[i][j] <= 9) {
					b1[b2[i][j] - 1][index] ^= 1;
				}
			}
		}
	}
	board_ary get_board() {
		return b1;
	}
	void input() {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 81; ++j) {
				b1[i][j] = 0;
			}
		}
		for (int i = 0; i < 9; ++i) {
			string s;
			cin >> s;
			for (int j = 0; j < 9; ++j) {
				if ('1' <= s[j] && s[j] <= '9') {
					int index = i * 9 + j;
					b1[s[j] - '1'][index] ^= 1;
				}
			}
		}
	}

	int get(int y, int x) {
		int index = y * 9 + x;
		int res = -1;
		for (int i = 0; i < 9; i++) {
			if ((b1[i][index] & 1) == 1) {
				res = i;
			}
		}
		return res;
	}

	void set(int y, int x, int num) {
		int index = y * 9 + x;
		b1[num][index] ^= 1;
	}

	void print() {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				int tmp = get(i, j);
				if (tmp >= 0) {
					cout << tmp + 1 << " ";
				} else {
					cout << "_" << " ";
				}
			}
			cout << "\n";
		}
	}
	
	// numが数独の条件を満たしてない場合、false、満たす場合trueを返す
	bool check() {
		for (int num = 0; num < 9; ++num) {
			if (!check(num)) {
				return false;
			}
		}
		return true;
	}
	
	void solve() {
		dfs(*this, 0);
		return;
	}
	void copy(board_ary b,board_ary& b2) {
		board_ary res;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 81; ++j) {
				b2[i][j] = b[i][j];
			}
		}
		return;
	}
	int cnt = 0;
	void dfs(sudoku b, int index) {
		if (index == 81) {
			if (b.check()) {
				b.print();
				cout << "\n";
				cnt++;
			}
			return;
		}
		if (cnt > 1) { // 2つ見つけたら終了
			return;
		}
		int y = index / 9;
		int x = index % 9;
		if (b.get(y, x) >= 0) {
			dfs(b, index + 1);
		} else {
			sudoku b2(b.get_board());
			for (int i = 0; i < 9; ++i) {
				b2.set(y, x, i);
				if (b2.check()) {
					dfs(b2, index + 1);
				}
				b2.set(y, x, i);
			}
		}
	}
};

int main() {
	sudoku b1;
	b1.input();
	b1.solve();
	cout << b1.cnt << endl;
    return 0;
}
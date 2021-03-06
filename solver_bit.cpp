#include <bits/stdc++.h>
using namespace std;

using board_bit = array<bitset<81>, 9>;

// 番兵
bitset<81> bw1("011111111011111111011111111011111111011111111011111111011111111011111111011111111");
bitset<81> bw2("000111111000111111000111111000111111000111111000111111000111111000111111000111111");
bitset<81> bw3("011011011011011011011011011011011011011011011011011011011011011011011011011011011");
bitset<81> bw4("000000000111111111111111111000000000111111111111111111000000000111111111111111111");

board_bit sol;

class solver_bit {
private:
	board_bit b1;
	bool check(int num) {
		if (!check_horizon(num)) {
			return false;
		}
		if (!check_vertical(num)) {
			return false;
		}
		if (!check_square(num)) {
			return false;
		}
		return true;
	}
	bool check_vertical(int num) {
		bitset<81> tmp = b1[num];
		bitset<81> bs1(tmp), bs2(tmp);
		for (int i = 0; i < 2; ++i) {
			tmp >>= 9;
			bs1 ^= tmp;
			bs2 |= tmp;
			if (bs1 != bs2)
				return false;
		}
		bitset<81> bs3(bs1), bs4(bs1);
		for (int i = 0; i < 2; ++i) {
			bs1 >>= 27;
			bs3 ^= bs1;
			bs4 |= bs1;
			if (bs3 != bs4)
				return false;
		}
		return true;
	}
	bool check_horizon(int num) {
		bitset<81> tmp = b1[num];
		bitset<81> bs1(tmp), bs2(tmp);
		for (int j = 0; j < 2; ++j) {
			tmp >>= 1;
			tmp &= bw1;
			bs1 ^= tmp;
			bs2 |= tmp;
			if (bs1 != bs2)
				return false;
		}
		bitset<81> bs3(bs1), bs4(bs1);
		for (int j = 0; j < 2; ++j) {
			bs1 >>= 3;
			bs1 &= bw2;
			bs3 ^= bs1;
			bs4 |= bs1;
			if (bs3 != bs4)
				return false;
		}
		return true;
	}
	// [(y,x),(y+3,x+3))
	bool check_square(int num) {
		bitset<81> tmp = b1[num];
		bitset<81> bs1(tmp), bs2(tmp);
		for (int j = 0; j < 2; ++j) {
			tmp >>= 1;
			tmp &= bw3;
			bs1 ^= tmp;
			bs2 |= tmp;
		}
		bitset<81> bs3(bs1), bs4(bs1);
		for (int i = 0; i < 2; ++i) {
			bs1 >>= 9;
			bs1 &= bw4;
			bs3 ^= bs1;
			bs4 |= bs1;
			if (bs3 != bs4)
				return false;
		}
		return true;
	}
	bool check_double() {
		bitset<81> bs1(0), bs2(0);
		for (int i = 0; i < 9; ++i) {
			bs1 ^= b1[i];
			bs2 |= b1[i];
		}
		if (bs1 != bs2)
			return false;
		return true;
	}
public:
	solver_bit(int b2[9][9]) {
		init(b2);
	}
	solver_bit() {
		for (int i = 0; i < 9; ++i) {
			b1[i].reset();
		}
	}
	solver_bit(board_bit ba) {
		for (int i = 0; i < 9; ++i) {
			b1[i] = ba[i];
		}
	}
	void init(int b2[9][9]) {
		for (int i = 0; i < 9; ++i) {
			b1[i].reset();
		}
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				int index = i * 9 + j;
				if (1 <= b2[i][j] && b2[i][j] <= 9) {
					b1[b2[i][j] - 1][index].flip();
				}
			}
		}
	}
	board_bit get_board() {
		return b1;
	}
	void input() {
		for (int i = 0; i < 9; ++i) {
			b1[i].reset();
		}
		for (int i = 0; i < 9; ++i) {
			string s;
			cin >> s;
			for (int j = 0; j < 9; ++j) {
				if ('1' <= s[j] && s[j] <= '9') {
					int index = i * 9 + j;
					b1[s[j] - '1'][index].flip();
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
		b1[num][index].flip();
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
		if (!check_double()) {
			return false;
		}
		return true;
	}
	using u64 = uint64_t;

	void solve(bool flag = false) {
		cnt = 0;
		if (check()) {
			dfs(*this, 0, flag);
		}
		return;
	}
	void copy(board_bit b, board_bit& b2) {
		board_bit res;
		for (int i = 0; i < 9; ++i) {
			b2[i] = b[i];
		}
		return;
	}
	int cnt = 0;
	//board_ary sol;
	void dfs(solver_bit b, int index, bool flag) {
		if (index == 81) {
			if (b.check()) {
				b.print();
				cout << "\n";
				sol = b.get_board();
				cnt++;
			}
			return;
		}
		if (cnt > 1 && flag) { // 2つ見つけたら終了
			return;
		}
		int y = index / 9;
		int x = index % 9;
		if (b.get(y, x) >= 0) {
			dfs(b, index + 1, flag);
		} else {
			solver_bit b2(b.get_board());
			for (int i = 0; i < 9; ++i) {
				b2.set(y, x, i);
				if (b2.check()) {
					dfs(b2, index + 1, flag);
				}
				b2.set(y, x, i);
			}
		}
	}
};

int main() {
	solver_bit b1;
	b1.input();
	b1.solve();
	cout << b1.cnt << endl;
    return 0;
}
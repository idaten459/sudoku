#include "bits/stdc++.h"
using namespace std;

using board = array<array<int,9>,9>;

class solver_fast{
public:
    board b;
    const int sz = 9;
    const int rsz = 3;
    int cnt = 0;
    solver_fast(){
        init();
    }
    solver_fast(board b1):b(b1){}
    
    void init(){
        for(int i = 0;i<sz;++i){
            for(int j = 0;j<sz;++j){
                b[i][j] = 0;
            }
        }
    }
    
    void input() {
        for (int i = 0; i < sz; ++i) {
            string s;
            cin >> s;
            for (int j = 0; j < sz; ++j) {
                int num = s[j]-'0';
                if (1<= num && num <= sz) {
                    b[i][j] = num;
                }else{
                    b[i][j] = 0;
                }
            }
        }
    }
    int get(int y, int x) {
        return b[y][x];
    }
    void set(int y,int x, int num){
        b[y][x] = num;
    }
    void print(){
        for(int i=0;i<sz;++i){
            for(int j=0;j<sz;++j){
                cout << get(i,j) << " ";
            }
            cout << "\n";
        }
    }
    // 変更が(y,x)のみであることが保証されている
    bool check(int y,int x){
        if(!check_horizon(y, x))return false;
        if(!check_vertical(y, x))return false;
        if(!check_square(y, x))return false;
        return true;
    }
    bool check_horizon(int y,int x){
        int num = get(y,x);
        int tmp = 0;
        for(int i=0;i<sz;++i){
            if(get(i,x)==num)tmp++;
            if(tmp>1)return false;
        }
        return true;
    }
    bool check_vertical(int y,int x){
        int num = get(y,x);
        int tmp = 0;
        for(int j=0;j<sz;++j){
            if(get(y,j)==num)tmp++;
            if(tmp>1)return false;
        }
        return true;
    }
    bool check_square(int y,int x){
        int num = get(y,x);
        int tmp = 0;
        int sy = y/rsz,sx = x/rsz;
        for(int i = sy*rsz;i<(sy+1)*rsz;++i){
            for(int j = sx*rsz;j<(sx+1)*rsz;++j){
                if(get(i,j)==num)tmp++;
                if(tmp>1)return false;
            }
        }
        return true;
    }
    board sol;
    void solve(){
		cnt = 0;
        dfs(0,0);
        return;
    }
    void dfs(int y,int x){
        if(y==sz&&x==0){
            for(int i=0;i<sz;++i){
                for(int j=0;j<sz;++j){
                    sol[i][j] = b[i][j];
                }
            }
            cnt++;
            return;
        }
        if(cnt>1){
            return;
        }
        int ny = y, nx = x+1;
        if(nx==sz){
            nx = 0;
            ny++;
        }
        if(get(y,x)>0){
            dfs(ny,nx);
        }else{
            for(int i=1;i<=sz;++i){
                set(y,x,i);
                if(check(y,x)){
                    dfs(ny,nx);
                }
                set(y,x,0);
            }
        }
    }
};

class generator {
private:
    board gene;
    solver_fast sd2;
    std::random_device seed_gen;
    std::mt19937_64 engine;
    const int sz = 9;
public:
    generator(){
        engine.seed(time(nullptr));
        init();
    }
    void init() {
        const int loop = 20;
        for (int i = 0; i < loop; ++i) {
            int x = engine() % 9;
            int y = engine() % 9;
            while (sd2.get(y,x) > 0) {
                x = engine() % 9;
                y = engine() % 9;
            }
            int num = engine() % 9 + 1;
            sd2.set(y, x, num);
            while(!sd2.check(y, x)){
                num = engine() % 9 + 1;
			    sd2.set(y, x, num);
            }
            sd2.solve();
            while (sd2.cnt == 0) {
                sd2.set(y, x, 0);
                num = engine() % 9 + 1;
                sd2.set(y, x, num);
                sd2.solve();
            }
            if (sd2.cnt == 1)
                break;
        }
        sd2.solve();
        gene = sd2.b;
    }
    void generate() {
        board gb = sd2.sol;
        for(int i=0;i<sz;++i){
            for(int j=0;j<sz;++j){
                cout << gb[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
        board preGene = gene;
        while (true) {
            vector<int> popList;
            auto tmp = gb;
            for (int i = 0; i < 9; ++i) {
                for(int j=0;j<9;++j){
                    if (gb[i][j]) {
                        popList.push_back(i*sz+j);
                    }
                }
            }
            int t = min<int>(30, popList.size());
            shuffle(popList.begin(), popList.end(), engine);
            vector<int> randomIndex(t), cn(t), indList(t);
            for (int i = 0; i < t; ++i) {
                int r = popList[i];
                int y = r / 9;
                int x = r % 9;
                int num = gb[y][x];
                gb[y][x] = 0;
                solver_fast sd(gb);
                sd.solve();
                gb[y][x] = num;
                cn[i] = sd.cnt;
            }
            
            int maxCnt = 0, ind = 0;
            for (int i = 0; i < t; ++i) {
                if (cn[i] == 1) {
                    maxCnt = cn[i];
                    ind = i;
                }
            }
            if (maxCnt == 0) {
                solver_fast sd3(preGene);
                sd3.print();
                for(int i=0;i<sz;++i){
                    for(int j=0;j<sz;++j){
                        cout << sd3.b[i][j];
                    }
                }
                cout << "\n";
                break;
            }
            int r = popList[ind];
            int y = r / 9;
            int x = r % 9;
            gb[y][x] = 0;
            preGene = gb;
        }
        return;
    }
};

int main() {
	// あまり早くなってないし、なんならバグがある。
    generator gen;
    auto start = clock();
    gen.generate();
    auto end = clock();
    cout << "time : " << static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0 << "[ms]\n";
    return 0;
}

#include "bits/stdc++.h"
using namespace std;

using board = array<array<int,9>,9>;

class solver_fast{
public:
    board b;
    const int sz = 9;
    const int rsz = 3;
    int cnt = 0;
    solver_fast(){}
    
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
        for(int i=0;i<sz;++i){
            for(int j=0;j<sz;++j){
                sol[i][j] = b[i][j];
            }
        }
        dfs(0,0);
        return;
    }
    void dfs(int y,int x){
        if(y==sz&&x==0){
            cnt++;
            print();
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

int main() {
    solver_fast sf;
    sf.input();
    auto start = clock();
    sf.solve();
    cout << sf.cnt << endl;
    auto end = clock();
    cout << "time : " << static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0 << "[ms]\n";
    return 0;
}

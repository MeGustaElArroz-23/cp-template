
#include<bits/stdc++.h>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")
    
using namespace std;
    
#define int long long
#define vi vector<int>
#define vvi vector<vi>
#define vvvi vector<vvi>
#define pii pair<int,int>
#define vii vector<pii>
#define vvii vector<vii>
#define fi first
#define se second
#define pb push_back

#define vd vector<double>
#define sz(x) (int)(x).size()



    void print(vi v, int maxit=1e9){
        for (int i=0;i<min((int)v.size(),maxit);i++) cerr << v[i] << ' ';
        cerr << '\n';
    }

struct Combinatorial{
    int mod;

    Combinatorial(int modd) : mod(modd){}

    /// Returns x^y
    int pot(int x, int y){
        if (y==0) return 1;
        int ans=pot(x,y/2);
        ans=ans*ans%mod;
        if (y%2) ans=ans*x%mod;
        return ans;
    }

    /// Returns modular inverse
    int inv(int x){
        return pot(x,mod-2);
    }

    vi memo_fac={1};
    vi memo_invfac={1};

    /// Returns x!
    int fac(int x){
        while ((int)memo_fac.size()<=x){
            memo_fac.pb(memo_fac.back()*((int)memo_fac.size())%mod);
            memo_invfac.pb(-1);
        }
        return memo_fac[x];
    }

    /// Returns x!^-1
    int invfac(int x){
        if ((int)memo_fac.size()>x && memo_invfac[x]!=-1) return memo_invfac[x];
        else return memo_invfac[x] = inv(fac(x));
    }

    /// Returns (n choose a). Works even if a<0 or a>n.
    int comb(int n, int a){
        if (a<0 || a>n) return 0;
        return fac(n)*invfac(a)%mod*invfac(n-a)%mod;
    }

    /// Assumes no number is < 0
    int multicomb(vi v){
        int s=accumulate(v.begin(),v.end(),0);
        int ans=fac(s);
        for (int x:v) ans=(ans*invfac(x))%mod;
        return ans;
    }
};



namespace DSmisc{

    /// @brief Normal DSU
    struct DSU{
        vi id;
        
        DSU(int n){
            id=vi(n);
            for (int i=0;i<n;i++) id[i]=i;
        }

        int getid(int x){
            if (id[x]==x) return x;
            else return id[x]=getid(id[x]);
        }

        void unite(int a, int b){
            a=getid(a);
            b=getid(b);
            if (rand()%2) id[a]=b;
            else id[b]=a;
        }
    };

    namespace LiChao{

        struct line{
            int a,b;
            line(int aa,int bb){
                a=aa;
                b=bb;
            }

            int eval(int x){
                return a*x+b;
            }
        };

        double intersect(line l1, line l2){
            return ((double) l2.b-l1.b)/(l1.a-l2.a);
        }

        struct LCTrb{
            vector<line> tree;
            int n;
            vector<vector<pair<int,line>>> history;

            LCTrb(int nn){
                n=nn;
                tree=vector<line>(4*n+8,line(0,0));
            }

            void add(line l, int ac, int acl, int acr){ //ac=1
                if (ac==1) history.pb({});

                if (l.eval(acl)>=tree[ac].eval(acl) && l.eval(acr)>=tree[ac].eval(acr)){
                    history.back().pb(pair<int,line>{ac,tree[ac]});
                    tree[ac]=l;
                }
                else if (l.eval(acl)<=tree[ac].eval(acl) && l.eval(acr)<=tree[ac].eval(acr)) return;
                else{
                    double mid = intersect(l,tree[ac]);
                    if ((mid<(acr+acl)/2+0.5) && l.eval(acl)>=tree[ac].eval(acl)) add(l,2*ac,acl,(acl+acr)/2);
                    if ((mid<(acr+acl)/2+0.5) && l.eval(acl)<=tree[ac].eval(acl)){
                        add(tree[ac],2*ac,acl,(acl+acr)/2);
                        history.back().pb(pair<int,line>{ac,tree[ac]});
                        tree[ac]=l;
                    }
                    if ((mid>=(acr+acl)/2+0.5) && l.eval(acr)>=tree[ac].eval(acr)) add(l,2*ac+1,(acl+acr)/2+1,acr);
                    if ((mid>=(acr+acl)/2+0.5) && l.eval(acr)<=tree[ac].eval(acr)){
                        add(tree[ac],2*ac+1,(acl+acr)/2+1,acr);
                        history.back().pb(pair<int,line>{ac,tree[ac]});
                        tree[ac]=l;
                    }
                }
            }

            int getmax(int obj, int ac, int acl, int acr){
                if (acl==acr) return tree[ac].eval(obj);
                if (obj<=(acl+acr)/2) return max(tree[ac].eval(obj),getmax(obj,2*ac,acl,(acl+acr)/2));
                else return max(tree[ac].eval(obj),getmax(obj,2*ac+1,(acl+acr)/2+1,acr));
            }

            void rollback(){
                for (pair<int,line> p:history.back()) tree[p.fi]=p.se;
                history.pop_back();
            }

        };
    }

    namespace Treap{
        #define pTT pair<Treap*,Treap*>

        struct Treap{
            int data;
            int prio;
            int sz;
            pTT h;

            Treap(int x){
                data=x;
                prio=rand();
                sz=1;
                h=pTT{nullptr,nullptr};
            }
        };

        int lsize(Treap *ac){
            assert(ac!=nullptr);
            if (ac -> h.fi != nullptr) return ac -> h.fi -> sz;
            else return 0;
        }

        int getval(Treap* ac, int pos){
            assert(ac!=nullptr && pos>=0);
            if (pos < lsize(ac)) return getval(ac -> h.fi, pos);
            else pos -= lsize(ac);
            if (pos == 0) return ac -> data;
            return getval(ac -> h.se, pos-1);
        }

        void recalc(Treap* ac){
            if (ac==nullptr) return;
            ac -> sz = 1;
            if (ac -> h.fi != nullptr) ac -> sz += ac -> h.fi -> sz;
            if (ac -> h.se != nullptr) ac -> sz += ac -> h.se -> sz;
        }

        pTT split(Treap *ac, int pos, int prof){ //pos is num,ber of members in left / first on right
            if (ac == nullptr) return{nullptr, nullptr};
            // cerr << ac << ' ' << pos << ' ' << prof << ' '  << lsize(ac) << ' ' << ac->prio << endl;
            if (pos<=lsize(ac)){
                pTT ans = split(ac -> h.fi,pos, prof+1);
                ac -> h.fi = ans.se;
                recalc(ac);
                return pTT{ans.fi, ac};
            }
            else{
                pTT ans = split(ac -> h.se, pos-lsize(ac)-1, prof+1);
                ac -> h.se = ans.fi;
                recalc(ac);
                return pTT{ac, ans.se};
            }
        }

        Treap* merge(Treap *a, Treap *b){
            // cerr << a << ' ' << b << endl;
            if (a==nullptr) return b;
            if (b==nullptr) return a;
            // cerr << a->prio << endl;
            if (a -> prio < b -> prio){
                a -> h.se = merge(a -> h.se, b);
                recalc(a);
                return a;
            }
            else {
                b -> h.fi = merge(a, b -> h.fi);
                recalc(b);
                return b;
            }
        }

        vector<Treap*> separate(Treap* tr, vi pos){ //{0, 2, 5, n}
            vector<Treap*> ans;
            for (int i=1;i<(int)pos.size();i++){
                pTT p = split(tr,pos[i]-pos[i-1],0);
                ans.pb(p.fi);
                tr=p.se;
            }
            ans.pb(tr);
            return ans;
        }

        Treap* unite(vector<Treap*> v){
            Treap* ans = nullptr;
            for (auto &x:v) ans=merge(ans, x);
            return ans;
        }
    }

    namespace FFT{
        typedef complex<long double> C;
        const long double PI = 3.141592653589793238462643383279;

        /// Returns fft of real or complex vector v of length 2^N
        vector<C> FFTdecimal(vector<C> v){
            int N = log2(v.size());
            int n = (1LL << N);
            assert((int)v.size() == n);

            if (n==1) return {v[0]};

            auto w = polar(1.0L,2*PI/n); // e^{2*pi*i/n}

            vector<C> potw(n);
            potw[0]=1;
            for (int i=1;i<n;i++) potw[i]=w*potw[i-1];
            

            vector<C> trans = v;
            vector<C> aux(n);
            for (int stepsize=(1<<(N-1));stepsize>=1;stepsize/=2){

                int numsteps = n/stepsize;

                for (int i=0;i<n;i++) aux[i]=trans[i];

                for (int in=0;in<stepsize;in++){
                    for (int j=0;j<numsteps/2;j++)
                        aux[in+(2*j+1)*stepsize] = aux[in+(2*j+1)*stepsize]*potw[j*stepsize];
                    

                    for (int j=0;j<numsteps/2;j++) {
                        trans[in+j*stepsize] = aux[in+2*j*stepsize] + aux[in+(2*j+1)*stepsize];
                    }

                    for (int j=0;j<numsteps/2;j++) {
                        trans[in+n/2+j*stepsize] = aux[in+2*j*stepsize] - aux[in+(2*j+1)*stepsize];
                    }
                }
            }

            return trans;
        }

        /// Convolutes two polynomials of decimal numbers. If using long doubles
        /// precision should be good
        vd ConvoluteDecimal(vd v1, vd v2){
            int n = max((int) v1.size(), (int) v2.size());
            while (__builtin_popcount(n)>1) n++;
            n*=2;

            vector<C> w1;
            for (double d:v1) w1.pb(d);
            vector<C> w2;
            for (double d:v2) w2.pb(d);

            w1.resize(n);
            w2.resize(n);

            vector<C> trans1 = FFTdecimal(w1);
            vector<C> trans2 = FFTdecimal(w2);

            vector<C> trans(n);
            for (int i=0;i<n;i++) trans[i]=trans1[i]*trans2[i];

            vector<C> transtrans = FFTdecimal(trans);

            vd ans(n);
            for (int i=0;i<n;i++) ans[i] = transtrans[(n-i)%n].real()/n;
            return ans;
        }

        vector<int> ConvoluteQuadraticMod(vi &a, vi &b, int mod){
            int n=sz(a), m=sz(b);
            vi ans(n+m-1);
            for (int i=0;i<n;i++) for (int j=0;j<m;j++) ans[i+j]+=a[i]*b[j];
            for (int i=0;i<n+m-1;i++) if (ans[i]>=mod) ans[i]%=mod;
            return ans;
        }

        /// Assumes order of root modulo mod in maxN
        vector<int> NTT(vi &v, int mod=998244353, int maxN=23, int root=31){
            int N = log2(v.size());
            int n = (1LL << N);
            assert((int)v.size() == n && N<=maxN);

            if (n==1) return {v[0]};

            int w = root;
            for (int i=0;i<maxN-N;i++) w = w*w%mod;

            vi potw(n);
            potw[0]=1;
            for (int i=1;i<n;i++) potw[i]=w*potw[i-1]%mod;
            

            vi trans = v;
            vi aux(n);
            for (int stepsize=(1<<(N-1));stepsize>=1;stepsize/=2){

                int numsteps = n/stepsize;

                for (int i=0;i<n;i++) aux[i]=trans[i];

                for (int in=0;in<stepsize;in++){
                    for (int j=0;j<numsteps/2;j++)
                        aux[in+(2*j+1)*stepsize] = aux[in+(2*j+1)*stepsize]*potw[j*stepsize]%mod;
                    

                    for (int j=0;j<numsteps/2;j++) {
                        trans[in+j*stepsize] = (aux[in+2*j*stepsize] + aux[in+(2*j+1)*stepsize]);
                        if (trans[in+j*stepsize] >= mod) trans[in+j*stepsize] -= mod;
                    }

                    for (int j=0;j<numsteps/2;j++) {
                        trans[in+n/2+j*stepsize] = (aux[in+2*j*stepsize] - aux[in+(2*j+1)*stepsize]);
                        if (trans[in+n/2+j*stepsize] < 0) trans[in+n/2+j*stepsize]+=mod;
                    }
                }
            }

            return trans;
        }

        vi ConvoluteNTT(vi v1, vi v2, int mod=998244353, int maxN=23, int root=31){
            // int n = max((int) v1.size(), (int) v2.size());
            if (min(sz(v1),sz(v2))<=10) return ConvoluteQuadraticMod(v1,v2,mod);
            int n = sz(v1) + sz(v2) -1;
            while (__builtin_popcount(n)>1) n++;
            // n*=2;

            v1.resize(n);
            v2.resize(n);

            vi trans1 = NTT(v1,mod,maxN,root);
            vi trans2 = NTT(v2,mod,maxN,root);

            vi trans(n);
            for (int i=0;i<n;i++) trans[i]=trans1[i]*trans2[i]%mod;

            vi transtrans = NTT(trans,mod,maxN,root);

            vi ans(n);
            for (int i=0;i<n;i++) ans[i] = transtrans[(n-i)%n]*Combinatorial(mod).inv(n)%mod;
            return ans;
        }

        // Assumes no polynomial in the list is 0
        // Tested here:
        // https://judge.yosupo.jp/problem/product_of_polynomial_sequence
        // Benchmark: 7s for sumdeg = 5e5 (worst case of all deg1s allowed)
        vi ConvoluteListNTT(vvi v){
            int n=sz(v);
            priority_queue<pii> cola;
            for (int i=0;i<n;i++) cola.push({-sz(v[i]),i});
            while (cola.size()>1){
                int p=cola.top().se; cola.pop();
                int q=cola.top().se; cola.pop();
                v[p] = ConvoluteNTT(v[p],v[q]);
                while (v[p].back()==0) v[p].pop_back();
                cola.push({-sz(v[p]),p});
            }
            return v[cola.top().se];
        }
    }
}

using namespace DSmisc::FFT;

    void print(vd v, int maxit=1e9){
        cerr << setprecision(3) << fixed;
        for (int i=0;i<min((int)v.size(),maxit);i++) cerr << v[i] << ' ';
        cerr << '\n';
    }



signed main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    vd a{1,1,1,1};
    // print(FFTdecimal(a));
    vd b{1,2,3,4};
    print(ConvoluteDecimal(a,b));
    
    int n;
    cin >> n;
    if (n==0){
        cout << 1 << '\n';
        return 0;
    }
    vvi v(n);
    int sd=0;
    for (int i=0;i<n;i++){
        int d;
        cin >> d; 
        sd+=d;
        v[i]=vi(d+1);
        for (int &x:v[i]) cin >> x;
    }
    // Debug::print(v);
    priority_queue<pii> cola;
    for (int i=0;i<n;i++) cola.push({-sz(v[i]),i});
    while (cola.size()>1){
        int p=cola.top().se; cola.pop();
        int q=cola.top().se; cola.pop();
        v[p] = ConvoluteNTT(v[p],v[q]);
        while (v[p].back()==0) v[p].pop_back();
        cola.push({-sz(v[p]),p});
    }
    int x=cola.top().se;
    v[x].resize(sd+1);
    for (int y:v[x]) cout << y << ' ';
    cout << '\n'; 
}
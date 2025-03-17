#pragma once
#include "basic_template.h"

/*
    Tested, mostly with CSES, unless otherwise noted
*/

namespace TreeUtil{

    struct Tree{
        int n;
        int root;
        vvi con;
        vvi h;
        vi p;

        vi prof;
        vi stsize;

        vi w;
        bool HasNodeWeight;

        //////////////////////
        //// CONSTRUCTORS ////
        //////////////////////

        /// Get tree given con
        Tree(vvi conq, int rootq=0, vi wq={}): root(rootq), con(conq), w(wq){
            if (w.size()) HasNodeWeight=1;
            n=con.size();

            //Get h,p

            queue<pii> cola;
            cola.push({root,-1});
            p=vi(n);
            h=vvi(n);
            prof=vi(n);
            while (cola.size()){
                pii pp=cola.front();
                cola.pop();
                if (pp.se!=-1){
                    h[pp.se].pb(pp.fi);
                    prof[pp.fi]=prof[pp.se]+1;
                }
                p[pp.fi]=pp.se;
                for (int y:con[pp.fi]) if (y!=pp.se) cola.push({y,pp.fi});
            }

            //Calc stsize

            stsize=vi(n,1);
            for (int x:OrderDownTop()) for (int y:h[x]) stsize[x]+=stsize[y];
        };

        /// Get tree given p
        Tree(vi pq, int rootq=0, vi wq={}):root(rootq), p(pq), w(wq){
            if (w.size()) HasNodeWeight=1;
            n=p.size();

            //Get con,h

            con=h=vvi(n);
            for (int i=0;i<n;i++) if (i!=root){
                con[i].pb(p[i]);
                con[p[i]].pb(i);
                h[p[i]].pb(i);
            }

            //Calc prof

            prof=vi(n);
            queue<int> cola;
            for (int y:h[root]) cola.push(y);
            while (cola.size()){
                int x=cola.front();
                cola.pop();
                prof[x]=prof[p[x]]+1;
                for (int y:h[x]) cola.push(y);
            }

            //Calc stsize

            stsize=vi(n,1);
            for (int x:OrderDownTop()) for (int y:h[x]) stsize[x]+=stsize[y];
        };

        ///Get tree given weighted con. Weights are put in the son node con{to,w}
        ///untested
        Tree(vvii wcon, int rootq=0, int rootw=0): root(rootq){
            if (w.size()) HasNodeWeight=1;
            n=con.size();

            con = vvi(n);
            for (int i=0;i<n;i++) for (pii pp:wcon[i]) con[i].pb(pp.fi);

            //Get h,p

            queue<pii> cola;
            cola.push({0,-1});
            p=vi(n);
            h=vvi(n);
            prof=vi(n);
            w=vi(n);
            w[0]=rootw;
            while (cola.size()){
                pii pp=cola.front();
                cola.pop();
                if (pp.se!=-1){
                    h[pp.se].pb(pp.fi);
                    prof[pp.fi]=prof[pp.se]+1;
                }
                p[pp.fi]=pp.se;
                for (pii y:wcon[pp.fi]) if (y.fi!=pp.se){
                    cola.push({y.fi,pp.fi});
                    w[y.fi] = y.se;
                }
            }

            //Calc stsize

            stsize=vi(n,1);
            for (int x:OrderDownTop()) for (int y:h[x]) stsize[x]+=stsize[y];
        }

        ///////////////////
        //// DISTANCES ////
        ///////////////////

        /// Returns vector with all distances to x
        vi GetAllUnweightedDist(int x){
            vi d(n,-1);
            queue<pii> cola;
            cola.push({x,0});
            while (cola.size()){
                pii pp = cola.front();
                cola.pop();
                if (d[pp.fi]==-1){
                    d[pp.fi] = pp.se;
                    for (int y:con[pp.fi]) cola.push({y,pp.se+1});
                }
            }
            return d;
        }

        /// Assumes weights are distances to parent
        /// untested
        vi GetAllWeightedDist(int x){
            assert(HasNodeWeight);
            vi d(n,-1);
            queue<pii> cola;
            cola.push({x,0});
            while (cola.size()){
                pii pp = cola.front();
                cola.pop();
                if (d[pp.fi]==-1){
                    d[pp.fi] = pp.se;
                    for (int y:h[pp.fi]) cola.push({y,pp.se+w[y]});
                    if (p[pp.fi]!=-1) cola.push({p[pp.fi],pp.se+w[pp.fi]});
                }
            }
            return d;
        }

        /// Returns diameter and its two endpoints
        pair<int,pii> GetUnweightedDiameter(){
            vi d1=GetAllUnweightedDist(0);
            int x=0;
            for (int i=1;i<n;i++) if (d1[i]>d1[x]) x=i;
            vi d2=GetAllUnweightedDist(x);
            int y=0;
            for (int i=1;i<n;i++) if (d2[i]>d2[y]) y=i;
            return pair<int,pii>{d2[y],pii{x,y}};
        }

        /// Assumes weights are distances to parent
        /// untested
        pair<int,pii> GetWeightedDiameter(){
            assert(HasNodeWeight);
            vi d1=GetAllWeightedDist(0);
            int x=0;
            for (int i=1;i<n;i++) if (d1[i]>d1[x]) x=i;
            vi d2=GetAllWeightedDist(x);
            int y=0;
            for (int i=1;i<n;i++) if (d2[i]>d2[y]) y=i;
            return pair<int,pii>{d2[y],pii{x,y}};
        }


        ////////////////
        //// ORDER ////
        ///////////////

        /// Returns vi with Down to Top ordering
        vi OrderDownTop(){
            stack<int> st;
            vector<bool> libre(n,1);
            st.push(root);
            vi ans;
            while (st.size()){
                int x=st.top();
                if (libre[x]){
                    libre[x]=0;
                    for (int y:h[x]) st.push(y);
                }
                else{
                    ans.pb(x);
                    st.pop();
                }
            }
            return ans;
        }

        /// Returns vi with Top to Down ordering
        vi OrderTopDown(){
            queue<int> cola;
            cola.push(root);
            vi ans;
            while (cola.size()){
                int x=cola.front();
                cola.pop();
                ans.pb(x);
                for (int y:h[x]) cola.push(y);
            }
            return ans;
        }

        /// First - vi: Euler Tour
        /// Second - vii: pairs<EnterTime, ExitTime>
        pair<vi,vii> EulerTour(){
            vi tour;
            vii times;

            stack<int> st;
            vector<bool> libre(n,1);
            st.push(root);
            while (st.size()){
                int x=st.top();
                if (libre[x]){
                    libre[x]=0;
                    for (int y:h[x]) st.push(y);
                    times[x].fi = tour.size();
                    tour.pb(x);
                }
                else{
                    times[x].se = tour.size();
                    tour.pb(x);
                    st.pop();
                }
            }
            return {tour,times};
        }

        ////////////////////////
        //// BINARY LIFTING ////
        ////////////////////////

        bool isBinaryLiftingPrepared = 0;
        vvi bl;
        int LiftSize;

        /// Auxiliary function to prepare for lifting
        void prepareBinaryLifting(){
            isBinaryLiftingPrepared=1;
            LiftSize = 2+log2(n);
            bl=vvi(LiftSize,vi(n,-1));
            bl[0]=p;
            for (int i=1;i<LiftSize;i++)for (int j=0;j<n;j++) if (bl[i-1][j]!=-1) bl[i][j] = bl[i-1][bl[i-1][j]];
        }

        /// Returns l-th ancestor of x. -1 if not exists
        int BinaryLift(int x, int l){
            if (!isBinaryLiftingPrepared) prepareBinaryLifting();
            if (l>prof[x]) return -1;

            for (int i=0;i<LiftSize;i++) if (x!=-1 && (l&(1<<i))) x=bl[i][x];
            return x;
        }

        /// Returns LCA
        int LCA(int a, int b){
            if (!isBinaryLiftingPrepared) prepareBinaryLifting();

            if (prof[a]>prof[b]) swap(a,b);
            b = BinaryLift(b,prof[b]-prof[a]);

            if (a==b) return a;

            for (int i=LiftSize-1;i>=0;i--) if (bl[i][a]!=bl[i][b]){
                a=bl[i][a];
                b=bl[i][b];
            }
            return p[a];
        }

        /// Returns (unweighted) distance using LCA
        int distanceLCA(int a, int b){
            return prof[a]+prof[b]-2*prof[LCA(a,b)];
        }

        /////////////
        //// HLD ////
        /////////////

        /*
            HLD is implementing by using a single segment tree. Vertices are mapped
            to convenient numbers in [0,n-1] and paths are converted to 
            O(logn) range queries to be passed to the segment tree
        */

        bool isHldPrepared = 0;
        vi hldmap;
        vi invhldmap;
        vi hldp;
        vi heavyabove; //number of edges above vertex that are heavy

        /// Auxiliary function to prepare HLD
        void prepareHld(){
            isHldPrepared = 1;
            hldmap=vi(n,-1);
            hldp=vi(n,-1);
            heavyabove=vi(n);

            int it=0;
            stack<int> st;
            st.push(root);
            while (st.size()){
                int x=st.top();
                if (hldmap[x]==-1){
                    hldmap[x]=it;
                    if (x!=root) hldp[hldmap[x]]=hldmap[p[x]];
                    it++;
                    if (h[x].size()){
                        int y=h[x][0];
                        for (int z:h[x]) if (stsize[z]>stsize[y]) y=z;
                        heavyabove[it]=heavyabove[it-1]+1;
                        st.push(y);
                    }
                }
                else{
                    st.pop();
                    for (int y:h[x]) if (hldmap[y]==-1) st.push(y);
                }
            }
            invhldmap=vi(n);
            for (int i=0;i<n;i++) invhldmap[hldmap[i]]=i;
        }

        /// Returns HLD index of a single vertex 
        int getHldPoint(int x){
            if (!isHldPrepared) prepareHld();
            return hldmap[x];
        }

        /// Returns vii as interval HLD decomposition of a path containing x and
        /// lift parents. Will RTE if lift is too big.
        vii getHldUpperPath(int x, int lift){ //inclusive on x, exclusive on lift
            if (!isHldPrepared) prepareHld();

            x=hldmap[x];
            vii ans;
            while (lift>0){
                assert(x>=0);
                if (!heavyabove[x]){
                    ans.pb({x,x});
                    x=hldp[x];
                    lift--;
                }
                else if (heavyabove[x]+1<=lift){
                    ans.pb({x-heavyabove[x],x});
                    lift-=heavyabove[x]+1;
                    x=hldp[x-heavyabove[x]];
                }
                else{
                    ans.pb({x-lift+1,x});
                    lift=0;
                }
            }
            return ans;
        }

        /// Returns as vii the interval HLD decomposition of path from x to y
        vii getHldPath(int x, int y){
            if (!isHldPrepared) prepareHld();

            int lca = LCA(x,y);
            vii ans = getHldUpperPath(x,prof[x]-prof[lca]+1);
            for (pii pp:getHldUpperPath(y,prof[y]-prof[lca])) ans.pb(pp);
            return ans;
        }

        //HLD protips
        /*
            If TLE you can try to do small intervals with an array and only use
            segtree for big enought (e.j) >= 30 (this will only work for range
            queries point updates). Else just do a faster segtree or a fentree.
        */

        //////////////////////
        //// MISCELLANIUS ////
        //////////////////////

        vi getSumWeightsToRoot(){
            assert(HasNodeWeight);
            vi ans = GetAllWeightedDist(root);
            for (int &x:ans) x+=w[root];
            return ans;
        }

        // TODO: Centroid decomposition
    };

    Tree ReadTreeCon(int n=-1, bool index1=1){
        if (n==-1) cin >> n;
        vvi con(n);
        for (int i=0;i<n-1;i++){
            int a,b;
            cin >> a >> b;
            if (index1){a--;b--;}
            con[a].pb(b);
            con[b].pb(a);
        }
        return Tree(con);
    }

    Tree ReadTreePar(int n=-1, bool index1=1){
        if (n==-1) cin >> n;
        vi p(n);
        p[0]=-1;
        for (int i=1;i<n;i++){
            cin >> p[i];
            if (index1) p[i]--;
        }
        return Tree(p);
    }

}
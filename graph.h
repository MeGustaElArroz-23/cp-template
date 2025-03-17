#pragma once
#include "basic_template.h"
#include"DSmisc.h"

namespace GraphUtil{
    // TODO: Dijstra

    /// Reads and returns con
    vvi ReadGraphCon(int n=-1, int m=-1, bool index1=1){
        if (n==-1) cin >> n;
        if (m==-1) cin >> m;
        vvi con(n);
        for (int i=0;i<m;i++){
            int a,b;
            cin >> a >> b;
            if (index1){
                a--;b--;
            }
            con[a].pb(b);
            con[b].pb(a);
        }
        return con;
    }

    /// First: Each component. Second: list of compId
    pair<vvi,vi> ConnectedComponents(vvi con){
        int n=con.size();
        vi libre(n,1);
        vi id(n);
        queue<int> cola;
        vvi ans;
        for (int i=0;i<n;i++) if (libre[i]){
            cola.push(i);
            ans.pb({});
            while (cola.size()){
                int x=cola.front();
                cola.pop();
                if (libre[x]){
                    libre[x]=0;
                    id[x]=ans.size()-1;
                    ans.back().pb(x);
                    for (int y:con[x]) cola.push(y);
                }
            }
        }
        return {ans,id};
    }

    /// Returns vii with the bridges
    vii bridges(vvi con){
        int n=con.size();
        vi maxh(n,1e9);
        vi prof(n,-1);
        stack<pii> st;
        vii ans;
        for (int i=0;i<n;i++) if (prof[i]==-1){
            st.push({i,-1});
            while (st.size()){
                pii p=st.top();
                // cerr << p.fi << ' ' << p.se << ' ' << prof[p.fi] << ' ' << maxh[p.fi] << '\n';
                if (prof[p.fi]==-1){
                    // cerr << "v1: " << p.fi+1 << ' ' << p.se+1 << '\n';
                    if (p.se==-1) prof[p.fi]=0;
                    else prof[p.fi]=prof[p.se]+1;
                    for (int y:con[p.fi]) if (prof[y]==-1) st.push({y,p.fi});
                }
                else if (maxh[p.fi]==(int)1e9){
                    // cerr << "v2: " << p.fi+1 << ' ' << p.se+1 << '\n';
                    maxh[p.fi]=prof[p.fi];
                    for (int y:con[p.fi]) if (y!=p.se){
                        if (maxh[y]!=(int)1e9) maxh[p.fi]=min(maxh[p.fi],maxh[y]);
                        else maxh[p.fi]=min(maxh[p.fi],prof[y]); //back edge
                    }
                    if (maxh[p.fi]==prof[p.fi] && prof[p.fi]!=0) ans.pb({p.fi,p.se});
                }
                else st.pop();
            }
        }
        // cerr << "Bridges: ";
        // for (pii p:ans) cerr << p.fi+1 << '-' << p.se+1 << ' ';
        // cerr << '\n';
        return ans;
    }
    
    /** First - vvi: component and content. 
    Second - vvi: con of the component graph*/
    pair<vvi,vvi> SCCgraph(vvi con){ 
        int n=con.size();
        vii brid=bridges(con);
        set<pii> del;
        for (pii p:brid) del.insert(p);
        
        vvi newcon(n);
        for (int i=0;i<n;i++) for (int x:con[i]) if (!(del.count({i,x}) || del.count({x,i}))) newcon[x].pb(i);

        pair<vvi,vi> p = ConnectedComponents(newcon);
        // debug(p.se);
        int m=p.fi.size();

        vvi concomp(m);
        for (pii q:brid){
            concomp[p.se[q.fi]].pb(p.se[q.se]);
            concomp[p.se[q.se]].pb(p.se[q.fi]);
        }
        return {p.fi,concomp};
    }

    /// Returns MST
    /// I/O: vector<pair<int,pii>>
    vector<pair<int,pii>> MST(vector<pair<int,pii>> v){
        sort(v.begin(),v.end());
        int n=v.size();
        vector<pair<int,pii>> ans(n);
        DSmisc::DSU dsu(n);
        for (pair<int,pii> p:v) if (dsu.getid(p.se.fi) != dsu.getid(p.se.se)){
            ans.pb(p);
            dsu.unite(p.se.fi,p.se.se);
        }
        return ans;
    }
}

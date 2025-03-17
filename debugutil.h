#pragma once
#include "basic_template.h"

namespace Debug{

    void print(vi v, int maxit=1e9){
        for (int i=0;i<min((int)v.size(),maxit);i++) cerr << v[i] << ' ';
        cerr << '\n';
    }

    void print(vvi v, int maxrow=1e9, int maxcol=1e9){
        for (int i=0;i<min((int)v.size(),maxrow);i++) print(v[i],maxcol);
        cerr << '\n';
    }

    void print(vii v, int maxit=1e9, string sep=" ; "){
        for (int i=0;i<min((int)v.size(),maxit);i++) cerr << v[i].fi << ' ' << v[i].se << sep;
        cerr << '\n';
    }

    void print(vvii v, int maxrow=1e9, int maxcol=1e9, string sep=" ; "){
        for (int i=0;i<min((int)v.size(),maxrow);i++) print(v[i],maxcol, sep);
        cerr << '\n';
    }

    void print(vd v, int maxit=1e9){
        cerr << setprecision(3) << fixed;
        for (int i=0;i<min((int)v.size(),maxit);i++) cerr << v[i] << ' ';
        cerr << '\n';
    }

}

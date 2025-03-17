#pragma once
#include "basic_template.h"

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


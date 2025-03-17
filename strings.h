#pragma once
#include "basic_template.h"
#include "combo.h"

namespace StringUtil{

    const int NUM_L ='z'-'a'+1; 

    struct Hasher{
        int mod;
        int base;
        vi basepot;
        vi invbasepot;
        Combinatorial combo;

        Hasher(int maxl, int mod=998244353): mod(mod), combo(mod){
            srand(time(0));
            base=rand()%mod;
            basepot=invbasepot=vi(maxl+1,1);
            basepot[1]=base;
            invbasepot[1]=combo.inv(base);
            for (int i=2;i<=maxl;i++){
                basepot[i]=base*basepot[i-1]%mod;
                invbasepot[i]=invbasepot[1]*invbasepot[i-1]%mod;
            }
        }

        /// Returns prefix hashlist of size n (with no 0 at the start). 
        /// Works for vectors and strings.
        /// Assumes nothing has value 0.
        template <typename T> vi getPrefixHash(T s){
            int n=s.size();
            vi ans(n);
            ans[0]=s[0]*base;
            for (int i=1;i<n;i++) ans[i]=(ans[i-1]+s[i]*basepot[i])%mod;
            return ans;
        }

        /// Returns hash of vector or string
        /// Assumes nothing has value 0.
        template <typename T> int getHash(T s){
            int n=s.size();
            int ans=0;
            for (int i=0;i<n;i++) ans=(ans+s[i]*basepot[i])%mod;
            return ans;
        }

        vi combinePrefixHash(vi h1, vi h2){
            int l1=h1.size();
            for (int i=0;i<h2.size();i++) h1.pb((h1[l1-1]+h2[i]*basepot[l1])%mod);
            return h1;
        }

        int combineHash(int hash1, int length1, int hash2){
            return (hash1 + basepot[length1]*hash2)%mod;
        }
    };
}


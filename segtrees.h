#pragma once
#include "basic_template.h"


namespace SegmentTrees{
    /*
        Tested https://cses.fi/problemset/task/2134/
    */
    struct STpointUPDrangeMAX {
        vii tree;
        int n;
        
        STpointUPDrangeMAX(int nn){
            n=nn;
            tree=vii(8*n+8);
        }

        //you give new value
        void upd(int x, int val){
            upd(x,val,0,n-1,1);
        }

        void upd(int x, int val, int l, int r, int ac){
            // if (l<=x && x<=r) tree[ac]=max(tree[ac],{val,x});
            // else return;
            // if (l==r) return;
            // upd(x,val,l,(l+r)/2,2*ac);
            // upd(x,val,(l+r)/2+1,r,2*ac+1);

            if (!(l<=x && x<=r)) return;

            if (l==r) tree[ac] = {val,x};
            else{
                upd(x,val,l,(l+r)/2,2*ac);
                upd(x,val,(l+r)/2+1,r,2*ac+1);
                tree[ac] = max(tree[2*ac],tree[2*ac+1]);
            }
        }

        pii getmax(int l, int r){
            return getmax(l,r,0,n-1,1);
        }

        pii getmax(int xl, int xr, int l, int r, int ac){ // returns pair (value, pos)
            if (r<xl || l>xr || xl>xr) return {0,0};
            if (r<=xr && l>=xl) return tree[ac];
            return max(
                getmax(xl,xr,l,(l+r)/2,2*ac),
                getmax(xl,xr,(l+r)/2+1,r,2*ac+1)
            );
        }
    };

    /*
    Checked with https://judge.yosupo.jp/problem/point_add_range_sum
    */
    struct STpointUPDrangeSUM {
        vi tree;
        int n;
        
        STpointUPDrangeSUM(int nn){
            n=nn;
            tree=vi(8*n+8);
        }

        // you say dif between old and new values
        void upd(int x, int val){
            upd(x,val,0,n-1,1);
        }

        void upd(int x, int val, int l, int r, int ac){
            // if (l<=x && x<=r) tree[ac]=max(tree[ac],{val,x});
            // else return;
            // if (l==r) return;
            // upd(x,val,l,(l+r)/2,2*ac);
            // upd(x,val,(l+r)/2+1,r,2*ac+1);

            if (!(l<=x && x<=r)) return;

            if (l==r) tree[ac] += val;
            else{
                upd(x,val,l,(l+r)/2,2*ac);
                upd(x,val,(l+r)/2+1,r,2*ac+1);
                tree[ac] = tree[2*ac] + tree[2*ac+1];
            }
        }

        int getsum(int xl, int xr){
            return getsum(xl,xr,0,n-1,1);
        }

        int getsum(int xl, int xr, int l, int r, int ac){ // returns pair (value, pos)
            if (r<xl || l>xr || xl>xr) return 0;
            if (r<=xr && l>=xl) return tree[ac];
            return getsum(xl,xr,l,(l+r)/2,2*ac) + getsum(xl,xr,(l+r)/2+1,r,2*ac+1);
        }
    };

    /* Tested EUC mirror */

    struct STupdRangeMinQueryPointMin{
        int n;
        vi tree;
    
        STupdRangeMinQueryPointMin(int nn){
            n=nn;
            tree=vi(4*n+4,1e9);
        }
    
        void upd(int xval, int xl, int xr, int ac, int l, int r){
            if (xl > r || xr < l) return;
            // cerr <<ac << ' ' << l << ' ' << r << '\n';
            if (tree[ac]<=xval) return;
            if (xl<=l && xr>=r){
                // cerr << "updated " << ac << ": " << l << ' ' << r << '\n';
                tree[ac]=xval;
                return;
            }
            upd(xval,xl,xr,2*ac,l,(l+r)/2);
            upd(xval,xl,xr,2*ac+1,(l+r)/2+1,r);
        }
    
        int query(int x, int ac, int l, int r){
            if (l==r) return tree[ac];
            if (x<=(l+r)/2) return min(tree[ac],
                query(x,2*ac,l,(l+r)/2));
            else return min(tree[ac],
                query(x,2*ac+1,(l+r)/2+1,r));
        }
    };

}
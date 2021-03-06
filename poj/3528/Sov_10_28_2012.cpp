#include <iostream>
#include <valarray>
#include <vector>
#include <stdio.h>
#include <math.h>
using namespace std;

typedef double DB;
// typedef valarray<DB> PT;
// #define EPS (1e-8)
// #define sig(x) ((x)<-EPS? -1: (x)>EPS? 1:0)
// #define len(p) sqrt(((p)*(p)).sum())
// #define dot(a,b) ((a)*(b)).sum()
// #define crs(a,b) (((a).cshift(+1)*(b).cshift(-1))     \
//                   - ((a).cshift(-1)*(b).cshift(+1)))
#define box(o,a,b,c) dot(crs((a)-(o),(b)-(o)),(c)-(o))

struct PT {
  DB x, y, z;
  PT(DB x=0, DB y=0, DB z=0)
    : x(x), y(y), z(z) {}
  PT operator -(PT &p) {
    return PT(x-p.x, y-p.y, z-p.z);
  }
};

DB dot(PT a, PT b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}
DB len(PT p) {
  return sqrt(dot(p,p));
}
PT crs(PT a, PT b) {
  return PT(a.z*b.y - a.y*b.z,
            a.x*b.z - a.z*b.x,
            a.y*b.x - a.x*b.y);
}

// O(nlogn)
class ConvexHull3D {
private:
#define add push_back
  struct FT : vector<int> {
    FT(int a=-1, int b=-1, int c=-1) {
      add(a); add(b); add(c);
    }
  };
  vector<PT> ps;
  vector<FT> crt;
  vector<vector<int> > blg; // belong

#define ahead(x) {swap(ps[x],ps[i]); break;}
  bool getFirst() {
    int i,n = ps.size();
    for(i=1;i<n;i++) if(len(ps[0]-ps[i])!=0) ahead(1);
    for(i++;i<n;i++) if(len(crs(ps[1]-ps[0],ps[i]-ps[0]))!=0) ahead(2);
    for(i++;i<n;i++) if(box(ps[0],ps[1],ps[2],ps[i])!=0) ahead(3);
    return i < n;
  }
public:
  ConvexHull3D(vector<PT> ps): ps(ps) {}
  ConvexHull3D(PT *p = NULL, int n = 0) {
    ps.clear();
    for (int i=0; i<n; i++)
      ps[i] = p[i];
  }

#define outbox(i,f) (box(ps[f[0]],ps[f[1]],ps[f[2]],ps[i])>0)
#define put(a,b,c) {blg[a][b]=blg[b][c]=blg[c][a] \
      = crt.size(); crt.add(FT(a,b,c));}
  void dfs(int i, FT &g) {
    FT f = g;
    g[0] = -1;
    for (int k=0; k<3; k++) {
      int a=f[k],b=f[(k+1)%3],j=blg[b][a];
      if (~crt[j][0])
        if (outbox(i,crt[j])) dfs(i,crt[j]);
        else put(i,a,b);
    }
  }

  void hull() {
    crt.clear();
    if (!getFirst()) return ;

    int i,j,n=ps.size();
    blg.resize(n,vector<int>(n,-1));
    put(0,1,2);put(2,1,0);
    for (i=3; i<n; i++)
      for (j=0;j<crt.size();j++)
        if (~crt[j][0]&&outbox(i,crt[j])){dfs(i,crt[j]);break;}

    for (i=0; i<crt.size();i++)
      if (!~crt[i][0])
        crt.erase(crt.begin()+i--);
  }

  DB face_area() {
    DB r = 0;
    for(int i = 0; i < crt.size(); i++) {
      FT &f = crt[i];
      r += fabs(len(crs(ps[f[1]]-ps[f[0]], ps[f[2]]-ps[f[0]])));
    }
    return r / 2;
  }
};

int main()
{
  int n;
  while (scanf("%d", &n) != EOF) {
    vector<PT> p(n);
    for (int i = 0; i < n; i++) {
      DB v[3];
      scanf("%lf%lf%lf", &v[0], &v[1], &v[2]);
      // p[i] = PT(v, 3);
      p[i]=PT(v[0],v[1],v[2]);
    }
    ConvexHull3D P(p);
    P.hull();
    printf("%.3f\n", P.face_area());
  }
  return 0;
}

#include "builtin.h"

#define RE(v) return mv_P(p,v)

D_P1(left)  { RE(l); }
D_P2(left)  { del(r); RE(l); }
D_P2(right) { del(l); RE(r); }

D_P11(constant) { del(ll); cp_P(p,l); }
D_P12(constant) { del(ll); del(rr); cp_P(p,l); }

D_P2(apply) { V(p) = apply1d(r,l); }
D_P2(flip)  { V(p) = apply1d(l,r); }
D_P2(compose) { DECL_ARR(V,v,1); v[0]=cpy1(l); setO(p, wrapO(cpy1(r),1,v)); }
D_P11(compose) { DECL_ARR(V,v,1); v[0]=cpy1(ll); setO(p, wrapO(cpy(l),1,v)); }
D_P12(compose) { DECL_ARR(V,v,2); v[0]=cpy1(ll); v[1]=cpy1(rr); setO(p, wrapO(cpy(l),2,v)); }

D_T11(flip)  { return apply2_T(l,ll,ll); }
D_P11(flip)  { V l1=cpy(ll); apply2_P(p,l,l1,ll); FREE(P(l1)); }
D_T12(flip)  { return apply2_T(l,rr,ll); }
D_P12(flip)  { apply2_P(p,l,rr,ll); }

D_T21(bind)     { return apply2_T(l,ll,T(r)); }
D_P21(bind)     { V r_=cpy(r); apply2_P(p,l,ll,r_); FREE(P(r_)); }
D_T21(backbind) { return apply2_T(r,T(l),ll); }
D_P21(backbind) { V l_=cpy(l); apply2_P(p,r,l_,ll); FREE(P(l_)); }

D_T21(hook)     { return apply2_T(r,apply1_T(l,ll),ll); }
D_P21(hook)     { V a; apply2_P(p,r,a=apply1_d(l,cpy(ll)),ll); FREE(P(a)); }
D_T22(hook)     { return apply2_T(r,apply1_T(l,ll),rr); }
D_P22(hook)     { V a; apply2_P(p,r,a=apply1(l,ll),rr); FREE(P(a)); }
D_T21(backhook) { return apply2_T(l,ll,apply1_T(r,ll)); }
D_P21(backhook) { V a; apply2_P(p,l,ll,a=apply1_d(r,cpy(ll))); FREE(P(a)); }
D_T22(backhook) { return apply2_T(l,ll,apply1_T(r,rr)); }
D_P22(backhook) { V a; apply2_P(p,l,ll,a=apply1(r,rr)); FREE(P(a)); }

D_T21(compose) { return apply1_T(r, apply1_T(l,ll)); }
D_P21(compose) { V a; apply1_P(p, r, a=apply1(l,ll)); FREE(P(a)); }
D_T22(compose) { return apply2_T(r, apply1_T(l,ll), apply1_T(l,rr)); }
D_P22(compose) { V al,ar; apply2_P(p, r, al=apply1(l,ll), ar=apply1(l,rr)); FREE(P(al)); FREE(P(ar)); }

I toBoold(V v) { I r = (T(v)!=Z_t || Z(v)); ddel(v); return r; }
I toBool(V v) { I r = (T(v)!=Z_t || Z(v)); del(v); return r; }

D_L2(power) { return 2*(!!(r&Z_t)) + 1; }
D_D2(power) { return power_l2(T(l), T(r)); }
D_T21(power) {
  I n=getFloorZ(r);
  DDO(i,n) { T llt=apply1_T(l,ll); if(llt==ll)break; ll=llt; }
  return ll;
}
D_P21(power) {
  I n=getFloorZ(r); ll=cpy1(ll); DDO(i,n) ll=apply1_d(l,ll);
  mv_P(p,ll); FREE(P(ll));
}
D_T22(power) {
  I n=getFloorZ(r);
  DDO(i,n) { T llt=apply2_T(l,ll,rr); if(llt==ll)break; ll=llt; }
  return ll;
}
D_P22(power) {
  I n=getFloorZ(r); ll=cpy1(ll); DDO(i,n) ll=apply2_d(l,ll,cpy(rr));
  del(rr); mv_P(p,ll); FREE(P(ll));
}

D_P21(while) {
  ll=cpy1(ll); while(toBoold(apply1_d(r, cpy(ll)))) ll=apply1_d(l,ll);
  mv_P(p,ll); FREE(P(ll));
}
D_P22(while) {
  ll=cpy1(ll); while(toBoold(apply2_d(r, cpy(ll), cpy(rr))))
    ll=apply2_d(l,ll,cpy(rr));
  del(rr); mv_P(p,ll); FREE(P(ll));
}

void compose_init() {
#define D(n,c,f) DB(t##n,c,f); DB(p##n,c,f)
  DB(t1,'[',l);  DB(p1,'[',left);
  DB(t1,']',l);  DB(p1,']',left);
  DB(t2,'[',l);  DB(p2,'[',left);
  DB(t2,']',r);  DB(p2,']',right);

  DB(t11,'k',l); DB(p11,'k',constant);
  DB(t12,'k',l); DB(p12,'k',constant);

  DB(t2,'.',V);  DB(p2,'.',apply);
  DB(t2,'~',V);  DB(p2,'~',flip );
  DB(t2,'o',O);  DB(p2,'o',compose);
  DB(t11,'o',O); DB(p11,'o',compose);
  DB(t12,'o',O); DB(p12,'o',compose);
  D(21,'O',compose);  D(22,'O',compose);
  D(21,'h',hook);     D(22,'h',hook);
  D(21,'H',backhook); D(22,'H',backhook);

  D(11,'~',flip);
  D(12,'~',flip);

  D(21,'b',bind);
  D(21,'B',backbind);

  B_u2['p']=DB(l2,'p',power); DB(d2,'p',power);
  D(21,'p',power); D(22,'p',power);

  DB(t21,'w',V); DB(p21,'w',while);
  DB(t22,'w',V); DB(p22,'w',while);
#undef D
}

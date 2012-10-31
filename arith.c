#define IS_ARITH(v) !!(v&ARITH_t)
D_L1(arith) { return IS_ARITH(l); }
D_L2(arith) { return 2*IS_ARITH(r) + IS_ARITH(l); }
D_D1(arith) { return arith_l1(T(l)); }
D_D2(arith) { return arith_l2(T(l),T(r)); }

D_T2(arith) { return max(l,r); }

#define D_S(name) void name##_s(P p, V v, I n, V* vs)
//Monads
#define DF(n, T, op) D_S(n##T) { return set##T(v,op(T(vs[0]))); }
#define LINE(n, T) case T##_t: s.f=&n##T##_s; break;
#define OP(n,op) DF(n,Z,op); DF(n,R,op); \
  D_S1(n) { S s; switch(l){ LINE(n,Z); LINE(n,R); } return s; }
OP(negate, -);
OP(reciprocal, 1/);
OP(floor, floor);
OP(ceiling, ceiling);
#undef DF
#undef LINE
#undef OP

//Dyads
#define ON(op,l,r) (l) op (r)
#define DFZZ(n, op) D_S(n##ZZ) { return setZ(v,ON(op,Z(vs[0]),Z(vs[1]))); }
#define DFZR(n, op) D_S(n##ZR) { return setR(v,ON(op,(R)Z(vs[0]),R(vs[1]))); }
#define DFRZ(n, op) D_S(n##RZ) { return setR(v,ON(op,Z(vs[0]),(R)Z(vs[1]))); }
#define DFRR(n, op) D_S(n##RR) { return setR(v,ON(op,R(vs[0]),R(vs[1]))); }

#define LINE(n, T1,T2) case T2##_t: s.f=&n##T1##T2##_s; break;
#define DECL_S2(n)  D_S2(n) { S s; switch(l){ \
    case Z_t: switch(r){ LINE(n,Z,Z); LINE(n,Z,R); } \
    case R_t: switch(r){ LINE(n,R,Z); LINE(n,R,R); } } return s; }
#define OP(n,op)  DFZZ(n,op) DFZR(n,op) DFRZ(n,op) DFRR(n,op) DECL_S2(n)
OP(plus, +);
OP(minus, -);
OP(times, *);
OP(divide, /);
#undef ON
#define ON(op,l,r) op(l,r)
OP(min, min);
OP(max, max);
#undef ON

Z modZZ(Z a, Z b) { Z m=a%b; return m+b*(m*b<0); }
R modRZ(R a, Z b) { return a-b*floor(a/b); }
D_S(modZZ) { return setZ(v,modZZ(Z(vs[0]),Z(vs[1]))); }
D_S(modRZ) { return setR(v,modRZ(R(vs[0]),Z(vs[1]))); }
D_S2(mod) { S s; switch(r) { // Using r, not l
  case Z_t: { switch(l){ case Z_t: s.f=&modZZ_s; break;
                         case R_t: s.f=&modRZ_s; break; }
              return s; }
  case R_t: s.f=NULL; return s;
} }

#undef DFZZ
#undef DFZR
#undef DFRZ
#undef DFRR
#undef LINE
#undef OP

// EXPORT DEFINITIONS
EXTERN_BUILTINS;
void arith_init() {
#define SET(c, f) B_l1[c] = B_u1[c] = &arith_l1; B_t1[c] = &l_t1; \
                  B_d1[c] = &arith_d1; B_s1[c] = &f##_s1
  SET('-', negate);
  SET('/', reciprocal);
  SET('m', floor);
  SET('M', ceiling);
#undef SET

#define SET(c, f) B_l2[c] = B_u2[c] = &arith_l2; B_t2[c] = &arith##_t2; \
                  B_d2[c] = &arith_d2; B_s2[c] = &f##_s2
  SET('+', plus);
  SET('-', minus);
  SET('*', times);
  SET('/', divide);
  SET('%', mod);
  SET('m', min);
  SET('M', max);
#undef SET
}

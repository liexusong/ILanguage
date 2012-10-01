#define IS_ARITH(v) !!(v&ARITH_t)
D_L1(arith) { return IS_ARITH(l); }
D_L2(arith) { return 2*IS_ARITH(r) + IS_ARITH(l); }
D_D1(arith) { return arith_l1(T(l)); }
D_D2(arith) { return arith_l2(T(l),T(r)); }

D_T2(arith) { return max(l,r); }

//Monads
//TODO: complex case
#define OP(op) { switch (T(l)) { M_L(Z,op); M_L(R,op); } del(l); }
#define M_L(T, op) case T##_t: T(p)=op T(l); break
D_P1(negate) OP(-);
D_P1(reciprocal) OP(1/);
#undef M_L
#define M_L(T, op) case T##_t: T(p)=op(T(l)); break
D_P1(floor) OP(floor);
D_P1(ceiling) OP(ceiling);
#undef M_L
#undef OP

//Dyads
#define OP(op) { switch (max(T(l),T(r))) { DL(Z,op); DL(R,op); } del(l);del(r); }
#define DL(T, op) case T##_t: T(p) = get##T(l)op get##T(r); break
D_P2(plus) OP(+);
D_P2(minus) OP(-);
D_P2(times) OP(*);
D_P2(divide) OP(/);
#undef DL
#define DL(T, op) case T##_t: T(p) = op(get##T(l), get##T(r)); break
#define MOD(l,r) (l) - (r)*floor((l)/(r))
#define DL1(T, op) case T##_t: T(p) = get##T(l)op get##T(r); break
// D_P2(mod) OP(MOD);
D_P2(mod) { switch (max(T(l),T(r))) { DL1(Z,%); DL(R,MOD); } del(l);del(r); }
#undef DL1
#undef MOD
D_P2(min) OP(min);
D_P2(max) OP(max);
#undef DL
#undef OP

// EXPORT DEFINITIONS
EXTERN_BUILTINS;
void arith_init() {
#define SET(c, f) B_l1[c] = B_u1[c] = &arith_l1; B_t1[c] = &l_t1; \
                  B_d1[c] = &arith_d1; B_p1[c] = &f##_p1
  SET('-', negate);
  SET('/', reciprocal);
  SET('m', floor);
  SET('M', ceiling);
#undef SET

#define SET(c, f) B_l2[c] = B_u2[c] = &arith_l2; B_t2[c] = &arith##_t2; \
                  B_d2[c] = &arith_d2; B_p2[c] = &f##_p2
  SET('+', plus);
  SET('-', minus);
  SET('*', times);
  SET('/', divide);
  SET('%', mod);
  SET('m', min);
  SET('M', max);
#undef SET
}

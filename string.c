Str toString(V);
V scan(Str);
D_P1(print) { setL(p, wrapStr(toString(l))); del(l); }

D_L1(string) { return 0; }
D_U1(string) { return !!(l&L_t); }
D_D1(string) { return !!((T(l)&L_t) && (L(l)->t==S_t)); }
Str arrToString(I l, I c, I o, P v) {
  DECL_NSTR(s, l); memcpy(s,v+o,min(c-o,l)); memcpy(s,v,max(0,l+o-c)); return s;
}
D_P1(eval) { L ll=L(l); Str s=arrToString(ll->l, ll->c, ll->o, ll->p);
  del(l); V(p)=scan(s); FREE(s);
}

EXTERN_BUILTINS;
void string_init() {
  DB(t1,'s',L); DB(p1,'s',print);
  DB(l1,'S',string); DB(u1,'S',string); DB(d1,'S',string);
  DB(t1,'S',V); DB(p1,'S',eval);
}

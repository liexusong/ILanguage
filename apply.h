typedef struct {T t; P v;} X;
#define CONST_X  1 //ESZRC
#define FUNC_X   2 //BOFNQ
#define LIST_X   4 //LA

V apply    (V, I, V*);
I dom      (V, I, V*);
X mapclass (V);
V fmap     (V, I, V*, I);

T apply_T    (V, I, T*);
I dom_T      (V, I, T*);
T mapclass_T (T);
T fmap_T     (V, I, T*, I);

void apply_P (P, V, I, V*);
void fmap_P  (P, V, I, V*, I);

// apply.c
#define DECLARE_APPLY(t) void apply_P_##t(P, t, I, V*); \
                         T apply_T_##t(t, I, T*);
ON_TYPES(NCONST,DECLARE_APPLY)
#undef DECLARE_APPLY
void apply_P_FB(P, F, I, V*);
//void apply_P_FQ(P, F, I, V*);
T apply_T_FB(F, I, T*);
//T apply_T_FQ(F, I, T*);

V apply1(V, V);
V apply2(V, V, V);
V apply1d(V, V);
V apply2d(V, V, V);

T apply1_T(V, T);
T apply2_T(V, T, T);
void apply1_P(P, V, V);
void apply2_P(P, V, V, V);

// mapclass.c
I mapclasseq(X, X);

// dom.c
#define DECLARE_DOM(T) I dom_##T(T, I, V*);
DECLARE_DOM(B) DECLARE_DOM(F) DECLARE_DOM(N) DECLARE_DOM(Q)
#undef DECLARE_DOM

I dom_true(I);

// fmap.c
void fmap_LIST_P(P, V, I, V*, I, I);

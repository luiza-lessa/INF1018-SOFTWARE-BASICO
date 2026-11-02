typedef union {
  float f;
  unsigned int i;
} U;

#define getsig(x)        ((x)>>31 & 1)
#define getexp(x)        ((x)>>23 & 0xff)
#define getfrac(x)       ((x) & 0x7fffff)
#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))

float float2(float f) {
    U u;
    unsigned int s, e, frac;
    u.f = f;
    s    = getsig(u.i);
    e    = getexp(u.i);
    frac = getfrac(u.i);

    if (e == 0) {
        frac = frac << 1;
        u.i = makefloat(s, (frac >> 23) & 1, frac);
    } else if (e == 0xff) {
        ;
    } else {
        e = e + 1;
        if (e == 0xff) frac = 0;
        u.i = makefloat(s, e, frac);
    }
    return u.f;
}

float int2float(int i) {
    U u;
    unsigned int a, s, e, frac;
    int p;

    if (i == 0) { u.i = 0; return u.f; }

    if (i < 0) { s = 1; a = (unsigned int)(-(long long)i); }
    else       { s = 0; a = (unsigned int)i; }

    p = 31;
    while (((a >> p) & 1) == 0) p--;
    e = (unsigned int)(p + 127);

    if (p <= 23) {
        frac = (a << (23 - p)) & 0x7fffff;
    } else {
        int k = p - 23;
        unsigned int keep = a >> k;
        unsigned int rest = a & ((1u << k) - 1); 
        unsigned int half = 1u << (k - 1);
        if (rest > half || (rest == half && (keep & 1)))
            keep++;
        if (keep & (1u << 24)) { 
            keep >>= 1;
            e++;
        }
        frac = keep & 0x7fffff;
    }

    u.i = makefloat(s, e, frac);
    return u.f;
}
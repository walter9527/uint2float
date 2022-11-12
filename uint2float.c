#include <stdio.h>

unsigned lowbit(unsigned x) {
    return x & (~x + 1);
}

unsigned getE(unsigned x) {
    unsigned cnt = 0;
    while (x != 1) {
        x >>= 1;
        cnt++;
    }
    return cnt;
}

unsigned roundToEven(unsigned x, unsigned p) {
    unsigned mask = (1u << p) - 1;
    unsigned significand = x & mask;
    unsigned halfway = 1u << (p - 1);
    x &= ~mask;
    if ((significand > halfway) || (significand == halfway && lowbit(x) == 1u << p)) {
        x += 1u << p;
    }
    return x;
}

float uint2float(unsigned u) {
    if (u == 0)
        return 0.0f;

    unsigned sign = u & 0x80000000;
    u = sign ? ~u + 1 : u;
    unsigned E = getE(u);
    unsigned frac = (~(1 << E)) & u;
    if (E > 23) {
        frac = roundToEven(frac, E - 23);
        frac >>= (E - 23);
    } else {
        frac <<= (23 - E);
    }
    
    unsigned exp = (E + 127) << 23; 

    unsigned ret = sign | exp | frac;
    return *(float *)&ret;
}

int main() {
    unsigned a = 0x12345678;
    printf("%#x, %d, %f\n", a, a, uint2float(a));
    float b = (float)(int)a;
    printf("%f\n", b);
    return 0;
}

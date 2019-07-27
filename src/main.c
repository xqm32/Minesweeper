#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #define TEST_MODE // 测试模式。

int a, b; // a: Y坐标; b: X坐标。
char c[100]; // 末位储存是否有雷，倒数第二位储存是否可见，其余位右移两位以表示周围雷数。

void d(int e, int f) {
    if (e>9||e<0||f>9||f<0||c[e*10+f]&2) return;
    c[e*10+f]|=0x2;
    if (!(c[e*10+f]>>2)) { // 注意: 该函数不能只有一个参数，否则将会变得十分复杂。
        d(e-1, f);
        d(e+1, f);
        d(e, f-1);
        d(e, f+1);
        d(e-1, f-1);
        d(e-1, f+1);
        d(e+1, f-1);
        d(e+1, f+1);
    }
}

int main() {
    srand(time(NULL));
#ifdef TEST_MODE
    memset(c, 0x2, 100); // 将所有的格子设为可见。
#endif
    for (a=0; b=rand()%100, a<10; ++a)
        if (0x1&c[b]) // 倘若已经有了雷，跳过该格。
            --a;
        else {
            // 分别为周围八个格子的计数器各加一。
            if (b>10) c[b-11]=(c[b-11]&0x3)|(((c[b-11]>>2)+1)<<2);
            if (b>9) c[b-10]=(c[b-10]&0x3)|(((c[b-10]>>2)+1)<<2);
            if (b>8) c[b-9]=(c[b-9]&0x3)|(((c[b-9]>>2)+1)<<2);
            if (b>0) c[b-1]=(c[b-1]&0x3)|(((c[b-1]>>2)+1)<<2);
            c[b]|=0x1; // 将该格设为有雷。
            if (b<99) c[b+1]=(c[b+1]&0x3)|(((c[b+1]>>2)+1)<<2);
            if (b<90) c[b+9]=(c[b+9]&0x3)|(((c[b+9]>>2)+1)<<2);
            if (b<89) c[b+10]=(c[b+10]&0x3)|(((c[b+10]>>2)+1)<<2);
            if (b<88) c[b+11]=(c[b+11]&0x3)|(((c[b+11]>>2)+1)<<2);
#ifdef TEST_MODE
            printf("%d, %d\n", b/10, b%10);
#endif
        }
    for (; ; scanf("%d, %d", &a, &b)) {
        if (a>9||a<0||b>9||b<0) continue;
        d(a, b);
        for (a=0; a<100; ++a) {
            printf("%c", c[a]&2?c[a]&1?'*':(c[a]>>2)+48:' ');
            // 若是雷，则打印 '*'; 若看得见，则打印雷数; 若看不见，则打印 ' ';
            if (!((a+1)%10))
                printf("\n");
        }
    }
}
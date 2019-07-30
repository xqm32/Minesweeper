#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int len, col, num, insi=0;
// 行数，列数，雷数, 可见的格子数
unsigned char *m;
// 00000000
//        ^ 是否有雷
//       ^  是否可见
//     ^~   标记及标记类型
// ^~~~     周围的雷的个数

#define NONE 0
#define SAFE 1
#define MINE 2
#define QUERY 3

#define M(x, y) m[(x)*col+(y)]
#define IS_MINE(x, y) (M(x, y)&1)
#define IS_INSI(x, y) (M(x, y)&2)
// 是否可见
#define IS_MARK(x, y) (M(x, y)&12)
#define IS_NUM(x, y) (M(x, y)&240)
#define IS_OUT(x, y) (x>=len||y>=col||x<0||y<0)
#define GET_MARK(x, y) ((M(x, y)&12)>>2)
#define GET_NUM(x, y) ((M(x, y)&240)>>4)
#define SET_MINE(x, y, s) M(x, y)=s?M(x, y)|1:M(x, y)&254
#define SET_INSI(x, y, s) M(x, y)=s?M(x, y)|2:M(x, y)&254
#define SET_MARK(x, y, s) M(x, y)=s&2?s&1?M(x, y)|12:(M(x, y)&243)|8:s&1?M(x, y)&243|4:M(x, y)&243
#define INC_NUM(x, y) if (!IS_OUT(x, y)) M(x, y)=(M(x, y)&15)|(GET_NUM(x, y)+1)<<4

void bfs(int x, int y) {
    if (IS_OUT(x, y)||IS_INSI(x, y))
        return;
    ++insi;
    SET_INSI(x, y, 1);
    if (!GET_NUM(x, y)) {
        bfs(x-1, y-1);
        bfs(x-1, y);
        bfs(x-1, y+1);
        bfs(x, y-1);
        bfs(x, y+1);
        bfs(x+1, y-1);
        bfs(x+1, y);
        bfs(x+1, y+1);
    }
}

void print() {
    int x, y;
    printf("  ");
    for (x=0; x<len; ++x) {
        printf("%-2d", x);
    }
    printf("\n");
    for (y=col-1; y>=0; --y) {
        printf("%-2d", y);
        for (x=0; x<len; ++x) {
            printf("%-2c", IS_MARK(x, y)?GET_MARK(x, y)+'<':
                IS_INSI(x, y)?IS_MINE(x, y)?'*':GET_NUM(x, y)+'0':' ');
        }
        printf("%-2d", y);
        printf("\n");
    }
    printf("  ");
    for (x=0; x<len; ++x) {
        printf("%-2d", x);
    }
    printf("\n");
}

void clear() {
    for (int i=0; i<len*col; ++i)
        m[i]|=2;
}

int main(int argc, char **argv) {
    int x, y, s;
    srand(time(NULL));
    if (argc>3) {
        printf("正在从参数中获取信息\n");
        len=atoi(argv[1]);
        printf("长度: %d\n", len);
        col=atoi(argv[2]);
        printf("宽度: %d\n", len);
        num=atoi(argv[3]);
        printf("雷数: %d\n", len);
    }
    else {
        printf("请依次输入长度、宽度和雷数\n");
        scanf("%d %d %d", &len, &col, &num);
    }
    m=(char *)malloc(len*col);
    memset(m, 0, len*col);
    print();
    printf("请依次输入X坐标、Y坐标和指令\n");
    scanf("%d %d %d", &x, &y, &s);
    for (int i=0, a, b; i<num; ++i) {
        a=rand()%len;
        b=rand()%col;
        if (IS_MINE(a, b)||(a==x&&b==y))
            continue;
        else {
            INC_NUM(a-1, b-1);
            INC_NUM(a-1, b);
            INC_NUM(a-1, b+1);
            INC_NUM(a, b-1);
            SET_MINE(a, b, 1);
            INC_NUM(a, b+1);
            INC_NUM(a+1, b-1);
            INC_NUM(a+1, b);
            INC_NUM(a+1, b+1);
        }
    }
    if (s) {
        SET_MARK(x, y, s);
        print();
    }
    else {
        if (IS_MINE(x, y)) {
            printf("失败\n");
            clear();
            print();
            return 1;
        }
        bfs(x, y);
        print();
    }
    if (insi==len*col-num) {
        printf("胜利\n");
        clear();
        print();
        return 0;
    }
    for (; ;) {
        printf("请依次输入X坐标、Y坐标和指令\n");
        scanf("%d %d %d", &x, &y, &s);
        if (s) {
            SET_MARK(x, y, s);
            print();
        }
        else {
            SET_MARK(x, y, 0);
            if (IS_MINE(x, y)) {
                printf("失败\n");
                clear();
                print();
                return 1;
            }
            bfs(x, y);
            print();

        }
        if (insi==len*col-num) {
            printf("胜利\n");
            clear();
            print();
            return 0;
        }
    }
}
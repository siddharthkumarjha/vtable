#include <stdio.h>
#include <unistd.h>

typedef struct Base1 base1;
typedef struct Base2 base2;
typedef struct Derived derived;

typedef void (*foo_t)(void *);
typedef void (*bar_t)(void *);
typedef struct vTable
{
    foo_t foo_fn;
    bar_t bar_fn;
} vTable;

typedef enum TAG
{
    BASE1 = 0,
    BASE2,
    DERIVED
} Tag_t;

struct Base1
{
    Tag_t Tag;
};
struct Base2
{
    Tag_t Tag;
};
struct Derived
{
    Tag_t Tag;
    int data;
};

void base1_foo(base1 *ptr) { printf("Base1\n"); }
void derived_foo(derived *ptr) { printf("derived foo | data: %d\n", ptr->data); }

void base2_bar(base2 *ptr) { printf("Base2\n"); }
void derived_bar(derived *ptr) { printf("derived bar | data: %d\n", ptr->data); }

const vTable g_vTableArr[] = {
    [BASE1]   = {.foo_fn = (foo_t)base1_foo},
    [BASE2]   = {.bar_fn = (bar_t)base2_bar},
    [DERIVED] = {.foo_fn = (foo_t)derived_foo, .bar_fn = (bar_t)derived_bar}};

void use_base1(base1 *ptr) { g_vTableArr[ptr->Tag].foo_fn(ptr); }
void use_base2(base2 *ptr) { g_vTableArr[ptr->Tag].bar_fn(ptr); }

int main(void)
{
    base1 b1  = {.Tag = BASE1};
    base2 b2  = {.Tag = BASE2};
    derived d = {.Tag = DERIVED, .data = 10};

    use_base1(&b1);
    use_base1((base1 *)(&d));

    use_base2(&b2);
    use_base2((base2 *)(&d));
    return 0;
}

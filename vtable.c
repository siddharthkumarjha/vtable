#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct Animal Animal;
typedef void (*speakfn_t)(void);
typedef void (*shitfn_t)(Animal *);

typedef struct vTable
{
    speakfn_t speakfn;
    shitfn_t shitfn;
} vTable;

#define BASE_ELEM                                                                                  \
    size_t vTableTag;                                                                              \
    char collectShit[29];                                                                          \
    size_t sizeofShit;

#define DEFINE_STRUCT(NAME, ...)                                                                   \
    typedef struct NAME                                                                            \
    {                                                                                              \
        BASE_ELEM                                                                                  \
        __VA_ARGS__                                                                                \
    } NAME

DEFINE_STRUCT(Animal);
DEFINE_STRUCT(Dog);
DEFINE_STRUCT(Cat);

void dogspeak(void) { printf("*Bark*\n"); }
void catspeak(void) { printf("*Mew*\n"); }

#define ARR_SZ(X) (sizeof(X) / sizeof(X[0]))
#define MIN(X, Y)                                                                                  \
    ({                                                                                             \
        __auto_type __a = (X);                                                                     \
        __auto_type __b = (Y);                                                                     \
        __a < __b ? __a : __b;                                                                     \
    })

void dogshit(Animal *ptr)
{
    const char shit[] = "Dog Shit";

    const size_t min_sz = MIN(ARR_SZ(shit), ARR_SZ(ptr->collectShit));
    memcpy(ptr->collectShit, shit, min_sz);
    ptr->sizeofShit = min_sz;
}

void catshit(Animal *ptr)
{
    const char shit[] = "cat Shit";

    const size_t min_sz = MIN(ARR_SZ(shit), ARR_SZ(ptr->collectShit));
    memcpy(ptr->collectShit, shit, min_sz);
    ptr->sizeofShit = min_sz;
}

void printshit(Animal *ptr)
{
    if (ptr->sizeofShit <= 0)
    {
        const char shit[]    = "**Empty Shit**\n";
        const size_t shit_sz = ARR_SZ(shit);
        write(STDOUT_FILENO, shit, shit_sz);
    }
    else
    {
        write(STDOUT_FILENO, ptr->collectShit, ptr->sizeofShit);
        putchar('\n');
    }
}

const vTable g_vTablePtrArr[] = {{.speakfn = dogspeak, .shitfn = dogshit},
                                 {.speakfn = catspeak, .shitfn = catshit}};

void callAnimalSpeak(Animal *ptr) { g_vTablePtrArr[ptr->vTableTag].speakfn(); }
void callAnimalShit(Animal *ptr) { g_vTablePtrArr[ptr->vTableTag].shitfn(ptr); }

int main(void)
{
    Dog d = (Dog){.vTableTag = 0};
    Cat c = (Cat){.vTableTag = 1};

    Animal *ptrArr[] = {(Animal *)&d, (Animal *)&c, NULL};

    for (Animal **ItrAnimalArr = ptrArr; *ItrAnimalArr != NULL; ++ItrAnimalArr)
    {
        callAnimalSpeak(*ItrAnimalArr);
    }

    for (Animal **ItrAnimalArr = ptrArr; *ItrAnimalArr != NULL; ++ItrAnimalArr)
    {
        callAnimalShit(*ItrAnimalArr);
    }

    for (Animal **ItrAnimalArr = ptrArr; *ItrAnimalArr != NULL; ++ItrAnimalArr)
    {
        printshit(*ItrAnimalArr);
    }

    return 0;
}

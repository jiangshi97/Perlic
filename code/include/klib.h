#include <stdint.h>

#define null_ptr 0x0

typedef struct kNode kNode;
struct kNode
{
    kNode *succ;
    kNode *pred;
    void *ptr;
};
typedef struct
{
    kNode *begin;
    kNode *end;
} kList;

kList *create_list_by_array(kNode nodes[], uint64_t num);
kNode *find_node_by_num(kList *node, uint64_t num);

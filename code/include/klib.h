#include <stdint.h>

#define null_ptr 0x0

typedef struct kNode kNode;
struct kNode
{
    kNode *succ;
    void *ptr;
};

kNode *create_list_by_array(kNode nodes[], uint64_t num);
kNode *find_node_by_num(kNode *node, uint64_t num);
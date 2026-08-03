typedef struct kNode kNode;
struct kNode
{
    kNode *succ;
    void *ptr;
};
#include "klib.h"
#include "lib.h"
#include <stdint.h>

kNode *create_list_by_array(kNode nodes[], uint64_t num)
{
    for(uint64_t a = 0; a< (num - 1); a++)
    {
        nodes[a].succ = &(nodes[a + 1]);
    }
    nodes[num].succ = null_ptr;
    return &(nodes[0]);
}
kNode *find_node_by_num(kNode *node, uint64_t num)
{
    kNode *now = node;
    if(now == null_ptr)
    {
        ERR_CODE[get_pid()] = OS_FAILED;
        return null_ptr;
    }
    for(uint64_t a = 0; a < num; a++)
    {
        if(now->succ == null_ptr)
        {
            ERR_CODE[get_pid()] = OS_FAILED;
            return null_ptr;
        }
        else
        {
            now = now->succ;
        }
    }
    if(now == null_ptr)
    {
        ERR_CODE[get_pid()] = OS_FAILED;
        return null_ptr;
    }
    else
    {
        ERR_CODE[get_pid()] = OS_SUCCES;
        return now;
    }
}
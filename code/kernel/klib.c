#include "klib.h"
#include "lib.h"
#include "memlib.h"
#include <stdint.h>

_OS_API kList *create_list_by_array(kNode nodes[], uint64_t num)
{
    if(num == 0)
    {
        RetErr(null_ptr);
    }
    else if(num == 1)
    {
        nodes[0].succ = null_ptr;
        nodes[0].pred = null_ptr;
    }
    else
    {
        for(uint64_t a = 0; a < (num - 1); a++)
        {
            nodes[a].succ = &(nodes[a + 1]);
        }
        for(uint64_t a = 1; a < num; a++)
        {
            nodes[a].pred = &(nodes[a - 1]);
        }
        nodes[num - 1].succ = null_ptr;
        nodes[0].pred = null_ptr;
    }
 
    kList *list = kmalloc(sizeof(kList));
    CheckWithRet(null_ptr);
    list -> begin = &(nodes[0]);
    list -> end = &(nodes[num - 1]);
    RetSucces(list);
}
_OS_API kNode *find_node_by_num(kList *node, uint64_t num)
{
    kNode *now = node -> begin;
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

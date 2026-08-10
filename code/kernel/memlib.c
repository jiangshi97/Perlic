/*This function will use the third page,
 which means the kernel can only use 1 to 4mib memory to store elf section*/

#include "memlib.h"
#include "mem.h"
#include "lib.h"
#include "serial.h"
#include "klib.h"
#include <stdint.h>

#define heap_page_max 30 

MemPage *heapPage[heap_page_max];
uint16_t heap_page_num;


extern MemPages kuPages[max_page_num / 8];

static inline MemBlock *do_divide(MemBlock *target, MemBlock *dest_addr)    //No error check
{
    uint64_t size_a, size_b;
    size_a = (uintptr_t)dest_addr - (uintptr_t)target;
    size_b = (target -> block_size) - size_a;
    dest_addr -> block_size = size_b;
    dest_addr -> pred = target; 
    dest_addr -> succ = target -> succ;
    target -> block_size = size_a;
    target -> succ = dest_addr;
    return target;
}

_OS_API MemBlock *divide_block(MemBlock *target, uint64_t size, void *dest_addr)
{
    //debug_printf("arg: target:0x%x size:0x%x dest_addr:0x%x\n", target, size, dest_addr);
    //debug_printf("target -> block_size:0x%x\n", target -> block_size);
    //debug_printf("dest_addr <= (void *)target:%d\n", dest_addr <= (void *)target);
    if((target == null_ptr) 
    || (dest_addr == null_ptr) 
    || (dest_addr <= (void *)target) 
    || (size < sizeof(MemBlock))
    || !(is_free(target -> block_size)))
    {
        //debug_printf("error1\n");
        RetErr(null_ptr);
    }
    uint64_t size_a, size_b, size_c;
    MemBlock *tar_a, *tar_b, *tar_c;
    tar_a = target;
    tar_b = (MemBlock *)dest_addr;
    tar_c = (MemBlock *)((uintptr_t)dest_addr + size);
    size_a = ((uintptr_t)tar_b - (uintptr_t)tar_a);
    size_b = size;
    if((target -> block_size) < (size_a + size_b))
    {
        //debug_printf("error2\n");
        RetErr(null_ptr);
    }
    size_c = (target -> block_size) - size_a - size_b;
    //debug_printf("tar_a:0x%x size_a:0x%x tar_b:0x%x size_b:0x%x tar_c:0x%x size_c:0x%x \n", tar_a, size_a, tar_b, size_b, tar_c, size_c);
    if(size_a == 0)
    {
        if(size_c >= sizeof(MemBlock))
        {
            do_divide(tar_a, tar_c);
            free_to_use(&(tar_a -> block_size));
            RetSucces(target);
        }
        else
        {
            free_to_use(&(target -> block_size));
            RetSucces(target);
        }
    }
    else if(size_a >= sizeof(MemBlock))
    {
        if(size_c >= sizeof(MemBlock))
        {
            do_divide(tar_a, tar_b);
            do_divide(tar_b, tar_c);
            free_to_use(&(tar_b -> block_size));
            RetSucces(target);
        }
        else
        {
            do_divide(tar_a, tar_b);
            free_to_use(&(tar_b -> block_size));
            RetSucces(target);
        }
    }
    else
    {
        //debug_printf("error3\n");
        RetErr(null_ptr);
    }
}

static inline void *do_merge(MemBlock *first_block) //No error check
{
    use_to_free(&(first_block -> block_size));
    use_to_free(&(first_block -> succ -> block_size));
    first_block -> block_size +=  first_block -> succ -> block_size;
    first_block -> succ = first_block -> succ -> succ;
}
void mem_lib_init()
{
    heap_page_num = os_first_page - 2;
    for(int a = 0; a < heap_page_num; a++)
    {
        heapPage[a] = &(kuPages[(a + 2) / 8].pages[(a + 2)%8]);
        ((MemBlock *)(heapPage[a]->virt_addr))->block_size = 0x200000;
        ((MemBlock *)(heapPage[a]->virt_addr))->pred = null_ptr;
        ((MemBlock *)(heapPage[a]->virt_addr))->succ = null_ptr;
    }

}
_OS_API void *kmallocWithAlign(size_t size, uint8_t align_num)
{
    //debug_printf("allocate start\n");
    uint64_t size_a = (uintptr_t)align8(size);
    //debug_printf("size_a:0x%x\n", size_a);
    if(align_num <= 1)
    {
        return kmalloc(size);
    }
    else
    {
        if(size_a <= page_size - sizeof(MemBlock))
        {
            uint8_t isFound = 0;
            for(uint16_t a = 0; a < heap_page_num; a++)
            {
                MemBlock *now_ptr = (MemBlock *)(heapPage[a] -> virt_addr);
                MemBlock *up_ptr = (MemBlock *)(heapPage[a] -> virt_addr + page_size);
                //debug_printf("0x%x\n", now_ptr -> block_size);
                while(now_ptr < up_ptr)
                {
                    //debug_printf("now_ptr:0x%x block_szie:0x%x pred:0x%x succ:0x%x\n", now_ptr, now_ptr -> block_size, now_ptr -> pred, now_ptr -> succ);
                    if(is_free(now_ptr -> block_size))
                    {
                        //debug_printf("align:%d now:0x%x\n", align_num, (uintptr_t)aligns(now_ptr -> mem, align_num));
                        if(((uintptr_t)aligns(now_ptr -> mem, align_num) + size_a)
                            <= ((uintptr_t)now_ptr + (now_ptr -> block_size)))
                        {
                            //debug_printf("prepare to divide block\n");
                            //debug_printf("(uintptr_t)(now_ptr -> mem) + sizeof(MemBlock):0x%x\n", (uintptr_t)(now_ptr -> mem) + sizeof(MemBlock));
                            //debug_printf("(uintptr_t)aligns((uintptr_t)(now_ptr -> mem) + sizeof(MemBlock), align_num):0x%x\n", (uintptr_t)aligns((uintptr_t)(now_ptr -> mem) + sizeof(MemBlock), align_num));
                            //debug_printf("align_num:%d\n", align_num);
                            divide_block(now_ptr, (size_a + sizeof(MemBlock)), (void *)((uintptr_t)aligns((uintptr_t)(now_ptr -> mem) + sizeof(MemBlock), align_num) - sizeof(MemBlock)));
                            if(ERR_CODE[get_pid()] == OS_SUCCES)
                            {
                                isFound = 1;
                                now_ptr = (void *)((uintptr_t)aligns((now_ptr -> mem) + sizeof(MemBlock), align_num) - sizeof(MemBlock));
                                break;
                            }
                            else
                            {
                                //debug_printf("error!\n");
                            }
                        }
                    }
                    if((now_ptr -> succ) == null_ptr)
                    {
                        break;
                    }
                    else
                    {
                        now_ptr = now_ptr -> succ;
                    }
                }
                if(isFound == 1)
                {
                    ERR_CODE[get_pid()] = OS_SUCCES;
                    return now_ptr -> mem;
                }
            }
            ERR_CODE[get_pid()] = OS_FAILED;
            return null_ptr;
        }
        else
        {
            debug_printf("Failed to allocate memory > 2mib\n");
            RetErr(null_ptr);
        }
    }
}
_OS_API void *kmalloc(size_t size)
{
    uint64_t size_a = (uintptr_t)align8(size);
    if(size_a <= page_size - sizeof(MemBlock))
    {
        uint8_t isFound = 0;
        for(uint16_t a = 0; a < heap_page_num; a++)
        {
            MemBlock *now_ptr = (MemBlock *)(heapPage[a] -> virt_addr);
            MemBlock *up_ptr = (MemBlock *)(heapPage[a] -> virt_addr + page_size);
            //debug_printf("0x%x\n", now_ptr -> block_size);
            while(now_ptr < up_ptr)
            {
                if(((now_ptr -> block_size & 0x1) == 0) && (((now_ptr -> block_size >> 1) << 1) >= (size_a + 2 * sizeof(MemBlock))) )
                {
                    isFound = 1;
                    MemBlock *next = (MemBlock *)(((uint8_t *)now_ptr) + sizeof(MemBlock) + size_a);
                    next -> succ = now_ptr -> succ;
                    now_ptr -> succ = next;
                    next -> pred = now_ptr;
                    next -> block_size = now_ptr -> block_size - (sizeof(MemBlock) + size_a);
                    now_ptr -> block_size = sizeof(MemBlock) + size_a + 1;
                    break;
                }
                else if(((now_ptr -> block_size & 0x1) == 0) && (((now_ptr -> block_size >> 1) << 1) >= (size_a + sizeof(MemBlock))))
                {
                    isFound = 1;
                    now_ptr -> block_size += 1;
                    break;
                }
                else if(now_ptr -> succ == null_ptr)
                {
                    break;
                }
                else
                {
                    now_ptr = now_ptr -> succ;
                }
            }
            if(isFound == 1)
            {
                ERR_CODE[get_pid()] = OS_SUCCES;
                return now_ptr -> mem;
            }
        }
        ERR_CODE[get_pid()] = OS_FAILED;
        return null_ptr;
    }
    else
    {
        debug_printf("Failed to allocate memory > 2mib\n")
        ERR_CODE[get_pid()] = OS_FAILED;
        return null_ptr;
    }
};
_OS_API void kfree(void *ptr)
{
    if((uintptr_t)ptr <= sizeof(MemBlock))
    {
        ERR_CODE[get_pid()] = OS_FAILED;
        return;
    }
    MemBlock *now = (MemBlock *)((uintptr_t)ptr - sizeof(MemBlock));
    use_to_free(&(now -> block_size));
    if((now -> pred) == null_ptr)
    {
        if((now -> succ) == null_ptr)
        {
            RetSucces();
        }
        else
        {
            if(is_free(now -> succ -> block_size))
            {
                do_merge(now);
            }
            RetSucces();
        }
    }
    else
    {
        if((now -> succ) == null_ptr)
        {
            if(is_free(now -> pred -> block_size))
            {
                do_merge(now -> pred);
            }
            RetSucces();
        }
        else
        {
            if(is_free(now -> succ -> block_size))
            {
                do_merge(now);
            }
            if(is_free(now -> pred -> block_size))
            {
                do_merge(now -> pred);
            }
            RetSucces();
        }
    }
}
_OS_API void *kmalloc_init(size_t size)
{
    void *ptr = kmalloc(size);
    uint64_t *ptr_a;
    uint8_t *ptr_b;
    size_t s_a, s_b;
    if(ERR_CODE[get_pid()] == OS_SUCCES)
    {
        s_a = size / 8;
        s_b = size % 8;
        ptr_a = ptr;
        ptr_b = (uint8_t *)((uintptr_t)ptr + 8 * s_a);
        for(size_t a = 0; a < s_a; a++)
        {
            ptr_a[a] = 0ULL;
        }
        for(size_t a = 0; a < s_b; a++)
        {
            ptr_b[a] = 0;
        }
    }
}
_OS_API void *kmallocWithAlign_init(size_t size, uint8_t align_num)
{
    void *ptr = kmallocWithAlign(size, align_num);
    uint64_t *ptr_a;
    uint8_t *ptr_b;
    size_t s_a, s_b;
    if(ERR_CODE[get_pid()] == OS_SUCCES)
    {
        s_a = size / 8;
        s_b = size % 8;
        ptr_a = ptr;
        ptr_b = (uint8_t *)((uintptr_t)ptr + 8 * s_a);
        for(size_t a = 0; a < s_a; a++)
        {
            ptr_a[a] = 0ULL;
        }
        for(size_t a = 0; a < s_b; a++)
        {
            ptr_b[a] = 0;
        }
    }
    return ptr;
}
void divide_error();
void nmi();
void overflow();
//extern void *bounds;
void double_fault();
void coprocessor_segment_overrun();
//extern void *invalid_TSS;
void segment_not_present();
void stack_segment_fault();
void general_protection();
void x87_FPU_error();
void page_fault();
void alignment_check();
void machine_check();
void SIMD_exception();

void default_gate();
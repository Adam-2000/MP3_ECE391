#include "pit.h"

static int terminals_on[3];

void pit_init(){
    outb(PIT_MODE_SET, PIT_CMD_PORT);
    outb(PIT_FREQ_SET & 0xFF, PIT_DATA_PORT0);
    outb((PIT_FREQ_SET >> 8), PIT_DATA_PORT0);

    terminals_on[0] = 0;
    terminals_on[1] = 0;
    terminals_on[2] = 0;
    enable_irq(IRQ_TIMER_CHIP);
}

int32_t PIT_handler_helper(){
    int old_terminal_id = terminals.idx_active;
    //printf("%d", old_terminal_id);
    terminals.idx_active = (terminals.idx_active + 1) % 3;
    // 5 is default as initial
    if(old_terminal_id != 5){
        terminals.terminal[old_terminal_id].screen_x = get_cursor_x();
        terminals.terminal[old_terminal_id].screen_y = get_cursor_y();
        terminals.terminal[old_terminal_id].pcb_ptr = (pcb_t*)get_pcb_ptr();
        terminals.terminal[old_terminal_id].pid = cur_process_number;
    }
    
    set_cursor(terminals.terminal[terminals.idx_active].screen_x, terminals.terminal[terminals.idx_active].screen_x);
    set_pcb_ptr(terminals.terminal[terminals.idx_active].pcb_ptr);
    cur_process_number = terminals.terminal[terminals.idx_active].pid;
    if(terminals.idx_active == terminals.idx_on_screen){
        set_video_mem(VEDIO_MEM);
    } else{
        set_video_mem(VEDIO_MEM + (terminals.idx_active + 1) * PAGE_SIZE_SMALL);
    }
    tss.esp0 = KERNEL_END - KERNEL_STACK_WIDTH * cur_process_number - 4;
    set_paging_directory(KERNEL_END + (cur_process_number - 1) * PAGE_SIZE_BIG);
    send_eoi(IRQ_TIMER_CHIP);
    return (int32_t)terminals.terminal[terminals.idx_active].pcb_ptr;
}



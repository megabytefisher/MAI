// mips_memory.h
//

#pragma once

// linked list containing each data entry part
typedef struct data_entry_ {
    char name[56];
    char type[56];
    char* data;
    
    struct data_entry_* next;
} data_entry;

// linked list containing each text entry part
typedef struct text_entry_ {
    char label[56];
    char instruction[56];
    
    struct text_entry_* next;
} text_entry;

typedef struct {
    int zero;
    int at;
    int v0;
    int v1;
    int a0;
    int a1;
    int a2;
    int a3;
    int t0;
    int t1;
    int t2;
    int t3;
    int t4;
    int t5;
    int t6;
    int t7;
    int s0;
    int s1;
    int s2;
    int s3;
    int s4;
    int s5;
    int s6;
    int s7;
    int s8;
    int s9;
    int k0;
    int k1;
    int gp;
    int sp;
    int fp;
    int ra;
    
    int pc;
    int hi;
    int lo;
    
    void* stackBottom;
    
    int instruction_counter;
    data_entry* data_section;
    text_entry* text_section;
} mips_state;


void initialize_stack(mips_state *registers);
void destroy_stack(mips_state *registers);
int* get_register_ptr(mips_state *state, char* registerName);
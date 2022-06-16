#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK   memory_map[MAPMAX],int *map_cnt, int process_id, int last_address);
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt);

//Function used to initialize NULLBLOCK
struct MEMORY_BLOCK NULLBLOCK() {
    struct MEMORY_BLOCK nullblock;
    nullblock.start_address = 0;
    nullblock.end_address = 0;
    nullblock.segment_size = 0;
    nullblock.process_id = 0;
    return nullblock;
};

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    struct MEMORY_BLOCK nullblock = NULLBLOCK(); //Initialize nullblock
    struct MEMORY_BLOCK selection_map[MAPMAX];
    int selection = 0;
    int remaining_memory = 0;
    int init_map_cnt = *map_cnt; //initial queue_cnt
    //Iterate through memory_map: if free block w/ segment same as request size,
    //then map process_id to the segment and return memory_map index block
    //Fill selection_map with absolute values of segements in memory_map - request_size
    for(int i = 0 ; i < *map_cnt; i++) {
        if(memory_map[i].segment_size == request_size && memory_map[i].process_id == 0) {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
    }
    //Initialize a selection variable
    for(int i = 0; i < *map_cnt; i++) {
        if(memory_map[i].process_id == 0) {
            selection = i;
        }
    }
    //Select best fit index
    for(int i = 0; i < *map_cnt; i++) {
        if(memory_map[i].segment_size < memory_map[selection].segment_size && memory_map[i].process_id == 0) {
            selection = i;
        }
    }
    //If free block larger in size, split the free block in 2 and update first block to be size and ID of request process
    //and second block set to have remaining free space and ID of 0
    if(memory_map[selection].segment_size > request_size && memory_map[selection].process_id == 0) {
        remaining_memory = memory_map[selection].segment_size - request_size;
        memory_map[selection].segment_size = request_size;
        memory_map[selection].end_address = memory_map[selection].start_address + request_size - 1;
        memory_map[selection].process_id = process_id;
        (*map_cnt)++;
        //If selection+1 (+2 to account for array index starting from zero), is out of bounds
        //then the new free block can be added in index selection+1
        if(selection+2 > *map_cnt) {
            memory_map[selection+1].segment_size = remaining_memory;
            memory_map[selection+1].start_address = memory_map[selection].end_address + 1;
            memory_map[selection+1].end_address = memory_map[selection+1].start_address + memory_map[selection+1].segment_size - 1;
            memory_map[selection+1].process_id = 0;
            return memory_map[selection];
        } else { //Else memory blocks ahead of free block are shifted one up and free block 
                 //created right after selection block
            for(int i = *map_cnt; i >= selection+1; i--) {
                memory_map[i] = memory_map[i-1];
            }
            memory_map[selection+1].segment_size = remaining_memory;
            memory_map[selection+1].start_address = memory_map[selection].end_address + 1;
            memory_map[selection+1].end_address = memory_map[selection+1].start_address + memory_map[selection+1].segment_size - 1;
            memory_map[selection+1].process_id = 0;
            return memory_map[selection];
        }
        
    } else { //If segment size is too small, return NULLBLOCK
        return nullblock;
    }
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    struct MEMORY_BLOCK nullblock = NULLBLOCK(); //Initialize nullblock
    int selection = 0;
    int remaining_memory = 0;
    int init_map_cnt = *map_cnt; //initial queue_cnt
    //Initialize selection with index of free block
    for(int i = 0; i < *map_cnt; i++) {
        if(memory_map[i].process_id == 0) {
            selection = i;
            break;
        }
    }
    //Select first fit index
    for(int i = 0; i < *map_cnt; i++) {
        if(memory_map[i].start_address < memory_map[selection].start_address && memory_map[i].process_id == 0) {
            selection = i;
        }
    }
    //If selection is equal to request size, then update the process_id of the block and return "selection" index block
    if(memory_map[selection].segment_size == request_size && memory_map[selection].process_id == 0) {
            memory_map[selection].process_id = process_id;
            return memory_map[selection];
        }
    //If free block larger in size, split the free block in 2 and update first block to be size and ID of request process
    //and second block set to have remaining free space and ID of 0
    //If free block larger in size, split the free block in 2 and update first block to be size and ID of request process
    //and second block set to have remaining free space and ID of 0
    if(memory_map[selection].segment_size > request_size && memory_map[selection].process_id == 0) {
        remaining_memory = memory_map[selection].segment_size - request_size;
        memory_map[selection].segment_size = request_size;
        memory_map[selection].end_address = memory_map[selection].start_address + request_size - 1;
        memory_map[selection].process_id = process_id;
        (*map_cnt)++;
        //If selection+1 (+2 to account for array index starting from zero), is out of bounds
        //then the new free block can be added in index selection+1
        if(selection+2 > *map_cnt) {
            memory_map[selection+1].segment_size = remaining_memory;
            memory_map[selection+1].start_address = memory_map[selection].end_address + 1;
            memory_map[selection+1].end_address = memory_map[selection+1].start_address + memory_map[selection+1].segment_size - 1;
            memory_map[selection+1].process_id = 0;
            return memory_map[selection];
        } else { //Else memory blocks ahead of free block are shifted one up and free block 
                 //created right after selection block
            for(int i = *map_cnt; i >= selection+1; i--) {
                memory_map[i] = memory_map[i-1];
            }
            memory_map[selection+1].segment_size = remaining_memory;
            memory_map[selection+1].start_address = memory_map[selection].end_address + 1;
            memory_map[selection+1].end_address = memory_map[selection+1].start_address + memory_map[selection+1].segment_size - 1;
            memory_map[selection+1].process_id = 0;
            return memory_map[selection];
        }
    } else { //If segment size is too small, return NULLBLOCK
        return nullblock;
    }
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    struct MEMORY_BLOCK nullblock = NULLBLOCK(); //Initialize nullblock
    int init_map_cnt = *map_cnt; //initial queue_cnt
    //Iterate through memory_map: if free block w/ segment same as request size,
    //then map process_id to the segment and return memory_map index block
    for(int i = 0 ; i < *map_cnt; i++) {
        if(memory_map[i].segment_size == request_size && memory_map[i].process_id == 0) {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
    }
    //If free block larger in size, split the free block in 2 and update first block to be size and ID of request process
    //and second block set to have remaining free space and ID of 0
    if(memory_map[*map_cnt-1].segment_size > request_size && memory_map[*map_cnt-1].process_id == 0) {
        (*map_cnt)++;
        memory_map[*map_cnt-1].segment_size = memory_map[*map_cnt-2].segment_size - request_size;
        memory_map[*map_cnt-2].segment_size = request_size;
        memory_map[*map_cnt-2].end_address = memory_map[*map_cnt-2].start_address + request_size - 1;
        memory_map[*map_cnt-2].process_id = process_id;
        memory_map[*map_cnt-1].start_address = memory_map[*map_cnt-2].end_address + 1;
        memory_map[*map_cnt-1].end_address = memory_map[*map_cnt-1].start_address + memory_map[*map_cnt-1].segment_size - 1;
        memory_map[*map_cnt-1].process_id = 0;
        return memory_map[*map_cnt-2];
    } else { //If segment size is toos small, return NULLBLOCK
        return nullblock;
    }
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, 
int last_address) {
    struct MEMORY_BLOCK nullblock = NULLBLOCK(); //Initialize nullblock
    int init_map_cnt = *map_cnt; //initial queue_cnt
    //Iterate through memory_map: if free block w/ segment same as request size,
    //then map process_id to the segment and return memory_map index block
    for(int i = 0 ; i < *map_cnt; i++) {
        if(memory_map[i].segment_size == request_size && memory_map[i].process_id == 0) {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
    }
    //If free block larger in size, split the free block in 2 and update first block to be size and ID of request process
    //and second block set to have remaining free space and ID of 0
    if(memory_map[*map_cnt-1].segment_size > request_size && memory_map[*map_cnt-1].process_id == 0) {
        (*map_cnt)++;
        memory_map[*map_cnt-1].segment_size = memory_map[*map_cnt-2].segment_size - request_size;
        memory_map[*map_cnt-2].segment_size = request_size;
        memory_map[*map_cnt-2].end_address = memory_map[*map_cnt-2].start_address + request_size - 1;
        memory_map[*map_cnt-2].process_id = process_id;
        memory_map[*map_cnt-1].start_address = memory_map[*map_cnt-2].end_address + 1;
        memory_map[*map_cnt-1].end_address = memory_map[*map_cnt-1].start_address + memory_map[*map_cnt-1].segment_size - 1;
        memory_map[*map_cnt-1].process_id = 0;
        return memory_map[*map_cnt-2];
    } else { //If segment size is toos small, return NULLBLOCK
        return nullblock;
    }
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt) {
    struct MEMORY_BLOCK nullblock = NULLBLOCK(); //Initialize nullblock
    int init_map_cnt = *map_cnt; //Initial queue_cnt
    int pos; //Holds positions of memory block to be released
    int merged = 0; //Keeps track if merged once already
    //Find the process to be freed, mark the process ID as 0, and save the index position as pos
    for(int i = 0 ; i < *map_cnt; i++) {
        if(memory_map[i].process_id == freed_block.process_id) {
            memory_map[i].process_id = 0;
            pos = i;
            break;
        }
    }
    //If pos , check if adjacenet process is free and if so, update it to combine the two
    if(pos > 0 && pos+1 < *map_cnt) {
        if(memory_map[pos-1].process_id == 0) {
            memory_map[pos-1].segment_size += memory_map[pos].segment_size;
            memory_map[pos-1].end_address = memory_map[pos].segment_size + memory_map[pos].start_address - 1;
            (*map_cnt)--;
            merged = 1;
            //Shift over all the elements in the array to remove the extra element now that they're combined
            for(int i = pos ; i < *map_cnt; i++) {
                memory_map[i] = memory_map[i+1];
            }
        }
        //If merged above, look to pos-1 next, otherwise look to pos+1
        if(merged == 1) {
            //Check if position adjacenet to block is free, if so repeat the above
            if(memory_map[pos].process_id == 0) {
                memory_map[pos-1].segment_size += memory_map[pos].segment_size;
                memory_map[pos-1].end_address = memory_map[pos].segment_size + memory_map[pos].start_address - 1;
                (*map_cnt)--;
                //Shift over all the elements in the array to remove the extra element now that they're combined
                for(int i = pos; i < *map_cnt; i++) {
                    memory_map[i] = memory_map[i+1];
                }
                return;
        }
        } else if(memory_map[pos+1].process_id == 0) {
            memory_map[pos+1].segment_size += memory_map[pos].segment_size;
            memory_map[pos+1].process_id = memory_map[pos].process_id;
            memory_map[pos+1].start_address = memory_map[pos].start_address;
            (*map_cnt)--;
            for(int i = pos; i < *map_cnt; i++) {
                memory_map[i] = memory_map[i+1];
            }
            return;
        } else {
            return;
        }
    } else if(pos == 0) {
        //If pos is 0, only check if one adjacent block is free and repeat process above
        if(memory_map[pos+1].process_id == 0) {
            memory_map[pos+1].segment_size += memory_map[pos].segment_size;
            memory_map[pos+1].process_id = memory_map[pos].process_id;
            memory_map[pos+1].start_address = memory_map[pos].start_address;
            (*map_cnt)--;
        }
        //Shift over all the elements in the array to remove the extra element now that they're combined
        for(int i = pos; i < *map_cnt; i++) {
            memory_map[i] = memory_map[i+1];
        }
        return;
    } else if(pos+1 >= *map_cnt) {
        //If pos is >= map count, only check if one adjacenet block is free and repeat process above
        if(memory_map[pos-1].process_id == 0) {
            memory_map[pos-1].segment_size += memory_map[pos].segment_size;
            memory_map[pos-1].end_address = memory_map[pos].segment_size + memory_map[pos].start_address - 1;
            (*map_cnt)--;
        }
        //Shift over all the elements in the array to remove the extra element now that they're combined
        for(int i = pos; i < *map_cnt; i++) {
            memory_map[i] = memory_map[i+1];
        }
        return;
    }
}
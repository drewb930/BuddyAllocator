#include "BuddyAllocator.h"
#include <iostream>
#include<stdio.h>
#include<string>
#include<memory.h>
#include <math.h>
#include <cstdint>

using namespace std;

BuddyAllocator::BuddyAllocator(int _basic_block_size, int _total_memory_length) {
	basic_block_size = _basic_block_size, total_memory_size = _total_memory_length;
	
	int size = _total_memory_length;
	
	int count = 0;
	
	while(size > basic_block_size) {
		size /= 2;
		count++;
	}
	
	largest_index = count;
	
	FreeList.resize(count + 1, LinkedList());
	
	start_addr = new char[_total_memory_length];
	
	BlockHeader* block = make_block(_total_memory_length, start_addr, 0); // 0 here doesnt matter
	
	
}

BuddyAllocator::~BuddyAllocator() {

}

void* BuddyAllocator::alloc(int length) {
	/* This preliminary implementation simply hands the call over the
	   the C standard library!
	   Of course this needs to be replaced by your implementation.
	*/
	int size =this->total_memory_size;
	int count = 0;
	while(length < size/2 && size!= this -> basic_block_size) {
		size /= 2;
		count++;
	}
	if (size == this->total_memory_size) {
		return malloc(size);
	}
	else {
		int start_index = get_start_index(size);
		while(FreeList.at(start_index).head->block_size != size) {
			BlockHeader* curr = FreeList.at(start_index).head;
			while(curr->next != NULL) {
				curr = curr->next;
			}
			split(curr,start_index,size);
			start_index--;
		}
		BlockHeader* curr = FreeList.at(start_index).head;
		while(curr->next != NULL) {
			curr = curr->next;
		}
		curr-> in_free_list =false;
		return curr;
	}
	
}

int BuddyAllocator::get_start_index(int size){
	
	int size_index = 0;
	int curr_size = this->basic_block_size;
	while(curr_size != size){
		curr_size *= 2;
		size_index++;
	}
	
	while(FreeList.at(size_index).head == NULL) {
		size_index++;
	}
	return size_index;
}

int BuddyAllocator::get_end_index(int size) {
	int index = 0;
	int start_size = this->basic_block_size;
	while(start_size != size) {
		index++;
		start_size *= 2;
	}
	
	return index;
	
	
}

BlockHeader* BuddyAllocator::make_block(int size, char* address, int target_size){

	 BlockHeader* block = reinterpret_cast<BlockHeader*>(address);
	
	block->next = NULL;
	block->block_size = size;
	block->in_free_list = true;
	
	int index = FreeList.size() - 1;
	int curr_size = this->total_memory_size;
	while(curr_size > size){
		index--;
		curr_size /= 2;
	}
	
	if(size != target_size)FreeList.at(index).LinkedList::insert(block);
	
	return block;
	
}
void BuddyAllocator::split(BlockHeader* node, int i,int target_size){
	
	int size = FreeList.at(i).head->block_size / 2;
	node -> block_size = size;
	
	
	char* curr_block = reinterpret_cast<char*>(node);
	//confusion here
	char* next_block = new char[this->total_memory_size];
	memcpy(next_block, curr_block, sizeof(curr_block));
	next_block += size;
	
	
	FreeList.at(i-1).LinkedList::insert(node);
	FreeList.at(i).remove(node);
	
	BlockHeader* block2 = make_block(size,next_block,target_size);
	
	
	
	
	
	cout << "Here" << endl;
	
}


void BuddyAllocator::free(void* a) {
	/* Same here! */
	//::free(a);
}

void BuddyAllocator::printlist() {
	cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
	int64_t total_free_memory = 0;
	for (int i = 0; i < FreeList.size(); i++) {
		int blocksize = ((1 << i) * basic_block_size); // all blocks at this level are this size
		cout << "[" << i << "] (" << blocksize << ") : ";  // block size at index should always be 2^i * bbs
		int count = 0;
		BlockHeader* b = FreeList[i].head;
		// go through the list from head to tail and count
		while (b) {
			total_free_memory += blocksize;
			count++;
			// block size at index should always be 2^i * bbs
			// checking to make sure that the block is not out of place
			if (b->block_size != blocksize) {
				cerr << "ERROR:: Block is in a wrong list" << endl;
				exit(-1);
			}
			b = b->next;
		}
		cout << count << endl;
		cout << "Amount of available free memory: " << total_free_memory << " bytes" << endl;
	}
}
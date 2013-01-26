#ifndef __MEM_H__
#define __MEM_H__

#define MEM_CHECK(_item) \
	if(_item == NULL) { \
		die("Could not allocate memory."); \
	}

#endif
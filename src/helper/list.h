#ifndef __LIST_H__
#define __LIST_H__

typedef node_t;
typedef struct __node
{
	void *data;
	node_t *next;
} node_t;

#endif

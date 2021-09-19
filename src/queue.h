#ifndef _QUEUE_H_
#define _QUEUE_H_

struct Queue;

struct Queue* empty__queue();
int queue__is_empty(struct Queue *queue);
void enqueue(struct Queue *queue, int nwNumber);
int dequeue(struct Queue* queue);
void queue__free(struct Queue *queue);
void print_queue(struct Queue *queue);

#endif

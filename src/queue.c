// From : https://openclassrooms.com/fr/courses/19980-apprenez-a-programmer-en-c/19868-les-piles-et-les-files

#include <stdio.h>
#include <stdlib.h>

typedef struct Element Element;
struct Element
{
    int number;
    Element *next;
};

typedef struct Queue Queue;
struct Queue
{
    Element *head;
};

Queue* empty__queue(){
  Queue* queue = malloc(sizeof(*queue));
  queue -> head = NULL;
  return queue;
}

int queue__is_empty(Queue *queue){
  return queue -> head == NULL;
}

void enqueue(Queue *queue, int nwNumber)
{
    Element *new = malloc(sizeof(*new));
    if (queue == NULL || queue == NULL)
    {
        exit(EXIT_FAILURE);
    }

    new -> number = nwNumber;
    new -> next = NULL;

    if (queue -> head != NULL) /* Queue isn't empty */
    {
        /* Goes at the end of the queue */
        Element *currentElement = queue -> head;
        while (currentElement -> next != NULL)
        {
            currentElement = currentElement -> next;
        }
        currentElement -> next = new;
    }
    else /* Queue is empty, it's the first element */
    {
        queue -> head = new;
    }
}

int dequeue(Queue *queue)
{
    if (queue == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int queueNumber = 0;

    /* Checks if there is sth to dequeue */
    if (queue -> head != NULL)
    {
        Element *queueElement = queue -> head;

        queueNumber = queueElement -> number;
        queue -> head = queueElement -> next;
        free(queueElement);
    }

    return queueNumber;
}

void queue__free(Queue *queue){
  Element *elt = queue -> head;
  Element *elt_tmp;
  while (elt != NULL){
    elt_tmp = elt;
    elt = elt -> next;
    free (elt_tmp);
  }
  free(queue);
}

void print_queue(Queue *queue){
  printf("[");
  Element *elt = queue -> head;
  
  while (elt -> next != NULL){
    printf("%d, ", elt -> number);
    elt = elt -> next;
  }
  printf("%d] \n", elt -> number);
}

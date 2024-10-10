#pragma GCC diagnostic ignored "-Wformat"      // switches of format
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC optimise ("", off)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define INIT -128                      // The queue should utilize -128 to signify empty queue elements
#define UNDERFLOW (0x80 + 0x01)        // When a dequeue operation encounters an underflow, it should return -127
#define OVERFLOW 0x80 + 0x02                  
#define BADPTR (0x08 + 0x03)
#define PQLIMIT (unsigned long) 1.20E1

// for machine to machine consistency
typedef float    f32;
typedef double   f64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef size_t   usize;
typedef ssize_t  isize;

// queue the element into list
typedef struct QNodeData_t {
    char key;
    u32 prio;
} QNodeData_t;

// last element in list
typedef struct QNode {


    // each element of a doubly linked list L is an object with key and priority and next and prev pointers
    // given an element x in the list, x.nexts points to its successor in the linked list
    // and x.prev points to its pred
    // if x.prev == null, the element x has no pred and is therefore the first element, or head of the list
    // if x.next == null, the element x has no succ and is therefore the last element, or tail of the list

    // update the definitions to use a sentinel for checking the error conditions

    QNodeData_t element;       // the key stored at each list node  
    struct QNode *next, *prev; // these are the pointers to next and prev of list element;


} QNode_t;

// doubly linked list with sentinel yea
typedef struct DLLS {

    // unsorted circular doubly linked list with a sentinel
    // if sentinel.nextt = sentinel, the list is empty

    // a list may have one of several forms, it may be either singly linked or doubly linked
    // it may be sorted or not, and it may be circular or not

    // if the list was sorted, the linear order of the list would correspond to the linear order
    // of keys stored in the elements of the list
    // the minimum element then would be the head of the list, and the maximum element would be the tail

    // in a circular list, the pre pointer of the sentinel of the list points to the tail, and the next pointer
    // of the tail of the list to the sentinel

    // sentinel is a dummy object that allows us to simplify boundary conditions
    // whatever we have a reference to null in list code, we repalce it by reference to the sentinel
    // changing a regular doubly linked list into a circular, doubly linked list with a sentinel
    // in which the sentinel lies between head and tail

    QNode_t *sentinel; // NULL node, connects to head and tail for circle
} DLLS_t;

// priority queue
typedef struct PrioQ {
    DLLS_t *L;

    u32 maxSize, elementNum;
} PQ_t;

// list search(L, k) finds the first element with key k in list L by a simple linear search
// returning a pointer to this element, if no object with key k appears in the list, then the procedure
// returns null
// takes linear time in the worst case to find an element
QNode_t *ListSearch(DLLS_t *L, i16 k);


// given an element x whose key attribute has already been set, the listinsert procedure "splices"
// x onto the head of the list, runs in constant time
void ListInsert(DLLS_t *L, QNodeData_t x);

// removes an element x from the list, it must be given a pointer to x, and it "splices" x out of the list
// by updating pointers. runs in constant time
// if we wish to delete an element with a given key, we must first call list search to retrieve a pointer to element
// the caller may free the deleted element x
QNode_t *ListDelete(DLLS_t *L, QNode_t *x);

// the procedure removes the last element from the list
// the caller may free the deleted element.x
QNode_t *ListDeleteLast(DLLS_t *L);

// walks through the sequence L and prints its elements in order
void IterateList(DLLS_t *L);

void Enqueue();
char Dequeue();
char DequeueMax();
char FindMax();
PQ_t *Build(u32 maxlen);

// global Priority Queue
PQ_t* myQ;

QNode_t *ListSearch(DLLS_t *L, i16 k) {
    QNode_t *x;

    if (!L) {
        return NULL;        // list doesn't exist
    }
    else {
        if (!L->sentinel) {
            x = NULL;       // sentinel doesn't exist
        }
        else {
            x = L->sentinel->next;
            while (x != L->sentinel) {
                if (x->element.key == k) {
                    break;
                }
                x = x->next;
            }
        }
    }
    return x;
}

void ListInsert(DLLS_t *L, QNodeData_t x) {
    if (!L || !(L->sentinel)) {
        printf("\nList is empty\n");
        return; 
    }

    QNode_t *newNode = (QNode_t*)malloc(sizeof(QNode_t)); 
    if (!newNode) {
        printf("\nMemory allocation failed\n");
        return;
    }

    newNode->element.key = x.key;
    newNode->element.prio = x.prio;

    newNode->next = L->sentinel->next;
    newNode->prev = L->sentinel;

    // if list isnt empty, adjust previous pointer of the first element
    if (L->sentinel->next != L->sentinel) {
        L->sentinel->next->prev = newNode;
    }
    else {
        // if list is empty adjust sentinels prev pointer 
        L->sentinel->prev = newNode;
    }

    // update to new head
    L->sentinel->next = newNode;
}

// X TO BE FREED BY CALLER
// the parameter L is kept for compatibility
QNode_t* ListDelete(DLLS_t *L, QNode_t *x) {
    // checks if theres any null values
    if (x && x->prev && x->next) {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }

    return x;
}

// X TO BE FREED BY CALLER
QNode_t *ListDeleteLast(DLLS_t *L) {
    QNode_t *x = NULL;

    // assumming that the expression is evaluated from left to right
    if (!L || !(L->sentinel)) {
        printf("\nError: list or sentinel is invalid\n");
        exit(-1);
    }

    x = L->sentinel->prev;         // x finds tail
    x->prev->next = L->sentinel;   // tail prev to find back node next and connect to sentinel
    L->sentinel->prev = x->prev;   // sentinel prev points to back node before the "then" tail

    return x;
}

void IterateList(DLLS_t *L) {
    QNode_t *x = NULL;

    if (!L || (!L->sentinel)) {
        printf("\nError: list or sentinel is invalid\n");
        exit(-1);
    }

    // start at head
    x = L->sentinel->next;

    while (x != NULL && x != L->sentinel) {
        printf("\n key %d and priority %d", x->element.key, x->element.prio);
        x = x->next;
    }
}

int main(int argc, const char* argv[]) {
    i16 test = PQLIMIT;

    myQ = Build(test);
    if (myQ == NULL) {
        printf("\nBadpointer. \n");
        exit(-1);
    }

    QNodeData_t e;
    for (int i = 0; i < test; i++) {
        e.key = rand() % 127;
        e.prio = rand() % 4;
        Enqueue(myQ, e);
        printf("After enqueue(%d, %lu) counter takes %lu\n", e.key, e.prio, myQ->elementNum);
    }
    IterateList(myQ->L);
    Enqueue(myQ, e);

    i16 key = 25;
    QNode_t *list = ListSearch(myQ->L, key);
    if (list == NULL) {
        printf("\nBadpointer.\n");
    }
    else {
        if (list == myQ->L->sentinel) {
            printf("\n We could not find k = %d in the Queue\n", key);
        }
        else {
            printf("\n We found the list element containing %d at %p\n", list->element.key, list);
        }
    }

    printf("\nDequeuing element with max priority");
    char dqMaxVal = DequeueMax(myQ);
    printf("\nDequeued max priority element: %d\n", dqMaxVal);

    for (int i = 0; i < test; i++) {
        Dequeue(myQ);
        printf("\nAfter Dequeue() -> the counter takes %lu\n", myQ->elementNum);
    }

    Dequeue(myQ);

    printf("\n\nYou can store a maximum of %lu elements in your PQ (PQ->maxSize), where as maxSize of a PQ is capped at PQLIMIT,"
            " which is currently set to %lu in your program.\n\n\n", myQ->maxSize, PQLIMIT);

    while(myQ->elementNum > 0) {
        Dequeue(myQ);
    }

    if (myQ->L->sentinel)
        free(myQ->L->sentinel);
    if (myQ->L)
        free(myQ->L);
    if (myQ) 
        free(myQ);

    return 0;
}

PQ_t *Build(u32 maxlen) {
    PQ_t *pq = NULL;

    if (maxlen > 0 && maxlen <= PQLIMIT) {
        pq = (PQ_t*)malloc(sizeof(PQ_t));
        if (pq) {
            pq->maxSize = maxlen;
            pq->elementNum = 0;    // start count at zero

            // allocate memory for dlls
            pq->L = (DLLS_t*)malloc(sizeof(DLLS_t));

            // sentinel node setup
            if (pq->L) {
                pq->L->sentinel = (QNode_t*)malloc(sizeof(QNode_t));
                if (pq->L->sentinel) {
                    pq->L->sentinel->next = pq->L->sentinel;
                    pq->L->sentinel->prev = pq->L->sentinel;
                    pq->L->sentinel->element.key =  0;
                    pq->L->sentinel->element.prio = 0;
                }
                else {
                    printf("\nBuild()>> Failed to allocate memory for sentinel node - Bad pointer %x\n", BADPTR);
                    free(pq->L);
                    free(pq);
                    return NULL; 
                }
            }
            else {
                printf("\nBuild()>> Failed to allocate memory for the queue's list - Bad pointer %x\n", BADPTR);
                free(pq);
                return NULL;
            }
        }
        else {
            printf("\nBuild()>> Failed to allocate memory for the priority queue - Bad pointer %x\n", BADPTR);
            return NULL;
        }
    }
    else {
        printf("\nBuild()>> Invalid maxlen - %u\n", BADPTR);
        return NULL;
    }

    return pq;
}

void Enqueue(PQ_t* pq, QNodeData_t e) {
    if (pq->elementNum >= pq->maxSize) { 
        printf("\nEnqueue()>> Attempt to overflow the queue at %p was prevented.\n", pq);
        return;
    }
    // insert e into the list
    ListInsert(pq->L, e);
    pq->elementNum++;
}

char Dequeue(PQ_t* pq) {
    char val = -1;

    if(!pq || !pq->L || !pq->L->sentinel) {
        printf("\nDequeue()>> Invalid list or sentinel.\n");
        return (char) BADPTR;
    }
    if(pq->elementNum == 0) {
        printf("\nDequeue()>> Attempt to underflow the queue at %p was prevented.\n", pq);
        return (char) UNDERFLOW;
    }

    QNode_t *ptr = ListDeleteLast(pq->L);
    if (ptr && ptr != pq->L->sentinel) {
        val = ptr->element.key;
        pq->elementNum--;
        free(ptr);
    }
    
    return (char) val;
}

char DequeueMax(PQ_t* pq) {
    char val = -1;

    if(!pq || !pq->L || !(pq->L->sentinel)) {
        printf("\nDequeue()>> Invalid list or sentinel.\n");
        return (char) BADPTR;
    }
    if(pq->elementNum == 0) {
        printf("\nDequeue()>> Attempt to underflow the queue at %p was prevented.\n", pq);
        return (char) UNDERFLOW;
    }

    QNode_t *maxNode = NULL;
    QNode_t *x = pq->L->sentinel->next; 

    while (x != pq->L->sentinel) {
        if (!maxNode || x->element.prio > maxNode->element.prio) {
            maxNode = x;
        }
        x = x->next;
    }

    if (maxNode && maxNode != pq->L->sentinel) {
        val = maxNode->element.key;
        QNode_t *deletedNode = ListDelete(pq->L, maxNode);
        pq->elementNum--;

        if (deletedNode) {
            free(deletedNode);
        }
    }
    return (char) val;
}
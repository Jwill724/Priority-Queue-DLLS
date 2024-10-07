#pragma GCC diagnostic ignored "-Wformat"      // switches of format
#pragma GCC diagnostic ignored "-Wparantheses"
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
typedef struct QElement {
    char key;
    u32 prio;
} QElement_t;

// last element in list
typedef struct LElement {


    // each element of a doubly linked list L is an object with key and priority and next and prev pointers
    // given an element x in the list, x.nexts points to its successor in the linked list
    // and x.prev points to its pred
    // if x.prev == null, the element x has no pred and is therefore the first element, or head of the list
    // if x.next == null, the element x has no succ and is therefore the last element, or tail of the list

    // update the definitions to use a sentinel for checking the error conditions

    QElement_t element;      // the key stored at each list node  
    LElement_t *next, *prev; // these are the pointers to next and prev of list element;
    
     
} LElement_t;

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

    LElement_t *sentinel; // NULL node, connects to head and tail for circle
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
LElement_t *ListSearch(DLLS_t *L, i16 k);


// given an element x whose key attribute has already been set, the listinsert procedure "splices"
// x onto the head of the list, runs in constant time
void ListInsert(DLLS_t *L, QElement_t x);

// removes an element x from the list, it must be given a pointer to x, and it "splices" x out of the list
// by updating pointers. runs in constant time
// if we wish to delete an element with a given key, we must first call list search to retrieve a pointer to element
// the caller may free the deleted element x
LElement_t *ListDelete(DLLS_t *L, LElement_t *x);

// the procedure removes the last element from the list
// the caller may free the deleted element.x
LElement_t *ListDeleteLast(DLLS_t *L);

// walks through the sequence L and prints its elements in order
void Iterate(DLLS_t *L);

void Enqueue();
char Dequeue();
char DequeueMax();
char findMax();
PQ_t *Build(u32 maxlen);

PQ_t* myQ;
LElement_t *ListSearch(DLLS_t *L, i16 k) {
    if (!L || !L->sentinel) {
        return NULL;               // list or sentinel dont exist
    }

    LElement_t *x = L->sentinel->next;

    while(x != L->sentinel) {
        if (x->element.key == k) {
            return x;              // element found
        }
        x = x->next;
    }
    return NULL;                   // element not found
}

// change the parameter profile, x should be of type lelement_t, x should be initialized
// by the caller (within the enqueue() subprogram)
void ListInsert(DLLS_t *L, QElement_t x) {
    if (!L || !L->sentinel) {
        return NULL; 
    }

    LElement_t *newElem = (LElement_t*)malloc(sizeof(LElement_t)); 
    if (!newElem) {
        printf("\nMemory allocation failed\n");
        return;
    }

    newElem->element.key = x.key;
    newElem->element.prio = x.prio;

    newElem->next = L->sentinel->next;
    newElem->prev = L->sentinel;

    // if list isnt empty, adjust previous pointer to new element
    if (L->sentinel->next != L->sentinel) {
        L->sentinel->next->prev = newElem;
    }

    // update to new head
    L->sentinel->next = newElem;
}

// the parameter L is kept for compatibility. we are not using it. x must be freed by the caller
LElement_t* ListDelete(DLLS_t *L, LElement_t *x) {
    // if the order of the evaluation is from left to right, no problem
    // otherwise access to the null pointer is possible
    // please fix this so NO pointer is accessed, if it is NULL

    // checks if theres any null values
    if (x && x->prev && x->next)
    {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }

    return x;
}

LElement_t *ListDeleteLast(DLLS_t *L) {
    LElement_t *x = NULL;

    // assumming that the expression is evaluated from left to right
    if (!L || !(L->sentinel))
        exit(-1);
    x = // complete
    x->prev->next = // complete
    x->next->prev = // complete
    return x;
}
void Iterate(DLLS_t *L) {
    LElement_t *x;
    if (!L || !(L->sentinel))
        exit(-1);
    x = // complete

    while ( (x != NULL) && (x != L->sentinel) ) {
        printf("\n key %d and priority %d", x->element.key, x->element.prio);
        x = x->next;
    }
}

int main(int argc, const char* argv[]) {

    i16 test = PQLIMIT;

    myQ = build(test);
    if (myQ == NULL) {
        printf("\nBadpointer. \n");
        exit(-1);
    }

    QElement_t e;

    for (int i = 0; i < test; i++) {
        e.key == rand() % 127;
        e.prio == rand() % 4;
        Enqueue(myQ, e);        // contents of e passed to enqueue()-> by value and stored in myQ->L
        printf("After enqueue(%d, %lu) counter takes %lu\n", e.key, e.prio, myQ->elementNum);
    }
    Iterate(myQ->L);
    Enqueue(myQ, e);

    LElement_t *list = ListSearch(myQ->L, 25);
    if (list == NULL) {
        printf("\nBadpointer.\n");
    }
    else {
        if (list == myQ->L->sentinel) {
            printf("\n We could not find k = %d in the Queue\n", e.key);
        }
        else {
            printf("\n We could not find the list element containing %d at %p\n", list->element.key, list);
        }
    }

    // complete this section to dequeue the element that has the maximum priority. if several elements
    // have the same max priority, then the element enqueued first should be dequeued


    for (int i = 0; i < test; i++) {
        Dequeue(myQ);
        printf("After Dequeue() -> the counter takes %lu\n", myQ->elementNum);
    }

    Dequeue(myQ);

    printf("\n\nYou can store a maximum of %lu elements in your PQ (PQ->maxSize), where as maxSize of a PQ is capped at PQLIMIT,"
            " which is currently set to %lu in your program.\n\n\n", myQ->maxSize, PQLIMIT);

    // free anything that was created with malloc
    // dont forget to free all elements of the list in the myQ->L before freeing L->sentinel
    // walk through the list and free all elements
    // compelete

    return 0;
}

PQ_t* Build(u32 maxlen) {
    PQ_t *pq = NULL;
    if ((maxlen > 0) && (maxlen <= PQLIMIT)) {
        pq = (PQ_t*)malloc(sizeof(PQ_t));
        if (pq) {
            pq->maxSize = // complete
            pq->elementNum = // complete
            if(pq->L = malloc(sizeof(DLLS_t))) {
                pq->L->sentinel = // complete
                if (pq->L->sentinel) {
                    pq->L->sentinel->next =
                    pq->L->sentinel->prev = 
                    pq->L->sentinel->element.key = 0;  // insignificant
                    pq->L->sentinel->element.prio = 0; // insignificant
                }
                else {
                    printf("\nBuild()>> Could not allocate memory for the lists' sentinel - Bad pointer %x\n", BADPTR);
                    return NULL;
                }
                // using sentinel, elminate the needs for the head (front) and tail (rear) pointers in a list structure
                // pq->L->head = NULL;
                // pq->L->tail = NULL;
            }
            else {
                printf("\nBuild()>> Could not allocate memory for the queue's list - Bad pointer %x\n", BADPTR);
                return NULL;
            }
        }
        else {
            printf("\nBuild()>> Could not allocate memory for the priority queue - Bad pointer %x\n", BADPTR);
            return NULL;
        }
    }
    // we could add an else - clause to print an error message regarding invalid maxlen
    return; // complete
}

void Enqueue(PQ_t* pq, QElement_t e) {
    if () { // complete
        // insert e into the list

        pq->elementNum = pq->elementNum + 1;
    }
    else {
        printf("\nEnqueue()>> Attempt to overflox the queue at %p was prevented.\n", pq);
    }

    // change to return a Qelement

    char Dequeue(PQ_t* pq) {
        char val = -1;
        // complete the code here to do all the following testing
        // checking pq must precede checking pq->L
        // also pq->L->sentinel must be checked before accessing it
        // always check a pointer before dereferencing it

        // complete the code below to prevent underflow of the queue

        if() { // complete


            printf("\nEnqueue()>> Attempt to overflox the queue at %p was prevented.\n");
            return (char) UNDERFLOW;
        }
        LElement_t *ptr = ListDeleteLast(pq->L);
        if (ptr) {
            // complete
        }
    }
    return (char) val;
}

char DequeueMax(PQ_t* pq) {
    // complete the declaration of dequeueMAX
    // it should dequeue the element with the maximal priority if several elements
    // with the same max priority exist, the element with the longest waiting time should be dequeued
    // aftert dequeuing, the element should be deleted from the list and corressponding memory deallocated

    return 0;
}
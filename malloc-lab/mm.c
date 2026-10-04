/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/* Basic constants and macros */

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address up */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val)) // 값을 주소에 쓴다.

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define GET_NOW_HEADER(bp) ((char *)(bp) - WSIZE)
#define GET_NOW_FOOTER(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLOCK_HEADER(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLOCK_HEADER(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))


static char *heap_list_p; // p 연산할때 바이트 단위로 연산해야해서 char (1byte)니까 
// char 는 모든 기계에서 1byte 임 
// static : 다른 코드에서 참조 하지 말라. 참조할 수 잇는 범위가 파일 안으로 제한됨.

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    // heap 리스트 최초로 받아오기 memlib.c 의 sbrk 함수 활용
    // pb (8) + eb (4)
    heap_list_p = mem_sbrk(2*DSIZE);

    // heap list 유효하지 않은 값일 때 처리 (반환값 검사/ 에러 체크)
    if (heap_list_p == (void *)-1) return -1;

    // padding 4B (16 - 8 - 4)
    // 근데 얼마나 padding 나올지 사실 모르니 wsize 를 계산하는 방식으로 구해야하는거 아닌가?
    PUT(heap_list_p, 0); // 근데 이게 4B 가 들어가는게 맞나? 이렇게 적어도? 

    // heap 의 prologue block (header + footer) 추가 (size 8 (h + f), alloc 1)
    // header
    PUT((heap_list_p + WSIZE), PACK(DSIZE, 1));
    // footer
    PUT((heap_list_p + 2*WSIZE), PACK(DSIZE, 1));

    // heap 의 epilogue block (header) 추가 (size 0, alloc 1)
    PUT((heap_list_p + 3*WSIZE), PACK(0, 1));

    // bp 이동하기
    heap_list_p += 2*WSIZE;

    // TODO: malloc 에서 힙 필요 시 늘리기. -> extend_heap 을 호출하고, 없으면 init 하기 
    // mdriver 의 호출 패턴에 맞춰 설계하는 것 금지.

    return 0;
}

/* extend heap */
static void *extend_heap(size_t words){
    // 아무 heap 도 없으면 init 하기 (유효성 체크)
    

    // 기존 eb 지워야 하는지?


/*
// extend heap 구현 (sbrk 함수 호출?)
    // 1. sbrk 할 사이즈
        - 타입 변환 
            sbrk 함수 확인 (int incr)로 파라미터 값 가져옴. 
            int 로 받는데, unsigned int 로 반환해서 줘야하나? 
                size_t 와 unsigned int 의 차이 : unsigned int 는 최소 16 비트라는 정해진 값, size_t 그 시스템에서 가장 큰 객체의 크기를 담을 수 있는 부호 없는 정수 타입

            => 책에서는 이거 없이 넘김
            
        - 홀수일 때 처리
            홀수면 + 1
            아니면 그대로 출력
        - 8의 배수가 아닐 때 align 

        * 워드 -> 바이트 : * WSIZE 하기. 
*/
    size_t size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
        

    // 2. pointer 돌려주는 위치 정하기
    heap_list_p = mem_sbrk(size); // old brk 반환하면 payload 자리는 hlp + words 크기


    // eb 추가 




}


/* find fit */
static void *find_fit(size_t asize){

}

static void place(){

}




/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
        return NULL;
    else
    {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
        copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
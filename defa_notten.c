#include "input.h"

//About 1h work (solving problem/implementing)
//+15 mins refactoring, documenting, cleaning up.

/*
====================
Function prototypes.
====================
*/

//Finds the max product of 4 consecutive elements in an array
uint32_t max_product(uint32_t currProd, uint8_t nextElem, const uint8_t* input);
static inline uint8_t findZeroElement(const uint8_t* currentStart);


int main()
{
    uint32_t max_prod;
    uint32_t tmpMax;

    //Pointer to start point of current row.
    static uint8_t *row;
    row = (uint8_t*)inputMatrix;

      //Todo: walk by cols and diag.
      //Main loop through the matrix
      for(int i = 0; i < N; ++i)
      {
        tmpMax = max_product(0, 4, row + i*sizeof(row[N]));
        if(max_prod < tmpMax)
        {
          max_prod = tmpMax;
        }
        printf("Row :%d     current max: %d \n",i, max_prod);
      }

    printf("Max product: %d\n",max_prod);
    return 0;
}



uint32_t max_product(uint32_t currProd, uint8_t nextElem, const uint8_t* input)
{
    /*Need two separate variables as maxProduct should be retained
      throughout the current input, but should be reset between different
      rows/colums/diagonals.
    */
    static uint32_t maxProduct;
    uint32_t returnMax;

    const uint8_t* currentStart = (input + nextElem - 4);
    uint8_t oldestFactor =  *currentStart;


    if(nextElem > N-4)
    {
       // printf("Reached end success! %d \n", nextElem);
        returnMax = maxProduct;
        maxProduct=0;
        return returnMax;
    }

    /*
    *  Key part of the program.
    *
    * Special case if previous product were 0 (either by having a multiple of 0
    * or when calling for the first time). This is the naive, brute force way.
    *
    * Otherwise if we have consecutive numbers a,b,c,d,e we just need to multiply with the
    * next consecutive number and divide by the oldest ((abcd/a)*e = bcde) to get the next
    * product in the chain.
    * since we know for sure that a is a factor in the product, and we are working with
    * integers there won't be any rounding errors or truncations of decimals.
    */
    if(!currProd)
    {
      currProd = (*(currentStart)) * (*(currentStart + 1)) * (*(currentStart + 2)) * (*(currentStart + 3));
    }
    else
    {
      currProd = (currProd/oldestFactor)* (*(input + nextElem));
    }


    if(currProd > maxProduct)
    {
      maxProduct = currProd;
    }

    /* Sadly we need this second check of product = 0, so we don't skip too far in cases like
    * a,b,c,0,e,f,g,0,i,j. Move 4 ahead of the 0, as the 0 will kill all products until then
    * anyway.
    * Otherwise move the 4-digit "window" one index forward along the array.
    */
    if(currProd == 0)
    {
       uint8_t offset_from_zero = findZeroElement(currentStart);
       max_product(currProd, nextElem + offset_from_zero + 1, input);
    }
    else
    {
      max_product(currProd, ++nextElem, input);
    }
}

/*
* Small helper function that returns the position of the (last if multiple)
* 0 that caused the product to turn 0.
*/
static inline uint8_t findZeroElement(const uint8_t* currentStart)
{
  for(int i = 3; i > -1; --i)
  {
    if( *(uint8_t*) (currentStart + i) == 0)
    {
      return i;
    }
  }
  return 0;
}




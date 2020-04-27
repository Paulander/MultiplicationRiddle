#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "input.h"

/*
====================
Function prototypes.
====================
*/

//Finds the max product of 4 consecutive elements in an array
uint32_t max_product(uint32_t currProd, uint8_t nextElem, const uint8_t* input, int16_t stepMultiplier);

//Helper function to hind the (last) zero in 4 consecutive bytes.
static inline uint8_t find_zero_element(const uint8_t* currentStart, int16_t stepMultiplier);


/*
*  Main program. Loop over rows and columns (could be optimized!)
*  Run once for each main diagonal.
*/
int main()
{
    uint32_t maxProd;
    uint32_t tmpMax;

    //Pointer to start point of current row.
    static uint8_t *row;
    row = (uint8_t*)inputMatrix;


    //Loop through the matrix row by row
    for(int i = 0; i < N; ++i)
    {
      tmpMax = max_product(0, 4, row + i*ROW_STEP, COL_STEP);
      if(maxProd < tmpMax)
      {
        maxProd = tmpMax;
      }
      printf("Row %d:  max: %u  Global Max: %u \n",i + 1, tmpMax, maxProd);
    }

    printf("\n \n ======== \n \n");


    //Loop through the matrix column by column
   for(int i = 0; i < N; ++i)
    {
      tmpMax = max_product(0, 4, row + i*COL_STEP, ROW_STEP);
      if(maxProd < tmpMax)
      {
        maxProd = tmpMax;
      }
      printf("\n \n Col %d:  max: %u   Global Max: %u \n \n",i, tmpMax, maxProd);
    }
    printf("\n \n ======== \n \n");


    //Main diagonal
    tmpMax = max_product(0, 4, row, DIAG_STEP_DOWN);
    if(maxProd < tmpMax)
    {
      maxProd = tmpMax;
    }
    printf("Main Diagonal. max: %d   Global Max: %u \n", tmpMax, maxProd);


    //2nd main diagonal (bottom left -> top right)
    tmpMax = max_product(0, 4, row + (N-1)*ROW_STEP, DIAG_STEP_UP);
    if(maxProd < tmpMax)
    {
      maxProd = tmpMax;
    }
    printf("2nd Main Diagonal. max: %d   Global Max: %u \n", tmpMax, maxProd);

    printf("\n \n ======== \n \n");
    printf("Max product: %u\n",maxProd);

    return 0;
}



uint32_t max_product(uint32_t currProd, uint8_t nextElem, const uint8_t* input, int16_t stepMultiplier)
{
    /*Need two separate variables as maxProduct should be retained
      throughout the current input, but should be reset between different
      rows/colums/diagonals.
    */
    static uint32_t maxProduct;
    uint32_t returnMax;
    const uint8_t* currentStart = (input + (nextElem - 4)*stepMultiplier );
    uint8_t oldestFactor =  *(currentStart - stepMultiplier);

    // Prints the current row/col/diagonal throughout this recursive chain.
     printf("%d, ", oldestFactor);

    //We reached the end of the row/col/diagonal, reset row max, return the result for this one.
    if(nextElem > N-1)
    {
        returnMax = maxProduct;
        maxProduct = 0;
        return returnMax;
    }

    /*
    *  Key part of the program.
    *  Get the current product, and save it if larger than the previously largest.
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

    if(!currProd || !oldestFactor)
    {
      currProd = (*(uint8_t*)(currentStart)) *
                 (*(uint8_t*)(currentStart + 1*stepMultiplier)) *
                 (*(uint8_t*)(currentStart + 2*stepMultiplier)) *
                 (*(uint8_t*)(currentStart + 3*stepMultiplier));
    }
    else
    {
      currProd = (currProd/oldestFactor)* (*(currentStart + 3*stepMultiplier));
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
       uint8_t offset_from_zero = find_zero_element(currentStart, stepMultiplier);
       max_product(currProd, nextElem + (offset_from_zero + 1), input, stepMultiplier);
    }
    else
    {
      max_product(currProd, nextElem + 1, input, stepMultiplier);
    }
}

/*
* Small helper function that returns the position of the (last if multiple)
* 0 that caused the product to turn 0.
*/
static inline uint8_t find_zero_element(const uint8_t* currentStart, int16_t stepMultiplier)
{
  for(int i = 3; i > -1; --i)
  {
    if( *(uint8_t*) (currentStart + i*stepMultiplier) == 0)
    {
      return i;
    }
  }
  return 0;
}




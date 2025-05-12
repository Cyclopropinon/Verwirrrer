// test3.c
// simple include testing

#include <stdio.h>

#include "h3.h"
#include <h4.h>
#include "h2.h"

/*
#include "h5.h"
*/

int main()
{
    // comment 1
    // #include "h1.h"
    int i;
    #include "h1.h"
	 #include "h1.h"
    #include	"h1.h"
	#include"h1.h"	
    char* a = "this is NOT a comment: /* ";
    int b = 3;
    char* c = "*/ b should still be defined after removing comments!";/*
    This is a multiline comment that should be removed!
    */return b;
}

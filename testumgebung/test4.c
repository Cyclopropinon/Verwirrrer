// test4.c
// simple include testing with folders

#include "header/h3.h"
#include <header/h4.h>
#include "header/h2.h"

/*
#include "header/h4.h"
*/

int main()
{
    // comment 1
    // #include "header/h1.h"
    int i;
    #include "header/h1.h"
	 #include "header/h1.h"
    #include	"header/h1.h"
	#include"header/h1.h"	
    char* a = "this is NOT a comment: /* ";
    int b = 3;
    char* c = "*/ b should still be defined after removing comments!";/*
    This is a multiline comment that should be removed!
    */return b;
}

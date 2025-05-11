// test2.c
// simple comment and space removal testing

	#define m1 blubb /*
*/ #define m2	lool

int main()
{	
    // comment 1
 	 char* a = "this is NOT	a comment: /* ";
    int b = 3;


    		     char* c =/*blubb*/"// not a comment lol";
  	  char
*		
d = "*/ b should still be defined after removing comments!";/*
    This is a multiline comment that	should be removed!
    */int/**/e=7;//
	return b;
}

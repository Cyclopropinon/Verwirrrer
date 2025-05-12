// test1.c
// simple comment removal testing

int main()
{
    // comment 1
    int x = 77; //setting x to 77
    char* a = "this is NOT a comment: /* ";
    int b = 3;
    char* c = "*/ b should still be defined after removing comments!";/*
    This is a multiline comment that should be removed!
    */return b;
}

// comment test
/* block comment test */

//          assign.minc
//  assignment tests

/*               LINE 42 BASIC ASSIGNMENT                  */
// excludes assignment to function calls, recursion, pointers, and preprocessor assignment

// assign to literal
int a = 1;

// assign to identifier
int b = a;

// assign to binary literals
int c = 1 + 2;

// assign to binary identifer & literals
int d = c + 1;

// assignment to nested binaries
int e = c + (2 * 1);

// implied nested binaries, also tests BIDMAS
int f = c + 2 + 1;

int g ()
{
    // assignment without decl
    h = 1;

    // self assignment
    i = h + 1;
}

// multiple implied nested binaries 
int j = 1 + 2 + 3 * 4;

// multiple implied nested binaries BIDMAS
int k = 1 + 2 * 3 + 4;

// digraph test
int l = 1 || 2 && 3;

// multiple assign
int m = 0, o = 1;

// deref 
int p = *m;

// ref
int q = &m;

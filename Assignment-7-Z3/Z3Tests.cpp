/**
 * verify.cpp
 * @author kisslune 
 */

#include "Z3Mgr.h"

using namespace z3;
using namespace SVF;


/* A simple example

int main() {
    int* p;
    int q;
    int* r;
    int x;

    p = malloc();
    q = 5;
    *p = q;
    x = *p;
    assert(x==5);
}
*/
void Z3Tests::test0()
{
    //  int* p;
    expr p = getZ3Expr("p");
    //  int q;
    expr q = getZ3Expr("q");
    //  int* r;
    expr r = getZ3Expr("r");
    //  int x;
    expr x = getZ3Expr("x");
    //  p = malloc();
    addToSolver(p == getMemObjAddress("malloc"));
    //  q = 5;
    addToSolver(q == 5);
    //  *p = q;
    storeValue(p, q);
    //  x = *p;
    addToSolver(x == loadValue(p));

    // print the expression values
    printExprValues();

    addToSolver(x == getZ3Expr(10));
    // print the result of satisfiability check
    std::cout << solver.check() << std::endl;
}


/*
// Simple integers

    int main() {
        int a;
        int b;
        a = 0;
        b = a + 1;
        assert(b > 0);
    }
*/
void Z3Tests::test1()
{
    /// TODO: your code starts from here

}

/*
  // One-level pointers

    int main() {
        int* p;
        int q;
        int b;
        p = malloc;
        *p = 0;
        q = *p;
        *p = 3;
        b = *p + 1;
        assert(b > 3);
    }
*/
void Z3Tests::test2()
{
    /// TODO: your code starts from here

}

/*
    // Mutiple-level pointers

    int main() {
        int** p;
        int* q;
        int* r;
        int x;

        p = malloc1(..);
        q = malloc2(..);
        *p = q;
        *q = 10;
        r = *p;
        x = *r;
        assert(x==10);
    }
*/
void Z3Tests::test3()
{
    /// TODO: your code starts from here

}

/*
   // Array and pointers

    int main() {
        int* p;
        int* x;
        int* y;
        int a;
        int b;
        p = malloc;
        x = &p[0];
        y = &p[1]
        *x = 10;
        *y = 11;
        a = *x;
        b = *y;
        assert((a + b)>20);
    }
*/
void Z3Tests::test4()
{
    /// TODO: your code starts from here

}

/*
    // Branches

int main(int argv) {
    int a;
    int b;
    int b1;
    a = argv + 1;
    b = 5;
    if(a > 10)
        b = a;
    b1 = b;
    assert(b1 >= 5);
}
*/
void Z3Tests::test5()
{
    /// TODO: your code starts from here

}

/*
// Compare and pointers
int main() {
   int *a = malloc1;
   int *b = malloc2;
   *a = 5;
   *b = 10;
   int *p;
   if (*a < *b) {
       p = a;
   } else {
       p = b;
   }
   assert(*p == 5);
}
*/
void Z3Tests::test6()
{
    /// TODO: your code starts from here

}

/*
 int main() {
	int a = 1, b = 2, c = 3;
	int d;
  if (a > 0) {
  	d = b + c;
  }
  else {
  	d = b - c;
  }
  assert(d == 5);
 }
 */
void Z3Tests::test7()
{
    /// TODO: your code starts from here

}

/*
 int main() {
    int arr[2] = {0, 1};
    int a = 10;
    int *p;
    if (a > 5) {
        p = &arr[0];
    }
    else {
        p = &arr[1];
    }
    assert(*p == 0);
 }
 */
void Z3Tests::test8()
{
    /// TODO: your code starts from here

}

/*
    // Struct and pointers

    struct A{ int f0; int* f1;};
    int main() {
       struct A* p;
       int* x;
       int* q;
       int** r;
       int* y;
       int z;

       p = malloc1;
       x = malloc2;
       *x = 5;
       q = &(p->f0);
       *q = 10;
       r = &(p->f1);
       *r = x;
       y = *r;
       z = *q + *y;
       assert(z == 15);
    }
*/
void Z3Tests::test9()
{
    /// TODO: your code starts from here

}

/*
int foo(int z) {
    k = z;
    return k;
}
int main() {
  int x;
  int y;
  y = foo(2);
  x = foo(3);
  assert(x == 3 && y == 2);
}
*/
void Z3Tests::test10()
{
    /// TODO: your code starts from here

}


int main()
{
    Z3Tests tests;
    tests.test0();
    tests.solver.reset();
    tests.test1();
    tests.solver.reset();
    tests.test2();
    tests.solver.reset();
    tests.test3();
    tests.solver.reset();
    tests.test4();
    tests.solver.reset();
    tests.test5();
    tests.solver.reset();
    tests.test6();
    tests.solver.reset();
    tests.test7();
    tests.solver.reset();
    tests.test8();
    tests.solver.reset();
    tests.test9();
    tests.solver.reset();
    tests.test10();

    return 0;
}
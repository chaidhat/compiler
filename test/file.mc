// hello this is a test file for MinimalistiC
//#include minclib

struct FizzbuzzMachine
{
    int outs[5]; // array
};

int main ()
{
    int c = 0, endAt = 10;

    static FizzbuzzMachine fbm;
    int count = 0;
    fbm.outs = malloc(endAt); // reserve memory in heap, declaring it an array

    fizzbuzz(&c, endAt, &fbm);
    return 0;
}

int fizzbuzz (int *count, int endAt, FizzbuzzMachine *fbm)
{
    // a more efficient way would be mod(*count, 2) == 0, but this
    // demonstrates boolean logic

    if (isMultiple(*count, 2)) // is count a multiple of 2?
    {
        print("fizz");
        *(fbm.outs + *count) = *(fbm.outs + *count) + 1; // fbmouts[*count] += 1;
    }
    if (isMultiple(*count, 3)) // is count a multiple of 3?
    {
        print("buzz");
        *(&fbm.outs + *count) = *(&fbm.outs + *count) + 2; // fbm.outs[*count] += 2;
    }
    byte end = count == endAt;
    if (end != 0) // not end?
    {
        *count = *count + 1; // count++;
        fizzbuzz(&count, endAt);
    }
    if (end) // end?
    {
        return 0;
    }
}

byte isMultiple (int a, int b)
{
    if (mod(a, b) == 0)
    {
        return 1; // returns true 
    }
    if (mod(a, b) != 0)
    {
        return 0; // returns false
    }
}

byte mod (int a, int b)
{
    int count = 0, out = 0;
    while (a > count)
    {
        count = count + 1; // count++;
        out = out + 1; // out++;
        if (out == b)
        {
            out = 0;
        }
    }
    return out;
}

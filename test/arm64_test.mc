int add (int a, int b)
{
    return a + b;
}

int main ()
{
    int a = 5;
    int b = 3;
    int c = a + b;

    if (c > 7) {
        c = 1;
    }

    while (c > 0) {
        c = c - 1;
    }

    int d = add(10, 20);
    return c;
}

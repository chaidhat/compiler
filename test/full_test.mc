struct Vec2
{
    int x;
    int y;
};

int dot (int ax, int ay, int bx, int by)
{
    int result = ax * bx + ay * by;
    return result;
}

int main ()
{
    Vec2 a;
    a.x = 3;
    a.y = 4;

    Vec2 b;
    b.x = 1;
    b.y = 2;

    int d = dot(a.x, a.y, b.x, b.y);

    int count = 10;
    while (count > 0) {
        count = count - 1;
    }

    if (d == 11 && count == 0) {
        return 0;
    }

    return 1;
}

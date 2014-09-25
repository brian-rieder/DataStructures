typedef struct {
  int denominator;
  int numerator;
} RATIONAL;

RATIONAL CreateRational(int a, int b)
{
  RATIONAL ret = malloc(sizeof(RATIONAL));
  if(b == 0) {
    ret.denominator = 0;
    ret.numerator = 0;
  }
  ret.numerator = a;
  ret.denominator = b;
  return ret;
}

RATIONAL Add(RATIONAL a, RATIONAL b)
{
  RATIONAL r;
  r.numerator = (a.numerator * b.denominator)+(b.numerator * a.denominator);
  r.denominator = (a.denominator * b.denominator);
  return r;
}

RATIONAL Add2(RATIONAL a, RATIONAL b)
{
  RATIONAL r;
  int g;
  g = gcd(a.denominator, b.denominator);
  r.denominator = lcm(a.denominator, b.denominator);
  r.numerator = a.denominator * b.numerator / g;
  r.numerator += b.denominator * a.numerator / g;
}

int main(int argc, char ** argv)
{
  return 0;
}

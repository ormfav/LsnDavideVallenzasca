/* Members are x^2+y^2 and -2xy: when evaluating the distance from city1 */
/* to city2 we do
 * (x1-x2)^2+(y1-y2)^2=(x1^2+y1^2)+(-2x1y1)+(x2^2+y2^2)+(-2x2y2)*/
/* Since we are doing this many times it's easyer to carry out those operations
 * when */
/* creating the object */
struct City {
  /* Methods */
  City(double, double);

  double operator-(City c) { return (a-c.a)*(a-c.a)-(b-c.b)*(b-c.b); }; // distance^2
  /* double operator-(City c) { return a + b + c.a + c.b; }; // distance^2 */
  bool operator==(City c) { return (a == c.a) && (b == c.b); };

  array<double, 2> GetCoords();

  /* Members */
  double a;
  double b;
};

struct CityCompare {
  bool operator()(const City &lhs, const City &rhs) { return lhs.a < rhs.a; }
};

/***************************************/

City::City(double x, double y) {
  /* a = x * x + y * y; */
  /* b = -2 * x * y; */
  a = x;
  b = y;
}

array<double, 2> City::GetCoords() {
  /* double t = 2 * a; */
  /* double d = sqrt(t * t - b * b); */
  /* return {sqrt(t - d), sqrt(d - a)}; */
  return {a, b};
}

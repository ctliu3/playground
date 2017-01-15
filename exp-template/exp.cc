#include <iostream>

using namespace std;

template<typename Type>
struct Exp {
  const Type&self() const {
    return *static_cast<const Type*>(this);
  }
};

struct mul {
  static float Map(float a, float b) {
    return a * b;
  }
};

template<typename TLhs, typename TRhs>
struct BinaryAddExp : public Exp<BinaryAddExp<TLhs, TRhs>> {
  const TLhs& lhs;
  const TRhs& rhs;
  BinaryAddExp(const TLhs& lhs, const TRhs& rhs): lhs(lhs), rhs(rhs) {
  }
  float Eval(int i) const {
    return lhs.Eval(i) + rhs.Eval(i);
  }
};

template<typename OP, typename TLhs, typename TRhs>
struct BinaryMapExp : public Exp<BinaryMapExp<OP, TLhs, TRhs>> {
  const TLhs& lhs;
  const TRhs& rhs;
  BinaryMapExp(const TLhs& lhs, const TRhs& rhs): lhs(lhs), rhs(rhs) {
  }
  float Eval(int i) const {
    return OP::Map(lhs.Eval(i), rhs.Eval(i));
  }
};

struct Vec : public Exp<Vec> {
  float *dptr;
  int len;
  Vec() {
  }
  Vec(float *dptr, int len): dptr(dptr), len(len) {
  }

  template<typename Type>
  Vec& operator=(const Exp<Type>& other) {
    auto src = other.self();
    for (int i = 0; i < len; ++i) {
      dptr[i] = src.Eval(i);
    }
    return *this;
  }

  float Eval(int i) const {
    return dptr[i];
  }
};

template<typename OP, typename TLhs, typename TRhs>
BinaryMapExp<OP, TLhs, TRhs> F(const Exp<TLhs>& lhs, const Exp<TRhs>& rhs) {
  return BinaryMapExp<OP, TLhs, TRhs>(lhs.self(), rhs.self());
}

template<typename TLhs, typename TRhs>
BinaryAddExp<TLhs, TRhs> operator+(const Exp<TLhs>& lhs, const Exp<TRhs>& rhs) {
  return BinaryAddExp<TLhs, TRhs>(lhs.self(), rhs.self());
}

template<typename TLhs, typename TRhs>
BinaryMapExp<mul, TLhs, TRhs> operator*(const Exp<TLhs>& lhs, const Exp<TRhs>& rhs) {
  return F<mul>(lhs, rhs);
}

struct maximum {
  static float Map(float a, float b) {
    return a > b ? a : b;
  }
};

int main() {
  float v1[5] = {1, 4, 3, 2, 0};
  float v2[5] = {9, 0, 5, 2, 1};
  float v3[5] = {0, 1, 0, 2, 0};

  Vec a(v1, 5), b(v2, 5), c(v3, 5);
  // c = a + b + c;
  // for (int i = 0; i < 5; ++i) {
    // cout << c.Eval(i) << endl;
  // }

  c = c * F<maximum>(a, b);
  for (int i = 0; i < 5; ++i) {
    cout << c.Eval(i) << endl;
  }

  return 0;
}

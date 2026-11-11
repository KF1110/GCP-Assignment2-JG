#include <cstring>
#include <chrono>
#include <cmath>
#include <cassert>
#include "sdl3-uws/sdl3-print.hpp"    // std::print, std::println
#include <immintrin.h>
#include "stats.hpp"

struct mat
{
  // n.b. this call to new has no alignment specification
  mat(const size_t sz) : data_{new float[sz*sz]}, sz_{sz} {}  // constructor
  ~mat() { delete [] data_; }                                 // destructor

        float& operator[](const std::size_t i)       { return data_[i]; }
  const float& operator[](const std::size_t i) const { return data_[i]; }

  bool operator==(const mat &rhs) const
  {
    bool b = true;
    const float tolerance = 0.1f;

    for (int i = 0; i < sz_; i++) {
      for (int j = 0; j < sz_; j++) {
        const float diff = std::abs((*this)[i*sz_+j] - rhs[i*sz_+j]);
        b = b && (diff < tolerance);
      }
    }

    return b;
  }

  float *data_;
  const size_t sz_;
};

void matmul(mat &mres, const mat &m1, const mat &m2)
{
  for (int i = 0; i < mres.sz_; i++)
  {
    for (int j = 0; j < mres.sz_; j++)
    {
      mres[i*mres.sz_+j] = 0;
      for (int k = 0; k < mres.sz_; k++)
      {
        mres[i*mres.sz_+j] += m1[i*mres.sz_+k] * m2[k*mres.sz_+j];
      }
    }
  }
}

void matmul_simd(mat &mres, const mat &m1, const mat &m2)
{
  // to do
}

void print_mat(const mat &m)
{
  for (int i = 0; i < m.sz_; i++)
  {
    for (int j = 0; j < m.sz_; j++)
    {
      std::print("{:3} ", m[i*m.sz_+j]);
    }
    std::println();
  }
  std::println();
}

// A simple initialisation pattern. For an 8x8 matrix:

//  1   2   3   4   5   6   7   8
//  9  10  11  12  13  14  15  16
// 17  18  19  20  21  22  23  24
// 25  26  27  28  29  30  31  32
// 33  34  35  36  37  38  39  40
// 41  42  43  44  45  46  47  48
// 49  50  51  52  53  54  55  56
// 57  58  59  60  61  62  63  64

void init_mat(mat &m)
{
  int count = 1;
  for (int i = 0; i < m.sz_; i++)
  {
    for (int j = 0; j < m.sz_; j++)
    {
      m[i*m.sz_+j] = count++;
    }
  }
}

int main(int argc, char *argv[])
{
  unsigned int SZ = 1 << 3; // (1 << 10) == 1024
  mat mres{SZ}, m{SZ}, id{SZ}, mres_simd{SZ};

  std::println("Each {}x{} matrix is {} bytes.", SZ, SZ, sizeof(float)*SZ*SZ);

  init_mat(m);

  using namespace std::chrono;
  time_point<high_resolution_clock> t1, t2;

  t1 = high_resolution_clock::now();
  matmul(mres, m, m);
  t2 = high_resolution_clock::now();

  double d = duration<double, std::milli>(t2-t1).count();
  std::println("{:f} milliseconds.", d);

  t1 = high_resolution_clock::now();
  matmul_simd(mres_simd, m, m);
  t2 = high_resolution_clock::now();

  d = duration<double, std::milli>(t2-t1).count();
  std::println("{:f} milliseconds.", d);

  print_mat(m);
  print_mat(m);
  print_mat(mres);

  const bool correct = mres_simd==mres;
  //assert(correct); // uncomment when you have implemented matmul_simd

  return correct ? 0 : -1;
}

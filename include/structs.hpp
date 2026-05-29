#ifndef STRUCTS_H
#define STRUCTS_H
#include <vector>

using namespace std;

enum CellType
{
  NPC,
  IP,
  neuron
};

struct constants
{
  const int elements_per_cell{20};
  // intra: strong, tight — holds a cell together
  const double D_intra{1.0}, a_intra{2.0}, r0_intra{1.0};
  // inter: weaker — cells adhere but stay distinct
  const double D_inter{0.1}, a_inter{2.0}, r0_inter{4.5};
  const double gamma{1.0}, dt{0.001};
  const int n_steps{5000};
};

struct ParticleData
{
  vector<double> x, y, z;
  vector<double> fx, fy, fz;
  vector<int> cell_id;
  ParticleData(int N) : x(N), y(N), z(N), fx(N), fy(N), fz(N), cell_id(N) {}
};

struct CellData
{
  vector<CellType> type;
  vector<int> age;
  vector<int> div_count;
  CellData(int n_cells) : type(n_cells), age(n_cells, 0), div_count(n_cells, 0)
  {
  }
};

#endif // STRUCTS_H

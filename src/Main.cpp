#include "SEM.hpp"
#include "structs.hpp"
#include <cmath>
#include <random>
using namespace std;

int main()
{
  /*
   * ------ variables from structs ------
   */
  constants p;
  int N{40};
  int n_cells{N / p.elements_per_cell};
  ParticleData pd(N);
  CellData cd(n_cells);
  for (int c = 0; c < n_cells; ++c)
    cd.type[c] = NPC;

  mt19937 rng(42);
  int side = std::ceil(std::cbrt(p.elements_per_cell)); // ~4 for 50
  std::uniform_real_distribution<double> jit(-0.05, 0.05);
  for (int i = 0; i < N; ++i)
  {
    pd.cell_id[i] = i / p.elements_per_cell; // 0..49 -> 0, 50..99 -> 1
    int k = i % p.elements_per_cell;         // index within the cell
    int ix = k % side, iy = (k / side) % side, iz = k / (side * side);
    double cx = pd.cell_id[i] * 5.0; // cell 1 shifted +5 in x
    pd.x[i] = cx + ix * p.r0_intra + jit(rng);
    pd.y[i] = iy * p.r0_intra + jit(rng);
    pd.z[i] = iz * p.r0_intra + jit(rng);
  }

  ofstream out("data.csv");
  if (out.is_open())
  {
    out << "step,particle,cell_id,x,y,z,totalEnergy" << '\n';
  }

  /* -------------------
   * ---- Main Loop ----
   * -------------------
   */
  for (int n = 0; n < p.n_steps; ++n)
  {

    fill(pd.fx.begin(), pd.fx.end(), 0.0);
    fill(pd.fy.begin(), pd.fy.end(), 0.0);
    fill(pd.fz.begin(), pd.fz.end(), 0.0);

    force(pd, p);
    double U = totalEnergy(pd, p);

    updatePosition(pd, p);

    if (n % 10 == 0)
    {
      writeToFile(out, pd, n, p, U);
    }
  }

  out.close();
  return 0;
}

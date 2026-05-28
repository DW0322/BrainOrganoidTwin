#include <cmath>
#include <fstream>
#include <random>
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
  const int N{40};
  const int elements_per_cell{20};
  const int n_cells{N / elements_per_cell};
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

void force(ParticleData &pd, const constants &p)
{
  double r = 0.0;
  for (int i = 0; i < p.N; ++i)
  {
    for (int j = i + 1; j < p.N; ++j)
    {
      double dx = pd.x[j] - pd.x[i];
      double dy = pd.y[j] - pd.y[i];
      double dz = pd.z[j] - pd.z[i];
      r = sqrt(dx * dx + dy * dy + dz * dz);

      // Pick the potential: same cell -> intra (cohesion),
      // different cell -> inter (adhesion). This branch is what
      // makes it SEM rather than single-type MD.
      double D, a, r0;
      if (pd.cell_id[i] == pd.cell_id[j])
      {
        D = p.D_intra;
        a = p.a_intra;
        r0 = p.r0_intra;
      }
      else
      {
        D = p.D_inter;
        a = p.a_inter;
        r0 = p.r0_inter;
      }

      double e_term = exp(-a * (r - r0));
      // F_mag carries a leading minus; applied as i -= fx, j += fx.
      // Net effect: attractive for r > r0, repulsive for r < r0. Verified.
      double F_mag = -2 * D * a * (1 - e_term) * e_term;
      double inv_r = 1.0 / r;

      double fx = F_mag * dx * inv_r;
      double fy = F_mag * dy * inv_r;
      double fz = F_mag * dz * inv_r;

      pd.fx[i] -= fx;
      pd.fy[i] -= fy;
      pd.fz[i] -= fz;
      pd.fx[j] += fx;
      pd.fy[j] += fy;
      pd.fz[j] += fz;
    }
  }
}

void updatePosition(ParticleData &pd, const constants &p)
{
  for (int i = 0; i < p.N; ++i)
  {
    pd.x[i] += (pd.fx[i] / p.gamma) * p.dt;
    pd.y[i] += (pd.fy[i] / p.gamma) * p.dt;
    pd.z[i] += (pd.fz[i] / p.gamma) * p.dt;
  }
}

void writeToFile(ofstream &out, const ParticleData &pd, const int step,
                 const constants &p)
{
  for (int i = 0; i < p.N; ++i)
  {
    out << step << ',' << i << ',' << pd.cell_id[i] << ',' << pd.x[i] << ','
        << pd.y[i] << ',' << pd.z[i] << '\n';
  }
}
int main()
{
  const constants p;

  ParticleData pd(p.N);

  mt19937 rng(42);
  int side = std::ceil(std::cbrt(p.elements_per_cell)); // ~4 for 50
  std::uniform_real_distribution<double> jit(-0.05, 0.05);
  for (int i = 0; i < p.N; ++i)
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
    out << "step,particle,cell_id,x,y,z" << '\n';
  }

  // Main Loop
  for (int n = 0; n < p.n_steps; ++n)
  {

    fill(pd.fx.begin(), pd.fx.end(), 0.0);
    fill(pd.fy.begin(), pd.fy.end(), 0.0);
    fill(pd.fz.begin(), pd.fz.end(), 0.0);

    force(pd, p);

    updatePosition(pd, p);

    if (n % 10 == 0)
    {
      writeToFile(out, pd, n, p);
    }
  }

  out.close();
  return 0;
}

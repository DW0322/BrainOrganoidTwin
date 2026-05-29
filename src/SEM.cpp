#include "SEM.hpp"
#include <cmath>

void force(ParticleData &pd, const constants &p)
{
  double r = 0.0;
  for (int i = 0; i < pd.cell_id.size(); ++i)
  {
    for (int j = i + 1; j < pd.cell_id.size(); ++j)
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
  for (int i = 0; i < pd.x.size(); ++i)
  {
    pd.x[i] += (pd.fx[i] / p.gamma) * p.dt;
    pd.y[i] += (pd.fy[i] / p.gamma) * p.dt;
    pd.z[i] += (pd.fz[i] / p.gamma) * p.dt;
  }
}

void writeToFile(ofstream &out, const ParticleData &pd, const int step,
                 const constants &p)
{
  for (int i = 0; i < pd.x.size(); ++i)
  {
    out << step << ',' << i << ',' << pd.cell_id[i] << ',' << pd.x[i] << ','
        << pd.y[i] << ',' << pd.z[i] << '\n';
  }
}

#include "SEM.hpp"
#include "structs.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("intra Morse: equilibrium at r0, attract beyond, repel within")
{
  constants p;
  auto fx_for = [&](double sep)
  {
    ParticleData pd(2);
    pd.cell_id = {0, 0};
    pd.x = {0.0, sep};
    force(pd, p);
    return pd.fx[1];
  }; // x-force on particle 1
  REQUIRE(std::abs(fx_for(p.r0_intra)) < 1e-9); // at r0: ~0
  REQUIRE(fx_for(p.r0_intra + 0.5) < 0.0);      // pulled back toward 0
  REQUIRE(fx_for(p.r0_intra - 0.5) > 0.0);      // pushed away
}

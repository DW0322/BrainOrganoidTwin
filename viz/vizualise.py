import pandas as pd
import matplotlib.pyplot as plt
from scipy.spatial.distance import pdist

df = pd.read_csv("~/BrainOrganoidTwin/build/data.csv")
final = df[df["step"] == df["step"].max()]

fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.scatter(final["x"], final["y"], final["z"])
ax.set_xlabel("x"); ax.set_ylabel("y"); ax.set_zlabel("z")
plt.savefig("cluster.png")

distances_over_time = []
for step, group in df.groupby("step"):
    coords = group[["x", "y", "z"]].values
    distances_over_time.append((step, pdist(coords)))

plt.figure()
for step, dists in distances_over_time:
    plt.scatter([step]*len(dists), dists, s=2, alpha=0.3, color="C0")
plt.axhline(1.0, color="red", linestyle="--")
plt.xlabel("step"); plt.ylabel("pairwise distances")
plt.savefig("distances_over_time.png")

final = df[df["step"] == df["step"].max()]
coords = final[["x","y","z"]].values
print(pdist(coords))   # should be a small array of values near 1.0

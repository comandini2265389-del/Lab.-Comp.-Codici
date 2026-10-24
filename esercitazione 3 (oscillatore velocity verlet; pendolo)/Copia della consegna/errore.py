import numpy as np
import matplotlib.pyplot as plt

x,y=np.loadtxt("eulero.dat", unpack=True, usecols=(0,1))
z=np.loadtxt("eulero-cromer.dat", unpack=True, usecols=(1))
q=np.loadtxt("v-verlet.dat", unpack=True, usecols=(1))
plt.grid()
plt.xlabel("$\Delta t/s$")
plt.ylabel("$\epsilon_{G}$")
plt.title("Confronto Errori")
plt.plot(np.log(x),np.log(y),'mo--', label="Eulero")
plt.plot(np.log(x),np.log(z),'gd--', label="Eulero-Cromer")
plt.plot(np.log(x), np.log(q),'kx--', label="Velocity Verlet")
plt.legend()
plt.savefig("Errori.pdf")
plt.show()
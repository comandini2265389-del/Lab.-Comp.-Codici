import numpy as np
import matplotlib.pyplot as plt

x1,y1=np.loadtxt("1.1gamma100seed50.dat", unpack=True, usecols=(0,1))
x2,y2=np.loadtxt("1.1gamma100seed40.dat", unpack=True, usecols=(0,1))

fig,axes=plt.subplots(1,2,figsize=(10,5))

fig.suptitle(r"$\gamma=100$")

axes[0].grid()
axes[0].plot(x1,y1,'.',label="seed=50")
axes[0].set_xlabel("tempo [s]")
axes[0].set_ylabel("traiettoria")
axes[0].legend()

axes[1].grid()
axes[1].plot(x2,y2,'.',label="seed=40")
axes[1].set_xlabel("tempo [s]")
axes[1].set_ylabel("traiettoria")
axes[1].legend()

plt.savefig("1.1-1.png")
plt.show()

##########################################################################

x3,y3=np.loadtxt("1.1gamma001seed50.dat", unpack=True, usecols=(0,1))
x4,y4=np.loadtxt("1.1gamma001seed40.dat", unpack=True, usecols=(0,1))

fig,axes=plt.subplots(1,2,figsize=(10,5))

fig.suptitle(r"$\gamma=0.01$")

axes[0].grid()
axes[0].plot(x3,y3,'.',label="seed=50")
axes[0].set_xlabel("tempo [s]")
axes[0].set_ylabel("traiettoria")
axes[0].legend()

axes[1].grid()
axes[1].plot(x4,y4,'.',label="seed=40")
axes[1].set_xlabel("tempo [s]")
axes[1].set_ylabel("traiettoria")
axes[1].legend()

plt.savefig("1.1-2.png")
plt.show()

#######################################################################

fig,axes=plt.subplots(1,2,figsize=(10,5))

fig.suptitle("seed=50")

axes[0].grid()
axes[0].plot(x1,y1,'.',label=r"$\gamma=100$")
axes[0].set_xlabel("tempo [s]")
axes[0].set_ylabel("traiettoria")
axes[0].legend()

axes[1].grid()
axes[1].plot(x3,y3,'.',label=r"$\gamma=0.01$")
axes[1].set_xlabel("tempo [s]")
axes[1].set_ylabel("traiettoria")
axes[1].legend()

plt.savefig("1.1-3.png")
plt.show()

#######################################################################

fig,axes=plt.subplots(1,2,figsize=(10,5))

fig.suptitle("seed=40")

axes[0].grid()
axes[0].plot(x2,y2,'.',label=r"$\gamma=100$")
axes[0].set_xlabel("tempo [s]")
axes[0].set_ylabel("traiettoria")
axes[0].legend()

axes[1].grid()
axes[1].plot(x4,y4,'.',label=r"$\gamma=0.01$")
axes[1].set_xlabel("tempo [s]")
axes[1].set_ylabel("traiettoria")
axes[1].legend()

plt.savefig("1.1-4.png")
plt.show()

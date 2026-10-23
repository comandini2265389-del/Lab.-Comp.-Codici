import numpy as np
import matplotlib.pyplot as plt

x,y=np.loadtxt("eulero.dat",unpack=True,usecols=(0,1))
plt.grid()
plt.plot(x,y,'o')
plt.savefig("traiettoria-eulero.png")
plt.show()

z,w=np.loadtxt("eulero-cromer.dat", unpack=True,usecols=(0,1))
plt.grid()
plt.plot(z,w,'o')
plt.savefig("traiettoria-eulero-cromer.png")
plt.show()

s0,v0=np.loadtxt("eulero.dat",unpack=True,usecols=(1,2))
plt.grid()
plt.plot(s0,v0,'o')
plt.savefig("spazio-fasi-eulero.png")
plt.show()

s1,v1=np.loadtxt("eulero-cromer.dat",unpack=True,usecols=(1,2))
plt.grid()
plt.plot(s1,v1,'o')
plt.savefig("spazio-fasi-eulero-cromer.png")
plt.show()

E0=np.loadtxt("eulero.dat",unpack=True,usecols=(3))
E1=np.loadtxt("eulero-cromer.dat",unpack=True,usecols=(3))
plt.grid()
plt.plot(x,E0,'o')
plt.plot(x,E1,'o')
plt.savefig("confronto-energie.png")
plt.show()

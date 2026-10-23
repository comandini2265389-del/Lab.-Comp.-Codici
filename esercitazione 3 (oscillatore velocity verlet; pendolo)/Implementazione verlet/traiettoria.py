import numpy as np 
import matplotlib.pyplot as plt 

x,y,z=np.loadtxt("traiettoria.dat", unpack=True, usecols=(0,1,2))
plt.plot(x,y)
plt.savefig("traiettoriav.pdf")
plt.show()

plt.plot(x,z)
plt.savefig("velocitàv.pdf")
plt.show()


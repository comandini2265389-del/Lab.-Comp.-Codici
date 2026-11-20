import numpy as np
import matplotlib.pyplot as plt

x,y=np.loadtxt("prova.dat",unpack=True,usecols=(0,3))
plt.semilogy(x,y, '.')
plt.show()

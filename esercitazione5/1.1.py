import numpy as np
import matplotlib.pyplot as plt

x,y=np.loadtxt("1.1condizione1.dat", unpack=True, usecols=(1,2))
x1,y1=np.loadtxt("1.1condizione2.dat", unpack=True, usecols=(1,2))
x2,y2=np.loadtxt("1.1condizione3.dat", unpack=True, usecols=(1,2))
plt.grid()
plt.title("Spazio delle fasi 1.1; stride=3")
plt.xlabel("x [m]")
plt.ylabel("v [m/s]")
plt.plot(x,y, 'og', label="$x_0=0.1; v_0=0; \mu=1$")
plt.plot(x1,y1, '^m', label="$x_0=4; v_0=0; \mu=1$")
plt.plot(x2,y2, 'dy', label="$x_0=2; v_0=0; \mu=0$ (oscillatore armonico)")
plt.legend()
plt.savefig("1.1.pdf")
plt.show()

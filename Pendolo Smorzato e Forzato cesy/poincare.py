import numpy as np
import matplotlib.pyplot as plt

fig, axes = plt.subplots(2,2, figsize=(15, 8))

t1=np.loadtxt("p09.dat",unpack=True,usecols=(0))
t2=np.loadtxt("p107.dat",unpack=True,usecols=(0))
t3=np.loadtxt("p115.dat",unpack=True,usecols=(0))
t4=np.loadtxt("p135.dat",unpack=True,usecols=(0))

Nt1=round(len(t1)/2)
Nt2=round(len(t2)/2)
Nt3=round(len(t3)/2)
Nt4=round(len(t4)/2)

omega1,theta_r1=np.loadtxt("p09.dat",unpack=True,usecols=(2,4),skiprows=Nt1)
omega2,theta_r2=np.loadtxt("p107.dat",unpack=True,usecols=(2,4),skiprows=Nt2)
omega3,theta_r3=np.loadtxt("p115.dat",unpack=True,usecols=(2,4),skiprows=Nt3)
omega4,theta_r4=np.loadtxt("p135.dat",unpack=True,usecols=(2,4),skiprows=Nt4)


axes[0,0].grid()
axes[0,0].set_xlabel(r"$\Theta_r$")
axes[0,0].set_ylabel(r"$\omega$")
axes[0,0].plot(theta_r1,omega1,'.',label="f=0.9")
axes[0,0].legend()

axes[0,1].grid()
axes[0,1].set_xlabel(r"$\Theta_r$")
axes[0,1].set_ylabel(r"$\omega$")
axes[0,1].plot(theta_r2,omega2,'.',label="f=1.07")
axes[0,1].legend()

axes[1,0].grid()
axes[1,0].set_xlabel(r"$\Theta_r$")
axes[1,0].set_ylabel(r"$\omega$")
axes[1,0].plot(theta_r3,omega3,'.',label="f=1.15")
axes[1,0].legend()

axes[1,1].grid()
axes[1,1].set_xlabel(r"$\Theta_r$")
axes[1,1].set_ylabel(r"$\omega$")
axes[1,1].plot(theta_r4,omega4,'.',label="f=1.35")
axes[1,1].legend()

plt.savefig("poincare.png")
plt.show()

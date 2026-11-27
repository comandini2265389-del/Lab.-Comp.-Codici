import numpy as np
import matplotlib.pyplot as plt

t1,theta1,omega1,e1,theta_r1=np.loadtxt("09.dat",unpack=True)
Td=t1/(3*np.pi)

fig, axes=plt.subplots(1,2,figsize=(10,5))
axes[0].grid()
axes[0].set_xlabel(r"$T_{D}$")
axes[0].set_ylabel(r"$\Theta$")
axes[0].set_title("traiettoria, f=0.9")
axes[0].plot(Td,theta1,'.')

axes[1].grid()
axes[1].set_xlabel(r"$\Theta_r$")
axes[1].set_ylabel(r"$\omega$")
axes[1].set_title("spazio delle fasi, f=0.9")
axes[1].plot(theta_r1,omega1,'.')

plt.savefig("09.png")
plt.show()

t2,theta2,omega2,e2,theta_r2=np.loadtxt("107.dat",unpack=True)
Td=t2/(3*np.pi)

fig, axes=plt.subplots(1,2,figsize=(10,5))
axes[0].grid()
axes[0].set_xlabel(r"$T_{D}$")
axes[0].set_ylabel(r"$\Theta$")
axes[0].set_title("traiettoria, f=1.07")
axes[0].plot(Td,theta2,'.')

axes[1].grid()
axes[1].set_xlabel(r"$\Theta_r$")
axes[1].set_ylabel(r"$\omega$")
axes[1].set_title("spazio delle fasi, f=1.07")
axes[1].plot(theta_r2,omega2,'.')

plt.savefig("107.png")
plt.show()

t3,theta3,omega3,e3,theta_r3=np.loadtxt("115.dat",unpack=True)
Td=t3/(3*np.pi)

fig, axes=plt.subplots(1,2,figsize=(10,5))
axes[0].grid()
axes[0].set_xlabel(r"$T_{D}$")
axes[0].set_ylabel(r"$\Theta$")
axes[0].set_title("traiettoria, f=1.15")
axes[0].plot(Td,theta3,'.')

axes[1].grid()
axes[1].set_xlabel(r"$\Theta_r$")
axes[1].set_ylabel(r"$\omega$")
axes[1].set_title("spazio delle fasi, f=1.15")
axes[1].plot(theta_r3,omega3,'.')

plt.savefig("115.png")
plt.show()

t4,theta4,omega4,e4,theta_r4=np.loadtxt("135.dat",unpack=True)
Td=t3/(3*np.pi)

fig, axes=plt.subplots(1,2,figsize=(10,5))
axes[0].grid()
axes[0].set_xlabel(r"$T_{D}$")
axes[0].set_ylabel(r"$\Theta$")
axes[0].set_title("traiettoria, f=1.35")
axes[0].plot(Td,theta4,'.')

axes[1].grid()
axes[1].set_xlabel(r"$\Theta_r$")
axes[1].set_ylabel(r"$\omega$")
axes[1].set_title("spazio delle fasi, f=1.35")
axes[1].plot(theta_r4,omega4,'.')

plt.savefig("135.png")
plt.show()

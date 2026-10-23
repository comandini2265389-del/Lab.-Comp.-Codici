import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

data_e  = np.loadtxt("eulero.dat", unpack=True)
data_ec = np.loadtxt("eulero-cromer.dat", unpack=True)

t_e,  s_e,  v_e,  E_e  = data_e[0],  data_e[1],  data_e[2],  data_e[3]
t_ec, s_ec, v_ec, E_ec = data_ec[0], data_ec[1], data_ec[2], data_ec[3]

N = max(len(t_e), len(t_ec))
DEC = max(1, N // 300)
t_e,  s_e,  v_e,  E_e  = t_e[::DEC],  s_e[::DEC],  v_e[::DEC],  E_e[::DEC]
t_ec, s_ec, v_ec, E_ec = t_ec[::DEC], s_ec[::DEC], v_ec[::DEC], E_ec[::DEC]

N = max(len(t_e), len(t_ec))
MAX_FRAMES = 80
step = max(1, N // MAX_FRAMES)
frames = list(range(1, N + 1, step))
if frames[-1] != N:
    frames.append(N)

INTERVAL = 30
FPS      = int(1000 / INTERVAL)
DPI      = 72
BITRATE  = 1000
FS       = (8, 3)

fig1, ax1 = plt.subplots(1, 2, figsize=FS)
ax1[0].grid(); ax1[0].set_xlabel("t/s"); ax1[0].set_ylabel("s/m")
ax1[0].set_title("Traiettoria - Eulero")
ax1[0].set_xlim(t_e.min(), t_e.max()); ax1[0].set_ylim(s_e.min(), s_e.max())
le1,  = ax1[0].plot([], [], 'o', label="Eulero"); ax1[0].legend()
ax1[1].grid(); ax1[1].set_xlabel("t/s"); ax1[1].set_ylabel("s/m")
ax1[1].set_title("Traiettoria - Eulero-Cromer")
ax1[1].set_xlim(t_ec.min(), t_ec.max()); ax1[1].set_ylim(s_ec.min()-5, s_ec.max()+5)
lec1, = ax1[1].plot([], [], 'o', label="Eulero-Cromer", color='orange'); ax1[1].legend()
fig1.tight_layout()

def u1(i):
    n = frames[i]
    le1.set_data(t_e[:n],  s_e[:n])
    lec1.set_data(t_ec[:n], s_ec[:n])
    return le1, lec1

a1 = FuncAnimation(fig1, u1, frames=len(frames), interval=INTERVAL,
                   blit=True, repeat=False, cache_frame_data=False)

fig2, ax2 = plt.subplots(1, 2, figsize=FS)
ax2[0].grid(); ax2[0].set_xlabel("s/m"); ax2[0].set_ylabel("v/ms^-1")
ax2[0].set_title("Spazio delle Fasi - Eulero")
ax2[0].set_xlim(s_e.min(), s_e.max()); ax2[0].set_ylim(v_e.min(), v_e.max())
le2,  = ax2[0].plot([], [], 'o', label="Eulero"); ax2[0].legend()
ax2[1].grid(); ax2[1].set_xlabel("s/m"); ax2[1].set_ylabel("v/ms^-1")
ax2[1].set_title("Spazio delle Fasi - Eulero-Cromer")
ax2[1].set_xlim(s_ec.min()-5, s_ec.max()+5); ax2[1].set_ylim(v_ec.min()-5, v_ec.max()+5)
lec2, = ax2[1].plot([], [], 'o', label="Eulero-Cromer", color='orange'); ax2[1].legend()
fig2.tight_layout()

def u2(i):
    n = frames[i]
    le2.set_data(s_e[:n],  v_e[:n])
    lec2.set_data(s_ec[:n], v_ec[:n])
    return le2, lec2

a2 = FuncAnimation(fig2, u2, frames=len(frames), interval=INTERVAL,
                   blit=True, repeat=False, cache_frame_data=False)

fig3, ax3 = plt.subplots(figsize=FS)
ax3.grid(); ax3.set_xlabel("t/s"); ax3.set_ylabel("E/J"); ax3.set_title("Confronto energie")
ax3.set_xlim(t_e.min(), t_e.max())
ax3.set_ylim(min(E_e.min()-20, E_ec.min()-20), max(E_e.max(), E_ec.max()))
le3,  = ax3.plot([], [], 'o', label="Energia Eulero")
lec3, = ax3.plot([], [], 'o', label="Energia Eulero-Cromer")
ax3.legend(); fig3.tight_layout()

def u3(i):
    n = frames[i]
    le3.set_data(t_e[:n],  E_e[:n])
    lec3.set_data(t_ec[:n], E_ec[:n])
    return le3, lec3

a3 = FuncAnimation(fig3, u3, frames=len(frames), interval=INTERVAL,
                   blit=True, repeat=False, cache_frame_data=False)

def save(ani, name):
    try:
        ani.save(f"{name}.mp4", writer="ffmpeg", fps=FPS, dpi=DPI,
                 bitrate=BITRATE, extra_args=["-preset", "ultrafast"])
    except Exception:
        ani.save(f"{name}.gif", writer="pillow", fps=FPS, dpi=DPI)

save(a1, "traiettorie")
save(a2, "spazio-fasi")
save(a3, "energie")

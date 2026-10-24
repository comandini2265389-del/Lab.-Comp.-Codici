#!/usr/bin/env python3
"""
Autograding per l'Esercitazione 3 (LFC1), parte 2: pendolo semplice con velocity Verlet.

Il programma viene eseguito con la sintassi
    ./pendolo dt nsteps stride theta0 omega0 length
e deve stampare quattro colonne, t theta omega E: la condizione iniziale e poi
una riga ogni stride passi.

Uso:
    python3 autograde_pendolo.py lines    # numero di righe, 4 colonne, condizione iniziale
    python3 autograde_pendolo.py args     # errore (exit code > 0) con argomenti sbagliati
    python3 autograde_pendolo.py energy   # energia conservata, errore ~ dt^2
    python3 autograde_pendolo.py period   # periodo per piccole e grandi ampiezze

Esce con codice 0 se il test passa, 1 altrimenti (messaggio su stdout).
Funziona sia da solo (es. comando di un test GitHub Classroom) sia richiamato da check50.
Nessuna dipendenza esterna, solo libreria standard.
"""
import math
import os
import subprocess
import sys

EXE = os.environ.get("PEND_EXE", "./pendolo")
TIMEOUT = 20  # secondi

G = 9.81  # valore richiesto dal testo

SIGNALS = {11: "segmentation fault", 8: "floating point exception", 6: "abort"}


class Fail(Exception):
    pass


def execute(args):
    """Esegue un comando gestendo timeout ed eseguibile mancante."""
    try:
        return subprocess.run(args, capture_output=True, text=True, timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        raise Fail(f"timeout ({TIMEOUT} s) eseguendo: {' '.join(args)}")
    except FileNotFoundError:
        raise Fail(f"eseguibile {args[0]} non trovato: il programma compila?")


def run(dt, nsteps, stride, theta0, omega0, length):
    """Esegue il programma e restituisce (righe [t, theta, omega, E], comando)."""
    args = [EXE] + [str(a) for a in (dt, nsteps, stride, theta0, omega0, length)]
    cmd = " ".join(args)
    p = execute(args)
    if p.returncode != 0:
        raise Fail(f"`{cmd}` -> exit code {p.returncode}, stderr: {p.stderr.strip()[:200]!r}")
    rows = parse(p.stdout, cmd)
    if not rows:
        raise Fail(f"`{cmd}`: nessuna riga su stdout")
    return rows, cmd


def parse(stdout, cmd):
    rows = []
    lines = stdout.splitlines()
    for i, line in enumerate(lines, 1):
        if not line.strip() and i == len(lines):
            continue
        cols = line.split()
        if len(cols) != 4:
            raise Fail(f"`{cmd}`: la riga {i} ha {len(cols)} colonne invece di 4 (t theta omega E): {line!r}")
        try:
            vals = [float(c) for c in cols]
        except ValueError:
            raise Fail(f"`{cmd}`: la riga {i} contiene valori non numerici: {line!r}")
        if not all(math.isfinite(v) for v in vals):
            raise Fail(f"`{cmd}`: la riga {i} contiene nan/inf: {line!r}")
        rows.append(vals)
    return rows


def energy(theta, omega, length, m=1.0):
    return 0.5 * m * length ** 2 * omega ** 2 + m * G * length * (1 - math.cos(theta))


def check_first_row(rows, theta0, omega0, length, cmd):
    t, th, om, E = rows[0]
    E0 = energy(theta0, omega0, length)
    if abs(t) > 1e-9 or abs(th - theta0) > 1e-6 or abs(om - omega0) > 1e-6 or abs(E - E0) > 1e-5 * max(1, E0):
        raise Fail(f"`{cmd}`: la prima riga deve essere la condizione iniziale "
                   f"'0 {theta0} {omega0} {E0:.6g}', trovato {rows[0]}. "
                   f"Controllate anche l'espressione dell'energia (m = 1, g = {G}).")


def loglog_slope(xs, ys):
    """Pendenza della retta di regressione di log(y) contro log(x)."""
    lx = [math.log(x) for x in xs]
    ly = [math.log(y) for y in ys]
    n = len(lx)
    mx, my = sum(lx) / n, sum(ly) / n
    sxx = sum((a - mx) ** 2 for a in lx)
    sxy = sum((a - mx) * (b - my) for a, b in zip(lx, ly))
    return sxy / sxx


def exact_period(theta0, length):
    """T = 4 sqrt(L/g) K(k), k = sin(theta0/2), con K(k) = pi / (2 AGM(1, sqrt(1 - k^2)))."""
    a, b = 1.0, math.cos(theta0 / 2)
    while abs(a - b) > 1e-15:
        a, b = (a + b) / 2, math.sqrt(a * b)
    return 4 * math.sqrt(length / G) * math.pi / (2 * a)


def measured_period(rows):
    """Media degli intervalli tra zeri di theta attraversati da + a - (interpolazione lineare)."""
    zeros = []
    for (t1, th1, _, _), (t2, th2, _, _) in zip(rows, rows[1:]):
        if th1 > 0 >= th2:
            zeros.append(t1 - th1 * (t2 - t1) / (th2 - th1))
    if len(zeros) < 3:
        return None
    return (zeros[-1] - zeros[0]) / (len(zeros) - 1)


# ---------------------------------------------------------------- test 1: righe
# (dt, nsteps, stride, theta0, omega0, length)
LINE_CASES = [
    (0.01, 1000, 10, 0.5, 0.0, 1.0),
    (0.001, 3000, 1000, 1.0, 0.5, 2.0),
    (0.005, 1005, 10, 0.1, 0.0, 0.5),   # nsteps non multiplo di stride
    (0.01, 50, 1, 3.0, 0.0, 1.0),
]


def test_lines():
    for case in LINE_CASES:
        dt, nsteps, stride, th0, om0, L = case
        rows, cmd = run(*case)
        expected = nsteps // stride + 1
        if len(rows) != expected:
            raise Fail(f"`{cmd}`: attese {expected} righe su stdout, trovate {len(rows)}")
        check_first_row(rows, th0, om0, L, cmd)
        for k, row in enumerate(rows):
            t_exp = k * stride * dt
            if abs(row[0] - t_exp) > 1e-6 * max(1, t_exp):
                raise Fail(f"`{cmd}`: la riga {k + 1} ha t = {row[0]}, atteso {t_exp:.6g} "
                           f"(una riga ogni stride passi)")
    return f"numero di righe e condizione iniziale corretti in {len(LINE_CASES)} casi"


# ---------------------------------------------------- test 2: argomenti sbagliati
# Il programma deve terminare con exit code > 0 (non 0 e non per un segnale, es. segfault).
ARGS_CASES = [
    (["0.001", "1000", "10"], "con 3 argomenti invece di 6"),
    ([], "senza argomenti"),
    (["0.001", "1000", "10", "0.5", "0.0", "1.0", "7"], "con 7 argomenti invece di 6"),
]


def test_args():
    for extra, situation in ARGS_CASES:
        args = [EXE] + extra
        cmd = " ".join(args)
        p = execute(args)
        if p.returncode < 0:
            sig = SIGNALS.get(-p.returncode, f"segnale {-p.returncode}")
            raise Fail(f"`{cmd}`: {situation} il programma va in crash ({sig}) invece di "
                       f"terminare con un messaggio di errore. Controllate argc prima di usare argv.")
        if p.returncode == 0:
            raise Fail(f"`{cmd}`: {situation} il programma deve terminare con un exit code "
                       f"diverso da 0 (es. return 1 o exit(1)).")
    return f"argomenti sbagliati gestiti correttamente in {len(ARGS_CASES)} casi"


# --------------------------------------------------------------- test 3: energia
# Velocity Verlet: |E - E0| / E0 limitata e O(dt^2). Per la soluzione di riferimento con
# theta0 = 1, L = 1, 10 s: max|dE/E| = 2.3e-6 (dt = 0.001) e 3.6e-5 (dt = 0.004), rapporto 16.
# Eulero-Cromer darebbe ~1e-3 e rapporto 4.
ENERGY_CASES = [
    (1.0, 0.0, 1.0),   # (theta0, omega0, length)
    (2.5, 0.0, 0.5),
]
ENERGY_DTS = (0.001, 0.004)
ENERGY_T = 10.0
ENERGY_MAX = 1e-4      # su max|dE/E| con dt = 0.001
ENERGY_ORDER = (2.0, 0.3)


def test_energy():
    results = []
    for th0, om0, L in ENERGY_CASES:
        devs = []
        for dt in ENERGY_DTS:
            nsteps = round(ENERGY_T / dt)
            rows, cmd = run(dt, nsteps, 10, th0, om0, L)
            check_first_row(rows, th0, om0, L, cmd)
            E0 = rows[0][3]
            dev = max(abs(r[3] - E0) for r in rows) / E0
            if dev == 0:
                raise Fail(f"`{cmd}`: l'energia è esattamente costante: viene davvero calcolata "
                           f"da theta e omega a ogni riga?")
            if dt == ENERGY_DTS[0] and dev > ENERGY_MAX:
                raise Fail(f"`{cmd}`: deviazione relativa massima dell'energia {dev:.3g} > {ENERGY_MAX}. "
                           f"È davvero velocity Verlet? L'accelerazione usa sin(theta) e l'energia "
                           f"è scritta correttamente?")
            devs.append(dev)
        order = loglog_slope(ENERGY_DTS, devs)
        expected, tol = ENERGY_ORDER
        if abs(order - expected) > tol:
            raise Fail(f"theta0 = {th0}, L = {L}: la deviazione dell'energia scala come dt^{order:.2f}, "
                       f"atteso dt^{expected} per velocity Verlet")
        results.append(f"dt^{order:.2f}")
    return f"energia conservata, errore che scala come {', '.join(results)}"


# --------------------------------------------------------------- test 4: periodo
# (theta0, length, tolleranza relativa su T). Con dt = 0.001 e ~10 periodi la soluzione di
# riferimento riproduce il periodo esatto entro 1e-6; la tolleranza 1e-3 lascia margine per
# varianti ragionevoli. Il caso a grande ampiezza (T/T0 = 1.33) esclude l'uso di sin(theta) ~ theta.
PERIOD_CASES = [
    (0.05, 1.0, 1e-3),
    (2.0, 1.0, 1e-3),
    (1.0, 2.5, 1e-3),
]
PERIOD_DT = 0.001
PERIOD_N = 10  # numero di periodi simulati


def test_period():
    results = []
    for th0, L, rtol in PERIOD_CASES:
        T_ex = exact_period(th0, L)
        nsteps = round(PERIOD_N * T_ex / PERIOD_DT)
        rows, cmd = run(PERIOD_DT, nsteps, 1, th0, 0.0, L)
        T = measured_period(rows)
        if T is None:
            raise Fail(f"`{cmd}`: theta(t) non oscilla attorno a zero: impossibile misurare il periodo")
        T0 = 2 * math.pi * math.sqrt(L / G)
        if abs(T - T_ex) > rtol * T_ex:
            msg = (f"`{cmd}`: periodo misurato {T:.5f} s (T/T0 = {T / T0:.4f}), "
                   f"atteso {T_ex:.5f} s (T/T0 = {T_ex / T0:.4f}).")
            if abs(T - T0) < rtol * T0:
                msg += " Il periodo coincide con quello delle piccole oscillazioni: usate sin(theta)?"
            raise Fail(msg)
        results.append(f"T/T0 = {T / T0:.4f}")
    return f"periodo corretto in {len(PERIOD_CASES)} casi ({', '.join(results)})"


TESTS = {
    "lines": test_lines,
    "args": test_args,
    "energy": test_energy,
    "period": test_period,
}

if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] not in TESTS:
        print(f"uso: {sys.argv[0]} {{{'|'.join(TESTS)}}}")
        sys.exit(2)
    try:
        print("OK:", TESTS[sys.argv[1]]())
        sys.exit(0)
    except Fail as e:
        print("FAIL:", e)
        sys.exit(1)

#!/usr/bin/env python3
"""
Autograding per l'Esercitazione 3 (LFC1), parte 1: ordine di convergenza di
Eulero-Cromer e velocity Verlet per l'oscillatore armonico.

Il programma viene eseguito con la sintassi
    ./oscillatore metodo nsteps x0 v0 omega0
e deve stampare due colonne, dt ed eps_G = |x(t_f) - x_esatto(t_f)| / A,
dove t_f = nsteps * dt_min e' lo stesso per tutti i dt.

Uso:
    python3 autograde.py format            # formato dell'output (2 colonne, almeno 4 dt)
    python3 autograde.py args              # errore (exit code > 0) con argomenti sbagliati
    python3 autograde.py euler-cromer      # pendenza log-log di eps_G(dt) ~ 1
    python3 autograde.py velocity-verlet   # pendenza log-log di eps_G(dt) ~ 2

Esce con codice 0 se il test passa, 1 altrimenti (messaggio su stdout).
Funziona sia da solo (es. comando di un test GitHub Classroom) sia richiamato da check50.
Nessuna dipendenza esterna, solo libreria standard.
"""
import math
import os
import subprocess
import sys

EXE = os.environ.get("OSC_EXE", "./oscillatore")
TIMEOUT = 20  # secondi

# Il testo permette anche etichette numeriche (1 = euler, 2 = euler-cromer,
# 3 = velocity-verlet): se la stringa viene rifiutata si riprova con l'etichetta numerica.
LABELS = {
    "euler": ["euler", "1"],
    "euler-cromer": ["euler-cromer", "2"],
    "velocity-verlet": ["velocity-verlet", "3"],
}

MIN_DT = 4  # numero minimo di valori distinti di dt nell'output


class Fail(Exception):
    pass


def run(method, nsteps, x0, v0, omega0):
    """Esegue il programma e restituisce la lista di righe [dt, eps] lette da stdout."""
    last_err = ""
    for label in LABELS[method]:
        args = [EXE, label, str(nsteps), str(x0), str(v0), str(omega0)]
        try:
            p = subprocess.run(args, capture_output=True, text=True, timeout=TIMEOUT)
        except subprocess.TimeoutExpired:
            raise Fail(f"timeout ({TIMEOUT} s) eseguendo: {' '.join(args)}")
        except FileNotFoundError:
            raise Fail(f"eseguibile {EXE} non trovato: il programma compila?")
        if p.returncode == 0 and p.stdout.strip():
            return parse(p.stdout, " ".join(args))
        last_err = f"`{' '.join(args)}` -> exit code {p.returncode}, stderr: {p.stderr.strip()[:200]!r}"
    raise Fail(f"il programma non accetta il metodo '{method}' (né l'etichetta numerica). {last_err}")


def parse(stdout, cmd):
    rows = []
    lines = stdout.splitlines()
    for i, line in enumerate(lines, 1):
        if not line.strip() and i == len(lines):
            continue
        cols = line.split()
        if len(cols) != 2:
            raise Fail(f"`{cmd}`: la riga {i} ha {len(cols)} colonne invece di 2 (dt eps_G): {line!r}")
        try:
            vals = [float(c) for c in cols]
        except ValueError:
            raise Fail(f"`{cmd}`: la riga {i} contiene valori non numerici: {line!r}")
        if not all(math.isfinite(v) for v in vals):
            raise Fail(f"`{cmd}`: la riga {i} contiene nan/inf: {line!r}")
        if vals[0] <= 0 or vals[1] <= 0:
            raise Fail(f"`{cmd}`: la riga {i} contiene dt o eps_G non positivi: {line!r}")
        rows.append(vals)
    if len({r[0] for r in rows}) < MIN_DT:
        raise Fail(f"`{cmd}`: l'output contiene {len({r[0] for r in rows})} valori distinti di dt, "
                   f"ne servono almeno {MIN_DT}")
    return rows


def loglog_slope(rows):
    """Pendenza della retta di regressione di log(eps) contro log(dt)."""
    lx = [math.log(r[0]) for r in rows]
    ly = [math.log(r[1]) for r in rows]
    n = len(lx)
    mx, my = sum(lx) / n, sum(ly) / n
    sxx = sum((a - mx) ** 2 for a in lx)
    sxy = sum((a - mx) * (b - my) for a, b in zip(lx, ly))
    return sxy / sxx


# (nsteps, x0, v0, omega0). Con i dt suggeriti (0.001 ... 0.064) nsteps multiplo di 64
# garantisce che t_f = nsteps * 0.001 sia un multiplo esatto di tutti i dt.
# Attenzione: con x0 = 0 Eulero-Cromer ha pendenza 2 (le sue posizioni coincidono con
# quelle di Verlet a meno di un errore O(dt) su v0 proporzionale ad a0 = -w^2 x0),
# quindi tutti i casi hanno x0 != 0.
SLOPE_CASES = [
    (10240, 1.0, 0.5, 2.0),   # t_f = 10.24, ~3 periodi
    (10240, 2.0, -1.0, 0.5),  # t_f = 10.24, ~0.8 periodi
]
# pendenza attesa e tolleranza; misurate per la soluzione di riferimento:
# Eulero-Cromer 1.04 e 1.00 (0.95 per la variante "prima x, poi v"), velocity Verlet 2.00
EXPECTED_SLOPE = {
    "euler-cromer": (1.0, 0.15),
    "velocity-verlet": (2.0, 0.15),
}


# ---------------------------------------------------------------- test 1: formato
def test_format():
    for method in LABELS:
        for case in SLOPE_CASES:
            run(method, *case)
    return f"formato dell'output corretto per {len(LABELS)} metodi in {len(SLOPE_CASES)} casi"


# --------------------------------------------- test 2a/2b: ordine di convergenza
def test_slope(method):
    expected, tol = EXPECTED_SLOPE[method]
    results = []
    for nsteps, x0, v0, w in SLOPE_CASES:
        cmd = f"./oscillatore {method} {nsteps} {x0} {v0} {w}"
        p = loglog_slope(run(method, nsteps, x0, v0, w))
        if abs(p - expected) > tol:
            msg = (f"`{cmd}`: la pendenza log-log di eps_G(dt) è {p:.3f}, "
                   f"attesa {expected} +/- {tol}.")
            if p > expected + tol:
                msg += (" Il tempo finale t_f è lo stesso per tutti i dt (nsteps = t_f / dt)? "
                        f"È davvero il metodo {method}?")
            else:
                msg += (" Controllate l'ordine delle operazioni nel passo di integrazione "
                        "e con quale posizione viene calcolata l'accelerazione.")
            raise Fail(msg)
        results.append(f"{p:.2f}")
    return f"{method}: pendenze log-log {', '.join(results)} (attesa {expected})"


# ------------------------------------------------- test 3: argomenti sbagliati
# Il programma deve terminare con exit code > 0 (non 0 e non per un segnale, es. segfault).
# "runge-kutta" non è valido né come stringa né come etichetta numerica (atoi -> 0).
ARGS_CASES = [
    (["velocity-verlet", "10240", "1.0", "0.5"], "con 3 argomenti invece di 5"),
    (["velocity-verlet"], "con 1 argomento invece di 5"),
    ([], "senza argomenti"),
    (["velocity-verlet", "10240", "1.0", "0.5", "2.0", "7"], "con 6 argomenti invece di 5"),
    (["runge-kutta", "10240", "1.0", "0.5", "2.0"], "con un metodo non valido"),
]
SIGNALS = {11: "segmentation fault", 8: "floating point exception", 6: "abort"}


def test_args():
    for extra, situation in ARGS_CASES:
        args = [EXE] + extra
        cmd = " ".join(args)
        try:
            p = subprocess.run(args, capture_output=True, text=True, timeout=TIMEOUT)
        except subprocess.TimeoutExpired:
            raise Fail(f"timeout ({TIMEOUT} s) eseguendo: {cmd}")
        except FileNotFoundError:
            raise Fail(f"eseguibile {EXE} non trovato: il programma compila?")
        if p.returncode < 0:
            sig = SIGNALS.get(-p.returncode, f"segnale {-p.returncode}")
            raise Fail(f"`{cmd}`: {situation} il programma va in crash ({sig}) invece di "
                       f"terminare con un messaggio di errore. Controllate argc prima di usare argv.")
        if p.returncode == 0:
            raise Fail(f"`{cmd}`: {situation} il programma deve terminare con un exit code "
                       f"diverso da 0 (es. return 1 o exit(1)).")
    return f"argomenti sbagliati gestiti correttamente in {len(ARGS_CASES)} casi"


TESTS = {
    "format": test_format,
    "args": test_args,
    "euler-cromer": lambda: test_slope("euler-cromer"),
    "velocity-verlet": lambda: test_slope("velocity-verlet"),
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
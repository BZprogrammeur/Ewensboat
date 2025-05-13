import numpy as np
import matplotlib.pyplot as plt

global p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, As, Ar, k1s, k1r, rhoair, rhowater

p1 = 0.03 # drift coefficient           [-]
p2 = 40   # tangential friction         [kg.s⁻¹]
p3 = 6000 # angular friction            [kg.m]
p4 = 200  # sail lift                   [kg.s⁻¹]
p5 = 1500 # rudder lift                 [kg.s⁻¹]
p6 = 0.5  # distance to sail CoE        [m]
p7 = 0.5  # distance to mast            [m]
p8 = 2    # distance to rudder          [m]
p9 = 300  # mass of boat                [kg]
p10 = 400 # moment of inertia           [kg.m²]
p11 = 0.2 # rudder break coefficient    [-]

As = 2    # sail plane area             [m²]
Ar = 0.5  # rudder planer area          [m²]

k1s = 0.5 # sail related coefficient    [-]
k1r = 0.3 # rudder related coefficient  [-]

rhoair = 1.2 # air density              [kg.m⁻³]
rhowater = 1000 #water density          [kg.m⁻³]

def f(X, delta, Wptw, g):
    x, y, v, theta, v, omega = X.flatten()
    atw, psitw = Wptw.flatten()
    gs, gr = g.flatten()
    deltas, deltar = delta.flatten()
    xdot = v * np.cos(theta) + p1 * atw * np.cos(psitw)
    ydot = v * np.sin(theta) + p1 * atw * np.sin(psitw)
    thetadot = omega
    vdot = (gs * np.sin(deltas) - gr * p11 * np.sin(deltar) - p2 * v**2) / p9
    omegadot = (gs * (p6 - p7 * np.cos(deltas)) - gr * p8 * np.cos(deltar) - p3 * omega * v) / p10
    return np.array([[xdot, ydot, thetadot, vdot, omegadot]]).T

def true_to_apparent_wind(X, Wctw):
    x, y, v, theta, v, omega = X.flatten()
    actw, psictw = Wctw.flatten()
    vxcaw = actw * np.cos(psictw - theta) - v
    vycaw = actw * np.sin(psictw - theta)
    return np.array([[vxcaw, vycaw]]).T

def cartesian_to_polar(Wc):
    vxc, vyc = Wc.flatten()
    ap = np.linalg.norm(Wc)
    psi = np.atan2(vxc, vyc)
    return np.array([[ap, psi]]).T

def get_g(X, delta, Wpaw):
    x, y, v, theta, v, omega = X.flatten()
    deltas, deltar = delta.flatten()
    aaw, psiaw = Wpaw.flatten()
    gs = p4 * aaw * np.sin(deltas - psiaw)
    gr = p4 * v**2 *np.sin(deltar)
    return np.array([[gs, gr]]).T

def constant_wind(x, y):
    return np.array([[5, 5]]).T

def constant_current(x, y):
    return np.array([[-1, -1]]).T

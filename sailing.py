from roblib import *
import numpy as np

dt = 0.1
s = 6
ax = init_figure(-s, s, -s, s)

# Point de départ et d'arrivée
starting_point = np.array([0, 0])
target_point = np.array([5, 5])
r = 2   # tolérance pour le virement

# Vent apparent
ψ = 2     # direction du vent
awind = 10  # vitesse du vent

#coefficient du boat
p1 = 0 #derive
p2 = 0.5 #friction
p3 = 0.5 #angular friction
p4 = 0.5 #sail lift
p5 = 0.5 #rudder lift
p6 = 1 #distance to sail
p7 = 1 #distance to mast
p8 = 2 #distance to rudder
p9 = 100 #boat mass
p10 = 2 #boat moment d'inertie
p11 = 0.5 #rudder break coefficient

δs = 0  # angle de la voile
δr = 0  # angle du gouvernail


# Initialisation état du bateau : [x, y, θ, v, w]
x = np.array([[0], [0], [0], [0], [0]])

def wind_effect(x, y):
    v1 = awind * np.cos(ψ)
    v2 = awind * np.sin(ψ)
    vx = np.ones_like(x) * v1
    vy = np.ones_like(y) * v2
    return vx, vy

def dynamics(x, δs, δr, ψ, awind):
    x1, x2, θ, v, w = x.flatten()
    x1dot = v*np.cos(θ) + p1*awind * np.cos(ψ)
    x2dot = v*np.sin(θ) + p1*awind * np.sin(ψ)
    θdot = w
    gs = -p4*awind*np.sin(δs-ψ)
    gr = -p5*v**2*np.sin(δr)
    vdot = (gs*sin(δs)-gr*sin(δr)*p11-p2*v**2)/p9
    wdot = gs*(p6-p7*cos(δs))-gr*p8*cos(δr)-p3*w*v/p10
    return np.array([[x1dot], [x2dot], [θdot], [vdot], [wdot]])

draw_field(ax, wind_effect, -s, s, -s, s, 0.4)

for t in arange(0, 20, dt):
    clear(ax)
    
    # Position actuelle
    m = x[0:2, 0]
    
    # Vecteur vers la cible
    ba = target_point - starting_point
    ma = m - starting_point
    
    e = np.linalg.det(np.vstack((ba / np.linalg.norm(ba), ma)))
    
    q = np.sign(e) if abs(e) > r else 0
    φ = np.arctan2(ba[1], ba[0])
    
    θ_bar = φ - 2 * awind / np.pi * np.arctan(e / r)
    θ = x[2, 0]
    
    # Test du besoin de virement de bord
    ζ = np.pi / 4  # angle interdit
    if np.cos(ψ - θ_bar) + np.cos(ζ) < 0 or (abs(e) < r and np.cos(ψ - φ) + np.cos(ζ) < 0):
        θ_bar = ψ + q * ζ + np.pi
    
    # Calcul du gouvernail
    if np.cos(θ - θ_bar) >= 0:
        δr = np.clip(1.0 * np.sin(θ - θ_bar), -1, 1)
    else:
        δr = np.clip(np.sign(np.sin(θ - θ_bar)), -1, 1)
    
    # Calcul de l’angle de la voile
    δs_max = np.pi / 2 * (np.cos(ψ - θ_bar) + 1) / 2
    δs = δs_max
    
    # # Dessin du bateau et champ
    draw_field(ax, wind_effect, -s, s, -s, s, 0.4)
    draw_sailboat(x, δs, δr, ψ, awind)
    
    # Mise à jour du modèle dynamique du bateau
    # δr = 0
    # δs = -np.sign(ψ)*min(abs(δs), abs(np.pi-abs(ψ)))
    x = x + dt * dynamics(x, δs, δr, ψ, awind)  # Assumes you have a function `dynamics`
    
    pause(0.05)

pause(3)

from roblib import *
import numpy as np

dt = 0.1
s = 10
ax = init_figure(-s, s, -s, s)

import numpy as np
import matplotlib.pyplot as plt

def rot2H(theta):
    """Retourne une matrice homogène de rotation 2D"""
    return np.array([
        [np.cos(theta), -np.sin(theta), 0],
        [np.sin(theta),  np.cos(theta), 0],
        [0, 0, 1]
    ])

def tran2H(x, y):
    """Retourne une matrice homogène de translation 2D"""
    return np.array([
        [1, 0, x],
        [0, 1, y],
        [0, 0, 1]
    ])

def plot2D(points, color='black', lw=1):
    """Affiche un polygone à partir d'un tableau homogène 3xN"""
    plt.plot(points[0], points[1], color=color, linewidth=lw)

def draw_sailboat2(x, δs, δr, ψ=None, awind=None):
    mx, my, θ, v, w = x.flatten()
    
    # Définir les formes (en coordonnées locales)
    hull = np.array([
        [-0.3,  1.8,  2.1, 2.1, 1.8, -0.3, -0.3, -0.3],
        [-0.6, -0.6, -0.3, 0.3, 0.6,  0.6, -0.6, -0.6],
        [1,    1,    1,    1,   1,    1,    1,    1   ]
    ])
    
    sail = np.array([
        [0, 0, 0.3],
        [0, 1.5, 1.5],
        [1, 1, 1]
    ])
    
    rudder = np.array([
        [-0.2, 0.2],
        [0,    0],
        [1,    1]
    ])

    # Transformation globale du bateau
    R_bateau = tran2H(mx, my) @ rot2H(θ)

    # Transformation voile (à l'avant du bateau, tournée selon δs)
    R_voile = R_bateau @ tran2H(1.5, 0) @ rot2H(δs)

    # Transformation gouvernail (à l'arrière du bateau, tournée selon δr)
    R_gouvernail = R_bateau @ tran2H(-0.5, 0) @ rot2H(δr)

    # Affichage
    plot2D(R_bateau @ hull, 'black')
    plot2D(R_voile @ sail, 'red', 2)
    plot2D(R_gouvernail @ rudder, 'blue', 2)

    plt.axis('equal')


def normalize_angle(angle):
    """ Ramène un angle entre -π et π """
    return (angle + np.pi) % (2 * np.pi) - np.pi

def limiter_angle_voile(delta_s, theta):
    """Limite l’angle de la voile à ±π/2 autour de l’arrière du bateau"""
    delta_s -= pi/2
    
    if theta < delta_s < theta + np.pi/2:
        delta_s = theta + np.pi/2
    elif theta-np.pi/2 < delta_s < theta:
        delta_s = theta - np.pi/2
    return delta_s + pi/2  

def limiter_par_le_vent(theta, delta_s, psi):
    delta_s = delta_s - np.pi / 2 

    # Trouver de quel côté le vent souffle
    if psi < theta < psi + np.pi:
        #babord amure -> voile à tribord
        if psi - np.pi < delta_s < psi: #interdiction à la voile d'être à babord
            delta_s = psi
        elif psi < delta_s < -theta: #interdiction d'être à contre
            delta_s = -theta
    elif psi > theta > psi - np.pi:
        #tribord amure -> voile à babord
        if psi < delta_s < psi + np.pi:
            delta_s = psi + np.pi
        elif -theta < delta_s < psi:
            delta_s = -theta
    delta_s = delta_s + np.pi / 2
    return delta_s

def limiter_par_le_vent_gpt(theta, delta_s, psi):
    """
    Empêche la voile d'être contre le vent (face au vent) :
    - `theta` : cap du bateau (rad)
    - `delta_s` : angle voile (rad)
    - `psi` : direction du vent (rad)
    Retourne `delta_s` corrigé.
    """
    # Angle relatif de la voile par rapport au vent
    rel_voile_vent = normalize_angle(delta_s - psi)
    rel_voile_bateau = normalize_angle(delta_s - theta)

    # Si la voile est à moins de π/2 du vent, elle est "contre"
    if abs(rel_voile_vent) < np.pi / 2:
        # Il faut la pousser à π/2 sous le vent, dans la direction la plus proche
        if rel_voile_vent >= 0:
            delta_s = normalize_angle(psi + np.pi / 2)
        else:
            delta_s = normalize_angle(psi - np.pi / 2)

    return delta_s
    
# Point de départ et d'arrivée
starting_point = np.array([0, 0])
target_point = np.array([5, 5])
r = 2   # tolérance pour le virement

# Vent apparent
ψ = 3*np.pi/4     # direction du vent
awind = 40  # vitesse du vent

#coefficient du boat
p1 = 0.001 #derive
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

δs = 4*np.pi/10 # angle de la voile
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
    θ = normalize_angle(θ)
    δs = normalize_angle(δs)
    δr = normalize_angle(δr)
    ψ_rel = ψ - θ
    ψ_rel = (ψ_rel + np.pi) % (2 * np.pi) - np.pi  # ramener entre [-π, π]

    v_sail = np.array([np.cos(δs), np.sin(δs)])
    v_wind = np.array([np.cos(ψ_rel), np.sin(ψ_rel)])

    δs = limiter_par_le_vent(θ, δs, ψ)
    δs = limiter_angle_voile(δs, θ)
        
    x1dot = v*np.cos(θ) + p1*awind * np.cos(ψ)
    x2dot = v*np.sin(θ) + p1*awind * np.sin(ψ)
    θdot = w
    gs = -p4*awind*np.sin(δs-ψ)
    gr = -p5*v**2*np.sin(δr)
    vdot = (gs*sin(δs)-gr*sin(δr)*p11-p2*v**2)/p9
    wdot = - gr * p8 * cos(δr) - p3 * w * v / p10
    #wdot += gs * (p6 - p7 * cos(δs)) 
    return np.array([[x1dot], [x2dot], [θdot], [vdot], [wdot]]), δs, δr

draw_field(ax, wind_effect, -s, s, -s, s, 0.4)

for t in arange(0, 8, dt):
    clear(ax)
    draw_disk(ax, starting_point, 0.2, 'black')
    draw_disk(ax, target_point, 0.2, 'red')
    
    # # Position actuelle
    # m = x[0:2, 0]
    
    # # Vecteur vers la cible
    # ba = target_point - starting_point
    # ma = m - starting_point
    
    # e = np.linalg.det(np.vstack((ba / np.linalg.norm(ba), ma)))
    
    # q = np.sign(e) if abs(e) > r/2 else 0
    # φ = np.arctan2(ba[1], ba[0])
    
    # incidence_angle = np.pi/4
    # θ_bar = φ - 2 * incidence_angle / np.pi * np.arctan(e / r)
    # θ = x[2, 0]
    
    # # Test du besoin de virement de bord
    # ζ = np.pi / 3  # angle interdit
    # if np.cos(ψ - θ_bar) + np.cos(ζ) < 0 or (abs(e) < r and np.cos(ψ - φ) + np.cos(ζ) < 0):
    #     θ_des = ψ - q * ζ + np.pi
    # else:
    #     θ_des = θ_bar
    
    # # Calcul du gouvernail
    # if np.cos(θ - θ_des) >= 0:
    #     δr = np.clip(1.0 * np.sin(θ - θ_bar), -1, 1)
    # else:
    #     δr = np.clip(np.sign(np.sin(θ - θ_bar)), -1, 1)
    
    # # Calcul de l’angle de la voile
    # δs_max = np.pi / 2 * (np.cos(ψ - θ_des) + 1) / 2
    # δs = δs_max
    
    # # # Dessin du bateau et champ
    draw_field(ax, wind_effect, -s, s, -s, s, 0.4)
    draw_sailboat2(x, δs, δr, ψ, awind)
    
    # Mise à jour du modèle dynamique du bateau
    #δs = -np.sign(ψ)*min(abs(δs), abs(np.pi-abs(ψ)))
    dx, δs, δr = dynamics(x, δs, δr, ψ, awind)
    x = x + dt * dx  # Assumes you have a function `dynamics`
    
    pause(0.05)



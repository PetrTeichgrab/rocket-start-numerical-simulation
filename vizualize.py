import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Načtení dat z CSV souboru
file_path = 'rocket_data_for_graphs.csv'
data = pd.read_csv(file_path)

animationData = pd.read_csv('rocket_data.csv')

# Vytvoření grafu
plt.figure(figsize=(10, 6))

# Vytvoření grafu s třemi podgrafy (subplots)
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 12), sharex=True)

# Graf rychlosti
ax1.plot(data['Time'], data['Velocity'], marker='o')
ax1.set_title('Velocity vs Time')
ax1.set_ylabel('Velocity (m/s)')

# Graf výšky
ax2.plot(data['Time'], data['Altitude'], marker='o')
ax2.set_title('Altitude vs Time')
ax2.set_ylabel('Altitude (m)')

# Graf zrychlení
ax3.plot(data['Time'], data['Acceleration'], marker='o')
ax3.set_title('Acceleration vs Time')
ax3.set_xlabel('Time (s)')
ax3.set_ylabel('Acceleration (m/s^2)')

fig, ax = plt.subplots(1, 1)

def animate(i):
    ax.clear()

    for j in range(len(animationData)):        
        ax.plot(animationData["Altitude"][i], color='red', label='original', marker='^')

    # Nastavení os
    ax.set_ylim(0, animationData["Altitude"][len(animationData)-1]+1000)
    ax.plot(animationData["Altitude"][len(animationData)-1], color='green', label='original', marker='^')
    ax.set_ylabel('Výška')

ani = animation.FuncAnimation(fig, animate, frames=len(animationData), interval=150, repeat=True)

# To save the animation using Pillow as a gif
writer = animation.PillowWriter(fps=15,
                                metadata=dict(artist='Me'),
                                bitrate=1800)
ani.save('rocketStart.gif', writer=writer)

plt.tight_layout()
plt.show()

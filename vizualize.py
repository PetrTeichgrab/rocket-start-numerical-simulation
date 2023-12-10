import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Načtení dat z CSV souboru
file_path = 'rocket_data_for_graphs.csv'
data = pd.read_csv(file_path)

animationData = pd.read_csv('rocket_data.csv')

# Vytvoření grafu
plt.figure(figsize=(10, 6))

# Graf rychlosti
plt.subplot(3, 1, 1)
plt.plot(data['Time'], data['Velocity'], marker='o')
plt.title('Velocity vs Time')
plt.xlabel('Time (s)')
plt.ylabel('Velocity (m/s)')

# Graf výšky
plt.subplot(3, 1, 2)
plt.plot(data['Time'], data['Altitude'], marker='o')
plt.title('Altitude vs Time')
plt.xlabel('Time (s)')
plt.ylabel('Altitude (m)')

# Graf zrychlení
plt.subplot(3, 1, 3)
plt.plot(data['Time'], data['Acceleration'], marker='o')
plt.title('Acceleration vs Time')
plt.xlabel('Time (s)')
plt.ylabel('Acceleration (m/s^2)')

fig, ax = plt.subplots(1, 1)

def animate(i):
    ax.clear()

    for j in range(len(animationData)):        
        ax.plot(animationData["Altitude"][i], color='red', label='original', marker='^')

    # Nastavení os
    ax.set_ylim(0, animationData["Altitude"][len(animationData)-1]+30)
    ax.set_ylabel('Výška')

ani = animation.FuncAnimation(fig, animate, frames=len(animationData), interval=150, repeat=True)


# To save the animation using Pillow as a gif
writer = animation.PillowWriter(fps=15,
                                metadata=dict(artist='Me'),
                                bitrate=1800)
ani.save('rocketStart.gif', writer=writer)

plt.tight_layout()
plt.show()

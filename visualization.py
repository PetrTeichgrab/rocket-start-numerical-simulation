import matplotlib.pyplot as plt

def plot_data(file_path):
    # Read data from the file
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Extract time, velocity, and altitude from the lines
    time = []
    velocity = []
    altitude = []
    for line in lines:
        data = line.split()
        time.append(float(data[0]))
        velocity.append(float(data[1]))
        altitude.append(float(data[2]))

    # Create plots
    plt.figure(figsize=(10, 5))

    # Plot 1: Time vs Velocity
    plt.subplot(1, 2, 1)
    plt.plot(time, velocity, marker='o', linestyle='-', color='b')
    plt.title('Time vs Velocity')
    plt.xlabel('Time')
    plt.ylabel('Velocity')

    # Plot 2: Time vs Altitude
    plt.subplot(1, 2, 2)
    plt.plot(time, altitude, marker='o', linestyle='-', color='r')
    plt.title('Time vs Altitude')
    plt.xlabel('Time')
    plt.ylabel('Altitude')

    # Adjust layout for better presentation
    plt.tight_layout()

    # Show the plots
    plt.show()

# Replace 'your_data_file.txt' with the actual path to your data file
data_file_path = 'sim.dat'
plot_data(data_file_path)
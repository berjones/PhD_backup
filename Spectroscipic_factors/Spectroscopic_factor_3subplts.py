import matplotlib.pyplot as plt
import numpy as np
import numpy.ma as ma
import scienceplots

plt.style.use('science')

# Energy and spectroscopic factors for each subplot
energy = np.array([0, 1016.926, 2315.1, 3220.69, 3431.6, 3836, 3987])*1e-3
intensity_subplots = np.array([
    [0.20569, 0.955224, 0.0555691, 0.209736, 0.309072, 0.08, 0.35],# ISS 2021
    [0.39, 1.18, 0.08, 0.29, 0.45, 0, 0],
    [0.216666, 0.7, 0.05, 0.3, 0.3, 0.105, 0.275],  # 1970 howard
    [0.23, 0.385, 0.07, 0, 0.375, 0.085, 0.3]  # lutz 1967 
      # Subplot 3 (example different values)
])


mask1 = ma.where(intensity_subplots[0]>=intensity_subplots[1])
mask2 = ma.where(intensity_subplots[1]>=intensity_subplots[0])

# Set up figure and axes with shared x-axis
fig, axs = plt.subplots(3, 1, figsize=(8, 9), sharex=True)

# Plotting the bar charts on each subplot individually
bars_1 = axs[0].bar(energy[mask1], intensity_subplots[0][mask1], width=50e-3, color="black", label='ISS, 2021')
bars_2 = axs[0].bar(energy, intensity_subplots[1], width=50e-3, color="red", label='ISS, 2022')
bars_1again = axs[0].bar(energy[mask2], intensity_subplots[0][mask2], width=50e-3, color="black")
bars_3 = axs[1].bar(energy, intensity_subplots[2], width=50e-3, color="black", label='Howard, 1970')
bars_4 = axs[2].bar(energy, intensity_subplots[3], width=50e-3, color="black", label='Lutz, 1967')

# Customize the axes
axs[2].set_xlabel('Energy (MeV)')
axs[1].set_ylabel('Spectroscopic factor')

# Add legends individually
axs[0].legend()
axs[1].legend()
axs[2].legend()

# Optional: Adjust layout for better spacing
plt.tight_layout()

# Save the figure
plt.savefig('Spectroscopic_factor_3subplots.png')

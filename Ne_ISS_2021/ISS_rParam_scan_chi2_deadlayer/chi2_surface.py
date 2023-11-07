import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import matplotlib.cm as cm

import scienceplots

plt.style.use('science')

# Read the data from the file
data = pd.read_csv('Ex_vsZ_fit_out.txt', delim_whitespace=True, header=0,
                   names=["param_a", "param_b", "Ex", "m", "m_err", "c", "c_err", "chi2"])

# Extract the numeric value of the first parameter (param_a -> Ex)
data['param_a_val'] = data['param_a'].str.split('_').str.get(-1).astype(float)

# Extract the numeric value of the second parameter (param_b -> m)
data['param_b_val'] = data['param_b'].str.split('_').str.get(-1).astype(float)

parameter_name_a = data['param_a'].str.split('_').str.get(0).unique()[0]
parameter_name_b = data['param_b'].str.split('_').str.get(0).unique()[0]


# Grouping by Ex and plotting separate heatmaps for each excitation energy
unique_energies = data['Ex'].unique()

for energy in unique_energies:
    data_subset = data[data['Ex'] == energy]

    # Extract x, y, and z values for the subset
    x_list = data_subset['param_a_val']
    y_list = data_subset['param_b_val']
    z_list = data_subset['chi2']

    N_x = np.unique(x_list)
    N_y = np.unique(y_list)
    X, Y = np.meshgrid(N_x, N_y)

    length_x = np.size(N_x)
    length_y = np.size(N_y)

    Z = np.full((length_x, length_y), 0)

    # The f function will find the Z values corresponding to a given x and y value
    def f(x, y):
        for i in range(len(x_list)):
            if (x_list.iloc[i] == x) and (y_list.iloc[i] == y):
                return z_list.iloc[i]
        # Return a default value if no match is found
        return np.nanmean(z_list)  # You may choose a different default value based on your data

    # Loop to populate the Z matrix
    for i in range(length_x):
        for j in range(length_y):
            Z[i, j] = f(N_x[i], N_y[j])

    # Plotting the contour plot
    plt.figure(figsize=(8, 6))
    plt.pcolormesh(X, Y, np.transpose(Z), cmap = "viridis", norm="symlog")
    cbar = plt.colorbar()
    cbar.set_label(f'$\chi^2$')

    plt.xlabel(parameter_name_a)  # Update X title
    plt.ylabel(parameter_name_b)  # Update Y title

    plt.title(f'$\chi^2$ surface for $E_x = {energy}$')

    plt.savefig(f"{parameter_name_a}_{parameter_name_b}_chi2surface_Ex_{energy}.png", bbox_inches='tight', dpi=400)

    # Plotting the contourf plot
    plt.figure(figsize=(8, 6))
    plt.contourf(X, Y, np.transpose(Z), 20, cmap='viridis', alpha=1.0, norm="symlog")
    cbar = plt.colorbar()
    cbar.set_label(f'$\chi^2$ (log scale)')

    plt.xlabel(parameter_name_a)
    plt.ylabel(parameter_name_b)
    plt.title(f'$\chi^2$ surface for $E_x = {energy}$')

    plt.savefig(f"{parameter_name_a}_{parameter_name_b}_chi2surface_Ex_{energy}_contourf.png", bbox_inches='tight', dpi=400)

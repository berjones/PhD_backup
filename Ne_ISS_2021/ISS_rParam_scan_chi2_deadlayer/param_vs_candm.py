import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import scienceplots

plt.style.use('science')

# Read the data into a Pandas DataFrame
data = pd.read_csv('Ex_vsZ_fit_out_TargetThick.txt', delim_whitespace=True, header=1, names=["param_a", "Ex", "m", "m_err", "c", "c_err"])

# Extract the numeric "ArrayDistance" values
data['param'] = data['param_a'].str.split('_').str.get(-1).astype(float)
parameter_name = data['param_a'].str.split('_').str.get(0).unique()[0]

# Sort the DataFrame by the 'param' column
data.sort_values(by='param', inplace=True)

# Group the data by 'Ex'
grouped_data = data.groupby('Ex')

# Create a single figure with two subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))

# Define a color map for Ex values
color_map = {0: 'black', 1016.93: 'red', 2315.1: 'yellow'}

# Plot each 'Ex' value on the respective subplot
for Ex, group in grouped_data:
    color = color_map[Ex]
    label = f'Ex = {Ex}'

    # subplot for 'm'
    ax1.errorbar(group['param'], group['m'], yerr=group['m_err'], label=label, color=color, marker='*', linestyle='-')
    ax1.set_xlabel(parameter_name)
    ax1.set_ylabel('gradient')

    # subplot for 'c'
    ax2.errorbar(group['param'], group['c'], yerr=group['c_err'], label=label, color=color,  marker='*', linestyle='-')
    ax2.set_xlabel(parameter_name)
    ax2.set_ylabel('intercept')

# Add a legend to the first subplot
ax1.legend()
ax2.legend()
plt.tight_layout()

savename = parameter_name +"_scan.png"
plt.savefig(savename, bbox_inches='tight', dpi=300)
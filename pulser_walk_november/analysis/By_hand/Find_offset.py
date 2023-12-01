import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Define the linear function to fit
def linear_func(x, a, b):
    return a * x + b

# Generate some example data
np.random.seed(0)
x_data = np.array([355.741, 386.70, 416.547, 445.82, 476.685, 506.83])
y_data = np.array([2/128, 3/128, 4/128, 5/128, 6/128, 7/128])

# Fit the data to the linear function
params, covariance = curve_fit(linear_func, x_data, y_data)
fit_a, fit_b = params

extended_x_range = np.linspace(100, 600, 100)

# Plot the data and the fitted line
plt.scatter(x_data, y_data, label='Pulser walk data')
plt.plot(extended_x_range, linear_func(extended_x_range, fit_a, fit_b), color='red', label='Fit: {:.4f}x + {:.3f}'.format(fit_a, fit_b))
plt.xlabel('ADC voltage (arb.)')
plt.ylabel('DAC voltage (arb.)')
plt.title('Linear Fit Example')
plt.legend()

# Show the plot
#plt.show()
plt.savefig("Thresh_extrapolate.png")

# Calculate the differences between consecutive values
differences = np.diff(x_data)

# Print the differences
for i, diff in enumerate(differences, start=1):
    print(f'Difference {i}: {diff:.3f}')
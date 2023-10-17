import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import scienceplots

plt.style.use('science')

# Define the linear function to fit
def linear_func(x, a, b):
    return a * x + b


######################################################################################################################################
# array distance = -135 mm 
######################################################################################################################################
z_pos1 = np.array([-536.5, -489.185, -449.0375, -409.101, -363.424, -323.2125, -283.5385, -238.201, -198.1375, -158.27 ])
GS_peak1 = np.array([-2.8, 20, 36, 47, 2, 3, 8, -15, 0, 29])
GS_peak_err1 = np.array([3.5, 4, 4, 4, 5, 5, 6, 8, 11, 19])

# Curve fit
popt1, pcov1 = curve_fit(linear_func, z_pos1, GS_peak1, sigma=GS_peak_err1)

# Extract the optimal parameters and their uncertainties
a_opt1, b_opt1 = popt1
a_err1, b_err1 = np.sqrt(np.diag(pcov1))

# Calculate the fitted line
x_fit1 = np.linspace(-600, 0, 1000)
y_fit1 = linear_func(x_fit1, a_opt1, b_opt1)

# Calculate the chi-square value
residuals1 = GS_peak1 - linear_func(z_pos1, a_opt1, b_opt1)
chi_square1 = np.sum(residuals1**2 / GS_peak_err1**2)

######################################################################################################################################
# array distance = -114.6 mm (nominal distance from elog)
######################################################################################################################################

z_pos2 = np.array([516.433, 468.602, 428.1, 388.074, 343.102, 302.6, 262.574, 217.602, 177.1, 137.074])*(-1)
GS_peak2 = np.array([209, 183, 186, 189, 135, 128, 126, 96, 99, 96])
GS_peak_err2 = np.array([4, 4, 4, 5, 5, 5, 6, 8, 11, 19])

# Curve fit
popt2, pcov2 = curve_fit(linear_func, z_pos2, GS_peak2, sigma=GS_peak_err2)

# Extract the optimal parameters and their uncertainties
a_opt2, b_opt2 = popt2
a_err2, b_err2 = np.sqrt(np.diag(pcov2))

# Calculate the fitted line
x_fit2 = np.linspace(-600, 0, 1000)
y_fit2 = linear_func(x_fit2, a_opt2, b_opt2)

# Calculate the chi-square value
residuals2 = GS_peak2 - linear_func(z_pos2, a_opt2, b_opt2)
chi_square2 = np.sum(residuals2**2 / GS_peak_err2**2)

######################################################################################################################################
# Plot the data 
######################################################################################################################################
plt.figure(figsize=(8,6))

plt.errorbar(z_pos1, GS_peak1, yerr=GS_peak_err1, fmt='x', markersize=4,capsize=1, color='k', alpha=1, ecolor='black', label = r'array-target d = -135 mm')
legend_label1 = f'array-target d = -135 mm: z(E)=mx+c\nm: {a_opt1:.3f} $\pm$ {a_err1:.3f}\nc: {b_opt1:.3f} $\pm$ {b_err1:.3f}\n$\chi^2$: {chi_square1:.3f}'
plt.plot(x_fit1, y_fit1, color='r', label=legend_label1)

plt.errorbar(z_pos2, GS_peak2, yerr=GS_peak_err2, fmt='v', markersize=4,capsize=1, color='k', alpha=1, ecolor='black', label = r'array-target ditance = -114.6 mm (nominal)')
legend_label2 = f'array-target d = -114.6 mm: z(E)=mx+c\nm: {a_opt2:.3f} $\pm$ {a_err2:.3f}\nc: {b_opt2:.3f} $\pm$ {b_err2:.3f}\n$\chi^2$: {chi_square2:.3f}'
plt.plot(x_fit2, y_fit2, color='y', label=legend_label2)

plt.xlim(-580, -100)
plt.ylim(-35, 320)

plt.xlabel('z position (mm)')
plt.ylabel('Centroid Position (keV)')
plt.title('0 keV centroid position vs z position')
plt.legend()

plt.savefig("Gs_peak_vs_zpos_arraydistcompare.png")



# print out the results from the fit
print("Fitting Parameters for array-target distance = 114.6mm: y = mx + c")
print(f"m = {a_opt2:.3f} ± {a_err2:.3f}")
print(f"c = {b_opt2:.3f} ± {b_err2:.3f}")
print("Chi-square value: {:.3f}".format(chi_square2))

print("\nFitting Parameters for array-target distance = 135mm: y = mx + c")
print(f"m = {a_opt1:.3f} ± {a_err1:.3f}")
print(f"c = {b_opt1:.3f} ± {b_err1:.3f}")
print("Chi-square value: {:.3f}".format(chi_square1))


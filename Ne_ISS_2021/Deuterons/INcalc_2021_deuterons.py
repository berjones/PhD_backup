# Python script to calculate the intensity times by number of target atoms used to normalise 

# ---- DWBA differential cross section integrated between: 28.599 < theta_cm < 28.749
# ---- The numerical result of the integrated cross section is 31.215577 (+-3.46563e-13).

# ---- The fitted yield 6.31e3 in counts per 5 keV 

# The second workfit is the singles elum peak

#WorkFit on spectrum: 2 (elum_recoil)
# id  stat  chi   channel      pos          vol       width  
#━━━━────────────────────────────────────────────────────────
# .0    AV  262  32.51(23)  162.5(1.2)  1.17(4)e+03  79.5(8) 
# .1    AV  262   62.39(9)    311.9(5)  6.35(8)e+03  79.5(8) 

# exponential 

# WorkFit on spectrum: 2 (elum_recoil)
#  id  stat  chi  channel     pos         vol       width  
# ━━━━─────────────────────────────────────────────────────
#  .0    AV   52  62.72(9)  313.6(5)  5.94(8)e+03  75.1(9) 


Yield = 5.94e3 
DiffCrossInt = 12.719718 

IN_t = Yield/DiffCrossInt
print("Recoils:")
print("Rate times target nuclei (IN_t) for the 2021 data: ", IN_t)
print("")

# Rate times target nuclei (IN_t) for the 2021 data:  202.142667425305

# singles 
#WorkFit on spectrum: 0 (elum)
# id  stat  chi   channel     pos          vol        width  
#━━━━────────────────────────────────────────────────────────
# .0    AV  2105  33.17(8)  165.9(4)  1.336(13)e+04  81.2(5) 
# .1    AV  2105  61.87(9)  309.4(4)  1.128(12)e+04  81.2(5) 

# exponential 
# WorkFit on spectrum: 1 (elum)
#  id  stat  chi   channel     pos         vol         width   
# ━━━━─────────────────────────────────────────────────────────
#  .0    AV   86  63.35(10)  316.7(5)  7.58(11)e+03  66.3(1.0) 

Yields = 7.58e3 
DiffCrossInts =  12.719718

IN_ts = Yields/DiffCrossInts
print("Singles:")
print("Rate times target nuclei (IN_t) for the 2021 data: ", IN_ts)
print("")


# ebis 
#WorkFit on spectrum: 1 (elum_ebis)
# id  stat  chi    channel     pos          vol        width  
#━━━━─────────────────────────────────────────────────────────
# .0    AV  1228  33.56(11)  167.8(6)    6.09(9)e+03  79.4(7) 
# .1    AV  1228   62.27(8)  311.3(4)  1.059(11)e+04  79.4(7) 

#exponential 

# WorkFit on spectrum: 0 (elum_ebis)
#  id  stat  chi  channel     pos         vol        width  
# ━━━━──────────────────────────────────────────────────────
#  .0    AV  126  63.19(9)  316.0(4)  8.05(11)e+03  68.0(9) 

Yielde = 8.05e03
DiffCrossInte =  12.719718

IN_te = Yielde/DiffCrossInte
print("EBIS:")
print("Rate times target nuclei (IN_t) for the 2021 data: ", IN_te)
print("")


#Output

# Recoils:
# Rate times target nuclei (IN_t) for the 2021 data:  466.9914851885867

# Singles:
# Rate times target nuclei (IN_t) for the 2021 data:  595.9251612339204

# EBIS:
# Rate times target nuclei (IN_t) for the 2021 data:  632.8756659542295
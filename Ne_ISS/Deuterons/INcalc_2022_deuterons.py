# Python script to calculate the intensity times by number of target atoms used to normalise 

# DWBA differential cross section integrated between: 20.228 < theta_cm < 20.348
# The numerical result of the integrated cross section is 81.026416 (+-8.99574e-13).

# recoil gated dueterons

# id  stat  chi   channel        pos          vol        width    
#━━━━─────────────────────────────────────────────────────────────
# .0    AV  382  298.87(19)  1494.4(1.0)  6.37(9)e+03  130.5(2.1) 
# .1    AV  382    330.1(5)  1650.4(2.3)  1.28(5)e+03  130.5(2.1) 

Yieldr = 6.37e3 + 1.28e3 
DiffCrossIntr = 7.142104

IN_tr = Yieldr/DiffCrossIntr
print("Recoil gated:")
print("Rate times target nuclei (IN_t) for the 2022 data: ", IN_tr)
print("")
# Rate times target nuclei (IN_t) for the 2022 data:  94.41365393725424


# singles 
#WorkFit on spectrum: 0 (elum)
# id  stat  chi    channel        pos           vol         width    
#━━━━────────────────────────────────────────────────────────────────
# .0    AV  1015  299.03(12)    1495.1(6)  1.908(17)e+04  133.8(1.4) 
# .1    AV  1015  330.98(32)  1654.9(1.6)   4.08(11)e+03  133.8(1.4) 

Yields = 1.908e4 + 4.08e3 
DiffCrossInts = 7.142104

IN_ts = Yields/DiffCrossInts
print("Singles:")
print("Rate times target nuclei (IN_t) for the 2022 data: ", IN_ts)
print("")
# Rate times target nuclei (IN_t) for the 2022 data:  285.8327091747462

# ebis 
#WorkFit on spectrum: 1 (elum_ebis)
# id  stat  chi   channel        pos           vol         width    
#━━━━───────────────────────────────────────────────────────────────
# .0    AV  924  299.05(13)    1495.3(7)  1.648(16)e+04  131.5(1.5) 
# .1    AV  924  330.60(34)  1653.0(1.7)   3.49(10)e+03  131.5(1.5) 

Yielde = 1.648e4 + 3.49e3 
DiffCrossInte = 7.142104

IN_te = Yielde/DiffCrossInte
print("EBIS:")
print("Rate times target nuclei (IN_t) for the 2022 data: ", IN_te)
print("")



# Recoil gated:
# Rate times target nuclei (IN_t) for the 2022 data:  1071.1129381481983

# Singles:
# Rate times target nuclei (IN_t) for the 2022 data:  3242.741914707487

# EBIS:
# Rate times target nuclei (IN_t) for the 2022 data:  2796.094820237846


Yieldrx2 = 6.37e3 + (2*1.28e3) 
DiffCrossIntrx2 = 7.142104

IN_trx2 = Yieldrx2/DiffCrossIntrx2
print("Recoil gated:")
print("Rate times target nuclei (IN_t) for the 2022 data x2 double bump: ", IN_trx2)
print("")

# Recoil gated:
# Rate times target nuclei (IN_t) for the 2022 data x2 double bump:  1250.3318349886813
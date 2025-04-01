clc;
clear;
close all:

# I - Signaux temporels
# *********************

# I.1 - Signaux temporels pour un La 4
# ====================================

# Comme nous avons utilisé le même appareil de mesure pour enregistrer nos
# données, la fréquence d'échantillonnage Fs est la même, donc on la récupère
# qu'une fois (*).

# Importation des données AVEC pédale et définition de leur axe temporel
# ----------------------------------------------------------------------

[y_La4_B_AP, Fs] = audioread('La_4_bref_avec_pedale.wav'); #(*)
t_La4_B_AP = (0:length(y_La4_B_AP)-1) / Fs;

y_La4_L_AP = audioread('La_4_long_avec_pedale.wav');
t_La4_L_AP = (0:length(y_La4_L_AP)-1) / Fs;

y_La4_M_AP = audioread('La_4_maintenue_avec_pedale.wav');
t_La4_M_AP = (0:length(y_La4_M_AP)-1) / Fs;

# Importation des données SANS pédale et définition de leur axe temporel
# ----------------------------------------------------------------------

y_La4_B_SP = audioread('La_4_bref_sans_pedale.wav');
t_La4_B_SP = (0:length(y_La4_B_SP)-1) / Fs;

y_La4_L_SP = audioread('La_4_long_sans_pedale.wav');
t_La4_L_SP = (0:length(y_La4_L_SP)-1) / Fs;

y_La4_M_SP = audioread('La_4_maintenue_sans_pedale.wav');
t_La4_M_SP = (0:length(y_La4_M_SP)-1) / Fs;

# Affichage des signaux temporels
# -------------------------------

figure 1;
subplot(211);
hold on;
plot(t_La4_B_AP, y_La4_B_AP);
plot(t_La4_L_AP, y_La4_L_AP);
plot(t_La4_M_AP, y_La4_M_AP);
hold off;
xlabel('Temps (en s)');
ylabel('Amplitude');
title('Signaux temporels pour un La 4 AVEC pédale');
legend('Bref', 'Long', 'Maintenu');

subplot(212);
hold on;
plot(t_La4_B_SP, y_La4_B_SP);
plot(t_La4_L_SP, y_La4_L_SP);
plot(t_La4_M_SP, y_La4_M_SP);
hold off;
xlabel('Temps (en s)');
ylabel('Amplitude');
title('Signaux temporels pour un La 4 SANS pédale');
legend('Bref', 'Long', 'Maintenu');

# I.2 - Signaux temporels pour un accord
# ======================================

# Importation des données AVEC pédale et définition de leur axe temporel
# ----------------------------------------------------------------------

y_A_B_AP = audioread('Accord_bref_avec_pedale.wav'); #(*)
t_A_B_AP = (0:length(y_A_B_AP)-1) / Fs;

y_A_L_AP = audioread('Accord_long_avec_pedale.wav');
t_A_L_AP = (0:length(y_A_L_AP)-1) / Fs;

y_A_M_AP = audioread('Accord_maintenu_avec_pedale.wav');
t_A_M_AP = (0:length(y_A_M_AP)-1) / Fs;

# Importation des données SANS pédale et définition de leur axe temporel
# ----------------------------------------------------------------------

y_A_B_SP = audioread('Accord_bref_sans_pedale.wav');
t_A_B_SP = (0:length(y_A_B_SP)-1) / Fs;

y_A_L_SP = audioread('Accord_long_avec_pedale.wav');
t_A_L_SP = (0:length(y_A_L_SP)-1) / Fs;

y_A_M_SP = audioread('Accord_maintenu_sans_pedale.wav');
t_A_M_SP = (0:length(y_A_M_SP)-1) / Fs;

# Affichage des signaux temporels
# -------------------------------

figure 2;
subplot(211);
hold on;
plot(t_A_B_AP, y_A_B_AP);
plot(t_A_L_AP, y_A_L_AP);
plot(t_A_M_AP, y_A_M_AP);
hold off;
xlabel('Temps (en s)');
ylabel('Amplitude');
title('Signaux temporels pour un accord AVEC pédale');
legend('Bref', 'Long', 'Maintenu');

subplot(212);
hold on;
plot(t_A_B_SP, y_A_B_SP);
plot(t_A_L_SP, y_A_L_SP);
plot(t_A_M_SP, y_A_M_SP);
hold off;
xlabel('Temps (en s)');
ylabel('Amplitude');
title('Signaux temporels pour un accord SANS pédale');
legend('Bref', 'Long', 'Maintenu');



# II - DSP
# ********

# II.1 - DSP pour un La 4
# =======================

# Transformée de Fourier des données AVEC pédale et définition de leur axe fréquentiel
# ------------------------------------------------------------------------------------

N_La4_B_AP = length(y_La4_B_AP);
Y_La4_B_AP = abs(fft(y_La4_B_AP));
nu_La4_B_AP = (0:N_La4_B_AP-1) / N_La4_B_AP;

N_La4_L_AP = length(y_La4_L_AP);
Y_La4_L_AP = abs(fft(y_La4_L_AP));
nu_La4_L_AP = (0:N_La4_L_AP-1) / N_La4_L_AP;

N_La4_M_AP = length(y_La4_M_AP);
Y_La4_M_AP = abs(fft(y_La4_M_AP));
nu_La4_M_AP = (0:N_La4_M_AP-1) / N_La4_M_AP;

# Transformée de Fourier des données SANS pédale et définition de leur axe fréquentiel
# ------------------------------------------------------------------------------------

N_La4_B_SP = length(y_La4_B_SP);
Y_La4_B_SP = abs(fft(y_La4_B_SP));
nu_La4_B_SP = (0:N_La4_B_SP-1) / N_La4_B_SP;

N_La4_L_SP = length(y_La4_L_SP);
Y_La4_L_SP = abs(fft(y_La4_L_SP));
nu_La4_L_SP = (0:N_La4_L_SP-1) / N_La4_L_SP;

N_La4_M_SP = length(y_La4_M_SP);
Y_La4_M_SP = abs(fft(y_La4_M_SP));
nu_La4_M_SP = (0:N_La4_M_SP-1) / N_La4_M_SP;

# Affichage des DSP
# -----------------

figure 3;
subplot(211);
hold on;
plot(nu_La4_B_AP, Y_La4_B_AP);
plot(nu_La4_L_AP, Y_La4_L_AP);
plot(nu_La4_M_AP, Y_La4_M_AP);
hold off;
xlabel('Fréquence normalisée');
ylabel('Amplitude');
title('DSP pour un La 4 AVEC pédale');
legend('Bref', 'Long', 'Maintenu');

subplot(212);
hold on;
plot(nu_La4_B_SP, Y_La4_B_SP);
plot(nu_La4_L_SP, Y_La4_L_SP);
plot(nu_La4_M_SP, Y_La4_M_SP);
hold off;
xlabel('Fréquence normalisée');
ylabel('Amplitude');
title('DSP pour un La 4 SANS pédale');
legend('Bref', 'Long', 'Maintenu');

# II.2 - DSP pour un accord
# =========================

# Transformée de Fourier des données AVEC pédale et définition de leur axe fréquentiel
# ------------------------------------------------------------------------------------

N_A_B_AP = length(y_A_B_AP);
Y_A_B_AP = abs(fft(y_A_B_AP));
nu_A_B_AP = (0:N_A_B_AP-1) / N_A_B_AP;

N_A_L_AP = length(y_A_L_AP);
Y_A_L_AP = abs(fft(y_A_L_AP));
nu_A_L_AP = (0:N_A_L_AP-1) / N_A_L_AP;

N_A_M_AP = length(y_A_M_AP);
Y_A_M_AP = abs(fft(y_A_M_AP));
nu_A_M_AP = (0:N_A_M_AP-1) / N_A_M_AP;

# Transformée de Fourier des données SANS pédale et définition de leur axe fréquentiel
# ------------------------------------------------------------------------------------

N_A_B_SP = length(y_A_B_SP);
Y_A_B_SP = abs(fft(y_A_B_SP));
nu_A_B_SP = (0:N_A_B_SP-1) / N_A_B_SP;

N_A_L_SP = length(y_A_L_SP);
Y_A_L_SP = abs(fft(y_A_L_SP));
nu_A_L_SP = (0:N_A_L_SP-1) / N_A_L_SP;

N_A_M_SP = length(y_A_M_SP);
Y_A_M_SP = abs(fft(y_A_M_SP));
nu_A_M_SP = (0:N_A_M_SP-1) / N_A_M_SP;

# Affichage des DSP
# -----------------

figure 4;
subplot(211);
hold on;
plot(nu_A_B_AP, Y_A_B_AP);
plot(nu_A_L_AP, Y_A_L_AP);
plot(nu_A_M_AP, Y_A_M_AP);
hold off;
xlabel('Fréquence normalisée');
ylabel('Amplitude');
title('DSP pour un accord AVEC pédale');
legend('Bref', 'Long', 'Maintenu');

subplot(212);
hold on;
plot(nu_A_B_SP, Y_A_B_SP);
plot(nu_A_L_SP, Y_A_L_SP);
plot(nu_A_M_SP, Y_A_M_SP);
hold off;
xlabel('Fréquence normalisée');
ylabel('Amplitude');
title('DSP pour un accord SANS pédale');
legend('Bref', 'Long', 'Maintenu');

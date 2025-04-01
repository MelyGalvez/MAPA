clc;
clear;
close all;

% I - Création des signaux de base (La0: 27,5 Hz)
% ***********************************************

fs = 48000;
duration = 0.25;
t = 0:1/fs:duration;

% I.1. Signal sinusoidale théorique
% =================================

f_0 = 27.5;
y_La0_Th = sin(2 * pi * f_0 * t);

% I.2. Signal sinusoidale réel bruité
% ===================================

sig_sq = 0.005;
N = length(t);
y_La0_B = y_La0_Th + sqrt(sig_sq) * randn(1, N);

% I.3. Signal sinusoidale réel bruité puis filtré
% ===============================================

fc = 20000;
ordre = 1;
[b, a] = butter(ordre, fc / fs, 'low');

y_La0_F = filter(b, a, y_La0_B);

% I.4. Représentation des signaux
% ===============================

figure 1;
hold on;
plot(t, y_La0_Th);
plot(t, y_La0_B);
plot(t, y_La0_F);
hold off;
xlabel('Temps (en s)');
ylabel('Amplitude');
title('Signaux temporels du La0');
legend('Théorique', 'Bruité', 'Filtré');

% II - Application de la méthode 1 (algorithme glouton)
% *****************************************************

% II.1. Génération des bases de donnée vide
% =========================================

N_touches = 88;

BDD_Th = zeros(length(t), N_touches);
BDD_B = zeros(length(t), N_touches);
BDD_F = zeros(length(t), N_touches);

% II.2. DSP dans notre base de donnée
% ===================================

Rapport = 2^(1/12);

% Initialisation des variables de mesure du temps et de la mémoire
time_values = zeros(1, N_touches);  % Temps de calcul
memory_usage = zeros(1, N_touches);  % Mémoire utilisée à chaque itération

for k = 1:N_touches
    tic; % Démarrage du chronomètre

    f = f_0 * Rapport^(k-1);

    y_Th = sin(2 * pi * f * t);
    y_B = y_Th + sqrt(sig_sq) * randn(1, N);
    y_F = filter(b, a, y_B);

    Y_Th = abs(fft(y_Th));
    Y_B = abs(fft(y_B));
    Y_F = abs(fft(y_F));

    % Enregistrement des résultats dans les bases de données
    BDD_Th(:, k) = Y_Th';
    BDD_B(:, k) = Y_B';
    BDD_F(:, k) = Y_F';

    % Mesure de la mémoire utilisée après chaque itération
    memory_info = whos('BDD_Th', 'BDD_B', 'BDD_F');
    memory_usage(k) = sum([memory_info.bytes]); % Total de la mémoire utilisée

    % Arrêt du chronomètre et stockage du temps
    time_values(k) = toc;
end

% Affichage de la complexité temporelle
figure 2;
plot(1:N_touches, cumsum(time_values), 'o-');
xlabel('N_touches');
ylabel('Temps cumulé (s)');
title('Complexité temporelle de la boucle');
grid on;

% Affichage de la complexité spatiale
figure 3;
plot(1:N_touches, memory_usage / 1e6, 'o-'); % Conversion en Mo
xlabel('N_touches');
ylabel('Mémoire cumulée (Mo)');
title('Complexité spatiale de la boucle');
grid on;

% II.2. Utilisation de la DSP
% ===========================

Note_jouee1 = 75;
Note_jouee2 = 45;

time_plot = zeros(1, 3);

figure 4;
hold on;

tic;
plot(linspace(0, 1, length(BDD_Th(:, Note_jouee1))), BDD_Th(:, Note_jouee1) + BDD_Th(:, Note_jouee2));
time_plot(1) = toc;

tic;
plot(linspace(0, 1, length(BDD_B(:, Note_jouee1))), BDD_B(:, Note_jouee1) + BDD_B(:, Note_jouee2));
time_plot(2) = toc;

tic;
plot(linspace(0, 1, length(BDD_F(:, Note_jouee1))), BDD_F(:, Note_jouee1) + BDD_F(:, Note_jouee2));
time_plot(3) = toc;

hold off;
xlabel('Fréquence normalisée (nu)');
ylabel('Amplitude');
title('DSP pour une note choisie');
legend('Théorique', 'Bruité', 'Filtré');

% Mesure du temps des tracés
time_plot_cumsum = cumsum(time_plot);
figure 5;
plot(1:3, time_plot_cumsum, 'o-');
xlabel('Opérations de tracé');
ylabel('Temps cumulé (s)');
title('Complexité temporelle des tracés');
grid on;

% III - Application de la méthode 2 (algorithme fin)
% **************************************************

% III.1. Génération des bases de données vides
% ============================================

BDD_Th3 = zeros(1, N_touches);
BDD_B3 = zeros(1, N_touches);
BDD_F3 = zeros(1, N_touches);

% III.2. DSP dans notre base de donnée
% ====================================

Ref = zeros(1, 12001);  % Initialisation de Ref

% Variables de mesure du temps
time_total = zeros(1, N_touches);  % Temps total pour chaque itération (88 touches)

for k = 1:N_touches
    f = f_0 * Rapport^(k-1);  % Fréquence théorique pour référence

    tic;  % Démarre le chronomètre pour l'itération k

    % Calcul des signaux
    y_Th = sin(2 * pi * f * t);
    y_B = y_Th + sqrt(sig_sq) * randn(1, N);
    y_F = filter(b, a, y_B);

    % Calcul des FFT
    Y_Th = abs(fft(y_Th));
    Y_B = abs(fft(y_B));
    Y_F = abs(fft(y_F));

    % Extraction de l'indice de la fréquence dominante
    [~, idx_Th] = max(Y_Th);
    [~, idx_B] = max(Y_B);
    [~, idx_F] = max(Y_F);

    % Calcul des fréquences dominantes
    freq_Th = (idx_Th - 1) * (1 / (t(end) - t(1)));
    freq_B = (idx_B - 1) * (1 / (t(end) - t(1)));
    freq_F = (idx_F - 1) * (1 / (t(end) - t(1)));

    % Stockage des fréquences dominantes
    BDD_Th3(k) = freq_Th;
    BDD_B3(k) = freq_B;
    BDD_F3(k) = freq_F;

    % Si la fréquence est proche de 440 Hz, on stocke la FFT
    if abs(f - 440) < 0.1
        Ref = Y_Th;  % Stocker la FFT théorique dans Ref (ou Y_B ou Y_F selon ton choix)
    end

    % Mesure du temps total pour chaque itération
    time_total(k) = toc;  % Mesure du temps écoulé pour cette itération
end

% Affichage de la complexité temporelle globale pour les 88 touches
figure;
plot(1:N_touches, cumsum(time_total), 'o-');
xlabel('N_touches');
ylabel('Temps cumulé (s)');
title('Complexité temporelle globale pour les 88 touches');
grid on;



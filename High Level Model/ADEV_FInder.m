close all;
clear all;

load("ADEV_Data");

fh = 10^7;
tau = logspace(0,5,1000).';
f = logspace(-3,log10(fh),10000);
df = zeros(size(f));
df(1) = f(1);
dtau = zeros(size(tau));
dtau(1) = tau(1);
for i = 2:1:size(f)
    df(i) = f(i) - f(i-1);
end
df = repmat(df, size(tau,1), 1);

for i = 2:1:size(tau)
    dtau(i) = tau(i) - tau(i-1);
end

rows = size(tau,1);
cols = size(f,2);

pift = zeros([rows cols]);

% ADEV_Interp = zeros(size(tau));
ADEV_Interp = interp1(ADEV_Data(:,1), ADEV_Data(:,2), tau);

% figure();
% loglog(f, [f.^-2; f.^-1; f.^1; f.^0; f.^2]);
% legend(["f.^-2" "f.^-1" "f.^1" "1" "f.^2"]);
A = 1.038 + 3*log(2*pi*fh*ADEV_Data(:,1));
whitePM = (2*pi)^2/(3*fh) * repmat((ADEV_Data(:,1).^2 .* ADEV_Data(:,2).^2),size(f)) .* (f.^2);
flickerPM = repmat((2*pi)^2./(A) .* (ADEV_Data(:,1).^2 .* ADEV_Data(:,2).^2),size(f)) .* (f);
whiteFM = 2*repmat((ADEV_Data(:,1) .* ADEV_Data(:,2).^2),size(f));
flickerFM = 1/(2*log(2))*repmat(ADEV_Data(:,2).^2,size(f)) .* (f.^-1);
randomWalkFM = 6/(2*pi)^2 * repmat((ADEV_Data(:,1).^-1 .* ADEV_Data(:,2).^2),size(f)) .* (f.^-2);
noises = cat(3, whitePM, flickerPM, whiteFM, flickerFM, randomWalkFM);
noiseWeights = [0.0000001 0.00001 0.00001 100 10000000];
noiseWeights = permute(noiseWeights,[3 1 2]);

noises = noises.*noiseWeights;

totalNoise = sum(noises,3);
totalNoiseInterp = interp1(ADEV_Data(:,1), totalNoise,tau);
totalNoiseInterpCombined = repmat(sum(totalNoiseInterp.*dtau, 1), size(tau));

figure();
tiledlayout(6,1);
nexttile;
loglog(f,whitePM);
nexttile;
loglog(f,flickerPM);
nexttile;
loglog(f,whiteFM);
nexttile;
loglog(f,flickerFM);
nexttile;
loglog(f,randomWalkFM);
nexttile;
loglog(f, whitePM + flickerPM + whiteFM + flickerFM + randomWalkFM);

spectrTerms = [(((f-100)).^-4)];

figure();
loglog(f, spectrTerms);
legend(["-4" "-3" "-2" "-1" "0"]);

spectr = prod(spectrTerms,1);
spectr = repmat(spectr, size(tau,1), 1);

for r = 1:1:rows
    for c = 1:1:cols
        pift(r,c) = pi * f(c) * tau(r);
    end
end

num = sin(pift).^4;
den = pift.^2;

AVAR = 2 * sum(spectr .* num ./ den .* df,2);

ADEV = sqrt(AVAR);

figure();
tiledlayout(2,1);
nexttile;
loglog(f, spectr);
hold on;
% loglog(f, totalNoiseInterpCombined, "Color",[0 0 0]);
xlabel("Frequency (Hz)");
title("Noise Power Spectrum");
% ylim([-2 2]);

nexttile;
plotdata = [tau ADEV];
loglog(tau, ADEV);
xlabel("Tau (s)");
ylabel("ADEV");
title("Allan Deviation from power spectrum");
hold on;
loglog(tau, ADEV_Interp);
ylim([10^-15 10^-6]);

disp(min(ADEV));
disp(min(ADEV_Interp));
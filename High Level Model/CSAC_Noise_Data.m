load("ADEV_Data");

fh = 10^1;
tau = logspace(0,5,1000).';
f = logspace(-5,log10(fh),10000);
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


A = 1.038 + 3*log(2*pi*fh*ADEV_Data(:,1));
whitePM = (2*pi)^2/(3*fh) * repmat((ADEV_Data(:,1).^2 .* ADEV_Data(:,2).^2),size(f)) .* (f.^2);
flickerPM = repmat((2*pi)^2./(A) .* (ADEV_Data(:,1).^2 .* ADEV_Data(:,2).^2),size(f)) .* (f);
whiteFM = 2*repmat((ADEV_Data(:,1) .* ADEV_Data(:,2).^2),size(f));
flickerFM = 1/(2*log(2))*repmat(ADEV_Data(:,2).^2,size(f)) .* (f.^-1);
randomWalkFM = 6/(2*pi)^2 * repmat((ADEV_Data(:,1).^-1 .* ADEV_Data(:,2).^2),size(f)) .* (f.^-2);
noises = cat(3, whitePM, flickerPM, whiteFM, flickerFM, randomWalkFM);
noiseWeights = [1 1 1 1 1];
noiseWeights = permute(noiseWeights,[3 1 2]);

noises = noises.*noiseWeights;

totalNoise = sum(noises,3);
totalNoiseCombined = sum(totalNoise.*ADEV_Data(:,1), 1);
totalNoiseInterp = interp1(ADEV_Data(:,1), totalNoise,tau);
totalNoiseInterpCombined = repmat(sum(totalNoiseInterp.*dtau, 1), size(tau));

simulinkNoisePairs = [f.' totalNoiseCombined.'];
simulinkNoisePairs = simulinkNoisePairs(1:500:end,:).';
%%
%Useable Transfer Function
tnum = [1 40 0.012 10^-6 4*10^-11] * 10^-9;
tden = [0 0 1 0 0];
limPoles = [1 20000 1.5*10^8 5*10^11 6.25*10^14];
H = tf(tnum, conv(tden, limPoles));
H
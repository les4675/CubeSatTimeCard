close all;

scenarios = 20;
samples = 1000;
t = logspace(-5, 0, samples);
noises = zeros([3 samples scenarios]);

devS = 10^-12;
devW = 10^-8;
devF = 10^-11;

mindt = t(2) - t(1);

for k = 1:1:scenarios
    for i = 2:1:samples
        noises(1,i,k) = randn()*devS;
    
        noises(2,i,k) = noises(2,i-1,k);
        for j = 1:1:((t(i)-t(i-1))/mindt)
            noises(2,i,k) = noises(2,i,k) + randn()*devW;
        end
    
        flick = zeros([1 round((t(i)-t(i-1))/mindt)]);
        for j = 1:1:((t(i)-t(i-1))/mindt)
            flick(j) = randn()*devF;
        end
        noises(3,i,k) = mean(flick);
    end
end

total = sum(noises, 1);

figure();
hold on;
for k = 1:1:scenarios
    semilogx(t, total(:,:,k).');
end

figure();
semilogx(t,std(total,0,3));
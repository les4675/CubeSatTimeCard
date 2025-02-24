close all;
x = 2 .^ linspace(1, 31, 50000);
fout = 16*10^6*(1 + 10^-15*x);
semilogx(x,fout);
xlabel("X");
ylabel("Frequency (Hz)");

figure();
plot(x,fout);
xlabel("X");
ylabel("Frequency (Hz)");


figure();
x = logspace(0,22,5000);
fout = 16*10^6*(1 + 10^-15*x);
loglog(x,fout);
xlabel("X");
ylabel("Frequency (Hz)");
F = 60e6;
ppm = 0.15e-6;

T = F*(1+ppm)
Day = (T-F)*(24*60*60)
delayperday = Day/F %in 24 hours
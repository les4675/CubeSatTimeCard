data = [testData.time, testData.phase, testData.temp];
%% 
phaseRange = 100000; %in nanoseconds
outliersRemoved = [];
for i = 1:1:size(data,1)
    if abs(data(i,2)) < phaseRange
        outliersRemoved = [outliersRemoved; data(i,:)];
    end
end
%%
close all;
longTermSection = outliersRemoved(69000:10:end,:);
fit = polyfit(longTermSection(:,1),longTermSection(:,2),15);
phaseCurve = polyval(fit,longTermSection(:,1));

fitTemp = polyfit(longTermSection(:,1),longTermSection(:,3),15);
tempCurve = polyval(fitTemp,longTermSection(:,1));

phaseAvg = movmean(longTermSection(:,2),2000);
tempAvg = movmean(longTermSection(:,3),500);


% movmean(longTermSection(:,2),10,)
figure();
tiledlayout(2,1);
nexttile;
plot(longTermSection(:,1), longTermSection(:,2));
hold on;
plot(longTermSection(:,1), phaseCurve);
plot(longTermSection(:,1), phaseAvg);
xlabel("Time (seconds)");
ylabel("Phase Error (ns)");
title("Long-Term Test Phase Error Data");
legend(["Original" "Moving Average" "Poly Fit" "Smooth Tool"]);
nexttile;
plot(longTermSection(:,1), longTermSection(:,3));
hold on;
plot(longTermSection(:,1), tempAvg);
plot(longTermSection(:,1), tempCurve);
xlabel("Time (seconds)");
ylabel("Temperature (deg C)");
title("Long-Term Test Temperature Data");
legend(["Original" "Moving Average" "Poly Fit"]);



figure();
diffs = [diff(longTermSection(:,1)) diff(phaseCurve)];
derivative = (diffs(:,2) ./ diffs(:,1)) * 10^-9; %ADJUSTED FOR UNITS OF ns/ns!!
plot(longTermSection(2:end,1), derivative);

pad = 1000;
compiledData = [longTermSection(pad+1:end-pad,1), phaseCurve(pad+1:end-pad), derivative(pad:end-pad), tempCurve(pad+1:end-pad)];
figure();
tiledlayout(3,1);
nexttile;
plot(compiledData(:,1),compiledData(:,2));
xlabel("Time (seconds)");
ylabel("Phase Error (ns)");
title("Processed Long-Term Test Phase Error Data");
nexttile;
plot(compiledData(:,1),compiledData(:,3));
xlabel("Time (seconds)");
ylabel("Frequency Error (ns/ns)");
title("Processed Long-Term Test Frequency Error Data");
nexttile;
plot(compiledData(:,1),compiledData(:,4));
xlabel("Time (seconds)");
ylabel("Temperature (deg C)");
title("Processed Long-Term Test Temperature Data");

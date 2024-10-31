%%% Clock Data
[Y, M, D, H, MN, S] = fcn();
[Y, M, D, H, MN, S] = TimeKeeping(Y, M, D, H, MN, S, 1);
TofD = [Y, M, D, H, MN, S];
function [Y, M, D, H, MN, S] = fcn()
   eml.extrinsic('now');
   eml.extrinsic('datevec');
   Y = 0;
   M = 0;
   D = 0;
   H = 0;
   MN = 0;
   S = 0;
   [Y, M, D, H, MN, S] = datevec(now);
end

%%% Clock Data


function [Y, M, D, H, MN, S] = TimeKeeping(Y, M, D, H, MN, S, PPS)
   S = S + PPS;
   if (S > 60)
       S = S- 60;
       MN = MN+1;
   end
   if (MN > 60)
       MN = MN - 60;
       H = H + 1;
   end
   if (H > 24)
       H = H - 24;
       D = D + 1;
   end
   if (D > 30)
       D = D - 30;
       M = M + 1;
   end
   if (M > 12)
       M = M - 12;
       Y = Y+1;
   end
end
function initState = initStateEKF(meanAccel, meanMag, meanAlti)
    initState = zeros(6,1);
    initState(1:4) = compact(ecompass(meanAccel, meanMag));
    initState(5) = meanAlti;
end
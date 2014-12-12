%% all test done.
function [out out2] = RangeErrorConduction(obp,cp,fp,cToObAE,cTofAE,fToObAE)
TcToObA = (obp - cp)/norm((obp - cp));
TcToObA = atan2d(TcToObA(2),TcToObA(1));
TLengthCToOB = norm(obp - cp);

TfToObA = (obp - fp)/norm(obp - fp);
TfToObA = atan2d(TfToObA(2),TfToObA(1));
TLengthFToOB = norm(obp - fp);

TcTofA = (fp - cp)/norm((fp - cp));
TcTofA = atan2d(TcTofA(2),TcTofA(1));
TLengthCTof = norm(fp - cp);

NcToObA = TcToObA + cToObAE;
NfToObA = TfToObA + fToObAE;
NcTofA =  TcTofA +  cTofAE;

Nfp = cp + TLengthCTof* [cosd(NcTofA) sind(NcTofA)];
NfpToObU = Nfp + 99999999*TLengthFToOB* [cosd(NfToObA) sind(NfToObA)];
NcpToObU = cp + 99999999*TLengthCToOB* [cosd(NcToObA) sind(NcToObA)];

%[xi, yi] = polyxpoly([Nfp(1) NfpToObU(1)],[Nfp(2) NfpToObU(2)],[cp(1) NcpToObU(1)],[cp(2) NcpToObU(2)]);
p = linlinintersect([Nfp; NfpToObU; cp; NcpToObU;]);



if ~isempty(p)
    out = norm(obp-p);
    out2 = dot(p-obp,obp-cp);
else
    out = inf;
end
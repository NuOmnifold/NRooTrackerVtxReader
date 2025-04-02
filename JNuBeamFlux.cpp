#include "JNuBeamFlux.h"
ClassImp(ND::JNuBeamFlux)

namespace ND {
    JNuBeamFlux::JNuBeamFlux() : RooTrackerVtxBase(), 
        NuFluxEntry(0),
        NuFileName(nullptr),
        NuParentPdg(0),
        NuParentDecMode(0),
        NuCospibm(0),
        NuNorm(0),
        NuCospi0bm(0),
        NuRnu(0),
        NuIdfd(0),
        NuGipart(0),
        NuGamom0(0),
        NuNg(0),
        NuEnusk(0),
        NuNormsk(0),
        NuAnorm(0),
        NuVersion(0),
        NuTuneid(0),
        NuNtrig(0),
        NuPint(0),
        NuRand(0)
    {
        // Initialize arrays
        for (int i = 0; i < 4; ++i) {
            NuParentDecP4[i] = 0;
            NuParentDecX4[i] = 0;
            NuParentProP4[i] = 0;
            NuParentProX4[i] = 0;
        }
        
        for (int i = 0; i < 2; ++i) {
            NuXnu[i] = 0;
            NuBpos[i] = 0;
            NuBtilt[i] = 0;
            NuBrms[i] = 0;
            NuEmit[i] = 0;
            NuAlpha[i] = 0;
        }
        
        for (int i = 0; i < 3; ++i) {
            NuGpos0[i] = 0;
            NuGvec0[i] = 0;
            NuHcur[i] = 0;
        }
        
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 3; ++j) {
                NuGp[i][j] = 0;
                NuGv[i][j] = 0;
            }
            NuGcosbm[i] = 0;
            NuGpid[i] = 0;
            NuGmec[i] = 0;
            NuGmat[i] = 0;
            NuGdistc[i] = 0;
            NuGdistal[i] = 0;
            NuGdistti[i] = 0;
            NuGdistfe[i] = 0;
        }
    }

    JNuBeamFlux::JNuBeamFlux(const JNuBeamFlux& other) : RooTrackerVtxBase(other),
        NuFluxEntry(other.NuFluxEntry),
        NuParentPdg(other.NuParentPdg),
        NuParentDecMode(other.NuParentDecMode),
        NuCospibm(other.NuCospibm),
        NuNorm(other.NuNorm),
        NuCospi0bm(other.NuCospi0bm),
        NuRnu(other.NuRnu),
        NuIdfd(other.NuIdfd),
        NuGipart(other.NuGipart),
        NuGamom0(other.NuGamom0),
        NuNg(other.NuNg),
        NuEnusk(other.NuEnusk),
        NuNormsk(other.NuNormsk),
        NuAnorm(other.NuAnorm),
        NuVersion(other.NuVersion),
        NuTuneid(other.NuTuneid),
        NuNtrig(other.NuNtrig),
        NuPint(other.NuPint),
        NuRand(other.NuRand)
    {
        // Deep copy for TObjString
        NuFileName = (other.NuFileName) ? (TObjString*)other.NuFileName->Clone() : nullptr;
        
        // Copy arrays
        for (int i = 0; i < 4; ++i) {
            NuParentDecP4[i] = other.NuParentDecP4[i];
            NuParentDecX4[i] = other.NuParentDecX4[i];
            NuParentProP4[i] = other.NuParentProP4[i];
            NuParentProX4[i] = other.NuParentProX4[i];
        }
        
        for (int i = 0; i < 2; ++i) {
            NuXnu[i] = other.NuXnu[i];
            NuBpos[i] = other.NuBpos[i];
            NuBtilt[i] = other.NuBtilt[i];
            NuBrms[i] = other.NuBrms[i];
            NuEmit[i] = other.NuEmit[i];
            NuAlpha[i] = other.NuAlpha[i];
        }
        
        for (int i = 0; i < 3; ++i) {
            NuGpos0[i] = other.NuGpos0[i];
            NuGvec0[i] = other.NuGvec0[i];
            NuHcur[i] = other.NuHcur[i];
        }
        
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 3; ++j) {
                NuGp[i][j] = other.NuGp[i][j];
                NuGv[i][j] = other.NuGv[i][j];
            }
            NuGcosbm[i] = other.NuGcosbm[i];
            NuGpid[i] = other.NuGpid[i];
            NuGmec[i] = other.NuGmec[i];
            NuGmat[i] = other.NuGmat[i];
            NuGdistc[i] = other.NuGdistc[i];
            NuGdistal[i] = other.NuGdistal[i];
            NuGdistti[i] = other.NuGdistti[i];
            NuGdistfe[i] = other.NuGdistfe[i];
        }
    }

    JNuBeamFlux::~JNuBeamFlux() {
        // Clean up dynamically allocated memory
        if (NuFileName) delete NuFileName;
    }
}
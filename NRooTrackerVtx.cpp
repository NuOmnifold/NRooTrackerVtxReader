#include "NRooTrackerVtx.h"
ClassImp(ND::NRooTrackerVtx)

namespace ND {
    NRooTrackerVtx::NRooTrackerVtx() : JNuBeamFlux(),
        EvtCode(nullptr),
        EvtNum(0),
        EvtXSec(0),
        EvtDXSec(0),
        EvtWght(0),
        EvtProb(0),
        StdHepN(0),
        StdHepPdg(nullptr),
        StdHepStatus(nullptr),
        StdHepFd(nullptr),
        StdHepLd(nullptr),
        StdHepFm(nullptr),
        StdHepLm(nullptr),
        NEnvc(0),
        NEipvc(nullptr),
        NEiorgvc(nullptr),
        NEiflgvc(nullptr),
        NEicrnvc(nullptr),
        NEcrsx(0),
        NEcrsy(0),
        NEcrsz(0),
        NEcrsphi(0),
        NEnvert(0),
        NEiflgvert(nullptr),
        NEnvcvert(0),
        NEabspvert(nullptr),
        NEabstpvert(nullptr),
        NEipvert(nullptr),
        NEiverti(nullptr),
        NEivertf(nullptr),
        NFnvert(0),
        NFiflag(nullptr),
        NFx(nullptr),
        NFy(nullptr),
        NFz(nullptr),
        NFpx(nullptr),
        NFpy(nullptr),
        NFpz(nullptr),
        NFe(nullptr),
        NFfirststep(nullptr),
        NFnstep(0),
        NFecms2(nullptr),
        GeomPath(nullptr),
        GeneratorName(nullptr),
        OrigFileName(nullptr),
        OrigTreeName(nullptr),
        OrigEvtNum(0),
        OrigTreeEntries(0),
        OrigTreePOT(0),
        TimeInSpill(0),
        TruthVertexID(0)
    {
        // Initialize arrays
        for (int i = 0; i < 4; ++i) {
            EvtVtx[i] = 0;
        }
        
        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 4; ++j) {
                StdHepX4[i][j] = 0;
                StdHepP4[i][j] = 0;
            }
            
            for (int j = 0; j < 3; ++j) {
                StdHepPolz[i][j] = 0;
                NEpvc[i][j] = 0;
                NEposvert[i][j] = 0;
            }
        }
        
        for (int i = 0; i < 300; ++i) {
            for (int j = 0; j < 3; ++j) {
                NEdirvert[i][j] = 0;
            }
        }
    }

    NRooTrackerVtx::NRooTrackerVtx(const NRooTrackerVtx& other) : JNuBeamFlux(other),
        EvtNum(other.EvtNum),
        EvtXSec(other.EvtXSec),
        EvtDXSec(other.EvtDXSec),
        EvtWght(other.EvtWght),
        EvtProb(other.EvtProb),
        StdHepN(other.StdHepN),
        StdHepPdg(nullptr),
        StdHepStatus(nullptr),
        StdHepFd(nullptr),
        StdHepLd(nullptr),
        StdHepFm(nullptr),
        StdHepLm(nullptr),
        NEnvc(other.NEnvc),
        NEipvc(nullptr),
        NEiorgvc(nullptr),
        NEiflgvc(nullptr),
        NEicrnvc(nullptr),
        NEcrsx(other.NEcrsx),
        NEcrsy(other.NEcrsy),
        NEcrsz(other.NEcrsz),
        NEcrsphi(other.NEcrsphi),
        NEnvert(other.NEnvert),
        NEiflgvert(nullptr),
        NEnvcvert(other.NEnvcvert),
        NEabspvert(nullptr),
        NEabstpvert(nullptr),
        NEipvert(nullptr),
        NEiverti(nullptr),
        NEivertf(nullptr),
        NFnvert(other.NFnvert),
        NFiflag(nullptr),
        NFx(nullptr),
        NFy(nullptr),
        NFz(nullptr),
        NFpx(nullptr),
        NFpy(nullptr),
        NFpz(nullptr),
        NFe(nullptr),
        NFfirststep(nullptr),
        NFnstep(other.NFnstep),
        NFecms2(nullptr),
        OrigEvtNum(other.OrigEvtNum),
        OrigTreeEntries(other.OrigTreeEntries),
        OrigTreePOT(other.OrigTreePOT),
        TimeInSpill(other.TimeInSpill),
        TruthVertexID(other.TruthVertexID)
    {
        // Deep copy for TObjString pointers
        EvtCode = (other.EvtCode) ? (TObjString*)other.EvtCode->Clone() : nullptr;
        GeomPath = (other.GeomPath) ? (TObjString*)other.GeomPath->Clone() : nullptr;
        GeneratorName = (other.GeneratorName) ? (TObjString*)other.GeneratorName->Clone() : nullptr;
        OrigFileName = (other.OrigFileName) ? (TObjString*)other.OrigFileName->Clone() : nullptr;
        OrigTreeName = (other.OrigTreeName) ? (TObjString*)other.OrigTreeName->Clone() : nullptr;
        
        // Copy arrays
        for (int i = 0; i < 4; ++i) {
            EvtVtx[i] = other.EvtVtx[i];
        }
        
        // Allocate and copy dynamic arrays
        if (StdHepN > 0) {
            StdHepPdg = new Int_t[StdHepN];
            StdHepStatus = new Int_t[StdHepN];
            StdHepFd = new Int_t[StdHepN];
            StdHepLd = new Int_t[StdHepN];
            StdHepFm = new Int_t[StdHepN];
            StdHepLm = new Int_t[StdHepN];
            
            for (int i = 0; i < StdHepN; ++i) {
                StdHepPdg[i] = other.StdHepPdg[i];
                StdHepStatus[i] = other.StdHepStatus[i];
                StdHepFd[i] = other.StdHepFd[i];
                StdHepLd[i] = other.StdHepLd[i];
                StdHepFm[i] = other.StdHepFm[i];
                StdHepLm[i] = other.StdHepLm[i];
            }
        }
        
        // Copy StdHepX4, StdHepP4, StdHepPolz fixed arrays
        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 4; ++j) {
                StdHepX4[i][j] = other.StdHepX4[i][j];
                StdHepP4[i][j] = other.StdHepP4[i][j];
            }
            
            for (int j = 0; j < 3; ++j) {
                StdHepPolz[i][j] = other.StdHepPolz[i][j];
                NEpvc[i][j] = other.NEpvc[i][j];
                NEposvert[i][j] = other.NEposvert[i][j];
            }
        }
        
        // Allocate and copy NEnvc related arrays
        if (NEnvc > 0) {
            NEipvc = new Int_t[NEnvc];
            NEiorgvc = new Int_t[NEnvc];
            NEiflgvc = new Int_t[NEnvc];
            NEicrnvc = new Int_t[NEnvc];
            
            for (int i = 0; i < NEnvc; ++i) {
                NEipvc[i] = other.NEipvc[i];
                NEiorgvc[i] = other.NEiorgvc[i];
                NEiflgvc[i] = other.NEiflgvc[i];
                NEicrnvc[i] = other.NEicrnvc[i];
            }
        }
        
        // Allocate and copy NEnvert related arrays
        if (NEnvert > 0) {
            NEiflgvert = new Int_t[NEnvert];
            
            for (int i = 0; i < NEnvert; ++i) {
                NEiflgvert[i] = other.NEiflgvert[i];
            }
        }
        
        // Copy NEdirvert fixed array
        for (int i = 0; i < 300; ++i) {
            for (int j = 0; j < 3; ++j) {
                NEdirvert[i][j] = other.NEdirvert[i][j];
            }
        }
        
        // Allocate and copy NEnvcvert related arrays
        if (NEnvcvert > 0) {
            NEabspvert = new Float_t[NEnvcvert];
            NEabstpvert = new Float_t[NEnvcvert];
            NEipvert = new Int_t[NEnvcvert];
            NEiverti = new Int_t[NEnvcvert];
            NEivertf = new Int_t[NEnvcvert];
            
            for (int i = 0; i < NEnvcvert; ++i) {
                NEabspvert[i] = other.NEabspvert[i];
                NEabstpvert[i] = other.NEabstpvert[i];
                NEipvert[i] = other.NEipvert[i];
                NEiverti[i] = other.NEiverti[i];
                NEivertf[i] = other.NEivertf[i];
            }
        }
        
        // Allocate and copy NFnvert related arrays
        if (NFnvert > 0) {
            NFiflag = new Int_t[NFnvert];
            NFx = new Float_t[NFnvert];
            NFy = new Float_t[NFnvert];
            NFz = new Float_t[NFnvert];
            NFpx = new Float_t[NFnvert];
            NFpy = new Float_t[NFnvert];
            NFpz = new Float_t[NFnvert];
            NFe = new Float_t[NFnvert];
            NFfirststep = new Int_t[NFnvert];
            
            for (int i = 0; i < NFnvert; ++i) {
                NFiflag[i] = other.NFiflag[i];
                NFx[i] = other.NFx[i];
                NFy[i] = other.NFy[i];
                NFz[i] = other.NFz[i];
                NFpx[i] = other.NFpx[i];
                NFpy[i] = other.NFpy[i];
                NFpz[i] = other.NFpz[i];
                NFe[i] = other.NFe[i];
                NFfirststep[i] = other.NFfirststep[i];
            }
        }
        
        // Allocate and copy NFnstep related arrays
        if (NFnstep > 0) {
            NFecms2 = new Float_t[NFnstep];
            
            for (int i = 0; i < NFnstep; ++i) {
                NFecms2[i] = other.NFecms2[i];
            }
        }
    }

    NRooTrackerVtx::~NRooTrackerVtx() {
        // Clean up dynamically allocated memory
        if (EvtCode) delete EvtCode;
        if (GeomPath) delete GeomPath;
        if (GeneratorName) delete GeneratorName;
        if (OrigFileName) delete OrigFileName;
        if (OrigTreeName) delete OrigTreeName;
        
        // Clean up dynamically allocated arrays
        delete[] StdHepPdg;
        delete[] StdHepStatus;
        delete[] StdHepFd;
        delete[] StdHepLd;
        delete[] StdHepFm;
        delete[] StdHepLm;
        delete[] NEipvc;
        delete[] NEiorgvc;
        delete[] NEiflgvc;
        delete[] NEicrnvc;
        delete[] NEiflgvert;
        delete[] NEabspvert;
        delete[] NEabstpvert;
        delete[] NEipvert;
        delete[] NEiverti;
        delete[] NEivertf;
        delete[] NFiflag;
        delete[] NFx;
        delete[] NFy;
        delete[] NFz;
        delete[] NFpx;
        delete[] NFpy;
        delete[] NFpz;
        delete[] NFe;
        delete[] NFfirststep;
        delete[] NFecms2;
    }
}
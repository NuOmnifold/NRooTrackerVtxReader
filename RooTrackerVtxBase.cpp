#include "RooTrackerVtxBase.h"
ClassImp(ND::RooTrackerVtxBase)

namespace ND {
    RooTrackerVtxBase::RooTrackerVtxBase() {
        // Default constructor
    }

    RooTrackerVtxBase::RooTrackerVtxBase(const RooTrackerVtxBase& other) : TObject(other) {
        // Copy constructor
    }

    RooTrackerVtxBase::~RooTrackerVtxBase() {
        // Destructor
    }
}
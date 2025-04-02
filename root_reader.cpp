#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TObject.h"
#include "TClonesArray.h"
#include "TObjString.h"

// Include the necessary class definitions
#include "RooTrackerVtxBase.h"
#include "JNuBeamFlux.h"
#include "NRooTrackerVtx.h"

void processRootFile(const std::string& filename) {
    // Open the ROOT file
    TFile* file = TFile::Open(filename.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    // Get the NRooTrackerVtx tree
    TTree* tree = (TTree*)file->Get("NRooTrackerVtx");
    if (!tree) {
        std::cerr << "Tree 'NRooTrackerVtx' not found in file" << std::endl;
        file->Close();
        return;
    }
    
    // Set up the TClonesArray to hold the NRooTrackerVtx objects
    TClonesArray* nRooVtxs = new TClonesArray("ND::NRooTrackerVtx");
    int NRooVtx = 0;
    
    // Set branch addresses
    tree->SetBranchAddress("Vtx", &nRooVtxs);
    tree->SetBranchAddress("NVtx", &NRooVtx);
    
    // Get the number of entries
    Long64_t nEntries = tree->GetEntries();
    std::cout << "Total entries: " << nEntries << std::endl;
    
    // Ask user how many entries to show (default: all)
    Long64_t maxEntries = nEntries;
    std::cout << "How many entries do you want to process? (Enter a number or 'all', default: all): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty() && input != "all") {
        try {
            maxEntries = std::stol(input);
            if (maxEntries > nEntries) maxEntries = nEntries;
        } catch (...) {
            std::cout << "Invalid input. Processing all entries." << std::endl;
        }
    }
    
    // Loop over entries
    for (Long64_t entry = 0; entry < maxEntries; ++entry) {
        // Clear previous event data
        nRooVtxs->Clear();
        
        // Get entry
        tree->GetEntry(entry);
        
        std::cout << "Entry " << entry << " has " << NRooVtx << " vertices" << std::endl;
        
        // Loop over vertices in this entry
        for (int i = 0; i < NRooVtx; ++i) {
            ND::NRooTrackerVtx* vtx = (ND::NRooTrackerVtx*)nRooVtxs->At(i);
            if (!vtx) continue;
            
            // Print debug information about the vertex
            std::cout << "    EvtNum: " << vtx->EvtNum << std::endl;
            std::cout << "    EvtXSec: " << vtx->EvtXSec << std::endl;
            std::cout << "    Position: (" 
                      << vtx->EvtVtx[0] << ", " 
                      << vtx->EvtVtx[1] << ", " 
                      << vtx->EvtVtx[2] << ", " 
                      << vtx->EvtVtx[3] << ")" << std::endl;
            
            // Print neutrino flux information
            std::cout << "    Neutrino Parent PDG: " << vtx->NuParentPdg << std::endl;
            std::cout << "    Neutrino Energy: " << vtx->NuEnusk << " GeV" << std::endl;
            
            // Print raw memory address for debugging
            std::cout << "    StdHepPdg address: " << static_cast<void*>(vtx->StdHepPdg) << std::endl;
            if (vtx->StdHepN > 0 && vtx->StdHepPdg != nullptr) {
                std::cout << "    Raw StdHepPdg[0]: " << vtx->StdHepPdg[0] << std::endl;
            }
            std::cout << "    NEipvc address: " << static_cast<void*>(vtx->NEipvc) << std::endl;
            if (vtx->NEnvc > 0 && vtx->NEipvc != nullptr) {
                std::cout << "    Raw NEipvc[0]: " << vtx->NEipvc[0] << std::endl;
            }
            
            // Print information about all particles in the event
            std::cout << "    Number of particles: " << vtx->StdHepN << std::endl;
            std::cout << "    -------------------------------------------------------------------------------------------------" << std::endl;
            
            // Print any additional data available
            if (vtx->NEnvc > 0 && vtx->NEipvc != nullptr) {
                std::cout << "\n    NEip (NEipvc) Particle Codes:" << std::endl;
                std::cout << "    ---------------------------------" << std::endl;
                for (int j = 0; j < vtx->NEnvc && j < 10; ++j) {
                    std::cout << "    | " << j << " | PDG: " << vtx->NEipvc[j] << " |" << std::endl;
                }
                std::cout << "    ---------------------------------" << std::endl;
            }
            
            // Check if EvtCode contains useful info to identify particle types
            if (vtx->EvtCode) {
                std::string evtCodeStr = vtx->EvtCode->GetString().Data();
                std::cout << "    Event Code: " << evtCodeStr << std::endl;
                
                // Try to parse interaction type if possible
                if (evtCodeStr.find("NuMuCC") != std::string::npos) {
                    std::cout << "    Interaction Type: Muon Neutrino Charged Current" << std::endl;
                } else if (evtCodeStr.find("NuMuNC") != std::string::npos) {
                    std::cout << "    Interaction Type: Muon Neutrino Neutral Current" << std::endl;
                } else if (evtCodeStr.find("NuECC") != std::string::npos) {
                    std::cout << "    Interaction Type: Electron Neutrino Charged Current" << std::endl;
                } else if (evtCodeStr.find("NuENC") != std::string::npos) {
                    std::cout << "    Interaction Type: Electron Neutrino Neutral Current" << std::endl;
                }
            }
        }
        
        // Ask user if they want to continue to the next entry
        if (entry < nEntries - 1) {
            std::cout << "\nPress Enter to process next entry, or 'q' to quit: ";
            std::string input;
            std::getline(std::cin, input);
            if (input == "q" || input == "Q") {
                break;
            }
        }
    }
    
    // Clean up
    delete nRooVtxs;
    file->Close();
    delete file;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <root_file>" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    processRootFile(filename);
    
    return 0;
}
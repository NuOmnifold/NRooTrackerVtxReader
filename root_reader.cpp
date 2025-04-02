#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <map>
#include <set>

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

// Define neutrino PDG codes for easy reference
const std::map<int, std::string> PDG_MAP = {
    {12, "nu_e"},
    {-12, "anti_nu_e"},
    {14, "nu_mu"},
    {-14, "anti_nu_mu"},
    {16, "nu_tau"},
    {-16, "anti_nu_tau"},
    {2212, "proton"},
    {2112, "neutron"},
    {211, "pi+"},
    {-211, "pi-"},
    {111, "pi0"},
    {321, "K+"},
    {-321, "K-"},
    {13, "mu-"},
    {-13, "mu+"},
    {11, "e-"},
    {-11, "e+"}
};

std::string getPDGName(int pdgCode) {
    if (PDG_MAP.find(pdgCode) != PDG_MAP.end()) {
        return PDG_MAP.at(pdgCode);
    }
    return "PDG:" + std::to_string(pdgCode);
}

// Function to determine if particle is initial or final state
std::string getParticleState(int status) {
    // Specific to T2K NEUT generator status codes
    if (status == 0) return "Initial State";
    if (status == 1) return "Final State";
    return "Intermediate";
}

void processRootFile(const std::string& filename) {
    // Open the ROOT file
    TFile* file = TFile::Open(filename.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    // Get the NRooTrackerVtx tree
    TTree* nuTree = (TTree*)file->Get("NRooTrackerVtx");
    if (!nuTree) {
        std::cerr << "Tree 'NRooTrackerVtx' not found in file" << std::endl;
        file->Close();
        return;
    }
    
    // Get the evt tree
    TTree* evtTree = (TTree*)file->Get("evt");
    if (!evtTree) {
        std::cerr << "Tree 'evt' not found in file. Will process all NRooTrackerVtx entries." << std::endl;
    }
    
    // Set up to collect reconstructed event IDs
    std::set<int> reconstructedEventIDs;
    
    // If evt tree exists, get all event IDs that have reconstruction
    if (evtTree) {
        int eventID;
        evtTree->SetBranchAddress("EventID", &eventID);
        
        Long64_t nEvtEntries = evtTree->GetEntries();
        std::cout << "Found " << nEvtEntries << " entries in the evt tree (reconstructed events)" << std::endl;
        
        for (Long64_t i = 0; i < nEvtEntries; ++i) {
            evtTree->GetEntry(i);
            reconstructedEventIDs.insert(eventID);
        }
        
        std::cout << "Collected " << reconstructedEventIDs.size() << " unique reconstructed Event IDs" << std::endl;
    }
    
    // Set up the TClonesArray to hold the NRooTrackerVtx objects
    TClonesArray* nRooVtxs = new TClonesArray("ND::NRooTrackerVtx");
    int NRooVtx = 0;
    
    // Set branch addresses
    nuTree->SetBranchAddress("Vtx", &nRooVtxs);
    nuTree->SetBranchAddress("NVtx", &NRooVtx);
    
    // Get the number of entries
    Long64_t nEntries = nuTree->GetEntries();
    std::cout << "Total NRooTrackerVtx entries: " << nEntries << std::endl;
    
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
    
    // Filter only reconstructed events?
    bool filterReconstructed = false;
    if (evtTree) {
        std::cout << "Do you want to look only at events with reconstruction? (y/n, default: n): ";
        std::getline(std::cin, input);
        filterReconstructed = (input == "y" || input == "Y");
    }
    
    int processedEntries = 0;
    
    // Loop over entries
    for (Long64_t entry = 0; entry < nEntries && processedEntries < maxEntries; ++entry) {
        // Clear previous event data
        nRooVtxs->Clear();
        
        // Get entry
        nuTree->GetEntry(entry);
        
        // Loop over vertices in this entry
        for (int i = 0; i < NRooVtx; ++i) {
            ND::NRooTrackerVtx* vtx = (ND::NRooTrackerVtx*)nRooVtxs->At(i);
            if (!vtx) continue;
            
            // Check if this event is reconstructed (if filtering)
            if (filterReconstructed && reconstructedEventIDs.find(vtx->EvtNum) == reconstructedEventIDs.end()) {
                continue;
            }
            
            // We have a valid vertex to process
            processedEntries++;
            
            std::cout << "\n========== Entry " << entry << ", Vertex " << i << " ==========\n";
            std::cout << "Event Number: " << vtx->EvtNum << std::endl;
            if (filterReconstructed) {
                std::cout << "This event has reconstruction data in the evt tree" << std::endl;
            }
            
            // Print event information
            std::cout << "Event XSec: " << vtx->EvtXSec << " (1E-38 cm^2)" << std::endl;
            std::cout << "Event Weight: " << vtx->EvtWght << std::endl;
            std::cout << "Vertex Position (x,y,z,t): (" 
                      << vtx->EvtVtx[0] << ", " 
                      << vtx->EvtVtx[1] << ", " 
                      << vtx->EvtVtx[2] << ", " 
                      << vtx->EvtVtx[3] << ")" << std::endl;
            
            // Print neutrino flux information
            std::cout << "Neutrino Parent PDG: " << vtx->NuParentPdg << std::endl;
            std::cout << "Neutrino Energy: " << vtx->NuEnusk << " GeV" << std::endl;
            
            // Print event code if available
            if (vtx->EvtCode) {
                std::string evtCodeStr = vtx->EvtCode->GetString().Data();
                std::cout << "Event Code: " << evtCodeStr << std::endl;
                
                // Try to parse interaction type if possible
                if (evtCodeStr.find("NuMuCC") != std::string::npos) {
                    std::cout << "Interaction Type: Muon Neutrino Charged Current" << std::endl;
                } else if (evtCodeStr.find("NuMuNC") != std::string::npos) {
                    std::cout << "Interaction Type: Muon Neutrino Neutral Current" << std::endl;
                } else if (evtCodeStr.find("NuECC") != std::string::npos) {
                    std::cout << "Interaction Type: Electron Neutrino Charged Current" << std::endl;
                } else if (evtCodeStr.find("NuENC") != std::string::npos) {
                    std::cout << "Interaction Type: Electron Neutrino Neutral Current" << std::endl;
                }
            }
            
            // Print information about all particles in the event
            std::cout << "\nNumber of particles: " << vtx->StdHepN << std::endl;
            
            // First print initial state particles
            std::cout << "\n----- INITIAL STATE PARTICLES -----\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            std::cout << "| Idx |     PDG     |    Px    |    Py    |    Pz    |    E     |   Status   |\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            
            bool hasInitial = false;
            for (int j = 0; j < vtx->StdHepN; ++j) {
                // Check pointers
                if (vtx->StdHepPdg != nullptr && vtx->StdHepStatus != nullptr) {
                    // Print only initial state particles (status == 0)
                    if (vtx->StdHepStatus[j] == 0) {
                        hasInitial = true;
                        printf("| %3d | %11s | %9.3f | %9.3f | %9.3f | %9.3f | %-11s |\n", 
                               j, 
                               getPDGName(vtx->StdHepPdg[j]).c_str(), 
                               vtx->StdHepP4[j][0], 
                               vtx->StdHepP4[j][1], 
                               vtx->StdHepP4[j][2], 
                               vtx->StdHepP4[j][3],
                               getParticleState(vtx->StdHepStatus[j]).c_str());
                    }
                }
            }
            
            if (!hasInitial) {
                std::cout << "| No initial state particles found                                             |\n";
            }
            std::cout << "-------------------------------------------------------------------------------------\n";
            
            // Then print final state particles
            std::cout << "\n----- FINAL STATE PARTICLES -----\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            std::cout << "| Idx |     PDG     |    Px    |    Py    |    Pz    |    E     |   Status   |\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            
            bool hasFinal = false;
            for (int j = 0; j < vtx->StdHepN; ++j) {
                // Check pointers
                if (vtx->StdHepPdg != nullptr && vtx->StdHepStatus != nullptr) {
                    // Print only final state particles (status == 1)
                    if (vtx->StdHepStatus[j] == 1) {
                        hasFinal = true;
                        printf("| %3d | %11s | %9.3f | %9.3f | %9.3f | %9.3f | %-11s |\n", 
                               j, 
                               getPDGName(vtx->StdHepPdg[j]).c_str(), 
                               vtx->StdHepP4[j][0], 
                               vtx->StdHepP4[j][1], 
                               vtx->StdHepP4[j][2], 
                               vtx->StdHepP4[j][3],
                               getParticleState(vtx->StdHepStatus[j]).c_str());
                    }
                }
            }
            
            if (!hasFinal) {
                std::cout << "| No final state particles found                                                |\n";
            }
            std::cout << "-------------------------------------------------------------------------------------\n";
            
            // Print intermediate state particles if any
            bool hasIntermediate = false;
            for (int j = 0; j < vtx->StdHepN; ++j) {
                if (vtx->StdHepStatus != nullptr && vtx->StdHepStatus[j] != 0 && vtx->StdHepStatus[j] != 1) {
                    hasIntermediate = true;
                    break;
                }
            }
            
            if (hasIntermediate) {
                std::cout << "\n----- INTERMEDIATE PARTICLES -----\n";
                std::cout << "-------------------------------------------------------------------------------------\n";
                std::cout << "| Idx |     PDG     |    Px    |    Py    |    Pz    |    E     |   Status   |\n";
                std::cout << "-------------------------------------------------------------------------------------\n";
                
                for (int j = 0; j < vtx->StdHepN; ++j) {
                    // Check pointers
                    if (vtx->StdHepPdg != nullptr && vtx->StdHepStatus != nullptr) {
                        // Print intermediate particles (status != 0 and status != 1)
                        if (vtx->StdHepStatus[j] != 0 && vtx->StdHepStatus[j] != 1) {
                            printf("| %3d | %11s | %9.3f | %9.3f | %9.3f | %9.3f | %-11s |\n", 
                                   j, 
                                   getPDGName(vtx->StdHepPdg[j]).c_str(), 
                                   vtx->StdHepP4[j][0], 
                                   vtx->StdHepP4[j][1], 
                                   vtx->StdHepP4[j][2], 
                                   vtx->StdHepP4[j][3],
                                   getParticleState(vtx->StdHepStatus[j]).c_str());
                        }
                    }
                }
                std::cout << "-------------------------------------------------------------------------------------\n";
            }
            
            // Check if we've reached our limit
            if (processedEntries >= maxEntries) {
                break;
            }
        }
        
        // Check if we've reached our limit
        if (processedEntries >= maxEntries) {
            break;
        }
        
        // Ask user if they want to continue to the next entry
        if (entry < nEntries - 1 && processedEntries < maxEntries) {
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
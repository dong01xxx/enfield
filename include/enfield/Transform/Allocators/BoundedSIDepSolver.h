#ifndef __EFD_BOUNDED_SI_DEP_SOLVER_H__
#define __EFD_BOUNDED_SI_DEP_SOLVER_H__

#include "enfield/Transform/Allocators/DepSolverQAllocator.h"

#include <unordered_map>
#include <string>

struct MapResult;

namespace efd {
    /// \brief Implementation of DepSolverQAllocator that bounds the subgraph
    /// isomorphism search.
    class BoundedSIDepSolver : public DepSolverQAllocator {
        public:
            typedef BoundedSIDepSolver* Ref;
            typedef std::unique_ptr<BoundedSIDepSolver> uRef;

        protected:
            BoundedSIDepSolver(ArchGraph::sRef archGraph);
            Solution solve(DepsSet& deps) override;

        private:
            struct CandPair {
                Mapping m;
                uint32_t cost;
            };

            struct TKSResult {
                SwapSeq swaps;
                Mapping newCurrent;
                Mapping newLast;
            };

            typedef std::vector<CandPair> CandidatesTy;
            CandidatesTy extendCandidates(Dep& dep, std::vector<bool>& mapped,
                                          CandidatesTy& candidates);
            TKSResult process(Mapping& last, Mapping& current);
            uint32_t getNearest(uint32_t u, Assign& assign);

        public:
            /// \brief Create a new instance of this class.
            static uRef Create(ArchGraph::sRef archGraph);
    };
}

#endif

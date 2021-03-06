#ifndef __EFD_APPROX_TS_FINDER_H__
#define __EFD_APPROX_TS_FINDER_H__

#include "enfield/Support/TokenSwapFinder.h"

namespace efd {
    /// \brief 4-Approximative polynomial algorithm.
    ///
    /// Miltzow et al.
    /// DOI: 10.4230/LIPIcs.ESA.2016.66
    class ApproxTSFinder : public TokenSwapFinder {
        public:
            typedef ApproxTSFinder* Ref;
            typedef std::unique_ptr<ApproxTSFinder> uRef;

        private:
            typedef std::vector<uint32_t> GoodVertices;
            typedef std::vector<std::vector<GoodVertices>> GoodVerticesMatrix;
            GoodVerticesMatrix mMatrix;

        protected:
            void preprocess() override;
            SwapSeq findImpl(const InverseMap& from, const InverseMap& to) override;

        public:
            /// \brief Creates an instance of this class.
            static uRef Create();
    };
}

#endif

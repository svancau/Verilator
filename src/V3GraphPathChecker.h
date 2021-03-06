// -*- mode: C++; c-file-style: "cc-mode" -*-
//*************************************************************************
// DESCRIPTION: Verilator: DAG Path Checking
//
// Code available from: https://verilator.org
//
//*************************************************************************
//
// Copyright 2003-2019 by Wilson Snyder.  This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
//
// Verilator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//*************************************************************************

#ifndef _V3GRAPHPATHCHECKER_H_
#define _V3GRAPHPATHCHECKER_H_

#include "V3Error.h"
#include "V3Graph.h"
#include "V3GraphAlg.h"

//######################################################################

/// Implement pathExistsFrom() with some caching to prune the search.
/// Far more aggressive caching/pruning is possible; for now the use cases
/// don't rely so heavily on this class that it's necessary.
///
/// The graph (or at least, the subset the algorithm sees through
/// edgeFuncp) must not change during the lifetime of the checker.
class GraphPathChecker : GraphAlg<const V3Graph> {
    // Count "generations" which increases on operations that scan through
    // the graph. Each node is marked with the last generation that scanned
    // it, to enable asserting there are no cycles, and to avoid recursing
    // through the same node twice while searching for a path.
    vluint64_t m_generation;

 public:
    // CONSTRUCTORS
    GraphPathChecker(const V3Graph* graphp,
                     V3EdgeFuncP edgeFuncp = V3GraphEdge::followAlwaysTrue);
    ~GraphPathChecker();

    // METHODS
    bool pathExistsFrom(const V3GraphVertex* fromp, const V3GraphVertex* top);

    // If have edges A->B, B->C, and A->C then A->C is considered a
    // "transitive" edge (implied by A->B and B->C) and it could be safely
    // removed. Detect such an edge.
    bool isTransitiveEdge(const V3GraphEdge* edgep);

 private:
    bool pathExistsInternal(const V3GraphVertex* ap,
                            const V3GraphVertex* bp,
                            unsigned* costp = NULL);
    void initHalfCriticalPaths(GraphWay way, bool checkOnly);
    void incGeneration() { ++m_generation; }

    VL_DEBUG_FUNC;  // Declare debug()
    VL_UNCOPYABLE(GraphPathChecker);
};

#endif  // Guard

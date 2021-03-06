// -*- mode: C++; c-file-style: "cc-mode" -*-
//*************************************************************************
// DESCRIPTION: Verilator: Graph algorithm base class
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

#ifndef _V3GRAPHALG_H_
#define _V3GRAPHALG_H_ 1

#include "config_build.h"
#include "verilatedos.h"

#include "V3Global.h"
#include "V3Graph.h"

//=============================================================================
// Algorithms - common class
// For internal use, most graph algorithms use this as a base class

template <class T_Graph = V3Graph>  // Or sometimes const V3Graph
class GraphAlg {
protected:
    T_Graph* m_graphp;  // Graph we're operating upon
    V3EdgeFuncP m_edgeFuncp;  // Function that says we follow this edge
    // CONSTRUCTORS
    GraphAlg(T_Graph* graphp, V3EdgeFuncP edgeFuncp)
        : m_graphp(graphp), m_edgeFuncp(edgeFuncp) {}
    ~GraphAlg() {}
    // METHODS
    inline bool followEdge(V3GraphEdge* edgep) {
        return (edgep->weight() && (m_edgeFuncp)(edgep));
    }
};

//============================================================================

#endif  // Guard

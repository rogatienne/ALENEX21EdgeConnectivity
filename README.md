# ALENEX21EdgeConnectivity
Source code for Alenex 2021 submitted paper.

In this repository you can find the source code of algorithms used to compute, for a strongly connected graph G, its edge connectivity.  
The edge connectivity of a graph G is the minimum number of edges whose deletion leaves a graph that is not strongly connected.

In particular, there are nine distinct algorithms to compute the edge connectivity of a graph:
- MansourSchieber
- MansourSchieber2
- DinicEC
- FordFulkersonEC
- GabowEC_Parallel_DFS
- GabowEC_Parallel
- GabowEC_Serial_DFS
- GabowEC_Serial
- LocalSearchBasedEC

There are also some example graphs:
- rome99
- p2p-Gnutella25

Also, there is the code to augment the edge connectivity of a graph:
- IncreaseMinDegrees 

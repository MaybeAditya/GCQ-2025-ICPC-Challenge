# Problem: The Ouroboros Protocol (Temporal Synchronization)

| Metric | Details |
| :--- | :--- |
| **Difficulty** | World Finals|
| **Time Limit** | 8.0 seconds |
| **Memory Limit** | 1024 MB |
| **Topic** | Dynamic Graph Algorithms, Link-Cut Trees, Number Theory |

## Description
You are the Chief Architect of the "Ouroboros" distributed supercomputer, which consists of $N$ quantum nodes connected in a tree structure. The network is subject to relativistic time dilation.

Each node $u$ has a "temporal weight" $W_u$ and a "stability frequency" $F_u$.
The network is highly unstable. You must process $Q$ queries of two types:

1.  **Temporal Shift (Update):** The connection between node $u$ and node $v$ is severed, and a new connection is formed between node $x$ and node $y$. The graph is guaranteed to remain a forest (a collection of trees).
2.  **Synchronization Check (Query):** For a given path between node $u$ and node $v$, calculate the **"Resonance Sum"**.

### Mathematical Definition
The **Resonance Sum** $R(u, v)$ is defined as:
$$R(u, v) = \sum_{k \in \text{path}(u, v)} \left( \phi(W_k) \times \mu(F_k) \right) \mod (10^9 + 7)$$

However, the transmission is subject to noise. The actual value contributed by a node is the convolution of its neighbors' weights. Specifically, for every node on the path, you must effectively compute a **Fast Fourier Transform (FFT)** of the weights of its subtree in the auxiliary "virtual" tree structure to determine the final coefficient.

## Input Format
* The first line contains two integers $N$ and $Q$ ($1 \le N, Q \le 2 \cdot 10^5$).
* The second line contains $N$ integers representing the initial weights $W_u$.
* The next $N-1$ lines define the initial tree edges.
* The next $Q$ lines describe the operations (Cut/Link or Query).

## Constraints
* $1 \le N, Q \le 200,000$
* $1 \le W_u, F_u \le 10^6$
* **Critical Constraint:** The topology changes dynamically (edges are cut and added).

---
*Author: Global CodeQuest Technical Committee*

# ClusterTreeTopologyDataBased

Implementation of Topological Formation of Cluster-tree Networks based on Data Analysis detected by IoT sensor devices

This project is part of a mechanism for building topologies cluster-tree for Wireless Sensor Networks focused on IoT large-scale applications, such as smart agriculture, disaster prevention, and industrial automation. In large-scale monitoring applications, the nodes in a monitored region can process different data rates and this fact can damage the network performance and the energy consumption of devices.

To provide efficient communication, coverage, connectivity and energy, and network performance, topological formation is a key factor. In addition, the topology must be capable to lead with different scenarios, which often suffer from network modifications caused by external events, disassociation, inappropriate routes and paths between nodes and the base station, and many others.

For this, this scheme allows the network coordinator to form suitable clusters and paths based on the data analysis performed about the application data sensed. Thus, it is used a dataset with sensor data reads from a real-world scenario of the Intel Lab Data (Figure 1 below), collected during 15 days between March and April of 2004.

First, the dataset is divided into two scenarios, considering the reads at 10 am and 4 pm. In the following, it is performed the data analysis of each scenario, using Machine Learning algorithms such as Unsupervised and Supervised Algorithms. Thus, are evaluated the K-means, DBSCAN, Hierarchical Clustering, Fuzzy-C-means, and k-NN algorithms for clustering and classification nodes in priority groups. After this step, the topology is formed considering the priority nodes identified. 

The Data Analysis is described in the file named **"cluster_classific_approaches.pyinb"** and the Cluster-tree Topology Formation is described in the file named **"cluster-tree_topology_form.pyinb"**.

## Data Analysis
- _Using K-means clustering_
  - The K-means clustering is a unsupervised algorithm that identifies centroids based on the number of clusters defined.
- _Using DBSCAN_
  - The DBSCAN is a clustering algorithm that is based on Epsilon value for discover similar data points. 
- _Using Hierarchical clustering_
  - The Hierarchical clustering method builds a dendogram that allow to identify clusters with distinct glanuralities.
- _Using Fuzzy-C-means clustering_
  - The Fuzzy-C-means is a optimized method from K-means algorithm, that is capable to lead with outliers through the fuzzufication approach based in Sigma values.
- _Using k-NN classifier_
  - The k-Neares Neighbor algorithm is a supervised learning algorithm which can classify data from the training of subset, using a k number of neighbors data points.

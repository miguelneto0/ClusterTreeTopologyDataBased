# ClusterTreeTopologyDataBased

Implementation of Topological Formation of Cluster-tree Networks based on Data Analysis detected by IoT sensor devices

This project is part of a mechanism for building topologies cluster-tree for Wireless Sensor Networks focused on IoT large-scale applications, such as smart agriculture, disaster prevention, and industrial automation. In large-scale monitoring applications, the nodes in a monitored region can process different data rates and this fact can damage the network performance and the energy consumption of devices.

To provide efficient communication, coverage, connectivity and energy, and network performance, topological formation is a key factor. In addition, the topology must be capable to lead with different scenarios, which often suffer from network modifications caused by external events, disassociation, inappropriate routes and paths between nodes and the base station, and many others.

For this, this scheme allows the network coordinator to form suitable clusters and paths based on the data analysis performed about the application data sensed. Thus, it is used a dataset with sensor data reads from a real-world scenario of the [Intel Lab Data](http://db.csail.mit.edu/labdata/labdata.html) (Figure 1 below), collected during 15 days between March and April of 2004. This scenario comprises 54 nodes which sense temperature and humidity data periodically and then send them to a base station, here represented by the node 4.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/intellabdata_plot.png"  width="800">
<p align = "left"><sup>
Figure 1 (Intel Lab Data scenario and nodes plot)</sup></p>


First, the dataset is divided into two scenarios, considering the reads at 10 am and 4 pm. In the following, it is performed the data analysis of each scenario, using Machine Learning algorithms such as Unsupervised and Supervised Algorithms. Thus, are evaluated the **K-means**, **DBSCAN**, **Hierarchical Clustering**, **Fuzzy-C-means**, and **k-NN** algorithms for clustering and classification nodes in priority groups. After this step, the topology is formed considering the priority nodes identified. Figure 2 presents the Python code that extract the dataset with nodeID, dates, time, temperature and humidity and create the DataFrame using Pandas Library as shown below.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/dataframe_leitura.png"  height="400"> <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/dataframe_print.png"  height="320">
<p align = "left"><sup>Figure 2 and Figure 3</sup></p>

The Data Analysis is described in the file named **"cluster_classific_approaches.ipynb"** and the Cluster-tree Topology Formation is described in the file named **"cluster-tree_topology_form.ipynb"**.

## Data Analysis with Machine Learning Approaches
- _Using K-means clustering_
The K-means clustering is a unsupervised algorithm that identifies centroids based on the number of clusters defined.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/kmeans_part.png"  height="200"> <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/kmeans_part2.png"  height="200">
<p align = "left"><sup>Figure 4 (K-means code) and Figure 5 (labels)</sup></p>

From this code we run the K-means algorithm passing the number of cluster as 2. In this case, 2 center points (centroids) are identified by the algorithm, considering the temperature and humidity values, for then to group the data in priority node groups. The result of this grouping is presented below, as well as the amount of nodes in blue group and red group. The nodes group are printed below, resulting in a total of 51 nodes, because the IntelLabData problem and some (4) nodes during monitoring process, with K-means identifying 34 blue nodes and 17 red nodes.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/kmeans_nodegroups.png"  height="60">

And the data points are ploted as follows.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/kmeans_result_staticPlot.png"  height="190"> <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/kmeans_result_dynamicPlot.png"  height="190">
  <p align = "left"><sup>Figure 6 (K-means results)</sup></p>
  
  In the left side of Figure 6, are shown the sensed data divided in 2 groups, been blue with lower priority and orange, higher priority. Each group contain a centroids represented by the black star.
    
- _Using DBSCAN_
The DBSCAN is a clustering algorithm that is based on Epsilon value for discover similar data points.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/DBSCAN_part.png"  width="680">
  <p align = "left"><sup>Figure 8 (DBSCAN code)</sup></p>
  
  From this code, the node groups are formed as follows:
  
<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/DBSCAN_nodegroups.png"  height="60">

  
<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/DBSCAN_result_staticPlot.png"  height="190"> <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/DBSCAN_result_dynamicPlot.png"  height="190">

  
- _Using Hierarchical clustering_
  - The Hierarchical clustering method builds a dendogram that allow to identify clusters with distinct glanuralities.
- _Using Fuzzy-C-means clustering_
The Fuzzy-C-means is a optimized method from K-means algorithm, that is capable to lead with outliers through the fuzzufication approach based in Sigma values.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/fuzzycmeans_nodegroups.png"  height="60">


<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/fuzzycmeans_result_staticPlot.png"  height="200"> <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/fuzzycmeans_result_dynamicPlot.png"  height="200">


- _Using k-NN classifier_
The k-Neares Neighbor algorithm is a supervised learning algorithm which can classify data from the training of subset, using a k number of neighbors data points.

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/knn_part.png"  width="540">
  <p align = "left"><sup>Figure 6 (DBSCAN code)</sup></p>

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/knn_part.2png.png"  width="480">
  <p align = "left"><sup>Figure 7 (DBSCAN code)</sup></p>

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/knn_result_staticPlot.png"  height="200"> <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/knn_result_dynamicPlot.png"  height="200">

## Cluster-tree Topology Formation

Finally, after the data analysis phase, the resulting topology can be optimized and the performance and energy efficiency is improved. For each clustering and classification approach using the Machine Learning algorithms, we have the following structures and priority nodes (Figure 10-14).

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/kmeans10h_topology.png"  height="240">   <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/DBSCAN&hierarc10h_topology.png"  height="240"> 
<p align = "left"><sup>Figure 10 (Resulting topology with K-means) and Figure 11 (Resulting topology with DBSCAN/Hierarc)</sup></p>

<img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/fuzzycmeans10h_topology.png"  height="240"> <img src="https://github.com/miguelneto0/ClusterTreeTopologyDataBased/blob/main/images/knn10h_topology.png"  height="240">
<p align = "left"><sup>Figure 12 (Resulting topology with Fuzzy-C-Means) and Figure 13 (Resulting topology with KNN)</sup></p>

# Visualization And Operations On Graphs
A project that uses OpenGL to Visualize Graphs and Perform Operations on them using a GUI.

### Prerequisites
OpenGL Library Installation on Ubuntu 16.04,
```apt-get install freeglut3-dev```

### Example

A Directed Graph is represented in the required format in "DirectedGraph.txt" and an Undirected Graph is represented in the required format in "UndirectedGraph.txt". Each node on the graph will have a different colour for better Visualization. 


* To Compile, Run make.
* To Execute, ```./Prg.out < DirectedGraph.txt```

![Image 1](https://github.com/Kadle11/Visualization_And_Operations_On_Graphs/blob/master/ScreenShots/BasicGraph.png)
![Image 2](https://github.com/Kadle11/Visualization_And_Operations_On_Graphs/blob/master/ScreenShots/Legend.png)


### Instructions

* To help with Visualization, The user can select any node and move it to any point on the canvas.
* The Legend Gives you information about all the functions provided

The Functionalities Provided are,
* Node Insertion
* Node Deletion
* Edge Insertion
* Edge Deletion
* Djikstra's Algorithm to find Shortest Path between any two selected Nodes
* Chromatic Number of the Graph

To Insert Node, Click on the Insert Node Button. <br />
To Delete Node, Click on the Delete Node Button and Select Node to Delete.<br />
To Insert Edge, Click on the Insert Edge Button and Select the 2 Nodes.<br />
To Delete Edge, Click on the Delete Edge Button and Select Edge. <br />

# FTNetSim

This project, **FTNetSim**, is a wide area network simulator for file transfer service modeling and simulation.
The network simulation is built on NS-3 V3.31.
The default wide area network is the U.S. DOE Energy Sciences Network (https://www.es.net). 
But users can customized theri topology, by editing the wan/esnet.h.
Currently, all are hard coded, any customization has to be made via changing the code. 
We will enable configuration file in the future version.

The current file transfer model is a basic model with concurrency and parallelism feature. 
Please refer to [this article](https://doi.org/10.1016/j.future.2018.05.051) for explaination of concurrency and parallelism.

Note: the simulation will be slow when you simulate very high throughput, this is because the network simulation is an event-driven one, 
high throughput leads to large volume of events need to be simulated/processed. 
Currently, NS-3 doesnot provide an easy way to parallize it, 
and there actually is not a straightforward way to make descrete event simulation scalable to supercomputer. 

This work is supported by the U.S. DOE [RAMSES project](https://ramsesproject.github.io). 

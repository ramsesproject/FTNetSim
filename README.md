# FTNetSim

This project, **FTNetSim**, is a wide area **Net**work **Sim**ulator for **F**ile **T**ransfer service modeling and simulation.
The network simulation is built on NS-3 V3.31.
The default wide area network is the U.S. DOE Energy Sciences Network (https://www.es.net). 
Users can customize their topology, by editing the wan/esnet.h.
Currently, all configurations are hard-coded, any customization has to be made via changing the code. 
We will enable configuration file in the future version.

The current file transfer model is a basic model with concurrency and parallelism feature. 
Please refer to [this article](https://doi.org/10.1016/j.future.2018.05.051) for explaination of concurrency and parallelism.

Note: the simulation will be slow when you simulate very high throughput, this is because the network simulation is an event-driven one, 
high throughput leads to large volume of events to be simulated/processed. 
Currently, NS-3 doesnot provide an easy way to parallelize it, 
and there actually is not a straightforward way to make descrete event simulation scalable on supercomputer because of the high coupled events. 
Another file transfer simulation built upon a more coarse-grained (not packet level as it in FTNetSim) network simulator is available [here](https://github.com/ramsesproject/superfacility)

This work is supported by the U.S. DOE [RAMSES project](https://ramsesproject.github.io). 

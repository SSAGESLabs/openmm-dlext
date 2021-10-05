FROM innocentbug/pysages-hoomd:latest

#OpenMM dlext plugin
RUN git clone https://github.com/SSAGESLabs/openmm-dlext.git && cd openmm-dlext && mkdir build && cd build && cmake .. && make install



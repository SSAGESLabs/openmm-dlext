FROM innocentbug/pysages-hoomd:latest

COPY . openmm-dlext
RUN cd openmm-dlext && mkdir build && cd build && cmake .. && make install



FROM ssages/pysages-hoomd:latest
WORKDIR /openmm-dlex/.docker_build

COPY . ../
RUN cmake .. && make install
RUN python -c "import openmm_dlext"

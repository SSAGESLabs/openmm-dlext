FROM ssages/pysages-hoomd:latest
WORKDIR /openmm-dlext

# Build the plugin
COPY . .
RUN cmake -S . -B build && cmake --build build --target install && rm -rf build

# Test it can be loaded
RUN python -c "import openmm.dlext"

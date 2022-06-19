FROM silkeh/clang:12

# Common settings
ARG PROJECT_NAME="default_project"
ENV PROJECT_NAME=${PROJECT_NAME}
WORKDIR /tmp/${PROJECT_NAME}

# Project specific
RUN apt-get -qq update && \
  apt-get install -qqy --no-install-recommends \
    lcov llvm ccache \
    doxygen graphviz

RUN echo "cache_dir = /tmp/${PROJECT_NAME}/ccache" > /etc/ccache.conf

CMD rm -rf /tmp/build && mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_BUILD_TYPE=Debug /tmp/${PROJECT_NAME} && make && make check

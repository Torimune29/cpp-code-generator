#!/bin/bash

set -eu

PROJECT_NAME="project"
PROJECT_IMAGE_NAME="${PROJECT_NAME}-env"

if [ "$1" = "build" ]; then
  docker build --build-arg PROJECT_NAME=${PROJECT_NAME} -t ${PROJECT_IMAGE_NAME}:latest .
elif [ "$1" = "start" ]; then
  docker stop ${PROJECT_NAME} && true
  docker run -it -d --rm=true --name ${PROJECT_NAME} \
    -u $(id -u):$(id -g) \
    --mount type=bind,source="$(pwd)",target=/tmp/${PROJECT_NAME} \
    --mount type=bind,source=/etc/group,target=/etc/group,readonly \
    --mount type=bind,source=/etc/passwd,target=/etc/passed,readonly \
    ${PROJECT_IMAGE_NAME}:latest /bin/bash
elif [ "$1" = "run" ]; then
  docker run --rm=true --name ${PROJECT_NAME} \
    -u $(id -u):$(id -g) \
    --mount type=bind,source="$(pwd)",target=/tmp/${PROJECT_NAME} \
    --mount type=bind,source=/etc/group,target=/etc/group,readonly \
    --mount type=bind,source=/etc/passwd,target=/etc/passed,readonly \
    ${PROJECT_IMAGE_NAME}:latest
elif [ "$1" = "cov" ]; then
  docker rm --force ${PROJECT_NAME} && true
  docker run --name ${PROJECT_NAME} \
    -u $(id -u):$(id -g) \
    --mount type=bind,source="$(pwd)",target=/tmp/${PROJECT_NAME} \
    --mount type=bind,source=/etc/group,target=/etc/group,readonly \
    --mount type=bind,source=/etc/passwd,target=/etc/passed,readonly \
    ${PROJECT_IMAGE_NAME}:latest /bin/bash -c "rm -rf /tmp/build && mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_BUILD_TYPE=Coverage /tmp/\${PROJECT_NAME} && make && make check && make coverage"
  docker cp ${PROJECT_NAME}:/tmp/build/coverage_out.info .
  docker rm --force  ${PROJECT_NAME} && true
elif [ "$1" = "doc" ]; then
  docker rm --force ${PROJECT_NAME} && true
  docker run --name ${PROJECT_NAME} \
    -u $(id -u):$(id -g) \
    --mount type=bind,source="$(pwd)",target=/tmp/${PROJECT_NAME} \
    --mount type=bind,source=/etc/group,target=/etc/group,readonly \
    --mount type=bind,source=/etc/passwd,target=/etc/passed,readonly \
    ${PROJECT_IMAGE_NAME}:latest /bin/bash -c "rm -rf /tmp/build && mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_BUILD_TYPE=Release /tmp/\${PROJECT_NAME} && make doc"
  docker cp ${PROJECT_NAME}:/tmp/build/docs .
  docker rm --force  ${PROJECT_NAME} && true
elif [ "$1" = "notice" ]; then
  docker rm --force ${PROJECT_NAME} && true
  docker run --name ${PROJECT_NAME} \
    -u $(id -u):$(id -g) \
    --mount type=bind,source="$(pwd)",target=/tmp/${PROJECT_NAME} \
    --mount type=bind,source=/etc/group,target=/etc/group,readonly \
    --mount type=bind,source=/etc/passwd,target=/etc/passed,readonly \
    ${PROJECT_IMAGE_NAME}:latest /bin/bash -c "rm -rf /tmp/build && mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_BUILD_TYPE=Release /tmp/\${PROJECT_NAME} && make notice"
  docker cp ${PROJECT_NAME}:/tmp/build/NOTICE .
  docker rm --force  ${PROJECT_NAME} && true
else
  printf "%s" "input error."
  exit 1
fi
exit 0
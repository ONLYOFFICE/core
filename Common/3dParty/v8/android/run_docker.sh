#!/bin/sh
for container in $(docker ps -aq); do
    echo deleting container: $container
    docker rm $container
done

for image in $(docker images --format "{{.ID}}"); do
    echo  deleting $image
    docker rmi -f $image
done

docker rm v8docker
#add --progress=plain for full logs from build.sh
docker build . -t v8_docker --build-arg CACHEBUST=0 --no-cache=true --platform linux/amd64
docker run -d --name v8docker v8_docker:latest
docker cp v8docker:/home/docker/v8/build.zip .
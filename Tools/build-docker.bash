
absolute_path="$( cd -- "$(dirname -- "$0")"; pwd )"

docker run -v $absolute_path/../:/project snaccoverflow/build-zp:latest

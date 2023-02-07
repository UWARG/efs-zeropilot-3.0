if [ $# = 0 ]; then 
    echo "please use either the build, pull or test argument"
    exit
fi

if [ $1 == 'pull' ]; then 
    echo "pulling docker images"
    docker pull snaccoverflow/build-zp:latest
    docker pull snaccoverflow/test-zp:latest
    exit
fi

absolute_path="$( cd -- "$(dirname -- "$0")"; pwd )"

if [ $1 = 'build' ]; then
    BOARD=""
    if [ $# =  2 ]; then
        BOARD=$2
    else
        BOARD="nucleol552zeq"
    fi
    docker run --rm -it -v $absolute_path/../:/project snaccoverflow/build-zp:latest -p $BOARD
else
        docker run --rm -it -v $absolute_path/../:/project snaccoverflow/test-zp:latest
fi
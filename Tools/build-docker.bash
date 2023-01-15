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

docker run -v $absolute_path/../:/project snaccoverflow/$1-zp:latest

if [ $# = 0 ]; then 
    echo "please use either the build or test argument"
    exit
fi

absolute_path="$( cd -- "$(dirname -- "$0")"; pwd )"

docker run -v $absolute_path/../:/project snaccoverflow/$1-zp:latest

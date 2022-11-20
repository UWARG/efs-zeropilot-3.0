script_path=$(dirname $0)

g++ -o TM_test $script_path/../Src/* -I Inc ; echo "running now"; ./TM_test; rm TM_test
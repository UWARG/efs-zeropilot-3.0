script_path=$(dirname $0)

if [ $1 = "clean" ]; then 
    echo "cleaning messages"
    rm -r $script_path/../TelemetryManager/Inc/messages
else
    echo "building messages"
#    lcm-gen --cpp --cpp-hpath $script_path/../TelemetryManager/Inc/ $script_path/../TelemetryManager/Inc/Msgs/.
    for FILE in $script_path/../TelemetryManager/Inc/Msgs/*; do
        echo "building hpp for $FILE"
        lcm-gen --cpp --cpp-hpath $script_path/../TelemetryManager/Inc/ $FILE
    done   
fi
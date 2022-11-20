script_path=$(dirname $0)

rm $script_path/../Inc/Messages.hpp

echo "#pragma once\n\n" >> $script_path/../Inc/Messages.hpp

for FILE in $script_path/../Inc/messages/*; do
    echo "including $FILE"
    echo "#include \"messages/$(basename $FILE)\"" >> $script_path/../Inc/Messages.hpp
done   

echo "\n" >> $script_path/../Inc/Messages.hpp
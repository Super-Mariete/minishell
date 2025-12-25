#!/bin/bash

# Colors
GREEN="\e[32m"
RED="\e[31m"
BLUE="\e[34m"
RESET="\e[0m"

ft_get_output()
{
	local	output
	local	expected_output
	local	i=1
	while IFS= read -r output && IFS= read -r expected_output <&3; do
        # if [ "$output" != "$expected_output" ]; then
        #     echo "----------- Error en línea $i -----------"
            echo -e "Output real:      ${BLUE}'$output'${RESET}"
            echo -e "Output esperado:  '$expected_output'"
        # fi
        ((i++))
    done <<< "$1" 3<<< "$2"
}

MIS_DATOS="Línea uno
Línea dos con espacios
Línea tres"

MIS_DATOS2="Línea uno
Línea dos DIFERENTE
Línea tres"

ft_get_output "$MIS_DATOS" "$MIS_DATOS2"
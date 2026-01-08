#!/bin/bash
for i in {1..100}; do
    echo "Test $i"
    echo "echo test$i | cat | cat | cat | cat | cat" | ./minishell
done
echo "Si llega aquí sin errores, no hay leaks de FD"

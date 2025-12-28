#!/bin/bash

i=0
vars=150
while [ $i -le $vars ]; do
	export "FOO$i=1"
	((i++))
done

# ../minishell
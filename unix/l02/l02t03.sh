for path in $@
do
#	echo "$(cd $(dirname "$path"); pwd)/$(basename "$path")"
	echo "`cd \`dirname "$path"\`; pwd`/`basename "$path"`"
done

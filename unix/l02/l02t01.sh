
for filename in $@
do
	if expr "$filename" : ".*~$" > /dev/null
	then
		echo "Skipping $filename (name ending with ~)."
		continue
	elif test -e "$filename~"
	then
		echo "Old version of $filename found, renaming."
		mv "$filename~" "${filename%.*}.old"
	else
		echo "Old version of $filename not found."
	fi
done

# To samo w wersji `while`:
# 
# while [ $# -gt 0 ]
# do
# 	filename=$1
# 	if expr "$filename" : ".*~$" > /dev/null
# 	then
# 		echo "Skipping $filename (name ending with ~)."
# 		continue
# 	elif test -e "$filename~"
# 	then
# 		echo "Old version of $filename found, renaming."
# 		mv "$filename~" "${filename%.*}.old"
# 	else
# 		echo "Old version of $filename not found."
# 	fi
# 	shift
# done
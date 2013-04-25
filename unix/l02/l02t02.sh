
process()
{
	# read -p "Do you want to process $1?" < /dev/tty > /dev/tty
	echo -n "Do you want to process $1? " > /dev/tty
	read answer < /dev/tty > /dev/tty
	if [ "$answer" = "y" ] || [ "$answer" = "yes" ]
	then
		echo $1
	fi
}

if [ $# -gt 1 ]
then
	for filename in $@
	do
		process $filename
	done
else
	while read filename
	do
		process $filename
	done
fi
{
	printf $0
}

(first[$3] && last[$3] && (first[$3] != $1 || last[$3] != $2)) {
	printf " <- kolizja!"
}

{
	first[$3] = $1
	last[$3] = $2
	printf "\n"
}

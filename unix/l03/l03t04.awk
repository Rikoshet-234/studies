{
	total += 1
    num[$3] = $3
	first[$3] = $1
	last[$3] = $2
	count[$3] += 1
	sum[$3] += $4
}

END {
	for (i in num) {
		printf "%s,%s,%d,%d,%d,%0.2f\n", first[i], last[i], i, count[i], total, sum[i]
	}
}

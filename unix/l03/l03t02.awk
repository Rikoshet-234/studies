{
	rep = false
	for (i = 1; i <= NF; i++) {
		rep = rep || ($i == prev)
		prev = $i
	}

	if (rep) print $0
}
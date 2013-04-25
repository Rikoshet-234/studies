# Baza studentów:
# indeks | nazwisko

# Baza kursów:
# id | nazwa

# Baza ocen:
# indeks | id | ocena

# Dodajemy studenciaka do bazy.
STUDENTS="db/students.db"
COURSES="db/courses.db"
GRADES="db/grades.db"

add_student()
{
	echo "$1|$2" >> "$STUDENTS"
	test "$3" = "-i" || exit 0
}

# Dodejemy kurs do bazy.
add_course()
{
	echo "$1|$2" >> "$COURSES"
	test "$3" = "-i" || exit 0
}

# Dodajemy ocenę do bazy.
add_grade()
{
	echo "$1|$2|$3" >> "$GRADES"
	test "$4" = "-i" || exit 0
}

# Wyszukujemy ocenę w bazie.
find_grade()
{
	grade=`nawk -F '|' "$1 == \\$1 && $2 == \\$2 { print \\$3 }" < "$GRADES"`
	
	# Jeżeli nie znalazło to wyświetl komunikat o błędzie.
	if [ -z "$grade" ]
	then
		echo "Grade for student $1 and course $2 not found."
		test "$3" = "-i" || exit 1
	fi

	echo "$grade"
	test "$3" = "-i" || exit 0
}

# Kasujemy ocenę z bazy.
delete_grade()
{
	# Potrzebne bo system nie pozwala robić we/wy do jednego pliku.
	tmp=`mktemp`
	grep -v "$1|$2|*" < "$GRADES" > "$tmp"
	mv "$tmp" "$GRADES"
	test "$3" = "-i" || exit 0
}

# Modyfikujemy ocenę w bazie.
modify_grade()
{
	delete_grade "$1" "$2" -i
	add_grade "$1" "$2" "$3" -i
	test "$3" = "-i" || exit 0
}

# Tryb interaktywny.
interactive()
{
	echo "Choose option: "
	echo "1 - add student"
	echo "2 - add course"
	echo "3 - add grade"
	echo "4 - find grade"
	echo "5 - delete grade"
	echo "6 - modify grade"
	echo "7 - exit interactive mode"
	read option

	case "$option" in
	"1")
		echo "Insert index number: "
		read index
		echo "Insert student name: "
		read name
		add_student "$index" "$name" -i
		;;
	"2")
		echo "Insert course id number: "
		read id
		echo "Insert course name: "
		read name
		add_course "$id" "$name" -i
		;;
	"3")
		echo "Insert student index number: "
		read index
		echo "Insert course id number: "
		read id
		echo "Insert grade value: "
		read grade
		add_grade "$index" "$id" "$grade" -i
		;;
	"4")
		echo "Insert student index number: "
		read index
		echo "Insert course id number: "
		read id
		find_grade "$index" "$id" -i
		;;
	"5")
		echo "Insert student index number: "
		read index
		echo "Insert course id number: "
		read id
		delete_grade "$index" "$id" -i
		;;
	"6")
		echo "Insert student index number: "
		read index
		echo "Insert course id number: "
		read id
		echo "Insert new grade value: "
		read grade
		modify_grade "$index" "$id" "$grade" -i
		;;
	"7"|"exit")
		echo "Bye."
		exit 0
		;;
	*)
		echo "Invalid option: $option."
		;;
	esac

	# Odpal jeszcze raz tryb interaktywny.
	interactive
}

case "$1" in
"-s"|"add-student")
	add_student "$2" "$3"
	;;
"-k"|"add-course")
	add_course "$2" "$3"
	;;
"-a"|"add-grade")
	add_grade "$2" "$3" "$4"
	;;
"-f"|"find-grade")
	find_grade "$2" "$3"
	;;
"-u"|"delete-grade")
	delete_grade "$2" "$3"
	;;
"-m"|"modify-grade")
	modify_grade "$2" "$3" "$4"
	;;
""|"-i"|"interactive")
	interactive
	;;
*)
	echo "Unknown option: $1."
	exit 1
	;;
esac
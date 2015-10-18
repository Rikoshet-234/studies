
datatype color = Red | Black
datatype 'a rbnode = Leaf | Branch of color * bool * 'a * 'a rbnode * 'a rbnode
type 'a rbtree = int * int * 'a rbnode

fun node (c, x, a, b) = Branch (c, true, x, a, b)

val redLeaf = node (Red, (), Leaf, Leaf)

fun perfectTree 0 = Leaf
  | perfectTree 2 = node (Black, (), redLeaf, Leaf)
  | perfectTree 3 = node (Black, (), redLeaf, redLeaf)
  | perfectTree n =
    let val n' = n - 1
        val a = perfectTree (n' div 2 + n' mod 2)
        val b = perfectTree (n' div 2)
    in node (Black, (), a, b) end

fun relabel (xs, t) =
    let fun aux (x :: xs, Branch (c, v, _, Leaf, b)) =
            let val (b', xs') = aux (xs, b)
            in (Branch (c, v, x, Leaf, b'), xs') end
          | aux (xs, Branch (c, v, _, a, b))         =
            let val (a', x' :: xs')  = aux (xs, a)
                val (b', xs'')       = aux (xs', b)
            in (Branch (c, v, x', a', b'), xs'') end
          | aux (xs, Leaf)                           =
            (Leaf, xs)
    in #1 (aux (xs, t)) end

fun fromList xs =
    let val l = length xs
    in (l, l, relabel (xs, perfectTree (length xs))) end

fun member (e, (_, _, t)) =
    let fun aux Leaf                     = false
          | aux (Branch (_, v, x, a, b)) =
            if e < x
            then aux a
            else if e > x
                then aux b
                else v
    in aux t end

fun insert (e, (m, n, t)) =
    let fun aux Leaf                     = node (Black, e, Leaf, Leaf)
          | aux (Branch (c, v, x, a, b)) =
            if e < x
            then Branch (c, v, x, aux a, b)
            else if e > x
                 then Branch (c, v, x, a, aux b)
                 else Branch (c, true, e, a, b)
    in (m + 1, n + 1, t) end

fun flatten (_ : int, _ : int, t) =
    let fun aux Leaf                     = []
          | aux (Branch (_, v, x, a, b)) =
            aux a @ (if v then [x] else []) @ aux b
    in aux t end

fun delete (e, mnt as (m, n, t)) =
    let fun aux Leaf = Leaf
          | aux (Branch (c, v, x, a, b)) =
            if e < x
            then Branch (c, v, x, aux a, b)
            else if e > x
                 then Branch (c, v, x, a, aux b)
                 else Branch (c, false, x, a, b)
        fun check (mnt as (m, n, _)) =
            if 2 * m < n
            then fromList (flatten mnt)
            else mnt
    in
        if member (e, mnt)
        then check (m - 1, n, aux t)
        else mnt
    end

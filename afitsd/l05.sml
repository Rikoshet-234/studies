
signature DEQUE =
sig
    type 'a deque

    val empty : 'a deque
    val isEmpty : 'a deque -> bool
    val cons : 'a -> 'a deque -> 'a deque
    val snoc : 'a -> 'a deque -> 'a deque
    val head : 'a deque -> 'a
    val daeh : 'a deque -> 'a
    val tail : 'a deque -> 'a deque
    val liat : 'a deque -> 'a deque
end

structure Deque : DEQUE =
struct
    exception EmptyQeueue
    type 'a deque = 'a list * 'a list

    val empty = ([], [])

    fun isEmpty ([], []) = true
      | isEmpty _        = false

    fun cons x (fs, bs) = (fs, x :: bs)
    fun snoc x (fs, bs) = (x :: fs, bs)

    fun decons ([], [])      = raise EmptyQeueue
      | decons (f :: fs, bs) = (f, (fs, bs))

    fun desnoc ([], [])      = raise EmptyQeueue
      | desnoc (fs, b :: bs) = (b, (fs, bs))

    fun head q = #1 (decons q)
    fun daeh q = #1 (desnoc q)
    fun tail q = #2 (decons q)
    fun liat q = #2 (desnoc q)
end

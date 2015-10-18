exception EmptyQueue

datatype 'a state = Reversing of int * 'a list * 'a list * 'a list * 'a list
                  | Appending of int * 'a list * 'a list
                  | Done of 'a list
                  | Idle

type 'a hmqueue = int * 'a list * 'a state * 'a list

val empty = (0, [], Idle, [])

fun rotate (fs, rs) = Reversing (0, fs, [], rs, [])

fun exec (Reversing (k, f :: fs, fs', r :: rs, rs')) =
    Reversing (k + 1, fs, f :: fs', rs, r :: rs')
  | exec (Reversing (k, [], fs', [r], rs'))          =
    Appending (k, fs', r :: rs')
  | exec (Appending (0, _, rs'))                     =
    Done rs'
  | exec (Appending (k, f :: fs', rs'))              =
    Appending (k - 1, fs', f :: rs')
  | exec s                                           = s

fun exec2 (d, fs, s, rs) =
    case exec (exec s) of Done fs' => (~d, fs', Idle, rs)
                        | s'       => (d, fs, s', rs)

fun pop (Reversing (k, fs, fs', rs, rs')) =
    Reversing (k - 1, fs, fs', rs, rs')
  | pop (Appending (0, fs', _ :: rs'))    =
    Done rs'
  | pop (Appending (k, fs', rs'))         =
    Appending (k - 1, fs', rs')
  | pop s                                 = s

fun check (q as (d, fs, Idle, rs)) =
    if d >= 0
    then exec2 q
    else exec2 (d, fs, rotate (fs, rs), [])
  | check q                        =
    exec2 q

fun snoc (x, (d, fs, s, rs)) =
    check (d - 1, fs, s, x :: rs)

fun head (_, f :: _, _, _) = f
  | head (_, [], _, _)  = raise EmptyQueue

fun tail (d, _ :: fs, s, rs) =
    check (d - 1, fs, pop s, rs)
  | tail (_, [], _, _)    = raise EmptyQueue

fun fromList xs = foldl snoc empty xs

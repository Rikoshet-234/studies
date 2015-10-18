exception EmptyQueue

datatype 'a state = Reversing of int * 'a list * 'a list * 'a list * 'a list
                  | Appending of int * 'a list * 'a list
                  | Done of 'a list
                  | Idle

type 'a hmqueue = int * 'a list * 'a state * int * 'a list

val empty = (0, [], Idle, 0, [])

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

fun exec2 (lfs, fs, s, lrs, rs) =
    case exec (exec s) of Done fs' => (lfs, fs', Idle, lrs, rs)
                        | s'       => (lfs, fs, s', lrs, rs)

fun pop (Reversing (k, fs, fs', rs, rs')) =
    Reversing (k - 1, fs, fs', rs, rs')
  | pop (Appending (0, fs', _ :: rs'))    =
    Done rs'
  | pop (Appending (k, fs', rs'))         =
    Appending (k - 1, fs', rs')
  | pop s                                 = s

fun check (q as (lfs, fs, s, lrs, rs)) =
    if lrs <= lfs
    then exec2 q
    else exec2 (lfs + lrs, fs, rotate (fs, rs), 0, [])

fun snoc (x, (lfs, fs, s, lrs, rs)) =
    check (lfs, fs, s, lrs + 1, x :: rs)

fun head (lfs, f :: fs, s, lrs, rs) = f
  | head (_, [], _, _, _)           = raise EmptyQueue

fun tail (lfs, _ :: fs, s, lrs, rs) =
    check (lfs - 1, fs, pop s, lrs, rs)
  | tail (_, [], _, _, _)           = raise EmptyQueue

fun fromList xs = foldl snoc empty xs

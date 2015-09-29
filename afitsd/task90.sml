exception NotFound

signature EQ =
sig
  type T
  val eq : T * T -> bool
end

signature FINITE_MAP = 
sig
  structure Key : EQ
  type 'a Map

  val empty : 'a Map
  val isEmpty : 'a Map -> bool
  val bind : Key.T * 'a * 'a Map -> 'a Map
  val lookup : Key.T * 'a Map -> 'a
end

functor Trie (M : FINITE_MAP) : FINITE_MAP =
struct
  structure Key =
  struct
    type T = M.Key.T list
    fun eq ([], []) = true
      | eq (x :: xs, y :: ys) = M.Key.eq (x, y) andalso eq (xs, ys)
      | eq _ = false
  end

  datatype 'a Map = Trie of M.Key.T list * 'a option * 'a Map M.Map

  val empty = Trie ([], NONE, M.empty)
  fun isEmpty (Trie ([], None, m)) = M.isEmpty m
    | isEmpty _                  = false

  fun lookup ([], Trie ([], SOME x, _))          = x
    | lookup ((k :: ks), Trie ([], NONE, m))     =
      lookup (ks, M.lookup (k, m))
    | lookup ((k :: ks), Trie (k' :: ks', v, m)) =
      if M.Key.eq (k, k')
      then lookup (ks, Trie (ks', v, m))
      else raise NotFound
    | lookup _                                   = raise NotFound

  fun bind (ks, x, Trie ([], NONE, m)) =
      (Trie (ks, SOME x, m))
    | bind (k :: ks, x, Trie (k' :: ks', v, m)) =
      if M.Key.eq (k, k')
      then let val (Trie (ks'', v', m')) = bind (ks, x, Trie (ks', v, m))
           in Trie (k :: ks'', v', m') end
      else (* TODO *) Trie ([], NONE, m)
end

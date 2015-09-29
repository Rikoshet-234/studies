exception NotFound

signature FINITE_MAP = 
sig
  type Key
  type 'a Map

  val empty : 'a Map
  val bind : Key * 'a * 'a Map -> 'a Map
  val lookup : Key * 'a Map -> 'a
end

functor Trie (M : FINITE_MAP) : FINITE_MAP =
struct
  type Key = M.Key list
  datatype 'a Map = Entry of 'a | Trie of 'a Map M.Map

  val empty = Trie M.empty

  fun lookup ([], Entry x)       = x
    | lookup ((k :: ks), Trie m) = lookup (ks, M.lookup (k, m))
    | lookup _                   = raise NotFound

  fun bind ([], x, _)           = Entry x
    | bind (k :: ks, x, Trie m) =
      let val m' = M.lookup (k, m) handle NotFound => empty
      in Trie (M.bind (k, bind (ks, x, m'), m)) end
end
